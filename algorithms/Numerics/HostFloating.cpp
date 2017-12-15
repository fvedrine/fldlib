/**************************************************************************/
/*                                                                        */
/*  This file is part of FLDLib                                           */
/*                                                                        */
/*  Copyright (C) 2005-2017                                               */
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
// Library   : Numerics
// Unit      : Floating
// File      : Floating.cpp
// Description :
//   Implementation of a class of floating points with unbound size.
//

#include "Numerics/HostFloating.h"
// #include "Numerics/Numerics.hpp"

namespace Numerics {}

#include "Numerics/Integer.template"
#include "Numerics/Floating.template"
// #include "Numerics/Numerics.template"

template class Numerics::TDoubleElement<Numerics::TFloatingBase<Numerics::DoubleTraits> >;
template class Numerics::TDoubleElement<Numerics::TFloatingBase<Numerics::FloatTraits> >;
template class Numerics::TDoubleElement<Numerics::TFloatingBase<Numerics::LongDoubleTraits> >;

