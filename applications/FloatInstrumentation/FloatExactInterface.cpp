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
// Unit      : FloatExact
// File      : FloatExactInterface.cpp
// Description :
//   Implementation of a class of comparison between floating points
//

#include <fstream>
#include <sstream>

#include "NumericalAnalysis/FloatExactExecutionPath.h"
#include "FloatInstrumentation/FloatExactInterface.h"

#include "NumericalAnalysis/FloatExactExecutionPath.template"

#if !defined(FLOAT_GENERIC_BASE_UNSIGNED) && !defined(FLOAT_GENERIC_BASE_LONG)
#include "NumericalDomains/FloatExactBase.template"
#else
#include "NumericalDomains/FloatExactBaseGeneric.template"
#endif

#ifndef FLOAT_LONG_WRITE
#if !defined(FLOAT_GENERIC_BASE_UNSIGNED) && !defined(FLOAT_GENERIC_BASE_LONG)
#include "NumericalLattices/FloatExactBaseTypes.template"
#else
#include "NumericalLattices/FloatExactBaseTypesGeneric.template"
#endif
#endif

#include "Pointer/Vector.template"
#include "FloatInstrumentation/FloatExact.inch"
#include "FloatInstrumentation/FloatExact.incc"

namespace NumericalDomains { namespace DDoubleExact {

#if !defined(FLOAT_GENERIC_BASE_UNSIGNED) && !defined(FLOAT_GENERIC_BASE_LONG)
template <int USizeMantissa, int USizeExponent>
using TBuiltFloatExact = TBuiltFloat<FLOAT_REAL_BITS_NUMBER, USizeMantissa, USizeExponent>;
#elif defined(FLOAT_GENERIC_BASE_LONG)
template <int USizeMantissa, int USizeExponent>
using TBuiltFloatExact = TGBuiltFloat<Numerics::UnsignedLongBaseStoreTraits, FLOAT_REAL_BITS_NUMBER, USizeMantissa, USizeExponent>;
#else // defined(FLOAT_GENERIC_BASE_UNSIGNED)
template <int USizeMantissa, int USizeExponent>
using TBuiltFloatExact = TGBuiltFloat<Numerics::UnsignedBaseStoreTraits, FLOAT_REAL_BITS_NUMBER, USizeMantissa, USizeExponent>;
#endif

} // end of namespace DDoubleExact

namespace DDoubleExactInterface {

void
ExecutionPath::splitBranches(const char* file, int line) {
   DDoubleExact::BaseFloatExact::splitBranches(file, line);
}

void
ExecutionPath::setSupportAtomic() {
   DDoubleExact::ExecutionPath::setSupportAtomic();
}

void
ExecutionPath::setSupportUnstableInLoop() {
   DDoubleExact::ExecutionPath::setSupportUnstableInLoop();
}

void
ExecutionPath::setSupportVerbose() {
   DDoubleExact::ExecutionPath::setSupportVerbose();
}

void
ExecutionPath::setSupportThreshold() {
   DDoubleExact::ExecutionPath::setSupportThreshold();
}

void
ExecutionPath::setSupportFirstFollowFloat() {
   DDoubleExact::ExecutionPath::setSupportFirstFollowFloat();
}

void
ExecutionPath::initializeGlobals(const char* fileSuffix) {
   DDoubleExact::ExecutionPath::initializeGlobals(fileSuffix);
}

void
ExecutionPath::finalizeGlobals() {
   DDoubleExact::ExecutionPath::finalizeGlobals();
}

void
ExecutionPath::flushOut() {
   DDoubleExact::ExecutionPath::flushOut();
}

void
ExecutionPath::setSourceLine(const char* file, int line) {
   DDoubleExact::ExecutionPath::setSourceLine(file, line);
}

void
ExecutionPath::writeCurrentPath(std::ostream& out) {
   DDoubleExact::ExecutionPath::writeCurrentPath(out);
}

int
ExecutionPath::getCurrentUnstableBranch() {
   return DDoubleExact::ExecutionPath::getCurrentUnstableBranch();
}

// float loop unstable
bool
ExecutionPath::doesFollowFlow() {
   return DDoubleExact::ExecutionPath::doesFollowFlow();
}

bool
ExecutionPath::setFollowFlow(bool doesFollow) {
   return DDoubleExact::ExecutionPath::setFollowFlow(doesFollow);
}

void
ExecutionPath::clearFollowFlow() {
   DDoubleExact::ExecutionPath::clearFollowFlow();
}
// end of float loop unstable

#if defined __GNUC__ && GCC_VERSION >= 50000
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wstrict-aliasing"
#endif

#include "StandardClasses/UndefineNew.h"
MergeBranches::MergeBranches(const char* file, int line) {
   AssumeCondition(sizeof(DDoubleExact::MergeBranches) <= UMergeBranchesSize*sizeof(AlignType))
   new (content) DDoubleExact::MergeBranches(file, line);
}
#include "StandardClasses/DefineNew.h"

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
MergeBranches&
MergeBranches::operator<<(TFloatExact<USizeMantissa, USizeExponent, TypeImplementation>& value) {
   reinterpret_cast<DDoubleExact::MergeBranches*>(content)->operator<<(
         *reinterpret_cast<DDoubleExact::TFloatExact<DDoubleExact::ExecutionPath,
            DDoubleExact::TBuiltFloatExact<USizeMantissa, USizeExponent>, TypeImplementation>*>(value.content));
   return *this;
}

bool
MergeBranches::operator<<(end) {
   try {
   return reinterpret_cast<DDoubleExact::MergeBranches*>(content)->operator<<(
         DDoubleExact::BaseFloatExact::end());
   }
   catch (DDoubleExact::ExecutionPath::anticipated_termination) {
      throw DDoubleExactInterface::ExecutionPath::anticipated_termination();
   };
}

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
const char*
TFloatExact<USizeMantissa, USizeExponent, TypeImplementation>::queryDebugValue() const {
   return reinterpret_cast<const DDoubleExact::TFloatExact<DDoubleExact::ExecutionPath,
          DDoubleExact::TBuiltFloatExact<USizeMantissa, USizeExponent>, TypeImplementation>*>(content)
      ->queryDebugValue();
}

#include "StandardClasses/UndefineNew.h"
template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
TFloatExact<USizeMantissa, USizeExponent, TypeImplementation>::TFloatExact() {
   typedef DDoubleExact::TFloatExact<DDoubleExact::ExecutionPath, DDoubleExact::TBuiltFloatExact<USizeMantissa, USizeExponent>, TypeImplementation> Implementation;
   AssumeCondition(sizeof(Implementation) <= UFloatExactSize*sizeof(AlignType))
   new (content) Implementation();
}

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
TFloatExact<USizeMantissa, USizeExponent, TypeImplementation>::TFloatExact(float value) {
   typedef DDoubleExact::TFloatExact<DDoubleExact::ExecutionPath, DDoubleExact::TBuiltFloatExact<USizeMantissa, USizeExponent>, TypeImplementation> Implementation;
   AssumeCondition(sizeof(Implementation) <= UFloatExactSize*sizeof(AlignType))
   new (content) Implementation();
   reinterpret_cast<Implementation*>(content)->initFrom(value);
}

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
TFloatExact<USizeMantissa, USizeExponent, TypeImplementation>::TFloatExact(float value, float error, ErrorParameter) {
   typedef DDoubleExact::TFloatExact<DDoubleExact::ExecutionPath, DDoubleExact::TBuiltFloatExact<USizeMantissa, USizeExponent>, TypeImplementation> Implementation;
   AssumeCondition(sizeof(Implementation) <= UFloatExactSize*sizeof(AlignType))
   new (content) Implementation();
   reinterpret_cast<Implementation*>(content)->initFrom(value);
   Implementation addition;
   addition.initFrom(error);
   addition.clearImplementation();
   reinterpret_cast<Implementation*>(content)->operator+=(addition);
}

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
TFloatExact<USizeMantissa, USizeExponent, TypeImplementation>::TFloatExact(double value) {
   typedef DDoubleExact::TFloatExact<DDoubleExact::ExecutionPath, DDoubleExact::TBuiltFloatExact<USizeMantissa, USizeExponent>, TypeImplementation> Implementation;
   AssumeCondition(sizeof(Implementation) <= UFloatExactSize*sizeof(AlignType))
   new (content) Implementation();
   reinterpret_cast<Implementation*>(content)->initFrom(value);
}

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
TFloatExact<USizeMantissa, USizeExponent, TypeImplementation>::TFloatExact(double value, double error, ErrorParameter) {
   typedef DDoubleExact::TFloatExact<DDoubleExact::ExecutionPath, DDoubleExact::TBuiltFloatExact<USizeMantissa, USizeExponent>, TypeImplementation> Implementation;
   AssumeCondition(sizeof(Implementation) <= UFloatExactSize*sizeof(AlignType))
   new (content) Implementation();
   reinterpret_cast<Implementation*>(content)->initFrom(value);
   Implementation addition;
   addition.initFrom(error);
   addition.clearImplementation();
   reinterpret_cast<Implementation*>(content)->operator+=(addition);
}

/*
template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
TFloatExact<USizeMantissa, USizeExponent, TypeImplementation>::TFloatExact(long double value) {
   typedef DDoubleExact::TFloatExact<DDoubleExact::ExecutionPath, DDoubleExact::TBuiltFloatExact<USizeMantissa, USizeExponent>, TypeImplementation> Implementation;
   AssumeCondition(sizeof(Implementation) <= UFloatExactSize*sizeof(AlignType))
   new (content) Implementation();
   reinterpret_cast<Implementation*>(content)->initFrom(value);
}
*/

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
TFloatExact<USizeMantissa, USizeExponent, TypeImplementation>::TFloatExact(long double value) {
   typedef DDoubleExact::TFloatExact<DDoubleExact::ExecutionPath, DDoubleExact::TBuiltFloatExact<USizeMantissa, USizeExponent>, TypeImplementation> Implementation;
   AssumeCondition(sizeof(Implementation) <= UFloatExactSize*sizeof(AlignType))
   new (content) Implementation();

   DDoubleExact::TFloatExact<DDoubleExact::ExecutionPath, DDoubleExact::BuiltLongDouble, long double> receiver;
   receiver.initFrom(value);
   reinterpret_cast<Implementation*>(content)->operator=(std::move(receiver));
}

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
TFloatExact<USizeMantissa, USizeExponent, TypeImplementation>::TFloatExact(long double value, long double error, ErrorParameter) {
   typedef DDoubleExact::TFloatExact<DDoubleExact::ExecutionPath, DDoubleExact::TBuiltFloatExact<USizeMantissa, USizeExponent>, TypeImplementation> Implementation;
   AssumeCondition(sizeof(Implementation) <= UFloatExactSize*sizeof(AlignType))
   new (content) Implementation();
   reinterpret_cast<Implementation*>(content)->initFrom(value);
   Implementation addition;
   addition.initFrom(error);
   addition.clearImplementation();
   reinterpret_cast<Implementation*>(content)->operator+=(addition);
}

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
TFloatExact<USizeMantissa, USizeExponent, TypeImplementation>::TFloatExact(int value) {
   typedef DDoubleExact::TFloatExact<DDoubleExact::ExecutionPath, DDoubleExact::TBuiltFloatExact<USizeMantissa, USizeExponent>, TypeImplementation> Implementation;
   AssumeCondition(sizeof(Implementation) <= UFloatExactSize*sizeof(AlignType))
   new (content) Implementation(value);
}

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
TFloatExact<USizeMantissa, USizeExponent, TypeImplementation>::TFloatExact(long int value) {
   typedef DDoubleExact::TFloatExact<DDoubleExact::ExecutionPath, DDoubleExact::TBuiltFloatExact<USizeMantissa, USizeExponent>, TypeImplementation> Implementation;
   AssumeCondition(sizeof(Implementation) <= UFloatExactSize*sizeof(AlignType))
   new (content) Implementation(value);
}

/*
template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
TFloatExact<USizeMantissa, USizeExponent, TypeImplementation>::TFloatExact(unsigned value) {
   typedef DDoubleExact::TFloatExact<DDoubleExact::ExecutionPath, DDoubleExact::TBuiltFloatExact<USizeMantissa, USizeExponent>, TypeImplementation> Implementation;
   AssumeCondition(sizeof(Implementation) <= UFloatExactSize*sizeof(AlignType))
   new (content) Implementation(value);
}

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
TFloatExact<USizeMantissa, USizeExponent, TypeImplementation>::TFloatExact(unsigned long value) {
   typedef DDoubleExact::TFloatExact<DDoubleExact::ExecutionPath, DDoubleExact::TBuiltFloatExact<USizeMantissa, USizeExponent>, TypeImplementation> Implementation;
   AssumeCondition(sizeof(Implementation) <= UFloatExactSize*sizeof(AlignType))
   new (content) Implementation(value);
}
*/

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
TFloatExact<USizeMantissa, USizeExponent, TypeImplementation>::TFloatExact(const thisType& asource) {
   typedef DDoubleExact::TFloatExact<DDoubleExact::ExecutionPath, DDoubleExact::TBuiltFloatExact<USizeMantissa, USizeExponent>, TypeImplementation> Implementation;
   const Implementation& source = *reinterpret_cast<const Implementation*>(asource.content);
   new (content) Implementation(source);
}

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
template <int USizeMantissaArgument, int USizeExponentArgument, typename TypeImplementationArgument>
TFloatExact<USizeMantissa, USizeExponent, TypeImplementation>::TFloatExact(
      const TFloatExact<USizeMantissaArgument, USizeExponentArgument, TypeImplementationArgument>& asource) {
   typedef DDoubleExact::TFloatExact<DDoubleExact::ExecutionPath, DDoubleExact::TBuiltFloatExact<USizeMantissa, USizeExponent>, TypeImplementation> Implementation;
   typedef DDoubleExact::TFloatExact<DDoubleExact::ExecutionPath, DDoubleExact::TBuiltFloatExact<USizeMantissaArgument, USizeExponentArgument>, TypeImplementationArgument> ImplementationSource;
   const ImplementationSource& source = *reinterpret_cast<const ImplementationSource*>(asource.content);
   new (content) Implementation(source);
}

#include "StandardClasses/DefineNew.h"

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
TFloatExact<USizeMantissa, USizeExponent, TypeImplementation>::~TFloatExact() {
   typedef DDoubleExact::TFloatExact<DDoubleExact::ExecutionPath, DDoubleExact::TBuiltFloatExact<USizeMantissa, USizeExponent>, TypeImplementation> Implementation;
   reinterpret_cast<Implementation*>(content)->~Implementation();
}

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
TFloatExact<USizeMantissa, USizeExponent, TypeImplementation>&
TFloatExact<USizeMantissa, USizeExponent, TypeImplementation>::operator=(const thisType& asource) {
   typedef DDoubleExact::TFloatExact<DDoubleExact::ExecutionPath, DDoubleExact::TBuiltFloatExact<USizeMantissa, USizeExponent>, TypeImplementation> Implementation;
   const Implementation& source = *reinterpret_cast<const Implementation*>(asource.content);
   Implementation& thisExact = *reinterpret_cast<Implementation*>(content);
   thisExact.operator=(source);
   return *this;
}

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
template <int USizeMantissaArgument, int USizeExponentArgument, typename TypeImplementationArgument>
TFloatExact<USizeMantissa, USizeExponent, TypeImplementation>&
TFloatExact<USizeMantissa, USizeExponent, TypeImplementation>::operator=(
      const TFloatExact<USizeMantissaArgument, USizeExponentArgument, TypeImplementationArgument>& asource) {
   typedef DDoubleExact::TFloatExact<DDoubleExact::ExecutionPath, DDoubleExact::TBuiltFloatExact<USizeMantissa, USizeExponent>, TypeImplementation> Implementation;
   typedef DDoubleExact::TFloatExact<DDoubleExact::ExecutionPath, DDoubleExact::TBuiltFloatExact<USizeMantissaArgument, USizeExponentArgument>, TypeImplementationArgument> ImplementationSource;
   const ImplementationSource& source = *reinterpret_cast<const ImplementationSource*>(asource.content);
   Implementation& thisExact = *reinterpret_cast<Implementation*>(content);
   thisExact.operator=(source);
   return *this;
}

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
void
TFloatExact<USizeMantissa, USizeExponent, TypeImplementation>::writeImplementation(
      std::ostream& out) const {
   typedef DDoubleExact::TFloatExact<DDoubleExact::ExecutionPath, DDoubleExact::TBuiltFloatExact<USizeMantissa, USizeExponent>, TypeImplementation> Implementation;
   out << reinterpret_cast<const Implementation*>(content)->asImplementation();
}

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
void
TFloatExact<USizeMantissa, USizeExponent, TypeImplementation>::readImplementation(
      std::istream& in) {
   typedef DDoubleExact::TFloatExact<DDoubleExact::ExecutionPath, DDoubleExact::TBuiltFloatExact<USizeMantissa, USizeExponent>, TypeImplementation> Implementation;
   decltype(reinterpret_cast<const Implementation*>(content)->asImplementation()) result; 
   in >> result;
   reinterpret_cast<Implementation*>(content)->initFrom(result);
}

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
bool
TFloatExact<USizeMantissa, USizeExponent, TypeImplementation>::operator<(const thisType& asource) const {
   try {
   typedef DDoubleExact::TFloatExact<DDoubleExact::ExecutionPath, DDoubleExact::TBuiltFloatExact<USizeMantissa, USizeExponent>, TypeImplementation> Implementation;
   const Implementation& source = *reinterpret_cast<const Implementation*>(asource.content);
   const Implementation& thisExact = *reinterpret_cast<const Implementation*>(content);
   return thisExact.operator<(source);
   }
   catch (DDoubleExact::ExecutionPath::anticipated_termination) {
      throw DDoubleExactInterface::ExecutionPath::anticipated_termination();
   };
}

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
bool
TFloatExact<USizeMantissa, USizeExponent, TypeImplementation>::operator<(TypeImplementation source) const {
   try {
   typedef DDoubleExact::TFloatExact<DDoubleExact::ExecutionPath, DDoubleExact::TBuiltFloatExact<USizeMantissa, USizeExponent>, TypeImplementation> Implementation;
   const Implementation& thisExact = *reinterpret_cast<const Implementation*>(content);
   TFloatExact<USizeMantissa, USizeExponent, TypeImplementation> asource(source);
   return thisExact.operator<(*reinterpret_cast<const Implementation*>(asource.content));
   }
   catch (DDoubleExact::ExecutionPath::anticipated_termination) {
      throw DDoubleExactInterface::ExecutionPath::anticipated_termination();
   };
}

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
bool
TFloatExact<USizeMantissa, USizeExponent, TypeImplementation>::operator<=(const thisType& asource) const {
   try {
   typedef DDoubleExact::TFloatExact<DDoubleExact::ExecutionPath, DDoubleExact::TBuiltFloatExact<USizeMantissa, USizeExponent>, TypeImplementation> Implementation;
   const Implementation& source = *reinterpret_cast<const Implementation*>(asource.content);
   const Implementation& thisExact = *reinterpret_cast<const Implementation*>(content);
   return thisExact.operator<=(source);
   }
   catch (DDoubleExact::ExecutionPath::anticipated_termination) {
      throw DDoubleExactInterface::ExecutionPath::anticipated_termination();
   };
}

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
bool
TFloatExact<USizeMantissa, USizeExponent, TypeImplementation>::operator<=(TypeImplementation source) const {
   try {
   typedef DDoubleExact::TFloatExact<DDoubleExact::ExecutionPath, DDoubleExact::TBuiltFloatExact<USizeMantissa, USizeExponent>, TypeImplementation> Implementation;
   const Implementation& thisExact = *reinterpret_cast<const Implementation*>(content);
   TFloatExact<USizeMantissa, USizeExponent, TypeImplementation> asource(source);
   return thisExact.operator<=(*reinterpret_cast<const Implementation*>(asource.content));
   }
   catch (DDoubleExact::ExecutionPath::anticipated_termination) {
      throw DDoubleExactInterface::ExecutionPath::anticipated_termination();
   };
}

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
bool
TFloatExact<USizeMantissa, USizeExponent, TypeImplementation>::operator==(const thisType& asource) const {
   try {
   typedef DDoubleExact::TFloatExact<DDoubleExact::ExecutionPath, DDoubleExact::TBuiltFloatExact<USizeMantissa, USizeExponent>, TypeImplementation> Implementation;
   const Implementation& source = *reinterpret_cast<const Implementation*>(asource.content);
   const Implementation& thisExact = *reinterpret_cast<const Implementation*>(content);
   return thisExact.operator==(source);
   }
   catch (DDoubleExact::ExecutionPath::anticipated_termination) {
      throw DDoubleExactInterface::ExecutionPath::anticipated_termination();
   };
}

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
bool
TFloatExact<USizeMantissa, USizeExponent, TypeImplementation>::operator==(TypeImplementation source) const {
   try {
   typedef DDoubleExact::TFloatExact<DDoubleExact::ExecutionPath, DDoubleExact::TBuiltFloatExact<USizeMantissa, USizeExponent>, TypeImplementation> Implementation;
   const Implementation& thisExact = *reinterpret_cast<const Implementation*>(content);
   TFloatExact<USizeMantissa, USizeExponent, TypeImplementation> asource(source);
   return thisExact.operator==(*reinterpret_cast<const Implementation*>(asource.content));
   }
   catch (DDoubleExact::ExecutionPath::anticipated_termination) {
      throw DDoubleExactInterface::ExecutionPath::anticipated_termination();
   };
}

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
bool
TFloatExact<USizeMantissa, USizeExponent, TypeImplementation>::operator!=(const thisType& asource) const {
   try {
   typedef DDoubleExact::TFloatExact<DDoubleExact::ExecutionPath, DDoubleExact::TBuiltFloatExact<USizeMantissa, USizeExponent>, TypeImplementation> Implementation;
   const Implementation& source = *reinterpret_cast<const Implementation*>(asource.content);
   const Implementation& thisExact = *reinterpret_cast<const Implementation*>(content);
   return thisExact.operator!=(source);
   }
   catch (DDoubleExact::ExecutionPath::anticipated_termination) {
      throw DDoubleExactInterface::ExecutionPath::anticipated_termination();
   };
}

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
bool
TFloatExact<USizeMantissa, USizeExponent, TypeImplementation>::operator!=(TypeImplementation source) const {
   try {
   typedef DDoubleExact::TFloatExact<DDoubleExact::ExecutionPath, DDoubleExact::TBuiltFloatExact<USizeMantissa, USizeExponent>, TypeImplementation> Implementation;
   const Implementation& thisExact = *reinterpret_cast<const Implementation*>(content);
   TFloatExact<USizeMantissa, USizeExponent, TypeImplementation> asource(source);
   return thisExact.operator!=(*reinterpret_cast<const Implementation*>(asource.content));
   }
   catch (DDoubleExact::ExecutionPath::anticipated_termination) {
      throw DDoubleExactInterface::ExecutionPath::anticipated_termination();
   };
}

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
bool
TFloatExact<USizeMantissa, USizeExponent, TypeImplementation>::operator>=(const thisType& asource) const {
   try {
   typedef DDoubleExact::TFloatExact<DDoubleExact::ExecutionPath, DDoubleExact::TBuiltFloatExact<USizeMantissa, USizeExponent>, TypeImplementation> Implementation;
   const Implementation& source = *reinterpret_cast<const Implementation*>(asource.content);
   const Implementation& thisExact = *reinterpret_cast<const Implementation*>(content);
   return thisExact.operator>=(source);
   }
   catch (DDoubleExact::ExecutionPath::anticipated_termination) {
      throw DDoubleExactInterface::ExecutionPath::anticipated_termination();
   };
}

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
bool
TFloatExact<USizeMantissa, USizeExponent, TypeImplementation>::operator>=(TypeImplementation source) const {
   try {
   typedef DDoubleExact::TFloatExact<DDoubleExact::ExecutionPath, DDoubleExact::TBuiltFloatExact<USizeMantissa, USizeExponent>, TypeImplementation> Implementation;
   const Implementation& thisExact = *reinterpret_cast<const Implementation*>(content);
   TFloatExact<USizeMantissa, USizeExponent, TypeImplementation> asource(source);
   return thisExact.operator>=(*reinterpret_cast<const Implementation*>(asource.content));
   }
   catch (DDoubleExact::ExecutionPath::anticipated_termination) {
      throw DDoubleExactInterface::ExecutionPath::anticipated_termination();
   };
}

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
bool
TFloatExact<USizeMantissa, USizeExponent, TypeImplementation>::operator>(const thisType& asource) const {
   try {
   typedef DDoubleExact::TFloatExact<DDoubleExact::ExecutionPath, DDoubleExact::TBuiltFloatExact<USizeMantissa, USizeExponent>, TypeImplementation> Implementation;
   const Implementation& source = *reinterpret_cast<const Implementation*>(asource.content);
   const Implementation& thisExact = *reinterpret_cast<const Implementation*>(content);
   return thisExact.operator>(source);
   }
   catch (DDoubleExact::ExecutionPath::anticipated_termination) {
      throw DDoubleExactInterface::ExecutionPath::anticipated_termination();
   };
}

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
bool
TFloatExact<USizeMantissa, USizeExponent, TypeImplementation>::operator>(TypeImplementation source) const {
   try {
   typedef DDoubleExact::TFloatExact<DDoubleExact::ExecutionPath, DDoubleExact::TBuiltFloatExact<USizeMantissa, USizeExponent>, TypeImplementation> Implementation;
   const Implementation& thisExact = *reinterpret_cast<const Implementation*>(content);
   TFloatExact<USizeMantissa, USizeExponent, TypeImplementation> asource(source);
   return thisExact.operator>(*reinterpret_cast<const Implementation*>(asource.content));
   }
   catch (DDoubleExact::ExecutionPath::anticipated_termination) {
      throw DDoubleExactInterface::ExecutionPath::anticipated_termination();
   };
}

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
TFloatExact<USizeMantissa, USizeExponent, TypeImplementation>&
TFloatExact<USizeMantissa, USizeExponent, TypeImplementation>::operator+=(const thisType& asource) {
   typedef DDoubleExact::TFloatExact<DDoubleExact::ExecutionPath, DDoubleExact::TBuiltFloatExact<USizeMantissa, USizeExponent>, TypeImplementation> Implementation;
   const Implementation& source = *reinterpret_cast<const Implementation*>(asource.content);
   Implementation& thisExact = *reinterpret_cast<Implementation*>(content);
   thisExact += source;
   return *this;
}

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
TFloatExact<USizeMantissa, USizeExponent, TypeImplementation>&
TFloatExact<USizeMantissa, USizeExponent, TypeImplementation>::operator-=(const thisType& asource) {
   typedef DDoubleExact::TFloatExact<DDoubleExact::ExecutionPath, DDoubleExact::TBuiltFloatExact<USizeMantissa, USizeExponent>, TypeImplementation> Implementation;
   const Implementation& source = *reinterpret_cast<const Implementation*>(asource.content);
   Implementation& thisExact = *reinterpret_cast<Implementation*>(content);
   thisExact -= source;
   return *this;
}

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
TFloatExact<USizeMantissa, USizeExponent, TypeImplementation>&
TFloatExact<USizeMantissa, USizeExponent, TypeImplementation>::operator*=(const thisType& asource) {
   typedef DDoubleExact::TFloatExact<DDoubleExact::ExecutionPath, DDoubleExact::TBuiltFloatExact<USizeMantissa, USizeExponent>, TypeImplementation> Implementation;
   const Implementation& source = *reinterpret_cast<const Implementation*>(asource.content);
   Implementation& thisExact = *reinterpret_cast<Implementation*>(content);
   thisExact *= source;
   return *this;
}

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
TFloatExact<USizeMantissa, USizeExponent, TypeImplementation>&
TFloatExact<USizeMantissa, USizeExponent, TypeImplementation>::operator/=(const thisType& asource) {
   typedef DDoubleExact::TFloatExact<DDoubleExact::ExecutionPath, DDoubleExact::TBuiltFloatExact<USizeMantissa, USizeExponent>, TypeImplementation> Implementation;
   const Implementation& source = *reinterpret_cast<const Implementation*>(asource.content);
   Implementation& thisExact = *reinterpret_cast<Implementation*>(content);
   thisExact /= source;
   return *this;
}

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
void
TFloatExact<USizeMantissa, USizeExponent, TypeImplementation>::oppositeAssign() {
   typedef DDoubleExact::TFloatExact<DDoubleExact::ExecutionPath, DDoubleExact::TBuiltFloatExact<USizeMantissa, USizeExponent>, TypeImplementation> Implementation;
   reinterpret_cast<Implementation*>(content)->oppositeAssign();
}

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
TFloatExact<USizeMantissa, USizeExponent, TypeImplementation>::operator TypeImplementation() const {
   typedef DDoubleExact::TFloatExact<DDoubleExact::ExecutionPath, DDoubleExact::TBuiltFloatExact<USizeMantissa, USizeExponent>, TypeImplementation> Implementation;
   return reinterpret_cast<const Implementation*>(content)->asImplementation();
}

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
TFloatExact<USizeMantissa, USizeExponent, TypeImplementation>::operator int() const {
   typedef DDoubleExact::TFloatExact<DDoubleExact::ExecutionPath, DDoubleExact::TBuiltFloatExact<USizeMantissa, USizeExponent>, TypeImplementation> Implementation;
   typedef Numerics::DDouble::Access::ReadParameters ReadParametersBase;
   return reinterpret_cast<const Implementation*>(content)->asInt(ReadParametersBase::RMZero);
}

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
TFloatExact<USizeMantissa, USizeExponent, TypeImplementation>::operator short int() const {
   typedef DDoubleExact::TFloatExact<DDoubleExact::ExecutionPath, DDoubleExact::TBuiltFloatExact<USizeMantissa, USizeExponent>, TypeImplementation> Implementation;
   typedef Numerics::DDouble::Access::ReadParameters ReadParametersBase;
   return reinterpret_cast<const Implementation*>(content)->asInt(ReadParametersBase::RMZero);
}

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
TFloatExact<USizeMantissa, USizeExponent, TypeImplementation>::operator unsigned() const {
   typedef DDoubleExact::TFloatExact<DDoubleExact::ExecutionPath, DDoubleExact::TBuiltFloatExact<USizeMantissa, USizeExponent>, TypeImplementation> Implementation;
   typedef Numerics::DDouble::Access::ReadParameters ReadParametersBase;
   return reinterpret_cast<const Implementation*>(content)->asUnsigned(ReadParametersBase::RMZero);
}

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
TFloatExact<USizeMantissa, USizeExponent, TypeImplementation>::operator short unsigned() const {
   typedef DDoubleExact::TFloatExact<DDoubleExact::ExecutionPath, DDoubleExact::TBuiltFloatExact<USizeMantissa, USizeExponent>, TypeImplementation> Implementation;
   typedef Numerics::DDouble::Access::ReadParameters ReadParametersBase;
   return reinterpret_cast<const Implementation*>(content)->asUnsigned(ReadParametersBase::RMZero);
}

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
TFloatExact<USizeMantissa, USizeExponent, TypeImplementation>::operator long int() const {
   typedef DDoubleExact::TFloatExact<DDoubleExact::ExecutionPath, DDoubleExact::TBuiltFloatExact<USizeMantissa, USizeExponent>, TypeImplementation> Implementation;
   typedef Numerics::DDouble::Access::ReadParameters ReadParametersBase;
   return reinterpret_cast<const Implementation*>(content)->asLongInt(ReadParametersBase::RMZero);
}

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
TFloatExact<USizeMantissa, USizeExponent, TypeImplementation>::operator unsigned long() const {
   typedef DDoubleExact::TFloatExact<DDoubleExact::ExecutionPath, DDoubleExact::TBuiltFloatExact<USizeMantissa, USizeExponent>, TypeImplementation> Implementation;
   typedef Numerics::DDouble::Access::ReadParameters ReadParametersBase;
   return reinterpret_cast<const Implementation*>(content)->asUnsignedLong(ReadParametersBase::RMZero);
}

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
void
TFloatExact<USizeMantissa, USizeExponent, TypeImplementation>::persist(const char* prefix) const {
   typedef DDoubleExact::TFloatExact<DDoubleExact::ExecutionPath, DDoubleExact::TBuiltFloatExact<USizeMantissa, USizeExponent>, TypeImplementation> Implementation;
   const Implementation& thisExact = *reinterpret_cast<const Implementation*>(content);
   thisExact.notifyForPersistence(thisExact, prefix);
}

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
void
TFloatExact<USizeMantissa, USizeExponent, TypeImplementation>::sqrtAssign() {
   typedef DDoubleExact::TFloatExact<DDoubleExact::ExecutionPath, DDoubleExact::TBuiltFloatExact<USizeMantissa, USizeExponent>, TypeImplementation> Implementation;
   reinterpret_cast<Implementation*>(content)->sqrtAssign();
}

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
void
TFloatExact<USizeMantissa, USizeExponent, TypeImplementation>::sinAssign() {
   typedef DDoubleExact::TFloatExact<DDoubleExact::ExecutionPath, DDoubleExact::TBuiltFloatExact<USizeMantissa, USizeExponent>, TypeImplementation> Implementation;
   reinterpret_cast<Implementation*>(content)->sinAssign();
}

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
void
TFloatExact<USizeMantissa, USizeExponent, TypeImplementation>::cosAssign() {
   typedef DDoubleExact::TFloatExact<DDoubleExact::ExecutionPath, DDoubleExact::TBuiltFloatExact<USizeMantissa, USizeExponent>, TypeImplementation> Implementation;
   reinterpret_cast<Implementation*>(content)->cosAssign();
}

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
void
TFloatExact<USizeMantissa, USizeExponent, TypeImplementation>::asinAssign() {
   typedef DDoubleExact::TFloatExact<DDoubleExact::ExecutionPath, DDoubleExact::TBuiltFloatExact<USizeMantissa, USizeExponent>, TypeImplementation> Implementation;
   reinterpret_cast<Implementation*>(content)->asinAssign();
}

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
void
TFloatExact<USizeMantissa, USizeExponent, TypeImplementation>::acosAssign() {
   typedef DDoubleExact::TFloatExact<DDoubleExact::ExecutionPath, DDoubleExact::TBuiltFloatExact<USizeMantissa, USizeExponent>, TypeImplementation> Implementation;
   reinterpret_cast<Implementation*>(content)->acosAssign();
}

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
void
TFloatExact<USizeMantissa, USizeExponent, TypeImplementation>::tanAssign() {
   typedef DDoubleExact::TFloatExact<DDoubleExact::ExecutionPath, DDoubleExact::TBuiltFloatExact<USizeMantissa, USizeExponent>, TypeImplementation> Implementation;
   reinterpret_cast<Implementation*>(content)->tanAssign();
}

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
void
TFloatExact<USizeMantissa, USizeExponent, TypeImplementation>::atanAssign() {
   typedef DDoubleExact::TFloatExact<DDoubleExact::ExecutionPath, DDoubleExact::TBuiltFloatExact<USizeMantissa, USizeExponent>, TypeImplementation> Implementation;
   reinterpret_cast<Implementation*>(content)->atanAssign();
}

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
void
TFloatExact<USizeMantissa, USizeExponent, TypeImplementation>::expAssign() {
   typedef DDoubleExact::TFloatExact<DDoubleExact::ExecutionPath, DDoubleExact::TBuiltFloatExact<USizeMantissa, USizeExponent>, TypeImplementation> Implementation;
   reinterpret_cast<Implementation*>(content)->expAssign();
}

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
void
TFloatExact<USizeMantissa, USizeExponent, TypeImplementation>::logAssign() {
   typedef DDoubleExact::TFloatExact<DDoubleExact::ExecutionPath, DDoubleExact::TBuiltFloatExact<USizeMantissa, USizeExponent>, TypeImplementation> Implementation;
   reinterpret_cast<Implementation*>(content)->logAssign();
}

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
void
TFloatExact<USizeMantissa, USizeExponent, TypeImplementation>::log10Assign() {
   typedef DDoubleExact::TFloatExact<DDoubleExact::ExecutionPath, DDoubleExact::TBuiltFloatExact<USizeMantissa, USizeExponent>, TypeImplementation> Implementation;
   reinterpret_cast<Implementation*>(content)->log10Assign();
}

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
void
TFloatExact<USizeMantissa, USizeExponent, TypeImplementation>::powAssign(const thisType& value) {
   typedef DDoubleExact::TFloatExact<DDoubleExact::ExecutionPath, DDoubleExact::TBuiltFloatExact<USizeMantissa, USizeExponent>, TypeImplementation> Implementation;
   reinterpret_cast<Implementation*>(content)->powAssign(
         *reinterpret_cast<const Implementation*>(value.content));
}

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
void
TFloatExact<USizeMantissa, USizeExponent, TypeImplementation>::absAssign() {
   typedef DDoubleExact::TFloatExact<DDoubleExact::ExecutionPath, DDoubleExact::TBuiltFloatExact<USizeMantissa, USizeExponent>, TypeImplementation> Implementation;
   reinterpret_cast<Implementation*>(content)->absAssign();
}

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
void
TFloatExact<USizeMantissa, USizeExponent, TypeImplementation>::minAssign(const thisType& value) {
   typedef DDoubleExact::TFloatExact<DDoubleExact::ExecutionPath, DDoubleExact::TBuiltFloatExact<USizeMantissa, USizeExponent>, TypeImplementation> Implementation;
   reinterpret_cast<Implementation*>(content)->minAssign(*reinterpret_cast<const Implementation*>(content));
}

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
void
TFloatExact<USizeMantissa, USizeExponent, TypeImplementation>::maxAssign(const thisType& value) {
   typedef DDoubleExact::TFloatExact<DDoubleExact::ExecutionPath, DDoubleExact::TBuiltFloatExact<USizeMantissa, USizeExponent>, TypeImplementation> Implementation;
   reinterpret_cast<Implementation*>(content)->maxAssign(*reinterpret_cast<const Implementation*>(content));
}

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
void
TFloatExact<USizeMantissa, USizeExponent, TypeImplementation>::medianAssign(const thisType& fst, const thisType& snd) {
   typedef DDoubleExact::TFloatExact<DDoubleExact::ExecutionPath, DDoubleExact::TBuiltFloatExact<USizeMantissa, USizeExponent>, TypeImplementation> Implementation;
   reinterpret_cast<Implementation*>(content)->medianAssign(*reinterpret_cast<const Implementation*>(fst.content), *reinterpret_cast<const Implementation*>(snd.content));
}

#if defined __GNUC__ && GCC_VERSION >= 50000
#pragma GCC diagnostic pop
#endif

template class TFloatExact<23, 8, float>;
template class TFloatExact<52, 11, double>;

#ifndef LDBL_EXPONENT_DIG
#define LDBL_EXPONENT_DIG \
      ((LDBL_MAX_EXP == (1 << (16-2))) ? 15 : sizeof(long double)*8-LDBL_MANT_DIG)
#endif

template class TFloatExact<LDBL_MANT_DIG-1, LDBL_EXPONENT_DIG, long double>;

template TFloatExact<23, 8, float>::TFloatExact(const TFloatExact<52, 11, double>&);
template TFloatExact<23, 8, float>::TFloatExact(const TFloatExact<LDBL_MANT_DIG-1, LDBL_EXPONENT_DIG, long double>&);
template TFloatExact<52, 11, double>::TFloatExact(const TFloatExact<23, 8, float>&);
template TFloatExact<52, 11, double>::TFloatExact(const TFloatExact<LDBL_MANT_DIG-1, LDBL_EXPONENT_DIG, long double>&);
template TFloatExact<LDBL_MANT_DIG-1, LDBL_EXPONENT_DIG, long double>::TFloatExact(const TFloatExact<23, 8, float>&);
template TFloatExact<LDBL_MANT_DIG-1, LDBL_EXPONENT_DIG, long double>::TFloatExact(const TFloatExact<52, 11, double>&);

template TFloatExact<23, 8, float>& TFloatExact<23, 8, float>::operator=(const TFloatExact<52, 11, double>&);
template TFloatExact<23, 8, float>& TFloatExact<23, 8, float>::operator=(const TFloatExact<LDBL_MANT_DIG-1, LDBL_EXPONENT_DIG, long double>&);
template TFloatExact<52, 11, double>& TFloatExact<52, 11, double>::operator=(const TFloatExact<23, 8, float>&);
template TFloatExact<52, 11, double>& TFloatExact<52, 11, double>::operator=(const TFloatExact<LDBL_MANT_DIG-1, LDBL_EXPONENT_DIG, long double>&);
template TFloatExact<LDBL_MANT_DIG-1, LDBL_EXPONENT_DIG, long double>& TFloatExact<LDBL_MANT_DIG-1, LDBL_EXPONENT_DIG, long double>::operator=(const TFloatExact<23, 8, float>&);
template TFloatExact<LDBL_MANT_DIG-1, LDBL_EXPONENT_DIG, long double>& TFloatExact<LDBL_MANT_DIG-1, LDBL_EXPONENT_DIG, long double>::operator=(const TFloatExact<52, 11, double>&);

template MergeBranches& MergeBranches::operator<<(TFloatExact<23, 8, float>&);
template MergeBranches& MergeBranches::operator<<(TFloatExact<52, 11, double>&);
template MergeBranches& MergeBranches::operator<<(TFloatExact<LDBL_MANT_DIG-1, LDBL_EXPONENT_DIG, long double>&);

}} // end of namespace NumericalDomains::DDoubleExactInterface

