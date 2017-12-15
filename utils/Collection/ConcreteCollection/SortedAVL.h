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
// File      : SortedAVL.h
// Description :
//   Definition of AVLs as concrete sorted collections.
//

#ifndef COL_SortedAVLH
#define COL_SortedAVLH

#include "Collection/VirtualCollection/VirtualSortedCollection.h"
#include "Collection/Implementation/BinaryParent.h"

namespace COL {

/* Definition of the class GenericAVL */

#include "Collection/ConcreteCollection/SortedAVL.inch"

/* Definition of the class GenericParentAVL */

#define DefTypeBinaryParent
#include "Collection/ConcreteCollection/SortedAVL.inch"
#undef DefTypeBinaryParent

} // end of namespace COL

#endif
