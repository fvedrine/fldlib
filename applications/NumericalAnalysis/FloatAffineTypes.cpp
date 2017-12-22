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

template class Numerics::TBuiltDouble<NumericalDomains::DAffine::BuiltDoubleTraits<FLOAT_REAL_BITS_NUMBER, 23, 8> >;
template class Numerics::TBuiltDouble<NumericalDomains::DAffine::BuiltDoubleTraits<FLOAT_REAL_BITS_NUMBER, 52, 11> >;
template class Numerics::TBuiltDouble<NumericalDomains::DAffine::BuiltDoubleTraits<FLOAT_REAL_BITS_NUMBER, 80, 15> >;
template class Numerics::TBuiltDouble<NumericalDomains::DAffine::BuiltDoubleTraits<FLOAT_REAL_BITS_NUMBER, FLOAT_REAL_BITS_NUMBER, 15> >;

template class Numerics::TReadDouble<
   Numerics::TBuiltDouble<NumericalDomains::DAffine::BuiltDoubleTraits<FLOAT_REAL_BITS_NUMBER, 23, 8> >,
   Numerics::TBuiltDouble<NumericalDomains::DAffine::BuiltDoubleTraits<FLOAT_REAL_BITS_NUMBER, 28, 8> > >;
template class Numerics::TReadDouble<
   Numerics::TBuiltDouble<NumericalDomains::DAffine::BuiltDoubleTraits<FLOAT_REAL_BITS_NUMBER, 52, 11> >,
   Numerics::TBuiltDouble<NumericalDomains::DAffine::BuiltDoubleTraits<FLOAT_REAL_BITS_NUMBER, 57, 11> > >;
template class Numerics::TReadDouble<
   Numerics::TBuiltDouble<NumericalDomains::DAffine::BuiltDoubleTraits<FLOAT_REAL_BITS_NUMBER, 80, 15> >,
   Numerics::TBuiltDouble<NumericalDomains::DAffine::BuiltDoubleTraits<FLOAT_REAL_BITS_NUMBER, 85, 15> > >;
template class Numerics::TReadDouble<
   Numerics::TBuiltDouble<NumericalDomains::DAffine::BuiltDoubleTraits<FLOAT_REAL_BITS_NUMBER, FLOAT_REAL_BITS_NUMBER, 15> >,
   Numerics::TBuiltDouble<NumericalDomains::DAffine::BuiltDoubleTraits<FLOAT_REAL_BITS_NUMBER, FLOAT_REAL_BITS_NUMBER+5, 15> > >;

template class NumericalDomains::DAffine::TBuiltReal<FLOAT_REAL_BITS_NUMBER>;

#else
#include "NumericalLattices/FloatAffineBaseTypesGeneric.template"

#if defined(FLOAT_GENERIC_BASE_LONG)

template class Numerics::TGBuiltDouble<NumericalDomains::DAffine::GBuiltDoubleTraits<Numerics::UnsignedLongBaseStoreTraits, FLOAT_REAL_BITS_NUMBER, 23, 8> >;
template class Numerics::TGBuiltDouble<NumericalDomains::DAffine::GBuiltDoubleTraits<Numerics::UnsignedLongBaseStoreTraits, FLOAT_REAL_BITS_NUMBER, 52, 11> >;
template class Numerics::TGBuiltDouble<NumericalDomains::DAffine::GBuiltDoubleTraits<Numerics::UnsignedLongBaseStoreTraits, FLOAT_REAL_BITS_NUMBER, 80, 15> >;
template class Numerics::TGBuiltDouble<NumericalDomains::DAffine::GBuiltDoubleTraits<Numerics::UnsignedLongBaseStoreTraits, FLOAT_REAL_BITS_NUMBER, FLOAT_REAL_BITS_NUMBER, 15> >;

template class Numerics::TGReadDouble<
   Numerics::TGBuiltDouble<NumericalDomains::DAffine::GBuiltDoubleTraits<Numerics::UnsignedLongBaseStoreTraits, FLOAT_REAL_BITS_NUMBER, 23, 8> >,
   Numerics::TGBuiltDouble<NumericalDomains::DAffine::GBuiltDoubleTraits<Numerics::UnsignedLongBaseStoreTraits, FLOAT_REAL_BITS_NUMBER, 28, 8> > >;
template class Numerics::TGReadDouble<
   Numerics::TGBuiltDouble<NumericalDomains::DAffine::GBuiltDoubleTraits<Numerics::UnsignedLongBaseStoreTraits, FLOAT_REAL_BITS_NUMBER, 52, 11> >,
   Numerics::TGBuiltDouble<NumericalDomains::DAffine::GBuiltDoubleTraits<Numerics::UnsignedLongBaseStoreTraits, FLOAT_REAL_BITS_NUMBER, 57, 11> > >;
template class Numerics::TGReadDouble<
   Numerics::TGBuiltDouble<NumericalDomains::DAffine::GBuiltDoubleTraits<Numerics::UnsignedLongBaseStoreTraits, FLOAT_REAL_BITS_NUMBER, 80, 15> >,
   Numerics::TGBuiltDouble<NumericalDomains::DAffine::GBuiltDoubleTraits<Numerics::UnsignedLongBaseStoreTraits, FLOAT_REAL_BITS_NUMBER, 85, 15> > >;
template class Numerics::TGReadDouble<
   Numerics::TGBuiltDouble<NumericalDomains::DAffine::GBuiltDoubleTraits<Numerics::UnsignedLongBaseStoreTraits, FLOAT_REAL_BITS_NUMBER, FLOAT_REAL_BITS_NUMBER, 15> >,
   Numerics::TGBuiltDouble<NumericalDomains::DAffine::GBuiltDoubleTraits<Numerics::UnsignedLongBaseStoreTraits, FLOAT_REAL_BITS_NUMBER, FLOAT_REAL_BITS_NUMBER+5, 15> > >;

template class NumericalDomains::DAffine::TGBuiltReal<Numerics::UnsignedLongBaseStoreTraits, FLOAT_REAL_BITS_NUMBER>;

#else // defined(FLOAT_GENERIC_BASE_UNSIGNED)

template class Numerics::TGBuiltDouble<NumericalDomains::DAffine::GBuiltDoubleTraits<Numerics::UnsignedBaseStoreTraits, FLOAT_REAL_BITS_NUMBER, 23, 8> >;
template class Numerics::TGBuiltDouble<NumericalDomains::DAffine::GBuiltDoubleTraits<Numerics::UnsignedBaseStoreTraits, FLOAT_REAL_BITS_NUMBER, 52, 11> >;
template class Numerics::TGBuiltDouble<NumericalDomains::DAffine::GBuiltDoubleTraits<Numerics::UnsignedBaseStoreTraits, FLOAT_REAL_BITS_NUMBER, 80, 15> >;
template class Numerics::TGBuiltDouble<NumericalDomains::DAffine::GBuiltDoubleTraits<Numerics::UnsignedBaseStoreTraits, FLOAT_REAL_BITS_NUMBER, FLOAT_REAL_BITS_NUMBER, 15> >;

template class Numerics::TGReadDouble<
   Numerics::TGBuiltDouble<NumericalDomains::DAffine::GBuiltDoubleTraits<Numerics::UnsignedBaseStoreTraits, FLOAT_REAL_BITS_NUMBER, 23, 8> >,
   Numerics::TGBuiltDouble<NumericalDomains::DAffine::GBuiltDoubleTraits<Numerics::UnsignedBaseStoreTraits, FLOAT_REAL_BITS_NUMBER, 28, 8> > >;
template class Numerics::TGReadDouble<
   Numerics::TGBuiltDouble<NumericalDomains::DAffine::GBuiltDoubleTraits<Numerics::UnsignedBaseStoreTraits, FLOAT_REAL_BITS_NUMBER, 52, 11> >,
   Numerics::TGBuiltDouble<NumericalDomains::DAffine::GBuiltDoubleTraits<Numerics::UnsignedBaseStoreTraits, FLOAT_REAL_BITS_NUMBER, 57, 11> > >;
template class Numerics::TGReadDouble<
   Numerics::TGBuiltDouble<NumericalDomains::DAffine::GBuiltDoubleTraits<Numerics::UnsignedBaseStoreTraits, FLOAT_REAL_BITS_NUMBER, 80, 15> >,
   Numerics::TGBuiltDouble<NumericalDomains::DAffine::GBuiltDoubleTraits<Numerics::UnsignedBaseStoreTraits, FLOAT_REAL_BITS_NUMBER, 85, 15> > >;
template class Numerics::TGReadDouble<
   Numerics::TGBuiltDouble<NumericalDomains::DAffine::GBuiltDoubleTraits<Numerics::UnsignedBaseStoreTraits, FLOAT_REAL_BITS_NUMBER, FLOAT_REAL_BITS_NUMBER, 15> >,
   Numerics::TGBuiltDouble<NumericalDomains::DAffine::GBuiltDoubleTraits<Numerics::UnsignedBaseStoreTraits, FLOAT_REAL_BITS_NUMBER, FLOAT_REAL_BITS_NUMBER+5, 15> > >;

template class NumericalDomains::DAffine::TGBuiltReal<Numerics::UnsignedBaseStoreTraits, FLOAT_REAL_BITS_NUMBER>;

#endif
#endif

