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
// Library   : Collection
// Unit      : Concrete collections
// File      : BasicSortedAVL.cpp
// Description :
//   Implementation of concrete sorted collections based on AVLs
//   without any hierarchy requires on elements.
//

#include "Collection/ConcreteCollection/BasicSortedAVL.h"
// #include "Collection/Collection.hpp"

namespace COL {}

#include "Pointer/Binary.template"
#include "Collection/VirtualCollection/VirtualCollection.template"
#include "Collection/ConcreteCollection/SortedAVL.template"

namespace COL {

#include "Collection/ConcreteCollection/BasicSortedAVL.incc"

#define DefTypeBinaryParent
#include "Collection/ConcreteCollection/BasicSortedAVL.incc"
#undef DefTypeBinaryParent

} // end of namespace COL

template COL::VirtualSortedCollection::LocationResult COL::GenericAVL::tlocate<COL::BasicGenericAVL::BasicTemplateLocate>(COL::BasicGenericAVL::BasicTemplateLocate) const;
template COL::VirtualSortedCollection::LocationResult COL::GenericParentAVL::tlocate<COL::BasicGenericParentAVL::BasicTemplateLocate>(COL::BasicGenericParentAVL::BasicTemplateLocate) const;
