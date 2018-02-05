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
// File      : FloatIntervalBase.h
// Description :
//   Definition of a class of floating point intervals
//

#ifndef NumericalDomains_FloatIntervalBaseH
#define NumericalDomains_FloatIntervalBaseH

#include "NumericalLattices/FloatIntervalBaseTypes.h"
#include <fstream>
#include <sstream>

namespace NumericalDomains { namespace DDoubleInterval {

/*********************************************************************/
/* Contract for the first template argument of TCompareFloatInterval */
/*********************************************************************/

template <class TDiagnosisTraits>
class TBaseFloatIntervalContract {
  public:
   static void initializeGlobals(const char* fileSuffix) {}
   static void finalizeGlobals() {}
   class Initialization {
     public:
      Initialization(const char* fileSuffix) { initializeGlobals(fileSuffix); }
      ~Initialization() { finalizeGlobals(); }
   };

   template <class TCompareFloatInterval>
   void notifyForCompare(const TCompareFloatInterval& source) const {}
   template <class TCompareFloatInterval>
   void notifyForBranchCompare(const TCompareFloatInterval& source) const {}
   template <class TCompareFloatInterval>
   void notifyForDivisionByZero(const TCompareFloatInterval& source) const {}
   template <class TCompareFloatInterval>
   void notifyForNegativeSqrt(const TCompareFloatInterval& source) const {}
   template <class TCompareFloatInterval>
   void notifyForNegativePow(const TCompareFloatInterval& source) const {}
   template <class TCompareFloatInterval>
   void notifyForNegativeOrNulLog(const TCompareFloatInterval& source) const {}

  protected:
   bool getThenBranch(bool executionResult) const { return executionResult; }
   bool getConversionBranch(unsigned diff, unsigned executionResult) const { return executionResult; }
   bool doesFollow() const { return false; }

  public:
   Numerics::DDouble::Access::ReadParameters& minParams() const
      {  return *(Numerics::DDouble::Access::ReadParameters*) nullptr; }
   Numerics::DDouble::Access::ReadParameters& maxParams() const
      {  return *(Numerics::DDouble::Access::ReadParameters*) nullptr; }
   void persist() {}
};

template <int UMaxBitsNumber, class TypeBaseFloatInterval, class TypeBuiltDouble, typename TypeImplementation>
class TCompareFloatInterval : public TypeBaseFloatInterval {
  private:
   typedef TCompareFloatInterval<UMaxBitsNumber, TypeBaseFloatInterval, TypeBuiltDouble, TypeImplementation> thisType;
   typedef TypeBaseFloatInterval inherited;

   TypeImplementation dValue;
   TypeBuiltDouble bfMin;
   TypeBuiltDouble bfMax;

   void adjustMinMax(TypeImplementation value);
   void adjustMin(TypeImplementation value);
   void adjustMax(TypeImplementation value);

   template <int MaxBitsNumberArgument, class TypeBaseFloatIntervalArgument, class TypeBuiltDoubleArgument, typename TypeImplementationArgument>
   friend class TCompareFloatInterval;

  public:
   void initFrom(TypeImplementation value);
   void initFromAtomic(TypeImplementation value);

  protected:
   TypeBuiltDouble& smin() { return bfMin; }
   TypeBuiltDouble& smax() { return bfMax; }

  public:
   typedef TypeBuiltDouble BuiltDouble;
   void retrieveRelativeError(TypeBuiltDouble& result) const;

   const TypeBuiltDouble& min() const { return bfMin; }
   const TypeBuiltDouble& max() const { return bfMax; }

  public:
   TCompareFloatInterval() : dValue(0) {}
   TCompareFloatInterval(const thisType& source) = default;
   TCompareFloatInterval(TypeImplementation min, TypeImplementation max);
   TCompareFloatInterval(int value);
   TCompareFloatInterval(long int value);
   TCompareFloatInterval(unsigned value);
   TCompareFloatInterval(unsigned long value);
   TCompareFloatInterval(thisType&& source) = default; // [TODO] keep symbolic for constraints
   TCompareFloatInterval& operator=(const thisType& source) = default;
   TCompareFloatInterval& operator=(thisType&& source) = default; // [TODO] keep symbolic for constraints

   void mergeWith(const thisType& source)
      {  dValue = source.dValue;
         if (bfMin > source.bfMin)
            bfMin = source.bfMin;
         if (bfMax < source.bfMax)
            bfMax = source.bfMax;
      }
   template <class TypeBuiltArgument, typename TypeImplementationArgument>
   TCompareFloatInterval(const TCompareFloatInterval<UMaxBitsNumber, TypeBaseFloatInterval, TypeBuiltArgument, TypeImplementationArgument>& source);
   template <class TypeBuiltArgument, typename TypeImplementationArgument>
   TCompareFloatInterval& operator=(const TCompareFloatInterval<UMaxBitsNumber, TypeBaseFloatInterval, TypeBuiltArgument, TypeImplementationArgument>& source)
      {  return operator=(thisType(source)); }

   bool operator<(const thisType& source) const;
   bool operator<=(const thisType& source) const;
   bool operator==(const thisType& source) const;
   bool operator!=(const thisType& source) const;
   bool operator>=(const thisType& source) const;
   bool operator>(const thisType& source) const;

   thisType& operator+=(const thisType& source);
   thisType& operator-=(const thisType& source);
   thisType& operator*=(const thisType& source);
   thisType& operator/=(const thisType& source);

   void oppositeAssign()
      {  dValue = -dValue;
         bfMin.swap(bfMax);
         bfMin.opposite();
         bfMax.opposite();
         inherited::notifyForCompare(*this);
      }

   typedef TypeImplementation ImplementationType;
   TypeImplementation asImplementation() const { return dValue; }
   typedef Numerics::DDouble::Access::ReadParameters ReadParametersBase;
   // roundMode=RMZero for C conversions, roundMode=RMNearest for Ada conversions, roundMode=RMLowest for floor
   int asInt(ReadParametersBase::RoundMode roundMode=ReadParametersBase::RMZero) const;
   unsigned asUnsigned(ReadParametersBase::RoundMode roundMode=ReadParametersBase::RMLowest) const;
   long int asLongInt(ReadParametersBase::RoundMode roundMode=ReadParametersBase::RMZero) const
      {  return asInt(roundMode); }
   unsigned long asUnsignedLong(ReadParametersBase::RoundMode roundMode=ReadParametersBase::RMLowest) const
      {  return asUnsigned(roundMode); }

   void sqrtAssign();
   void sinAssign();
   void cosAssign();
   void asinAssign();
   void acosAssign();
   void tanAssign();
   void atanAssign();
   void expAssign();
   void logAssign();
   void log10Assign();
   void powAssign(const thisType& value);
   void absAssign();
};

template <int UMaxBitsNumber, class TypeBaseFloatInterval, class TypeBuiltDouble, typename TypeImplementation>
inline void
TCompareFloatInterval<UMaxBitsNumber, TypeBaseFloatInterval, TypeBuiltDouble, TypeImplementation>::adjustMinMax(TypeImplementation value) {
   if (value < 0)
      value = -value;
   if (value > 0 && !bfMin.isInfty()) {
      bool isMinApproximate, isMaxApproximate;
      unsigned shiftCount = bfMin.BitSizeMantissa/2;
      if (shiftCount < 8*sizeof(unsigned)) {
         isMinApproximate = (bfMin.getMantissa()[0] & ~(~0U << shiftCount)) != 0;
         isMaxApproximate = (bfMax.getMantissa()[0] & ~(~0U << shiftCount)) != 0;
      }
      else {
         isMinApproximate = bfMin.getMantissa()[0] != 0;
         isMaxApproximate = bfMax.getMantissa()[0] != 0;
      }
      if (isMinApproximate) {
         if (bfMin.getSMantissa().dec().hasCarry()) {
            if (bfMin.getSBasicExponent().dec().hasCarry())
               bfMin.setZero();
         };
      };
      if (isMaxApproximate) {
         if (bfMax.getSMantissa().inc().hasCarry()) {
            bfMax.getSBasicExponent().inc();
            if (bfMax.isInfty() && inherited::maxParams().isInftyAvoided()) {
               bfMax.getSMantissa().dec();
               bfMax.getSBasicExponent().dec();
            };
         };
      };
   };
}

template <int UMaxBitsNumber, class TypeBaseFloatInterval, class TypeBuiltDouble, typename TypeImplementation>
inline void
TCompareFloatInterval<UMaxBitsNumber, TypeBaseFloatInterval, TypeBuiltDouble, TypeImplementation>::initFrom(TypeImplementation value) {
   dValue = value;
   DDoubleInterval::fillContent(bfMin, value);
   DDoubleInterval::fillContent(bfMax, value);
   adjustMinMax(value);
   inherited::notifyForCompare(*this);
}

template <int UMaxBitsNumber, class TypeBaseFloatInterval, class TypeBuiltDouble, typename TypeImplementation>
inline void
TCompareFloatInterval<UMaxBitsNumber, TypeBaseFloatInterval, TypeBuiltDouble, TypeImplementation>::initFromAtomic(TypeImplementation value) {
   dValue = value;
   DDoubleInterval::fillContent(bfMin, value);
   DDoubleInterval::fillContent(bfMax, value);
   inherited::notifyForCompare(*this);
}

template <int UMaxBitsNumber, class TypeBaseFloatInterval, class TypeBuiltDouble, typename TypeImplementation>
template <class TypeBuiltArgument, typename TypeImplementationArgument>
inline
TCompareFloatInterval<UMaxBitsNumber, TypeBaseFloatInterval, TypeBuiltDouble, TypeImplementation>::TCompareFloatInterval(
      const TCompareFloatInterval<UMaxBitsNumber, TypeBaseFloatInterval, TypeBuiltArgument, TypeImplementationArgument>& source)
   :  dValue(source.dValue) {
   auto& minParams = inherited::minParams();
   auto& maxParams = inherited::maxParams();
   typename inherited::FloatConversion conversion;
   conversion.setSizeMantissa(source.bfMin.BitSizeMantissa).setSizeExponent(source.bfMin.BitSizeExponent);
   int sizeMantissa = (source.bfMin.BitSizeMantissa + 8*sizeof(unsigned) - 1)/(8*sizeof(unsigned));
   for (int index = 0; index < sizeMantissa; ++index)
      conversion.mantissa()[index] = source.bfMin.getMantissa()[index];
   int sizeExponent = (source.bfMin.BitSizeExponent + 8*sizeof(unsigned) - 1)/(8*sizeof(unsigned));
   for (int index = 0; index < sizeExponent; ++index)
      conversion.exponent()[index] = source.bfMin.getBasicExponent()[index];
   conversion.setNegative(source.bfMin.isNegative());
   bfMin.setFloat(conversion, minParams);
   minParams.clear();
   for (int index = 0; index < sizeMantissa; ++index)
      conversion.mantissa()[index] = source.bfMax.getMantissa()[index];
   for (int index = 0; index < sizeExponent; ++index)
      conversion.exponent()[index] = source.bfMax.getBasicExponent()[index];
   conversion.setNegative(source.bfMax.isNegative());
   bfMax.setFloat(conversion, maxParams);
   maxParams.clear();
   inherited::notifyForCompare(*this);
}

template <int UMaxBitsNumber, class TypeBaseFloatInterval, class TypeBuiltDouble, typename TypeImplementation>
inline void
TCompareFloatInterval<UMaxBitsNumber, TypeBaseFloatInterval, TypeBuiltDouble, TypeImplementation>::adjustMin(TypeImplementation min) {
   if (min < 0)
      min = -min;
   if (min > 0 && !bfMin.isInfty()) {
      bool isApproximate;
      unsigned shiftCount = bfMin.BitSizeMantissa/2;
      if (shiftCount < 8*sizeof(unsigned)) {
         isApproximate = (bfMin.getMantissa()[0] & ~(~0U << shiftCount)) != 0;
      }
      else
         isApproximate = bfMin.getMantissa()[0] != 0;
      if (isApproximate) {
         if (bfMin.getSMantissa().dec().hasCarry()) {
            if (bfMin.getSBasicExponent().dec().hasCarry())
               bfMin.setZero();
         };
      };
   };
}

template <int UMaxBitsNumber, class TypeBaseFloatInterval, class TypeBuiltDouble, typename TypeImplementation>
inline void
TCompareFloatInterval<UMaxBitsNumber, TypeBaseFloatInterval, TypeBuiltDouble, TypeImplementation>::adjustMax(TypeImplementation max) {
   if (max < 0)
      max = -max;
   if (max > 0 && !bfMax.isInfty()) {
      bool isApproximate;
      unsigned shiftCount = bfMin.BitSizeMantissa/2;
      if (shiftCount < 8*sizeof(unsigned)) {
         isApproximate = (bfMax.getMantissa()[0] & ~(~0U << shiftCount)) != 0;
      }
      else
         isApproximate = bfMax.getMantissa()[0] != 0;
      if (isApproximate) {
         if (bfMax.getSMantissa().inc().hasCarry()) {
            bfMax.getSBasicExponent().inc();
            if (bfMax.isInfty() && inherited::maxParams().isInftyAvoided()) {
               bfMax.getSMantissa().dec();
               bfMax.getSBasicExponent().dec();
            };
         };
      };
   };
}

template <int UMaxBitsNumber, class TypeBaseFloatInterval, class TypeBuiltDouble, typename TypeImplementation>
inline
TCompareFloatInterval<UMaxBitsNumber, TypeBaseFloatInterval, TypeBuiltDouble, TypeImplementation>::TCompareFloatInterval(
      TypeImplementation min, TypeImplementation max)
   :  dValue((min + max)/2) {
   DDoubleInterval::fillContent(bfMin, min);
   DDoubleInterval::fillContent(bfMax, max);
   adjustMin(min);
   adjustMax(max);
   inherited::notifyForCompare(*this);
}

template <int UMaxBitsNumber, class TypeBaseFloatInterval, class TypeBuiltDouble, typename TypeImplementation>
inline
TCompareFloatInterval<UMaxBitsNumber, TypeBaseFloatInterval, TypeBuiltDouble, TypeImplementation>::TCompareFloatInterval(int value)
   :  dValue(value) {
   auto& minParams = inherited::minParams();
   auto& maxParams = inherited::maxParams();
   typename TypeBuiltDouble::IntConversion conversion;
   conversion.setSigned();
   conversion.assign(value);
   bfMin.setInteger(conversion, minParams);
   minParams.clear();
   bfMax.setInteger(conversion, maxParams);
   maxParams.clear();
   inherited::notifyForCompare(*this);
}

template <int UMaxBitsNumber, class TypeBaseFloatInterval, class TypeBuiltDouble, typename TypeImplementation>
inline
TCompareFloatInterval<UMaxBitsNumber, TypeBaseFloatInterval, TypeBuiltDouble, TypeImplementation>::TCompareFloatInterval(long int value)
   :  dValue(value) { // [TODO] to improve
   auto& minParams = inherited::minParams();
   auto& maxParams = inherited::maxParams();
   typename TypeBuiltDouble::IntConversion conversion;
   conversion.setSigned();
   conversion.assign((int) value);
   bfMin.setInteger(conversion, minParams);
   minParams.clear();
   bfMax.setInteger(conversion, maxParams);
   maxParams.clear();
   inherited::notifyForCompare(*this);
}

template <int UMaxBitsNumber, class TypeBaseFloatInterval, class TypeBuiltDouble, typename TypeImplementation>
inline
TCompareFloatInterval<UMaxBitsNumber, TypeBaseFloatInterval, TypeBuiltDouble, TypeImplementation>::TCompareFloatInterval(unsigned value)
   :  dValue(value) {
   auto& minParams = inherited::minParams();
   auto& maxParams = inherited::maxParams();
   typename TypeBuiltDouble::IntConversion conversion;
   conversion.setUnsigned();
   conversion.assign(value);
   bfMin.setInteger(conversion, minParams);
   minParams.clear();
   bfMax.setInteger(conversion, maxParams);
   maxParams.clear();
   inherited::notifyForCompare(*this);
}

template <int UMaxBitsNumber, class TypeBaseFloatInterval, class TypeBuiltDouble, typename TypeImplementation>
inline
TCompareFloatInterval<UMaxBitsNumber, TypeBaseFloatInterval, TypeBuiltDouble, TypeImplementation>::TCompareFloatInterval(unsigned long value)
   :  dValue(value) { // [TODO] to improve
   auto& minParams = inherited::minParams();
   auto& maxParams = inherited::maxParams();
   typename TypeBuiltDouble::IntConversion conversion;
   conversion.setUnsigned();
   conversion.assign((unsigned) value);
   bfMin.setInteger(conversion, minParams);
   minParams.clear();
   bfMax.setInteger(conversion, maxParams);
   maxParams.clear();
   inherited::notifyForCompare(*this);
}

template <int UMaxBitsNumber, class TypeBaseFloatInterval, class TypeBuiltDouble, typename TypeImplementation>
inline TCompareFloatInterval<UMaxBitsNumber, TypeBaseFloatInterval, TypeBuiltDouble, TypeImplementation>&
TCompareFloatInterval<UMaxBitsNumber, TypeBaseFloatInterval, TypeBuiltDouble, TypeImplementation>::operator+=(const thisType& source) {
   dValue += source.dValue;
   auto& minParams = inherited::minParams();
   auto& maxParams = inherited::maxParams();
   bfMin.plusAssign(source.bfMin, minParams);
   minParams.clear();
   bfMax.plusAssign(source.bfMax, maxParams);
   maxParams.clear();
   inherited::notifyForCompare(*this);
   return *this;
}
   
template <int UMaxBitsNumber, class TypeBaseFloatInterval, class TypeBuiltDouble, typename TypeImplementation>
inline TCompareFloatInterval<UMaxBitsNumber, TypeBaseFloatInterval, TypeBuiltDouble, TypeImplementation>&
TCompareFloatInterval<UMaxBitsNumber, TypeBaseFloatInterval, TypeBuiltDouble, TypeImplementation>::operator-=(const thisType& source) {
   dValue -= source.dValue;
   auto& minParams = inherited::minParams();
   auto& maxParams = inherited::maxParams();
   bfMin.minusAssign(source.bfMax, minParams);
   minParams.clear();
   bfMax.minusAssign(source.bfMin, maxParams);
   maxParams.clear();
   inherited::notifyForCompare(*this);
   return *this;
}
         
template <int UMaxBitsNumber, class TypeBaseFloatInterval, class TypeBuiltDouble, typename TypeImplementation>
inline void
TCompareFloatInterval<UMaxBitsNumber, TypeBaseFloatInterval, TypeBuiltDouble, TypeImplementation>::sqrtAssign() {
   bool hasNegativeSqrt = bfMin.isNegative() && !bfMin.isZero();
   if (hasNegativeSqrt) {
      bfMin = 0;
      if (bfMax.isNegative() && !bfMax.isZero())
         bfMax = 0;
   };

   TypeImplementation min, max;
   DDoubleInterval::setContent(min, bfMin, false /* isUpper */);
   DDoubleInterval::setContent(max, bfMax, true /* isUpper */);
   min = ::sqrt(min);
   max = ::sqrt(max);

   DDoubleInterval::fillContent(bfMin, min);
   DDoubleInterval::fillContent(bfMax, max);
   adjustMin(min);
   adjustMax(max);
   if (hasNegativeSqrt)
      inherited::notifyForNegativeSqrt(*this);

   dValue = ::sqrt((double) dValue);
   inherited::notifyForCompare(*this);
}

template <int UMaxBitsNumber, class TypeBaseFloatInterval, class TypeBuiltDouble, typename TypeImplementation>
inline void
TCompareFloatInterval<UMaxBitsNumber, TypeBaseFloatInterval, TypeBuiltDouble, TypeImplementation>::absAssign() {
   if (bfMin.isNegative()) {
      if (bfMax.isPositive()) {
         bfMin.opposite();
         if (bfMin > bfMax)
            bfMin.swap(bfMax);
         bfMin.setZero();
      }
      else {
         bfMin.opposite();
         bfMax.opposite();
         bfMin.swap(bfMax);
      };
   };

   if (dValue < 0)
      dValue = -dValue;
   // dValue = ::fabs((double) dValue);
   inherited::notifyForCompare(*this);
}

} // end of namespace DDoubleInterval

} // end of namespace NumericalDomains

#endif // NumericalDomains_FloatIntervalBaseH

