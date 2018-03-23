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
// File      : FloatAffine.h
// Description :
//   Definition of a class of affine relations.
//

#ifndef FloatInstrumentation_FloatAffineH
#define FloatInstrumentation_FloatAffineH

#include "NumericalAnalysis/FloatAffineExecutionPath.h"

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

#include <math.h>
#include <fstream>
#include <sstream>

#include "FloatInstrumentation/FloatAffine.inch"

namespace NumericalDomains { namespace DAffine {

typedef STG::IOObject::OSBase DiagnosisReadStream;

template <typename T1, class TypeSaveMemory>
class TSaveMemory {
  public:
   T1 save;
   TypeSaveMemory next;

   TSaveMemory(T1& saveArg, TypeSaveMemory nextArg)
      :  save(saveArg), next(nextArg)
      {  if (saveArg.doesSupportUnstableInLoop()) {
            saveArg.getSRealDomain().setHolder(saveArg.currentPathExplorer);
            saveArg.getSError().setHolder(saveArg.currentPathExplorer);
            save.getSRealDomain().clearHolder();
            save.getSError().clearHolder();
         };
      }
   TSaveMemory(const TSaveMemory<T1, TypeSaveMemory>& source)
      :  save(source.save), next(source.next)
      {  if (save.doesSupportUnstableInLoop()) {
            save.getSRealDomain().clearHolder();
            save.getSError().clearHolder();
         };
      }
   TSaveMemory(TSaveMemory<T1, TypeSaveMemory>&& source)
      :  save(std::move(source.save)), next(std::move(source.next))
      {  if (save.doesSupportUnstableInLoop()) {
            save.getSRealDomain().clearHolder();
            save.getSError().clearHolder();
         };
      }

   template <typename T>
   TSaveMemory<T, TSaveMemory<T1, TypeSaveMemory> > operator<<(T& t)
      {  return TSaveMemory<T, TSaveMemory<T1, TypeSaveMemory> >(t, *this); }
   template <typename T>
   TSaveMemory<T, TSaveMemory<T1, TypeSaveMemory> > operator<<(const T& t)
      {  return TSaveMemory<T, TSaveMemory<T1, TypeSaveMemory> >(const_cast<T&>(t), *this); }
   TSaveMemory<T1, TypeSaveMemory>& operator<<(BaseExecutionPath::end) { return *this; }
   TSaveMemory<T1, TypeSaveMemory>& setCurrentResult(bool result)
      {  next.setCurrentResult(result); return *this; }
   TypeSaveMemory& operator>>(T1& val)
      {  if (!next.getResult())
            val = save;
         return next;
      }
   // to remove for the emission of compiler warnings
   TypeSaveMemory& operator>>(const T1& aval)
      {  T1& val = const_cast<T1&>(aval);
         if (!next.getResult())
            val = save;
         return next;
      }
   bool getResult() const { return next.getResult(); }
};

class SaveMemory {
  private:
   bool fResult;

  public:
   SaveMemory() : fResult(false) {}

   template <typename T>
   TSaveMemory<T, SaveMemory> operator<<(T& t)
      {  return TSaveMemory<T, SaveMemory>(t, *this); }
   template <typename T>
   TSaveMemory<T, SaveMemory> operator<<(const T& t)
      {  return TSaveMemory<T, SaveMemory>(const_cast<T&>(t), *this); }
   SaveMemory& operator<<(BaseExecutionPath::end) { return *this; }
   SaveMemory& setCurrentResult(bool result) { fResult = result; return *this; }
   bool getResult() const { return fResult; }
   bool operator>>(BaseExecutionPath::end)
      {  bool result = fResult;
         if (fResult)
            fResult = false;
         return result;
      }
};

template <typename T1, class TypeMergeMemory>
class TMergeMemory {
  public:
   T1 merge;
   TypeMergeMemory next;

   TMergeMemory(const T1&, TypeMergeMemory nextArg)
      :  merge(), next(nextArg) {}
   TMergeMemory(const TMergeMemory<T1, TypeMergeMemory>& source) = default;
   TMergeMemory(TMergeMemory<T1, TypeMergeMemory>&& source) = default;

   template <typename T>
   TMergeMemory<T, TMergeMemory<T1, TypeMergeMemory> > operator>>(T& t)
      {  return TMergeMemory<T, TMergeMemory<T1, TypeMergeMemory> >(t, *this); }
   template <typename T>
   TMergeMemory<T, TMergeMemory<T1, TypeMergeMemory> > operator>>(const T& t)
      {  return TMergeMemory<T, TMergeMemory<T1, TypeMergeMemory> >(const_cast<T&>(t), *this); }
   TMergeMemory<T1, TypeMergeMemory>& operator>>(BaseExecutionPath::end) { return *this; }
   TMergeMemory<T1, TypeMergeMemory>& setCurrentComplete(bool isComplete)
      {  next.setCurrentComplete(isComplete); return *this; }
   TypeMergeMemory& operator<<(T1& val)
      {  if (next.isComplete()) {
            if (val.optimizeValue()) {
               if (next.isFirst()) {
                  merge = val;
                  if (merge.doesSupportUnstableInLoop()) {
                     merge.getSRealDomain().clearHolder();
                     merge.getSError().clearHolder();
                  };
               }
               else
                  merge.mergeWith(val);
            }
            else
               next.setCurrentComplete(false);
         };
         val = merge;
         return next;
      }
   // to remove for the emission of compiler warnings
   TypeMergeMemory& operator<<(const T1& aval)
      {  T1& val = const_cast<T1&>(aval);
         if (next.isComplete()) {
            if (val.optimizeValue()) {
               if (next.isFirst()) {
                  merge = val;
                  if (merge.doesSupportUnstableInLoop()) {
                     merge.getSRealDomain().clearHolder();
                     merge.getSError().clearHolder();
                  };
               }
               else
                  merge.mergeWith(val);
            }
            else
               next.setCurrentComplete(false);
         };
         val = merge;
         return next;
      }
   bool isFirst() const { return next.isFirst(); }
   bool isComplete() const { return next.isComplete(); }
};

class MergeMemory {
  private:
   bool fFirst;
   bool fComplete;

  public:
   MergeMemory() : fFirst(true), fComplete(false) {}

   template <typename T>
   TMergeMemory<T, MergeMemory> operator>>(T& t)
      {  return TMergeMemory<T, MergeMemory>(t, *this); }
   template <typename T>
   TMergeMemory<T, MergeMemory> operator>>(const T& t)
      {  return TMergeMemory<T, MergeMemory>(const_cast<T&>(t), *this); }
   MergeMemory& operator>>(BaseExecutionPath::end) { return *this; }
   MergeMemory& setCurrentComplete(bool isComplete)
      {  fComplete = isComplete; return *this; }
   bool isFirst() const { return fFirst; }
   bool isComplete() const { return fComplete; }
   bool operator<<(BaseExecutionPath::end)
      {  if (fComplete)
            fFirst = false;
         return true;
      }
};

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
class TInstrumentedFloatZonotope : public TFloatZonotope<ExecutionPath, USizeMantissa, USizeExponent, TypeImplementation> {
  private:
   typedef TFloatZonotope<ExecutionPath, USizeMantissa, USizeExponent, TypeImplementation> inherited;
   typedef TInstrumentedFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation> thisType;

  protected:
   friend class TMergeBranches<ExecutionPath>;

  public:
   TInstrumentedFloatZonotope() = default;
   TInstrumentedFloatZonotope(float value)
      {  if (!inherited::fSupportAtomic)
            inherited::initFrom(value);
         else
            inherited::initFromAtomic(value);
      }
   TInstrumentedFloatZonotope(double value)
      {  if (!inherited::fSupportAtomic)
            inherited::initFrom(value);
         else
            inherited::initFromAtomic(value);
      }
   TInstrumentedFloatZonotope(long double value);
   TInstrumentedFloatZonotope(TypeImplementation min, TypeImplementation max)
      :  inherited(min, max)
      {  if (inherited::doesSupportUnstableInLoop())
            inherited::getSRealDomain().setHolder(inherited::currentPathExplorer);
      }
   TInstrumentedFloatZonotope(TypeImplementation min, TypeImplementation max,
         TypeImplementation errmin, TypeImplementation errmax)
      :  inherited(min, max)
      {  inherited::setError(errmin, errmax);
         if (inherited::doesSupportUnstableInLoop()) {
            inherited::getSRealDomain().setHolder(inherited::currentPathExplorer);
            inherited::getSError().setHolder(inherited::currentPathExplorer);
         };
      }
   TInstrumentedFloatZonotope(int value) : inherited(value) {}
   TInstrumentedFloatZonotope(long int value) : inherited(value) {}
   // TInstrumentedFloatZonotope(unsigned value) : inherited(value) {}
   // TInstrumentedFloatZonotope(unsigned long value) : inherited(value) {}
   TInstrumentedFloatZonotope(const thisType& source) : inherited(source)
      {  if (inherited::doesSupportUnstableInLoop()) {
            inherited::getSRealDomain().setHolder(inherited::currentPathExplorer);
            inherited::getSError().setHolder(inherited::currentPathExplorer);
         };
      }
   TInstrumentedFloatZonotope(thisType&& source) : inherited(std::forward<inherited>(source)) // [TODO] keep symbolic for constraints
      {  if (inherited::doesSupportUnstableInLoop()) {
            inherited::getSRealDomain().setHolder(inherited::currentPathExplorer);
            inherited::getSError().setHolder(inherited::currentPathExplorer);
         };
      }
   TInstrumentedFloatZonotope& operator=(const thisType& source)
      {  inherited::operator=(source);
         if (inherited::doesSupportUnstableInLoop()) {
            inherited::getSRealDomain().setHolder(inherited::currentPathExplorer);
            inherited::getSError().setHolder(inherited::currentPathExplorer);
         };
         return *this;
      }
   TInstrumentedFloatZonotope& operator=(thisType&& source) // [TODO] keep symbolic for constraints
      {  inherited::operator=(source);
         if (inherited::doesSupportUnstableInLoop()) {
            inherited::getSRealDomain().setHolder(inherited::currentPathExplorer);
            inherited::getSError().setHolder(inherited::currentPathExplorer);
         };
         return *this;
      }

   template <int USizeMantissaArgument, int USizeExponentArgument, typename TypeImplementationArgument>
   TInstrumentedFloatZonotope(const TInstrumentedFloatZonotope<USizeMantissaArgument, USizeExponentArgument, TypeImplementationArgument>& source)
      :  inherited(source)
      {  if (inherited::doesSupportUnstableInLoop()) {
            inherited::getSRealDomain().setHolder(inherited::currentPathExplorer);
            inherited::getSError().setHolder(inherited::currentPathExplorer);
         };
      }
   template <int USizeMantissaArgument, int USizeExponentArgument, typename TypeImplementationArgument>
   TInstrumentedFloatZonotope(TInstrumentedFloatZonotope<USizeMantissaArgument, USizeExponentArgument, TypeImplementationArgument>&& source)
      :  inherited(source)
      {  if (inherited::doesSupportUnstableInLoop()) {
            inherited::getSRealDomain().setHolder(inherited::currentPathExplorer);
            inherited::getSError().setHolder(inherited::currentPathExplorer);
         };
      }
   template <int USizeMantissaArgument, int USizeExponentArgument, typename TypeImplementationArgument>
   TInstrumentedFloatZonotope& operator=(const TInstrumentedFloatZonotope<USizeMantissaArgument, USizeExponentArgument, TypeImplementationArgument>& source)
      {  inherited::operator=(source);
         if (inherited::doesSupportUnstableInLoop()) {
            inherited::getSRealDomain().setHolder(inherited::currentPathExplorer);
            inherited::getSError().setHolder(inherited::currentPathExplorer);
         };
         return *this;
      }
   template <int USizeMantissaArgument, int USizeExponentArgument, typename TypeImplementationArgument>
   TInstrumentedFloatZonotope& operator=(TInstrumentedFloatZonotope<USizeMantissaArgument, USizeExponentArgument, TypeImplementationArgument>&& source)
      {  inherited::operator=(std::forward<TInstrumentedFloatZonotope<USizeMantissaArgument, USizeExponentArgument, TypeImplementationArgument> >(source));
         if (inherited::doesSupportUnstableInLoop()) {
            inherited::getSRealDomain().setHolder(inherited::currentPathExplorer);
            inherited::getSError().setHolder(inherited::currentPathExplorer);
         };
         return *this;
      }

   bool operator<(const thisType& source) const { return inherited::operator<(source); }
   bool operator<(TypeImplementation source) const { return inherited::operator<(thisType(source)); }
   bool operator<=(const thisType& source) const { return inherited::operator<=(source); }
   bool operator<=(TypeImplementation source) const { return inherited::operator<=(thisType(source)); }
   bool operator==(const thisType& source) const { return inherited::operator==(source); }
   bool operator==(TypeImplementation source) const { return inherited::operator==(thisType(source)); }
   bool operator!=(const thisType& source) const { return inherited::operator!=(source); }
   bool operator!=(TypeImplementation source) const { return inherited::operator!=(thisType(source)); }
   bool operator>=(const thisType& source) const { return inherited::operator>=(source); }
   bool operator>=(TypeImplementation source) const { return inherited::operator>=(thisType(source)); }
   bool operator>(const thisType& source) const { return inherited::operator>(source); }
   bool operator>(TypeImplementation source) const { return inherited::operator>(thisType(source)); }

   thisType& operator+=(const thisType& source)
      {  this->plusAssign(source, Equation::PCSourceRValue);
         if (inherited::doesSupportUnstableInLoop()) {
            inherited::getSRealDomain().setHolder(inherited::currentPathExplorer);
            inherited::getSError().setHolder(inherited::currentPathExplorer);
         };
         return *this;
      }
   thisType& operator+=(thisType&& source)
      {  this->plusAssign(source, Equation::PCSourceXValue);
         if (inherited::doesSupportUnstableInLoop()) {
            inherited::getSRealDomain().setHolder(inherited::currentPathExplorer);
            inherited::getSError().setHolder(inherited::currentPathExplorer);
         };
         return *this;
      }
   thisType& operator-=(const thisType& source)
      {  this->minusAssign(source, Equation::PCSourceRValue);
         if (inherited::doesSupportUnstableInLoop()) {
            inherited::getSRealDomain().setHolder(inherited::currentPathExplorer);
            inherited::getSError().setHolder(inherited::currentPathExplorer);
         };
         return *this;
      }
   thisType& operator-=(thisType&& source)
      {  this->minusAssign(source, Equation::PCSourceXValue);
         if (inherited::doesSupportUnstableInLoop()) {
            inherited::getSRealDomain().setHolder(inherited::currentPathExplorer);
            inherited::getSError().setHolder(inherited::currentPathExplorer);
         };
         return *this;
      }
   thisType& operator*=(const thisType& source)
      {  this->multAssign(source, Equation::PCSourceRValue);
         if (inherited::doesSupportUnstableInLoop()) {
            inherited::getSRealDomain().setHolder(inherited::currentPathExplorer);
            inherited::getSError().setHolder(inherited::currentPathExplorer);
         };
         return *this;
      }
   thisType& operator*=(thisType&& source)
      {  this->multAssign(source, Equation::PCSourceXValue);
         if (inherited::doesSupportUnstableInLoop()) {
            inherited::getSRealDomain().setHolder(inherited::currentPathExplorer);
            inherited::getSError().setHolder(inherited::currentPathExplorer);
         };
         return *this;
      }
   thisType& operator/=(const thisType& source)
      {  this->divAssign(source, Equation::PCSourceRValue);
         if (inherited::doesSupportUnstableInLoop()) {
            inherited::getSRealDomain().setHolder(inherited::currentPathExplorer);
            inherited::getSError().setHolder(inherited::currentPathExplorer);
         };
         return *this;
      }
   thisType& operator/=(thisType&& source)
      {  this->divAssign(source, Equation::PCSourceXValue);
         if (inherited::doesSupportUnstableInLoop()) {
            inherited::getSRealDomain().setHolder(inherited::currentPathExplorer);
            inherited::getSError().setHolder(inherited::currentPathExplorer);
         };
         return *this;
      }

   friend thisType operator-(const thisType& first)
      {  thisType result(first); result.oppositeAssign(); return result; }
   friend thisType operator-(thisType&& first)
      {  thisType result(std::forward<thisType>(first)); result.oppositeAssign(); return result; }
   friend thisType operator+(const thisType& first) { return first; }
   friend thisType operator+(thisType&& first) { return first; }
   friend thisType operator+(const thisType& first, const thisType& second)
      {  thisType result(first);
         result.plusAssign(second, Equation::PCSourceRValue);
         return result;
      }
   friend thisType operator+(thisType&& first, const thisType& second)
      {  thisType result(std::forward<thisType>(first));
         result.plusAssign(second, Equation::PCSourceRValue);
         return result;
      }
   friend thisType operator+(const thisType& first, thisType&& second)
      {  thisType result(first);
         result.plusAssign(second, Equation::PCSourceXValue);
         return result;
      }
   friend thisType operator+(thisType&& first, thisType&& second)
      {  thisType result(std::forward<thisType>(first));
         result.plusAssign(second, Equation::PCSourceXValue);
         return result;
      }
   friend thisType operator+(const thisType&& first, TypeImplementation second)
      {  thisType result(first);
         result.plusAssign(thisType(second), Equation::PCSourceXValue);
         return result;
      }
   friend thisType operator+(thisType&& first, TypeImplementation second)
      {  thisType result(std::forward<thisType>(first));
         result.plusAssign(thisType(second), Equation::PCSourceXValue);
         return result;
      }
   friend thisType operator-(const thisType& first, const thisType& second)
      {  thisType result(first);
         result.minusAssign(second, Equation::PCSourceRValue);
         return result;
      }
   friend thisType operator-(thisType&& first, const thisType& second)
      {  thisType result(std::forward<thisType>(first));
         result.minusAssign(second, Equation::PCSourceRValue);
         return result;
      }
   friend thisType operator-(const thisType& first, thisType&& second)
      {  thisType result(first);
         result.minusAssign(second, Equation::PCSourceXValue);
         return result;
      }
   friend thisType operator-(thisType&& first, thisType&& second)
      {  thisType result(std::forward<thisType>(first));
         result.minusAssign(second, Equation::PCSourceXValue);
         return result;
      }
   friend thisType operator-(const thisType&& first, TypeImplementation second)
      {  thisType result(first);
         result.minusAssign(thisType(second), Equation::PCSourceXValue);
         return result;
      }
   friend thisType operator-(thisType&& first, TypeImplementation second)
      {  thisType result(std::forward<thisType>(first));
         result.minusAssign(thisType(second), Equation::PCSourceXValue);
         return result;
      }
   friend thisType operator*(const thisType& first, const thisType& second)
      {  thisType result(first);
         result.multAssign(second, Equation::PCSourceRValue);
         return result;
      }
   friend thisType operator*(thisType&& first, const thisType& second)
      {  thisType result(std::forward<thisType>(first));
         result.multAssign(second, Equation::PCSourceRValue);
         return result;
      }
   friend thisType operator*(const thisType& first, thisType&& second)
      {  thisType result(first);
         result.multAssign(second, Equation::PCSourceXValue);
         return result;
      }
   friend thisType operator*(thisType&& first, thisType&& second)
      {  thisType result(std::forward<thisType>(first));
         result.multAssign(second, Equation::PCSourceXValue);
         return result;
      }
   friend thisType operator*(const thisType&& first, TypeImplementation second)
      {  thisType result(first);
         result.multAssign(thisType(second), Equation::PCSourceXValue);
         return result;
      }
   friend thisType operator*(thisType&& first, TypeImplementation second)
      {  thisType result(std::forward<thisType>(first));
         result.multAssign(thisType(second), Equation::PCSourceXValue);
         return result;
      }
   friend thisType operator/(const thisType& first, const thisType& second)
      {  thisType result(first);
         result.divAssign(second, Equation::PCSourceRValue);
         return result;
      }
   friend thisType operator/(thisType&& first, const thisType& second)
      {  thisType result(std::forward<thisType>(first));
         result.divAssign(second, Equation::PCSourceRValue);
         return result;
      }
   friend thisType operator/(const thisType& first, thisType&& second)
      {  thisType result(first);
         result.divAssign(second, Equation::PCSourceXValue);
         return result;
      }
   friend thisType operator/(thisType&& first, thisType&& second)
      {  thisType result(std::forward<thisType>(first));
         result.divAssign(second, Equation::PCSourceXValue);
         return result;
      }
   friend thisType operator/(const thisType&& first, TypeImplementation second)
      {  thisType result(first);
         result.divAssign(thisType(second), Equation::PCSourceXValue);
         return result;
      }
   friend thisType operator/(thisType&& first, TypeImplementation second)
      {  thisType result(std::forward<thisType>(first));
         result.divAssign(thisType(second), Equation::PCSourceXValue);
         return result;
      }

   explicit operator TypeImplementation() const { return inherited::asImplementation(); }
   explicit operator int() const
      {  return inherited::asInt(inherited::ReadParametersBase::RMZero); }
   explicit operator short int() const
      {  return inherited::asInt(inherited::ReadParametersBase::RMZero); }
   explicit operator unsigned() const
      {  return inherited::asUnsigned(inherited::ReadParametersBase::RMZero); }
   explicit operator short unsigned() const
      {  return inherited::asUnsigned(inherited::ReadParametersBase::RMZero); }
   explicit operator long int() const
      {  return inherited::asLong(inherited::ReadParametersBase::RMZero); }
   explicit operator unsigned long() const
      {  return inherited::asUnsignedLong(inherited::ReadParametersBase::RMZero); }

   thisType abs() const
      {  thisType result = *this;
         // see float_diagnosis.h FLOAT_SPLIT_ALL FLOAT_MERGE_ALL
         bool oldSupportUnstableInLoop = inherited::doesSupportUnstableInLoop();
         inherited::setSupportUnstableInLoop();
         auto* oldPathExplorer = ExecutionPath::getCurrentPathExplorer();
         bool oldDoesFollow = ExecutionPath::doesFollowFlow();
         ExecutionPath::clearFollowFlow();
         auto* oldInputTraceFile = ExecutionPath::inputTraceFile();
         const char* oldSynchronisationFile = ExecutionPath::synchronisationFile();
         int oldSynchronisationLine = ExecutionPath::synchronisationLine();
         bool isCompleteFlow = true;
         PathExplorer pathExplorer(ExecutionPath::queryMode(oldPathExplorer));
         ExecutionPath::setCurrentPathExplorer(&pathExplorer);
         auto mergeMemory = MergeMemory() >> result >> BaseExecutionPath::end();
         auto saveMemory = SaveMemory() << result << BaseExecutionPath::end();
         const char* sourceFile = __FILE__;
         int sourceLine = __LINE__;
         do {
            try {
               BaseFloatAffine::splitBranches(sourceFile, sourceLine);

               if (result < 0)
                  result.oppositeAssign();

               isCompleteFlow = MergeBranches(sourceFile, sourceLine) << result << BaseExecutionPath::end();
            } 
            catch (typename thisType::anticipated_termination&) {
               isCompleteFlow = false;
               ExecutionPath::clearSynchronizationBranches();
            }
            catch (STG::EReadError& error) {
               if (const char* message = error.getMessage())
                  std::cerr << "error: " << message << std::endl;
               else
                  std::cerr << "error while reading input file!" << std::endl;
               isCompleteFlow = false;
               ExecutionPath::clearSynchronizationBranches();
            }
            ExecutionPath::setFollowFlow();
         } while ((mergeMemory.setCurrentComplete(isCompleteFlow) << result << BaseExecutionPath::end())
               && !(saveMemory.setCurrentResult(pathExplorer.isFinished(ExecutionPath::queryMode(oldPathExplorer))) >> result >> BaseExecutionPath::end()));
         ExecutionPath::setFollowFlow(oldDoesFollow, oldInputTraceFile,
               oldSynchronisationFile, oldSynchronisationLine);
         ExecutionPath::setCurrentPathExplorer(oldPathExplorer);
         inherited::setSupportUnstableInLoop(oldSupportUnstableInLoop);
         if (mergeMemory.isFirst())
            ExecutionPath::throwEmptyBranch(true);
         return result;
      }
   thisType min(const thisType& asource) const
      {  thisType result = *this, source = asource;
         // see float_diagnosis.h FLOAT_SPLIT_ALL FLOAT_MERGE_ALL
         bool oldSupportUnstableInLoop = inherited::doesSupportUnstableInLoop();
         inherited::setSupportUnstableInLoop();
         auto* oldPathExplorer = ExecutionPath::getCurrentPathExplorer();
         bool oldDoesFollow = ExecutionPath::doesFollowFlow();
         ExecutionPath::clearFollowFlow();
         auto* oldInputTraceFile = ExecutionPath::inputTraceFile();
         const char* oldSynchronisationFile = ExecutionPath::synchronisationFile();
         int oldSynchronisationLine = ExecutionPath::synchronisationLine();
         bool isCompleteFlow = true;
         PathExplorer pathExplorer(ExecutionPath::queryMode(oldPathExplorer));
         ExecutionPath::setCurrentPathExplorer(&pathExplorer);
         auto mergeMemory = MergeMemory() >> result >> BaseExecutionPath::end();
         auto saveMemory = SaveMemory() << result << source << BaseExecutionPath::end();
         const char* sourceFile = __FILE__;
         int sourceLine = __LINE__;
         do {
            try {
               BaseFloatAffine::splitBranches(sourceFile, sourceLine);

               if (source < result)
                  result = source;

               isCompleteFlow = MergeBranches(sourceFile, sourceLine) << result << BaseExecutionPath::end();
            } 
            catch (typename thisType::anticipated_termination&) {
               isCompleteFlow = false;
               ExecutionPath::clearSynchronizationBranches();
            }
            catch (STG::EReadError& error) {
               if (const char* message = error.getMessage())
                  std::cerr << "error: " << message << std::endl;
               else
                  std::cerr << "error while reading input file!" << std::endl;
               isCompleteFlow = false;
               ExecutionPath::clearSynchronizationBranches();
            }
            ExecutionPath::setFollowFlow();
         } while ((mergeMemory.setCurrentComplete(isCompleteFlow) << result << BaseExecutionPath::end())
               && !(saveMemory.setCurrentResult(pathExplorer.isFinished(ExecutionPath::queryMode(oldPathExplorer))) >> source >> result >> BaseExecutionPath::end()));
         ExecutionPath::setFollowFlow(oldDoesFollow, oldInputTraceFile,
               oldSynchronisationFile, oldSynchronisationLine);
         ExecutionPath::setCurrentPathExplorer(oldPathExplorer);
         inherited::setSupportUnstableInLoop(oldSupportUnstableInLoop);
         if (mergeMemory.isFirst())
            ExecutionPath::throwEmptyBranch(true);
         return result;
      }
   thisType min(TypeImplementation source) const
      {  thisType affineSource(source);
         return min(affineSource);
      }
   thisType max(const thisType& asource) const
      {  thisType result = *this, source = asource;
         // see float_diagnosis.h FLOAT_SPLIT_ALL FLOAT_MERGE_ALL
         bool oldSupportUnstableInLoop = inherited::doesSupportUnstableInLoop();
         inherited::setSupportUnstableInLoop();
         auto* oldPathExplorer = ExecutionPath::getCurrentPathExplorer();
         bool oldDoesFollow = ExecutionPath::doesFollowFlow();
         ExecutionPath::clearFollowFlow();
         auto* oldInputTraceFile = ExecutionPath::inputTraceFile();
         const char* oldSynchronisationFile = ExecutionPath::synchronisationFile();
         int oldSynchronisationLine = ExecutionPath::synchronisationLine();
         bool isCompleteFlow = true;
         PathExplorer pathExplorer(ExecutionPath::queryMode(oldPathExplorer));
         ExecutionPath::setCurrentPathExplorer(&pathExplorer);
         auto mergeMemory = MergeMemory() >> result >> BaseExecutionPath::end();
         auto saveMemory = SaveMemory() << result << source << BaseExecutionPath::end();
         const char* sourceFile = __FILE__;
         int sourceLine = __LINE__;
         do {
            try {
               BaseFloatAffine::splitBranches(sourceFile, sourceLine);

               if (source > result)
                  result = source;

               isCompleteFlow = MergeBranches(sourceFile, sourceLine) << result << BaseExecutionPath::end();
            } 
            catch (typename thisType::anticipated_termination&) {
               isCompleteFlow = false;
               ExecutionPath::clearSynchronizationBranches();
            }
            catch (STG::EReadError& error) {
               if (const char* message = error.getMessage())
                  std::cerr << "error: " << message << std::endl;
               else
                  std::cerr << "error while reading input file!" << std::endl;
               isCompleteFlow = false;
               ExecutionPath::clearSynchronizationBranches();
            }
            ExecutionPath::setFollowFlow();
         } while ((mergeMemory.setCurrentComplete(isCompleteFlow) << result << BaseExecutionPath::end())
               && !(saveMemory.setCurrentResult(pathExplorer.isFinished(ExecutionPath::queryMode(oldPathExplorer))) >> source >> result >> BaseExecutionPath::end()));
         ExecutionPath::setFollowFlow(oldDoesFollow, oldInputTraceFile,
               oldSynchronisationFile, oldSynchronisationLine);
         ExecutionPath::setCurrentPathExplorer(oldPathExplorer);
         inherited::setSupportUnstableInLoop(oldSupportUnstableInLoop);
         if (mergeMemory.isFirst())
            ExecutionPath::throwEmptyBranch(true);
         return result;
      }
   thisType max(TypeImplementation source) const
      {  thisType affineSource(source);
         return max(affineSource);
      }
   thisType median(const thisType& fst, const thisType& snd) const
      {  if (*this <= fst) {
            if (fst <= snd)
               return fst;
            // snd < fst 
            if (*this <= snd)
               return snd;
            // snd < *this <= fst
            return *this;
         }
         // fst < *this
         if (snd <= fst)
            return fst;
         // fst < snd
         if (*this <= snd)
            return *this;
         return snd;
      }
   thisType median(TypeImplementation fstValue, const thisType& snd) const
      {  thisType fst(fstValue);
         if (*this <= fst) {
            if (fst <= snd)
               return fst;
            // snd < fst 
            if (*this <= snd)
               return snd;
            // snd < *this <= fst
            return *this;
         }
         // fst < *this
         if (snd <= fst)
            return fst;
         // fst < snd
         if (*this <= snd)
            return *this;
         return snd;
      }
   thisType median(const thisType& fst, TypeImplementation sndValue) const
      {  thisType snd(sndValue);
         if (*this <= fst) {
            if (fst <= snd)
               return fst;
            // snd < fst 
            if (*this <= snd)
               return snd;
            // snd < *this <= fst
            return *this;
         }
         // fst < *this
         if (snd <= fst)
            return fst;
         // fst < snd
         if (*this <= snd)
            return *this;
         return snd;
      }
   thisType median(TypeImplementation fstValue, TypeImplementation sndValue) const
      {  thisType fst(fstValue), snd(sndValue);
         if (*this <= fst) {
            if (fst <= snd)
               return fst;
            // snd < fst 
            if (*this <= snd)
               return snd;
            // snd < *this <= fst
            return *this;
         }
         // fst < *this
         if (snd <= fst)
            return fst;
         // fst < snd
         if (*this <= snd)
            return *this;
         return snd;
      }

   void lightPersist(const char* prefix) const { inherited::lightPersist(*this, prefix); }
   void persist(const char* prefix) const { inherited::persist(*this, prefix); }
};

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
inline
TInstrumentedFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>
      ::TInstrumentedFloatZonotope(long double value)
{  typedef DAffine::FloatDigitsHelper::TFloatDigits<long double> FloatDigits;
   TFloatZonotope<ExecutionPath, FloatDigits::UBitSizeMantissa,
         FloatDigits::UBitSizeExponent, long double> receiver;
   if (!inherited::fSupportAtomic)
      receiver.initFrom(value);
   else
      receiver.initFromAtomic(value);
   inherited::operator=(std::move(receiver));
}

} // end of namespace DAffine

typedef DAffine::FloatDigitsHelper::TFloatDigits<long double> LongDoubleFloatDigits;

typedef DAffine::TInstrumentedFloatZonotope<23, 8, float> FloatZonotope;
typedef DAffine::TInstrumentedFloatZonotope<52, 11, double> DoubleZonotope;
typedef DAffine::TInstrumentedFloatZonotope<LongDoubleFloatDigits::UBitSizeMantissa,
        LongDoubleFloatDigits::UBitSizeExponent, long double> LongDoubleZonotope;

} // end of namespace NumericalDomains

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
inline NumericalDomains::DAffine::TInstrumentedFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>
sqrt(const NumericalDomains::DAffine::TInstrumentedFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>& source) {
   NumericalDomains::DAffine::TInstrumentedFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation> result(source);
   result.sqrtAssign();
   return result;
}

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
inline NumericalDomains::DAffine::TInstrumentedFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>
sqrt(NumericalDomains::DAffine::TInstrumentedFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>&& source) {
   typedef NumericalDomains::DAffine::TInstrumentedFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation> thisType;
   thisType result(std::forward<thisType>(source));
   result.sqrtAssign();
   return result;
}

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
inline NumericalDomains::DAffine::TInstrumentedFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>
sin(const NumericalDomains::DAffine::TInstrumentedFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>& source) {
   NumericalDomains::DAffine::TInstrumentedFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation> result(source);
   result.sinAssign();
   return result;
}

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
inline NumericalDomains::DAffine::TInstrumentedFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>
sin(NumericalDomains::DAffine::TInstrumentedFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>&& source) {
   typedef NumericalDomains::DAffine::TInstrumentedFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation> thisType;
   thisType result(std::forward<thisType>(source));
   result.sinAssign();
   return result;
}

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
inline NumericalDomains::DAffine::TInstrumentedFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>
cos(const NumericalDomains::DAffine::TInstrumentedFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>& source) {
   NumericalDomains::DAffine::TInstrumentedFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation> result(source);
   result.cosAssign();
   return result;
}

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
inline NumericalDomains::DAffine::TInstrumentedFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>
cos(NumericalDomains::DAffine::TInstrumentedFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>&& source) {
   typedef NumericalDomains::DAffine::TInstrumentedFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation> thisType;
   thisType result(std::forward<thisType>(source));
   result.cosAssign();
   return result;
}

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
inline NumericalDomains::DAffine::TInstrumentedFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>
asin(const NumericalDomains::DAffine::TInstrumentedFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>& source) {
   NumericalDomains::DAffine::TInstrumentedFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation> result(source);
   result.asinAssign();
   return result;
}

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
inline NumericalDomains::DAffine::TInstrumentedFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>
asin(NumericalDomains::DAffine::TInstrumentedFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>&& source) {
   typedef NumericalDomains::DAffine::TInstrumentedFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation> thisType;
   thisType result(std::forward<thisType>(source));
   result.asinAssign();
   return result;
}

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
inline NumericalDomains::DAffine::TInstrumentedFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>
acos(const NumericalDomains::DAffine::TInstrumentedFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>& source) {
   NumericalDomains::DAffine::TInstrumentedFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation> result(source);
   result.acosAssign();
   return result;
}

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
inline NumericalDomains::DAffine::TInstrumentedFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>
acos(NumericalDomains::DAffine::TInstrumentedFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>&& source) {
   typedef NumericalDomains::DAffine::TInstrumentedFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation> thisType;
   thisType result(std::forward<thisType>(source));
   result.acosAssign();
   return result;
}

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
inline NumericalDomains::DAffine::TInstrumentedFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>
tan(const NumericalDomains::DAffine::TInstrumentedFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>& source) {
   NumericalDomains::DAffine::TInstrumentedFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation> result(source);
   result.tanAssign();
   return result;
}

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
inline NumericalDomains::DAffine::TInstrumentedFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>
tan(NumericalDomains::DAffine::TInstrumentedFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>&& source) {
   typedef NumericalDomains::DAffine::TInstrumentedFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation> thisType;
   thisType result(std::forward<thisType>(source));
   result.tanAssign();
   return result;
}

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
inline NumericalDomains::DAffine::TInstrumentedFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>
atan(const NumericalDomains::DAffine::TInstrumentedFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>& source) {
   NumericalDomains::DAffine::TInstrumentedFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation> result(source);
   result.atanAssign();
   return result;
}

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
inline NumericalDomains::DAffine::TInstrumentedFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>
atan(NumericalDomains::DAffine::TInstrumentedFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>&& source) {
   typedef NumericalDomains::DAffine::TInstrumentedFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation> thisType;
   thisType result(std::forward<thisType>(source));
   result.atanAssign();
   return result;
}

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
inline NumericalDomains::DAffine::TInstrumentedFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>
exp(const NumericalDomains::DAffine::TInstrumentedFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>& source) {
   NumericalDomains::DAffine::TInstrumentedFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation> result(source);
   result.expAssign();
   return result;
}

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
inline NumericalDomains::DAffine::TInstrumentedFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>
exp(NumericalDomains::DAffine::TInstrumentedFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>&& source) {
   typedef NumericalDomains::DAffine::TInstrumentedFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation> thisType;
   thisType result(std::forward<thisType>(source));
   result.expAssign();
   return result;
}

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
inline NumericalDomains::DAffine::TInstrumentedFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>
log(const NumericalDomains::DAffine::TInstrumentedFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>& source) {
   NumericalDomains::DAffine::TInstrumentedFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation> result(source);
   result.logAssign();
   return result;
}

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
inline NumericalDomains::DAffine::TInstrumentedFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>
log(NumericalDomains::DAffine::TInstrumentedFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>&& source) {
   typedef NumericalDomains::DAffine::TInstrumentedFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation> thisType;
   thisType result(std::forward<thisType>(source));
   result.logAssign();
   return result;
}

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
inline NumericalDomains::DAffine::TInstrumentedFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>
log10(const NumericalDomains::DAffine::TInstrumentedFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>& source) {
   NumericalDomains::DAffine::TInstrumentedFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation> result(source);
   result.log10Assign();
   return result;
}

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
inline NumericalDomains::DAffine::TInstrumentedFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>
log10(NumericalDomains::DAffine::TInstrumentedFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>&& source) {
   typedef NumericalDomains::DAffine::TInstrumentedFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation> thisType;
   thisType result(std::forward<thisType>(source));
   result.log10Assign();
   return result;
}

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
inline NumericalDomains::DAffine::TInstrumentedFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>
pow(const NumericalDomains::DAffine::TInstrumentedFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>& source,
      const NumericalDomains::DAffine::TInstrumentedFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>& value) {
   NumericalDomains::DAffine::TInstrumentedFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation> result(source);
   result.powAssign(value, NumericalDomains::DAffine::Equation::PCSourceRValue);
   return result;
}

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
inline NumericalDomains::DAffine::TInstrumentedFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>
pow(const NumericalDomains::DAffine::TInstrumentedFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>& source,
      NumericalDomains::DAffine::TInstrumentedFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>&& value) {
   NumericalDomains::DAffine::TInstrumentedFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation> result(source);
   result.powAssign(value, NumericalDomains::DAffine::Equation::PCSourceXValue);
   return result;
}

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
inline NumericalDomains::DAffine::TInstrumentedFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>
pow(NumericalDomains::DAffine::TInstrumentedFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>&& source,
      const NumericalDomains::DAffine::TInstrumentedFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>& value) {
   typedef NumericalDomains::DAffine::TInstrumentedFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation> thisType;
   thisType result(std::forward<thisType>(source));
   result.powAssign(value, NumericalDomains::DAffine::Equation::PCSourceRValue);
   return result;
}

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
inline NumericalDomains::DAffine::TInstrumentedFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>
pow(NumericalDomains::DAffine::TInstrumentedFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>&& source,
      NumericalDomains::DAffine::TInstrumentedFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>&& value) {
   typedef NumericalDomains::DAffine::TInstrumentedFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation> thisType;
   thisType result(std::forward<thisType>(source));
   result.powAssign(value, NumericalDomains::DAffine::Equation::PCSourceXValue);
   return result;
}

template <typename TypeFst, int USizeMantissa, int USizeExponent, typename TypeImplementation>
inline NumericalDomains::DAffine::TInstrumentedFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>
pow(TypeFst source, const NumericalDomains::DAffine::TInstrumentedFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>& value) {
   NumericalDomains::DAffine::TInstrumentedFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation> result(source);
   result.powAssign(value, NumericalDomains::DAffine::Equation::PCSourceRValue);
   return result;
}

template <typename TypeFst, int USizeMantissa, int USizeExponent, typename TypeImplementation>
inline NumericalDomains::DAffine::TInstrumentedFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>
pow(TypeFst source, NumericalDomains::DAffine::TInstrumentedFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>&& value) {
   NumericalDomains::DAffine::TInstrumentedFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation> result(source);
   result.powAssign(value, NumericalDomains::DAffine::Equation::PCSourceXValue);
   return result;
}

template <typename TypeSnd, int USizeMantissa, int USizeExponent, typename TypeImplementation>
inline NumericalDomains::DAffine::TInstrumentedFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>
pow(const NumericalDomains::DAffine::TInstrumentedFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>& source,
      TypeSnd value) {
   typedef NumericalDomains::DAffine::TInstrumentedFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation> thisType;
   thisType result(source);
   result.powAssign(thisType(value), NumericalDomains::DAffine::Equation::PCSourceXValue);
   return result;
}

template <typename TypeSnd, int USizeMantissa, int USizeExponent, typename TypeImplementation>
inline NumericalDomains::DAffine::TInstrumentedFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>
pow(NumericalDomains::DAffine::TInstrumentedFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>&& source,
      TypeSnd value) {
   typedef NumericalDomains::DAffine::TInstrumentedFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation> thisType;
   thisType result(std::forward<thisType>(source));
   result.powAssign(thisType(value), NumericalDomains::DAffine::Equation::PCSourceXValue);
   return result;
}

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
inline NumericalDomains::DAffine::TInstrumentedFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>
powf(const NumericalDomains::DAffine::TInstrumentedFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>& source,
      const NumericalDomains::DAffine::TInstrumentedFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>& value) {
   NumericalDomains::DAffine::TInstrumentedFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation> result(source);
   result.powAssign(value, NumericalDomains::DAffine::Equation::PCSourceRValue);
   return result;
}

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
inline NumericalDomains::DAffine::TInstrumentedFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>
powf(const NumericalDomains::DAffine::TInstrumentedFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>& source,
      NumericalDomains::DAffine::TInstrumentedFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>&& value) {
   NumericalDomains::DAffine::TInstrumentedFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation> result(source);
   result.powAssign(value, NumericalDomains::DAffine::Equation::PCSourceXValue);
   return result;
}

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
inline NumericalDomains::DAffine::TInstrumentedFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>
powf(NumericalDomains::DAffine::TInstrumentedFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>&& source,
      const NumericalDomains::DAffine::TInstrumentedFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>& value) {
   typedef NumericalDomains::DAffine::TInstrumentedFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation> thisType;
   thisType result(std::forward<thisType>(source));
   result.powAssign(value, NumericalDomains::DAffine::Equation::PCSourceRValue);
   return result;
}

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
inline NumericalDomains::DAffine::TInstrumentedFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>
powf(NumericalDomains::DAffine::TInstrumentedFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>&& source,
      NumericalDomains::DAffine::TInstrumentedFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>&& value) {
   typedef NumericalDomains::DAffine::TInstrumentedFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation> thisType;
   thisType result(std::forward<thisType>(source));
   result.powAssign(value, NumericalDomains::DAffine::Equation::PCSourceXValue);
   return result;
}

template <typename TypeFst, int USizeMantissa, int USizeExponent, typename TypeImplementation>
inline NumericalDomains::DAffine::TInstrumentedFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>
powf(TypeFst source, const NumericalDomains::DAffine::TInstrumentedFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>& value) {
   NumericalDomains::DAffine::TInstrumentedFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation> result(source);
   result.powAssign(value, NumericalDomains::DAffine::Equation::PCSourceRValue);
   return result;
}

template <typename TypeFst, int USizeMantissa, int USizeExponent, typename TypeImplementation>
inline NumericalDomains::DAffine::TInstrumentedFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>
powf(TypeFst source, NumericalDomains::DAffine::TInstrumentedFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>&& value) {
   NumericalDomains::DAffine::TInstrumentedFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation> result(source);
   result.powAssign(value, NumericalDomains::DAffine::Equation::PCSourceXValue);
   return result;
}

template <typename TypeSnd, int USizeMantissa, int USizeExponent, typename TypeImplementation>
inline NumericalDomains::DAffine::TInstrumentedFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>
powf(const NumericalDomains::DAffine::TInstrumentedFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>& source,
      TypeSnd value) {
   typedef NumericalDomains::DAffine::TInstrumentedFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation> thisType;
   thisType result(source);
   result.powAssign(thisType(value), NumericalDomains::DAffine::Equation::PCSourceXValue);
   return result;
}

template <typename TypeSnd, int USizeMantissa, int USizeExponent, typename TypeImplementation>
inline NumericalDomains::DAffine::TInstrumentedFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>
powf(NumericalDomains::DAffine::TInstrumentedFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>&& source,
      TypeSnd value) {
   typedef NumericalDomains::DAffine::TInstrumentedFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation> thisType;
   thisType result(std::forward<thisType>(source));
   result.powAssign(thisType(value), NumericalDomains::DAffine::Equation::PCSourceXValue);
   return result;
}

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
inline NumericalDomains::DAffine::TInstrumentedFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>
atan2(const NumericalDomains::DAffine::TInstrumentedFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>& source,
      const NumericalDomains::DAffine::TInstrumentedFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>& value) {
   NumericalDomains::DAffine::TInstrumentedFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation> result(source);
   result.atan2Assign(value, NumericalDomains::DAffine::Equation::PCSourceRValue);
   return result;
}

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
inline NumericalDomains::DAffine::TInstrumentedFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>
atan2(const NumericalDomains::DAffine::TInstrumentedFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>& source,
      NumericalDomains::DAffine::TInstrumentedFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>&& value) {
   NumericalDomains::DAffine::TInstrumentedFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation> result(source);
   result.atan2Assign(value, NumericalDomains::DAffine::Equation::PCSourceXValue);
   return result;
}

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
inline NumericalDomains::DAffine::TInstrumentedFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>
atan2(NumericalDomains::DAffine::TInstrumentedFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>&& source,
      const NumericalDomains::DAffine::TInstrumentedFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>& value) {
   typedef NumericalDomains::DAffine::TInstrumentedFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation> thisType;
   thisType result(std::forward<thisType>(source));
   result.atan2Assign(value, NumericalDomains::DAffine::Equation::PCSourceRValue);
   return result;
}

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
inline NumericalDomains::DAffine::TInstrumentedFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>
atan2(NumericalDomains::DAffine::TInstrumentedFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>&& source,
      NumericalDomains::DAffine::TInstrumentedFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>&& value) {
   typedef NumericalDomains::DAffine::TInstrumentedFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation> thisType;
   thisType result(std::forward<thisType>(source));
   result.atan2Assign(value, NumericalDomains::DAffine::Equation::PCSourceXValue);
   return result;
}

template <typename TypeFst, int USizeMantissa, int USizeExponent, typename TypeImplementation>
inline NumericalDomains::DAffine::TInstrumentedFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>
atan2(TypeFst source, const NumericalDomains::DAffine::TInstrumentedFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>& value) {
   NumericalDomains::DAffine::TInstrumentedFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation> result(source);
   result.atan2Assign(value, NumericalDomains::DAffine::Equation::PCSourceRValue);
   return result;
}

template <typename TypeFst, int USizeMantissa, int USizeExponent, typename TypeImplementation>
inline NumericalDomains::DAffine::TInstrumentedFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>
atan2(TypeFst source, NumericalDomains::DAffine::TInstrumentedFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>&& value) {
   NumericalDomains::DAffine::TInstrumentedFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation> result(source);
   result.atan2Assign(value, NumericalDomains::DAffine::Equation::PCSourceXValue);
   return result;
}

template <typename TypeSnd, int USizeMantissa, int USizeExponent, typename TypeImplementation>
inline NumericalDomains::DAffine::TInstrumentedFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>
atan2(const NumericalDomains::DAffine::TInstrumentedFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>& source,
      TypeSnd value) {
   typedef NumericalDomains::DAffine::TInstrumentedFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation> thisType;
   thisType result(source);
   result.atan2Assign(thisType(value), NumericalDomains::DAffine::Equation::PCSourceXValue);
   return result;
}

template <typename TypeSnd, int USizeMantissa, int USizeExponent, typename TypeImplementation>
inline NumericalDomains::DAffine::TInstrumentedFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>
atan2(NumericalDomains::DAffine::TInstrumentedFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>&& source,
      TypeSnd value) {
   typedef NumericalDomains::DAffine::TInstrumentedFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation> thisType;
   thisType result(std::forward<thisType>(source));
   result.atan2Assign(thisType(value), NumericalDomains::DAffine::Equation::PCSourceXValue);
   return result;
}

template <typename TypeFst, int USizeMantissa, int USizeExponent, typename TypeImplementation>
inline bool
operator<(TypeFst fst, const NumericalDomains::DAffine::TInstrumentedFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>& snd)
   {  return NumericalDomains::DAffine::TInstrumentedFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>(fst).operator<(snd); }

template <typename TypeFst, int USizeMantissa, int USizeExponent, typename TypeImplementation>
inline bool
operator<=(TypeFst fst, const NumericalDomains::DAffine::TInstrumentedFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>& snd)
   {  return NumericalDomains::DAffine::TInstrumentedFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>(fst).operator<=(snd); }

template <typename TypeFst, int USizeMantissa, int USizeExponent, typename TypeImplementation>
inline bool
operator==(TypeFst fst, const NumericalDomains::DAffine::TInstrumentedFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>& snd)
   {  return NumericalDomains::DAffine::TInstrumentedFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>(fst).operator==(snd); }

template <typename TypeFst, int USizeMantissa, int USizeExponent, typename TypeImplementation>
inline bool
operator!=(TypeFst fst, const NumericalDomains::DAffine::TInstrumentedFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>& snd)
   {  return NumericalDomains::DAffine::TInstrumentedFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>(fst).operator!=(snd); }

template <typename TypeFst, int USizeMantissa, int USizeExponent, typename TypeImplementation>
inline bool
operator>=(TypeFst fst, const NumericalDomains::DAffine::TInstrumentedFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>& snd)
   {  return NumericalDomains::DAffine::TInstrumentedFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>(fst).operator>=(snd); }

template <typename TypeFst, int USizeMantissa, int USizeExponent, typename TypeImplementation>
inline bool
operator>(TypeFst fst, const NumericalDomains::DAffine::TInstrumentedFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>& snd)
   {  return NumericalDomains::DAffine::TInstrumentedFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>(fst).operator>(snd); }

template <typename TypeFst, int USizeMantissa, int USizeExponent, typename TypeImplementation>
inline NumericalDomains::DAffine::TInstrumentedFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>
operator+(TypeFst fst, const NumericalDomains::DAffine::TInstrumentedFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>& snd)
{  typedef NumericalDomains::DAffine::TInstrumentedFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation> thisType;
#if !defined(FLOAT_GENERIC_BASE_UNSIGNED) && !defined(FLOAT_GENERIC_BASE_LONG)
   typedef NumericalDomains::DAffine::TEquation<FLOAT_REAL_BITS_NUMBER, NumericalDomains::DAffine::TBaseFloatAffine<NumericalDomains::DAffine::ExecutionPath> > Equation;
#elif defined(FLOAT_GENERIC_BASE_LONG)
   typedef NumericalDomains::DAffine::TGEquation<Numerics::UnsignedLongBaseStoreTraits, FLOAT_REAL_BITS_NUMBER, NumericalDomains::DAffine::TBaseFloatAffine<NumericalDomains::DAffine::ExecutionPath> > Equation;
#else // defined(FLOAT_GENERIC_BASE_UNSIGNED)
   typedef NumericalDomains::DAffine::TGEquation<Numerics::UnsignedBaseStoreTraits, FLOAT_REAL_BITS_NUMBER, NumericalDomains::DAffine::TBaseFloatAffine<NumericalDomains::DAffine::ExecutionPath> > Equation;
#endif
   thisType fstConvert(fst);
   fstConvert.plusAssign(snd, Equation::PCSourceRValue);
   return fstConvert;
}

template <typename TypeFst, int USizeMantissa, int USizeExponent, typename TypeImplementation>
inline NumericalDomains::DAffine::TInstrumentedFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>
operator+(TypeFst fst, NumericalDomains::DAffine::TInstrumentedFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>&& snd)
{  typedef NumericalDomains::DAffine::TInstrumentedFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation> thisType;
#if !defined(FLOAT_GENERIC_BASE_UNSIGNED) && !defined(FLOAT_GENERIC_BASE_LONG)
   typedef NumericalDomains::DAffine::TEquation<FLOAT_REAL_BITS_NUMBER, NumericalDomains::DAffine::TBaseFloatAffine<NumericalDomains::DAffine::ExecutionPath> > Equation;
#elif defined(FLOAT_GENERIC_BASE_LONG)
   typedef NumericalDomains::DAffine::TGEquation<Numerics::UnsignedLongBaseStoreTraits, FLOAT_REAL_BITS_NUMBER, NumericalDomains::DAffine::TBaseFloatAffine<NumericalDomains::DAffine::ExecutionPath> > Equation;
#else // defined(FLOAT_GENERIC_BASE_UNSIGNED)
   typedef NumericalDomains::DAffine::TGEquation<Numerics::UnsignedBaseStoreTraits, FLOAT_REAL_BITS_NUMBER, NumericalDomains::DAffine::TBaseFloatAffine<NumericalDomains::DAffine::ExecutionPath> > Equation;
#endif
   thisType fstConvert(fst);
   fstConvert.plusAssign(snd, Equation::PCSourceXValue);
   return fstConvert;
}

template <typename TypeFst, int USizeMantissa, int USizeExponent, typename TypeImplementation>
inline NumericalDomains::DAffine::TInstrumentedFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>
operator-(TypeFst fst, const NumericalDomains::DAffine::TInstrumentedFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>& snd)
{  typedef NumericalDomains::DAffine::TInstrumentedFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation> thisType;
#if !defined(FLOAT_GENERIC_BASE_UNSIGNED) && !defined(FLOAT_GENERIC_BASE_LONG)
   typedef NumericalDomains::DAffine::TEquation<FLOAT_REAL_BITS_NUMBER, NumericalDomains::DAffine::TBaseFloatAffine<NumericalDomains::DAffine::ExecutionPath> > Equation;
#elif defined(FLOAT_GENERIC_BASE_LONG)
   typedef NumericalDomains::DAffine::TGEquation<Numerics::UnsignedLongBaseStoreTraits, FLOAT_REAL_BITS_NUMBER, NumericalDomains::DAffine::TBaseFloatAffine<NumericalDomains::DAffine::ExecutionPath> > Equation;
#else // defined(FLOAT_GENERIC_BASE_UNSIGNED)
   typedef NumericalDomains::DAffine::TGEquation<Numerics::UnsignedBaseStoreTraits, FLOAT_REAL_BITS_NUMBER, NumericalDomains::DAffine::TBaseFloatAffine<NumericalDomains::DAffine::ExecutionPath> > Equation;
#endif
   thisType fstConvert(fst);
   fstConvert.minusAssign(snd, Equation::PCSourceRValue);
   return fstConvert;
}

template <typename TypeFst, int USizeMantissa, int USizeExponent, typename TypeImplementation>
inline NumericalDomains::DAffine::TInstrumentedFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>
operator-(TypeFst fst, NumericalDomains::DAffine::TInstrumentedFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>&& snd)
{  typedef NumericalDomains::DAffine::TInstrumentedFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation> thisType;
#if !defined(FLOAT_GENERIC_BASE_UNSIGNED) && !defined(FLOAT_GENERIC_BASE_LONG)
   typedef NumericalDomains::DAffine::TEquation<FLOAT_REAL_BITS_NUMBER, NumericalDomains::DAffine::TBaseFloatAffine<NumericalDomains::DAffine::ExecutionPath> > Equation;
#elif defined(FLOAT_GENERIC_BASE_LONG)
   typedef NumericalDomains::DAffine::TGEquation<Numerics::UnsignedLongBaseStoreTraits, FLOAT_REAL_BITS_NUMBER, NumericalDomains::DAffine::TBaseFloatAffine<NumericalDomains::DAffine::ExecutionPath> > Equation;
#else // defined(FLOAT_GENERIC_BASE_UNSIGNED)
   typedef NumericalDomains::DAffine::TGEquation<Numerics::UnsignedBaseStoreTraits, FLOAT_REAL_BITS_NUMBER, NumericalDomains::DAffine::TBaseFloatAffine<NumericalDomains::DAffine::ExecutionPath> > Equation;
#endif
   thisType fstConvert(fst);
   fstConvert.minusAssign(snd, Equation::PCSourceXValue);
   return fstConvert;
}

template <typename TypeFst, int USizeMantissa, int USizeExponent, typename TypeImplementation>
inline NumericalDomains::DAffine::TInstrumentedFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>
operator*(TypeFst fst, const NumericalDomains::DAffine::TInstrumentedFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>& snd)
{  typedef NumericalDomains::DAffine::TInstrumentedFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation> thisType;
#if !defined(FLOAT_GENERIC_BASE_UNSIGNED) && !defined(FLOAT_GENERIC_BASE_LONG)
   typedef NumericalDomains::DAffine::TEquation<FLOAT_REAL_BITS_NUMBER, NumericalDomains::DAffine::TBaseFloatAffine<NumericalDomains::DAffine::ExecutionPath> > Equation;
#elif defined(FLOAT_GENERIC_BASE_LONG)
   typedef NumericalDomains::DAffine::TGEquation<Numerics::UnsignedLongBaseStoreTraits, FLOAT_REAL_BITS_NUMBER, NumericalDomains::DAffine::TBaseFloatAffine<NumericalDomains::DAffine::ExecutionPath> > Equation;
#else // defined(FLOAT_GENERIC_BASE_UNSIGNED)
   typedef NumericalDomains::DAffine::TGEquation<Numerics::UnsignedBaseStoreTraits, FLOAT_REAL_BITS_NUMBER, NumericalDomains::DAffine::TBaseFloatAffine<NumericalDomains::DAffine::ExecutionPath> > Equation;
#endif
   thisType fstConvert(fst);
   fstConvert.multAssign(snd, Equation::PCSourceRValue);
   return fstConvert;
}

template <typename TypeFst, int USizeMantissa, int USizeExponent, typename TypeImplementation>
inline NumericalDomains::DAffine::TInstrumentedFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>
operator*(TypeFst fst, NumericalDomains::DAffine::TInstrumentedFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>&& snd)
{  typedef NumericalDomains::DAffine::TInstrumentedFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation> thisType;
#if !defined(FLOAT_GENERIC_BASE_UNSIGNED) && !defined(FLOAT_GENERIC_BASE_LONG)
   typedef NumericalDomains::DAffine::TEquation<FLOAT_REAL_BITS_NUMBER, NumericalDomains::DAffine::TBaseFloatAffine<NumericalDomains::DAffine::ExecutionPath> > Equation;
#elif defined(FLOAT_GENERIC_BASE_LONG)
   typedef NumericalDomains::DAffine::TGEquation<Numerics::UnsignedLongBaseStoreTraits, FLOAT_REAL_BITS_NUMBER, NumericalDomains::DAffine::TBaseFloatAffine<NumericalDomains::DAffine::ExecutionPath> > Equation;
#else // defined(FLOAT_GENERIC_BASE_UNSIGNED)
   typedef NumericalDomains::DAffine::TGEquation<Numerics::UnsignedBaseStoreTraits, FLOAT_REAL_BITS_NUMBER, NumericalDomains::DAffine::TBaseFloatAffine<NumericalDomains::DAffine::ExecutionPath> > Equation;
#endif
   thisType fstConvert(fst);
   fstConvert.multAssign(snd, Equation::PCSourceXValue);
   return fstConvert;
}

template <typename TypeFst, int USizeMantissa, int USizeExponent, typename TypeImplementation>
inline NumericalDomains::DAffine::TInstrumentedFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>
operator/(TypeFst fst, const NumericalDomains::DAffine::TInstrumentedFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>& snd)
{  typedef NumericalDomains::DAffine::TInstrumentedFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation> thisType;
#if !defined(FLOAT_GENERIC_BASE_UNSIGNED) && !defined(FLOAT_GENERIC_BASE_LONG)
   typedef NumericalDomains::DAffine::TEquation<FLOAT_REAL_BITS_NUMBER, NumericalDomains::DAffine::TBaseFloatAffine<NumericalDomains::DAffine::ExecutionPath> > Equation;
#elif defined(FLOAT_GENERIC_BASE_LONG)
   typedef NumericalDomains::DAffine::TGEquation<Numerics::UnsignedLongBaseStoreTraits, FLOAT_REAL_BITS_NUMBER, NumericalDomains::DAffine::TBaseFloatAffine<NumericalDomains::DAffine::ExecutionPath> > Equation;
#else // defined(FLOAT_GENERIC_BASE_UNSIGNED)
   typedef NumericalDomains::DAffine::TGEquation<Numerics::UnsignedBaseStoreTraits, FLOAT_REAL_BITS_NUMBER, NumericalDomains::DAffine::TBaseFloatAffine<NumericalDomains::DAffine::ExecutionPath> > Equation;
#endif
   thisType fstConvert(fst);
   fstConvert.divAssign(snd, Equation::PCSourceRValue);
   return fstConvert;
}

template <typename TypeFst, int USizeMantissa, int USizeExponent, typename TypeImplementation>
inline NumericalDomains::DAffine::TInstrumentedFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>
operator/(TypeFst fst, NumericalDomains::DAffine::TInstrumentedFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>&& snd)
{  typedef NumericalDomains::DAffine::TInstrumentedFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation> thisType;
#if !defined(FLOAT_GENERIC_BASE_UNSIGNED) && !defined(FLOAT_GENERIC_BASE_LONG)
   typedef NumericalDomains::DAffine::TEquation<FLOAT_REAL_BITS_NUMBER, NumericalDomains::DAffine::TBaseFloatAffine<NumericalDomains::DAffine::ExecutionPath> > Equation;
#elif defined(FLOAT_GENERIC_BASE_LONG)
   typedef NumericalDomains::DAffine::TGEquation<Numerics::UnsignedLongBaseStoreTraits, FLOAT_REAL_BITS_NUMBER, NumericalDomains::DAffine::TBaseFloatAffine<NumericalDomains::DAffine::ExecutionPath> > Equation;
#else // defined(FLOAT_GENERIC_BASE_UNSIGNED)
   typedef NumericalDomains::DAffine::TGEquation<Numerics::UnsignedBaseStoreTraits, FLOAT_REAL_BITS_NUMBER, NumericalDomains::DAffine::TBaseFloatAffine<NumericalDomains::DAffine::ExecutionPath> > Equation;
#endif
   thisType fstConvert(fst);
   fstConvert.divAssign(snd, Equation::PCSourceXValue);
   return fstConvert;
}

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
inline NumericalDomains::DAffine::TInstrumentedFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>
log2(const NumericalDomains::DAffine::TInstrumentedFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>& fst)
{  typedef NumericalDomains::DAffine::TInstrumentedFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation> thisType;
   thisType result = fst;
   result.logAssign();
   result.divAssign(thisType(log(2)), NumericalDomains::DAffine::Equation::PCSourceXValue);
   return result;
}

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
inline NumericalDomains::DAffine::TInstrumentedFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>
log2(NumericalDomains::DAffine::TInstrumentedFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>&& fst)
{  typedef NumericalDomains::DAffine::TInstrumentedFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation> thisType;
   thisType result(std::forward<thisType>(fst));
   result.logAssign();
   result.divAssign(thisType(log(2)), NumericalDomains::DAffine::Equation::PCSourceXValue);
   return result;
}

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
inline NumericalDomains::DAffine::TInstrumentedFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>
exp2(const NumericalDomains::DAffine::TInstrumentedFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>& fst)
{  typedef NumericalDomains::DAffine::TInstrumentedFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation> thisType;
   thisType result = 2.0;
   result.powAssign(fst, NumericalDomains::DAffine::Equation::PCSourceRValue);
   return result;
}

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
inline NumericalDomains::DAffine::TInstrumentedFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>
exp2(NumericalDomains::DAffine::TInstrumentedFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>&& fst)
{  typedef NumericalDomains::DAffine::TInstrumentedFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation> thisType;
   thisType result = 2.0;
   result.powAssign(fst, NumericalDomains::DAffine::Equation::PCSourceXValue);
   return result;
}

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
inline NumericalDomains::DAffine::TInstrumentedFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>
floor(const NumericalDomains::DAffine::TInstrumentedFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>& fst)
{  typedef NumericalDomains::DAffine::TInstrumentedFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation> thisType;
   return thisType(std::forward<thisType>(thisType(fst)).asInt(thisType::ReadParametersBase::RMLowest));
}

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
inline NumericalDomains::DAffine::TInstrumentedFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>
floor(NumericalDomains::DAffine::TInstrumentedFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>&& fst)
{  typedef NumericalDomains::DAffine::TInstrumentedFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation> thisType;
   return thisType(std::forward<thisType>(thisType(fst)).asInt(thisType::ReadParametersBase::RMLowest));
}

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
inline NumericalDomains::DAffine::TInstrumentedFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>
ceil(const NumericalDomains::DAffine::TInstrumentedFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>& fst)
{  typedef NumericalDomains::DAffine::TInstrumentedFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation> thisType;
   return thisType(std::forward<thisType>(thisType(fst)).asInt(thisType::ReadParametersBase::RMHighest));
}

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
inline NumericalDomains::DAffine::TInstrumentedFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>
ceil(NumericalDomains::DAffine::TInstrumentedFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>&& fst)
{  typedef NumericalDomains::DAffine::TInstrumentedFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation> thisType;
   return thisType(std::forward<thisType>(thisType(fst)).asInt(thisType::ReadParametersBase::RMHighest));
}

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
inline NumericalDomains::DAffine::TInstrumentedFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>
trunc(const NumericalDomains::DAffine::TInstrumentedFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>& fst)
{  typedef NumericalDomains::DAffine::TInstrumentedFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation> thisType;
   return thisType(std::forward<thisType>(thisType(fst)).asInt(thisType::ReadParametersBase::RMZero));
}

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
inline NumericalDomains::DAffine::TInstrumentedFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>
trunc(NumericalDomains::DAffine::TInstrumentedFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>&& fst)
{  typedef NumericalDomains::DAffine::TInstrumentedFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation> thisType;
   return thisType(std::forward<thisType>(thisType(fst)).asInt(thisType::ReadParametersBase::RMZero));
}

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
inline NumericalDomains::DAffine::TInstrumentedFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>
round(const NumericalDomains::DAffine::TInstrumentedFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>& fst)
{  typedef NumericalDomains::DAffine::TInstrumentedFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation> thisType;
   return thisType(std::forward<thisType>(thisType(fst)).asInt(thisType::ReadParametersBase::RMNearest));
}

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
inline NumericalDomains::DAffine::TInstrumentedFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>
round(NumericalDomains::DAffine::TInstrumentedFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>&& fst)
{  typedef NumericalDomains::DAffine::TInstrumentedFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation> thisType;
   return thisType(std::forward<thisType>(thisType(fst)).asInt(thisType::ReadParametersBase::RMNearest));
}

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
inline NumericalDomains::DAffine::TInstrumentedFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>
rintf(const NumericalDomains::DAffine::TInstrumentedFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>& fst)
{  typedef NumericalDomains::DAffine::TInstrumentedFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation> thisType;
   return thisType(std::forward<thisType>(thisType(fst)).asInt(thisType::ReadParametersBase::RMNearest /* fegetround */));
}

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
inline NumericalDomains::DAffine::TInstrumentedFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>
rintf(NumericalDomains::DAffine::TInstrumentedFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>&& fst)
{  typedef NumericalDomains::DAffine::TInstrumentedFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation> thisType;
   return thisType(std::forward<thisType>(thisType(fst)).asInt(thisType::ReadParametersBase::RMNearest /* fegetround */));
}

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
inline NumericalDomains::DAffine::TInstrumentedFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>
rint(const NumericalDomains::DAffine::TInstrumentedFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>& fst)
{  typedef NumericalDomains::DAffine::TInstrumentedFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation> thisType;
   return thisType(std::forward<thisType>(thisType(fst)).asInt(thisType::ReadParametersBase::RMNearest /* fegetround */));
}

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
inline NumericalDomains::DAffine::TInstrumentedFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>
rint(NumericalDomains::DAffine::TInstrumentedFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>&& fst)
{  typedef NumericalDomains::DAffine::TInstrumentedFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation> thisType;
   return thisType(std::forward<thisType>(thisType(fst)).asInt(thisType::ReadParametersBase::RMNearest /* fegetround */));
}

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
inline NumericalDomains::DAffine::TInstrumentedFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>
fabs(const NumericalDomains::DAffine::TInstrumentedFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>& fst)
{  typedef NumericalDomains::DAffine::TInstrumentedFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation> thisType;
   thisType result = fst;

   // see float_diagnosis.h FLOAT_SPLIT_ALL FLOAT_MERGE_ALL
   bool oldSupportUnstableInLoop = fst.doesSupportUnstableInLoop();
   fst.setSupportUnstableInLoop();
   typedef NumericalDomains::DAffine::BaseExecutionPath BaseExecutionPath;
   auto* oldPathExplorer = NumericalDomains::DAffine::ExecutionPath::getCurrentPathExplorer();
   bool oldDoesFollow = NumericalDomains::DAffine::ExecutionPath::doesFollowFlow();
   NumericalDomains::DAffine::ExecutionPath::clearFollowFlow();
   auto* oldInputTraceFile = NumericalDomains::DAffine::ExecutionPath::inputTraceFile();
   const char* oldSynchronisationFile = NumericalDomains::DAffine::ExecutionPath::synchronisationFile();
   int oldSynchronisationLine = NumericalDomains::DAffine::ExecutionPath::synchronisationLine();
   bool isCompleteFlow = true;
   NumericalDomains::DAffine::PathExplorer pathExplorer(
        NumericalDomains::DAffine::ExecutionPath::queryMode(oldPathExplorer));
   NumericalDomains::DAffine::ExecutionPath::setCurrentPathExplorer(&pathExplorer);
   auto mergeMemory = NumericalDomains::DAffine::MergeMemory() >> result >> BaseExecutionPath::end();
   auto saveMemory = NumericalDomains::DAffine::SaveMemory() << result << BaseExecutionPath::end();
   const char* sourceFile = __FILE__;
   int sourceLine = __LINE__;
   do {
      try {
         NumericalDomains::DAffine::BaseFloatAffine::splitBranches(sourceFile, sourceLine);

         if (result < 0)
            result.oppositeAssign();

         isCompleteFlow = NumericalDomains::DAffine::MergeBranches(sourceFile, sourceLine) << result << BaseExecutionPath::end();
      } 
      catch (typename thisType::anticipated_termination&) {
         isCompleteFlow = false;
         NumericalDomains::DAffine::ExecutionPath::clearSynchronizationBranches();
      }
      catch (STG::EReadError& error) {
         if (const char* message = error.getMessage())
            std::cerr << "error: " << message << std::endl;
         else
            std::cerr << "error while reading input file!" << std::endl;
         isCompleteFlow = false;
         NumericalDomains::DAffine::ExecutionPath::clearSynchronizationBranches();
      }
      NumericalDomains::DAffine::ExecutionPath::setFollowFlow();
   } while ((mergeMemory.setCurrentComplete(isCompleteFlow) << result << BaseExecutionPath::end())
         && !(saveMemory.setCurrentResult(pathExplorer.isFinished(NumericalDomains::DAffine::ExecutionPath::queryMode(oldPathExplorer))) >> result >> BaseExecutionPath::end()));
   NumericalDomains::DAffine::ExecutionPath::setFollowFlow(oldDoesFollow, oldInputTraceFile,
         oldSynchronisationFile, oldSynchronisationLine);
   NumericalDomains::DAffine::ExecutionPath::setCurrentPathExplorer(oldPathExplorer);
   fst.setSupportUnstableInLoop(oldSupportUnstableInLoop);
   if (mergeMemory.isFirst())
      NumericalDomains::DAffine::ExecutionPath::throwEmptyBranch(true);
   return result;
}

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
inline NumericalDomains::DAffine::TInstrumentedFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>
fabs(NumericalDomains::DAffine::TInstrumentedFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>&& fst)
{  typedef NumericalDomains::DAffine::TInstrumentedFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation> thisType;
   thisType result(std::forward<thisType>(fst));

   // see float_diagnosis.h FLOAT_SPLIT_ALL FLOAT_MERGE_ALL
   bool oldSupportUnstableInLoop = fst.doesSupportUnstableInLoop();
   fst.setSupportUnstableInLoop();
   typedef NumericalDomains::DAffine::BaseExecutionPath BaseExecutionPath;
   auto* oldPathExplorer = NumericalDomains::DAffine::ExecutionPath::getCurrentPathExplorer();
   bool oldDoesFollow = NumericalDomains::DAffine::ExecutionPath::doesFollowFlow();
   NumericalDomains::DAffine::ExecutionPath::clearFollowFlow();
   auto* oldInputTraceFile = NumericalDomains::DAffine::ExecutionPath::inputTraceFile();
   const char* oldSynchronisationFile = NumericalDomains::DAffine::ExecutionPath::synchronisationFile();
   int oldSynchronisationLine = NumericalDomains::DAffine::ExecutionPath::synchronisationLine();
   bool isCompleteFlow = true;
   NumericalDomains::DAffine::PathExplorer pathExplorer(
        NumericalDomains::DAffine::ExecutionPath::queryMode(oldPathExplorer));
   NumericalDomains::DAffine::ExecutionPath::setCurrentPathExplorer(&pathExplorer);
   auto mergeMemory = NumericalDomains::DAffine::MergeMemory() >> result >> BaseExecutionPath::end();
   auto saveMemory = NumericalDomains::DAffine::SaveMemory() << result << BaseExecutionPath::end();
   const char* sourceFile = __FILE__;
   int sourceLine = __LINE__;
   do {
      try {
         NumericalDomains::DAffine::BaseFloatAffine::splitBranches(sourceFile, sourceLine);

         if (result < 0)
            result.oppositeAssign();

         isCompleteFlow = NumericalDomains::DAffine::MergeBranches(sourceFile, sourceLine) << result
		<< BaseExecutionPath::end();
      } 
      catch (typename thisType::anticipated_termination&) {
         isCompleteFlow = false;
         NumericalDomains::DAffine::ExecutionPath::clearSynchronizationBranches();
      }
      catch (STG::EReadError& error) {
         if (const char* message = error.getMessage())
            std::cerr << "error: " << message << std::endl;
         else
            std::cerr << "error while reading input file!" << std::endl;
         isCompleteFlow = false;
         NumericalDomains::DAffine::ExecutionPath::clearSynchronizationBranches();
      }
      NumericalDomains::DAffine::ExecutionPath::setFollowFlow();
   } while ((mergeMemory.setCurrentComplete(isCompleteFlow) << result << BaseExecutionPath::end())
         && !(saveMemory.setCurrentResult(pathExplorer.isFinished(NumericalDomains::DAffine::ExecutionPath::queryMode(oldPathExplorer))) >> result >> BaseExecutionPath::end()));
   NumericalDomains::DAffine::ExecutionPath::setFollowFlow(oldDoesFollow, oldInputTraceFile,
         oldSynchronisationFile, oldSynchronisationLine);
   NumericalDomains::DAffine::ExecutionPath::setCurrentPathExplorer(oldPathExplorer);
   fst.setSupportUnstableInLoop(oldSupportUnstableInLoop);
   if (mergeMemory.isFirst())
      NumericalDomains::DAffine::ExecutionPath::throwEmptyBranch(true);
   return result;
}

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
inline std::ostream&
operator<<(std::ostream& out, const NumericalDomains::DAffine::TInstrumentedFloatZonotope
      <USizeMantissa, USizeExponent, TypeImplementation>& value) {
   out << value.asImplementation();
   return out;
}

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
inline std::istream&
operator>>(std::istream& in, NumericalDomains::DAffine::TInstrumentedFloatZonotope
      <USizeMantissa, USizeExponent, TypeImplementation>& value) {
   decltype(value.asImplementation()) result; 
   in >> result;
   value = NumericalDomains::DAffine::TInstrumentedFloatZonotope
         <USizeMantissa, USizeExponent, TypeImplementation>(result, result);
   return in;
}

#endif // FloatInstrumentation_FloatAffineH

