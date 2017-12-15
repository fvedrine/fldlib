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

// [TODO] assume sizeof(unsigned) == 4
#if ((FLOAT_REAL_BITS_NUMBER+5+8*4-1)/(8*4)) > ((80+5+8*4-1)/(8*4))
template class Numerics::DInteger::TBigCellInt<Numerics::DInteger::TCellIntegerTraits<(FLOAT_REAL_BITS_NUMBER+5+8*sizeof(unsigned)-1)/(8*sizeof(unsigned))> >;
#endif
#if (((FLOAT_REAL_BITS_NUMBER+5+8*4-1)/(8*4)) > ((80+5+8*4-1)/(8*4))) && (((FLOAT_REAL_BITS_NUMBER+5+8*4-1)/(8*4)) > ((FLOAT_REAL_BITS_NUMBER+8*4-1)/(8*4)))
template class Numerics::DInteger::TBigCellInt<Numerics::DInteger::TCellIntegerTraits<(FLOAT_REAL_BITS_NUMBER+8*sizeof(unsigned)-1)/(8*sizeof(unsigned))> >;
#endif
template class Numerics::DInteger::TBigCellInt<Numerics::DInteger::TCellIntegerTraits<(80+5+8*sizeof(unsigned)-1)/(8*sizeof(unsigned))> >;
// template class Numerics::DInteger::TBigCellInt<Numerics::DInteger::TCellIntegerTraits<(80+8*sizeof(unsigned)-1)/(8*sizeof(unsigned))> >;
template class Numerics::DInteger::TBigCellInt<Numerics::DInteger::TCellIntegerTraits<(52+5+8*sizeof(unsigned)-1)/(8*sizeof(unsigned))> >;
// template class Numerics::DInteger::TBigCellInt<Numerics::DInteger::TCellIntegerTraits<(52+8*sizeof(unsigned)-1)/(8*sizeof(unsigned))> >;
template class Numerics::DInteger::TBigCellInt<Numerics::DInteger::TCellIntegerTraits<(23+5+8*sizeof(unsigned)-1)/(8*sizeof(unsigned))> >;
// template class Numerics::DInteger::TBigCellInt<Numerics::DInteger::TCellIntegerTraits<(23+8*sizeof(unsigned)-1)/(8*sizeof(unsigned))> >;

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
// template class Numerics::DInteger::TBigCellInt<Numerics::DInteger::TCellIntegerTraits<(FLOAT_REAL_BITS_NUMBER+8*sizeof(unsigned)-1)/(8*sizeof(unsigned))> >;

#else
#include "NumericalLattices/FloatAffineBaseTypesGeneric.template"

#if defined(FLOAT_GENERIC_BASE_LONG)

// [TODO] assume sizeof(unsigned long) == 8
#if ((FLOAT_REAL_BITS_NUMBER+5+8*8-1)/(8*8)) > ((80+5+8*8-1)/(8*8))
template class Numerics::DInteger::TGBigCellInt<Numerics::UnsignedLongBaseStoreTraits,
      Numerics::DInteger::TGCellIntegerTraits<Numerics::UnsignedLongBaseStoreTraits,
         (FLOAT_REAL_BITS_NUMBER+5+Numerics::UnsignedLongBaseStoreTraits::USizeBaseInBits-1)/Numerics::UnsignedLongBaseStoreTraits::USizeBaseInBits> >;
#endif
#if (((FLOAT_REAL_BITS_NUMBER+5+8*8-1)/(8*8)) > ((80+5+8*8-1)/(8*8))) && (((FLOAT_REAL_BITS_NUMBER+5+8*8-1)/(8*8)) > ((FLOAT_REAL_BITS_NUMBER+8*8-1)/(8*8)))
template class Numerics::DInteger::TGBigCellInt<Numerics::UnsignedLongBaseStoreTraits,
      Numerics::DInteger::TGCellIntegerTraits<Numerics::UnsignedLongBaseStoreTraits,
         (FLOAT_REAL_BITS_NUMBER+Numerics::UnsignedLongBaseStoreTraits::USizeBaseInBits-1)/Numerics::UnsignedLongBaseStoreTraits::USizeBaseInBits> >;
#endif
template class Numerics::DInteger::TGBigCellInt<Numerics::UnsignedLongBaseStoreTraits,
      Numerics::DInteger::TGCellIntegerTraits<Numerics::UnsignedLongBaseStoreTraits,
         (80+5+Numerics::UnsignedLongBaseStoreTraits::USizeBaseInBits-1)/Numerics::UnsignedLongBaseStoreTraits::USizeBaseInBits> >;
// template class Numerics::DInteger::TGBigCellInt<Numerics::UnsignedLongBaseStoreTraits,
//    Numerics::DInteger::TGCellIntegerTraits<Numerics::UnsignedLongBaseStoreTraits,
//       (80+Numerics::UnsignedLongBaseStoreTraits::USizeBaseInBits-1)/Numerics::UnsignedLongBaseStoreTraits::USizeBaseInBits> >;
template class Numerics::DInteger::TGBigCellInt<Numerics::UnsignedLongBaseStoreTraits,
      Numerics::DInteger::TGCellIntegerTraits<Numerics::UnsignedLongBaseStoreTraits,
         (52+5+Numerics::UnsignedLongBaseStoreTraits::USizeBaseInBits-1)/Numerics::UnsignedLongBaseStoreTraits::USizeBaseInBits> >;
// template class Numerics::DInteger::TGBigCellInt<Numerics::UnsignedLongBaseStoreTraits,
//    Numerics::DInteger::TGCellIntegerTraits<Numerics::UnsignedLongBaseStoreTraits,
//       (52+Numerics::UnsignedLongBaseStoreTraits::USizeBaseInBits-1)/Numerics::UnsignedLongBaseStoreTraits::USizeBaseInBits> >;
// template class Numerics::DInteger::TGBigCellInt<Numerics::UnsignedLongBaseStoreTraits,
//       Numerics::DInteger::TGCellIntegerTraits<Numerics::UnsignedLongBaseStoreTraits,
//          (23+5+Numerics::UnsignedLongBaseStoreTraits::USizeBaseInBits-1)/Numerics::UnsignedLongBaseStoreTraits::USizeBaseInBits> >;
// template class Numerics::DInteger::TGBigCellInt<Numerics::UnsignedLongBaseStoreTraits,
//    Numerics::DInteger::TGCellIntegerTraits<Numerics::UnsignedLongBaseStoreTraits,
//       (23+Numerics::UnsignedLongBaseStoreTraits::USizeBaseInBits-1)/Numerics::UnsignedLongBaseStoreTraits::USizeBaseInBits> >;

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
// template class Numerics::DInteger::TGBigCellInt<Numerics::UnsignedLongBaseStoreTraits, Numerics::DInteger::TGCellIntegerTraits<Numerics::UnsignedLongBaseStoreTraits, ((FLOAT_REAL_BITS_NUMBER+8*sizeof(unsigned)-1)/(8*sizeof(unsigned)))> >;

#else // defined(FLOAT_GENERIC_BASE_UNSIGNED)

// [TODO] assume sizeof(unsigned) == 4
#if ((FLOAT_REAL_BITS_NUMBER+5+8*4-1)/(8*4)) > ((80+5+8*4-1)/(8*4))
template class Numerics::DInteger::TGBigCellInt<Numerics::UnsignedBaseStoreTraits,
      Numerics::DInteger::TGCellIntegerTraits<Numerics::UnsignedBaseStoreTraits,
         (FLOAT_REAL_BITS_NUMBER+5+Numerics::UnsignedBaseStoreTraits::USizeBaseInBits-1)/Numerics::UnsignedBaseStoreTraits::USizeBaseInBits> >;
#endif
#if (((FLOAT_REAL_BITS_NUMBER+5+8*4-1)/(8*4)) > ((80+5+8*4-1)/(8*4))) && (((FLOAT_REAL_BITS_NUMBER+5+8*4-1)/(8*4)) > ((FLOAT_REAL_BITS_NUMBER+8*4-1)/(8*4)))
template class Numerics::DInteger::TGBigCellInt<Numerics::UnsignedBaseStoreTraits,
      Numerics::DInteger::TGCellIntegerTraits<Numerics::UnsignedBaseStoreTraits,
         (FLOAT_REAL_BITS_NUMBER+Numerics::UnsignedBaseStoreTraits::USizeBaseInBits-1)/Numerics::UnsignedBaseStoreTraits::USizeBaseInBits> >;
#endif
template class Numerics::DInteger::TGBigCellInt<Numerics::UnsignedBaseStoreTraits,
      Numerics::DInteger::TGCellIntegerTraits<Numerics::UnsignedBaseStoreTraits,
         (80+5+Numerics::UnsignedBaseStoreTraits::USizeBaseInBits-1)/Numerics::UnsignedBaseStoreTraits::USizeBaseInBits> >;
// template class Numerics::DInteger::TGBigCellInt<Numerics::UnsignedBaseStoreTraits,
//    Numerics::DInteger::TGCellIntegerTraits<Numerics::UnsignedBaseStoreTraits,
//       (80+Numerics::UnsignedBaseStoreTraits::USizeBaseInBits-1)/Numerics::UnsignedBaseStoreTraits::USizeBaseInBits> >;
template class Numerics::DInteger::TGBigCellInt<Numerics::UnsignedBaseStoreTraits,
      Numerics::DInteger::TGCellIntegerTraits<Numerics::UnsignedBaseStoreTraits,
         (52+5+Numerics::UnsignedBaseStoreTraits::USizeBaseInBits-1)/Numerics::UnsignedBaseStoreTraits::USizeBaseInBits> >;
// template class Numerics::DInteger::TGBigCellInt<Numerics::UnsignedBaseStoreTraits,
//    Numerics::DInteger::TGCellIntegerTraits<Numerics::UnsignedBaseStoreTraits,
//       (52+Numerics::UnsignedBaseStoreTraits::USizeBaseInBits-1)/Numerics::UnsignedBaseStoreTraits::USizeBaseInBits> >;
template class Numerics::DInteger::TGBigCellInt<Numerics::UnsignedBaseStoreTraits,
      Numerics::DInteger::TGCellIntegerTraits<Numerics::UnsignedBaseStoreTraits,
         (23+5+Numerics::UnsignedBaseStoreTraits::USizeBaseInBits-1)/Numerics::UnsignedBaseStoreTraits::USizeBaseInBits> >;
// template class Numerics::DInteger::TGBigCellInt<Numerics::UnsignedBaseStoreTraits,
//    Numerics::DInteger::TGCellIntegerTraits<Numerics::UnsignedBaseStoreTraits,
//       (23+Numerics::UnsignedBaseStoreTraits::USizeBaseInBits-1)/Numerics::UnsignedBaseStoreTraits::USizeBaseInBits> >;

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
// template class Numerics::DInteger::TGBigCellInt<Numerics::UnsignedLongBaseStoreTraits, Numerics::DInteger::TGCellIntegerTraits<Numerics::UnsignedLongBaseStoreTraits, ((FLOAT_REAL_BITS_NUMBER+8*sizeof(unsigned)-1)/(8*sizeof(unsigned)))> >;

#endif
#endif

