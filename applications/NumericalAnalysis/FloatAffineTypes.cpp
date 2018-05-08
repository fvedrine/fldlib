/**************************************************************************/
/*                                                                        */
/*  This file is part of FLDLib                                           */
/*                                                                        */
/*  Copyright (C) 2011-2017                                               */
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
// Unit      : FloatAffine
// File      : FloatAffineTypes.cpp
// Description :
//   Implementation of a class of affine relations.
//

#include "NumericalAnalysis/FloatAffineExecutionPath.h"

#if !defined(FLOAT_GENERIC_BASE_UNSIGNED) && !defined(FLOAT_GENERIC_BASE_LONG)
#include "NumericalLattices/FloatAffineBaseTypes.template"

typedef NumericalDomains::DAffine::FloatDigitsHelper::TFloatDigits<long double> LongDoubleFloatDigits;

template class Numerics::DInteger::TBigCellIntAlgo<Numerics::DInteger::TCellIntegerTraits<(FLOAT_REAL_BITS_NUMBER +8*sizeof(unsigned)-1)/(8*sizeof(unsigned))> >;
template class Numerics::TBuiltDouble<NumericalDomains::DAffine::BuiltDoubleTraits<FLOAT_REAL_BITS_NUMBER, 23, 8> >;
template class Numerics::TBuiltDouble<NumericalDomains::DAffine::BuiltDoubleTraits<FLOAT_REAL_BITS_NUMBER, 52, 11> >;
template class Numerics::TBuiltDouble<NumericalDomains::DAffine::BuiltDoubleTraits<FLOAT_REAL_BITS_NUMBER, LongDoubleFloatDigits::UBitSizeMantissa, LongDoubleFloatDigits::UBitSizeExponent> >;
template class Numerics::TBuiltDouble<NumericalDomains::DAffine::BuiltDoubleTraits<FLOAT_REAL_BITS_NUMBER, FLOAT_REAL_BITS_NUMBER, LongDoubleFloatDigits::UBitSizeExponent> >;

template class Numerics::TReadDouble<
   Numerics::TBuiltDouble<NumericalDomains::DAffine::BuiltDoubleTraits<FLOAT_REAL_BITS_NUMBER, 23, 8> >,
   Numerics::TBuiltDouble<NumericalDomains::DAffine::BuiltDoubleTraits<FLOAT_REAL_BITS_NUMBER, 28, 8> > >;
template class Numerics::TReadDouble<
   Numerics::TBuiltDouble<NumericalDomains::DAffine::BuiltDoubleTraits<FLOAT_REAL_BITS_NUMBER, 52, 11> >,
   Numerics::TBuiltDouble<NumericalDomains::DAffine::BuiltDoubleTraits<FLOAT_REAL_BITS_NUMBER, 57, 11> > >;
template class Numerics::TReadDouble<
   Numerics::TBuiltDouble<NumericalDomains::DAffine::BuiltDoubleTraits<FLOAT_REAL_BITS_NUMBER, LongDoubleFloatDigits::UBitSizeMantissa, LongDoubleFloatDigits::UBitSizeExponent> >,
   Numerics::TBuiltDouble<NumericalDomains::DAffine::BuiltDoubleTraits<FLOAT_REAL_BITS_NUMBER, LongDoubleFloatDigits::UBitSizeMantissa+5, LongDoubleFloatDigits::UBitSizeExponent> > >;
template class Numerics::TReadDouble<
   Numerics::TBuiltDouble<NumericalDomains::DAffine::BuiltDoubleTraits<FLOAT_REAL_BITS_NUMBER, FLOAT_REAL_BITS_NUMBER, LongDoubleFloatDigits::UBitSizeExponent> >,
   Numerics::TBuiltDouble<NumericalDomains::DAffine::BuiltDoubleTraits<FLOAT_REAL_BITS_NUMBER, FLOAT_REAL_BITS_NUMBER+5, LongDoubleFloatDigits::UBitSizeExponent> > >;

template class NumericalDomains::DAffine::TBuiltReal<FLOAT_REAL_BITS_NUMBER>;

#else
#include "NumericalLattices/FloatAffineBaseTypesGeneric.template"

typedef NumericalDomains::DAffine::FloatDigitsHelper::TFloatDigits<long double> LongDoubleFloatDigits;

#if defined(FLOAT_GENERIC_BASE_LONG)

template class Numerics::DInteger::TGBigCellIntAlgo<Numerics::UnsignedLongBaseStoreTraits, Numerics::DInteger::TGCellIntegerTraits<Numerics::UnsignedLongBaseStoreTraits, (FLOAT_REAL_BITS_NUMBER +8*sizeof(unsigned long)-1)/(8*sizeof(unsigned long))> >;
template class Numerics::TGBuiltDouble<NumericalDomains::DAffine::GBuiltDoubleTraits<Numerics::UnsignedLongBaseStoreTraits, FLOAT_REAL_BITS_NUMBER, 23, 8> >;
template class Numerics::TGBuiltDouble<NumericalDomains::DAffine::GBuiltDoubleTraits<Numerics::UnsignedLongBaseStoreTraits, FLOAT_REAL_BITS_NUMBER, 52, 11> >;
template class Numerics::TGBuiltDouble<NumericalDomains::DAffine::GBuiltDoubleTraits<Numerics::UnsignedLongBaseStoreTraits, FLOAT_REAL_BITS_NUMBER, LongDoubleFloatDigits::UBitSizeMantissa, LongDoubleFloatDigits::UBitSizeExponent> >;
template class Numerics::TGBuiltDouble<NumericalDomains::DAffine::GBuiltDoubleTraits<Numerics::UnsignedLongBaseStoreTraits, FLOAT_REAL_BITS_NUMBER, FLOAT_REAL_BITS_NUMBER, LongDoubleFloatDigits::UBitSizeExponent> >;

template class Numerics::TGReadDouble<
   Numerics::TGBuiltDouble<NumericalDomains::DAffine::GBuiltDoubleTraits<Numerics::UnsignedLongBaseStoreTraits, FLOAT_REAL_BITS_NUMBER, 23, 8> >,
   Numerics::TGBuiltDouble<NumericalDomains::DAffine::GBuiltDoubleTraits<Numerics::UnsignedLongBaseStoreTraits, FLOAT_REAL_BITS_NUMBER, 28, 8> > >;
template class Numerics::TGReadDouble<
   Numerics::TGBuiltDouble<NumericalDomains::DAffine::GBuiltDoubleTraits<Numerics::UnsignedLongBaseStoreTraits, FLOAT_REAL_BITS_NUMBER, 52, 11> >,
   Numerics::TGBuiltDouble<NumericalDomains::DAffine::GBuiltDoubleTraits<Numerics::UnsignedLongBaseStoreTraits, FLOAT_REAL_BITS_NUMBER, 57, 11> > >;
template class Numerics::TGReadDouble<
   Numerics::TGBuiltDouble<NumericalDomains::DAffine::GBuiltDoubleTraits<Numerics::UnsignedLongBaseStoreTraits, FLOAT_REAL_BITS_NUMBER, LongDoubleFloatDigits::UBitSizeMantissa, LongDoubleFloatDigits::UBitSizeExponent> >,
   Numerics::TGBuiltDouble<NumericalDomains::DAffine::GBuiltDoubleTraits<Numerics::UnsignedLongBaseStoreTraits, FLOAT_REAL_BITS_NUMBER, LongDoubleFloatDigits::UBitSizeMantissa+5, LongDoubleFloatDigits::UBitSizeExponent> > >;
template class Numerics::TGReadDouble<
   Numerics::TGBuiltDouble<NumericalDomains::DAffine::GBuiltDoubleTraits<Numerics::UnsignedLongBaseStoreTraits, FLOAT_REAL_BITS_NUMBER, FLOAT_REAL_BITS_NUMBER, LongDoubleFloatDigits::UBitSizeExponent> >,
   Numerics::TGBuiltDouble<NumericalDomains::DAffine::GBuiltDoubleTraits<Numerics::UnsignedLongBaseStoreTraits, FLOAT_REAL_BITS_NUMBER, FLOAT_REAL_BITS_NUMBER+5, LongDoubleFloatDigits::UBitSizeExponent> > >;

template class NumericalDomains::DAffine::TGBuiltReal<Numerics::UnsignedLongBaseStoreTraits, FLOAT_REAL_BITS_NUMBER>;

#else // defined(FLOAT_GENERIC_BASE_UNSIGNED)

template class Numerics::DInteger::TGBigCellIntAlgo<Numerics::UnsignedBaseStoreTraits, Numerics::DInteger::TGCellIntegerTraits<Numerics::UnsignedBaseStoreTraits, (FLOAT_REAL_BITS_NUMBER +8*sizeof(unsigned)-1)/(8*sizeof(unsigned))> >;
template class Numerics::TGBuiltDouble<NumericalDomains::DAffine::GBuiltDoubleTraits<Numerics::UnsignedBaseStoreTraits, FLOAT_REAL_BITS_NUMBER, 23, 8> >;
template class Numerics::TGBuiltDouble<NumericalDomains::DAffine::GBuiltDoubleTraits<Numerics::UnsignedBaseStoreTraits, FLOAT_REAL_BITS_NUMBER, 52, 11> >;
template class Numerics::TGBuiltDouble<NumericalDomains::DAffine::GBuiltDoubleTraits<Numerics::UnsignedBaseStoreTraits, FLOAT_REAL_BITS_NUMBER, LongDoubleFloatDigits::UBitSizeMantissa, LongDoubleFloatDigits::UBitSizeExponent> >;
template class Numerics::TGBuiltDouble<NumericalDomains::DAffine::GBuiltDoubleTraits<Numerics::UnsignedBaseStoreTraits, FLOAT_REAL_BITS_NUMBER, FLOAT_REAL_BITS_NUMBER, LongDoubleFloatDigits::UBitSizeExponent> >;

template class Numerics::TGReadDouble<
   Numerics::TGBuiltDouble<NumericalDomains::DAffine::GBuiltDoubleTraits<Numerics::UnsignedBaseStoreTraits, FLOAT_REAL_BITS_NUMBER, 23, 8> >,
   Numerics::TGBuiltDouble<NumericalDomains::DAffine::GBuiltDoubleTraits<Numerics::UnsignedBaseStoreTraits, FLOAT_REAL_BITS_NUMBER, 28, 8> > >;
template class Numerics::TGReadDouble<
   Numerics::TGBuiltDouble<NumericalDomains::DAffine::GBuiltDoubleTraits<Numerics::UnsignedBaseStoreTraits, FLOAT_REAL_BITS_NUMBER, 52, 11> >,
   Numerics::TGBuiltDouble<NumericalDomains::DAffine::GBuiltDoubleTraits<Numerics::UnsignedBaseStoreTraits, FLOAT_REAL_BITS_NUMBER, 57, 11> > >;
template class Numerics::TGReadDouble<
   Numerics::TGBuiltDouble<NumericalDomains::DAffine::GBuiltDoubleTraits<Numerics::UnsignedBaseStoreTraits, FLOAT_REAL_BITS_NUMBER, LongDoubleFloatDigits::UBitSizeMantissa, LongDoubleFloatDigits::UBitSizeExponent> >,
   Numerics::TGBuiltDouble<NumericalDomains::DAffine::GBuiltDoubleTraits<Numerics::UnsignedBaseStoreTraits, FLOAT_REAL_BITS_NUMBER, LongDoubleFloatDigits::UBitSizeMantissa+5, LongDoubleFloatDigits::UBitSizeExponent> > >;
template class Numerics::TGReadDouble<
   Numerics::TGBuiltDouble<NumericalDomains::DAffine::GBuiltDoubleTraits<Numerics::UnsignedBaseStoreTraits, FLOAT_REAL_BITS_NUMBER, FLOAT_REAL_BITS_NUMBER, LongDoubleFloatDigits::UBitSizeExponent> >,
   Numerics::TGBuiltDouble<NumericalDomains::DAffine::GBuiltDoubleTraits<Numerics::UnsignedBaseStoreTraits, FLOAT_REAL_BITS_NUMBER, FLOAT_REAL_BITS_NUMBER+5, LongDoubleFloatDigits::UBitSizeExponent> > >;

template class NumericalDomains::DAffine::TGBuiltReal<Numerics::UnsignedBaseStoreTraits, FLOAT_REAL_BITS_NUMBER>;

#endif
#endif

