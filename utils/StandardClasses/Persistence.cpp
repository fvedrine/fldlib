/**************************************************************************/
/*                                                                        */
/*  This file is part of FLDLib                                           */
/*                                                                        */
/*  Copyright (C) 2014-2017                                               */
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
// Library   : Standard classes
// Unit      : Basic object
// File      : Persistence.cpp
// Description :
//   Implementation of basic material for persistence. Each object that needs serialization
//   is likely to inherits for STG::IOObject.
//

#include "StandardClasses/Persistence.h"

namespace STG {

#if DefineDebugLevel > 1
EReadError::EReadError(const char* message)
   :  uPosition(0), szMessage(message) {}
EReadError::EReadError(int position, const char* message)
   :  uPosition(position), szMessage(message) {}
#endif

#if DefineDebugLevel > 1
EWriteError::EWriteError(const char* message)
   :  szMessage(message) {}
#endif

/* Implementation of the class IOObject */
   /* class ISStream */

#define DefineImplementsIStreamMethods
#define DefineTypeObject IOObject::ISStream
#define DefineLinkStream (*pisIn)
#include "StandardClasses/IOStreamMethods.incc"
#undef DefineLinkStream
#undef DefineTypeObject
#undef DefineImplementsIStreamMethods

   /* class OSStream */

#define DefineImplementsOStreamMethods
#define DefineTypeObject IOObject::OSStream
#define DefineLinkStream (*posOut)
#include "StandardClasses/IOStreamMethods.incc"
#undef DefineLinkStream
#undef DefineTypeObject
#undef DefineImplementsOStreamMethods

} // end of namespace STG

