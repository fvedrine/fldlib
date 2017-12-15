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
// File      : Tree.cpp
// Description :
//   Implementation of the class GenericTree. It is a tree class where each father
//     decides of the type of collection for its sons.
//

#include "Collection/ConcreteCollection/Tree.h"
// #include "Collection/Collection.hpp"

namespace COL {}

#include "Collection/Implementation/ImplCommonTree.template"
#include "Collection/ConcreteCollection/TreeCommon.template"
// #include "Collection/Collection.template"

namespace COL {

template class TGenericTree<GenericTreeElement, GenericImplTreeCursor, GenericImplTree>;
template class TGenericTreeCursor<GenericTreeElement, GenericImplTreeCursor, GenericImplTree>;

} // end of namespace COL

template class COL::TImplTree<COL::GenericTreeElement, COL::GenericImplTreeCursor>;
