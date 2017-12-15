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
// File      : Array.cpp
// Description :
//   Implementation of concrete classes based on arrays.
//

// #include "Collection/Collection.hpp"
#include "Collection/ConcreteCollection/Array.h"

namespace COL {

#define DefTypeVirtualInheritedCollection VirtualCollection
#define DefTypeVirtualInheritedCursor VirtualCollectionCursor
#define DefTypeCollection GenericArray
#define DefTypeCursor GenericArrayCursor
#define DefTypeFinal
#include "Collection/ConcreteCollection/Array.incc"
#undef DefTypeFinal
#undef DefTypeCollection
#undef DefTypeCursor
#undef DefTypeVirtualInheritedCollection
#undef DefTypeVirtualInheritedCursor

} // end of namespace COL

