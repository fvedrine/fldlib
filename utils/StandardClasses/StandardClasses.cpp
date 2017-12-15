/**************************************************************************/
/*                                                                        */
/*  This file is part of FLDLib                                           */
/*                                                                        */
/*  Copyright (C) 2013-2017                                               */
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
// File      : StandardClasses.cpp
// Description :
//   Implementation of the class EnhancedObject, inherited by any object whose
//   derivations are open.
//

#include "StandardClasses/StandardClasses.hpp"

#if DefineDebugLevel > 1
ESPreconditionError::ESPreconditionError(const char *textSource, int lineSource, const char* fileSource)
   :  uLine(lineSource), szFile(fileSource), szText(textSource) {}
#endif


#if DefineDebugLevel == 2

int EnhancedObject::uCountInstances = 0;

#elif DefineDebugLevel > 2

const char* EnhancedObject::pcReadFileSource = nullptr;
unsigned int EnhancedObject::uReadLineSource = 0;

int EnhancedObject::uCountInstances = 0;
EnhancedObject* EnhancedObject::peoFirst = nullptr;
EnhancedObject* EnhancedObject::peoMultipleRemoved = nullptr;

void
EnhancedObject::writeMessages(std::ostream& out) {
   if (peoFirst != nullptr) {
      EnhancedObject* cursor = peoFirst;
      do {
         out << "memory leak of " << cursor->pcFileSource << ", "
            << cursor->uLineSource << std::endl;
      } while ((cursor = cursor->peoNext) != peoFirst);
   };
   if (peoMultipleRemoved != nullptr) {
      EnhancedObject* cursor = peoMultipleRemoved;
      do {
         out << "multiple memory destruction of " << cursor->pcFileSource << ", " << cursor->uLineSource << std::endl;
      } while ((cursor = cursor->peoNext) != peoMultipleRemoved);
   };
}

namespace {

class BasicObject {
  public:
   virtual ~BasicObject() {}
};

}

int
EnhancedObject::queryAllocatedSize() {
   int uResult = 0;
   if (peoFirst != nullptr) {
      EnhancedObject* peoCursor = peoFirst;
      do {
         uResult += peoCursor->queryDebugSize() - sizeof(EnhancedObject) + sizeof(BasicObject);
      } while ((peoCursor = peoCursor->peoNext) != peoFirst);
   };
   return uResult;
}

#endif

