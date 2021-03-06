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
// Unit      : FloatExact
// File      : FloatExactBase.cpp
// Description :
//   Implementation of a class of comparison between floating points
//

#include "NumericalAnalysis/FloatExactExecutionPath.h"

namespace NumericalDomains { namespace DDoubleExact {

Numerics::DDouble::Access::ReadParameters
BaseExecutionPath::rpNearestParams;

}}

#if !defined(FLOAT_GENERIC_BASE_UNSIGNED) && !defined(FLOAT_GENERIC_BASE_LONG)
#include "NumericalLattices/FloatExactBaseTypes.template"

typedef NumericalDomains::DDoubleExact::FloatDigitsHelper::TFloatDigits<long double> LongDoubleFloatDigits;

template class Numerics::TBuiltDouble<NumericalDomains::DDoubleExact::BuiltDoubleTraits<FLOAT_REAL_BITS_NUMBER, 23, 8> >;
template class Numerics::TBuiltDouble<NumericalDomains::DDoubleExact::BuiltDoubleTraits<FLOAT_REAL_BITS_NUMBER, 52, 11> >;
template class Numerics::TBuiltDouble<NumericalDomains::DDoubleExact::BuiltDoubleTraits<FLOAT_REAL_BITS_NUMBER, LongDoubleFloatDigits::UBitSizeMantissa, LongDoubleFloatDigits::UBitSizeExponent> >;
template class Numerics::TBuiltDouble<NumericalDomains::DDoubleExact::BuiltDoubleTraits<FLOAT_REAL_BITS_NUMBER, FLOAT_REAL_BITS_NUMBER, LongDoubleFloatDigits::UBitSizeExponent> >;

template class Numerics::TReadDouble<
   Numerics::TBuiltDouble<NumericalDomains::DDoubleExact::BuiltDoubleTraits<FLOAT_REAL_BITS_NUMBER, 23, 8> >,
   Numerics::TBuiltDouble<NumericalDomains::DDoubleExact::BuiltDoubleTraits<FLOAT_REAL_BITS_NUMBER, 28, 8> > >;
template class Numerics::TReadDouble<
   Numerics::TBuiltDouble<NumericalDomains::DDoubleExact::BuiltDoubleTraits<FLOAT_REAL_BITS_NUMBER, 52, 11> >,
   Numerics::TBuiltDouble<NumericalDomains::DDoubleExact::BuiltDoubleTraits<FLOAT_REAL_BITS_NUMBER, 57, 11> > >;
template class Numerics::TReadDouble<
   Numerics::TBuiltDouble<NumericalDomains::DDoubleExact::BuiltDoubleTraits<FLOAT_REAL_BITS_NUMBER, LongDoubleFloatDigits::UBitSizeMantissa, LongDoubleFloatDigits::UBitSizeExponent> >,
   Numerics::TBuiltDouble<NumericalDomains::DDoubleExact::BuiltDoubleTraits<FLOAT_REAL_BITS_NUMBER, LongDoubleFloatDigits::UBitSizeMantissa+5, LongDoubleFloatDigits::UBitSizeExponent> > >;
template class Numerics::TReadDouble<
   Numerics::TBuiltDouble<NumericalDomains::DDoubleExact::BuiltDoubleTraits<FLOAT_REAL_BITS_NUMBER, FLOAT_REAL_BITS_NUMBER, LongDoubleFloatDigits::UBitSizeExponent> >,
   Numerics::TBuiltDouble<NumericalDomains::DDoubleExact::BuiltDoubleTraits<FLOAT_REAL_BITS_NUMBER, FLOAT_REAL_BITS_NUMBER+5, LongDoubleFloatDigits::UBitSizeExponent> > >;

template class NumericalDomains::DDoubleExact::TBuiltReal<FLOAT_REAL_BITS_NUMBER>;
template class NumericalDomains::DDoubleExact::TBuiltFloat<FLOAT_REAL_BITS_NUMBER, 23, 8>;
template class NumericalDomains::DDoubleExact::TBuiltFloat<FLOAT_REAL_BITS_NUMBER, 52, 11>;
template class NumericalDomains::DDoubleExact::TBuiltFloat<FLOAT_REAL_BITS_NUMBER, LongDoubleFloatDigits::UBitSizeMantissa, LongDoubleFloatDigits::UBitSizeExponent>;

#else
#include "NumericalLattices/FloatExactBaseTypesGeneric.template"

typedef NumericalDomains::DDoubleExact::FloatDigitsHelper::TFloatDigits<long double> LongDoubleFloatDigits;

#if defined(FLOAT_GENERIC_BASE_LONG)

template class Numerics::TGBuiltDouble<NumericalDomains::DDoubleExact::GBuiltDoubleTraits<Numerics::UnsignedLongBaseStoreTraits, FLOAT_REAL_BITS_NUMBER, 23, 8> >;
template class Numerics::TGBuiltDouble<NumericalDomains::DDoubleExact::GBuiltDoubleTraits<Numerics::UnsignedLongBaseStoreTraits, FLOAT_REAL_BITS_NUMBER, 52, 11> >;
template class Numerics::TGBuiltDouble<NumericalDomains::DDoubleExact::GBuiltDoubleTraits<Numerics::UnsignedLongBaseStoreTraits, FLOAT_REAL_BITS_NUMBER, LongDoubleFloatDigits::UBitSizeMantissa, LongDoubleFloatDigits::UBitSizeExponent> >;
template class Numerics::TGBuiltDouble<NumericalDomains::DDoubleExact::GBuiltDoubleTraits<Numerics::UnsignedLongBaseStoreTraits, FLOAT_REAL_BITS_NUMBER, FLOAT_REAL_BITS_NUMBER, LongDoubleFloatDigits::UBitSizeExponent> >;

template class Numerics::TGReadDouble<
   Numerics::TGBuiltDouble<NumericalDomains::DDoubleExact::GBuiltDoubleTraits<Numerics::UnsignedLongBaseStoreTraits, FLOAT_REAL_BITS_NUMBER, 23, 8> >,
   Numerics::TGBuiltDouble<NumericalDomains::DDoubleExact::GBuiltDoubleTraits<Numerics::UnsignedLongBaseStoreTraits, FLOAT_REAL_BITS_NUMBER, 28, 8> > >;
template class Numerics::TGReadDouble<
   Numerics::TGBuiltDouble<NumericalDomains::DDoubleExact::GBuiltDoubleTraits<Numerics::UnsignedLongBaseStoreTraits, FLOAT_REAL_BITS_NUMBER, 52, 11> >,
   Numerics::TGBuiltDouble<NumericalDomains::DDoubleExact::GBuiltDoubleTraits<Numerics::UnsignedLongBaseStoreTraits, FLOAT_REAL_BITS_NUMBER, 57, 11> > >;
template class Numerics::TGReadDouble<
   Numerics::TGBuiltDouble<NumericalDomains::DDoubleExact::GBuiltDoubleTraits<Numerics::UnsignedLongBaseStoreTraits, FLOAT_REAL_BITS_NUMBER, LongDoubleFloatDigits::UBitSizeMantissa, LongDoubleFloatDigits::UBitSizeExponent> >,
   Numerics::TGBuiltDouble<NumericalDomains::DDoubleExact::GBuiltDoubleTraits<Numerics::UnsignedLongBaseStoreTraits, FLOAT_REAL_BITS_NUMBER, LongDoubleFloatDigits::UBitSizeMantissa+5, LongDoubleFloatDigits::UBitSizeExponent> > >;
template class Numerics::TGReadDouble<
   Numerics::TGBuiltDouble<NumericalDomains::DDoubleExact::GBuiltDoubleTraits<Numerics::UnsignedLongBaseStoreTraits, FLOAT_REAL_BITS_NUMBER, FLOAT_REAL_BITS_NUMBER, LongDoubleFloatDigits::UBitSizeExponent> >,
   Numerics::TGBuiltDouble<NumericalDomains::DDoubleExact::GBuiltDoubleTraits<Numerics::UnsignedLongBaseStoreTraits, FLOAT_REAL_BITS_NUMBER, FLOAT_REAL_BITS_NUMBER+5, LongDoubleFloatDigits::UBitSizeExponent> > >;

template class NumericalDomains::DDoubleExact::TGBuiltReal<Numerics::UnsignedLongBaseStoreTraits, FLOAT_REAL_BITS_NUMBER>;
template class NumericalDomains::DDoubleExact::TGBuiltFloat<Numerics::UnsignedLongBaseStoreTraits, FLOAT_REAL_BITS_NUMBER, 23, 8>;
template class NumericalDomains::DDoubleExact::TGBuiltFloat<Numerics::UnsignedLongBaseStoreTraits, FLOAT_REAL_BITS_NUMBER, 52, 11>;
template class NumericalDomains::DDoubleExact::TGBuiltFloat<Numerics::UnsignedLongBaseStoreTraits, FLOAT_REAL_BITS_NUMBER, LongDoubleFloatDigits::UBitSizeMantissa, LongDoubleFloatDigits::UBitSizeExponent>;

template class Numerics::DInteger::TGBigCellIntAlgo<Numerics::UnsignedLongBaseStoreTraits, Numerics::DInteger::TGCellIntegerTraits<Numerics::UnsignedLongBaseStoreTraits, 2> >;

#else // defined(FLOAT_GENERIC_BASE_UNSIGNED)

template class Numerics::TGBuiltDouble<NumericalDomains::DDoubleExact::GBuiltDoubleTraits<Numerics::UnsignedBaseStoreTraits, FLOAT_REAL_BITS_NUMBER, 23, 8> >;
template class Numerics::TGBuiltDouble<NumericalDomains::DDoubleExact::GBuiltDoubleTraits<Numerics::UnsignedBaseStoreTraits, FLOAT_REAL_BITS_NUMBER, 52, 11> >;
template class Numerics::TGBuiltDouble<NumericalDomains::DDoubleExact::GBuiltDoubleTraits<Numerics::UnsignedBaseStoreTraits, FLOAT_REAL_BITS_NUMBER, LongDoubleFloatDigits::UBitSizeMantissa, LongDoubleFloatDigits::UBitSizeExponent> >;
template class Numerics::TGBuiltDouble<NumericalDomains::DDoubleExact::GBuiltDoubleTraits<Numerics::UnsignedBaseStoreTraits, FLOAT_REAL_BITS_NUMBER, FLOAT_REAL_BITS_NUMBER, LongDoubleFloatDigits::UBitSizeExponent> >;
// template class Numerics::TGBuiltDouble<NumericalDomains::DDoubleExact::GBuiltDoubleTraits<Numerics::UnsignedBaseStoreTraits, 123, 123, LongDoubleFloatDigits::UBitSizeExponent> >;

template class Numerics::TGReadDouble<
   Numerics::TGBuiltDouble<NumericalDomains::DDoubleExact::GBuiltDoubleTraits<Numerics::UnsignedBaseStoreTraits, FLOAT_REAL_BITS_NUMBER, 23, 8> >,
   Numerics::TGBuiltDouble<NumericalDomains::DDoubleExact::GBuiltDoubleTraits<Numerics::UnsignedBaseStoreTraits, FLOAT_REAL_BITS_NUMBER, 28, 8> > >;
template class Numerics::TGReadDouble<
   Numerics::TGBuiltDouble<NumericalDomains::DDoubleExact::GBuiltDoubleTraits<Numerics::UnsignedBaseStoreTraits, FLOAT_REAL_BITS_NUMBER, 52, 11> >,
   Numerics::TGBuiltDouble<NumericalDomains::DDoubleExact::GBuiltDoubleTraits<Numerics::UnsignedBaseStoreTraits, FLOAT_REAL_BITS_NUMBER, 57, 11> > >;
template class Numerics::TGReadDouble<
   Numerics::TGBuiltDouble<NumericalDomains::DDoubleExact::GBuiltDoubleTraits<Numerics::UnsignedBaseStoreTraits, FLOAT_REAL_BITS_NUMBER, LongDoubleFloatDigits::UBitSizeMantissa, LongDoubleFloatDigits::UBitSizeExponent> >,
   Numerics::TGBuiltDouble<NumericalDomains::DDoubleExact::GBuiltDoubleTraits<Numerics::UnsignedBaseStoreTraits, FLOAT_REAL_BITS_NUMBER, LongDoubleFloatDigits::UBitSizeMantissa+5, LongDoubleFloatDigits::UBitSizeExponent> > >;
template class Numerics::TGReadDouble<
   Numerics::TGBuiltDouble<NumericalDomains::DDoubleExact::GBuiltDoubleTraits<Numerics::UnsignedBaseStoreTraits, FLOAT_REAL_BITS_NUMBER, FLOAT_REAL_BITS_NUMBER, LongDoubleFloatDigits::UBitSizeExponent> >,
   Numerics::TGBuiltDouble<NumericalDomains::DDoubleExact::GBuiltDoubleTraits<Numerics::UnsignedBaseStoreTraits, FLOAT_REAL_BITS_NUMBER, FLOAT_REAL_BITS_NUMBER+5, LongDoubleFloatDigits::UBitSizeExponent> > >;

template class NumericalDomains::DDoubleExact::TGBuiltReal<Numerics::UnsignedBaseStoreTraits, FLOAT_REAL_BITS_NUMBER>;
template class NumericalDomains::DDoubleExact::TGBuiltFloat<Numerics::UnsignedBaseStoreTraits, FLOAT_REAL_BITS_NUMBER, 23, 8>;
template class NumericalDomains::DDoubleExact::TGBuiltFloat<Numerics::UnsignedBaseStoreTraits, FLOAT_REAL_BITS_NUMBER, 52, 11>;
template class NumericalDomains::DDoubleExact::TGBuiltFloat<Numerics::UnsignedBaseStoreTraits, FLOAT_REAL_BITS_NUMBER, LongDoubleFloatDigits::UBitSizeMantissa, LongDoubleFloatDigits::UBitSizeExponent>;

#endif
#endif
