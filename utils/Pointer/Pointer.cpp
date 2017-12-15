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
// Unit      : Basic pointer
// File      : Pointer.cpp
// Description :
//   Implementation of the class Pointer.
//

#include "Pointer/Pointer.hpp"
// #include "Pointer/Pointer.h"

namespace PNT {

void
Pointer::fullAssign(const Pointer& source, TypePointer type) {
   if (type == TPStandard)
     peoElement = source.peoElement;
   else if (peoElement != source.peoElement) {
      release(true);
      if (type == TPAuto) {
         if (source.peoElement)
            peoElement = source.peoElement->createCopy();
      }
      else { // type == TPPass
         if ((peoElement = source.peoElement) != nullptr)
            const_cast<Pointer&>(source).peoElement = nullptr;
      };
   };
}

void
Pointer::fullAssign(EnhancedObject* element, TypePointer type, Init) {
   if ((type != TPStandard) && (peoElement != element))
      release(true);
   peoElement = element;
}

} // end of namespace PNT
