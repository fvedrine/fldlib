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
// Unit      : Interval
// File      : FloatIntervalExecutionPath.h
// Description :
//   Definition of a class of floating point intervals
//

#ifndef FloatInstrumentation_FloatIntervalExecutionPathH
#define FloatInstrumentation_FloatIntervalExecutionPathH

#include <cfloat>
#include "config.h"
#include "NumericalDomains/FloatIntervalBase.h"

namespace NumericalDomains { namespace DDoubleInterval {

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

typedef TBuiltFloat<FloatDigitsHelper::TFloatDigits<long double>::UBitSizeMantissa+1, 23, 8> BuiltFloat;
typedef TBuiltFloat<FloatDigitsHelper::TFloatDigits<long double>::UBitSizeMantissa+1, 52, 11> BuiltDouble;
typedef TBuiltFloat<FloatDigitsHelper::TFloatDigits<long double>::UBitSizeMantissa+1,
   FloatDigitsHelper::TFloatDigits<long double>::UBitSizeMantissa,
   FloatDigitsHelper::TFloatDigits<long double>::UBitSizeExponent> BuiltLongDouble;

class BaseExecutionPath {
  protected:
   static Numerics::DDouble::Access::ReadParameters rpMinParams;
   static Numerics::DDouble::Access::ReadParameters rpMaxParams;

  public:
   typedef BuiltLongDouble::FloatConversion FloatConversion;
   static void initializeMinMaxParams()
      {  rpMinParams.setLowestRound();
         rpMaxParams.setHighestRound();
      }
   static Numerics::DDouble::Access::ReadParameters& minParams() { return rpMinParams; }
   static Numerics::DDouble::Access::ReadParameters& maxParams() { return rpMaxParams; }
   class end {};

   typedef DDoubleInterval::FloatDigitsHelper FloatDigitsHelper;
};

class ExecutionPathContract : public BaseExecutionPath {
  public:
   void flushOut() const {}

  protected:
   class DebugStream : public STG::IOObject::OSBase {};
   bool doesFollowFlow() const { return false; }
   bool hasOutput() const { return false; }

   void assumeDomain(char prefix) const {}
   void assumeDomain(const char* prefix) const {}
   void assumePrefixDomain(const char* prefix) const {}
   template <class TypeImplementation, class TypeParameters>
   void readValueFromInput(TypeImplementation& result, const TypeParameters& params) const {}
   template <class TypeImplementation, class TypeParameters>
   void emitErrorReadValueFromInput(const char* message,
         const TypeImplementation& readValue, const char* readPrefix,
         const TypeImplementation& expectedValue, const char* expectedPrefix,
         const TypeParameters& params) const {}

   void writeDomain(char prefix) const {}
   void writeDomain(const char* prefix) const {}
   void writePrefixDomain(const char* prefix) const {}
   template <class TypeImplementation, class TypeParameters>
   void writeValueToOutput(const TypeImplementation& value, const TypeParameters& params) const {}

   void assumeDivisionByZero() const {}
   void writeDivisionByZero() const {}
   void assumeNegativeSqrt() const {}
   void writeNegativeSqrt() const {}
   void assumeNegativePow() const {}
   void writeNegativePow() const {}
   void assumeNegativeOrNulLog() const {}
   void writeNegativeOrNulLog() const {}

   void assumeSourceLine() const {}
   void writeSourceLine() const {}

   template <class TypeBuiltDouble>
   static TypeBuiltDouble& threshold(const TypeBuiltDouble& relativeError) { return *(TypeBuiltDouble*) nullptr; }
   template <class TypeBuiltDouble>
   static TypeBuiltDouble& maximalAccuracy(const TypeBuiltDouble& relativeError) { return *(TypeBuiltDouble*) nullptr; }

   template <class TypeBuiltDouble>
   void assumeThresholdDetection(const TypeBuiltDouble& relativeError) const;
   template <class TypeBuiltDouble>
   void updateThresholdDetection(const TypeBuiltDouble& relativeError) const;

   bool followNewUnresolvedBranch(bool& isLastBranch) const { return true; }
   unsigned followNewConversionBranch(unsigned conversion, bool& isLastBranch) const { return 0; }
};

template <class TypeExecutionPath>
class TBaseFloatInterval : public TypeExecutionPath {
  private:
   typedef TypeExecutionPath inherited;

  public:
   bool getThenBranch(bool executionResult) const;
   unsigned getConversionBranch(unsigned diff, unsigned executionResult) const;

  public:
   static Numerics::DDouble::Access::ReadParameters& minParams() { return inherited::minParams(); }
   static Numerics::DDouble::Access::ReadParameters& maxParams() { return inherited::maxParams(); }

   template <class TFloatInterval>
   void assumeInterval(const TFloatInterval& source) const;
   template <class TFloatInterval>
   void writeInterval(const TFloatInterval& source) const;
   template <class TFloatInterval>
   void assumeBranchInterval(const TFloatInterval& source) const;
   template <class TFloatInterval>
   void writeBranchInterval(const TFloatInterval& source) const;

   template <class TFloatInterval>
   void notifyForCompare(const TFloatInterval& source) const;
   template <class TFloatInterval>
   void notifyForPersistence(const TFloatInterval& source, const char* prefix) const
      {  bool doesAssume = inherited::doesFollowFlow();
         bool hasOutput = inherited::hasOutput();
         if (doesAssume || hasOutput) {
            if (doesAssume && inherited::doesAssumeInput()) {
               inherited::assumeSourceLine();
               if (prefix)
                  inherited::assumePrefixDomain(prefix);
               assumeInterval(source);
            }
            if (hasOutput) {
               inherited::writeSourceLine();
               if (prefix)
                  inherited::writePrefixDomain(prefix);
               writeInterval(source);
            };
         };
      }
   template <class TFloatInterval>
   void notifyForBranchCompare(const TFloatInterval& source) const
      {  if (inherited::doesFollowFlow() && inherited::doesAssumeInput()) {
            inherited::assumeSourceLine();
            assumeBranchInterval(source);
         };
         if (inherited::hasOutput()) {
            inherited::writeSourceLine();
            writeBranchInterval(source);
         };
      }
   template <class TFloatInterval>
   void notifyForDivisionByZero(const TFloatInterval& source) const
      {  if (inherited::hasOutput()) {
            inherited::writeSourceLine();
            inherited::writeDivisionByZero();
         };
         if (inherited::doesFollowFlow() && inherited::doesAssumeInput()) {
            inherited::assumeSourceLine();
            inherited::assumeDivisionByZero();
         };
      }
   template <class TFloatInterval>
   void notifyForNegativeSqrt(const TFloatInterval& source) const
      {  if (inherited::hasOutput()) {
            inherited::writeSourceLine();
            inherited::writeNegativeSqrt();
         };
         if (inherited::doesFollowFlow() && inherited::doesAssumeInput()) {
            inherited::assumeSourceLine();
            inherited::assumeNegativeSqrt();
         };
      }
   template <class TFloatInterval>
   void notifyForNegativePow(const TFloatInterval& source) const
      {  if (inherited::hasOutput()) {
            inherited::writeSourceLine();
            inherited::writeNegativePow();
         };
         if (inherited::doesFollowFlow() && inherited::doesAssumeInput()) {
            inherited::assumeSourceLine();
            inherited::assumeNegativePow();
         };
      }
   template <class TFloatInterval>
   void notifyForNegativeOrNulLog(const TFloatInterval& source) const
      {  if (inherited::hasOutput()) {
            inherited::writeSourceLine();
            inherited::writeNegativeOrNulLog();
         };
         if (inherited::doesFollowFlow() && inherited::doesAssumeInput()) {
            inherited::assumeSourceLine();
            inherited::assumeNegativeOrNulLog();
         };
      }
};

template <class TypeExecutionPath>
template <class TFloatInterval>
inline void
TBaseFloatInterval<TypeExecutionPath>::notifyForCompare(const TFloatInterval& source) const {  
   bool doesAssume;
   bool hasOutput;
   if (inherited::fSupportVerbose || inherited::fSupportThreshold) {
      doesAssume = inherited::doesFollowFlow();
      hasOutput = inherited::hasOutput();
   };
   if (inherited::fSupportVerbose) {
      if (doesAssume || hasOutput) {
         if (doesAssume && inherited::doesAssumeInput()) {
            inherited::assumeSourceLine();
            assumeInterval(source);
         };
         if (hasOutput) {
            inherited::writeSourceLine();
            writeInterval(source);
         };
      };
   };
   if (inherited::fSupportThreshold) {
      const auto& threshold = inherited::threshold(source.min());
      if ((doesAssume && !threshold.isZero()) || hasOutput) {
         typename TFloatInterval::BuiltDouble relativeError;
         source.retrieveRelativeError(relativeError);
         if (!relativeError.isZero()) {
            if (doesAssume && !threshold.isZero() && inherited::doesAssumeInput())
               inherited::assumeThresholdDetection(relativeError);
            if (hasOutput) {
               auto& maximalAccuracy = inherited::maximalAccuracy(source.min());
               if (!threshold.isZero())
                  inherited::updateThresholdDetection(relativeError);
               if (relativeError > maximalAccuracy)
                  maximalAccuracy = relativeError;
            };
         };
      };
   };
}

template <class TypeExecutionPath, class TypeBuiltDouble, typename TypeImplementation>
class TFloatInterval : public TCompareFloatInterval<
      FloatDigitsHelper::TFloatDigits<long double>::UBitSizeMantissa+1,
      TBaseFloatInterval<TypeExecutionPath>, TypeBuiltDouble, TypeImplementation> {
  private:
   typedef TFloatInterval<TypeExecutionPath, TypeBuiltDouble, TypeImplementation> thisType;
   typedef TCompareFloatInterval<
      FloatDigitsHelper::TFloatDigits<long double>::UBitSizeMantissa+1,
      TBaseFloatInterval<TypeExecutionPath>, TypeBuiltDouble, TypeImplementation> inherited;

  public:
   const char* queryDebugValue() const;

  protected:
   template <class TypeExecutionPathArgument, class TypeBuiltArgument, typename TypeImplementationArgument>
   friend class TFloatInterval;
   typedef const char* (TFloatInterval<TypeExecutionPath, TypeBuiltDouble, TypeImplementation>::*PQueryDebugValue)() const;
   static PQueryDebugValue pqueryDebugValue;

  public:
   TFloatInterval() {}
   TFloatInterval(TypeImplementation min, TypeImplementation max) : inherited(min, max) {}
   TFloatInterval(int value) : inherited(value) {}
   TFloatInterval(long int value) : inherited(value) {}
   TFloatInterval(unsigned value) : inherited(value) {}
   TFloatInterval(unsigned long value) : inherited(value) {}
   TFloatInterval(const thisType& source) = default;
   TFloatInterval(thisType&& source) = default; // [TODO] keep symbolic for constraints
   TFloatInterval& operator=(const thisType& source) = default;
   TFloatInterval& operator=(thisType&& source) = default; // [TODO] keep symbolic for constraints

   template <class TypeBuiltArgument, typename TypeImplementationArgument>
   TFloatInterval(const TFloatInterval<TypeExecutionPath, TypeBuiltArgument, TypeImplementationArgument>& source)
      :  inherited(source) {}
   template <class TypeBuiltArgument, typename TypeImplementationArgument>
   thisType& operator=(const TFloatInterval<TypeExecutionPath, TypeBuiltArgument, TypeImplementationArgument>& source)
      {  return (thisType&) inherited::operator=(source); }
};

}} // end of namespace NumericalDomains::DDoubleInterval

#endif // FloatInstrumentation_FloatIntervalExecutionPathH

