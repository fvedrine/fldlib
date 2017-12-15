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
// Library   : Pointer
// Unit      : Basic pointers
// File      : MngPointer.cpp
// Description :
//   Implementation of the template MngPointer to manage the sharing of objects
//   through simple reference counters.
//

#include "Pointer/Pointer.hpp"
// #include "Pointer/MngPointer.h"

namespace PNT {

MngPointer&
MngPointer::operator=(const MngPointer& source) {
   EnhancedObject::operator=(source);
   if (this != &source) {
      MngElement* oldElement = nullptr;
      bool doesFree = false;
      if (Pointer::isValid()) {
         oldElement = &getElement();
         doesFree = oldElement->unlock();
         Pointer::release();
      };
      try {
         if (source.Pointer::isValid()) {
            Pointer::setElement(&source.Pointer::getElement());
            getElement().lock();
         };
      }
      catch (...) {
         Pointer::setElement(oldElement);
         throw;
      };
      if (doesFree && oldElement) delete oldElement;
   };
   return *this;
}

void
MngPointer::assign(MngElement* newElement, Init) {
   AssumeAllocation(newElement)
   if ((!Pointer::isValid()) || (&getElement() != newElement)) {
      MngElement* oldElement = nullptr;
      bool doesFree = false;
      if (Pointer::isValid()) {
         oldElement = &getElement();
         doesFree = oldElement->unlock();
      };
      try {
         Pointer::setElement(newElement);
         newElement->lock();
      }
      catch (...) {
         Pointer::setElement(oldElement);
         throw;
      };
      if (doesFree && oldElement) delete oldElement;
   };
}

void
MngPointer::fullAssign(const MngPointer& source) {
   EnhancedObject::operator=(source);
   if (this != &source) {
      MngElement* oldElement = nullptr;
      bool doesFree = false;
      if (Pointer::isValid()) {
         oldElement = &getElement();
         doesFree = oldElement->unlock();
         Pointer::release();
      };
      try {
         if (source.Pointer::isValid()) {
            Pointer::setElement(source.Pointer::getElement().createCopy());
            getElement().lock();
         };
      }
      catch (...) {
         Pointer::setElement(oldElement);
         throw;
      };
      if (doesFree && oldElement) delete oldElement;
   };
}

} // end of namespace PNT

