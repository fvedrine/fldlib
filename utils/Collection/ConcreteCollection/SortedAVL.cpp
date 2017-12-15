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
// File      : SortedAVL.cpp
// Description :
//   Implementation of AVLs as concrete sorted collections.
//

#include "Collection/ConcreteCollection/SortedAVL.h"
// #include "Collection/Collection.hpp"

namespace COL {}

#include "Collection/ConcreteCollection/SortedAVL.template"

namespace COL {

#include "Collection/ConcreteCollection/SortedAVL.incc"

#define DefTypeBinaryParent
#include "Collection/ConcreteCollection/SortedAVL.incc"
#undef DefTypeBinaryParent

template GenericAVL::LocationResult
GenericAVL::tlocate<GenericAVL::GenericTemplateLocate>(GenericTemplateLocate locate) const;

template GenericParentAVL::LocationResult
GenericParentAVL::tlocate<GenericParentAVL::GenericTemplateLocate>(GenericTemplateLocate locate) const;

} // end of namespace COL

