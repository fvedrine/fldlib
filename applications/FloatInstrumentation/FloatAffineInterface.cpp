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
// Unit      : Affine relationships
// File      : FloatAffineInterface.cpp
// Description :
//   Implementation of a class of affine relations.
//

#include <fstream>
#include <sstream>

#include "NumericalAnalysis/FloatAffineExecutionPath.h"
#include "NumericalAnalysis/FloatAffineExecutionPath.template"

#include "Pointer/Vector.template"

#include "FloatInstrumentation/FloatAffineInterface.h"

#ifndef FLOAT_ZONOTOPE_DOES_ABSORB_HIGH_LEVEL
#define FLOAT_ZONOTOPE_DOES_ABSORB_HIGH_LEVEL false
#endif

#ifndef FLOAT_ZONOTOPE_ALLOW_SIMPLEX
#define FLOAT_ZONOTOPE_ALLOW_SIMPLEX false
#endif

#ifndef FLOAT_ZONOTOPE_LIMIT_SYMBOL_ABSORPTION
#define FLOAT_ZONOTOPE_LIMIT_SYMBOL_ABSORPTION 48
#endif

#ifndef FLOAT_ZONOTOPE_DOES_EXCLUDE_CONSTANT_FROM_SYMBOL_ABSORPTION
#define FLOAT_ZONOTOPE_DOES_EXCLUDE_CONSTANT_FROM_SYMBOL_ABSORPTION false
#endif

#ifndef FLOAT_ZONOTOPE_START_SYMBOL_ABSORPTION
#define FLOAT_ZONOTOPE_START_SYMBOL_ABSORPTION 0
#endif

#include "FloatInstrumentation/FloatAffine.inch"
#include "FloatInstrumentation/FloatAffine.incc"

namespace NumericalDomains { namespace DAffineInterface {

#include "StandardClasses/UndefineNew.h"
PathExplorer::PathExplorer() {
   AssumeCondition(sizeof(DAffine::PathExplorer) <= UPathExplorerSize*sizeof(AlignType))
   new (content) DAffine::PathExplorer();
}

PathExplorer::PathExplorer(Mode mode) {
   AssumeCondition(sizeof(DAffine::PathExplorer) <= UPathExplorerSize*sizeof(AlignType))
   new (content) DAffine::PathExplorer((DAffine::BaseExecutionPath::Mode) mode);
}

PathExplorer::PathExplorer(const PathExplorer& source) {
   new (content) DAffine::PathExplorer(*reinterpret_cast<const DAffine::PathExplorer*>(source.content));
}

PathExplorer::PathExplorer(PathExplorer&& source) {
   new (content) DAffine::PathExplorer(std::move(*reinterpret_cast<DAffine::PathExplorer*>(source.content)));
}
#include "StandardClasses/DefineNew.h"

PathExplorer::~PathExplorer() {
   reinterpret_cast<DAffine::PathExplorer*>(content)->~PathExplorer();
}

PathExplorer&
PathExplorer::operator=(const PathExplorer& asource) {
   const DAffine::PathExplorer& source = *reinterpret_cast<const DAffine::PathExplorer*>(asource.content);
   DAffine::PathExplorer& thisExplorer = *reinterpret_cast<DAffine::PathExplorer*>(content);
   thisExplorer = source;
   return *this;
}

PathExplorer&
PathExplorer::operator=(PathExplorer&& asource) {
   DAffine::PathExplorer& source = *reinterpret_cast<DAffine::PathExplorer*>(asource.content);
   DAffine::PathExplorer& thisExplorer = *reinterpret_cast<DAffine::PathExplorer*>(content);
   thisExplorer.operator=(std::move(source));
   return *this;
}

PathExplorer::Mode
PathExplorer::mode() const {
   return (Mode) const_cast<DAffine::PathExplorer*>(
         reinterpret_cast<const DAffine::PathExplorer*>(content))->mode();
}

bool
PathExplorer::isFinished(Mode mode) {
   return reinterpret_cast<DAffine::PathExplorer*>(content)->isFinished((DAffine::BaseExecutionPath::Mode) mode);
}

bool
PathExplorer::isFinished() {
   return reinterpret_cast<DAffine::PathExplorer*>(content)->isFinished();
}

void
ExecutionPath::splitBranches(const char* file, int line) {
   DAffine::BaseFloatAffine::splitBranches(file, line);
}

bool
ExecutionPath::hasMultipleBranches() const {
   return DAffine::ExecutionPath::hasMultipleBranches();
}

void
ExecutionPath::setSupportAtomic() {
   DAffine::ExecutionPath::setSupportAtomic();
}

void
ExecutionPath::setSupportUnstableInLoop() {
   DAffine::ExecutionPath::setSupportUnstableInLoop();
}

void
ExecutionPath::setSupportVerbose() {
   DAffine::ExecutionPath::setSupportVerbose();
}

void
ExecutionPath::setSupportThreshold() {
   DAffine::ExecutionPath::setSupportThreshold();
}

void
ExecutionPath::setSupportFirstFollowFloat() {
   DAffine::ExecutionPath::setSupportFirstFollowFloat();
}

void
ExecutionPath::initializeGlobals(const char* fileSuffix) {
   DAffine::ExecutionPath::initializeGlobals(fileSuffix);
}

void
ExecutionPath::finalizeGlobals() {
   DAffine::ExecutionPath::finalizeGlobals();
}

void
ExecutionPath::setSimplificationTriggerPercent(double percent) {
   DAffine::ExecutionPath::setSimplificationTriggerPercent(percent);
}

void
ExecutionPath::flushOut() {
   DAffine::ExecutionPath::flushOut();
}

void
ExecutionPath::setSourceLine(const char* file, int line) {
   DAffine::ExecutionPath::setSourceLine(file, line);
}

void
ExecutionPath::clearSynchronizationBranches() {
   DAffine::ExecutionPath::clearSynchronizationBranches();
}

void
ExecutionPath::writeCurrentPath(std::ostream& out) {
   DAffine::ExecutionPath::writeCurrentPath(out);
}

DAffine::PathExplorer*
ExecutionPath::getCurrentPathExplorer() {
   return DAffine::ExecutionPath::getCurrentPathExplorer();
}

void
ExecutionPath::setCurrentPathExplorer(PathExplorer* pathExplorer) {
   if (pathExplorer)
      DAffine::ExecutionPath::currentPathExplorer = reinterpret_cast<DAffine::PathExplorer*>(pathExplorer->content);
   else
      DAffine::ExecutionPath::currentPathExplorer = nullptr;
}

void
ExecutionPath::setCurrentPathExplorer(DAffine::PathExplorer* pathExplorer) {
   DAffine::ExecutionPath::currentPathExplorer = pathExplorer;
}

DAffine::DiagnosisReadStream*
ExecutionPath::inputTraceFile() {
   return reinterpret_cast<DAffine::DiagnosisReadStream*>(DAffine::ExecutionPath::inputTraceFile());
}

const char*
ExecutionPath::synchronisationFile() {
   return DAffine::ExecutionPath::synchronisationFile();
}

int
ExecutionPath::synchronisationLine() {
   return DAffine::ExecutionPath::synchronisationLine();
}

bool
ExecutionPath::doesFollowFlow() {
   return DAffine::ExecutionPath::doesFollowFlow();
}

void
ExecutionPath::clearFollowFlow() {
   return DAffine::ExecutionPath::clearFollowFlow();
}

void
ExecutionPath::setFollowFlow(bool doesFollowFlow, DAffine::DiagnosisReadStream* inputTraceFile,
      const char* synchronizationFile, int synchronizationLine) {
   DAffine::ExecutionPath::setFollowFlow(doesFollowFlow,
         reinterpret_cast<STG::IOObject::ISBase*>(inputTraceFile),
         synchronizationFile, synchronizationLine);
}

void
ExecutionPath::setFollowFlow() {
   DAffine::ExecutionPath::setFollowFlow();
}

PathExplorer::Mode
ExecutionPath::queryMode(DAffine::PathExplorer* pathExplorer) {
   AssumeCondition(pathExplorer)
   return (PathExplorer::Mode) pathExplorer->mode();
}

void
ExecutionPath::throwEmptyBranch(bool isUnstable) {
   try {
      DAffine::ExecutionPath::throwEmptyBranch(isUnstable);
   }
   catch (DAffine::ExecutionPath::anticipated_termination) {
      throw DAffineInterface::ExecutionPath::anticipated_termination();
   };
}

#include "StandardClasses/UndefineNew.h"
MergeBranches::MergeBranches(const char* file, int line) {
   AssumeCondition(sizeof(DAffine::MergeBranches) <= UMergeBranchesSize*sizeof(AlignType))
   new (content) DAffine::MergeBranches(file, line);
}
#include "StandardClasses/DefineNew.h"

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
MergeBranches&
MergeBranches::operator<<(TFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>& value) {
   reinterpret_cast<DAffine::MergeBranches*>(content)->operator<<(
         *reinterpret_cast<DAffine::TFloatZonotope<DAffine::ExecutionPath, USizeMantissa, USizeExponent, TypeImplementation>*>(value.content));
   return *this;
}

bool
MergeBranches::operator<<(end) {
   try {
   return reinterpret_cast<DAffine::MergeBranches*>(content)->operator<<(
         DAffine::BaseFloatAffine::end());
   }
   catch (DAffine::ExecutionPath::anticipated_termination) {
      throw DAffineInterface::ExecutionPath::anticipated_termination();
   };
}

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
const char*
TFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>::queryDebugValue() const {
   return reinterpret_cast<const DAffine::TFloatZonotope<DAffine::ExecutionPath, USizeMantissa, USizeExponent, TypeImplementation>*>(content)
      ->queryDebugValue();
}

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
const char*
TFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>::queryLightDebugValue() const {
   return reinterpret_cast<const DAffine::TFloatZonotope<DAffine::ExecutionPath, USizeMantissa, USizeExponent, TypeImplementation>*>(content)
      ->queryLightDebugValue();
}

#include "StandardClasses/UndefineNew.h"
template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
TFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>::TFloatZonotope() {
   typedef DAffine::TFloatZonotope<DAffine::ExecutionPath, USizeMantissa, USizeExponent, TypeImplementation> Implementation;
   AssumeCondition(sizeof(Implementation) <= UFloatZonotopeSize*sizeof(AlignType))
   new (content) Implementation();
}

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
TFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>::TFloatZonotope(float value) {
   typedef DAffine::TFloatZonotope<DAffine::ExecutionPath, USizeMantissa, USizeExponent, TypeImplementation> Implementation;
   AssumeCondition(sizeof(Implementation) <= UFloatZonotopeSize*sizeof(AlignType))
   new (content) Implementation();
   if (!DAffine::ExecutionPath::doesSupportAtomic())
      reinterpret_cast<Implementation*>(content)->initFrom(value);
   else
      reinterpret_cast<Implementation*>(content)->initFromAtomic(value);
}

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
TFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>::TFloatZonotope(double value) {
   typedef DAffine::TFloatZonotope<DAffine::ExecutionPath, USizeMantissa, USizeExponent, TypeImplementation> Implementation;
   AssumeCondition(sizeof(Implementation) <= UFloatZonotopeSize*sizeof(AlignType))
   new (content) Implementation();
   if (!DAffine::ExecutionPath::doesSupportAtomic())
      reinterpret_cast<Implementation*>(content)->initFrom(value);
   else
      reinterpret_cast<Implementation*>(content)->initFromAtomic(value);
}

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
TFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>::TFloatZonotope(long double value) {
   typedef DAffine::TFloatZonotope<DAffine::ExecutionPath, USizeMantissa, USizeExponent, TypeImplementation> Implementation;
   AssumeCondition(sizeof(Implementation) <= UFloatZonotopeSize*sizeof(AlignType))
   new (content) Implementation();
   if (!DAffine::ExecutionPath::doesSupportAtomic())
      reinterpret_cast<Implementation*>(content)->initFrom(value);
   else
      reinterpret_cast<Implementation*>(content)->initFromAtomic(value);
}

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
TFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>::TFloatZonotope(
      TypeImplementation min, TypeImplementation max) {
   typedef DAffine::TFloatZonotope<DAffine::ExecutionPath, USizeMantissa, USizeExponent, TypeImplementation> Implementation;
   AssumeCondition(sizeof(Implementation) <= UFloatZonotopeSize*sizeof(AlignType))
   new (content) Implementation(min, max);
   if (DAffine::ExecutionPath::doesSupportUnstableInLoop())
      reinterpret_cast<Implementation*>(content)->getSRealDomain().setHolder(reinterpret_cast<Implementation*>(content)->currentPathExplorer);
}

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
TFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>::TFloatZonotope(
      TypeImplementation min, TypeImplementation max,
      TypeImplementation errmin, TypeImplementation errmax) {
   typedef DAffine::TFloatZonotope<DAffine::ExecutionPath, USizeMantissa, USizeExponent, TypeImplementation> Implementation;
   AssumeCondition(sizeof(Implementation) <= UFloatZonotopeSize*sizeof(AlignType))
   new (content) Implementation(min, max);
   reinterpret_cast<Implementation*>(content)->setError(errmin, errmax);
   if (DAffine::ExecutionPath::doesSupportUnstableInLoop()) {
      reinterpret_cast<Implementation*>(content)->getSRealDomain().setHolder(reinterpret_cast<Implementation*>(content)->currentPathExplorer);
      reinterpret_cast<Implementation*>(content)->getSError().setHolder(reinterpret_cast<Implementation*>(content)->currentPathExplorer);
   };
}

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
TFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>::TFloatZonotope(int value) {
   typedef DAffine::TFloatZonotope<DAffine::ExecutionPath, USizeMantissa, USizeExponent, TypeImplementation> Implementation;
   AssumeCondition(sizeof(Implementation) <= UFloatZonotopeSize*sizeof(AlignType))
   new (content) Implementation(value);
}

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
TFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>::TFloatZonotope(long int value) {
   typedef DAffine::TFloatZonotope<DAffine::ExecutionPath, USizeMantissa, USizeExponent, TypeImplementation> Implementation;
   AssumeCondition(sizeof(Implementation) <= UFloatZonotopeSize*sizeof(AlignType))
   new (content) Implementation(value);
}

/*
template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
TFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>::TFloatZonotope(unsigned value) {
   typedef DAffine::TFloatZonotope<DAffine::ExecutionPath, USizeMantissa, USizeExponent, TypeImplementation> Implementation;
   AssumeCondition(sizeof(Implementation) <= UFloatZonotopeSize*sizeof(AlignType))
   new (content) Implementation(value);
}

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
TFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>::TFloatZonotope(unsigned long value) {
   typedef DAffine::TFloatZonotope<DAffine::ExecutionPath, USizeMantissa, USizeExponent, TypeImplementation> Implementation;
   AssumeCondition(sizeof(Implementation) <= UFloatZonotopeSize*sizeof(AlignType))
   new (content) Implementation(value);
}
*/

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
TFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>::TFloatZonotope(const thisType& asource) {
   typedef DAffine::TFloatZonotope<DAffine::ExecutionPath, USizeMantissa, USizeExponent, TypeImplementation> Implementation;
   const Implementation& source = *reinterpret_cast<const Implementation*>(asource.content);
   new (content) Implementation(source);
   if (DAffine::ExecutionPath::doesSupportUnstableInLoop()) {
      reinterpret_cast<Implementation*>(content)->getSRealDomain().setHolder(reinterpret_cast<Implementation*>(content)->currentPathExplorer);
      reinterpret_cast<Implementation*>(content)->getSError().setHolder(reinterpret_cast<Implementation*>(content)->currentPathExplorer);
   };
}

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
TFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>::TFloatZonotope(const thisType& asource, Record) {
   typedef DAffine::TFloatZonotope<DAffine::ExecutionPath, USizeMantissa, USizeExponent, TypeImplementation> Implementation;
   const Implementation& source = *reinterpret_cast<const Implementation*>(asource.content);
   new (content) Implementation(source);
   if (DAffine::ExecutionPath::doesSupportUnstableInLoop()) {
      const_cast<Implementation&>(source).getSRealDomain().setHolder(source.currentPathExplorer);
      const_cast<Implementation&>(source).getSError().setHolder(source.currentPathExplorer);
      reinterpret_cast<Implementation*>(content)->getSRealDomain().clearHolder();
      reinterpret_cast<Implementation*>(content)->getSError().clearHolder();
   };
}

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
TFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>::TFloatZonotope(thisType&& asource) {
   typedef DAffine::TFloatZonotope<DAffine::ExecutionPath, USizeMantissa, USizeExponent, TypeImplementation> Implementation;
   Implementation& source = *reinterpret_cast<Implementation*>(asource.content);
   new (content) Implementation(std::move(source));
   if (DAffine::ExecutionPath::doesSupportUnstableInLoop()) {
      reinterpret_cast<Implementation*>(content)->getSRealDomain().setHolder(reinterpret_cast<Implementation*>(content)->currentPathExplorer);
      reinterpret_cast<Implementation*>(content)->getSError().setHolder(reinterpret_cast<Implementation*>(content)->currentPathExplorer);
   };
}

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
TFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>::TFloatZonotope(thisType&& asource, Record) {
   typedef DAffine::TFloatZonotope<DAffine::ExecutionPath, USizeMantissa, USizeExponent, TypeImplementation> Implementation;
   Implementation& source = *reinterpret_cast<Implementation*>(asource.content);
   new (content) Implementation(std::move(source));
   if (DAffine::ExecutionPath::doesSupportUnstableInLoop()) {
      const_cast<Implementation&>(source).getSRealDomain().setHolder(source.currentPathExplorer);
      const_cast<Implementation&>(source).getSError().setHolder(source.currentPathExplorer);
      reinterpret_cast<Implementation*>(content)->getSRealDomain().clearHolder();
      reinterpret_cast<Implementation*>(content)->getSError().clearHolder();
   };
}

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
template <int USizeMantissaArgument, int USizeExponentArgument, typename TypeImplementationArgument>
TFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>::TFloatZonotope(
      const TFloatZonotope<USizeMantissaArgument, USizeExponentArgument, TypeImplementationArgument>& asource) {
   typedef DAffine::TFloatZonotope<DAffine::ExecutionPath, USizeMantissa, USizeExponent, TypeImplementation> Implementation;
   typedef DAffine::TFloatZonotope<DAffine::ExecutionPath, USizeMantissaArgument, USizeExponentArgument, TypeImplementationArgument> ImplementationSource;
   const ImplementationSource& source = *reinterpret_cast<const ImplementationSource*>(asource.content);
   new (content) Implementation(source);
   if (DAffine::ExecutionPath::doesSupportUnstableInLoop()) {
      reinterpret_cast<Implementation*>(content)->getSRealDomain().setHolder(reinterpret_cast<Implementation*>(content)->currentPathExplorer);
      reinterpret_cast<Implementation*>(content)->getSError().setHolder(reinterpret_cast<Implementation*>(content)->currentPathExplorer);
   };
}

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
template <int USizeMantissaArgument, int USizeExponentArgument, typename TypeImplementationArgument>
TFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>::TFloatZonotope(
      TFloatZonotope<USizeMantissaArgument, USizeExponentArgument, TypeImplementationArgument>&& asource) {
   typedef DAffine::TFloatZonotope<DAffine::ExecutionPath, USizeMantissa, USizeExponent, TypeImplementation> Implementation;
   typedef DAffine::TFloatZonotope<DAffine::ExecutionPath, USizeMantissaArgument, USizeExponentArgument, TypeImplementationArgument> ImplementationSource;
   ImplementationSource& source = *reinterpret_cast<ImplementationSource*>(asource.content);
   new (content) Implementation(std::move(source));
   if (DAffine::ExecutionPath::doesSupportUnstableInLoop()) {
      reinterpret_cast<Implementation*>(content)->getSRealDomain().setHolder(reinterpret_cast<Implementation*>(content)->currentPathExplorer);
      reinterpret_cast<Implementation*>(content)->getSError().setHolder(reinterpret_cast<Implementation*>(content)->currentPathExplorer);
   };
}
#include "StandardClasses/DefineNew.h"

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
TFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>::~TFloatZonotope() {
   typedef DAffine::TFloatZonotope<DAffine::ExecutionPath, USizeMantissa, USizeExponent, TypeImplementation> Implementation;
   reinterpret_cast<Implementation*>(content)->~Implementation();
}

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
TFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>&
TFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>::operator=(const thisType& asource) {
   typedef DAffine::TFloatZonotope<DAffine::ExecutionPath, USizeMantissa, USizeExponent, TypeImplementation> Implementation;
   const Implementation& source = *reinterpret_cast<const Implementation*>(asource.content);
   Implementation& thisZonotope = *reinterpret_cast<Implementation*>(content);
   thisZonotope.operator=(source);
   if (DAffine::ExecutionPath::doesSupportUnstableInLoop()) {
      thisZonotope.getSRealDomain().setHolder(thisZonotope.currentPathExplorer);
      thisZonotope.getSError().setHolder(thisZonotope.currentPathExplorer);
   };
   return *this;
}

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
void
TFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>::recordFrom(const thisType& asource) {
   typedef DAffine::TFloatZonotope<DAffine::ExecutionPath, USizeMantissa, USizeExponent, TypeImplementation> Implementation;
   const Implementation& source = *reinterpret_cast<const Implementation*>(asource.content);
   Implementation& thisZonotope = *reinterpret_cast<Implementation*>(content);
   // if (!const_cast<Implementation&>(source).optimizeValue()) throw anticipated_termination();
   thisZonotope.operator=(source);
   if (DAffine::ExecutionPath::doesSupportUnstableInLoop()) {
      thisZonotope.getSRealDomain().clearHolder();
      thisZonotope.getSError().clearHolder();
   };
}

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
void
TFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>::mergeWith(const thisType& asource) {
   typedef DAffine::TFloatZonotope<DAffine::ExecutionPath, USizeMantissa, USizeExponent, TypeImplementation> Implementation;
   const Implementation& source = *reinterpret_cast<const Implementation*>(asource.content);
   Implementation& thisZonotope = *reinterpret_cast<Implementation*>(content);
   // if (!const_cast<Implementation&>(source).optimizeValue()) throw anticipated_termination();
   thisZonotope.mergeWith(source);
}

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
TFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>&
TFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>::operator=(thisType&& asource) {
   typedef DAffine::TFloatZonotope<DAffine::ExecutionPath, USizeMantissa, USizeExponent, TypeImplementation> Implementation;
   Implementation& source = *reinterpret_cast<Implementation*>(asource.content);
   Implementation& thisZonotope = *reinterpret_cast<Implementation*>(content);
   thisZonotope.operator=(std::move(source));
   if (DAffine::ExecutionPath::doesSupportUnstableInLoop()) {
      thisZonotope.getSRealDomain().setHolder(thisZonotope.currentPathExplorer);
      thisZonotope.getSError().setHolder(thisZonotope.currentPathExplorer);
   };
   return *this;
}

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
void
TFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>::recordFrom(thisType&& asource) {
   typedef DAffine::TFloatZonotope<DAffine::ExecutionPath, USizeMantissa, USizeExponent, TypeImplementation> Implementation;
   Implementation& source = *reinterpret_cast<Implementation*>(asource.content);
   Implementation& thisZonotope = *reinterpret_cast<Implementation*>(content);
   if (!source.optimizeValue()) throw anticipated_termination();
   thisZonotope.operator=(std::move(source));
   if (DAffine::ExecutionPath::doesSupportUnstableInLoop()) {
      thisZonotope.getSRealDomain().clearHolder();
      thisZonotope.getSError().clearHolder();
   };
}

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
void
TFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>::mergeWith(thisType&& asource) {
   typedef DAffine::TFloatZonotope<DAffine::ExecutionPath, USizeMantissa, USizeExponent, TypeImplementation> Implementation;
   Implementation& source = *reinterpret_cast<Implementation*>(asource.content);
   Implementation& thisZonotope = *reinterpret_cast<Implementation*>(content);
   if (!source.optimizeValue()) throw anticipated_termination();
   thisZonotope.mergeWith(std::move(source));
}

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
template <int USizeMantissaArgument, int USizeExponentArgument, typename TypeImplementationArgument>
TFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>&
TFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>::operator=(
      const TFloatZonotope<USizeMantissaArgument, USizeExponentArgument, TypeImplementationArgument>& asource) {
   typedef DAffine::TFloatZonotope<DAffine::ExecutionPath, USizeMantissa, USizeExponent, TypeImplementation> Implementation;
   typedef DAffine::TFloatZonotope<DAffine::ExecutionPath, USizeMantissaArgument, USizeExponentArgument, TypeImplementationArgument> ImplementationSource;
   const ImplementationSource& source = *reinterpret_cast<const ImplementationSource*>(asource.content);
   Implementation& thisZonotope = *reinterpret_cast<Implementation*>(content);
   thisZonotope.operator=(source);
   if (DAffine::ExecutionPath::doesSupportUnstableInLoop()) {
      thisZonotope.getSRealDomain().setHolder(thisZonotope.currentPathExplorer);
      thisZonotope.getSError().setHolder(thisZonotope.currentPathExplorer);
   };
   return *this;
}

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
template <int USizeMantissaArgument, int USizeExponentArgument, typename TypeImplementationArgument>
TFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>&
TFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>::operator=(
      TFloatZonotope<USizeMantissaArgument, USizeExponentArgument, TypeImplementationArgument>&& asource) {
   typedef DAffine::TFloatZonotope<DAffine::ExecutionPath, USizeMantissa, USizeExponent, TypeImplementation> Implementation;
   typedef DAffine::TFloatZonotope<DAffine::ExecutionPath, USizeMantissaArgument, USizeExponentArgument, TypeImplementationArgument> ImplementationSource;
   ImplementationSource& source = *reinterpret_cast<ImplementationSource*>(asource.content);
   Implementation& thisZonotope = *reinterpret_cast<Implementation*>(content);
   thisZonotope.operator=(std::move(source));
   if (DAffine::ExecutionPath::doesSupportUnstableInLoop()) {
      thisZonotope.getSRealDomain().setHolder(thisZonotope.currentPathExplorer);
      thisZonotope.getSError().setHolder(thisZonotope.currentPathExplorer);
   };
   return *this;
}


template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
void
TFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>::writeImplementation(
      std::ostream& out) const {
   typedef DAffine::TFloatZonotope<DAffine::ExecutionPath, USizeMantissa, USizeExponent, TypeImplementation> Implementation;
   out << reinterpret_cast<const Implementation*>(content)->asImplementation();
}

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
void
TFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>::readImplementation(
      std::istream& in) {
   typedef DAffine::TFloatZonotope<DAffine::ExecutionPath, USizeMantissa, USizeExponent, TypeImplementation> Implementation;
   decltype(reinterpret_cast<const Implementation*>(content)->asImplementation()) result; 
   in >> result;
   *reinterpret_cast<Implementation*>(content) = Implementation(result, result);
}

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
bool
TFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>::optimizeValue() {
   typedef DAffine::TFloatZonotope<DAffine::ExecutionPath, USizeMantissa, USizeExponent, TypeImplementation> Implementation;
   Implementation& thisZonotope = *reinterpret_cast<Implementation*>(content);
   return thisZonotope.optimizeValue();
}

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
bool
TFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>::operator<(const thisType& asource) const {
   try {
   typedef DAffine::TFloatZonotope<DAffine::ExecutionPath, USizeMantissa, USizeExponent, TypeImplementation> Implementation;
   const Implementation& source = *reinterpret_cast<const Implementation*>(asource.content);
   const Implementation& thisZonotope = *reinterpret_cast<const Implementation*>(content);
   return thisZonotope.operator<(source);
   }
   catch (DAffine::ExecutionPath::anticipated_termination) {
      throw DAffineInterface::ExecutionPath::anticipated_termination();
   };
}

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
bool
TFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>::operator<(TypeImplementation source) const {
   try {
   typedef DAffine::TFloatZonotope<DAffine::ExecutionPath, USizeMantissa, USizeExponent, TypeImplementation> Implementation;
   const Implementation& thisZonotope = *reinterpret_cast<const Implementation*>(content);
   TFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation> asource(source);
   return thisZonotope.operator<(*reinterpret_cast<const Implementation*>(asource.content));
   }
   catch (DAffine::ExecutionPath::anticipated_termination) {
      throw DAffineInterface::ExecutionPath::anticipated_termination();
   };
}

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
bool
TFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>::operator<=(const thisType& asource) const {
   try {
   typedef DAffine::TFloatZonotope<DAffine::ExecutionPath, USizeMantissa, USizeExponent, TypeImplementation> Implementation;
   const Implementation& source = *reinterpret_cast<const Implementation*>(asource.content);
   const Implementation& thisZonotope = *reinterpret_cast<const Implementation*>(content);
   return thisZonotope.operator<=(source);
   }
   catch (DAffine::ExecutionPath::anticipated_termination) {
      throw DAffineInterface::ExecutionPath::anticipated_termination();
   };
}

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
bool
TFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>::operator<=(TypeImplementation source) const {
   try {
   typedef DAffine::TFloatZonotope<DAffine::ExecutionPath, USizeMantissa, USizeExponent, TypeImplementation> Implementation;
   const Implementation& thisZonotope = *reinterpret_cast<const Implementation*>(content);
   TFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation> asource(source);
   return thisZonotope.operator<=(*reinterpret_cast<const Implementation*>(asource.content));
   }
   catch (DAffine::ExecutionPath::anticipated_termination) {
      throw DAffineInterface::ExecutionPath::anticipated_termination();
   };
}

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
bool
TFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>::operator==(const thisType& asource) const {
   try {
   typedef DAffine::TFloatZonotope<DAffine::ExecutionPath, USizeMantissa, USizeExponent, TypeImplementation> Implementation;
   const Implementation& source = *reinterpret_cast<const Implementation*>(asource.content);
   const Implementation& thisZonotope = *reinterpret_cast<const Implementation*>(content);
   return thisZonotope.operator==(source);
   }
   catch (DAffine::ExecutionPath::anticipated_termination) {
      throw DAffineInterface::ExecutionPath::anticipated_termination();
   };
}

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
bool
TFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>::operator==(TypeImplementation source) const {
   try {
   typedef DAffine::TFloatZonotope<DAffine::ExecutionPath, USizeMantissa, USizeExponent, TypeImplementation> Implementation;
   const Implementation& thisZonotope = *reinterpret_cast<const Implementation*>(content);
   TFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation> asource(source);
   return thisZonotope.operator==(*reinterpret_cast<const Implementation*>(asource.content));
   }
   catch (DAffine::ExecutionPath::anticipated_termination) {
      throw DAffineInterface::ExecutionPath::anticipated_termination();
   };
}

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
bool
TFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>::operator!=(const thisType& asource) const {
   try {
   typedef DAffine::TFloatZonotope<DAffine::ExecutionPath, USizeMantissa, USizeExponent, TypeImplementation> Implementation;
   const Implementation& source = *reinterpret_cast<const Implementation*>(asource.content);
   const Implementation& thisZonotope = *reinterpret_cast<const Implementation*>(content);
   return thisZonotope.operator!=(source);
   }
   catch (DAffine::ExecutionPath::anticipated_termination) {
      throw DAffineInterface::ExecutionPath::anticipated_termination();
   };
}

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
bool
TFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>::operator!=(TypeImplementation source) const {
   try {
   typedef DAffine::TFloatZonotope<DAffine::ExecutionPath, USizeMantissa, USizeExponent, TypeImplementation> Implementation;
   const Implementation& thisZonotope = *reinterpret_cast<const Implementation*>(content);
   TFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation> asource(source);
   return thisZonotope.operator!=(*reinterpret_cast<const Implementation*>(asource.content));
   }
   catch (DAffine::ExecutionPath::anticipated_termination) {
      throw DAffineInterface::ExecutionPath::anticipated_termination();
   };
}

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
bool
TFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>::operator>=(const thisType& asource) const {
   try {
   typedef DAffine::TFloatZonotope<DAffine::ExecutionPath, USizeMantissa, USizeExponent, TypeImplementation> Implementation;
   const Implementation& source = *reinterpret_cast<const Implementation*>(asource.content);
   const Implementation& thisZonotope = *reinterpret_cast<const Implementation*>(content);
   return thisZonotope.operator>=(source);
   }
   catch (DAffine::ExecutionPath::anticipated_termination) {
      throw DAffineInterface::ExecutionPath::anticipated_termination();
   };
}

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
bool
TFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>::operator>=(TypeImplementation source) const {
   try {
   typedef DAffine::TFloatZonotope<DAffine::ExecutionPath, USizeMantissa, USizeExponent, TypeImplementation> Implementation;
   const Implementation& thisZonotope = *reinterpret_cast<const Implementation*>(content);
   TFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation> asource(source);
   return thisZonotope.operator>=(*reinterpret_cast<const Implementation*>(asource.content));
   }
   catch (DAffine::ExecutionPath::anticipated_termination) {
      throw DAffineInterface::ExecutionPath::anticipated_termination();
   };
}

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
bool
TFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>::operator>(const thisType& asource) const {
   try {
   typedef DAffine::TFloatZonotope<DAffine::ExecutionPath, USizeMantissa, USizeExponent, TypeImplementation> Implementation;
   const Implementation& source = *reinterpret_cast<const Implementation*>(asource.content);
   const Implementation& thisZonotope = *reinterpret_cast<const Implementation*>(content);
   return thisZonotope.operator>(source);
   }
   catch (DAffine::ExecutionPath::anticipated_termination) {
      throw DAffineInterface::ExecutionPath::anticipated_termination();
   };
}

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
bool
TFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>::operator>(TypeImplementation source) const {
   try {
   typedef DAffine::TFloatZonotope<DAffine::ExecutionPath, USizeMantissa, USizeExponent, TypeImplementation> Implementation;
   const Implementation& thisZonotope = *reinterpret_cast<const Implementation*>(content);
   TFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation> asource(source);
   return thisZonotope.operator>(*reinterpret_cast<const Implementation*>(asource.content));
   }
   catch (DAffine::ExecutionPath::anticipated_termination) {
      throw DAffineInterface::ExecutionPath::anticipated_termination();
   };
}

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
TFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>&
TFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>::operator+=(const thisType& asource) {
   typedef DAffine::TFloatZonotope<DAffine::ExecutionPath, USizeMantissa, USizeExponent, TypeImplementation> Implementation;
   const Implementation& source = *reinterpret_cast<const Implementation*>(asource.content);
   Implementation& thisZonotope = *reinterpret_cast<Implementation*>(content);
   thisZonotope.plusAssign(source, DAffine::Equation::PCSourceRValue);
   if (DAffine::ExecutionPath::doesSupportUnstableInLoop()) {
      thisZonotope.getSRealDomain().setHolder(thisZonotope.currentPathExplorer);
      thisZonotope.getSError().setHolder(thisZonotope.currentPathExplorer);
   };
   return *this;
}

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
TFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>&
TFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>::operator+=(thisType&& asource) {
   typedef DAffine::TFloatZonotope<DAffine::ExecutionPath, USizeMantissa, USizeExponent, TypeImplementation> Implementation;
   Implementation& source = *reinterpret_cast<Implementation*>(asource.content);
   Implementation& thisZonotope = *reinterpret_cast<Implementation*>(content);
   thisZonotope.plusAssign(source, DAffine::Equation::PCSourceXValue);
   if (DAffine::ExecutionPath::doesSupportUnstableInLoop()) {
      thisZonotope.getSRealDomain().setHolder(thisZonotope.currentPathExplorer);
      thisZonotope.getSError().setHolder(thisZonotope.currentPathExplorer);
   };
   return *this;
}

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
TFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>&
TFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>::operator-=(const thisType& asource) {
   typedef DAffine::TFloatZonotope<DAffine::ExecutionPath, USizeMantissa, USizeExponent, TypeImplementation> Implementation;
   const Implementation& source = *reinterpret_cast<const Implementation*>(asource.content);
   Implementation& thisZonotope = *reinterpret_cast<Implementation*>(content);
   thisZonotope.minusAssign(source, DAffine::Equation::PCSourceRValue);
   if (DAffine::ExecutionPath::doesSupportUnstableInLoop()) {
      thisZonotope.getSRealDomain().setHolder(thisZonotope.currentPathExplorer);
      thisZonotope.getSError().setHolder(thisZonotope.currentPathExplorer);
   };
   return *this;
}

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
TFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>&
TFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>::operator-=(thisType&& asource) {
   typedef DAffine::TFloatZonotope<DAffine::ExecutionPath, USizeMantissa, USizeExponent, TypeImplementation> Implementation;
   Implementation& source = *reinterpret_cast<Implementation*>(asource.content);
   Implementation& thisZonotope = *reinterpret_cast<Implementation*>(content);
   thisZonotope.minusAssign(source, DAffine::Equation::PCSourceXValue);
   if (DAffine::ExecutionPath::doesSupportUnstableInLoop()) {
      thisZonotope.getSRealDomain().setHolder(thisZonotope.currentPathExplorer);
      thisZonotope.getSError().setHolder(thisZonotope.currentPathExplorer);
   };
   return *this;
}

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
TFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>&
TFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>::operator*=(const thisType& asource) {
   typedef DAffine::TFloatZonotope<DAffine::ExecutionPath, USizeMantissa, USizeExponent, TypeImplementation> Implementation;
   const Implementation& source = *reinterpret_cast<const Implementation*>(asource.content);
   Implementation& thisZonotope = *reinterpret_cast<Implementation*>(content);
   thisZonotope.multAssign(source, DAffine::Equation::PCSourceRValue);
   if (DAffine::ExecutionPath::doesSupportUnstableInLoop()) {
      thisZonotope.getSRealDomain().setHolder(thisZonotope.currentPathExplorer);
      thisZonotope.getSError().setHolder(thisZonotope.currentPathExplorer);
   };
   return *this;
}

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
TFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>&
TFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>::operator*=(thisType&& asource) {
   typedef DAffine::TFloatZonotope<DAffine::ExecutionPath, USizeMantissa, USizeExponent, TypeImplementation> Implementation;
   Implementation& source = *reinterpret_cast<Implementation*>(asource.content);
   Implementation& thisZonotope = *reinterpret_cast<Implementation*>(content);
   thisZonotope.multAssign(source, DAffine::Equation::PCSourceXValue);
   if (DAffine::ExecutionPath::doesSupportUnstableInLoop()) {
      thisZonotope.getSRealDomain().setHolder(thisZonotope.currentPathExplorer);
      thisZonotope.getSError().setHolder(thisZonotope.currentPathExplorer);
   };
   return *this;
}

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
TFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>&
TFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>::operator/=(const thisType& asource) {
   typedef DAffine::TFloatZonotope<DAffine::ExecutionPath, USizeMantissa, USizeExponent, TypeImplementation> Implementation;
   const Implementation& source = *reinterpret_cast<const Implementation*>(asource.content);
   Implementation& thisZonotope = *reinterpret_cast<Implementation*>(content);
   thisZonotope.divAssign(source, DAffine::Equation::PCSourceRValue);
   if (DAffine::ExecutionPath::doesSupportUnstableInLoop()) {
      thisZonotope.getSRealDomain().setHolder(thisZonotope.currentPathExplorer);
      thisZonotope.getSError().setHolder(thisZonotope.currentPathExplorer);
   };
   return *this;
}

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
TFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>&
TFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>::operator/=(thisType&& asource) {
   typedef DAffine::TFloatZonotope<DAffine::ExecutionPath, USizeMantissa, USizeExponent, TypeImplementation> Implementation;
   Implementation& source = *reinterpret_cast<Implementation*>(asource.content);
   Implementation& thisZonotope = *reinterpret_cast<Implementation*>(content);
   thisZonotope.divAssign(source, DAffine::Equation::PCSourceXValue);
   if (DAffine::ExecutionPath::doesSupportUnstableInLoop()) {
      thisZonotope.getSRealDomain().setHolder(thisZonotope.currentPathExplorer);
      thisZonotope.getSError().setHolder(thisZonotope.currentPathExplorer);
   };
   return *this;
}

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
void
TFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>::oppositeAssign() {
   typedef DAffine::TFloatZonotope<DAffine::ExecutionPath, USizeMantissa, USizeExponent, TypeImplementation> Implementation;
   reinterpret_cast<Implementation*>(content)->oppositeAssign();
}

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
void
TFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>::plusAssign(const thisType& source) {
   typedef DAffine::TFloatZonotope<DAffine::ExecutionPath, USizeMantissa, USizeExponent, TypeImplementation> Implementation;
   reinterpret_cast<Implementation*>(content)->plusAssign(
         *reinterpret_cast<const Implementation*>(source.content), DAffine::Equation::PCSourceRValue);
}

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
void
TFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>::plusAssign(thisType&& source) {
   typedef DAffine::TFloatZonotope<DAffine::ExecutionPath, USizeMantissa, USizeExponent, TypeImplementation> Implementation;
   reinterpret_cast<Implementation*>(content)->plusAssign(
         *reinterpret_cast<Implementation*>(source.content), DAffine::Equation::PCSourceXValue);
}

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
void
TFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>::minusAssign(const thisType& source) {
   typedef DAffine::TFloatZonotope<DAffine::ExecutionPath, USizeMantissa, USizeExponent, TypeImplementation> Implementation;
   reinterpret_cast<Implementation*>(content)->minusAssign(
         *reinterpret_cast<const Implementation*>(source.content), DAffine::Equation::PCSourceRValue);
}

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
void
TFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>::minusAssign(thisType&& source) {
   typedef DAffine::TFloatZonotope<DAffine::ExecutionPath, USizeMantissa, USizeExponent, TypeImplementation> Implementation;
   reinterpret_cast<Implementation*>(content)->minusAssign(
         *reinterpret_cast<Implementation*>(source.content), DAffine::Equation::PCSourceXValue);
}

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
void
TFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>::multAssign(const thisType& source) {
   typedef DAffine::TFloatZonotope<DAffine::ExecutionPath, USizeMantissa, USizeExponent, TypeImplementation> Implementation;
   reinterpret_cast<Implementation*>(content)->multAssign(
         *reinterpret_cast<const Implementation*>(source.content), DAffine::Equation::PCSourceRValue);
}

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
void
TFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>::multAssign(thisType&& source) {
   typedef DAffine::TFloatZonotope<DAffine::ExecutionPath, USizeMantissa, USizeExponent, TypeImplementation> Implementation;
   reinterpret_cast<Implementation*>(content)->multAssign(
         *reinterpret_cast<Implementation*>(source.content), DAffine::Equation::PCSourceXValue);
}

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
void
TFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>::divAssign(const thisType& source) {
   typedef DAffine::TFloatZonotope<DAffine::ExecutionPath, USizeMantissa, USizeExponent, TypeImplementation> Implementation;
   reinterpret_cast<Implementation*>(content)->divAssign(
         *reinterpret_cast<const Implementation*>(source.content), DAffine::Equation::PCSourceRValue);
}

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
void
TFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>::divAssign(thisType&& source) {
   typedef DAffine::TFloatZonotope<DAffine::ExecutionPath, USizeMantissa, USizeExponent, TypeImplementation> Implementation;
   reinterpret_cast<Implementation*>(content)->divAssign(
         *reinterpret_cast<Implementation*>(source.content), DAffine::Equation::PCSourceXValue);
}

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
TFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>::operator TypeImplementation() const {
   typedef DAffine::TFloatZonotope<DAffine::ExecutionPath, USizeMantissa, USizeExponent, TypeImplementation> Implementation;
   return reinterpret_cast<const Implementation*>(content)->asImplementation();
}

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
TFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>::operator int() const {
   try {
   typedef DAffine::TFloatZonotope<DAffine::ExecutionPath, USizeMantissa, USizeExponent, TypeImplementation> Implementation;
   return reinterpret_cast<const Implementation*>(content)->asInt();
   }
   catch (DAffine::ExecutionPath::anticipated_termination) {
      throw DAffineInterface::ExecutionPath::anticipated_termination();
   };
}

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
TFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>::operator short int() const {
   try {
   typedef DAffine::TFloatZonotope<DAffine::ExecutionPath, USizeMantissa, USizeExponent, TypeImplementation> Implementation;
   return reinterpret_cast<const Implementation*>(content)->asInt();
   }
   catch (DAffine::ExecutionPath::anticipated_termination) {
      throw DAffineInterface::ExecutionPath::anticipated_termination();
   };
}

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
TFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>::operator unsigned() const {
   try {
   typedef DAffine::TFloatZonotope<DAffine::ExecutionPath, USizeMantissa, USizeExponent, TypeImplementation> Implementation;
   return reinterpret_cast<const Implementation*>(content)->asUnsigned();
   }
   catch (DAffine::ExecutionPath::anticipated_termination) {
      throw DAffineInterface::ExecutionPath::anticipated_termination();
   };
}

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
TFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>::operator short unsigned() const {
   try {
   typedef DAffine::TFloatZonotope<DAffine::ExecutionPath, USizeMantissa, USizeExponent, TypeImplementation> Implementation;
   return reinterpret_cast<const Implementation*>(content)->asUnsigned();
   }
   catch (DAffine::ExecutionPath::anticipated_termination) {
      throw DAffineInterface::ExecutionPath::anticipated_termination();
   };
}

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
TFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>::operator long int() const {
   try {
   typedef DAffine::TFloatZonotope<DAffine::ExecutionPath, USizeMantissa, USizeExponent, TypeImplementation> Implementation;
   return reinterpret_cast<const Implementation*>(content)->asLong();
   }
   catch (DAffine::ExecutionPath::anticipated_termination) {
      throw DAffineInterface::ExecutionPath::anticipated_termination();
   };
}

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
TFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>::operator unsigned long() const {
   try {
   typedef DAffine::TFloatZonotope<DAffine::ExecutionPath, USizeMantissa, USizeExponent, TypeImplementation> Implementation;
   return reinterpret_cast<const Implementation*>(content)->asUnsignedLong();
   }
   catch (DAffine::ExecutionPath::anticipated_termination) {
      throw DAffineInterface::ExecutionPath::anticipated_termination();
   };
}

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
void
TFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>::lightPersist(const char* prefix) const {
   typedef DAffine::TFloatZonotope<DAffine::ExecutionPath, USizeMantissa, USizeExponent, TypeImplementation> Implementation;
   const Implementation& thisZonotope = *reinterpret_cast<const Implementation*>(content);
   thisZonotope.lightPersist(thisZonotope, prefix);
}

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
void
TFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>::persist(const char* prefix) const {
   typedef DAffine::TFloatZonotope<DAffine::ExecutionPath, USizeMantissa, USizeExponent, TypeImplementation> Implementation;
   const Implementation& thisZonotope = *reinterpret_cast<const Implementation*>(content);
   thisZonotope.persist(thisZonotope, prefix);
}

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
void
TFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>::inverseAssign() {
   typedef DAffine::TFloatZonotope<DAffine::ExecutionPath, USizeMantissa, USizeExponent, TypeImplementation> Implementation;
   reinterpret_cast<Implementation*>(content)->inverseAssign();
}

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
void
TFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>::sqrtAssign() {
   typedef DAffine::TFloatZonotope<DAffine::ExecutionPath, USizeMantissa, USizeExponent, TypeImplementation> Implementation;
   reinterpret_cast<Implementation*>(content)->sqrtAssign();
}

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
void
TFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>::sinAssign() {
   typedef DAffine::TFloatZonotope<DAffine::ExecutionPath, USizeMantissa, USizeExponent, TypeImplementation> Implementation;
   reinterpret_cast<Implementation*>(content)->sinAssign();
}

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
void
TFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>::cosAssign() {
   typedef DAffine::TFloatZonotope<DAffine::ExecutionPath, USizeMantissa, USizeExponent, TypeImplementation> Implementation;
   reinterpret_cast<Implementation*>(content)->cosAssign();
}

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
void
TFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>::asinAssign() {
   typedef DAffine::TFloatZonotope<DAffine::ExecutionPath, USizeMantissa, USizeExponent, TypeImplementation> Implementation;
   reinterpret_cast<Implementation*>(content)->asinAssign();
}

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
void
TFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>::acosAssign() {
   typedef DAffine::TFloatZonotope<DAffine::ExecutionPath, USizeMantissa, USizeExponent, TypeImplementation> Implementation;
   reinterpret_cast<Implementation*>(content)->acosAssign();
}

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
void
TFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>::tanAssign() {
   typedef DAffine::TFloatZonotope<DAffine::ExecutionPath, USizeMantissa, USizeExponent, TypeImplementation> Implementation;
   reinterpret_cast<Implementation*>(content)->tanAssign();
}

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
void
TFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>::atanAssign() {
   typedef DAffine::TFloatZonotope<DAffine::ExecutionPath, USizeMantissa, USizeExponent, TypeImplementation> Implementation;
   reinterpret_cast<Implementation*>(content)->atanAssign();
}

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
void
TFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>::expAssign() {
   typedef DAffine::TFloatZonotope<DAffine::ExecutionPath, USizeMantissa, USizeExponent, TypeImplementation> Implementation;
   reinterpret_cast<Implementation*>(content)->expAssign();
}

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
void
TFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>::logAssign() {
   typedef DAffine::TFloatZonotope<DAffine::ExecutionPath, USizeMantissa, USizeExponent, TypeImplementation> Implementation;
   reinterpret_cast<Implementation*>(content)->logAssign();
}

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
void
TFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>::log10Assign() {
   typedef DAffine::TFloatZonotope<DAffine::ExecutionPath, USizeMantissa, USizeExponent, TypeImplementation> Implementation;
   reinterpret_cast<Implementation*>(content)->log10Assign();
}

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
void
TFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>::powAssign(const thisType& value) {
   typedef DAffine::TFloatZonotope<DAffine::ExecutionPath, USizeMantissa, USizeExponent, TypeImplementation> Implementation;
   reinterpret_cast<Implementation*>(content)->powAssign(
         *reinterpret_cast<const Implementation*>(value.content), DAffine::Equation::PCSourceRValue);
}

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
void
TFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>::powAssign(thisType&& value) {
   typedef DAffine::TFloatZonotope<DAffine::ExecutionPath, USizeMantissa, USizeExponent, TypeImplementation> Implementation;
   reinterpret_cast<Implementation*>(content)->powAssign(
         *reinterpret_cast<const Implementation*>(value.content), DAffine::Equation::PCSourceXValue);
}

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
void
TFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>::atan2Assign(const thisType& value) {
   typedef DAffine::TFloatZonotope<DAffine::ExecutionPath, USizeMantissa, USizeExponent, TypeImplementation> Implementation;
   reinterpret_cast<Implementation*>(content)->atan2Assign(
         *reinterpret_cast<const Implementation*>(value.content), DAffine::Equation::PCSourceRValue);
}

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
void
TFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>::atan2Assign(thisType&& value) {
   typedef DAffine::TFloatZonotope<DAffine::ExecutionPath, USizeMantissa, USizeExponent, TypeImplementation> Implementation;
   reinterpret_cast<Implementation*>(content)->atan2Assign(
         *reinterpret_cast<const Implementation*>(value.content), DAffine::Equation::PCSourceXValue);
}

template class TFloatZonotope<23, 8, float>;
template class TFloatZonotope<52, 11, double>;
template class TFloatZonotope<80, 15, long double>;

template TFloatZonotope<23, 8, float>::TFloatZonotope(const TFloatZonotope<52, 11, double>&);
template TFloatZonotope<23, 8, float>::TFloatZonotope(const TFloatZonotope<80, 15, long double>&);
template TFloatZonotope<52, 11, double>::TFloatZonotope(const TFloatZonotope<23, 8, float>&);
template TFloatZonotope<52, 11, double>::TFloatZonotope(const TFloatZonotope<80, 15, long double>&);
template TFloatZonotope<80, 15, long double>::TFloatZonotope(const TFloatZonotope<23, 8, float>&);
template TFloatZonotope<80, 15, long double>::TFloatZonotope(const TFloatZonotope<52, 11, double>&);

template TFloatZonotope<23, 8, float>& TFloatZonotope<23, 8, float>::operator=(const TFloatZonotope<52, 11, double>&);
template TFloatZonotope<23, 8, float>& TFloatZonotope<23, 8, float>::operator=(const TFloatZonotope<80, 15, long double>&);
template TFloatZonotope<52, 11, double>& TFloatZonotope<52, 11, double>::operator=(const TFloatZonotope<23, 8, float>&);
template TFloatZonotope<52, 11, double>& TFloatZonotope<52, 11, double>::operator=(const TFloatZonotope<80, 15, long double>&);
template TFloatZonotope<80, 15, long double>& TFloatZonotope<80, 15, long double>::operator=(const TFloatZonotope<23, 8, float>&);
template TFloatZonotope<80, 15, long double>& TFloatZonotope<80, 15, long double>::operator=(const TFloatZonotope<52, 11, double>&);

template TFloatZonotope<23, 8, float>& TFloatZonotope<23, 8, float>::operator=(TFloatZonotope<52, 11, double>&&);
template TFloatZonotope<23, 8, float>& TFloatZonotope<23, 8, float>::operator=(TFloatZonotope<80, 15, long double>&&);
template TFloatZonotope<52, 11, double>& TFloatZonotope<52, 11, double>::operator=(TFloatZonotope<23, 8, float>&&);
template TFloatZonotope<52, 11, double>& TFloatZonotope<52, 11, double>::operator=(TFloatZonotope<80, 15, long double>&&);
template TFloatZonotope<80, 15, long double>& TFloatZonotope<80, 15, long double>::operator=(TFloatZonotope<23, 8, float>&&);
template TFloatZonotope<80, 15, long double>& TFloatZonotope<80, 15, long double>::operator=(TFloatZonotope<52, 11, double>&&);

template MergeBranches& MergeBranches::operator<<(TFloatZonotope<23, 8, float>&);
template MergeBranches& MergeBranches::operator<<(TFloatZonotope<52, 11, double>&);
template MergeBranches& MergeBranches::operator<<(TFloatZonotope<80, 15, long double>&);

}} // end of namespace NumericalDomains::DAffineInterface

