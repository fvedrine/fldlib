/**************************************************************************/
/*                                                                        */
/*  This file is part of FLDLib                                           */
/*                                                                        */
/*  Copyright (C) 2017-2017                                               */
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
// File      : FloatIntervalInterface.cpp
// Description :
//   Implementation of a class of floating point intervals
//

#include <fstream>
#include <sstream>

#include "NumericalAnalysis/FloatIntervalExecutionPath.h"
#include "NumericalAnalysis/FloatIntervalExecutionPath.template"

#include "Pointer/Vector.template"

#include "FloatInstrumentation/FloatIntervalInterface.h"

#include "FloatInstrumentation/FloatInterval.inch"
#include "FloatInstrumentation/FloatInterval.incc"

namespace NumericalDomains { namespace DDoubleInterval {

typedef FloatDigitsHelper::TFloatDigits<long double> LongDoubleFloatDigits;

template class TCompareFloatInterval<LongDoubleFloatDigits::UBitSizeMantissa+1, TBaseFloatInterval<ExecutionPath>, BuiltFloat, float>;
template class TCompareFloatInterval<LongDoubleFloatDigits::UBitSizeMantissa+1, TBaseFloatInterval<ExecutionPath>, BuiltDouble, double>;
template class TCompareFloatInterval<LongDoubleFloatDigits::UBitSizeMantissa+1, TBaseFloatInterval<ExecutionPath>, BuiltLongDouble, long double>;

} // end of namespace DDoubleInterval

namespace DDoubleIntervalInterface {

#include "StandardClasses/UndefineNew.h"
PathExplorer::PathExplorer() {
   AssumeCondition(sizeof(DDoubleInterval::PathExplorer) <= UPathExplorerSize*sizeof(AlignType))
   new (content) DDoubleInterval::PathExplorer();
}

PathExplorer::PathExplorer(const PathExplorer& source) {
   new (content) DDoubleInterval::PathExplorer(*reinterpret_cast<const DDoubleInterval::PathExplorer*>(source.content));
}

PathExplorer::PathExplorer(PathExplorer&& source) {
   new (content) DDoubleInterval::PathExplorer(std::move(*reinterpret_cast<DDoubleInterval::PathExplorer*>(source.content)));
}
#include "StandardClasses/DefineNew.h"

PathExplorer::~PathExplorer() {
   reinterpret_cast<DDoubleInterval::PathExplorer*>(content)->~PathExplorer();
}

PathExplorer&
PathExplorer::operator=(const PathExplorer& asource) {
   const DDoubleInterval::PathExplorer& source = *reinterpret_cast<const DDoubleInterval::PathExplorer*>(asource.content);
   DDoubleInterval::PathExplorer& thisExplorer = *reinterpret_cast<DDoubleInterval::PathExplorer*>(content);
   thisExplorer = source;
   return *this;
}

PathExplorer&
PathExplorer::operator=(PathExplorer&& asource) {
   DDoubleInterval::PathExplorer& source = *reinterpret_cast<DDoubleInterval::PathExplorer*>(asource.content);
   DDoubleInterval::PathExplorer& thisExplorer = *reinterpret_cast<DDoubleInterval::PathExplorer*>(content);
   thisExplorer.operator=(std::move(source));
   return *this;
}

bool
PathExplorer::isFinished() {
   return reinterpret_cast<DDoubleInterval::PathExplorer*>(content)->isFinished();
}

void
ExecutionPath::setSupportAtomic() {
   DDoubleInterval::ExecutionPath::setSupportAtomic();
}

void
ExecutionPath::setSupportUnstableInLoop() {
   DDoubleInterval::ExecutionPath::setSupportUnstableInLoop();
}

void
ExecutionPath::setSupportVerbose() {
   DDoubleInterval::ExecutionPath::setSupportVerbose();
}

void
ExecutionPath::setSupportThreshold() {
   DDoubleInterval::ExecutionPath::setSupportThreshold();
}

void
ExecutionPath::setSupportFirstFollowFloat() {
   DDoubleInterval::ExecutionPath::setSupportFirstFollowFloat();
}

void
ExecutionPath::initializeGlobals(const char* fileSuffix) {
   DDoubleInterval::ExecutionPath::initializeGlobals(fileSuffix);
}

void
ExecutionPath::finalizeGlobals() {
   DDoubleInterval::ExecutionPath::finalizeGlobals();
}

void
ExecutionPath::flushOut() {
   DDoubleInterval::ExecutionPath::flushOut();
}

void
ExecutionPath::setSourceLine(const char* file, int line) {
   DDoubleInterval::ExecutionPath::setSourceLine(file, line);
}

void
ExecutionPath::writeCurrentPath(std::ostream& out) {
   DDoubleInterval::ExecutionPath::writeCurrentPath(out);
}

DDoubleInterval::PathExplorer*
ExecutionPath::getCurrentPathExplorer() {
   return DDoubleInterval::ExecutionPath::getCurrentPathExplorer();
}

void
ExecutionPath::setCurrentPathExplorer(PathExplorer* apathExplorer) {
   DDoubleInterval::PathExplorer* pathExplorer = apathExplorer
      ? reinterpret_cast<DDoubleInterval::PathExplorer*>(apathExplorer->content)
      : nullptr;
   DDoubleInterval::ExecutionPath::setCurrentPathExplorer(pathExplorer);
}

void
ExecutionPath::setCurrentPathExplorer(DDoubleInterval::PathExplorer* pathExplorer) {
   DDoubleInterval::ExecutionPath::setCurrentPathExplorer(pathExplorer);
}

DDoubleInterval::DiagnosisReadStream*
ExecutionPath::inputTraceFile() {
   return reinterpret_cast<DDoubleInterval::DiagnosisReadStream*>(DDoubleInterval::ExecutionPath::inputTraceFile());
}

bool
ExecutionPath::doesFollowFlow() {
   return DDoubleInterval::ExecutionPath::doesFollowFlow();
}

void
ExecutionPath::clearFollowFlow() {
   DDoubleInterval::ExecutionPath::clearFollowFlow();
}

void
ExecutionPath::setFollowFlow(bool doesFollowFlow, DDoubleInterval::DiagnosisReadStream* inputTraceFile) {
   DDoubleInterval::ExecutionPath::setFollowFlow(doesFollowFlow,
         reinterpret_cast<STG::IOObject::ISBase*>(inputTraceFile));
}

void
ExecutionPath::setFollowFlow() {
   DDoubleInterval::ExecutionPath::setFollowFlow();
}

#if defined __GNUC__ && GCC_VERSION >= 50000
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wstrict-aliasing"
#endif

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
const char*
TFloatInterval<USizeMantissa, USizeExponent, TypeImplementation>::queryDebugValue() const {
   return reinterpret_cast<const DDoubleInterval::TFloatInterval<DDoubleInterval::ExecutionPath, DDoubleInterval::TBuiltFloat<DDoubleInterval::LongDoubleFloatDigits::UBitSizeMantissa+1, USizeMantissa, USizeExponent>, TypeImplementation>*>(content)
      ->queryDebugValue();
}

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
const char*
TFloatInterval<USizeMantissa, USizeExponent, TypeImplementation>::queryLightDebugValue() const {
   return reinterpret_cast<const DDoubleInterval::TFloatInterval<DDoubleInterval::ExecutionPath, DDoubleInterval::TBuiltFloat<DDoubleInterval::LongDoubleFloatDigits::UBitSizeMantissa+1, USizeMantissa, USizeExponent>, TypeImplementation>*>(content)
      ->queryDebugValue();
}

#include "StandardClasses/UndefineNew.h"
template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
TFloatInterval<USizeMantissa, USizeExponent, TypeImplementation>::TFloatInterval() {
   typedef DDoubleInterval::TFloatInterval<DDoubleInterval::ExecutionPath, DDoubleInterval::TBuiltFloat<DDoubleInterval::LongDoubleFloatDigits::UBitSizeMantissa+1, USizeMantissa, USizeExponent>, TypeImplementation> Implementation;
   AssumeCondition(sizeof(Implementation) <= UFloatIntervalSize*sizeof(AlignType))
   new (content) Implementation();
}

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
TFloatInterval<USizeMantissa, USizeExponent, TypeImplementation>::TFloatInterval(float value) {
   typedef DDoubleInterval::TFloatInterval<DDoubleInterval::ExecutionPath, DDoubleInterval::TBuiltFloat<DDoubleInterval::LongDoubleFloatDigits::UBitSizeMantissa+1, USizeMantissa, USizeExponent>, TypeImplementation> Implementation;
   AssumeCondition(sizeof(Implementation) <= UFloatIntervalSize*sizeof(AlignType))
   new (content) Implementation();
   if (!DDoubleInterval::ExecutionPath::doesSupportAtomic())
      reinterpret_cast<Implementation*>(content)->initFrom(value);
   else
      reinterpret_cast<Implementation*>(content)->initFromAtomic(value);
}

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
TFloatInterval<USizeMantissa, USizeExponent, TypeImplementation>::TFloatInterval(double value) {
   typedef DDoubleInterval::TFloatInterval<DDoubleInterval::ExecutionPath, DDoubleInterval::TBuiltFloat<DDoubleInterval::LongDoubleFloatDigits::UBitSizeMantissa+1, USizeMantissa, USizeExponent>, TypeImplementation> Implementation;
   AssumeCondition(sizeof(Implementation) <= UFloatIntervalSize*sizeof(AlignType))
   new (content) Implementation();
   if (!DDoubleInterval::ExecutionPath::doesSupportAtomic())
      reinterpret_cast<Implementation*>(content)->initFrom(value);
   else
      reinterpret_cast<Implementation*>(content)->initFromAtomic(value);
}

/*
template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
TFloatInterval<USizeMantissa, USizeExponent, TypeImplementation>::TFloatInterval(long double value) {
   typedef DDoubleInterval::TFloatInterval<DDoubleInterval::ExecutionPath, DDoubleInterval::TBuiltFloat<DDoubleInterval::LongDoubleFloatDigits::UBitSizeMantissa+1, USizeMantissa, USizeExponent>, TypeImplementation> Implementation;
   AssumeCondition(sizeof(Implementation) <= UFloatIntervalSize*sizeof(AlignType))
   new (content) Implementation();
   if (!DDoubleInterval::ExecutionPath::doesSupportAtomic())
      reinterpret_cast<Implementation*>(content)->initFrom(value);
   else
      reinterpret_cast<Implementation*>(content)->initFromAtomic(value);
}
*/

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
TFloatInterval<USizeMantissa, USizeExponent, TypeImplementation>::TFloatInterval(long double value) {
   typedef DDoubleInterval::TFloatInterval<DDoubleInterval::ExecutionPath, DDoubleInterval::TBuiltFloat<DDoubleInterval::LongDoubleFloatDigits::UBitSizeMantissa+1, USizeMantissa, USizeExponent>, TypeImplementation> Implementation;
   AssumeCondition(sizeof(Implementation) <= UFloatIntervalSize*sizeof(AlignType))
   new (content) Implementation();

   typedef DDoubleInterval::FloatDigitsHelper::TFloatDigits<long double> FloatDigits;
   DDoubleInterval::TFloatInterval<DDoubleInterval::ExecutionPath,
      DDoubleInterval::TBuiltFloat<
         FloatDigits::UBitSizeMantissa+1,
         FloatDigits::UBitSizeMantissa,
         FloatDigits::UBitSizeExponent>, long double> receiver;
   if (!DDoubleInterval::ExecutionPath::doesSupportAtomic())
      receiver.initFrom(value);
   else
      receiver.initFromAtomic(value);
   reinterpret_cast<Implementation*>(content)->operator=(receiver);
}

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
TFloatInterval<USizeMantissa, USizeExponent, TypeImplementation>::TFloatInterval(
      TypeImplementation min, TypeImplementation max) {
   typedef DDoubleInterval::TFloatInterval<DDoubleInterval::ExecutionPath, DDoubleInterval::TBuiltFloat<DDoubleInterval::LongDoubleFloatDigits::UBitSizeMantissa+1, USizeMantissa, USizeExponent>, TypeImplementation> Implementation;
   AssumeCondition(sizeof(Implementation) <= UFloatIntervalSize*sizeof(AlignType))
   new (content) Implementation(min, max);
}

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
TFloatInterval<USizeMantissa, USizeExponent, TypeImplementation>::TFloatInterval(
      TypeImplementation min, TypeImplementation max,
      TypeImplementation errmin, TypeImplementation errmax) {
   typedef DDoubleInterval::TFloatInterval<DDoubleInterval::ExecutionPath, DDoubleInterval::TBuiltFloat<DDoubleInterval::LongDoubleFloatDigits::UBitSizeMantissa+1, USizeMantissa, USizeExponent>, TypeImplementation> Implementation;
   AssumeCondition(sizeof(Implementation) <= UFloatIntervalSize*sizeof(AlignType))
   new (content) Implementation(errmin < 0 ? min+errmin : min, errmax > 0 ? max+errmax : max);
}

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
TFloatInterval<USizeMantissa, USizeExponent, TypeImplementation>::TFloatInterval(int value) {
   typedef DDoubleInterval::TFloatInterval<DDoubleInterval::ExecutionPath, DDoubleInterval::TBuiltFloat<DDoubleInterval::LongDoubleFloatDigits::UBitSizeMantissa+1, USizeMantissa, USizeExponent>, TypeImplementation> Implementation;
   AssumeCondition(sizeof(Implementation) <= UFloatIntervalSize*sizeof(AlignType))
   new (content) Implementation(value);
}

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
TFloatInterval<USizeMantissa, USizeExponent, TypeImplementation>::TFloatInterval(long int value) {
   typedef DDoubleInterval::TFloatInterval<DDoubleInterval::ExecutionPath, DDoubleInterval::TBuiltFloat<DDoubleInterval::LongDoubleFloatDigits::UBitSizeMantissa+1, USizeMantissa, USizeExponent>, TypeImplementation> Implementation;
   AssumeCondition(sizeof(Implementation) <= UFloatIntervalSize*sizeof(AlignType))
   new (content) Implementation(value);
}

/*
template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
TFloatInterval<USizeMantissa, USizeExponent, TypeImplementation>::TFloatInterval(unsigned value) {
   typedef DDoubleInterval::TFloatInterval<DDoubleInterval::ExecutionPath, DDoubleInterval::TBuiltFloat<DDoubleInterval::LongDoubleFloatDigits::UBitSizeMantissa+1, USizeMantissa, USizeExponent>, TypeImplementation> Implementation;
   AssumeCondition(sizeof(Implementation) <= UFloatIntervalSize*sizeof(AlignType))
   new (content) Implementation(value);
}

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
TFloatInterval<USizeMantissa, USizeExponent, TypeImplementation>::TFloatInterval(unsigned long value) {
   typedef DDoubleInterval::TFloatInterval<DDoubleInterval::ExecutionPath, DDoubleInterval::TBuiltFloat<DDoubleInterval::LongDoubleFloatDigits::UBitSizeMantissa+1, USizeMantissa, USizeExponent>, TypeImplementation> Implementation;
   AssumeCondition(sizeof(Implementation) <= UFloatIntervalSize*sizeof(AlignType))
   new (content) Implementation(value);
}
*/

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
TFloatInterval<USizeMantissa, USizeExponent, TypeImplementation>::TFloatInterval(const thisType& asource) {
   typedef DDoubleInterval::TFloatInterval<DDoubleInterval::ExecutionPath, DDoubleInterval::TBuiltFloat<DDoubleInterval::LongDoubleFloatDigits::UBitSizeMantissa+1, USizeMantissa, USizeExponent>, TypeImplementation> Implementation;
   const Implementation& source = *reinterpret_cast<const Implementation*>(asource.content);
   new (content) Implementation(source);
}

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
TFloatInterval<USizeMantissa, USizeExponent, TypeImplementation>::TFloatInterval(thisType&& asource) {
   typedef DDoubleInterval::TFloatInterval<DDoubleInterval::ExecutionPath, DDoubleInterval::TBuiltFloat<DDoubleInterval::LongDoubleFloatDigits::UBitSizeMantissa+1, USizeMantissa, USizeExponent>, TypeImplementation> Implementation;
   Implementation& source = *reinterpret_cast<Implementation*>(asource.content);
   new (content) Implementation(std::move(source));
}

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
template <int USizeMantissaArgument, int USizeExponentArgument, typename TypeImplementationArgument>
TFloatInterval<USizeMantissa, USizeExponent, TypeImplementation>::TFloatInterval(
      const TFloatInterval<USizeMantissaArgument, USizeExponentArgument, TypeImplementationArgument>& asource) {
   typedef DDoubleInterval::TFloatInterval<DDoubleInterval::ExecutionPath, DDoubleInterval::TBuiltFloat<DDoubleInterval::LongDoubleFloatDigits::UBitSizeMantissa+1, USizeMantissa, USizeExponent>, TypeImplementation> Implementation;
   typedef DDoubleInterval::TFloatInterval<DDoubleInterval::ExecutionPath, DDoubleInterval::TBuiltFloat<DDoubleInterval::LongDoubleFloatDigits::UBitSizeMantissa+1, USizeMantissaArgument, USizeExponentArgument>, TypeImplementationArgument> ImplementationSource;
   const ImplementationSource& source = *reinterpret_cast<const ImplementationSource*>(asource.content);
   new (content) Implementation(source);
}

#include "StandardClasses/DefineNew.h"

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
TFloatInterval<USizeMantissa, USizeExponent, TypeImplementation>::~TFloatInterval() {
   typedef DDoubleInterval::TFloatInterval<DDoubleInterval::ExecutionPath, DDoubleInterval::TBuiltFloat<DDoubleInterval::LongDoubleFloatDigits::UBitSizeMantissa+1, USizeMantissa, USizeExponent>, TypeImplementation> Implementation;
   reinterpret_cast<Implementation*>(content)->~Implementation();
}

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
TFloatInterval<USizeMantissa, USizeExponent, TypeImplementation>&
TFloatInterval<USizeMantissa, USizeExponent, TypeImplementation>::operator=(const thisType& asource) {
   typedef DDoubleInterval::TFloatInterval<DDoubleInterval::ExecutionPath, DDoubleInterval::TBuiltFloat<DDoubleInterval::LongDoubleFloatDigits::UBitSizeMantissa+1, USizeMantissa, USizeExponent>, TypeImplementation> Implementation;
   const Implementation& source = *reinterpret_cast<const Implementation*>(asource.content);
   Implementation& thisInterval = *reinterpret_cast<Implementation*>(content);
   thisInterval.operator=(source);
   return *this;
}

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
TFloatInterval<USizeMantissa, USizeExponent, TypeImplementation>&
TFloatInterval<USizeMantissa, USizeExponent, TypeImplementation>::operator=(thisType&& asource) {
   typedef DDoubleInterval::TFloatInterval<DDoubleInterval::ExecutionPath, DDoubleInterval::TBuiltFloat<DDoubleInterval::LongDoubleFloatDigits::UBitSizeMantissa+1, USizeMantissa, USizeExponent>, TypeImplementation> Implementation;
   Implementation& source = *reinterpret_cast<Implementation*>(asource.content);
   Implementation& thisInterval = *reinterpret_cast<Implementation*>(content);
   thisInterval.operator=(std::move(source));
   return *this;
}

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
template <int USizeMantissaArgument, int USizeExponentArgument, typename TypeImplementationArgument>
TFloatInterval<USizeMantissa, USizeExponent, TypeImplementation>&
TFloatInterval<USizeMantissa, USizeExponent, TypeImplementation>::operator=(
      const TFloatInterval<USizeMantissaArgument, USizeExponentArgument, TypeImplementationArgument>& asource) {
   typedef DDoubleInterval::TFloatInterval<DDoubleInterval::ExecutionPath, DDoubleInterval::TBuiltFloat<DDoubleInterval::LongDoubleFloatDigits::UBitSizeMantissa+1, USizeMantissa, USizeExponent>, TypeImplementation> Implementation;
   typedef DDoubleInterval::TFloatInterval<DDoubleInterval::ExecutionPath, DDoubleInterval::TBuiltFloat<DDoubleInterval::LongDoubleFloatDigits::UBitSizeMantissa+1, USizeMantissaArgument, USizeExponentArgument>, TypeImplementationArgument> ImplementationSource;
   const ImplementationSource& source = *reinterpret_cast<const ImplementationSource*>(asource.content);
   Implementation& thisInterval = *reinterpret_cast<Implementation*>(content);
   thisInterval.operator=(source);
   return *this;
}

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
void
TFloatInterval<USizeMantissa, USizeExponent, TypeImplementation>::mergeWith(const thisType& asource) {
   typedef DDoubleInterval::TFloatInterval<DDoubleInterval::ExecutionPath, DDoubleInterval::TBuiltFloat<DDoubleInterval::LongDoubleFloatDigits::UBitSizeMantissa+1, USizeMantissa, USizeExponent>, TypeImplementation> Implementation;
   const Implementation& source = *reinterpret_cast<const Implementation*>(asource.content);
   Implementation& thisInterval = *reinterpret_cast<Implementation*>(content);
   thisInterval.mergeWith(source);
}

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
void
TFloatInterval<USizeMantissa, USizeExponent, TypeImplementation>::mergeWith(thisType&& asource) {
   typedef DDoubleInterval::TFloatInterval<DDoubleInterval::ExecutionPath, DDoubleInterval::TBuiltFloat<DDoubleInterval::LongDoubleFloatDigits::UBitSizeMantissa+1, USizeMantissa, USizeExponent>, TypeImplementation> Implementation;
   Implementation& source = *reinterpret_cast<Implementation*>(asource.content);
   Implementation& thisInterval = *reinterpret_cast<Implementation*>(content);
   thisInterval.mergeWith(std::move(source));
}

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
void
TFloatInterval<USizeMantissa, USizeExponent, TypeImplementation>::writeImplementation(
      std::ostream& out) const {
   typedef DDoubleInterval::TFloatInterval<DDoubleInterval::ExecutionPath, DDoubleInterval::TBuiltFloat<DDoubleInterval::LongDoubleFloatDigits::UBitSizeMantissa+1, USizeMantissa, USizeExponent>, TypeImplementation> Implementation;
   out << reinterpret_cast<const Implementation*>(content)->asImplementation();
}

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
void
TFloatInterval<USizeMantissa, USizeExponent, TypeImplementation>::readImplementation(
      std::istream& in) {
   typedef DDoubleInterval::TFloatInterval<DDoubleInterval::ExecutionPath, DDoubleInterval::TBuiltFloat<DDoubleInterval::LongDoubleFloatDigits::UBitSizeMantissa+1, USizeMantissa, USizeExponent>, TypeImplementation> Implementation;
   decltype(reinterpret_cast<const Implementation*>(content)->asImplementation()) result; 
   in >> result;
   *reinterpret_cast<Implementation*>(content) = Implementation(result, result);
}

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
bool
TFloatInterval<USizeMantissa, USizeExponent, TypeImplementation>::operator<(const thisType& asource) const {
   typedef DDoubleInterval::TFloatInterval<DDoubleInterval::ExecutionPath, DDoubleInterval::TBuiltFloat<DDoubleInterval::LongDoubleFloatDigits::UBitSizeMantissa+1, USizeMantissa, USizeExponent>, TypeImplementation> Implementation;
   const Implementation& source = *reinterpret_cast<const Implementation*>(asource.content);
   const Implementation& thisInterval = *reinterpret_cast<const Implementation*>(content);
   return thisInterval.operator<(source);
}

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
bool
TFloatInterval<USizeMantissa, USizeExponent, TypeImplementation>::operator<(TypeImplementation source) const {
   typedef DDoubleInterval::TFloatInterval<DDoubleInterval::ExecutionPath, DDoubleInterval::TBuiltFloat<DDoubleInterval::LongDoubleFloatDigits::UBitSizeMantissa+1, USizeMantissa, USizeExponent>, TypeImplementation> Implementation;
   const Implementation& thisInterval = *reinterpret_cast<const Implementation*>(content);
   TFloatInterval<USizeMantissa, USizeExponent, TypeImplementation> asource(source);
   return thisInterval.operator<(*reinterpret_cast<const Implementation*>(asource.content));
}

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
bool
TFloatInterval<USizeMantissa, USizeExponent, TypeImplementation>::operator<=(const thisType& asource) const {
   typedef DDoubleInterval::TFloatInterval<DDoubleInterval::ExecutionPath, DDoubleInterval::TBuiltFloat<DDoubleInterval::LongDoubleFloatDigits::UBitSizeMantissa+1, USizeMantissa, USizeExponent>, TypeImplementation> Implementation;
   const Implementation& source = *reinterpret_cast<const Implementation*>(asource.content);
   const Implementation& thisInterval = *reinterpret_cast<const Implementation*>(content);
   return thisInterval.operator<=(source);
}

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
bool
TFloatInterval<USizeMantissa, USizeExponent, TypeImplementation>::operator<=(TypeImplementation source) const {
   typedef DDoubleInterval::TFloatInterval<DDoubleInterval::ExecutionPath, DDoubleInterval::TBuiltFloat<DDoubleInterval::LongDoubleFloatDigits::UBitSizeMantissa+1, USizeMantissa, USizeExponent>, TypeImplementation> Implementation;
   const Implementation& thisInterval = *reinterpret_cast<const Implementation*>(content);
   TFloatInterval<USizeMantissa, USizeExponent, TypeImplementation> asource(source);
   return thisInterval.operator<=(*reinterpret_cast<const Implementation*>(asource.content));
}

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
bool
TFloatInterval<USizeMantissa, USizeExponent, TypeImplementation>::operator==(const thisType& asource) const {
   typedef DDoubleInterval::TFloatInterval<DDoubleInterval::ExecutionPath, DDoubleInterval::TBuiltFloat<DDoubleInterval::LongDoubleFloatDigits::UBitSizeMantissa+1, USizeMantissa, USizeExponent>, TypeImplementation> Implementation;
   const Implementation& source = *reinterpret_cast<const Implementation*>(asource.content);
   const Implementation& thisInterval = *reinterpret_cast<const Implementation*>(content);
   return thisInterval.operator==(source);
}

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
bool
TFloatInterval<USizeMantissa, USizeExponent, TypeImplementation>::operator==(TypeImplementation source) const {
   typedef DDoubleInterval::TFloatInterval<DDoubleInterval::ExecutionPath, DDoubleInterval::TBuiltFloat<DDoubleInterval::LongDoubleFloatDigits::UBitSizeMantissa+1, USizeMantissa, USizeExponent>, TypeImplementation> Implementation;
   const Implementation& thisInterval = *reinterpret_cast<const Implementation*>(content);
   TFloatInterval<USizeMantissa, USizeExponent, TypeImplementation> asource(source);
   return thisInterval.operator==(*reinterpret_cast<const Implementation*>(asource.content));
}

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
bool
TFloatInterval<USizeMantissa, USizeExponent, TypeImplementation>::operator!=(const thisType& asource) const {
   typedef DDoubleInterval::TFloatInterval<DDoubleInterval::ExecutionPath, DDoubleInterval::TBuiltFloat<DDoubleInterval::LongDoubleFloatDigits::UBitSizeMantissa+1, USizeMantissa, USizeExponent>, TypeImplementation> Implementation;
   const Implementation& source = *reinterpret_cast<const Implementation*>(asource.content);
   const Implementation& thisInterval = *reinterpret_cast<const Implementation*>(content);
   return thisInterval.operator!=(source);
}

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
bool
TFloatInterval<USizeMantissa, USizeExponent, TypeImplementation>::operator!=(TypeImplementation source) const {
   typedef DDoubleInterval::TFloatInterval<DDoubleInterval::ExecutionPath, DDoubleInterval::TBuiltFloat<DDoubleInterval::LongDoubleFloatDigits::UBitSizeMantissa+1, USizeMantissa, USizeExponent>, TypeImplementation> Implementation;
   const Implementation& thisInterval = *reinterpret_cast<const Implementation*>(content);
   TFloatInterval<USizeMantissa, USizeExponent, TypeImplementation> asource(source);
   return thisInterval.operator!=(*reinterpret_cast<const Implementation*>(asource.content));
}

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
bool
TFloatInterval<USizeMantissa, USizeExponent, TypeImplementation>::operator>=(const thisType& asource) const {
   typedef DDoubleInterval::TFloatInterval<DDoubleInterval::ExecutionPath, DDoubleInterval::TBuiltFloat<DDoubleInterval::LongDoubleFloatDigits::UBitSizeMantissa+1, USizeMantissa, USizeExponent>, TypeImplementation> Implementation;
   const Implementation& source = *reinterpret_cast<const Implementation*>(asource.content);
   const Implementation& thisInterval = *reinterpret_cast<const Implementation*>(content);
   return thisInterval.operator>=(source);
}

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
bool
TFloatInterval<USizeMantissa, USizeExponent, TypeImplementation>::operator>=(TypeImplementation source) const {
   typedef DDoubleInterval::TFloatInterval<DDoubleInterval::ExecutionPath, DDoubleInterval::TBuiltFloat<DDoubleInterval::LongDoubleFloatDigits::UBitSizeMantissa+1, USizeMantissa, USizeExponent>, TypeImplementation> Implementation;
   const Implementation& thisInterval = *reinterpret_cast<const Implementation*>(content);
   TFloatInterval<USizeMantissa, USizeExponent, TypeImplementation> asource(source);
   return thisInterval.operator>=(*reinterpret_cast<const Implementation*>(asource.content));
}

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
bool
TFloatInterval<USizeMantissa, USizeExponent, TypeImplementation>::operator>(const thisType& asource) const {
   typedef DDoubleInterval::TFloatInterval<DDoubleInterval::ExecutionPath, DDoubleInterval::TBuiltFloat<DDoubleInterval::LongDoubleFloatDigits::UBitSizeMantissa+1, USizeMantissa, USizeExponent>, TypeImplementation> Implementation;
   const Implementation& source = *reinterpret_cast<const Implementation*>(asource.content);
   const Implementation& thisInterval = *reinterpret_cast<const Implementation*>(content);
   return thisInterval.operator>(source);
}

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
bool
TFloatInterval<USizeMantissa, USizeExponent, TypeImplementation>::operator>(TypeImplementation source) const {
   typedef DDoubleInterval::TFloatInterval<DDoubleInterval::ExecutionPath, DDoubleInterval::TBuiltFloat<DDoubleInterval::LongDoubleFloatDigits::UBitSizeMantissa+1, USizeMantissa, USizeExponent>, TypeImplementation> Implementation;
   const Implementation& thisInterval = *reinterpret_cast<const Implementation*>(content);
   TFloatInterval<USizeMantissa, USizeExponent, TypeImplementation> asource(source);
   return thisInterval.operator>(*reinterpret_cast<const Implementation*>(asource.content));
}

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
TFloatInterval<USizeMantissa, USizeExponent, TypeImplementation>&
TFloatInterval<USizeMantissa, USizeExponent, TypeImplementation>::operator+=(const thisType& asource) {
   typedef DDoubleInterval::TFloatInterval<DDoubleInterval::ExecutionPath, DDoubleInterval::TBuiltFloat<DDoubleInterval::LongDoubleFloatDigits::UBitSizeMantissa+1, USizeMantissa, USizeExponent>, TypeImplementation> Implementation;
   const Implementation& source = *reinterpret_cast<const Implementation*>(asource.content);
   Implementation& thisInterval = *reinterpret_cast<Implementation*>(content);
   thisInterval += source;
   return *this;
}

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
TFloatInterval<USizeMantissa, USizeExponent, TypeImplementation>&
TFloatInterval<USizeMantissa, USizeExponent, TypeImplementation>::operator-=(const thisType& asource) {
   typedef DDoubleInterval::TFloatInterval<DDoubleInterval::ExecutionPath, DDoubleInterval::TBuiltFloat<DDoubleInterval::LongDoubleFloatDigits::UBitSizeMantissa+1, USizeMantissa, USizeExponent>, TypeImplementation> Implementation;
   const Implementation& source = *reinterpret_cast<const Implementation*>(asource.content);
   Implementation& thisInterval = *reinterpret_cast<Implementation*>(content);
   thisInterval -= source;
   return *this;
}

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
TFloatInterval<USizeMantissa, USizeExponent, TypeImplementation>&
TFloatInterval<USizeMantissa, USizeExponent, TypeImplementation>::operator*=(const thisType& asource) {
   typedef DDoubleInterval::TFloatInterval<DDoubleInterval::ExecutionPath, DDoubleInterval::TBuiltFloat<DDoubleInterval::LongDoubleFloatDigits::UBitSizeMantissa+1, USizeMantissa, USizeExponent>, TypeImplementation> Implementation;
   const Implementation& source = *reinterpret_cast<const Implementation*>(asource.content);
   Implementation& thisInterval = *reinterpret_cast<Implementation*>(content);
   thisInterval *= source;
   return *this;
}

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
TFloatInterval<USizeMantissa, USizeExponent, TypeImplementation>&
TFloatInterval<USizeMantissa, USizeExponent, TypeImplementation>::operator/=(const thisType& asource) {
   typedef DDoubleInterval::TFloatInterval<DDoubleInterval::ExecutionPath, DDoubleInterval::TBuiltFloat<DDoubleInterval::LongDoubleFloatDigits::UBitSizeMantissa+1, USizeMantissa, USizeExponent>, TypeImplementation> Implementation;
   const Implementation& source = *reinterpret_cast<const Implementation*>(asource.content);
   Implementation& thisInterval = *reinterpret_cast<Implementation*>(content);
   thisInterval /= source;
   return *this;
}

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
void
TFloatInterval<USizeMantissa, USizeExponent, TypeImplementation>::oppositeAssign() {
   typedef DDoubleInterval::TFloatInterval<DDoubleInterval::ExecutionPath, DDoubleInterval::TBuiltFloat<DDoubleInterval::LongDoubleFloatDigits::UBitSizeMantissa+1, USizeMantissa, USizeExponent>, TypeImplementation> Implementation;
   reinterpret_cast<Implementation*>(content)->oppositeAssign();
}

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
TFloatInterval<USizeMantissa, USizeExponent, TypeImplementation>::operator TypeImplementation() const {
   typedef DDoubleInterval::TFloatInterval<DDoubleInterval::ExecutionPath, DDoubleInterval::TBuiltFloat<DDoubleInterval::LongDoubleFloatDigits::UBitSizeMantissa+1, USizeMantissa, USizeExponent>, TypeImplementation> Implementation;
   return reinterpret_cast<const Implementation*>(content)->asImplementation();
}

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
TFloatInterval<USizeMantissa, USizeExponent, TypeImplementation>::operator int() const {
   typedef DDoubleInterval::TFloatInterval<DDoubleInterval::ExecutionPath, DDoubleInterval::TBuiltFloat<DDoubleInterval::LongDoubleFloatDigits::UBitSizeMantissa+1, USizeMantissa, USizeExponent>, TypeImplementation> Implementation;
   typedef Numerics::DDouble::Access::ReadParameters ReadParametersBase;
   return reinterpret_cast<const Implementation*>(content)->asInt(ReadParametersBase::RMZero);
}

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
TFloatInterval<USizeMantissa, USizeExponent, TypeImplementation>::operator short int() const {
   typedef DDoubleInterval::TFloatInterval<DDoubleInterval::ExecutionPath, DDoubleInterval::TBuiltFloat<DDoubleInterval::LongDoubleFloatDigits::UBitSizeMantissa+1, USizeMantissa, USizeExponent>, TypeImplementation> Implementation;
   typedef Numerics::DDouble::Access::ReadParameters ReadParametersBase;
   return reinterpret_cast<const Implementation*>(content)->asInt(ReadParametersBase::RMZero);
}

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
TFloatInterval<USizeMantissa, USizeExponent, TypeImplementation>::operator unsigned() const {
   typedef DDoubleInterval::TFloatInterval<DDoubleInterval::ExecutionPath, DDoubleInterval::TBuiltFloat<DDoubleInterval::LongDoubleFloatDigits::UBitSizeMantissa+1, USizeMantissa, USizeExponent>, TypeImplementation> Implementation;
   typedef Numerics::DDouble::Access::ReadParameters ReadParametersBase;
   return reinterpret_cast<const Implementation*>(content)->asUnsigned(ReadParametersBase::RMZero);
}

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
TFloatInterval<USizeMantissa, USizeExponent, TypeImplementation>::operator short unsigned() const {
   typedef DDoubleInterval::TFloatInterval<DDoubleInterval::ExecutionPath, DDoubleInterval::TBuiltFloat<DDoubleInterval::LongDoubleFloatDigits::UBitSizeMantissa+1, USizeMantissa, USizeExponent>, TypeImplementation> Implementation;
   typedef Numerics::DDouble::Access::ReadParameters ReadParametersBase;
   return reinterpret_cast<const Implementation*>(content)->asUnsigned(ReadParametersBase::RMZero);
}

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
TFloatInterval<USizeMantissa, USizeExponent, TypeImplementation>::operator long int() const {
   typedef DDoubleInterval::TFloatInterval<DDoubleInterval::ExecutionPath, DDoubleInterval::TBuiltFloat<DDoubleInterval::LongDoubleFloatDigits::UBitSizeMantissa+1, USizeMantissa, USizeExponent>, TypeImplementation> Implementation;
   typedef Numerics::DDouble::Access::ReadParameters ReadParametersBase;
   return reinterpret_cast<const Implementation*>(content)->asLongInt(ReadParametersBase::RMZero);
}

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
TFloatInterval<USizeMantissa, USizeExponent, TypeImplementation>::operator unsigned long() const {
   typedef DDoubleInterval::TFloatInterval<DDoubleInterval::ExecutionPath, DDoubleInterval::TBuiltFloat<DDoubleInterval::LongDoubleFloatDigits::UBitSizeMantissa+1, USizeMantissa, USizeExponent>, TypeImplementation> Implementation;
   typedef Numerics::DDouble::Access::ReadParameters ReadParametersBase;
   return reinterpret_cast<const Implementation*>(content)->asUnsignedLong(ReadParametersBase::RMZero);
}

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
void
TFloatInterval<USizeMantissa, USizeExponent, TypeImplementation>::sqrtAssign() {
   typedef DDoubleInterval::TFloatInterval<DDoubleInterval::ExecutionPath, DDoubleInterval::TBuiltFloat<DDoubleInterval::LongDoubleFloatDigits::UBitSizeMantissa+1, USizeMantissa, USizeExponent>, TypeImplementation> Implementation;
   reinterpret_cast<Implementation*>(content)->sqrtAssign();
}

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
void
TFloatInterval<USizeMantissa, USizeExponent, TypeImplementation>::sinAssign() {
   typedef DDoubleInterval::TFloatInterval<DDoubleInterval::ExecutionPath, DDoubleInterval::TBuiltFloat<DDoubleInterval::LongDoubleFloatDigits::UBitSizeMantissa+1, USizeMantissa, USizeExponent>, TypeImplementation> Implementation;
   reinterpret_cast<Implementation*>(content)->sinAssign();
}

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
void
TFloatInterval<USizeMantissa, USizeExponent, TypeImplementation>::cosAssign() {
   typedef DDoubleInterval::TFloatInterval<DDoubleInterval::ExecutionPath, DDoubleInterval::TBuiltFloat<DDoubleInterval::LongDoubleFloatDigits::UBitSizeMantissa+1, USizeMantissa, USizeExponent>, TypeImplementation> Implementation;
   reinterpret_cast<Implementation*>(content)->cosAssign();
}

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
void
TFloatInterval<USizeMantissa, USizeExponent, TypeImplementation>::asinAssign() {
   typedef DDoubleInterval::TFloatInterval<DDoubleInterval::ExecutionPath, DDoubleInterval::TBuiltFloat<DDoubleInterval::LongDoubleFloatDigits::UBitSizeMantissa+1, USizeMantissa, USizeExponent>, TypeImplementation> Implementation;
   reinterpret_cast<Implementation*>(content)->asinAssign();
}

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
void
TFloatInterval<USizeMantissa, USizeExponent, TypeImplementation>::acosAssign() {
   typedef DDoubleInterval::TFloatInterval<DDoubleInterval::ExecutionPath, DDoubleInterval::TBuiltFloat<DDoubleInterval::LongDoubleFloatDigits::UBitSizeMantissa+1, USizeMantissa, USizeExponent>, TypeImplementation> Implementation;
   reinterpret_cast<Implementation*>(content)->acosAssign();
}

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
void
TFloatInterval<USizeMantissa, USizeExponent, TypeImplementation>::tanAssign() {
   typedef DDoubleInterval::TFloatInterval<DDoubleInterval::ExecutionPath, DDoubleInterval::TBuiltFloat<DDoubleInterval::LongDoubleFloatDigits::UBitSizeMantissa+1, USizeMantissa, USizeExponent>, TypeImplementation> Implementation;
   reinterpret_cast<Implementation*>(content)->tanAssign();
}

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
void
TFloatInterval<USizeMantissa, USizeExponent, TypeImplementation>::atanAssign() {
   typedef DDoubleInterval::TFloatInterval<DDoubleInterval::ExecutionPath, DDoubleInterval::TBuiltFloat<DDoubleInterval::LongDoubleFloatDigits::UBitSizeMantissa+1, USizeMantissa, USizeExponent>, TypeImplementation> Implementation;
   reinterpret_cast<Implementation*>(content)->atanAssign();
}

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
void
TFloatInterval<USizeMantissa, USizeExponent, TypeImplementation>::expAssign() {
   typedef DDoubleInterval::TFloatInterval<DDoubleInterval::ExecutionPath, DDoubleInterval::TBuiltFloat<DDoubleInterval::LongDoubleFloatDigits::UBitSizeMantissa+1, USizeMantissa, USizeExponent>, TypeImplementation> Implementation;
   reinterpret_cast<Implementation*>(content)->expAssign();
}

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
void
TFloatInterval<USizeMantissa, USizeExponent, TypeImplementation>::logAssign() {
   typedef DDoubleInterval::TFloatInterval<DDoubleInterval::ExecutionPath, DDoubleInterval::TBuiltFloat<DDoubleInterval::LongDoubleFloatDigits::UBitSizeMantissa+1, USizeMantissa, USizeExponent>, TypeImplementation> Implementation;
   reinterpret_cast<Implementation*>(content)->logAssign();
}

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
void
TFloatInterval<USizeMantissa, USizeExponent, TypeImplementation>::log10Assign() {
   typedef DDoubleInterval::TFloatInterval<DDoubleInterval::ExecutionPath, DDoubleInterval::TBuiltFloat<DDoubleInterval::LongDoubleFloatDigits::UBitSizeMantissa+1, USizeMantissa, USizeExponent>, TypeImplementation> Implementation;
   reinterpret_cast<Implementation*>(content)->log10Assign();
}

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
void
TFloatInterval<USizeMantissa, USizeExponent, TypeImplementation>::powAssign(const thisType& value) {
   typedef DDoubleInterval::TFloatInterval<DDoubleInterval::ExecutionPath, DDoubleInterval::TBuiltFloat<DDoubleInterval::LongDoubleFloatDigits::UBitSizeMantissa+1, USizeMantissa, USizeExponent>, TypeImplementation> Implementation;
   reinterpret_cast<Implementation*>(content)->powAssign(
         *reinterpret_cast<const Implementation*>(value.content));
}

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
void
TFloatInterval<USizeMantissa, USizeExponent, TypeImplementation>::persist(const char* prefix) const {
   typedef DDoubleInterval::TFloatInterval<DDoubleInterval::ExecutionPath, DDoubleInterval::TBuiltFloat<DDoubleInterval::LongDoubleFloatDigits::UBitSizeMantissa+1, USizeMantissa, USizeExponent>, TypeImplementation> Implementation;
   const Implementation& thisInterval = *reinterpret_cast<const Implementation*>(content);
   thisInterval.notifyForPersistence(thisInterval, prefix);
}

#if defined __GNUC__ && GCC_VERSION >= 50000
#pragma GCC diagnostic pop
#endif

typedef DDoubleInterval::FloatDigitsHelper::TFloatDigits<long double> LongDoubleFloatDigits;

template class TFloatInterval<23, 8, float>;
template class TFloatInterval<52, 11, double>;
template class TFloatInterval<LongDoubleFloatDigits::UBitSizeMantissa,
         LongDoubleFloatDigits::UBitSizeExponent, long double>;

template TFloatInterval<23, 8, float>::TFloatInterval(const TFloatInterval<52, 11, double>&);
template TFloatInterval<23, 8, float>::TFloatInterval(const TFloatInterval<LongDoubleFloatDigits::UBitSizeMantissa, LongDoubleFloatDigits::UBitSizeExponent, long double>&);
template TFloatInterval<52, 11, double>::TFloatInterval(const TFloatInterval<23, 8, float>&);
template TFloatInterval<52, 11, double>::TFloatInterval(const TFloatInterval<LongDoubleFloatDigits::UBitSizeMantissa, LongDoubleFloatDigits::UBitSizeExponent, long double>&);
template TFloatInterval<LongDoubleFloatDigits::UBitSizeMantissa, LongDoubleFloatDigits::UBitSizeExponent, long double>::TFloatInterval(const TFloatInterval<23, 8, float>&);
template TFloatInterval<LongDoubleFloatDigits::UBitSizeMantissa, LongDoubleFloatDigits::UBitSizeExponent, long double>::TFloatInterval(const TFloatInterval<52, 11, double>&);

template TFloatInterval<23, 8, float>& TFloatInterval<23, 8, float>::operator=(const TFloatInterval<52, 11, double>&);
template TFloatInterval<23, 8, float>& TFloatInterval<23, 8, float>::operator=(const TFloatInterval<LongDoubleFloatDigits::UBitSizeMantissa, LongDoubleFloatDigits::UBitSizeExponent, long double>&);
template TFloatInterval<52, 11, double>& TFloatInterval<52, 11, double>::operator=(const TFloatInterval<23, 8, float>&);
template TFloatInterval<52, 11, double>& TFloatInterval<52, 11, double>::operator=(const TFloatInterval<LongDoubleFloatDigits::UBitSizeMantissa, LongDoubleFloatDigits::UBitSizeExponent, long double>&);
template TFloatInterval<LongDoubleFloatDigits::UBitSizeMantissa, LongDoubleFloatDigits::UBitSizeExponent, long double>& TFloatInterval<LongDoubleFloatDigits::UBitSizeMantissa, LongDoubleFloatDigits::UBitSizeExponent, long double>::operator=(const TFloatInterval<23, 8, float>&);
template TFloatInterval<LongDoubleFloatDigits::UBitSizeMantissa, LongDoubleFloatDigits::UBitSizeExponent, long double>& TFloatInterval<LongDoubleFloatDigits::UBitSizeMantissa, LongDoubleFloatDigits::UBitSizeExponent, long double>::operator=(const TFloatInterval<52, 11, double>&);

}} // end of namespace NumericalDomains::DDoubleIntervalInterface

