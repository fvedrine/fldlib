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
// Unit      : Affine relationships
// File      : FloatAffineExecutionPath.h
// Description :
//   Definition of a class of affine relations.
//

#ifndef NumericalAnalysis_FloatAffineExecutionPathH
#define NumericalAnalysis_FloatAffineExecutionPathH

#include <cfloat>

#include "config.h"
#if !defined(FLOAT_GENERIC_BASE_UNSIGNED) && !defined(FLOAT_GENERIC_BASE_LONG)
#include "NumericalDomains/FloatAffineBase.h"
#else
#include "NumericalDomains/FloatAffineBaseGeneric.h"
#include "Numerics/Integer.h"
#endif

#ifndef FLOAT_REAL_BITS_NUMBER
#define FLOAT_REAL_BITS_NUMBER 123
#endif

namespace NumericalDomains { namespace DAffine {

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

#if !defined(FLOAT_GENERIC_BASE_UNSIGNED) && !defined(FLOAT_GENERIC_BASE_LONG)
typedef TBuiltReal<FLOAT_REAL_BITS_NUMBER> BuiltReal;
#elif defined(FLOAT_GENERIC_BASE_LONG)
typedef TGBuiltReal<Numerics::UnsignedLongBaseStoreTraits, FLOAT_REAL_BITS_NUMBER> BuiltReal;
#else // defined(FLOAT_GENERIC_BASE_UNSIGNED)
typedef TGBuiltReal<Numerics::UnsignedBaseStoreTraits, FLOAT_REAL_BITS_NUMBER> BuiltReal;
#endif

class BaseExecutionPath {
  public:
   enum Mode { MRealAndImplementation, MOnlyReal, MOnlyImplementation };
   enum BooleanChoice { BCNone, BCTrue, BCFalse, BCTop };

   class QuickDouble;
   class end {};
};

class ExecutionPathContract : public BaseExecutionPath {
  public:
   void flushOut() const {}
   // typedef NumericalDomains::Equation EquationHolder;
   // bool hasMultipleBranches() const { return false; }
   // std::function<bool(const NumericalDomains::Equation&)> getConditionSameBranch() const
   //    { return [](const NumericalDomains::Equation& equation){ return true; } }

   static void initializeParams()
      {  rpNearestParams.setNearestRound();
         rpNearestParams.setRoundToEven();
         rpMinParams.setLowestRound();
         rpMaxParams.setHighestRound();
      }

   static SymbolsManager& symbolsManager() { return smSymbolsManager; }
   static Numerics::DDouble::Access::ReadParameters& nearestParams() { return rpNearestParams; }
   static Numerics::DDouble::Access::ReadParameters& minParams() { return rpMinParams; }
   static Numerics::DDouble::Access::ReadParameters& maxParams() { return rpMaxParams; }

  protected:
   class DebugStream : public STG::IOObject::OSBase {};

   static Numerics::DDouble::Access::ReadParameters rpMinParams, rpMaxParams, rpNearestParams;
   static SymbolsManager smSymbolsManager;
   static SymbolsManager::MergeTable mtInitialTable;

   static void notifyPossibleSplit(const char* file, int line) {}
   static void notifyPossibleMerge() {}
   Mode getMode() const { return MRealAndImplementation; }
   bool doesFollow() const { return false; }
   bool doesFollowFlow() const { return false; }
   bool hasOutput() const { return false; }
   bool hasThreshold() const { return false; }

   bool hasSimplificationTriggerPercent() const { return false; }
   BuiltReal getSimplificationTriggerPercent() const { return BuiltReal(); }

   void assumeDomain(char prefix) const {}
   void assumeDomain(const char* prefix) const {}
   void assumePrefixDomain(const char* prefix) const {}
   void assumeDomainUntil(char delimiter) const {}
   template <class TypeAffine, class ImplReadParameters, class ImplWriteParameters,
         class EquationReadParameters, class EquationWriteParameters,
         class HighLevelUpdateVector>
   static void readSynchronizedValue(int& floatBranchesToRead, int floatSelection,
         int numberOfFloatBranches, int& realBranchesToRead, int realSelection,
         int numberOfRealBranches, BaseExecutionPath::Mode mode, TypeAffine& value,
         ImplReadParameters& implReadParams, ImplWriteParameters& implWriteParams,
         EquationReadParameters& equationReadParams, EquationWriteParameters& equationWriteParams,
         HighLevelUpdateVector& highLevelUpdates, bool& hasImplementation, bool& doesMergeValue) {}
   template <class TypeAffine, class ImplWriteParameters, class EquationWriteParameters>
   static void writeSynchronizedValue(BaseExecutionPath::Mode mode, const TypeAffine& value,
         const ImplWriteParameters& implWriteParams, const EquationWriteParameters& equationWriteParams) {}

   template <class TypeImplementation, class TypeParameters>
   void readValueFromInput(TypeImplementation& result, TypeParameters& params) const {}
   template <class TypeImplementation, class TypeParameters>
   void emitErrorReadValueFromInput(const char* message,
         const TypeImplementation& minReadValue, const TypeImplementation& maxReadValue, const char* readPrefix,
         const TypeImplementation& minExpectedValue, const TypeImplementation& maxExpectedValue, const char* expectedPrefix,
         const TypeParameters& params) const {}
   void emitErrorFromInput(const char* message) const {}

   void writeDomain(char prefix) const {}
   void writeDomain(const char* prefix) const {}
   void writePrefixDomain(const char* prefix) const {}
   template <class TypeImplementation, class TypeParameters>
   void writeValueToOutput(const TypeImplementation& value, const TypeParameters& params) const {}

   void writeUnstableTest() const {}
   void readUnstableTest() const {}
   void assumeDivisionByZero() const {}
   void writeDivisionByZero() const {}
   void assumeNegativeSqrt() const {}
   void writeNegativeSqrt() const {}
   void assumeNegativePow() const {}
   void writeNegativePow() const {}
   void assumeNegativeOrNulLog() const {}
   void writeNegativeOrNulLog() const {}

   void writeSourceLine() const {}
   void assumeSourceLine() const {}

   void throwEmptyBranch(bool isUnstable) const {}
   template <class TypeFloatAffine>
   void assumeThresholdDetection(const TypeFloatAffine& source) const {}
   template <class TypeFloatAffine>
   void updateThresholdDetection(const TypeFloatAffine& source) const {}

   void persist(const char* prefix) const {}
   bool doesAbsorbHighLevel() const { return false; }
   unsigned getLimitStartSymbolAbsorption() const { return 48; }
   bool doesExcludeConstantFromSymbolAbsorption() const { return false; }
   bool isPureZonotope() const { return false; }
   bool doesAllowSimplex() const { return false; }

   void followNewBranch(int cases, BooleanChoice& realChoice, BooleanChoice& errorChoice,
         bool doesChooseDefaultErrorChoice, BooleanChoice defaultErrorChoice) const {}
   unsigned followNewConversionBranch(unsigned conversion, bool& realChoice,
         bool& implementationChoice, unsigned realNumber, unsigned implementationNumber,
         unsigned commonNumber, unsigned firstRealCommon, unsigned firstImplementationCommon) const
      {  return 0; }

   void notifySplitWithSynchronization() const {}
   bool isSynchronizedWith(const char* file, int line) const { return false; }
   bool hasSynchronization() const { return false; }
   bool synchronizeCurrentFlow(unsigned& floatSelection, unsigned& realSelection,
         int numberOfFloatBranches, int numberOfRealBranches) const { return true; }
   void readUntilBranchNumbers(int& numberOfFloatBranches, int& numberOfRealBranches) const {}
   void selectBranch(unsigned& floatSelection, unsigned& realSelection,
         int numberOfFloatBranches, int numberOfRealBranches) const {}
   void writeBranchNumbers(int numberOfFloatBranches, int numberOfRealBranches) const {}
   void clearSynchronizationBranches() const {}
};

template <class TypeExecutionPath>
class TBaseFloatAffine : public TypeExecutionPath, public FloatDigitsHelper {
  private:
   typedef TypeExecutionPath inherited;

  protected:
   static const char* queryDebugRealValue(const BuiltReal& value);
#if !defined(FLOAT_GENERIC_BASE_UNSIGNED) && !defined(FLOAT_GENERIC_BASE_LONG)
   static const char* queryEquationValue(const TEquation<FLOAT_REAL_BITS_NUMBER, TBaseFloatAffine<TypeExecutionPath>>& equation);
   static const char* queryEquationValueAsInterval(const TEquation<FLOAT_REAL_BITS_NUMBER, TBaseFloatAffine<TypeExecutionPath>>& equation);
   static const char* queryBaseZonotopeValue(const TBaseFloatZonotope<FLOAT_REAL_BITS_NUMBER, TBaseFloatAffine<TypeExecutionPath>>& zonotope);
   static const char* queryBaseZonotopeValueAsInterval(const TBaseFloatZonotope<FLOAT_REAL_BITS_NUMBER, TBaseFloatAffine<TypeExecutionPath>>& zonotope);
   static const char* queryFloatZonotopeValue(const TBasicFloatZonotope<FLOAT_REAL_BITS_NUMBER, TBaseFloatAffine<TypeExecutionPath>, 23, 8, float>& zonotope);
   static const char* queryFloatZonotopeValueAsInterval(const TBasicFloatZonotope<FLOAT_REAL_BITS_NUMBER, TBaseFloatAffine<TypeExecutionPath>, 23, 8, float>& zonotope);
   static const char* queryDoubleZonotopeValue(const TBasicFloatZonotope<FLOAT_REAL_BITS_NUMBER, TBaseFloatAffine<TypeExecutionPath>, 52, 11, double>& zonotope);
   static const char* queryDoubleZonotopeValueAsInterval(const TBasicFloatZonotope<FLOAT_REAL_BITS_NUMBER, TBaseFloatAffine<TypeExecutionPath>, 52, 11, double>& zonotope);
   static const char* queryLongDoubleZonotopeValue(const TBasicFloatZonotope<FLOAT_REAL_BITS_NUMBER, TBaseFloatAffine<TypeExecutionPath>,
      TFloatDigits<long double>::UBitSizeMantissa, TFloatDigits<long double>::UBitSizeExponent, long double>& zonotope);
   static const char* queryLongDoubleZonotopeValueAsInterval(const TBasicFloatZonotope<FLOAT_REAL_BITS_NUMBER, TBaseFloatAffine<TypeExecutionPath>,
      TFloatDigits<long double>::UBitSizeMantissa, TFloatDigits<long double>::UBitSizeExponent, long double>& zonotope);

   static const char* (*pqueryDebugRealValue)(const BuiltReal&);
   static const char* (*pqueryEquationValue)(const TEquation<FLOAT_REAL_BITS_NUMBER, TBaseFloatAffine<TypeExecutionPath>>&);
   static const char* (*pqueryEquationValueAsInterval)(const TEquation<FLOAT_REAL_BITS_NUMBER, TBaseFloatAffine<TypeExecutionPath>>&);
   static const char* (*pqueryBaseZonotopeValue)(const TBaseFloatZonotope<FLOAT_REAL_BITS_NUMBER, TBaseFloatAffine<TypeExecutionPath>>&);
   static const char* (*pqueryBaseZonotopeValueAsInterval)(const TBaseFloatZonotope<FLOAT_REAL_BITS_NUMBER, TBaseFloatAffine<TypeExecutionPath>>&);
   static const char* (*pqueryFloatZonotopeValue)(const TBasicFloatZonotope<FLOAT_REAL_BITS_NUMBER, TBaseFloatAffine<TypeExecutionPath>, 23, 8, float>&);
   static const char* (*pqueryFloatZonotopeValueAsInterval)(const TBasicFloatZonotope<FLOAT_REAL_BITS_NUMBER, TBaseFloatAffine<TypeExecutionPath>, 23, 8, float>&);
   static const char* (*pqueryDoubleZonotopeValue)(const TBasicFloatZonotope<FLOAT_REAL_BITS_NUMBER, TBaseFloatAffine<TypeExecutionPath>, 52, 11, double>&);
   static const char* (*pqueryDoubleZonotopeValueAsInterval)(const TBasicFloatZonotope<FLOAT_REAL_BITS_NUMBER, TBaseFloatAffine<TypeExecutionPath>, 52, 11, double>&);
   static const char* (*pqueryLongDoubleZonotopeValue)(const TBasicFloatZonotope<FLOAT_REAL_BITS_NUMBER, TBaseFloatAffine<TypeExecutionPath>,
      TFloatDigits<long double>::UBitSizeMantissa, TFloatDigits<long double>::UBitSizeExponent, long double>&);
   static const char* (*pqueryLongDoubleZonotopeValueAsInterval)(const TBasicFloatZonotope<FLOAT_REAL_BITS_NUMBER, TBaseFloatAffine<TypeExecutionPath>,
      TFloatDigits<long double>::UBitSizeMantissa, TFloatDigits<long double>::UBitSizeExponent, long double>&);
#elif defined(FLOAT_GENERIC_BASE_LONG)
   static const char* queryEquationValue(const TGEquation<Numerics::UnsignedLongBaseStoreTraits, FLOAT_REAL_BITS_NUMBER, TBaseFloatAffine<TypeExecutionPath>>& equation);
   static const char* queryEquationValueAsInterval(const TGEquation<Numerics::UnsignedLongBaseStoreTraits, FLOAT_REAL_BITS_NUMBER, TBaseFloatAffine<TypeExecutionPath>>& equation);
   static const char* queryBaseZonotopeValue(const TGBaseFloatZonotope<Numerics::UnsignedLongBaseStoreTraits, FLOAT_REAL_BITS_NUMBER, TBaseFloatAffine<TypeExecutionPath>>& zonotope);
   static const char* queryBaseZonotopeValueAsInterval(const TGBaseFloatZonotope<Numerics::UnsignedLongBaseStoreTraits, FLOAT_REAL_BITS_NUMBER, TBaseFloatAffine<TypeExecutionPath>>& zonotope);
   static const char* queryFloatZonotopeValue(const TGBasicFloatZonotope<Numerics::UnsignedLongBaseStoreTraits, FLOAT_REAL_BITS_NUMBER, TBaseFloatAffine<TypeExecutionPath>, 23, 8, float>& zonotope);
   static const char* queryFloatZonotopeValueAsInterval(const TGBasicFloatZonotope<Numerics::UnsignedLongBaseStoreTraits, FLOAT_REAL_BITS_NUMBER, TBaseFloatAffine<TypeExecutionPath>, 23, 8, float>& zonotope);
   static const char* queryDoubleZonotopeValue(const TGBasicFloatZonotope<Numerics::UnsignedLongBaseStoreTraits, FLOAT_REAL_BITS_NUMBER, TBaseFloatAffine<TypeExecutionPath>, 52, 11, double>& zonotope);
   static const char* queryDoubleZonotopeValueAsInterval(const TGBasicFloatZonotope<Numerics::UnsignedLongBaseStoreTraits, FLOAT_REAL_BITS_NUMBER, TBaseFloatAffine<TypeExecutionPath>, 52, 11, double>& zonotope);
   static const char* queryLongDoubleZonotopeValue(const TGBasicFloatZonotope<Numerics::UnsignedLongBaseStoreTraits, FLOAT_REAL_BITS_NUMBER, TBaseFloatAffine<TypeExecutionPath>,
      TFloatDigits<long double>::UBitSizeMantissa, TFloatDigits<long double>::UBitSizeExponent, long double>& zonotope);
   static const char* queryLongDoubleZonotopeValueAsInterval(const TGBasicFloatZonotope<Numerics::UnsignedLongBaseStoreTraits, FLOAT_REAL_BITS_NUMBER, TBaseFloatAffine<TypeExecutionPath>,
      TFloatDigits<long double>::UBitSizeMantissa, TFloatDigits<long double>::UBitSizeExponent, long double>& zonotope);

   static const char* (*pqueryDebugRealValue)(const BuiltReal&);
   static const char* (*pqueryEquationValue)(const TGEquation<Numerics::UnsignedLongBaseStoreTraits, FLOAT_REAL_BITS_NUMBER, TBaseFloatAffine<TypeExecutionPath>>&);
   static const char* (*pqueryEquationValueAsInterval)(const TGEquation<Numerics::UnsignedLongBaseStoreTraits, FLOAT_REAL_BITS_NUMBER, TBaseFloatAffine<TypeExecutionPath>>&);
   static const char* (*pqueryBaseZonotopeValue)(const TGBaseFloatZonotope<Numerics::UnsignedLongBaseStoreTraits, FLOAT_REAL_BITS_NUMBER, TBaseFloatAffine<TypeExecutionPath>>&);
   static const char* (*pqueryBaseZonotopeValueAsInterval)(const TGBaseFloatZonotope<Numerics::UnsignedLongBaseStoreTraits, FLOAT_REAL_BITS_NUMBER, TBaseFloatAffine<TypeExecutionPath>>&);
   static const char* (*pqueryFloatZonotopeValue)(const TGBasicFloatZonotope<Numerics::UnsignedLongBaseStoreTraits, FLOAT_REAL_BITS_NUMBER, TBaseFloatAffine<TypeExecutionPath>, 23, 8, float>&);
   static const char* (*pqueryFloatZonotopeValueAsInterval)(const TGBasicFloatZonotope<Numerics::UnsignedLongBaseStoreTraits, FLOAT_REAL_BITS_NUMBER, TBaseFloatAffine<TypeExecutionPath>, 23, 8, float>&);
   static const char* (*pqueryDoubleZonotopeValue)(const TGBasicFloatZonotope<Numerics::UnsignedLongBaseStoreTraits, FLOAT_REAL_BITS_NUMBER, TBaseFloatAffine<TypeExecutionPath>, 52, 11, double>&);
   static const char* (*pqueryDoubleZonotopeValueAsInterval)(const TGBasicFloatZonotope<Numerics::UnsignedLongBaseStoreTraits, FLOAT_REAL_BITS_NUMBER, TBaseFloatAffine<TypeExecutionPath>, 52, 11, double>&);
   static const char* (*pqueryLongDoubleZonotopeValue)(const TGBasicFloatZonotope<Numerics::UnsignedLongBaseStoreTraits, FLOAT_REAL_BITS_NUMBER, TBaseFloatAffine<TypeExecutionPath>,
      TFloatDigits<long double>::UBitSizeMantissa, TFloatDigits<long double>::UBitSizeExponent, long double>&);
   static const char* (*pqueryLongDoubleZonotopeValueAsInterval)(const TGBasicFloatZonotope<Numerics::UnsignedLongBaseStoreTraits, FLOAT_REAL_BITS_NUMBER, TBaseFloatAffine<TypeExecutionPath>,
      TFloatDigits<long double>::UBitSizeMantissa, TFloatDigits<long double>::UBitSizeExponent, long double>&);
#else // defined(FLOAT_GENERIC_BASE_UNSIGNED)
   static const char* queryEquationValue(const TGEquation<Numerics::UnsignedBaseStoreTraits, FLOAT_REAL_BITS_NUMBER, TBaseFloatAffine<TypeExecutionPath>>& equation);
   static const char* queryEquationValueAsInterval(const TGEquation<Numerics::UnsignedBaseStoreTraits, FLOAT_REAL_BITS_NUMBER, TBaseFloatAffine<TypeExecutionPath>>& equation);
   static const char* queryBaseZonotopeValue(const TGBaseFloatZonotope<Numerics::UnsignedBaseStoreTraits, FLOAT_REAL_BITS_NUMBER, TBaseFloatAffine<TypeExecutionPath>>& zonotope);
   static const char* queryBaseZonotopeValueAsInterval(const TGBaseFloatZonotope<Numerics::UnsignedBaseStoreTraits, FLOAT_REAL_BITS_NUMBER, TBaseFloatAffine<TypeExecutionPath>>& zonotope);
   static const char* queryFloatZonotopeValue(const TGBasicFloatZonotope<Numerics::UnsignedBaseStoreTraits, FLOAT_REAL_BITS_NUMBER, TBaseFloatAffine<TypeExecutionPath>, 23, 8, float>& zonotope);
   static const char* queryFloatZonotopeValueAsInterval(const TGBasicFloatZonotope<Numerics::UnsignedBaseStoreTraits, FLOAT_REAL_BITS_NUMBER, TBaseFloatAffine<TypeExecutionPath>, 23, 8, float>& zonotope);
   static const char* queryDoubleZonotopeValue(const TGBasicFloatZonotope<Numerics::UnsignedBaseStoreTraits, FLOAT_REAL_BITS_NUMBER, TBaseFloatAffine<TypeExecutionPath>, 52, 11, double>& zonotope);
   static const char* queryDoubleZonotopeValueAsInterval(const TGBasicFloatZonotope<Numerics::UnsignedBaseStoreTraits, FLOAT_REAL_BITS_NUMBER, TBaseFloatAffine<TypeExecutionPath>, 52, 11, double>& zonotope);
   static const char* queryLongDoubleZonotopeValue(const TGBasicFloatZonotope<Numerics::UnsignedBaseStoreTraits, FLOAT_REAL_BITS_NUMBER, TBaseFloatAffine<TypeExecutionPath>,
      TFloatDigits<long double>::UBitSizeMantissa, TFloatDigits<long double>::UBitSizeExponent, long double>& zonotope);
   static const char* queryLongDoubleZonotopeValueAsInterval(const TGBasicFloatZonotope<Numerics::UnsignedBaseStoreTraits, FLOAT_REAL_BITS_NUMBER, TBaseFloatAffine<TypeExecutionPath>,
      TFloatDigits<long double>::UBitSizeMantissa, TFloatDigits<long double>::UBitSizeExponent, long double>& zonotope);

   static const char* (*pqueryDebugRealValue)(const BuiltReal&);
   static const char* (*pqueryEquationValue)(const TGEquation<Numerics::UnsignedBaseStoreTraits, FLOAT_REAL_BITS_NUMBER, TBaseFloatAffine<TypeExecutionPath>>&);
   static const char* (*pqueryEquationValueAsInterval)(const TGEquation<Numerics::UnsignedBaseStoreTraits, FLOAT_REAL_BITS_NUMBER, TBaseFloatAffine<TypeExecutionPath>>&);
   static const char* (*pqueryBaseZonotopeValue)(const TGBaseFloatZonotope<Numerics::UnsignedBaseStoreTraits, FLOAT_REAL_BITS_NUMBER, TBaseFloatAffine<TypeExecutionPath>>&);
   static const char* (*pqueryBaseZonotopeValueAsInterval)(const TGBaseFloatZonotope<Numerics::UnsignedBaseStoreTraits, FLOAT_REAL_BITS_NUMBER, TBaseFloatAffine<TypeExecutionPath>>&);
   static const char* (*pqueryFloatZonotopeValue)(const TGBasicFloatZonotope<Numerics::UnsignedBaseStoreTraits, FLOAT_REAL_BITS_NUMBER, TBaseFloatAffine<TypeExecutionPath>, 23, 8, float>&);
   static const char* (*pqueryFloatZonotopeValueAsInterval)(const TGBasicFloatZonotope<Numerics::UnsignedBaseStoreTraits, FLOAT_REAL_BITS_NUMBER, TBaseFloatAffine<TypeExecutionPath>, 23, 8, float>&);
   static const char* (*pqueryDoubleZonotopeValue)(const TGBasicFloatZonotope<Numerics::UnsignedBaseStoreTraits, FLOAT_REAL_BITS_NUMBER, TBaseFloatAffine<TypeExecutionPath>, 52, 11, double>&);
   static const char* (*pqueryDoubleZonotopeValueAsInterval)(const TGBasicFloatZonotope<Numerics::UnsignedBaseStoreTraits, FLOAT_REAL_BITS_NUMBER, TBaseFloatAffine<TypeExecutionPath>, 52, 11, double>&);
   static const char* (*pqueryLongDoubleZonotopeValue)(const TGBasicFloatZonotope<Numerics::UnsignedBaseStoreTraits, FLOAT_REAL_BITS_NUMBER, TBaseFloatAffine<TypeExecutionPath>,
      TFloatDigits<long double>::UBitSizeMantissa, TFloatDigits<long double>::UBitSizeExponent, long double>&);
   static const char* (*pqueryLongDoubleZonotopeValueAsInterval)(const TGBasicFloatZonotope<Numerics::UnsignedBaseStoreTraits, FLOAT_REAL_BITS_NUMBER, TBaseFloatAffine<TypeExecutionPath>,
      TFloatDigits<long double>::UBitSizeMantissa, TFloatDigits<long double>::UBitSizeExponent, long double>&);
#endif

  public:
   static void splitBranches(const char* file, int line)
      {  if (inherited::getMode() == BaseExecutionPath::MRealAndImplementation)
            inherited::notifyPossibleSplit(file, line);
      }

   template <class TypeFloatAffine>
   void assumeCompare(const TypeFloatAffine& source, bool isOnlyInterval) const;
   template <class TypeFloatAffine>
   void writeCompare(const TypeFloatAffine& source, bool isOnlyInterval) const;
   template <class TypeFloatAffine>
   void writeBranchCompare(const TypeFloatAffine& source, bool isUnstable) const;
   template <class TypeFloatAffine>
   void notifyForCompare(const TypeFloatAffine& source) const
      {  BaseExecutionPath::Mode mode;
         bool doesAssume;
         bool hasOutput;
         if (inherited::fSupportVerbose || inherited::fSupportThreshold) {
            mode = inherited::getMode();
            doesAssume = inherited::doesFollowFlow()
                  && mode == BaseExecutionPath::MRealAndImplementation;
            hasOutput = inherited::hasOutput();
         };
         if (inherited::fSupportVerbose) {
            if (doesAssume || hasOutput) {
               if (doesAssume && inherited::doesAssumeInput()) {
                  inherited::assumeSourceLine();
                  assumeCompare(source, false);
               };
               if (hasOutput) {
                  inherited::writeSourceLine();
                  writeCompare(source, false);
               };
            };
         };
         if (inherited::fSupportThreshold) {
            if ((doesAssume && inherited::hasThreshold()) || (hasOutput && mode == BaseExecutionPath::MRealAndImplementation)) {
               if (doesAssume && inherited::doesAssumeInput())
                  inherited::assumeThresholdDetection(source);
               if (hasOutput)
                  inherited::updateThresholdDetection(source);
            };
         };
      }
   template <class TypeFloatAffine>
   void notifyForBranchCompare(const TypeFloatAffine& source, bool isUnstable) const
      {  if (inherited::doesFollowFlow() && inherited::getMode() == BaseExecutionPath::MRealAndImplementation
               && inherited::doesAssumeInput())
            inherited::assumeSourceLine();
         if (inherited::hasOutput()) {
            inherited::writeSourceLine();
            writeBranchCompare(source, isUnstable);
         };
      }
   template <class TypeFloatAffine>
   void notifyForDivisionByZero(const TypeFloatAffine& source) const
      {  if (inherited::doesFollowFlow() && inherited::getMode() == BaseExecutionPath::MRealAndImplementation
               && inherited::doesAssumeInput()) {
            inherited::assumeSourceLine();
            inherited::assumeDivisionByZero();
         };
         if (inherited::hasOutput()) {
            inherited::assumeSourceLine();
            inherited::writeDivisionByZero();
         };
      }
   template <class TypeFloatAffine>
   void notifyForNegativeSqrt(const TypeFloatAffine& source) const
      {  if (inherited::doesFollowFlow() && inherited::getMode() == BaseExecutionPath::MRealAndImplementation
               && inherited::doesAssumeInput()) {
            inherited::assumeSourceLine();
            inherited::assumeNegativeSqrt();
         };
         if (inherited::hasOutput()) {
            inherited::writeSourceLine();
            inherited::writeNegativeSqrt();
         };
      }
   template <class TypeFloatAffine>
   void notifyForNegativePow(const TypeFloatAffine& source) const
      {  if (inherited::doesFollowFlow() && inherited::getMode() == BaseExecutionPath::MRealAndImplementation
               && inherited::doesAssumeInput()) {
            inherited::assumeSourceLine();
            inherited::assumeNegativePow();
         };
         if (inherited::hasOutput()) {
            inherited::writeSourceLine();
            inherited::writeNegativePow();
         };
      }
   template <class TypeFloatAffine>
   void notifyForNegativeOrNulLog(const TypeFloatAffine& source) const
      {  if (inherited::doesFollowFlow() && inherited::getMode() == BaseExecutionPath::MRealAndImplementation
               && inherited::doesAssumeInput()) {
            inherited::assumeSourceLine();
            inherited::assumeNegativeOrNulLog();
         };
         if (inherited::hasOutput()) {
            inherited::writeSourceLine();
            inherited::writeNegativeOrNulLog();
         };
      }

  protected:
   static int getCasesNumber(BaseExecutionPath::BooleanChoice realChoice,
         BaseExecutionPath::BooleanChoice errorChoice)
      {  int result;
         if (realChoice == BaseExecutionPath::BCNone)
            result = (errorChoice == BaseExecutionPath::BCTop)
               ? 2 : ((errorChoice == BaseExecutionPath::BCNone) ? 0 : 1);
         else if (errorChoice == BaseExecutionPath::BCNone)
            result = (realChoice == BaseExecutionPath::BCTop) ? 2 : 1;
         else if (realChoice == errorChoice)
            result = (realChoice == BaseExecutionPath::BCTop) ? 6 : 1;
         else if (realChoice == BaseExecutionPath::BCTop || errorChoice == BaseExecutionPath::BCTop)
            result = 3;
         else
            result = 2;
         return result;
      }
   void retrieveBranch(int cases, BaseExecutionPath::BooleanChoice& realChoice,
         BaseExecutionPath::BooleanChoice& errorChoice, bool executionResult) const;
   unsigned getConversionNumber(unsigned firstReal, unsigned lastReal,
         unsigned firstImplementation, unsigned lastImplementation) const
      {  unsigned firstCommon = (firstReal < firstImplementation) ? firstImplementation : firstReal;
         unsigned lastCommon = (lastReal < lastImplementation) ? lastReal : lastImplementation;
         unsigned commonNumber = (firstCommon <= lastCommon) ? (lastCommon-firstCommon+1) : 0;
         unsigned realNumber = (firstReal <= lastReal) ? (lastReal-firstReal+1) : 0;
         unsigned implementationNumber = (firstImplementation <= lastImplementation) ? (lastImplementation - firstImplementation+1) : 0;
         unsigned result;
         if (realNumber == 0 || implementationNumber == 0)
            result = (implementationNumber != 0) ? implementationNumber : realNumber;
         else if (realNumber == 1 || implementationNumber == 1)
            result = (implementationNumber > 1)
               ? (2*implementationNumber - commonNumber)
               : (2*realNumber - commonNumber);
         else
            result = 2*realNumber*implementationNumber - commonNumber; /* most of cases are unstable */
         return result;
      }
   static unsigned getConversionValue(unsigned realNumber, unsigned implementationNumber,
         unsigned firstReal, unsigned lastReal, unsigned firstImplementation, unsigned lastImplementation,
         unsigned commonNumber, unsigned firstCommon, unsigned lastCommon, unsigned executionResult,
         unsigned result, bool realChoice, bool implementationChoice);
   unsigned getConversionBranch(unsigned conversionNumber,
         unsigned firstReal, unsigned lastReal, unsigned firstImplementation, unsigned lastImplementation,
         bool& realChoice, bool& implementationChoice, unsigned executionResult, unsigned& alternativeResult) const;

  public:
   enum State
      {  SCompareFlow, SOnlyFloat, SOnlyReal, SOnlyFloatWithFutureMerge, SOnlyRealWithFutureMerge,
         SBadState
      };
   State queryState() const
      {  BaseExecutionPath::Mode mode = inherited::getMode();
         if (mode == BaseExecutionPath::MRealAndImplementation)
            return SCompareFlow;
         if (inherited::doesFollow())
            return (mode == BaseExecutionPath::MOnlyReal) ? SOnlyRealWithFutureMerge
               : ((mode == BaseExecutionPath::MOnlyImplementation) ? SOnlyFloatWithFutureMerge : SBadState);
         return (mode == BaseExecutionPath::MOnlyReal) ? SOnlyReal
            : ((mode == BaseExecutionPath::MOnlyImplementation) ? SOnlyFloat : SBadState);
      }

   template <class TypeFloatAffine>
   void lightPersist(const TypeFloatAffine& source, const char* prefix) const
      {  bool doesAssume = inherited::doesFollowFlow()
               && inherited::getMode() == BaseExecutionPath::MRealAndImplementation;
         bool hasOutput = inherited::hasOutput();
         if (doesAssume || hasOutput) {
            if (doesAssume && inherited::doesAssumeInput()) {
               inherited::assumeSourceLine();
               inherited::assumePrefixDomain(prefix);
               assumeCompare(source, true);
            }
            if (hasOutput) {
               inherited::writeSourceLine();
               inherited::writePrefixDomain(prefix);
               writeCompare(source, true);
            }
         }
      }
   template <class TypeFloatAffine>
   void persist(const TypeFloatAffine& source, const char* prefix) const
      {  bool doesAssume = inherited::doesFollowFlow()
               && inherited::getMode() == BaseExecutionPath::MRealAndImplementation;
         bool hasOutput = inherited::hasOutput();
         if (doesAssume || hasOutput) {
            if (doesAssume && inherited::doesAssumeInput()) {
               inherited::assumeSourceLine();
               inherited::assumePrefixDomain(prefix);
               assumeCompare(source, false);
            }
            if (hasOutput) {
               inherited::writeSourceLine();
               inherited::writePrefixDomain(prefix);
               writeCompare(source, false);
            }
         }
      }
};

template <class TypeExecutionPath>
class TMergeBranches;

template <class TypeExecutionPath, int USizeMantissa, int USizeExponent, typename TypeImplementation>
class TFloatZonotope
#if !defined(FLOAT_GENERIC_BASE_UNSIGNED) && !defined(FLOAT_GENERIC_BASE_LONG)
   :  public TBasicFloatZonotope<FLOAT_REAL_BITS_NUMBER, TBaseFloatAffine<TypeExecutionPath>, USizeMantissa, USizeExponent, TypeImplementation>
      /* TFloatZonotopeWithSymbolic<FLOAT_REAL_BITS_NUMBER, TBaseFloatAffine<TypeExecutionPath>, USizeMantissa, USizeExponent, TypeImplementation> */
#elif defined(FLOAT_GENERIC_BASE_LONG)
   :  public TGBasicFloatZonotope<Numerics::UnsignedLongBaseStoreTraits, FLOAT_REAL_BITS_NUMBER, TBaseFloatAffine<TypeExecutionPath>, USizeMantissa, USizeExponent, TypeImplementation>
      /* TFloatZonotopeWithSymbolic<Numerics::UnsignedLongBaseStoreTraits, FLOAT_REAL_BITS_NUMBER, TBaseFloatAffine<TypeExecutionPath>, USizeMantissa, USizeExponent, TypeImplementation> */
#else
   :  public TGBasicFloatZonotope<Numerics::UnsignedBaseStoreTraits, FLOAT_REAL_BITS_NUMBER, TBaseFloatAffine<TypeExecutionPath>, USizeMantissa, USizeExponent, TypeImplementation>
      /* TFloatZonotopeWithSymbolic<Numerics::UnsignedBaseStoreTraits, FLOAT_REAL_BITS_NUMBER, TBaseFloatAffine<TypeExecutionPath>, USizeMantissa, USizeExponent, TypeImplementation> */
#endif
{
  private:
#if !defined(FLOAT_GENERIC_BASE_UNSIGNED) && !defined(FLOAT_GENERIC_BASE_LONG)
   typedef TBasicFloatZonotope<FLOAT_REAL_BITS_NUMBER, TBaseFloatAffine<TypeExecutionPath>, USizeMantissa, USizeExponent, TypeImplementation> inherited;
#elif defined(FLOAT_GENERIC_BASE_LONG)
   typedef TGBasicFloatZonotope<Numerics::UnsignedLongBaseStoreTraits, FLOAT_REAL_BITS_NUMBER, TBaseFloatAffine<TypeExecutionPath>, USizeMantissa, USizeExponent, TypeImplementation> inherited;
#else
   typedef TGBasicFloatZonotope<Numerics::UnsignedBaseStoreTraits, FLOAT_REAL_BITS_NUMBER, TBaseFloatAffine<TypeExecutionPath>, USizeMantissa, USizeExponent, TypeImplementation> inherited;
#endif
   typedef TFloatZonotope<TypeExecutionPath, USizeMantissa, USizeExponent, TypeImplementation> thisType;

  public:
   const char* queryDebugValue() const;
   typedef const char* (thisType::*PQueryDebugValue)() const;
   static PQueryDebugValue pqueryDebugValue;
   const char* queryLightDebugValue() const;
   static PQueryDebugValue pqueryLightDebugValue;

   friend class TMergeBranches<TypeExecutionPath>;

  public:
   TFloatZonotope() = default;
   TFloatZonotope(TypeImplementation min, TypeImplementation max) : inherited(min, max) {}
   TFloatZonotope(int value) : inherited(value) {}
   TFloatZonotope(long int value) : inherited(value) {}
   TFloatZonotope(unsigned value) : inherited(value) {}
   TFloatZonotope(unsigned long value) : inherited(value) {}
   TFloatZonotope(const thisType& source) = default;
   TFloatZonotope(thisType&& source) = default; // [TODO] keep symbolic for constraints
   TFloatZonotope& operator=(const thisType& source) = default;
   TFloatZonotope& operator=(thisType&& source) = default; // [TODO] keep symbolic for constraints
   template <int USizeMantissaArgument, int USizeExponentArgument, typename TypeImplementationArgument>
   TFloatZonotope(const TFloatZonotope<TypeExecutionPath, USizeMantissaArgument, USizeExponentArgument, TypeImplementationArgument>& source)
      :  inherited(source) {}
   template <int USizeMantissaArgument, int USizeExponentArgument, typename TypeImplementationArgument>
   TFloatZonotope(TFloatZonotope<TypeExecutionPath, USizeMantissaArgument, USizeExponentArgument, TypeImplementationArgument>&& source)
      :  inherited(source) {}
   template <int USizeMantissaArgument, int USizeExponentArgument, typename TypeImplementationArgument>
   thisType& operator=(const TFloatZonotope<TypeExecutionPath, USizeMantissaArgument, USizeExponentArgument, TypeImplementationArgument>& source)
      {  return (thisType&) inherited::operator=(source); }
   template <int USizeMantissaArgument, int USizeExponentArgument, typename TypeImplementationArgument>
   thisType& operator=(TFloatZonotope<TypeExecutionPath, USizeMantissaArgument, USizeExponentArgument, TypeImplementationArgument>&& source)
      {  return (thisType&) inherited::operator=(source); }

   bool optimizeValue()
      {  bool result = inherited::restrictImplementationFromRealAndError();
         if (result) {
            inherited::restrictRealFromErrorAndImplementation();
            inherited::restrictRelativeErrorFromRealAndError();
         };
         return result;
      }
};

template <class TypeExecutionPath>
class TMergeBranches : public TBaseFloatAffine<TypeExecutionPath> {
  private:
   typedef TBaseFloatAffine<TypeExecutionPath> inherited;

   bool fActive;
   SymbolsManager::MergeTable mtMergeTable;
   bool fIsTableSynchronized;
   int uNumberOfFloatBranches;
   int uNumberOfRealBranches;
   unsigned uFloatSelection;
   unsigned uRealSelection;

   struct HighLevelUpdate {
#if !defined(FLOAT_GENERIC_BASE_UNSIGNED) && !defined(FLOAT_GENERIC_BASE_LONG)
      typedef TEquation<FLOAT_REAL_BITS_NUMBER, TBaseFloatAffine<TypeExecutionPath> > Equation;
#elif defined(FLOAT_GENERIC_BASE_LONG)
      typedef TGEquation<Numerics::UnsignedLongBaseStoreTraits, FLOAT_REAL_BITS_NUMBER, TBaseFloatAffine<TypeExecutionPath> > Equation;
#else // defined(FLOAT_GENERIC_BASE_UNSIGNED)
      typedef TGEquation<Numerics::UnsignedBaseStoreTraits, FLOAT_REAL_BITS_NUMBER, TBaseFloatAffine<TypeExecutionPath> > Equation;
#endif
      Equation* equation;
      BuiltReal highLevelError;

      HighLevelUpdate() : equation(nullptr) {}
      HighLevelUpdate(Equation& equationSource, const BuiltReal& highLevelErrorSource)
         :  equation(&equationSource), highLevelError(highLevelErrorSource) {}
   };
   class HighLevelUpdateVector : public COL::TVector<HighLevelUpdate> {
     private:
      typedef COL::TVector<HighLevelUpdate> inherited;

     public:
      void insertAtEnd(typename HighLevelUpdate::Equation& equation, const BuiltReal& highLevelError)
         {  inherited::insertAtEnd(HighLevelUpdate(equation, highLevelError)); }
   };
   HighLevelUpdateVector ahluHighLevelUpdates;

  public:
   TMergeBranches(const char* file, int line);

   template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
   TMergeBranches<TypeExecutionPath>& operator<<(TFloatZonotope<TypeExecutionPath, USizeMantissa, USizeExponent, TypeImplementation>& value);
   template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
   TMergeBranches<TypeExecutionPath>& operator<<(const TFloatZonotope<TypeExecutionPath, USizeMantissa, USizeExponent, TypeImplementation>& value)
      { return operator<<(const_cast<TFloatZonotope<TypeExecutionPath, USizeMantissa, USizeExponent, TypeImplementation>&>(value)); }
   bool operator<<(BaseExecutionPath::end);
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
         Numerics::DDouble::Access::ReadParameters nearestParams;
         nearestParams.setNearestRound().setRoundToEven();
         inherited::setFloat(conversion, nearestParams);
      }
};

}} // end of namespace NumericalDomains::DAffine

#endif // NumericalAnalysis_FloatAffineExecutionPathH

