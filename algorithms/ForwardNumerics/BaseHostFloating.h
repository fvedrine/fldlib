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
// Library   : ForwardNumerics
// Unit      : Floating
// File      : BaseHostFloating.h
// Description :
//   Definition of a class of native floating points in connection
//   with soft floating points.
//

#ifndef Numerics_BaseHostFloatingH
#define Numerics_BaseHostFloatingH

#include <limits.h>
#include <float.h>
#include "ForwardNumerics/BaseFloating.h"

namespace Numerics {

class DoubleTraits {
  public:
   typedef double TypeFloat;
   static double max()              { return DBL_MAX; }
   static double normalizedMin()    { return DBL_MIN; }
   static double denormalizedMin()  { return DBL_MIN*DBL_EPSILON; }
   static bool isBigEndian() { return DDouble::Access::isBigEndian(); }
   static bool isLittleEndian() { return !DDouble::Access::isBigEndian(); }

   static const int UBitSizeMantissa = DBL_MANT_DIG-1;
   static const int UByteSizeImplementation = sizeof(TypeFloat);
   static const int UBitSizeExponent = UByteSizeImplementation*8-DBL_MANT_DIG;
   static const int UBitFullSizeExponent = UBitSizeExponent;
   static unsigned int getMaxExponent() { return DBL_MAX_EXP-1; }
   static unsigned int getZeroExponent() { return DBL_MAX_EXP-1; }
   static double epsilon() { return DBL_EPSILON; }
   static bool isValid() { return (UBitSizeExponent <= (int) sizeof(int)*8); }
};

class FloatTraits {
  public:
   typedef float TypeFloat;
   static float max()              { return FLT_MAX; }
   static float normalizedMin()    { return FLT_MIN; }
   static float denormalizedMin()  { return FLT_MIN*FLT_EPSILON; }
   static bool isBigEndian() { return DDouble::Access::isBigEndian(); }
   static bool isLittleEndian() { return !DDouble::Access::isBigEndian(); }

   static const int UBitSizeMantissa = FLT_MANT_DIG-1;
   static const int UByteSizeImplementation = sizeof(TypeFloat);
   static const int UBitSizeExponent = UByteSizeImplementation*8-FLT_MANT_DIG;
   static const int UBitFullSizeExponent = UBitSizeExponent;
   inline static unsigned int getMaxExponent() { return FLT_MAX_EXP-1; }
   inline static unsigned int getZeroExponent() { return FLT_MAX_EXP-1; }
   inline static float epsilon() { return FLT_EPSILON; }
   inline static bool isValid() { return (UBitSizeExponent <= (int) sizeof(int)*8); }
};

class LongDoubleTraits {
  public:
   typedef long double TypeFloat;
   static long double max()              { return LDBL_MAX; }
   static long double normalizedMin()    { return LDBL_MIN; }
   static long double denormalizedMin()  { return LDBL_MIN*LDBL_EPSILON; }
   static bool isBigEndian() { return DDouble::Access::isBigEndian(); }
   static bool isLittleEndian() { return !DDouble::Access::isBigEndian(); }

   static const int UBitSizeMantissa = LDBL_MANT_DIG-1;
   static const int UByteSizeImplementation = sizeof(TypeFloat);
   static const int UBitSizeExponent
      = (LDBL_MAX_EXP == (1 << (16-2))) ? 15 /* leading 1 bit */
         : UByteSizeImplementation*8-LDBL_MANT_DIG;
   static const int UBitFullSizeExponent
      = (LDBL_MAX_EXP == (1 << (16-2))) ? 16 /* leading 1 bit */
         : UByteSizeImplementation*8-LDBL_MANT_DIG;
   static unsigned int getMaxExponent() { return LDBL_MAX_EXP-1; }
   static unsigned int getZeroExponent() { return LDBL_MAX_EXP-1; }
   static long double epsilon() { return LDBL_EPSILON; }
   static bool isValid() { return (UBitSizeExponent <= (int) sizeof(int)*8); }
};

template <class TypeTraits>
class TFloatingBase {
  public:
   typedef TBuiltDouble<BuiltDoubleTraits<TypeTraits::UBitSizeMantissa, TypeTraits::UBitSizeExponent> >
      BuiltDouble;

  protected:
   typedef typename TypeTraits::TypeFloat Implementation;
   typedef TFloatingBase<TypeTraits> thisType;

  private:
   Implementation dDouble;

  public:
   TFloatingBase() : dDouble(0.0) {}
   TFloatingBase(const Implementation& implementation) : dDouble(implementation) {}
   TFloatingBase(const BuiltDouble& softDouble) : dDouble(0.0) { operator=(softDouble); }
   TFloatingBase(const thisType& source) = default;
   thisType& operator=(const thisType& source) = default;
   thisType& operator=(const BuiltDouble& softDouble);
   operator BuiltDouble() const
      {  BuiltDouble result;
         fillMantissa(result.getSMantissa());
         result.setBasicExponent(typename BuiltDouble::Exponent(typename BuiltDouble::Exponent::Basic(), result.bitSizeExponent(), getBasicExponent()));
         result.setSign(isPositive());
         return result;
	  }
   
   Implementation& implementation() { return dDouble; } 
   const Implementation& implementation() const { return dDouble; } 
   thisType queryEpsilon() const
      {  Implementation result = implementation()*TypeTraits::epsilon();
         if ((result == 0.0) && (implementation() != 0.0))
            return thisType(TypeTraits::denormalizedMin());
         return thisType((result >= 0.0) ? result : -result);
      }
   
   bool isZero() const { return (dDouble == 0.0) || (-dDouble == 0.0); }
   
   typedef DDouble::Access::ReadParameters ReadParameters;
   thisType& plusAssign(const thisType& source, const ReadParameters& params);
   thisType& minusAssign(const thisType& source, const ReadParameters& params);
   thisType& multAssign(const thisType& source, const ReadParameters& params);
   thisType& divAssign(const thisType& source, const ReadParameters& params);

   thisType& operator-=(const thisType& source)
      {  dDouble -= source.dDouble; return *this; }
   thisType& operator*=(const thisType& source)
      {  dDouble *= source.dDouble; return *this; }
   thisType& operator+=(const thisType& source)
      {  dDouble += source.dDouble; return *this; }
   thisType& operator/=(const thisType& source)
      {  dDouble /= source.dDouble; return *this; }

   static const int UByteSizeImplementation = TypeTraits::UByteSizeImplementation;
   static const int UBitSizeMantissa = TypeTraits::UBitSizeMantissa;
   static const int UBitSizeExponent = TypeTraits::UBitSizeExponent;
   typedef typename BuiltDouble::Mantissa Mantissa;
   void fillMantissa(Mantissa& mantissa) const;
   void setMantissa(const Mantissa& mantissa);
   static unsigned int getMaxExponent() { return TypeTraits::getMaxExponent(); }
   static unsigned int getZeroExponent() { return TypeTraits::getZeroExponent(); }
   thisType& opposite() { dDouble = -dDouble; return *this; }
 
   unsigned int getBasicExponent() const;
   int queryExponent() const
      {  return ((int) getBasicExponent()) - getZeroExponent(); }
   void setBasicExponent(unsigned int exponent);
   void setExponent(int exponent) { setBasicExponent(exponent+getZeroExponent()); }
   bool isPositive() const
      {  unsigned char doubleContent[UByteSizeImplementation];
         memcpy(doubleContent, &dDouble, UByteSizeImplementation);
         unsigned char* mask = (unsigned char*) doubleContent;
         if (TypeTraits::isLittleEndian())
            mask += UByteSizeImplementation-1;
         return !((*mask) & 0x80);
      }
   bool isNegative() const { return !isPositive(); }
   void setSign(bool isPositive)
      {  unsigned char doubleContent[UByteSizeImplementation];
         memcpy(doubleContent, &dDouble, UByteSizeImplementation);
         unsigned char* mask = (unsigned char*) doubleContent;
         if (TypeTraits::isLittleEndian())
            mask += UByteSizeImplementation-1;
         if (isPositive)
            *mask &= 0x7f;
         else
            *mask |= 0x80;
         memcpy(&dDouble, doubleContent, UByteSizeImplementation);
      }
   void setPositive() { setSign(true); }
   void setNegative() { setSign(false); }
   bool isNormalised() const
      {  return ((TypeTraits::normalizedMin() <= dDouble) && (dDouble <= TypeTraits::max()))
            || ((-TypeTraits::normalizedMin() >= dDouble) && (dDouble >= -TypeTraits::max()));
      }
   bool isRanged() const
      {  return ((TypeTraits::denormalizedMin() <= dDouble) && (dDouble <= TypeTraits::max()))
            || ((-TypeTraits::denormalizedMin() >= dDouble) && (dDouble >= -TypeTraits::max()));
      }
   void clear() { dDouble = 0.0; }
   void swap(thisType& source)
      {  Implementation temp = dDouble;
         dDouble = source.dDouble;
         source.dDouble = temp;
      }
};

template <class TypeTraits>
inline TFloatingBase<TypeTraits>&
TFloatingBase<TypeTraits>::plusAssign(const thisType& source, const ReadParameters& params) {  
#ifdef _WIN32
   int oldState = _controlfp(0, _MCW_RC);
   if (params.isLowestRound())
      _controlfp(_RC_DOWN, _MCW_RC);
   else if (params.isHighestRound())
      _controlfp(_RC_UP, _MCW_RC);
   else if (params.isNearestRound())
      _controlfp(_RC_NEAR, _MCW_RC);
   else // (params.isZeroRound())
      _controlfp(_RC_CHOP, _MCW_RC);
   dDouble += source.dDouble;
   _controlfp(oldState, _MCW_RC);
#else
#if defined(__SUN__)
   if (params.isLowestRound())
      fpsetround(FP_RM);
   else if (params.isHighestRound())
      fpsetround(FP_RP);
   else if (params.isNearestRound())
      fpsetround(FP_RN);
   else // (params.isZeroRound())
      fpsetround(FP_RZ);
   dDouble += source.dDouble;
   fpsetround(FP_RN);
#elif defined(__LINUX__)
   if (params.isLowestRound())
      fesetround(FE_DOWNWARD);
   else if (params.isHighestRound())
      fesetround(FE_UPWARD);
   else if (params.isNearestRound())
      fesetround(FE_TONEAREST);
   else // (params.isZeroRound())
      fesetround(FE_TOWARDZERO);
   dDouble += source.dDouble;
   fesetround(FE_TONEAREST);
#else
   dDouble += source.dDouble;
#endif
#endif
   return *this;
}

template <class TypeTraits>
inline TFloatingBase<TypeTraits>&
TFloatingBase<TypeTraits>::minusAssign(const thisType& source, const ReadParameters& params) {  
#ifdef _WIN32
   int oldState = _controlfp(0, _MCW_RC);
   if (params.isLowestRound())
      _controlfp(_RC_DOWN, _MCW_RC);
   else if (params.isHighestRound())
      _controlfp(_RC_UP, _MCW_RC);
   else if (params.isNearestRound())
      _controlfp(_RC_NEAR, _MCW_RC);
   else // (params.isZeroRound())
      _controlfp(_RC_CHOP, _MCW_RC);
   dDouble -= source.dDouble;
   _controlfp(oldState, _MCW_RC);
#else
#if defined(__SUN__)
   if (params.isLowestRound())
      fpsetround(FP_RM);
   else if (params.isHighestRound())
      fpsetround(FP_RP);
   else if (params.isNearestRound())
      fpsetround(FP_RN);
   else // (params.isZeroRound())
      fpsetround(FP_RZ);
   dDouble -= source.dDouble;
   fpsetround(FP_RN);
#elif defined(__LINUX__)
   if (params.isLowestRound())
      fesetround(FE_DOWNWARD);
   else if (params.isHighestRound())
      fesetround(FE_UPWARD);
   else if (params.isNearestRound())
      fesetround(FE_TONEAREST);
   else // (params.isZeroRound())
      fesetround(FE_TOWARDZERO);
   dDouble -= source.dDouble;
   fesetround(FE_TONEAREST);
#else
   dDouble -= source.dDouble;
#endif
#endif
   return *this;
}

template <class TypeTraits>
inline TFloatingBase<TypeTraits>&
TFloatingBase<TypeTraits>::multAssign(const thisType& source, const ReadParameters& params) {  
#ifdef _WIN32
   int oldState = _controlfp(0, _MCW_RC);
   if (params.isLowestRound())
      _controlfp(_RC_DOWN, _MCW_RC);
   else if (params.isHighestRound())
      _controlfp(_RC_UP, _MCW_RC);
   else if (params.isNearestRound())
      _controlfp(_RC_NEAR, _MCW_RC);
   else // (params.isZeroRound())
      _controlfp(_RC_CHOP, _MCW_RC);
   dDouble *= source.dDouble;
   _controlfp(oldState, _MCW_RC);
#else
#if defined(__SUN__)
   if (params.isLowestRound())
      fpsetround(FP_RM);
   else if (params.isHighestRound())
      fpsetround(FP_RP);
   else if (params.isNearestRound())
      fpsetround(FP_RN);
   else // (params.isZeroRound())
      fpsetround(FP_RZ);
   dDouble *= source.dDouble;
   fpsetround(FP_RN);
#elif defined(__LINUX__)
   if (params.isLowestRound())
      fesetround(FE_DOWNWARD);
   else if (params.isHighestRound())
      fesetround(FE_UPWARD);
   else if (params.isNearestRound())
      fesetround(FE_TONEAREST);
   else // (params.isZeroRound())
      fesetround(FE_TOWARDZERO);
   dDouble *= source.dDouble;
   fesetround(FE_TONEAREST);
#else
   dDouble *= source.dDouble;
#endif
#endif
   return *this;
}

template <class TypeTraits>
inline TFloatingBase<TypeTraits>&
TFloatingBase<TypeTraits>::divAssign(const thisType& source, const ReadParameters& params) {  
#ifdef _WIN32
   int oldState = _controlfp(0, _MCW_RC);
   if (params.isLowestRound())
      _controlfp(_RC_DOWN, _MCW_RC);
   else if (params.isHighestRound())
      _controlfp(_RC_UP, _MCW_RC);
   else if (params.isNearestRound())
      _controlfp(_RC_NEAR, _MCW_RC);
   else // (params.isZeroRound())
      _controlfp(_RC_CHOP, _MCW_RC);
   dDouble /= source.dDouble;
   _controlfp(oldState, _MCW_RC);
#else
#if defined(__SUN__)
   if (params.isLowestRound())
      fpsetround(FP_RM);
   else if (params.isHighestRound())
      fpsetround(FP_RP);
   else if (params.isNearestRound())
      fpsetround(FP_RN);
   else // (params.isZeroRound())
      fpsetround(FP_RZ);
   dDouble /= source.dDouble;
   fpsetround(FP_RN);
#elif defined(__LINUX__)
   if (params.isLowestRound())
      fesetround(FE_DOWNWARD);
   else if (params.isHighestRound())
      fesetround(FE_UPWARD);
   else if (params.isNearestRound())
      fesetround(FE_TONEAREST);
   else // (params.isZeroRound())
      fesetround(FE_TOWARDZERO);
   dDouble /= source.dDouble;
   fesetround(FE_TONEAREST);
#else
   dDouble /= source.dDouble;
#endif
#endif
   return *this;
}

template <class FloatingBaseTraits>
class TDoubleElement : public FloatingBaseTraits, public DDouble::Access {
  private:
   typedef FloatingBaseTraits inherited;
   typedef TDoubleElement<FloatingBaseTraits> thisType;

   typedef typename FloatingBaseTraits::Mantissa Mantissa;

  public:
   typedef typename inherited::BuiltDouble BuiltDouble;
   typedef DDouble::Access::ReadParameters ReadParameters;
   TDoubleElement() : inherited() {}
   TDoubleElement(int value, ReadParameters& params)
      {  typename BuiltDouble::IntConversion conversionValue;
         conversionValue.setSigned().assign(value);
         BuiltDouble result(conversionValue, params);
         inherited::operator=(result);
      }
   TDoubleElement(typename FloatingBaseTraits::Implementation source) : inherited(source) {}
   TDoubleElement(const BuiltDouble& softDouble) : inherited(softDouble) {}
   TDoubleElement(const thisType& source) = default;

   int queryInteger(ReadParameters& params) const
      {  typename BuiltDouble::IntConversion result;
         BuiltDouble(*this).retrieveInteger(result.setSigned(), params);
         return result.asInt();
      }
   unsigned int queryUnsignedInteger(ReadParameters& params) const
      {  typename BuiltDouble::IntConversion result;
         BuiltDouble(*this).retrieveInteger(result.setUnsigned(), params);
         return result.asUnsignedInt();
      }

   bool isInternZero() const;
   bool isZero() const { return inherited::isZero(); }
   bool isNaN() const;
   bool isQNaN() const;
   bool isSNaN() const;
   bool isInfty() const;
   bool hasInftyExponent() const
      {  return inherited::getBasicExponent() == inherited::getMaxExponent()+inherited::getZeroExponent()+1; }

   enum ComparisonResult { CRNaN, CRLess, CREqual, CRGreater };
   ComparisonResult compare(const thisType& source) const
      {  ComparisonResult result = CRNaN;
         if (!isNaN() && !source.isNaN())
            result = (inherited::implementation() < source.inherited::implementation()) ? CRLess
               : ((inherited::implementation() > source.inherited::implementation()) ? CRGreater : CREqual);
         return result;
      }
   bool operator==(const thisType& source) const { return compare(source) == CREqual; }
   bool operator!=(const thisType& source) const
      {  ComparisonResult result = compare(source);
         return (result == CRLess) || (result == CRGreater);
      }
   bool operator<(const thisType& source) const { return compare(source) == CRLess; }
   bool operator>(const thisType& source) const { return compare(source) == CRGreater; }
   bool operator<=(const thisType& source) const
      {  ComparisonResult result = compare(source);
         return (result == CRLess) || (result == CREqual);
      }
   bool operator>=(const thisType& source) const
      {  ComparisonResult result = compare(source);
         return (result == CRGreater) || (result == CREqual);
      }

   static const int UByteSizeImplementation = inherited::UByteSizeImplementation;
   static const int UBitSizeMantissa = inherited::UBitSizeMantissa;
   static const int UBitSizeExponent = inherited::UBitSizeExponent;

   typedef typename BuiltDouble::DiffDouble DiffDouble;
   DiffDouble queryNumberOfFloatsBetween(const thisType& element) const
      {  BuiltDouble softThis = *this, softElement = element;
         return softThis.queryNumberOfFloatsBetween(softElement);
      }
   thisType queryNthSuccessor(const DiffDouble& intSource) const
      {  BuiltDouble softThis = *this;
         return thisType(softThis.queryNthSuccessor(intSource));
      }
   thisType queryNthPredecessor(const DiffDouble& intSource) const
      {  BuiltDouble softThis = *this;
         return thisType(softThis.queryNthPredecessor(intSource));
      }
   bool isPuiss2() const;
   DoubleErrors queryErrors() const;

   thisType& opposite() { return (thisType&) inherited::opposite(); }
   thisType& plusAssign(const thisType& source, ReadParameters& params)
      {  return (thisType&) inherited::plusAssign(source, params); }
   thisType& minusAssign(const thisType& source, ReadParameters& params)
      {  return (thisType&) inherited::minusAssign(source, params); }
   thisType& multAssign(const thisType& source, ReadParameters& params)
      {  return (thisType&) inherited::multAssign(source, params); }
   thisType& divAssign(const thisType& source, ReadParameters& params)
      {  return (thisType&) inherited::divAssign(source, params); }

   thisType& operator-=(const thisType& source)
      {  return (thisType&) inherited::operator-=(source); }
   thisType& operator*=(const thisType& source)
      {  return (thisType&) inherited::operator*=(source); }
   thisType& operator+=(const thisType& source)
      {  return (thisType&) inherited::operator+=(source); }
   thisType& operator/=(const thisType& source)
      {  return (thisType&) inherited::operator/=(source); }

   thisType operator-(const thisType& source) const
      {  thisType result(*this);
         return (result -= source);
      }
   thisType operator+(const thisType& source) const
      {  thisType result(*this);
         return (result += source);
      }
   thisType operator*(const thisType& source) const
      {  thisType result(*this);
         return (result *= source);
      }
   thisType operator/(const thisType& source) const
      {  thisType result(*this);
         return (result /= source);
      }

   thisType& setOpposite()
      {  inherited::setSign(!inherited::isPositive());
         return *this;
      }
   thisType& setInftyExponent()
      {  inherited::setBasicExponent(~0U);
         return *this;
      }

   void write(STG::IOObject::OSBase& out, WriteParameters& params) const;
   void read(STG::IOObject::ISBase& in, ReadParameters& params);
};

} // end of namespace Numerics

#endif // Numerics_BaseHostFloatingH

