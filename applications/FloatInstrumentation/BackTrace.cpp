/**************************************************************************/
/*                                                                        */
/*  This file is part of FLDLib                                           */
/*                                                                        */
/*  Copyright (C) 2017-2017                                               */
/*    CEA (Commissariat a l'Energie Atomique et aux Energies              */
/*         Alternatives)                                                  */
/*                                                                        */
/*  you can redistribute it and/or modify it under the terms of the GNU   */
/*  Lesser General Public License as published by the Free Software       */
/*  Foundation, version 2.1.                                              */
/*                                                                        */
/*  It is distributed in the hope that it will be useful,                 */
/*  but WITHOUT ANY WARRANTY; without even the implied warranty of        */
/*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         */
/*  GNU Lesser General Public License for more details.                   */
/*                                                                        */
/*  See the GNU Lesser General Public License version 2.1                 */
/*  for more details (enclosed in the file LICENSE).                      */
/*                                                                        */
/**************************************************************************/

/////////////////////////////////
//
// Library   : NumericalDomains
// Unit      : BackTrace
// File      : BackTrace.cpp
// Description :
//   Implementation of a function to write the backtrace.
//

#include "FloatInstrumentation/BackTrace.h"
#include "config.h"

#if defined(__GNUC__) && (__GNUC__ >= 3) && defined(HAVE_CXXABI)
#include <execinfo.h>
// #include <dlfcn.h>
#include <cxxabi.h>
#endif

namespace NumericalDomains {

void
writeBackTrace(STG::IOObject::OSBase& out) { // code from unisim
#if defined(__GNUC__) && (__GNUC__ >= 3) && defined(HAVE_CXXABI)
   static const unsigned int max_depth = 32;

   void **stack_addrs = (void **) malloc((max_depth + 1) * sizeof(void *));
   int stack_depth = backtrace(stack_addrs, max_depth + 1);
   stack_addrs = (void **) realloc(stack_addrs, stack_depth * sizeof(void *));

   char **stack_strings = backtrace_symbols(stack_addrs, stack_depth);

   for (int level = 1; level < stack_depth; level++) {
      char *begin_of_mangled_name = 0;
      char *end_of_mangled_name = 0;
      char *begin_of_file_location = stack_strings[level];
      char *end_of_file_location = 0;
      char *begin_of_offset = 0;
      char *end_of_offset = 0;
      char *begin_of_address = 0;
      char *end_of_address = 0;

      // find the parentheses and address offset surrounding the mangled name
      char *pos = begin_of_file_location;
      while(*pos) {
         if (*pos == '(') {
            *pos = 0;
            end_of_file_location = pos++;
            begin_of_mangled_name = pos;
            break;
         }
         pos++;
      }

      while(*pos) {
         if (*pos == '+') {
            *pos = 0;
            end_of_mangled_name = pos++;
            begin_of_offset = pos;
            break;
         }
         else if (*pos == ')') {
            *pos = 0;
            begin_of_mangled_name = 0;
            end_of_offset = pos++;
            break;
         }
         pos++;
      }

      while(*pos) {
         if (*pos == '[') {
            pos++;
            begin_of_address = pos;
            break;
         }
         pos++;
      }

      while(*pos) {
         if (*pos == ']') {
            *pos = 0;
            end_of_address = pos;
            break;
         }
         pos++;
      }

      out.put('#');
      out << level << "  ";

/*    if (!begin_of_mangled_name && begin_of_address) {
         size_t address = 0;
         pos = begin_of_address;
         bool success = true;
         if (*pos == '0' && (pos[1] == 'x' || pos[1] == 'X')) {
            pos += 2;
            while (*pos) {
               address *= 16;
               if (*pos >= '0' && *pos <= '9')
                  address += (*pos - '0');
               else if (*pos >= 'a' && *pos <= 'f')
                  address += (10 + *pos - 'a');
               else if (*pos >= 'A' && *pos <= 'F')
                  address += (10 + *pos - 'a');
               else {
                  success = false;
                  break;
               };
               pos++;
            };
         }
         else
            success = false;
         if (success) {
            void* addressPtr = nullptr;
            memcpy(&addressPtr, &address, sizeof(void*));
            Dl_info info;
            if (dladdr(addressPtr, &info)) {
               begin_of_mangled_name = const_cast<char*>(info.dli_sname);
               end_of_mangled_name = begin_of_mangled_name + strlen(info.dli_sname);
            }
         };
      }; */
      if (begin_of_address && end_of_address)
         out << begin_of_address;

      if (begin_of_mangled_name && end_of_mangled_name > begin_of_mangled_name) {
         out << " in ";

         size_t length = end_of_mangled_name-begin_of_mangled_name; // initial length
         char *output = 0;
         int status;
         char *demangled_name = abi::__cxa_demangle(begin_of_mangled_name, output, &length, &status);
         // meaning of status:
         // * 0: The demangling operation succeeded.
         // * -1: A memory allocation failure occurred.
         // * -2: mangled_name is not a valid name under the C++ ABI mangling rules.
         // * -3: One of the arguments is invalid.
         if (status == 0 && demangled_name)
            out << demangled_name;
         else {
            STG::TChunk<char> chunk;
            chunk.string = begin_of_mangled_name;
            chunk.length = end_of_mangled_name-begin_of_mangled_name;
            out.writechunk((void*) &chunk);
         }
         if (status != -1) free(output);
         if (demangled_name) free(demangled_name);
      }

      if (begin_of_offset && end_of_offset) {
         out.put('+');
         out << begin_of_offset;
      }

      if (begin_of_file_location && end_of_file_location)
         out << " from " << begin_of_file_location;
      out.put('\n');
   }
   free(stack_strings); // malloc()ed by backtrace_symbols
	if(stack_addrs) free(stack_addrs);
#endif
}

void
readBackTrace(STG::IOObject::ISBase& in) { // code from unisim
#if defined(__GNUC__) && (__GNUC__ >= 3) && defined(HAVE_CXXABI)
   static const unsigned int max_depth = 32;

   void **stack_addrs = (void **) malloc((max_depth + 1) * sizeof(void *));
   int stack_depth = backtrace(stack_addrs, max_depth + 1);
   stack_addrs = (void **) realloc(stack_addrs, stack_depth * sizeof(void *));

   char **stack_strings = backtrace_symbols(stack_addrs, stack_depth);

   for (int level = 1; level < stack_depth; level++) {
      char *begin_of_mangled_name = 0;
      char *end_of_mangled_name = 0;
      char *begin_of_file_location = stack_strings[level];
      char *end_of_file_location = 0;
      char *begin_of_offset = 0;
      char *end_of_offset = 0;
      char *begin_of_address = 0;
      char *end_of_address = 0;

      // find the parentheses and address offset surrounding the mangled name
      char *pos = begin_of_file_location;
      while(*pos) {
         if (*pos == '(') {
            *pos = 0;
            end_of_file_location = pos++;
            begin_of_mangled_name = pos;
            break;
         }
         pos++;
      }

      while(*pos) {
         if (*pos == '+') {
            *pos = 0;
            end_of_mangled_name = pos++;
            begin_of_offset = pos;
            break;
         }
         else if (*pos == ')') {
            *pos = 0;
            begin_of_mangled_name = 0;
            end_of_offset = pos++;
            break;
         }
         pos++;
      }

      while(*pos) {
         if (*pos == '[') {
            pos++;
            begin_of_address = pos;
            break;
         }
         pos++;
      }

      while(*pos) {
         if (*pos == ']') {
            *pos = 0;
            end_of_address = pos;
            break;
         }
         pos++;
      }

      in.assume('#');
      int readLevel=0;
      in >> readLevel;
      if (level != readLevel)
         throw STG::EReadError();
      in.assume("  ");

/*    if (!begin_of_mangled_name && begin_of_address) {
         size_t address = 0;
         pos = begin_of_address;
         bool success = true;
         if (*pos == '0' && (pos[1] == 'x' || pos[1] == 'X')) {
            pos += 2;
            while (*pos) {
               address *= 16;
               if (*pos >= '0' && *pos <= '9')
                  address += (*pos - '0');
               else if (*pos >= 'a' && *pos <= 'f')
                  address += (10 + *pos - 'a');
               else if (*pos >= 'A' && *pos <= 'F')
                  address += (10 + *pos - 'a');
               else {
                  success = false;
                  break;
               };
               pos++;
            };
         }
         else
            success = false;
         if (success) {
            void* addressPtr = nullptr;
            memcpy(&addressPtr, &address, sizeof(void*));
            Dl_info info;
            if (dladdr(addressPtr, &info)) {
               begin_of_mangled_name = const_cast<char*>(info.dli_sname);
               end_of_mangled_name = begin_of_mangled_name + strlen(info.dli_sname);
            }
         };
      }; */
      if (begin_of_address && end_of_address)
         in.assume(begin_of_address);

      if (begin_of_mangled_name && end_of_mangled_name > begin_of_mangled_name) {
         in.assume(" in ");

         size_t length = end_of_mangled_name-begin_of_mangled_name; // initial length
         char *output = 0;
         int status;
         char *demangled_name = abi::__cxa_demangle(begin_of_mangled_name, output, &length, &status);
         // meaning of status:
         // * 0: The demangling operation succeeded.
         // * -1: A memory allocation failure occurred.
         // * -2: mangled_name is not a valid name under the C++ ABI mangling rules.
         // * -3: One of the arguments is invalid.
         if (status == 0 && demangled_name)
            in.assume(demangled_name);
         else {
            STG::TChunk<char> chunk;
            chunk.string = begin_of_mangled_name;
            chunk.length = end_of_mangled_name-begin_of_mangled_name;
            in.assume((void*) &chunk);
         }
         if (status != -1) free(output);
         if (demangled_name) free(demangled_name);
      }

      if (begin_of_offset && end_of_offset) {
         in.assume('+');
         in.assume(begin_of_offset);
      }

      if (begin_of_file_location && end_of_file_location) {
         in.assume(" from ");
         in.assume(begin_of_file_location);
      };
      in.assume('\n');
   }
   free(stack_strings); // malloc()ed by backtrace_symbols
	if(stack_addrs) free(stack_addrs);
#endif
}

} // end of namespace NumericalDomains


