/**************************************************************************/
/*                                                                        */
/*  This file is part of FLDLib                                           */
/*                                                                        */
/*  Copyright (C) 2015-2017                                               */
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
// Library   : NumericalDomains
// Unit      : FloatInterval
// File      : FloatIntervalTypes.cpp
// Description :
//   Implementation of a class of floating point intervals
//

#include "NumericalAnalysis/FloatIntervalExecutionPath.h"

#include "ForwardNumerics/BaseInteger.template"
#include "ForwardNumerics/BaseFloating.template"
#include "ForwardNumerics/BaseTaylorFloating.template"

namespace NumericalDomains { namespace DDoubleInterval {

Numerics::DDouble::Access::ReadParameters
BaseExecutionPath::rpMinParams;

Numerics::DDouble::Access::ReadParameters
BaseExecutionPath::rpMaxParams;

}} // end of namespace NumericalDomains::DDoubleInterval

template class Numerics::DInteger::TBigCellInt<Numerics::DInteger::TCellIntegerTraits<(81+8*sizeof(unsigned)-1)/(8*sizeof(unsigned))> >;

template class Numerics::TBuiltDouble<NumericalDomains::DDoubleInterval::BuiltDoubleTraits<81, 24, 8> >;
template class Numerics::TBuiltDouble<NumericalDomains::DDoubleInterval::BuiltDoubleTraits<81, 53, 11> >;
template class Numerics::TBuiltDouble<NumericalDomains::DDoubleInterval::BuiltDoubleTraits<81, 81, 15> >;

template class Numerics::TReadDouble<
   Numerics::TBuiltDouble<NumericalDomains::DDoubleInterval::BuiltDoubleTraits<81, 24, 8> >,
   Numerics::TBuiltDouble<NumericalDomains::DDoubleInterval::BuiltDoubleTraits<81, 29, 8> > >;
template class Numerics::TReadDouble<
   Numerics::TBuiltDouble<NumericalDomains::DDoubleInterval::BuiltDoubleTraits<81, 53, 11> >,
   Numerics::TBuiltDouble<NumericalDomains::DDoubleInterval::BuiltDoubleTraits<81, 58, 11> > >;
template class Numerics::TReadDouble<
   Numerics::TBuiltDouble<NumericalDomains::DDoubleInterval::BuiltDoubleTraits<81, 81, 15> >,
   Numerics::TBuiltDouble<NumericalDomains::DDoubleInterval::BuiltDoubleTraits<81, 86, 15> > >;

template class NumericalDomains::DDoubleInterval::TBuiltFloat<81, 23, 8>;
template class NumericalDomains::DDoubleInterval::TBuiltFloat<81, 52, 11>;
template class NumericalDomains::DDoubleInterval::TBuiltFloat<81, 80, 15>;

