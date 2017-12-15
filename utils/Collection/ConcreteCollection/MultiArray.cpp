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
// File      : MultiArray.cpp
// Description :
//   Implementation of multi-sets based on sorted arrays.
//

#include "Collection/ConcreteCollection/MultiArray.h"
// #include "Collection/Collection.hpp"

namespace COL {}

#include "Pointer/ImplArray.template"

namespace COL {

#define DefTypeVirtualInheritedCollection VirtualMultiSortedCollection
#define DefTypeVirtualInheritedCursor VirtualMultiSortedCollectionCursor
#define DefTypeCollection GenericMultiArray
#define DefTypeCursor GenericMultiArrayCursor
#define DefTypeSorted
#define DefTypeMultiSorted
#define DefTypeFinal
#include "Collection/ConcreteCollection/Array.incc"
#undef DefTypeFinal
#undef DefTypeMultiSorted
#undef DefTypeSorted
#undef DefTypeCollection
#undef DefTypeCursor
#undef DefTypeVirtualInheritedCollection
#undef DefTypeVirtualInheritedCursor

} // end of namespace COL

