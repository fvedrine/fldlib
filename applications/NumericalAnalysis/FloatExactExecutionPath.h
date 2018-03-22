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
// File      : FloatExactExecutionPath.h
// Description :
//   Definition of a class of floating point comparison
//

#ifndef NumericalAnalysis_FloatExactExecutionPathH
#define NumericalAnalysis_FloatExactExecutionPathH

#include <cfloat>

#include "config.h"
#if !defined(FLOAT_GENERIC_BASE_UNSIGNED) && !defined(FLOAT_GENERIC_BASE_LONG)
#include "NumericalDomains/FloatExactBase.h"
#else
#include "NumericalDomains/FloatExactBaseGeneric.h"
#include "Numerics/Integer.h"
#endif

namespace NumericalDomains {

namespace DDoubleExact {

namespace DFloatDigitsHelper {
   template <typename TypeImplementation>
   class TFloatDigits {
     public:
      static const int UBitSizeMantissa=0;
      static const int UBitSizeExponent=0;
      static const int UBitFullSizeExponent=0;
   };

   template <>
   class TFloatDigits<float> {
     public:
      static const int UBitSizeMantissa=FLT_MANT_DIG-1;
      static const int UBitSizeExponent=sizeof(float)*8-FLT_MANT_DIG;
      static const int UBitFullSizeExponent=UBitSizeExponent;
   };

   template <>
   class TFloatDigits<double> {
     public:
      static const int UBitSizeMantissa=DBL_MANT_DIG-1;
      static const int UBitSizeExponent=sizeof(double)*8-DBL_MANT_DIG;
      static const int UBitFullSizeExponent=UBitSizeExponent;
   };

   template <>
   class TFloatDigits<long double> {
     public:
      static const int UBitSizeMantissa=LDBL_MANT_DIG-1;
      static const int UBitSizeExponent
         = (LDBL_MAX_EXP == (1 << (16-2))) ? 15 /* leading 1 bit */
            : sizeof(long double)*8-LDBL_MANT_DIG;
      static const int UBitFullSizeExponent
         = (LDBL_MAX_EXP == (1 << (16-2))) ? 16 /* leading 1 bit */
            : sizeof(long double)*8-LDBL_MANT_DIG;
   };
};

class FloatDigitsHelper {
  public:
   template <typename TypeImplementation>
   class TFloatDigits : public DFloatDigitsHelper::TFloatDigits<TypeImplementation> {};
};

#ifndef FLOAT_REAL_BITS_NUMBER
#define FLOAT_REAL_BITS_NUMBER 123
#endif

#if !defined(FLOAT_GENERIC_BASE_UNSIGNED) && !defined(FLOAT_GENERIC_BASE_LONG)
typedef TBuiltReal<FLOAT_REAL_BITS_NUMBER> BuiltReal;
typedef TBuiltFloat<FLOAT_REAL_BITS_NUMBER, 23, 8> BuiltFloat;
typedef TBuiltFloat<FLOAT_REAL_BITS_NUMBER, 52, 11> BuiltDouble;
typedef TBuiltFloat<FLOAT_REAL_BITS_NUMBER,
      FloatDigitsHelper::TFloatDigits<long double>::UBitSizeMantissa, FloatDigitsHelper::TFloatDigits<long double>::UBitSizeExponent> BuiltLongDouble;
#elif defined(FLOAT_GENERIC_BASE_LONG)
typedef TGBuiltReal<Numerics::UnsignedLongBaseStoreTraits, FLOAT_REAL_BITS_NUMBER> BuiltReal;
typedef TGBuiltFloat<Numerics::UnsignedLongBaseStoreTraits, FLOAT_REAL_BITS_NUMBER, 23, 8> BuiltFloat;
typedef TGBuiltFloat<Numerics::UnsignedLongBaseStoreTraits, FLOAT_REAL_BITS_NUMBER, 52, 11> BuiltDouble;
typedef TGBuiltFloat<Numerics::UnsignedLongBaseStoreTraits, FLOAT_REAL_BITS_NUMBER,
      FloatDigitsHelper::TFloatDigits<long double>::UBitSizeMantissa, FloatDigitsHelper::TFloatDigits<long double>::UBitSizeExponent> BuiltLongDouble;
#else // defined(FLOAT_GENERIC_BASE_UNSIGNED)
typedef TGBuiltReal<Numerics::UnsignedBaseStoreTraits, FLOAT_REAL_BITS_NUMBER> BuiltReal;
typedef TGBuiltFloat<Numerics::UnsignedBaseStoreTraits, FLOAT_REAL_BITS_NUMBER, 23, 8> BuiltFloat;
typedef TGBuiltFloat<Numerics::UnsignedBaseStoreTraits, FLOAT_REAL_BITS_NUMBER, 52, 11> BuiltDouble;
typedef TGBuiltFloat<Numerics::UnsignedBaseStoreTraits, FLOAT_REAL_BITS_NUMBER,
        FloatDigitsHelper::TFloatDigits<long double>::UBitSizeMantissa, FloatDigitsHelper::TFloatDigits<long double>::UBitSizeExponent> BuiltLongDouble;
#endif

class BaseExecutionPath {
  protected:
   enum SpecialValue { SVNone, SVNoReal, SVNoImplementation, SVEnd };
   static Numerics::DDouble::Access::ReadParameters rpNearestParams;

  public:
   static void initializeNearestParams()
      {  rpNearestParams.setNearestRound();
         rpNearestParams.setRoundToEven();
      }
   static Numerics::DDouble::Access::ReadParameters& nearestParams() { return rpNearestParams; }
   class QuickDouble;
   class end {};

   typedef DDoubleExact::FloatDigitsHelper FloatDigitsHelper;
};

class ExecutionPathContract : public BaseExecutionPath {
  public:
   void flushOut() const {}

  protected:
   class DebugStream : public STG::IOObject::OSBase {};
   SpecialValue getMode() const { return SVNone; }
   bool doesFollow() const { return false; }
   bool doesFollowFlow() const { return false; }
   bool hasOutput() const { return false; }
   bool doesAssumeInput() const { return true; }

   void assumeDomain(char prefix) const {}
   void assumeDomain(const char* prefix) const {}
   void assumePrefixDomain(const char* prefix) const {}
   void assumeDomainUntil(char delimiter) const {}
   template <class TypeImplementation, class TypeParameters>
   void readValueFromInput(TypeImplementation& result, const TypeParameters& params) const {}
   template <class TypeImplementation, class TypeParameters>
   void emitErrorReadValueFromInput(const char* message,
         const TypeImplementation& readValue, const char* readPrefix,
         const TypeImplementation& expectedValue, const char* expectedPrefix,
         const TypeParameters& params) const {}
   void emitErrorFromInput(const char* message) {}

   void writeDomain(char prefix) const {}
   void writeDomain(const char* prefix) const {}
   void writePrefixDomain(const char* prefix) const {}
   template <class TypeImplementation, class TypeParameters>
   void writeValueToOutput(const TypeImplementation& value, const TypeParameters& params) const {}
   void writeSourceLine() const {}

   void assumeDivisionByZero() const {}
   void writeDivisionByZero() const {}
   void assumeNegativeSqrt() const {}
   void writeNegativeSqrt() const {}
   void assumeNegativePow() const {}
   void writeNegativePow() const {}
   void assumeNegativeOrNulLog() const {}
   void writeNegativeOrNulLog() const {}
   void assumeSourceLine() const {}

   void assumeThresholdDetection(const BuiltReal& relativeError, const BuiltReal& value) const {}
   void updateThresholdDetection(const BuiltReal& relativeError, const BuiltReal& value) const {}
   bool hasThreshold() const { return false; }
   bool updateMaximalAccuracy(const BuiltReal& relativeError, const BuiltReal& value) const { return false; }

   void notifyPossibleSplit(const char* file, int line) const {}
   void notifyPossibleMerge() const {}
   void notifySplitWithSynchronization() const {}
   void followNewUnstableBranch() const {}
   void alternateUnstableBranch() const {}
   bool isSynchronizedWith(const char* file, int line) const { return false; }
   bool hasSynchronization() const { return false; }
   bool synchronizeCurrentFlow() const { return true; }
   template <class TypeImplementation, class ImplReadParameters, class TypeReal, class RealReadParameters>
   void readSynchronizedValue(TypeImplementation& implementation, ImplReadParameters& implParams,
         TypeReal& real, RealReadParameters& realParams, bool& hasImplementation) const {}
   template <class TypeImplementation, class ImplReadParameters, class TypeReal, class RealReadParameters>
   void writeSynchronizedValue(const TypeImplementation& implementation, const ImplReadParameters& implParams,
         const TypeReal& real, const RealReadParameters& realParams, bool hasImplementation) const {}
};

template <class TypeExecutionPath>
class TBaseFloatExact : public TypeExecutionPath {
  private:
   typedef TypeExecutionPath inherited;

  protected:
   bool getThenBranch(bool implementationResult, bool realResult) const;

  public:
   static void splitBranches(const char* file, int line)
      {  BaseExecutionPath::SpecialValue mode = inherited::getMode();
         if (mode == BaseExecutionPath::SVNone)
            inherited::notifyPossibleSplit(file, line);
      }

   enum State
      {  SCompareFlow, SOnlyFloat, SOnlyReal, SOnlyFloatWithFutureMerge, SOnlyRealWithFutureMerge,
         SBadState
      };
   State queryState() const
      {  BaseExecutionPath::SpecialValue mode = inherited::getMode();
         if (mode == BaseExecutionPath::SVNone)
            return SCompareFlow;
         if (inherited::doesFollow())
            return (mode == BaseExecutionPath::SVNoImplementation) ? SOnlyRealWithFutureMerge
               : ((mode == BaseExecutionPath::SVNoReal) ? SOnlyFloatWithFutureMerge : SBadState);
         return (mode == BaseExecutionPath::SVNoImplementation) ? SOnlyReal
            : ((mode == BaseExecutionPath::SVNoReal) ? SOnlyFloat : SBadState);
      }

   template <class TFloatExact>
   void assumeCompare(const TFloatExact& source, const BuiltReal& diff) const;
   template <class TFloatExact>
   void writeCompare(const TFloatExact& source, const BuiltReal& diff) const;
   template <class TFloatExact>
   void writeBranchCompare(const TFloatExact& source, const BuiltReal& diff) const;

   template <class TFloatExact>
   void notifyForCompare(const TFloatExact& source) const
      {  BaseExecutionPath::SpecialValue mode;
         bool doesAssume;
         bool hasOutput;
         if (inherited::fSupportVerbose || inherited::fSupportThreshold) {
            mode = inherited::getMode();
            doesAssume = inherited::doesFollowFlow() && mode == BaseExecutionPath::SVNone;
            hasOutput = inherited::hasOutput();
         };
         if (inherited::fSupportVerbose) {
            if (doesAssume || hasOutput) {
               BuiltReal diff;
               if (mode == BaseExecutionPath::SVNone) {
                  source.retrieveReal(source.implementation(), diff);
                  diff.minusAssign(source.real(), inherited::nearestParams());
                  inherited::nearestParams().clear();
               };

               if (doesAssume && inherited::doesAssumeInput()) {
                  inherited::assumeSourceLine();
                  assumeCompare(source, diff);
               };
               if (hasOutput) {
                  inherited::writeSourceLine();
                  writeCompare(source, diff);
               };
            };
         };
         if (inherited::fSupportThreshold) {
            if ((doesAssume && inherited::hasThreshold()) || (hasOutput && mode == BaseExecutionPath::SVNone)) {
               BuiltReal relativeError;
               source.retrieveRelativeError(relativeError);
               if (!relativeError.isZero()) {
                  if (doesAssume && inherited::hasThreshold() && inherited::doesAssumeInput()) {
                     if (source.real().isPositive())
                        inherited::assumeThresholdDetection(relativeError, source.real());
                     else {
                        BuiltReal sourceReal(source.real());
                        sourceReal.opposite();
                        inherited::assumeThresholdDetection(relativeError, sourceReal);
                     };
                  };
                  if (hasOutput && mode == BaseExecutionPath::SVNone) {
                     if (inherited::hasThreshold()) {
                        if (source.real().isPositive())
                           inherited::updateThresholdDetection(relativeError, source.real());
                        else {
                           BuiltReal sourceReal(source.real());
                           sourceReal.opposite();
                           inherited::updateThresholdDetection(relativeError, sourceReal);
                        };
                     };
                     inherited::updateMaximalAccuracy(relativeError, source.real());
                  };
               };
            }
         };
      }
   template <class TFloatExact>
   void notifyForPersistence(const TFloatExact& source, const char* prefix) const
      {  BaseExecutionPath::SpecialValue mode = inherited::getMode();
         bool doesAssume = inherited::doesFollowFlow() && mode == BaseExecutionPath::SVNone;
         bool hasOutput = inherited::hasOutput();
         if (doesAssume || hasOutput) {
            BuiltReal diff;
            source.retrieveReal(source.bfImplementation, diff);
            diff.minusAssign(source.brReal, inherited::nearestParams());
            inherited::nearestParams().clear();

            if (doesAssume && inherited::doesAssumeInput()) {
               inherited::assumeSourceLine();
               if (prefix)
                  inherited::assumePrefixDomain(prefix);
               assumeCompare(source, diff);
            }
            if (hasOutput) {
               inherited::writeSourceLine();
               if (prefix)
                  inherited::writePrefixDomain(prefix);
               writeCompare(source, diff);
            }
         };
      }
   template <class TFloatExact>
   void notifyForBranchCompare(const TFloatExact& source) const
      {  BaseExecutionPath::SpecialValue mode = inherited::getMode();
         bool doesAssume = inherited::doesFollowFlow() && mode == BaseExecutionPath::SVNone;
         if (doesAssume && inherited::doesAssumeInput())
            inherited::assumeSourceLine();
         if (inherited::hasOutput()) {
            inherited::writeSourceLine();
            BuiltReal diff;
            source.retrieveReal(source.implementation(), diff);
            diff.minusAssign(source.real(), inherited::nearestParams());
            inherited::nearestParams().clear();
            writeBranchCompare(source, diff);
         };
      }
   template <class TFloatExact>
   void notifyForDivisionByZero(const TFloatExact& source) const
      {  if (inherited::hasOutput()) {
            inherited::writeSourceLine();
            inherited::writeDivisionByZero();
         };
         if (inherited::doesFollowFlow() && inherited::getMode() == BaseExecutionPath::SVNone && inherited::doesAssumeInput()) {
            inherited::assumeSourceLine();
            inherited::assumeDivisionByZero();
         };
      }
   template <class TFloatExact>
   void notifyForNegativeSqrt(const TFloatExact& source) const
      {  if (inherited::hasOutput()) {
            inherited::writeSourceLine();
            inherited::writeNegativeSqrt();
         };
         if (inherited::doesFollowFlow() && inherited::getMode() == BaseExecutionPath::SVNone && inherited::doesAssumeInput()) {
            inherited::assumeSourceLine();
            inherited::assumeNegativeSqrt();
         };
      }
   template <class TFloatExact>
   void notifyForNegativePow(const TFloatExact& source) const
      {  if (inherited::hasOutput()) {
            inherited::writeSourceLine();
            inherited::writeNegativePow();
         };
         if (inherited::doesFollowFlow() && inherited::getMode() == BaseExecutionPath::SVNone && inherited::doesAssumeInput()) {
            inherited::assumeSourceLine();
            inherited::assumeNegativePow();
         };
      }
   template <class TFloatExact>
   void notifyForNegativeOrNulLog(const TFloatExact& source) const
      {  if (inherited::hasOutput()) {
            inherited::writeSourceLine();
            inherited::writeNegativeOrNulLog();
         };
         if (inherited::doesFollowFlow() && inherited::getMode() == BaseExecutionPath::SVNone && inherited::doesAssumeInput()) {
            inherited::assumeSourceLine();
            inherited::assumeNegativeOrNulLog();
         };
      }
};

template <class TypeExecutionPath>
class TMergeBranches;

template <class TypeExecutionPath, class TypeBuiltDouble, typename TypeImplementation>
class TFloatExact
#if !defined(FLOAT_GENERIC_BASE_UNSIGNED) && !defined(FLOAT_GENERIC_BASE_LONG)
   : public TCompareFloatExact<FLOAT_REAL_BITS_NUMBER, TBaseFloatExact<TypeExecutionPath>, TypeBuiltDouble, TypeImplementation>
#else
   : public TGCompareFloatExact<FLOAT_REAL_BITS_NUMBER, TBaseFloatExact<TypeExecutionPath>, TypeBuiltDouble, TypeImplementation>
#endif
{
  private:
   typedef TFloatExact<TypeExecutionPath, TypeBuiltDouble, TypeImplementation> thisType;
#if !defined(FLOAT_GENERIC_BASE_UNSIGNED) && !defined(FLOAT_GENERIC_BASE_LONG)
   typedef TCompareFloatExact<FLOAT_REAL_BITS_NUMBER, TBaseFloatExact<TypeExecutionPath>, TypeBuiltDouble, TypeImplementation> inherited;
#else
   typedef TGCompareFloatExact<FLOAT_REAL_BITS_NUMBER, TBaseFloatExact<TypeExecutionPath>, TypeBuiltDouble, TypeImplementation> inherited;
#endif

  public:
   const char* queryDebugValue() const;

  protected:
   template <class TypeExecutionPathArgument, class TypeBuiltArgument, typename TypeImplementationArgument>
   friend class TFloatExact;
   typedef const char* (TFloatExact<TypeExecutionPath, TypeBuiltDouble, TypeImplementation>::*PQueryDebugValue)() const;
   static PQueryDebugValue pqueryDebugValue;

   // auto value() const -> const decltype(inherited::dValue)& { return inherited::dValue; }
   // auto svalue() -> decltype(inherited::dValue)& { return inherited::dValue; }
   // auto implementation() const -> const decltype(inherited::bfImplementation)& { return inherited::bfImplementation; }
   // auto implementation() -> decltype(inherited::bfImplementation)& { return inherited::bfImplementation; }
   // BuiltReal& real() { return inherited::brReal; }
   // const BuiltReal& real() const { return inherited::brReal; }
   friend class TMergeBranches<TypeExecutionPath>;
   friend class TBaseFloatExact<TypeExecutionPath>;

  public:
   TFloatExact() {}
   TFloatExact(int value) : inherited(value) {}
   TFloatExact(unsigned value) : inherited(value) {}
   TFloatExact(long value) : inherited(value) {}
   TFloatExact(unsigned long value) : inherited(value) {}
   TFloatExact(const thisType& source) = default;
   TFloatExact& operator=(const thisType& source) = default;

   template <class TypeBuiltArgument, typename TypeImplementationArgument>
   TFloatExact(const TFloatExact<TypeExecutionPath, TypeBuiltArgument, TypeImplementationArgument>& source)
      :  inherited(source) {}
   template <class TypeBuiltArgument, typename TypeImplementationArgument>
   thisType& operator=(const TFloatExact<TypeExecutionPath, TypeBuiltArgument, TypeImplementationArgument>& source)
      {  return (thisType&) inherited::operator=(source); }
};

template <class TypeExecutionPath>
class TMergeBranches : public TBaseFloatExact<TypeExecutionPath> {
  private:
   typedef TBaseFloatExact<TypeExecutionPath> inherited;
   bool fActive;

  public:
   TMergeBranches(const char* file, int line)
      :  fActive((inherited::getMode() != BaseExecutionPath::SVNone) && inherited::isSynchronizedWith(file, line)) {}

   template <class TypeBuiltDouble, typename TypeImplementation>
   TMergeBranches<TypeExecutionPath>& operator<<(TFloatExact<TypeExecutionPath, TypeBuiltDouble, TypeImplementation>& value);
   template <class TypeBuiltDouble, typename TypeImplementation>
   TMergeBranches<TypeExecutionPath>& operator<<(const TFloatExact<TypeExecutionPath, TypeBuiltDouble, TypeImplementation>& value)
      {  return operator<<(const_cast<TFloatExact<TypeExecutionPath, TypeBuiltDouble, TypeImplementation>&>(value)); }

   template <class TypeIterator>
   struct TPacker {
      TypeIterator iter, end;
      TPacker(TypeIterator aiter, TypeIterator aend) : iter(aiter), end(aend) {}
   };

   template <class TypeIterator>
   static TPacker<TypeIterator> packer(TypeIterator iter, TypeIterator end)
      {  return TPacker<TypeIterator>(iter, end); }

   template <class TypeIterator>
   TMergeBranches<TypeExecutionPath>& operator<<(TPacker<TypeIterator>&& packer)
      {  for (; packer.iter != packer.end; ++packer.iter)
            operator<<(*packer.iter);
         return *this;
      }
   bool operator<<(BaseExecutionPath::end)
      {  if (!fActive) {
            inherited::notifyPossibleMerge();
            return true;
         };
         // inherited::getMode() != BaseExecutionPath::SVNone
         return inherited::synchronizeCurrentFlow();
      }
};

class BaseExecutionPath::QuickDouble
#if !defined(FLOAT_GENERIC_BASE_UNSIGNED) && !defined(FLOAT_GENERIC_BASE_LONG)
   : public Numerics::TBuiltDouble<Numerics::BuiltDoubleTraits<23, 8> >
#elif defined(FLOAT_GENERIC_BASE_LONG)
   : public Numerics::TGBuiltDouble<Numerics::GBuiltDoubleTraits<Numerics::UnsignedLongBaseStoreTraits, 23, 8> >
#else
   : public Numerics::TGBuiltDouble<Numerics::GBuiltDoubleTraits<Numerics::UnsignedBaseStoreTraits, 23, 8> >
#endif
{ private:
#if !defined(FLOAT_GENERIC_BASE_UNSIGNED) && !defined(FLOAT_GENERIC_BASE_LONG)
   typedef Numerics::TBuiltDouble<Numerics::BuiltDoubleTraits<23, 8> > inherited;
   typedef Numerics::UnsignedBaseStoreTraits BaseStoreTraits;
#elif defined(FLOAT_GENERIC_BASE_LONG)
   typedef Numerics::TGBuiltDouble<Numerics::GBuiltDoubleTraits<Numerics::UnsignedLongBaseStoreTraits, 23, 8> > inherited;
   typedef Numerics::UnsignedLongBaseStoreTraits BaseStoreTraits;
#else
   typedef Numerics::TGBuiltDouble<Numerics::GBuiltDoubleTraits<Numerics::UnsignedBaseStoreTraits, 23, 8> > inherited;
   typedef Numerics::UnsignedBaseStoreTraits BaseStoreTraits;
#endif

  public:
   QuickDouble() {}
   QuickDouble(const inherited& source) : inherited(source) {}
   QuickDouble(inherited&& source) : inherited(source) {}
   template <class TypeDouble>
   QuickDouble(const TypeDouble& source)
      {  typename inherited::FloatConversion conversion;
         int sizeMantissa = source.bitSizeMantissa();
         int sourceShiftMantissa = 0, sourceShiftExponent = 0;
         if (sizeMantissa > conversion.getMaxSizeMantissa()) {
            sourceShiftMantissa = (sizeMantissa- conversion.getMaxSizeMantissa()
                  + BaseStoreTraits::sizeBaseInBits()-1) / BaseStoreTraits::sizeBaseInBits();
            sizeMantissa = conversion.getMaxSizeMantissa() 
                  - (BaseStoreTraits::sizeBaseInBits()-1 - (sizeMantissa-1) % BaseStoreTraits::sizeBaseInBits());
         };
         int sizeExponent = source.bitSizeExponent();
         if (sizeExponent > conversion.getMaxSizeExponent()) {
            sourceShiftExponent = (sizeExponent - conversion.getMaxSizeExponent()
                  + BaseStoreTraits::sizeBaseInBits()-1) / BaseStoreTraits::sizeBaseInBits();
            sizeExponent = conversion.getMaxSizeExponent()
                  - (BaseStoreTraits::sizeBaseInBits()-1 - (sizeExponent-1) % BaseStoreTraits::sizeBaseInBits());
         };
         conversion.setSizeMantissa(sizeMantissa).setSizeExponent(sizeExponent);
         sizeMantissa = (sizeMantissa + BaseStoreTraits::sizeBaseInBits() - 1)/BaseStoreTraits::sizeBaseInBits();
         for (int index = 0; index < sizeMantissa; ++index)
            conversion.mantissa()[index] = source.getMantissa()[index + sourceShiftMantissa];
         sizeExponent = (sizeExponent + BaseStoreTraits::sizeBaseInBits() - 1)/BaseStoreTraits::sizeBaseInBits();
         for (int index = 0; index < sizeExponent; ++index)
            conversion.exponent()[index] = source.getBasicExponent()[index + sourceShiftExponent];
         conversion.setNegative(source.isNegative());
         inherited::setFloat(conversion, rpNearestParams);
         rpNearestParams.clear();
      }
};

}} // end of namespace NumericalDomains::DDoubleExact

#endif // NumericalAnalysis_FloatExactExecutionPathH

