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
// File      : BasicSortedAVL.h
// Description :
//   Definition of concrete sorted collections based on AVLs
//   without any hierarchy requires on elements.
//

#ifndef COL_BasicSortedAVLH
#define COL_BasicSortedAVLH

#include "Collection/ConcreteCollection/SortedAVL.h"

namespace COL {

/******************************************/
/* Definition of the class BasicSortedAVL */
/******************************************/

#include "Collection/ConcreteCollection/BasicSortedAVL.inch"
#define DefTypeBinaryParent
#include "Collection/ConcreteCollection/BasicSortedAVL.inch"
#undef DefTypeBinaryParent

} // end of namespace COL

#endif
