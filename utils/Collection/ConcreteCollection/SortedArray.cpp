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
// File      : SortedArray.cpp
// Description :
//   Implementation of concrete collections based on sorted arrays.
//

#include "Collection/ConcreteCollection/SortedArray.h"
// #include "Collection/Collection.hpp"
#include "Pointer/ImplArray.template"

namespace COL {

#define DefTypeVirtualInheritedCollection VirtualSortedCollection
#define DefTypeVirtualInheritedCursor VirtualSortedCollectionCursor
#define DefTypeCollection GenericSortedArray
#define DefTypeCursor GenericSortedArrayCursor
#define DefTypeSorted
#define DefTypeFinal
#include "Collection/ConcreteCollection/Array.incc"
#undef DefTypeFinal
#undef DefTypeSorted
#undef DefTypeCollection
#undef DefTypeCursor
#undef DefTypeVirtualInheritedCollection
#undef DefTypeVirtualInheritedCursor

} // end of namespace COL

template int COL::ImplArray::merge<TemplateElementKeyCastParameters<EnhancedObject, COL::VirtualSortedCollection::SimpleKey, SimpleCast> >(TemplateElementKeyCastParameters<EnhancedObject, COL::VirtualSortedCollection::SimpleKey, SimpleCast> const&, COL::ImplArray const&, int, int, bool, VirtualCast const*);
template int COL::ImplArray::localize<COL::VirtualSortedCollection::TemplateElementEnhancedKeyCastParameters<EnhancedObject, COL::VirtualSortedCollection::EnhancedKey, SimpleCast> >(COL::VirtualSortedCollection::TemplateElementEnhancedKeyCastParameters<EnhancedObject, COL::VirtualSortedCollection::EnhancedKey, SimpleCast>::Key::KeyType, COL::VirtualSortedCollection::TemplateElementEnhancedKeyCastParameters<EnhancedObject, COL::VirtualSortedCollection::EnhancedKey, SimpleCast> const&, int, int) const;

