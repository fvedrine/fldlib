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
// File      : FloatIntervalBaseTypes.h
// Description :
//   Definition of a class of floating point intervals
//

#ifndef NumericalDomains_FloatIntervalBaseTypesH
#define NumericalDomains_FloatIntervalBaseTypesH

#include "Numerics/Floating.h"
#include <math.h>

namespace NumericalDomains { namespace DDoubleInterval {

template <int UMaxBitsNumber>
class TFloatConversion : public Numerics::DDouble::BuiltAccess
      ::TFloatConversion<(UMaxBitsNumber+(8*sizeof(unsigned))+5-1)/(8*sizeof(unsigned)), 2> {
  public:
   TFloatConversion() {}
   TFloatConversion(const TFloatConversion<UMaxBitsNumber>& source) = default;
};

template <int UMaxBitsNumber, int USizeMantissa, int USizeExponent>
class BuiltDoubleTraits : public Numerics::BuiltDoubleTraits<USizeMantissa, USizeExponent> {
  private:
   typedef Numerics::BuiltDoubleTraits<USizeMantissa, USizeExponent> inherited;

  public:
   typedef DDoubleInterval::TFloatConversion<UMaxBitsNumber> FloatConversion;

   typedef typename inherited::Exponent Exponent;
   // overriden to avoid race conditions with global variables
   static Exponent getZeroExponent(const Exponent&) { return Exponent(typename Exponent::Zero()); }
   static Exponent getOneExponent(const Exponent&) { return Exponent(typename Exponent::One()); }
   static Exponent getMinusOneExponent(const Exponent&) { return Exponent(typename Exponent::MinusOne()); }
   static Exponent getInftyExponent(const Exponent&) { return Exponent(typename Exponent::Max()); }
   static Exponent getMaxExponent(const Exponent&) { return Exponent(typename Exponent::Zero()); }
};

template <int UMaxBitsNumber, int UMantissaBitsNumber, int UExponentBitsNumber>
class TBuiltFloat
   :  public Numerics::TReadDouble<
         Numerics::TBuiltDouble<BuiltDoubleTraits<UMaxBitsNumber, UMantissaBitsNumber+1, UExponentBitsNumber> >,
         Numerics::TBuiltDouble<BuiltDoubleTraits<UMaxBitsNumber, UMantissaBitsNumber+6, UExponentBitsNumber> > > {
  private:
   typedef Numerics::TReadDouble<
      Numerics::TBuiltDouble<BuiltDoubleTraits<UMaxBitsNumber, UMantissaBitsNumber+1, UExponentBitsNumber> >,
      Numerics::TBuiltDouble<BuiltDoubleTraits<UMaxBitsNumber, UMantissaBitsNumber+6, UExponentBitsNumber> > > inherited;
   typedef TBuiltFloat<UMaxBitsNumber, UMantissaBitsNumber, UExponentBitsNumber> thisType;

  public:
   typedef Numerics::TBuiltDouble<BuiltDoubleTraits<UMaxBitsNumber, UMantissaBitsNumber+1, UExponentBitsNumber> > BaseDouble;

  public:
   typedef typename inherited::ReadParameters ReadParameters;

  public:
   TBuiltFloat() {}
   TBuiltFloat(unsigned int value) : inherited(value) {}
   TBuiltFloat(const typename inherited::IntConversion& value, ReadParameters& params)
      :  inherited(value, params) {}
   TBuiltFloat(const TFloatConversion<UMaxBitsNumber>& value, ReadParameters& params)
      :  inherited(value, params) {}
   TBuiltFloat(const thisType& source) = default;

   static const int BitSizeMantissa = inherited::BitSizeMantissa;
   static const int BitSizeExponent = inherited::BitSizeExponent;
};

inline bool
isBigEndian() {
#if defined(__GNUC__) && !defined(__MINGW32__)
#if BYTE_ORDER == BIG_ENDIAN
   return true;
#else
   return false;
#endif
#else
   int dummy = 0x1234;
   unsigned char dummy[4];
   memcpy((unsigned char*) dummy, &dummy, 4);
   return *dummy == 0x12;
#endif
}

template <class TypeBuiltDouble, typename TypeImplementation>
void
fillContent(TypeBuiltDouble& builtDouble, const TypeImplementation& source) {
   int sourceSizeInSizeofUnsigned = (sizeof(source) + sizeof(unsigned) - 1)/sizeof(unsigned);
   unsigned doubleContent[sourceSizeInSizeofUnsigned];
   doubleContent[sourceSizeInSizeofUnsigned-1] = 0;
   memcpy(&doubleContent[0], &source, sizeof(source));

   {  unsigned* mask = (unsigned*) doubleContent;
      if (isBigEndian())
         mask += sourceSizeInSizeofUnsigned-1;
      int lastCellIndex = (builtDouble.bitSizeMantissa() - 2)/(8*sizeof(unsigned));
      int mantissaIndex = lastCellIndex+1-sourceSizeInSizeofUnsigned;
      if (mantissaIndex < 0)
         mantissaIndex = 0;
      for (; mantissaIndex < lastCellIndex; ++mantissaIndex) {
         builtDouble.getSMantissa()[mantissaIndex] = *mask;
         if (isBigEndian())
            --mask;
         else
            ++mask;
      };
      int shift = (builtDouble.bitSizeMantissa()-1) % (sizeof(unsigned)*8);
      builtDouble.getSMantissa()[lastCellIndex] = (shift == 0) ? *mask
         : ((*mask) & ~((~0U) << shift));
      builtDouble.getSMantissa() <<= 1;
   }
   {  unsigned char* mask = (unsigned char*) doubleContent;
      unsigned char* signedMask = mask;
      if (!isBigEndian()) {
         signedMask += sizeof(source)-1;
         mask = signedMask;
      };
      builtDouble.setNegative((*signedMask) & 0x80);

      unsigned result = 0;
      int shift = builtDouble.bitSizeExponent()-7;
      result |= (*mask & 0x7f) << shift;
      while ((shift -= 8) >= 0) {
         if (!isBigEndian())
            --mask;
         else
            ++mask;
         result |= *mask << shift;
      };
      if (!isBigEndian())
         --mask;
      else
         ++mask;
      if (shift > -8)
         result |= *mask >> (-shift);
      builtDouble.getSBasicExponent()[0] = result;
   }
}

template <class TypeBuiltDouble, typename TypeImplementation>
void
setContent(TypeImplementation& result, const TypeBuiltDouble& builtDouble, bool isUpper) {
   unsigned doubleContent[(sizeof(result) + sizeof(unsigned) - 1)/sizeof(unsigned)];
   memset(doubleContent, 0, sizeof(unsigned)*((sizeof(result) + sizeof(unsigned) - 1)/sizeof(unsigned)));
   {  unsigned char* mask = (unsigned char*) doubleContent;
      if (!isBigEndian())
         mask += sizeof(result)-1;
      if (builtDouble.isNegative())
         *mask |= 0x80;
      int shift = builtDouble.bitSizeExponent()-7;

      unsigned exponent = builtDouble.getBasicExponent()[0];
#if defined(__GNUC__) && !defined(__clang__)
      int shiftAssert = builtDouble.bitSizeExponent();
      AssumeCondition((shiftAssert >= (int) sizeof(unsigned int)*8)
            || (((~0U << shiftAssert) & exponent) == 0))
#else
#if defined(__clang__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wshift-count-overflow"
#endif

      AssumeCondition((builtDouble.bitSizeExponent() >= sizeof(unsigned int)*8)
            || (((~0U << builtDouble.bitSizeExponent()) & exponent) == 0))
      // AssumeCondition((TypeBuiltDouble::BitSizeExponent == sizeof(unsigned int)*8)
      //       || (((~1U << (TypeBuiltDouble::BitSizeExponent-1)) & exponent) == 0))
#if defined(__clang__)
#pragma GCC diagnostic pop
#endif
#endif

      *mask |= exponent >> shift;
      while ((shift -= 8) >= 0) {
         if (!isBigEndian())
            --mask;
         else
            ++mask;
         *mask |= exponent >> shift;
      };
      if (!isBigEndian())
         --mask;
      else
         ++mask;
      if (shift > -8)
         *mask |= exponent << (-shift);
   }

   {  typename TypeBuiltDouble::Mantissa mantissa = builtDouble.getMantissa();
      bool doesAdd = mantissa.cbitArray(0) && (isUpper == builtDouble.isPositive());
      mantissa >>= 1;
      // [TODO] use conversions with a mantissa of the right size
      //   this form is not correct but the use of setContent is outsize the interval library
      if (doesAdd && mantissa.inc().hasCarry())
         mantissa.dec();
      unsigned int* mask = (unsigned int*) doubleContent;
      if (isBigEndian())
         mask += ((sizeof(result)-1)/sizeof(unsigned int));
      int lastCellIndex = (builtDouble.bitSizeMantissa() - 2)/(8*sizeof(unsigned));
      int mantissaIndex = lastCellIndex-((sizeof(result)-1)/sizeof(unsigned int));
      if (mantissaIndex < 0)
         mantissaIndex = 0;
      for (; mantissaIndex < lastCellIndex; ++mantissaIndex) {
         *mask = mantissa[mantissaIndex];
         if (isBigEndian())
            --mask;
         else
            ++mask;
      };
      int shift = (builtDouble.bitSizeMantissa()-1) % (sizeof(unsigned int)*8);
      if (shift == 0)
         *mask = mantissa[lastCellIndex];
      else
         *mask |= (mantissa[lastCellIndex] & ~((~0U) << shift));
   }
   memcpy(&result, doubleContent, sizeof(result));
}

} // end of namespace DDoubleInterval

} // end of namespace NumericalDomains

#endif // NumericalDomains_FloatIntervalBaseTypesH

