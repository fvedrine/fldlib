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

template <typename TypeIterator, class TypeSaveMemory>
class TPackedSaveMemory;

template<typename> struct thasEquationHolder { typedef int type; };

struct EveryType {};
struct AffineType : public EveryType {};

template <typename T>
void
tremoveHolder(T&, EveryType) {}

template <typename T>
void
tsetHolder(T&, T&, EveryType) {}

template <typename T, typename thasEquationHolder<typename T::InstrumentedAffineType>::type=0>
void
tremoveHolder(T& save, AffineType) {
   if (save.doesSupportUnstableInLoop()) {
      save.getSRealDomain().clearHolder();
      save.getSError().clearHolder();
   };
}

template <typename T, typename thasEquationHolder<typename T::InstrumentedAffineType>::type=0>
void
tsetHolder(T& source, T& save, AffineType) {
   if (source.doesSupportUnstableInLoop()) {
      source.getSRealDomain().setHolder(source.currentPathExplorer);
      source.getSError().setHolder(source.currentPathExplorer);
      save.getSRealDomain().clearHolder();
      save.getSError().clearHolder();
   };
}

template <typename T1, class TypeSaveMemory>
class TSaveMemory {
  public:
   T1 save;
   TypeSaveMemory next;

   TSaveMemory(T1& saveArg, TypeSaveMemory nextArg)
      :  save(saveArg), next(nextArg)
      {  tsetHolder(saveArg, save, AffineType()); }
   TSaveMemory(const TSaveMemory<T1, TypeSaveMemory>& source)
      :  save(source.save), next(source.next)
      {  tremoveHolder(save, AffineType()); }
   TSaveMemory(TSaveMemory<T1, TypeSaveMemory>&& source)
      :  save(std::move(source.save)), next(std::move(source.next))
      {  tremoveHolder(save, AffineType()); }

   template <typename T>
   TSaveMemory<T, TSaveMemory<T1, TypeSaveMemory> > operator<<(T& t)
      {  return TSaveMemory<T, TSaveMemory<T1, TypeSaveMemory> >(t, *this); }
   template <typename T>
   TSaveMemory<T, TSaveMemory<T1, TypeSaveMemory> > operator<<(const T& t)
      {  return TSaveMemory<T, TSaveMemory<T1, TypeSaveMemory> >(const_cast<T&>(t), *this); }
   template <typename TypeIterator>
   TPackedSaveMemory<TypeIterator, TSaveMemory<T1, TypeSaveMemory> > operator<<(MergeBranches::TPacker<TypeIterator> packer);

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

template <typename TypeIterator, class TypeSaveMemory>
class TPackedSaveMemory {
  public:
   COL::TVector<typename TypeIterator::value_type> save;
   TypeSaveMemory next;

   TPackedSaveMemory(TypeIterator iter, TypeIterator end, TypeSaveMemory nextArg)
      :  next(nextArg)
      {  int count = end - iter;
         save.bookPlace(count);
         for (; iter != end; ++iter) {
            save.insertAtEnd(*iter);
            tsetHolder(*iter, save.referenceAt(save.count()-1), AffineType());
         }
      }
   TPackedSaveMemory(const TPackedSaveMemory<TypeIterator, TypeSaveMemory>& source)
      :  save(source.save), next(source.next)
      {  int count = save.count();
         for (int index = 0; index < count; ++index)
            tremoveHolder(save.referenceAt(index), AffineType());
      }
   TPackedSaveMemory(TPackedSaveMemory<TypeIterator, TypeSaveMemory>&& source)
      :  save(std::move(source.save)), next(std::move(source.next))
      {  int count = save.count();
         for (int index = 0; index < count; ++index)
            tremoveHolder(save.referenceAt(index), AffineType());
      }

   template <typename T>
   TSaveMemory<T, TPackedSaveMemory<TypeIterator, TypeSaveMemory> > operator<<(T& t)
      {  return TSaveMemory<T, TPackedSaveMemory<TypeIterator, TypeSaveMemory> >(t, *this); }
   template <class TypeIteratorArgument>
   TPackedSaveMemory<TypeIteratorArgument, TPackedSaveMemory<TypeIterator, TypeSaveMemory> >
      operator<<(MergeBranches::TPacker<TypeIteratorArgument> packer)
      {  return TPackedSaveMemory<TypeIteratorArgument, TPackedSaveMemory<TypeIterator, TypeSaveMemory> >
            (packer.iter, packer.end, *this);
      }
   TPackedSaveMemory<TypeIterator, TypeSaveMemory>& operator<<(BaseExecutionPath::end) { return *this; }

   TPackedSaveMemory<TypeIterator, TypeSaveMemory>& setCurrentResult(bool result)
      {  next.setCurrentResult(result); return *this; }
   
   TypeSaveMemory& operator>>(MergeBranches::TPacker<TypeIterator>&& packer)
      {  if (!next.getResult()) {
            int count = packer.end - packer.iter;
            AssumeCondition(count == save.count())
            for (int index = 0; index < count; ++index) {
               *packer.iter = save[index];
               ++packer.iter;
            }
         }
         return next;
      }
   bool getResult() const { return next.getResult(); }
};

template <typename T1, class TypeSaveMemory>
template <typename TypeIterator>
inline
TPackedSaveMemory<TypeIterator, TSaveMemory<T1, TypeSaveMemory> >
TSaveMemory<T1, TypeSaveMemory>::operator<<(MergeBranches::TPacker<TypeIterator> packer)
   {  return TPackedSaveMemory<TypeIterator, TSaveMemory<T1, TypeSaveMemory> >
         (packer.iter, packer.end, *this);
   }

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
   template <class TypeIterator>
   TPackedSaveMemory<TypeIterator, SaveMemory>
      operator<<(MergeBranches::TPacker<TypeIterator> packer)
      {  return TPackedSaveMemory<TypeIterator, SaveMemory>(packer.iter, packer.end, *this); }
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

template <typename TypeIterator, class TypeMergeMemory>
class TPackedMergeMemory;

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
   template <typename TypeIterator>
   TPackedMergeMemory<TypeIterator, TMergeMemory<T1, TypeMergeMemory> > operator>>(MergeBranches::TPacker<TypeIterator> packer);

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

template <typename TypeIterator, class TypeMergeMemory>
class TPackedMergeMemory {
  public:
   COL::TVector<typename TypeIterator::value_type> merge;
   TypeMergeMemory next;

   TPackedMergeMemory(TypeIterator iter, TypeIterator end, TypeMergeMemory nextArg)
      :  next(nextArg) {}
   TPackedMergeMemory(const TPackedMergeMemory<TypeIterator, TypeMergeMemory>&) = default;
   TPackedMergeMemory(TPackedMergeMemory<TypeIterator, TypeMergeMemory>&&) = default;

   template <typename T>
   TMergeMemory<T, TPackedMergeMemory<TypeIterator, TypeMergeMemory> > operator>>(T& t)
      {  return TMergeMemory<T, TPackedMergeMemory<TypeIterator, TypeMergeMemory> >(t, *this); }
   template <class TypeIteratorArgument>
   TPackedMergeMemory<TypeIteratorArgument, TPackedMergeMemory<TypeIterator, TypeMergeMemory> >
      operator>>(MergeBranches::TPacker<TypeIteratorArgument> packer)
      {  return TPackedMergeMemory<TypeIteratorArgument, TPackedMergeMemory<TypeIterator, TypeMergeMemory> >
            (packer.iter, packer.end, *this);
      }

   TPackedMergeMemory<TypeIterator, TypeMergeMemory>& operator>>(BaseExecutionPath::end) { return *this; }
   TPackedMergeMemory<TypeIterator, TypeMergeMemory>& setCurrentComplete(bool isComplete)
      {  next.setCurrentComplete(isComplete); return *this; }
   TypeMergeMemory& operator<<(MergeBranches::TPacker<TypeIterator>&& packer)
      {  int count = packer.end - packer.iter;
         if (next.isComplete()) {
            auto iter = packer.iter;
            if (next.isFirst()) {
               AssumeCondition(merge.count() == 0)
               merge.bookPlace(count);
               for (; iter != packer.end; ++iter) {
                  if (iter->optimizeValue()) {
                     merge.insertAtEnd(*iter);
                     if (merge.slast().doesSupportUnstableInLoop()) {
                        merge.slast().getSRealDomain().clearHolder();
                        merge.slast().getSError().clearHolder();
                     };
                  }
                  else
                     next.setCurrentComplete(false);
               };
            }
            else {
               for (int index = 0; index < count; ++index) {
                  if (iter->optimizeValue())
                     merge.referenceAt(index).mergeWith(*iter);
                  else
                     next.setCurrentComplete(false);
                  ++iter;
               }
            }
         }
         if (merge.count() > 0) {
            for (int index = 0; index < count; ++index) {
               *packer.iter = merge[index];
               ++packer.iter;
            }
         };
         return next;
      }
   bool isFirst() const { return next.isFirst(); }
   bool isComplete() const { return next.isComplete(); }
};

template <typename T1, class TypeMergeMemory>
template <typename TypeIterator>
inline
TPackedMergeMemory<TypeIterator, TMergeMemory<T1, TypeMergeMemory> >
TMergeMemory<T1, TypeMergeMemory>::operator>>(MergeBranches::TPacker<TypeIterator> packer)
   {  return TPackedMergeMemory<TypeIterator, TMergeMemory<T1, TypeMergeMemory> >
         (packer.iter, packer.end, *this);
   }

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
   template <typename TypeIterator>
   TPackedMergeMemory<TypeIterator, MergeMemory> operator>>(const MergeBranches::TPacker<TypeIterator>& packer)
      {  return TPackedMergeMemory<TypeIterator, MergeMemory>
            (packer.iter, packer.end, *this);
      }
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

template <typename TypeImplementation>
int tfinite(TypeImplementation val)
   {  AssumeUncalled return 0; }

template <> int tfinite(long double val);
template <> int tfinite(double val);
template <> int tfinite(float val);

template <typename TypeImplementation>
int tisfinite(TypeImplementation val)
   {  AssumeUncalled return 0; }

template <> int tisfinite(long double val);
template <> int tisfinite(double val);
template <> int tisfinite(float val);

template <typename TypeImplementation>
int tisnan(TypeImplementation val)
   {  AssumeUncalled return 0; }

template <> int tisnan(long double val);
template <> int tisnan(double val);
template <> int tisnan(float val);

template <typename TypeImplementation>
int tisinf(TypeImplementation val)
   {  AssumeUncalled return 0; }

template <> int tisinf(long double val);
template <> int tisinf(double val);
template <> int tisinf(float val);

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
class TInstrumentedFloatZonotope : public TFloatZonotope<ExecutionPath, USizeMantissa, USizeExponent, TypeImplementation> {
  private:
   typedef TFloatZonotope<ExecutionPath, USizeMantissa, USizeExponent, TypeImplementation> inherited;
   typedef TInstrumentedFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation> thisType;

  protected:
   friend class TMergeBranches<ExecutionPath>;

  public:
   typedef thisType InstrumentedAffineType;
   typedef DAffine::MergeBranches MergeBranches;
   struct ValueFromString {}; 

   TInstrumentedFloatZonotope() = default;
   TInstrumentedFloatZonotope(const char* value, ValueFromString)
      {  STG::IOObject::ISBase* in = ExecutionPath::acquireConstantStream(value);
         inherited::initFrom(*in);
         ExecutionPath::releaseConstantStream(in);
      }
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
   TInstrumentedFloatZonotope(short int value) : inherited(value) {}
   TInstrumentedFloatZonotope(int value) : inherited(value) {}
   TInstrumentedFloatZonotope(long int value) : inherited(value) {}
   TInstrumentedFloatZonotope(unsigned short value) : inherited(value) {}
   TInstrumentedFloatZonotope(unsigned int value) : inherited(value) {}
   TInstrumentedFloatZonotope(unsigned long value) : inherited(value) {}
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

   void continuousFlow(std::function<void (thisType& val)> funAssign)
      {  // see float_diagnosis.h FLOAT_SPLIT_ALL FLOAT_MERGE_ALL
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
         auto mergeMemory = MergeMemory() >> *this >> BaseExecutionPath::end();
         auto saveMemory = SaveMemory() << *this << BaseExecutionPath::end();
         const char* sourceFile = __FILE__;
         int sourceLine = __LINE__;
         auto oldSourceInfo = BaseFloatAffine::querySplitInfo();
         bool doesIterate;
         do {
            try {
               BaseFloatAffine::splitBranches(sourceFile, sourceLine);
               funAssign(*this);
               isCompleteFlow = MergeBranches(sourceFile, sourceLine) << *this << BaseExecutionPath::end();
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
            doesIterate = mergeMemory.setCurrentComplete(isCompleteFlow) << *this << BaseExecutionPath::end();
            if (doesIterate)
               doesIterate = !(saveMemory.setCurrentResult(pathExplorer.isFinished(ExecutionPath::queryMode(oldPathExplorer))) >> *this >> BaseExecutionPath::end());
         } while (doesIterate);
         ExecutionPath::setFollowFlow(oldDoesFollow, oldInputTraceFile,
               oldSynchronisationFile, oldSynchronisationLine);
         ExecutionPath::setCurrentPathExplorer(oldPathExplorer);
         inherited::setSupportUnstableInLoop(oldSupportUnstableInLoop);
         BaseFloatAffine::splitBranches(oldSourceInfo.first, oldSourceInfo.second);
         if (mergeMemory.isFirst())
            ExecutionPath::throwEmptyBranch(true);
      }
   void continuousFlow(std::function<void (thisType& val, const thisType& arg)> funAssign, const thisType& anarg)
      {  // see float_diagnosis.h FLOAT_SPLIT_ALL FLOAT_MERGE_ALL
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
         thisType& arg = const_cast<thisType&>(anarg);
         auto mergeMemory = MergeMemory() >> arg >> *this >> BaseExecutionPath::end();
         auto saveMemory = SaveMemory() << *this << arg << BaseExecutionPath::end();
         const char* sourceFile = __FILE__;
         int sourceLine = __LINE__;
         auto oldSourceInfo = BaseFloatAffine::querySplitInfo();
         bool doesIterate;
         do {
            try {
               BaseFloatAffine::splitBranches(sourceFile, sourceLine);
               funAssign(*this, anarg);
               isCompleteFlow = MergeBranches(sourceFile, sourceLine) << *this << arg << BaseExecutionPath::end();
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
            doesIterate = mergeMemory.setCurrentComplete(isCompleteFlow) << *this << arg << BaseExecutionPath::end();
            if (doesIterate)
               doesIterate = !(saveMemory.setCurrentResult(pathExplorer.isFinished(ExecutionPath::queryMode(oldPathExplorer))) >> arg >> *this >> BaseExecutionPath::end());
         } while (doesIterate);
         ExecutionPath::setFollowFlow(oldDoesFollow, oldInputTraceFile,
               oldSynchronisationFile, oldSynchronisationLine);
         ExecutionPath::setCurrentPathExplorer(oldPathExplorer);
         inherited::setSupportUnstableInLoop(oldSupportUnstableInLoop);
         BaseFloatAffine::splitBranches(oldSourceInfo.first, oldSourceInfo.second);
         if (mergeMemory.isFirst())
            ExecutionPath::throwEmptyBranch(true);
      }
   void continuousFlow(std::function<void (thisType& val, const thisType& fstarg, const thisType& sndarg)> funAssign,
         const thisType& afstarg, const thisType& asndarg)
      {  // see float_diagnosis.h FLOAT_SPLIT_ALL FLOAT_MERGE_ALL
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
         thisType& fstarg = const_cast<thisType&>(afstarg);
         thisType& sndarg = const_cast<thisType&>(asndarg);
         auto mergeMemory = MergeMemory() >> sndarg >> fstarg >> *this >> BaseExecutionPath::end();
         auto saveMemory = SaveMemory() << *this << fstarg << sndarg << BaseExecutionPath::end();
         const char* sourceFile = __FILE__;
         int sourceLine = __LINE__;
         auto oldSourceInfo = BaseFloatAffine::querySplitInfo();
         bool doesIterate;
         do {
            try {
               BaseFloatAffine::splitBranches(sourceFile, sourceLine);
               funAssign(*this, afstarg, asndarg);
               isCompleteFlow = MergeBranches(sourceFile, sourceLine) << *this << fstarg << sndarg << BaseExecutionPath::end();
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
            doesIterate = mergeMemory.setCurrentComplete(isCompleteFlow) << *this << fstarg << sndarg << BaseExecutionPath::end();
            if (doesIterate)
               doesIterate = !(saveMemory.setCurrentResult(pathExplorer.isFinished(ExecutionPath::queryMode(oldPathExplorer))) >> sndarg >> fstarg >> *this >> BaseExecutionPath::end());
         } while (doesIterate);
         ExecutionPath::setFollowFlow(oldDoesFollow, oldInputTraceFile,
               oldSynchronisationFile, oldSynchronisationLine);
         ExecutionPath::setCurrentPathExplorer(oldPathExplorer);
         inherited::setSupportUnstableInLoop(oldSupportUnstableInLoop);
         BaseFloatAffine::splitBranches(oldSourceInfo.first, oldSourceInfo.second);
         if (mergeMemory.isFirst())
            ExecutionPath::throwEmptyBranch(true);
      }
   thisType abs() const
      {  auto result(*this);
         result.continuousFlow([](thisType& val){ if (val < 0) val.oppositeAssign(); });
         return result;
      }
   thisType min(const thisType& asource) const
      {  auto result(*this), source(asource); // source should gain back its original value
         result.continuousFlow([](thisType& val, const thisType& source)
               { if (val > source) val = source; }, source);
         return result;
      }
   thisType min(thisType&& source) const
      {  auto result(*this);
         result.continuousFlow([](thisType& val, const thisType& source)
               { if (val > source) val = source; }, source);
         return result;
      }
   thisType min(TypeImplementation asource) const
      {  auto result(*this);
         thisType source(asource);
         result.continuousFlow([](thisType& val, const thisType& source)
               { if (val > source) val = source; }, source);
         return result;
      }
   thisType max(const thisType& asource) const
      {  auto result(*this), source(asource); // source should gain back its original value
         result.continuousFlow([](thisType& val, const thisType& source)
               { if (val < source) val = source; }, source);
         return result;
      }
   thisType max(thisType&& source) const
      {  auto result(*this);
         result.continuousFlow([](thisType& val, const thisType& source)
               { if (val < source) val = source; }, source);
         return result;
      }
   thisType max(TypeImplementation asource) const
      {  auto result(*this);
         thisType source(asource);
         result.continuousFlow([](thisType& val, const thisType& source)
               { if (val < source) val = source; }, source);
         return result;
      }
   thisType median(const thisType& afst, const thisType& asnd) const
      {  auto result(*this), fst(afst), snd(asnd);
         result.continuousFlow(
               [](thisType& val, const thisType& fst, const thisType& snd)
                  {  if (val <= fst) {
                        if (fst <= snd)
                           val = fst;
                        // snd < fst 
                        else if (val <= snd)
                           val = snd;
                        // snd < val <= fst
                     }
                     // fst < val
                     else if (snd <= fst)
                        val = fst;
                     // fst < snd
                     else if (val > snd)
                        val = snd;
                  }, fst, snd);
         return result;
      }
   thisType median(TypeImplementation afst, const thisType& asnd) const
      {  auto result(*this), snd(asnd);
         thisType fst(afst);
         result.continuousFlow(
               [](thisType& val, const thisType& fst, const thisType& snd)
                  {  if (val <= fst) {
                        if (fst <= snd)
                           val = fst;
                        // snd < fst 
                        else if (val <= snd)
                           val = snd;
                        // snd < val <= fst
                     }
                     // fst < val
                     else if (snd <= fst)
                        val = fst;
                     // fst < snd
                     else if (val > snd)
                        val = snd;
                  }, fst, snd);
         return result;
      }
   thisType median(const thisType& afst, TypeImplementation asnd) const
      {  auto result(*this), fst(afst);
         thisType snd(asnd);
         result.continuousFlow(
               [](thisType& val, const thisType& fst, const thisType& snd)
                  {  if (val <= fst) {
                        if (fst <= snd)
                           val = fst;
                        // snd < fst 
                        else if (val <= snd)
                           val = snd;
                        // snd < val <= fst
                     }
                     // fst < val
                     else if (snd <= fst)
                        val = fst;
                     // fst < snd
                     else if (val > snd)
                        val = snd;
                  }, fst, snd);
         return result;
      }
   thisType median(TypeImplementation afst, TypeImplementation asnd) const
      {  auto result(*this);
         thisType fst(afst), snd(asnd);
         result.continuousFlow(
               [](thisType& val, const thisType& fst, const thisType& snd)
                  {  if (val <= fst) {
                        if (fst <= snd)
                           val = fst;
                        // snd < fst 
                        else if (val <= snd)
                           val = snd;
                        // snd < val <= fst
                     }
                     // fst < val
                     else if (snd <= fst)
                        val = fst;
                     // fst < snd
                     else if (val > snd)
                        val = snd;
                  }, fst, snd);
         return result;
      }

   void lightPersist(const char* prefix) const { inherited::lightPersist(*this, prefix); }
   void persist(const char* prefix) const { inherited::persist(*this, prefix); }

   friend std::ostream& operator<<(std::ostream& out, const thisType& source)
      {  return out << source.asImplementation(); }
   friend std::istream& operator>>(std::istream& in, thisType& source)
      {  TypeImplementation val;
         in >> val;
         source = thisType(val);
         return in;
      }

   friend thisType sqrt(const thisType& source)
      {  thisType result(source); result.sqrtAssign(); return result; }
   friend thisType sqrt(thisType&& source)
      {  thisType result(std::forward<thisType>(source)); result.sqrtAssign(); return result; }
   friend thisType sin(const thisType& source)
      {  thisType result(source); result.sinAssign(); return result; }
   friend thisType sin(thisType&& source)
      {  thisType result(std::forward<thisType>(source)); result.sinAssign(); return result; }
   friend thisType cos(const thisType& source)
      {  thisType result(source); result.cosAssign(); return result; }
   friend thisType cos(thisType&& source)
      {  thisType result(std::forward<thisType>(source)); result.cosAssign(); return result; }
   friend thisType asin(const thisType& source)
      {  thisType result(source); result.asinAssign(); return result; }
   friend thisType asin(thisType&& source)
      {  thisType result(std::forward<thisType>(source)); result.asinAssign(); return result; }
   friend thisType acos(const thisType& source)
      {  thisType result(source); result.acosAssign(); return result; }
   friend thisType acos(thisType&& source)
      {  thisType result(std::forward<thisType>(source)); result.acosAssign(); return result; }
   friend thisType tan(const thisType& source)
      {  thisType result(source); result.tanAssign(); return result; }
   friend thisType tan(thisType&& source)
      {  thisType result(std::forward<thisType>(source)); result.tanAssign(); return result; }
   friend thisType atan(const thisType& source)
      {  thisType result(source); result.atanAssign(); return result; }
   friend thisType atan(thisType&& source)
      {  thisType result(std::forward<thisType>(source)); result.atanAssign(); return result; }
   friend thisType exp(const thisType& source)
      {  thisType result(source); result.expAssign(); return result; }
   friend thisType exp(thisType&& source)
      {  thisType result(std::forward<thisType>(source)); result.expAssign(); return result; }
   friend thisType log(const thisType& source)
      {  thisType result(source); result.logAssign(); return result; }
   friend thisType log(thisType&& source)
      {  thisType result(std::forward<thisType>(source)); result.logAssign(); return result; }
   friend thisType log2(const thisType& source)
      {  thisType result(source); result.logAssign(); result.divAssign(log(thisType(2.0)), Equation::PCSourceXValue); return result; }
   friend thisType log2(thisType&& source)
      {  thisType result(std::forward<thisType>(source)); result.logAssign(); result.divAssign(log(thisType(2.0)), Equation::PCSourceXValue); return result; }
   friend thisType exp2(const thisType& fst)
      {  thisType result = 2.0; result.powAssign(fst, Equation::PCSourceRValue); return result; }
   friend thisType exp2(thisType&& fst)
      {  thisType result = 2.0; result.powAssign(fst, Equation::PCSourceXValue); return result; }
   friend thisType log10(const thisType& source)
      {  thisType result(source); result.log10Assign(); return result; }
   friend thisType log10(thisType&& source)
      {  thisType result(std::forward<thisType>(source)); result.log10Assign(); return result; }
   friend thisType pow(const thisType& source, const thisType& value)
      {  thisType result(source); result.powAssign(value, Equation::PCSourceRValue); return result; }
   friend thisType pow(const thisType& source, thisType&& value)
      {  thisType result(source); result.powAssign(value, Equation::PCSourceXValue); return result; }
   friend thisType pow(thisType&& source, const thisType& value)
      {  thisType result(std::forward<thisType>(source)); result.powAssign(value, Equation::PCSourceRValue); return result; }
   friend thisType pow(thisType&& source, thisType&& value)
      {  thisType result(std::forward<thisType>(source)); result.powAssign(value, Equation::PCSourceXValue); return result; }

   template <int USizeMantissaArgument, int USizeExponentArgument, typename TypeImplementationArgument>
   friend thisType pow(const TInstrumentedFloatZonotope<USizeMantissaArgument, USizeExponentArgument, TypeImplementationArgument>& source, const thisType& value)
      {  thisType result(source); result.powAssign(value, Equation::PCSourceRValue); return result; }
   template <int USizeMantissaArgument, int USizeExponentArgument, typename TypeImplementationArgument>
   friend thisType pow(TInstrumentedFloatZonotope<USizeMantissaArgument, USizeExponentArgument, TypeImplementationArgument>&& source, const thisType& value)
      {  thisType result(std::forward<TInstrumentedFloatZonotope<USizeMantissaArgument, USizeExponentArgument, TypeImplementationArgument>>(source));
         result.powAssign(value, Equation::PCSourceRValue); return result;
      }
   template <int USizeMantissaArgument, int USizeExponentArgument, typename TypeImplementationArgument>
   friend thisType pow(const TInstrumentedFloatZonotope<USizeMantissaArgument, USizeExponentArgument, TypeImplementationArgument>& source, thisType&& value)
      {  thisType result(source); result.powAssign(value, Equation::PCSourceXValue); return result; }
   template <int USizeMantissaArgument, int USizeExponentArgument, typename TypeImplementationArgument>
   friend thisType pow(TInstrumentedFloatZonotope<USizeMantissaArgument, USizeExponentArgument, TypeImplementationArgument>&& source, thisType&& value)
      {  thisType result(std::forward<TInstrumentedFloatZonotope<USizeMantissaArgument, USizeExponentArgument, TypeImplementationArgument>>(source));
         result.powAssign(value, Equation::PCSourceXValue); return result;
      }
   friend thisType pow(long double source, const thisType& value)
      {  thisType result(source); result.powAssign(value, Equation::PCSourceRValue); return result; }
   friend thisType pow(double source, const thisType& value)
      {  thisType result(source); result.powAssign(value, Equation::PCSourceRValue); return result; }
   friend thisType pow(float source, const thisType& value)
      {  thisType result(source); result.powAssign(value, Equation::PCSourceRValue); return result; }
   friend thisType pow(unsigned long source, const thisType& value)
      {  thisType result(source); result.powAssign(value, Equation::PCSourceRValue); return result; }
   friend thisType pow(long source, const thisType& value)
      {  thisType result(source); result.powAssign(value, Equation::PCSourceRValue); return result; }
   friend thisType pow(unsigned int source, const thisType& value)
      {  thisType result(source); result.powAssign(value, Equation::PCSourceRValue); return result; }
   friend thisType pow(int source, const thisType& value)
      {  thisType result(source); result.powAssign(value, Equation::PCSourceRValue); return result; }
   friend thisType pow(unsigned short source, const thisType& value)
      {  thisType result(source); result.powAssign(value, Equation::PCSourceRValue); return result; }
   friend thisType pow(short source, const thisType& value)
      {  thisType result(source); result.powAssign(value, Equation::PCSourceRValue); return result; }
   friend thisType pow(long double source, thisType&& value)
      {  thisType result(source); result.powAssign(value, Equation::PCSourceXValue); return result; }
   friend thisType pow(double source, thisType&& value)
      {  thisType result(source); result.powAssign(value, Equation::PCSourceXValue); return result; }
   friend thisType pow(float source, thisType&& value)
      {  thisType result(source); result.powAssign(value, Equation::PCSourceXValue); return result; }
   friend thisType pow(unsigned long source, thisType&& value)
      {  thisType result(source); result.powAssign(value, Equation::PCSourceXValue); return result; }
   friend thisType pow(long source, thisType&& value)
      {  thisType result(source); result.powAssign(value, Equation::PCSourceXValue); return result; }
   friend thisType pow(unsigned int source, thisType&& value)
      {  thisType result(source); result.powAssign(value, Equation::PCSourceXValue); return result; }
   friend thisType pow(int source, thisType&& value)
      {  thisType result(source); result.powAssign(value, Equation::PCSourceXValue); return result; }
   friend thisType pow(unsigned short source, thisType&& value)
      {  thisType result(source); result.powAssign(value, Equation::PCSourceXValue); return result; }
   friend thisType pow(short source, thisType&& value)
      {  thisType result(source); result.powAssign(value, Equation::PCSourceXValue); return result; }
   template <int USizeMantissaArgument, int USizeExponentArgument, typename TypeImplementationArgument>
   friend thisType pow(const thisType& source, const TInstrumentedFloatZonotope<USizeMantissaArgument, USizeExponentArgument, TypeImplementationArgument>& value)
      {  thisType result(source); result.powAssign(thisType(value), Equation::PCSourceXValue); return result; }
   template <int USizeMantissaArgument, int USizeExponentArgument, typename TypeImplementationArgument>
   friend thisType pow(thisType&& source, const TInstrumentedFloatZonotope<USizeMantissaArgument, USizeExponentArgument, TypeImplementationArgument>& value)
      {  thisType result(std::forward<thisType>(source)); result.powAssign(thisType(value), Equation::PCSourceXValue); return result; }
   template <int USizeMantissaArgument, int USizeExponentArgument, typename TypeImplementationArgument>
   friend thisType pow(const thisType& source, TInstrumentedFloatZonotope<USizeMantissaArgument, USizeExponentArgument, TypeImplementationArgument>&& value)
      {  thisType result(source); result.powAssign(thisType(std::forward<TInstrumentedFloatZonotope<USizeMantissaArgument, USizeExponentArgument, TypeImplementationArgument> >(value)), Equation::PCSourceXValue); return result; }
   template <int USizeMantissaArgument, int USizeExponentArgument, typename TypeImplementationArgument>
   friend thisType pow(thisType&& source, TInstrumentedFloatZonotope<USizeMantissaArgument, USizeExponentArgument, TypeImplementationArgument>&& value)
      {  thisType result(std::forward<thisType>(source)); result.powAssign(thisType(std::forward<TInstrumentedFloatZonotope<USizeMantissaArgument, USizeExponentArgument, TypeImplementationArgument> >(value)), Equation::PCSourceXValue); return result; }
   friend thisType pow(const thisType& source, long double value)
      {  thisType result(source); result.powAssign(thisType(value), Equation::PCSourceXValue); return result; }
   friend thisType pow(const thisType& source, double value)
      {  thisType result(source); result.powAssign(thisType(value), Equation::PCSourceXValue); return result; }
   friend thisType pow(const thisType& source, float value)
      {  thisType result(source); result.powAssign(thisType(value), Equation::PCSourceXValue); return result; }
   friend thisType pow(const thisType& source, unsigned long value)
      {  thisType result(source); result.powAssign(thisType(value), Equation::PCSourceXValue); return result; }
   friend thisType pow(const thisType& source, long value)
      {  thisType result(source); result.powAssign(thisType(value), Equation::PCSourceXValue); return result; }
   friend thisType pow(const thisType& source, unsigned int value)
      {  thisType result(source); result.powAssign(thisType(value), Equation::PCSourceXValue); return result; }
   friend thisType pow(const thisType& source, int value)
      {  thisType result(source); result.powAssign(thisType(value), Equation::PCSourceXValue); return result; }
   friend thisType pow(const thisType& source, unsigned short value)
      {  thisType result(source); result.powAssign(thisType(value), Equation::PCSourceXValue); return result; }
   friend thisType pow(const thisType& source, short value)
      {  thisType result(source); result.powAssign(thisType(value), Equation::PCSourceXValue); return result; }
   friend thisType pow(thisType&& source, long double value)
      {  thisType result(std::forward<thisType>(source)); result.powAssign(thisType(value), Equation::PCSourceXValue); return result; }
   friend thisType pow(thisType&& source, double value)
      {  thisType result(std::forward<thisType>(source)); result.powAssign(thisType(value), Equation::PCSourceXValue); return result; }
   friend thisType pow(thisType&& source, float value)
      {  thisType result(std::forward<thisType>(source)); result.powAssign(thisType(value), Equation::PCSourceXValue); return result; }
   friend thisType pow(thisType&& source, unsigned long value)
      {  thisType result(std::forward<thisType>(source)); result.powAssign(thisType(value), Equation::PCSourceXValue); return result; }
   friend thisType pow(thisType&& source, long value)
      {  thisType result(std::forward<thisType>(source)); result.powAssign(thisType(value), Equation::PCSourceXValue); return result; }
   friend thisType pow(thisType&& source, unsigned int value)
      {  thisType result(std::forward<thisType>(source)); result.powAssign(thisType(value), Equation::PCSourceXValue); return result; }
   friend thisType pow(thisType&& source, int value)
      {  thisType result(std::forward<thisType>(source)); result.powAssign(thisType(value), Equation::PCSourceXValue); return result; }
   friend thisType pow(thisType&& source, unsigned short value)
      {  thisType result(std::forward<thisType>(source)); result.powAssign(thisType(value), Equation::PCSourceXValue); return result; }
   friend thisType pow(thisType&& source, short value)
      {  thisType result(std::forward<thisType>(source)); result.powAssign(thisType(value), Equation::PCSourceXValue); return result; }
   friend thisType powf(const thisType& source, const thisType& value)
      {  thisType result(source); result.powAssign(value, Equation::PCSourceRValue); return result; }
   friend thisType powf(const thisType& source, thisType&& value)
      {  thisType result(source); result.powAssign(value, Equation::PCSourceXValue); return result; }
   friend thisType powf(thisType&& source, const thisType& value)
      {  thisType result(std::forward<thisType>(source)); result.powAssign(value, Equation::PCSourceRValue); return result; }
   friend thisType powf(thisType&& source, thisType&& value)
      {  thisType result(std::forward<thisType>(source)); result.powAssign(value, Equation::PCSourceXValue); return result; }
   template <int USizeMantissaArgument, int USizeExponentArgument, typename TypeImplementationArgument>
   friend thisType powf(const TInstrumentedFloatZonotope<USizeMantissaArgument, USizeExponentArgument, TypeImplementationArgument>& source, const thisType& value)
      {  thisType result(source); result.powAssign(value, Equation::PCSourceRValue); return result; }
   template <int USizeMantissaArgument, int USizeExponentArgument, typename TypeImplementationArgument>
   friend thisType powf(const TInstrumentedFloatZonotope<USizeMantissaArgument, USizeExponentArgument, TypeImplementationArgument>& source, thisType&& value)
      {  thisType result(source); result.powAssign(value, Equation::PCSourceXValue); return result; }
   template <int USizeMantissaArgument, int USizeExponentArgument, typename TypeImplementationArgument>
   friend thisType powf(TInstrumentedFloatZonotope<USizeMantissaArgument, USizeExponentArgument, TypeImplementationArgument>&& source, const thisType& value)
      {  thisType result(std::forward<TInstrumentedFloatZonotope<USizeMantissaArgument, USizeExponentArgument, TypeImplementationArgument> >(source));
         result.powAssign(value, Equation::PCSourceRValue); return result;
      }
   template <int USizeMantissaArgument, int USizeExponentArgument, typename TypeImplementationArgument>
   friend thisType powf(TInstrumentedFloatZonotope<USizeMantissaArgument, USizeExponentArgument, TypeImplementationArgument>&& source, thisType&& value)
      {  thisType result(std::forward<TInstrumentedFloatZonotope<USizeMantissaArgument, USizeExponentArgument, TypeImplementationArgument> >(source));
         result.powAssign(value, Equation::PCSourceXValue); return result;
      }
   friend thisType powf(long double source, const thisType& value)
      {  thisType result(source); result.powAssign(value, Equation::PCSourceRValue); return result; }
   friend thisType powf(double source, const thisType& value)
      {  thisType result(source); result.powAssign(value, Equation::PCSourceRValue); return result; }
   friend thisType powf(float source, const thisType& value)
      {  thisType result(source); result.powAssign(value, Equation::PCSourceRValue); return result; }
   friend thisType powf(unsigned long source, const thisType& value)
      {  thisType result(source); result.powAssign(value, Equation::PCSourceRValue); return result; }
   friend thisType powf(long source, const thisType& value)
      {  thisType result(source); result.powAssign(value, Equation::PCSourceRValue); return result; }
   friend thisType powf(unsigned int source, const thisType& value)
      {  thisType result(source); result.powAssign(value, Equation::PCSourceRValue); return result; }
   friend thisType powf(int source, const thisType& value)
      {  thisType result(source); result.powAssign(value, Equation::PCSourceRValue); return result; }
   friend thisType powf(unsigned short source, const thisType& value)
      {  thisType result(source); result.powAssign(value, Equation::PCSourceRValue); return result; }
   friend thisType powf(short source, const thisType& value)
      {  thisType result(source); result.powAssign(value, Equation::PCSourceRValue); return result; }
   friend thisType powf(long double source, thisType&& value)
      {  thisType result(source); result.powAssign(value, Equation::PCSourceXValue); return result; }
   friend thisType powf(double source, thisType&& value)
      {  thisType result(source); result.powAssign(value, Equation::PCSourceXValue); return result; }
   friend thisType powf(float source, thisType&& value)
      {  thisType result(source); result.powAssign(value, Equation::PCSourceXValue); return result; }
   friend thisType powf(unsigned long source, thisType&& value)
      {  thisType result(source); result.powAssign(value, Equation::PCSourceXValue); return result; }
   friend thisType powf(long source, thisType&& value)
      {  thisType result(source); result.powAssign(value, Equation::PCSourceXValue); return result; }
   friend thisType powf(unsigned int source, thisType&& value)
      {  thisType result(source); result.powAssign(value, Equation::PCSourceXValue); return result; }
   friend thisType powf(int source, thisType&& value)
      {  thisType result(source); result.powAssign(value, Equation::PCSourceXValue); return result; }
   friend thisType powf(unsigned short source, thisType&& value)
      {  thisType result(source); result.powAssign(value, Equation::PCSourceXValue); return result; }
   friend thisType powf(short source, thisType&& value)
      {  thisType result(source); result.powAssign(value, Equation::PCSourceXValue); return result; }
   template <int USizeMantissaArgument, int USizeExponentArgument, typename TypeImplementationArgument>
   friend thisType powf(const thisType& source, const TInstrumentedFloatZonotope<USizeMantissaArgument, USizeExponentArgument, TypeImplementationArgument>& value)
      {  thisType result(source); result.powAssign(thisType(value), Equation::PCSourceXValue); return result; }
   template <int USizeMantissaArgument, int USizeExponentArgument, typename TypeImplementationArgument>
   friend thisType powf(thisType&& source, const TInstrumentedFloatZonotope<USizeMantissaArgument, USizeExponentArgument, TypeImplementationArgument>& value)
      {  thisType result(std::forward<thisType>(source)); result.powAssign(thisType(value), Equation::PCSourceXValue); return result; }
   template <int USizeMantissaArgument, int USizeExponentArgument, typename TypeImplementationArgument>
   friend thisType powf(const thisType& source, TInstrumentedFloatZonotope<USizeMantissaArgument, USizeExponentArgument, TypeImplementationArgument>&& value)
      {  thisType result(source); result.powAssign(thisType(std::forward<TInstrumentedFloatZonotope<USizeMantissaArgument, USizeExponentArgument, TypeImplementationArgument> >(value)), Equation::PCSourceXValue); return result; }
   template <int USizeMantissaArgument, int USizeExponentArgument, typename TypeImplementationArgument>
   friend thisType powf(thisType&& source, TInstrumentedFloatZonotope<USizeMantissaArgument, USizeExponentArgument, TypeImplementationArgument>&& value)
      {  thisType result(std::forward<thisType>(source)); result.powAssign(thisType(std::forward<TInstrumentedFloatZonotope<USizeMantissaArgument, USizeExponentArgument, TypeImplementationArgument> >(value)), Equation::PCSourceXValue); return result; }
   friend thisType powf(const thisType& source, long double value)
      {  thisType result(source); result.powAssign(thisType(value), Equation::PCSourceXValue); return result; }
   friend thisType powf(const thisType& source, double value)
      {  thisType result(source); result.powAssign(thisType(value), Equation::PCSourceXValue); return result; }
   friend thisType powf(const thisType& source, float value)
      {  thisType result(source); result.powAssign(thisType(value), Equation::PCSourceXValue); return result; }
   friend thisType powf(const thisType& source, unsigned long value)
      {  thisType result(source); result.powAssign(thisType(value), Equation::PCSourceXValue); return result; }
   friend thisType powf(const thisType& source, long value)
      {  thisType result(source); result.powAssign(thisType(value), Equation::PCSourceXValue); return result; }
   friend thisType powf(const thisType& source, unsigned int value)
      {  thisType result(source); result.powAssign(thisType(value), Equation::PCSourceXValue); return result; }
   friend thisType powf(const thisType& source, int value)
      {  thisType result(source); result.powAssign(thisType(value), Equation::PCSourceXValue); return result; }
   friend thisType powf(const thisType& source, unsigned short value)
      {  thisType result(source); result.powAssign(thisType(value), Equation::PCSourceXValue); return result; }
   friend thisType powf(const thisType& source, short value)
      {  thisType result(source); result.powAssign(thisType(value), Equation::PCSourceXValue); return result; }
   friend thisType powf(thisType&& source, long double value)
      {  thisType result(std::forward<thisType>(source)); result.powAssign(thisType(value), Equation::PCSourceXValue); return result; }
   friend thisType powf(thisType&& source, double value)
      {  thisType result(std::forward<thisType>(source)); result.powAssign(thisType(value), Equation::PCSourceXValue); return result; }
   friend thisType powf(thisType&& source, float value)
      {  thisType result(std::forward<thisType>(source)); result.powAssign(thisType(value), Equation::PCSourceXValue); return result; }
   friend thisType powf(thisType&& source, unsigned long value)
      {  thisType result(std::forward<thisType>(source)); result.powAssign(thisType(value), Equation::PCSourceXValue); return result; }
   friend thisType powf(thisType&& source, long value)
      {  thisType result(std::forward<thisType>(source)); result.powAssign(thisType(value), Equation::PCSourceXValue); return result; }
   friend thisType powf(thisType&& source, unsigned int value)
      {  thisType result(std::forward<thisType>(source)); result.powAssign(thisType(value), Equation::PCSourceXValue); return result; }
   friend thisType powf(thisType&& source, int value)
      {  thisType result(std::forward<thisType>(source)); result.powAssign(thisType(value), Equation::PCSourceXValue); return result; }
   friend thisType powf(thisType&& source, unsigned short value)
      {  thisType result(std::forward<thisType>(source)); result.powAssign(thisType(value), Equation::PCSourceXValue); return result; }
   friend thisType powf(thisType&& source, short value)
      {  thisType result(std::forward<thisType>(source)); result.powAssign(thisType(value), Equation::PCSourceXValue); return result; }
   friend thisType atan2(const thisType& source, const thisType& value)
      {  thisType result(source); result.atan2Assign(value, Equation::PCSourceRValue); return result; }
   friend thisType atan2(const thisType& source, thisType&& value)
      {  thisType result(source); result.atan2Assign(value, Equation::PCSourceXValue); return result; }
   friend thisType atan2(thisType&& source, const thisType& value)
      {  thisType result(std::forward<thisType>(source)); result.atan2Assign(value, Equation::PCSourceRValue); return result; }
   friend thisType atan2(thisType&& source, thisType&& value)
      {  thisType result(std::forward<thisType>(source)); result.atan2Assign(value, Equation::PCSourceXValue); return result; }
   template <int USizeMantissaArgument, int USizeExponentArgument, typename TypeImplementationArgument>
   friend thisType atan2(const TInstrumentedFloatZonotope<USizeMantissaArgument, USizeExponentArgument, TypeImplementationArgument>& source, const thisType& value)
      {  thisType result(source); result.atan2Assign(value, Equation::PCSourceRValue); return result; }
   template <int USizeMantissaArgument, int USizeExponentArgument, typename TypeImplementationArgument>
   friend thisType atan2(const TInstrumentedFloatZonotope<USizeMantissaArgument, USizeExponentArgument, TypeImplementationArgument>& source, thisType&& value)
      {  thisType result(source); result.atan2Assign(value, Equation::PCSourceXValue); return result; }
   template <int USizeMantissaArgument, int USizeExponentArgument, typename TypeImplementationArgument>
   friend thisType atan2(TInstrumentedFloatZonotope<USizeMantissaArgument, USizeExponentArgument, TypeImplementationArgument>&& source, const thisType& value)
      {  thisType result(std::forward<TInstrumentedFloatZonotope<USizeMantissaArgument, USizeExponentArgument, TypeImplementationArgument> >(source)); result.atan2Assign(value, Equation::PCSourceRValue); return result; }
   template <int USizeMantissaArgument, int USizeExponentArgument, typename TypeImplementationArgument>
   friend thisType atan2(TInstrumentedFloatZonotope<USizeMantissaArgument, USizeExponentArgument, TypeImplementationArgument>&& source, thisType&& value)
      {  thisType result(std::forward<TInstrumentedFloatZonotope<USizeMantissaArgument, USizeExponentArgument, TypeImplementationArgument> >(source)); result.atan2Assign(value, Equation::PCSourceXValue); return result; }
   friend thisType atan2(long double source, const thisType& value)
      {  thisType result(source); result.atan2Assign(value, Equation::PCSourceRValue); return result; }
   friend thisType atan2(double source, const thisType& value)
      {  thisType result(source); result.atan2Assign(value, Equation::PCSourceRValue); return result; }
   friend thisType atan2(float source, const thisType& value)
      {  thisType result(source); result.atan2Assign(value, Equation::PCSourceRValue); return result; }
   friend thisType atan2(unsigned long source, const thisType& value)
      {  thisType result(source); result.atan2Assign(value, Equation::PCSourceRValue); return result; }
   friend thisType atan2(long source, const thisType& value)
      {  thisType result(source); result.atan2Assign(value, Equation::PCSourceRValue); return result; }
   friend thisType atan2(unsigned int source, const thisType& value)
      {  thisType result(source); result.atan2Assign(value, Equation::PCSourceRValue); return result; }
   friend thisType atan2(int source, const thisType& value)
      {  thisType result(source); result.atan2Assign(value, Equation::PCSourceRValue); return result; }
   friend thisType atan2(unsigned short source, const thisType& value)
      {  thisType result(source); result.atan2Assign(value, Equation::PCSourceRValue); return result; }
   friend thisType atan2(short source, const thisType& value)
      {  thisType result(source); result.atan2Assign(value, Equation::PCSourceRValue); return result; }
   friend thisType atan2(long double source, thisType&& value)
      {  thisType result(source); result.atan2Assign(value, Equation::PCSourceXValue); return result; }
   friend thisType atan2(double source, thisType&& value)
      {  thisType result(source); result.atan2Assign(value, Equation::PCSourceXValue); return result; }
   friend thisType atan2(float source, thisType&& value)
      {  thisType result(source); result.atan2Assign(value, Equation::PCSourceXValue); return result; }
   friend thisType atan2(unsigned long source, thisType&& value)
      {  thisType result(source); result.atan2Assign(value, Equation::PCSourceXValue); return result; }
   friend thisType atan2(long source, thisType&& value)
      {  thisType result(source); result.atan2Assign(value, Equation::PCSourceXValue); return result; }
   friend thisType atan2(unsigned int source, thisType&& value)
      {  thisType result(source); result.atan2Assign(value, Equation::PCSourceXValue); return result; }
   friend thisType atan2(int source, thisType&& value)
      {  thisType result(source); result.atan2Assign(value, Equation::PCSourceXValue); return result; }
   friend thisType atan2(unsigned short source, thisType&& value)
      {  thisType result(source); result.atan2Assign(value, Equation::PCSourceXValue); return result; }
   friend thisType atan2(short source, thisType&& value)
      {  thisType result(source); result.atan2Assign(value, Equation::PCSourceXValue); return result; }
   template <int USizeMantissaArgument, int USizeExponentArgument, typename TypeImplementationArgument>
   friend thisType atan2(const thisType& source, const TInstrumentedFloatZonotope<USizeMantissaArgument, USizeExponentArgument, TypeImplementationArgument>& value)
      {  thisType result(source); result.atan2Assign(thisType(value), Equation::PCSourceXValue); return result; }
   template <int USizeMantissaArgument, int USizeExponentArgument, typename TypeImplementationArgument>
   friend thisType atan2(thisType&& source, const TInstrumentedFloatZonotope<USizeMantissaArgument, USizeExponentArgument, TypeImplementationArgument>& value)
      {  thisType result(std::forward<thisType>(source)); result.atan2Assign(thisType(value), Equation::PCSourceXValue); return result; }
   template <int USizeMantissaArgument, int USizeExponentArgument, typename TypeImplementationArgument>
   friend thisType atan2(const thisType& source, TInstrumentedFloatZonotope<USizeMantissaArgument, USizeExponentArgument, TypeImplementationArgument>&& value)
      {  thisType result(source); result.atan2Assign(thisType(std::forward<TInstrumentedFloatZonotope<USizeMantissaArgument, USizeExponentArgument, TypeImplementationArgument> >(value)), Equation::PCSourceXValue); return result; }
   template <int USizeMantissaArgument, int USizeExponentArgument, typename TypeImplementationArgument>
   friend thisType atan2(thisType&& source, TInstrumentedFloatZonotope<USizeMantissaArgument, USizeExponentArgument, TypeImplementationArgument>&& value)
      {  thisType result(std::forward<thisType>(source)); result.atan2Assign(thisType(std::forward<TInstrumentedFloatZonotope<USizeMantissaArgument, USizeExponentArgument, TypeImplementationArgument> >(value)), Equation::PCSourceXValue); return result; }
   friend thisType atan2(const thisType& source, long double value)
      {  thisType result(source); result.atan2Assign(thisType(value), Equation::PCSourceXValue); return result; }
   friend thisType atan2(const thisType& source, double value)
      {  thisType result(source); result.atan2Assign(thisType(value), Equation::PCSourceXValue); return result; }
   friend thisType atan2(const thisType& source, float value)
      {  thisType result(source); result.atan2Assign(thisType(value), Equation::PCSourceXValue); return result; }
   friend thisType atan2(const thisType& source, unsigned long value)
      {  thisType result(source); result.atan2Assign(thisType(value), Equation::PCSourceXValue); return result; }
   friend thisType atan2(const thisType& source, long value)
      {  thisType result(source); result.atan2Assign(thisType(value), Equation::PCSourceXValue); return result; }
   friend thisType atan2(const thisType& source, unsigned int value)
      {  thisType result(source); result.atan2Assign(thisType(value), Equation::PCSourceXValue); return result; }
   friend thisType atan2(const thisType& source, int value)
      {  thisType result(source); result.atan2Assign(thisType(value), Equation::PCSourceXValue); return result; }
   friend thisType atan2(const thisType& source, unsigned short value)
      {  thisType result(source); result.atan2Assign(thisType(value), Equation::PCSourceXValue); return result; }
   friend thisType atan2(const thisType& source, short value)
      {  thisType result(source); result.atan2Assign(thisType(value), Equation::PCSourceXValue); return result; }
   friend thisType atan2(thisType&& source, long double value)
      {  thisType result(std::forward<thisType>(source)); result.atan2Assign(thisType(value), Equation::PCSourceXValue); return result; }
   friend thisType atan2(thisType&& source, double value)
      {  thisType result(std::forward<thisType>(source)); result.atan2Assign(thisType(value), Equation::PCSourceXValue); return result; }
   friend thisType atan2(thisType&& source, float value)
      {  thisType result(std::forward<thisType>(source)); result.atan2Assign(thisType(value), Equation::PCSourceXValue); return result; }
   friend thisType atan2(thisType&& source, unsigned long value)
      {  thisType result(std::forward<thisType>(source)); result.atan2Assign(thisType(value), Equation::PCSourceXValue); return result; }
   friend thisType atan2(thisType&& source, long value)
      {  thisType result(std::forward<thisType>(source)); result.atan2Assign(thisType(value), Equation::PCSourceXValue); return result; }
   friend thisType atan2(thisType&& source, unsigned int value)
      {  thisType result(std::forward<thisType>(source)); result.atan2Assign(thisType(value), Equation::PCSourceXValue); return result; }
   friend thisType atan2(thisType&& source, int value)
      {  thisType result(std::forward<thisType>(source)); result.atan2Assign(thisType(value), Equation::PCSourceXValue); return result; }
   friend thisType atan2(thisType&& source, unsigned short value)
      {  thisType result(std::forward<thisType>(source)); result.atan2Assign(thisType(value), Equation::PCSourceXValue); return result; }
   friend thisType atan2(thisType&& source, short value)
      {  thisType result(std::forward<thisType>(source)); result.atan2Assign(thisType(value), Equation::PCSourceXValue); return result; }

   template <int USizeMantissaArgument, int USizeExponentArgument, typename TypeImplementationArgument>
   friend bool operator<(const TInstrumentedFloatZonotope<USizeMantissaArgument, USizeExponentArgument, TypeImplementationArgument>& fst, const thisType& snd)
      {  return thisType(fst).operator<(snd); }    
   friend bool operator<(long double fst, const thisType& snd)
      {  return thisType(fst).operator<(snd); }    
   friend bool operator<(double fst, const thisType& snd)
      {  return thisType(fst).operator<(snd); }    
   friend bool operator<(float fst, const thisType& snd)
      {  return thisType(fst).operator<(snd); }    
   friend bool operator<(unsigned long fst, const thisType& snd)
      {  return thisType(fst).operator<(snd); }    
   friend bool operator<(long fst, const thisType& snd)
      {  return thisType(fst).operator<(snd); }    
   friend bool operator<(unsigned int fst, const thisType& snd)
      {  return thisType(fst).operator<(snd); }    
   friend bool operator<(int fst, const thisType& snd)
      {  return thisType(fst).operator<(snd); }    
   friend bool operator<(unsigned short fst, const thisType& snd)
      {  return thisType(fst).operator<(snd); }    
   friend bool operator<(short fst, const thisType& snd)
      {  return thisType(fst).operator<(snd); }    
   template <int USizeMantissaArgument, int USizeExponentArgument, typename TypeImplementationArgument>
   friend bool operator<=(const TInstrumentedFloatZonotope<USizeMantissaArgument, USizeExponentArgument, TypeImplementationArgument>& fst, const thisType& snd)
      {  return thisType(fst).operator<=(snd); }   
   friend bool operator<=(long double fst, const thisType& snd)
      {  return thisType(fst).operator<=(snd); }   
   friend bool operator<=(double fst, const thisType& snd)
      {  return thisType(fst).operator<=(snd); }   
   friend bool operator<=(float fst, const thisType& snd)
      {  return thisType(fst).operator<=(snd); }   
   friend bool operator<=(unsigned long fst, const thisType& snd)
      {  return thisType(fst).operator<=(snd); }   
   friend bool operator<=(long fst, const thisType& snd)
      {  return thisType(fst).operator<=(snd); }   
   friend bool operator<=(unsigned int fst, const thisType& snd)
      {  return thisType(fst).operator<=(snd); }   
   friend bool operator<=(int fst, const thisType& snd)
      {  return thisType(fst).operator<=(snd); }   
   friend bool operator<=(unsigned short fst, const thisType& snd)
      {  return thisType(fst).operator<=(snd); }   
   friend bool operator<=(short fst, const thisType& snd)
      {  return thisType(fst).operator<=(snd); }   
   template <int USizeMantissaArgument, int USizeExponentArgument, typename TypeImplementationArgument>
   friend bool operator==(const TInstrumentedFloatZonotope<USizeMantissaArgument, USizeExponentArgument, TypeImplementationArgument>& fst, const thisType& snd)
      {  return thisType(fst).operator==(snd); }   
   friend bool operator==(long double fst, const thisType& snd)
      {  return thisType(fst).operator==(snd); }   
   friend bool operator==(double fst, const thisType& snd)
      {  return thisType(fst).operator==(snd); }   
   friend bool operator==(float fst, const thisType& snd)
      {  return thisType(fst).operator==(snd); }   
   friend bool operator==(unsigned long fst, const thisType& snd)
      {  return thisType(fst).operator==(snd); }   
   friend bool operator==(long fst, const thisType& snd)
      {  return thisType(fst).operator==(snd); }   
   friend bool operator==(unsigned int fst, const thisType& snd)
      {  return thisType(fst).operator==(snd); }   
   friend bool operator==(int fst, const thisType& snd)
      {  return thisType(fst).operator==(snd); }   
   friend bool operator==(unsigned short fst, const thisType& snd)
      {  return thisType(fst).operator==(snd); }   
   friend bool operator==(short fst, const thisType& snd)
      {  return thisType(fst).operator==(snd); }   
   template <int USizeMantissaArgument, int USizeExponentArgument, typename TypeImplementationArgument>
   friend bool operator!=(const TInstrumentedFloatZonotope<USizeMantissaArgument, USizeExponentArgument, TypeImplementationArgument>& fst, const thisType& snd)
      {  return thisType(fst).operator!=(snd); }   
   friend bool operator!=(long double fst, const thisType& snd)
      {  return thisType(fst).operator!=(snd); }   
   friend bool operator!=(double fst, const thisType& snd)
      {  return thisType(fst).operator!=(snd); }   
   friend bool operator!=(float fst, const thisType& snd)
      {  return thisType(fst).operator!=(snd); }   
   friend bool operator!=(unsigned long fst, const thisType& snd)
      {  return thisType(fst).operator!=(snd); }   
   friend bool operator!=(long fst, const thisType& snd)
      {  return thisType(fst).operator!=(snd); }   
   friend bool operator!=(unsigned int fst, const thisType& snd)
      {  return thisType(fst).operator!=(snd); }   
   friend bool operator!=(int fst, const thisType& snd)
      {  return thisType(fst).operator!=(snd); }   
   friend bool operator!=(unsigned short fst, const thisType& snd)
      {  return thisType(fst).operator!=(snd); }   
   friend bool operator!=(short fst, const thisType& snd)
      {  return thisType(fst).operator!=(snd); }   
   template <int USizeMantissaArgument, int USizeExponentArgument, typename TypeImplementationArgument>
   friend bool operator>=(const TInstrumentedFloatZonotope<USizeMantissaArgument, USizeExponentArgument, TypeImplementationArgument>& fst, const thisType& snd)
      {  return thisType(fst).operator>=(snd); }   
   friend bool operator>=(long double fst, const thisType& snd)
      {  return thisType(fst).operator>=(snd); }   
   friend bool operator>=(double fst, const thisType& snd)
      {  return thisType(fst).operator>=(snd); }   
   friend bool operator>=(float fst, const thisType& snd)
      {  return thisType(fst).operator>=(snd); }   
   friend bool operator>=(unsigned long fst, const thisType& snd)
      {  return thisType(fst).operator>=(snd); }   
   friend bool operator>=(long fst, const thisType& snd)
      {  return thisType(fst).operator>=(snd); }   
   friend bool operator>=(unsigned int fst, const thisType& snd)
      {  return thisType(fst).operator>=(snd); }   
   friend bool operator>=(int fst, const thisType& snd)
      {  return thisType(fst).operator>=(snd); }   
   friend bool operator>=(unsigned short fst, const thisType& snd)
      {  return thisType(fst).operator>=(snd); }   
   friend bool operator>=(short fst, const thisType& snd)
      {  return thisType(fst).operator>=(snd); }   
   template <int USizeMantissaArgument, int USizeExponentArgument, typename TypeImplementationArgument>
   friend bool operator>(const TInstrumentedFloatZonotope<USizeMantissaArgument, USizeExponentArgument, TypeImplementationArgument>& fst, const thisType& snd)
      {  return thisType(fst).operator>(snd); }    
   friend bool operator>(long double fst, const thisType& snd)
      {  return thisType(fst).operator>(snd); }    
   friend bool operator>(double fst, const thisType& snd)
      {  return thisType(fst).operator>(snd); }    
   friend bool operator>(float fst, const thisType& snd)
      {  return thisType(fst).operator>(snd); }    
   friend bool operator>(unsigned long fst, const thisType& snd)
      {  return thisType(fst).operator>(snd); }    
   friend bool operator>(long fst, const thisType& snd)
      {  return thisType(fst).operator>(snd); }    
   friend bool operator>(unsigned int fst, const thisType& snd)
      {  return thisType(fst).operator>(snd); }    
   friend bool operator>(int fst, const thisType& snd)
      {  return thisType(fst).operator>(snd); }    
   friend bool operator>(unsigned short fst, const thisType& snd)
      {  return thisType(fst).operator>(snd); }    
   friend bool operator>(short fst, const thisType& snd)
      {  return thisType(fst).operator>(snd); }    
   template <int USizeMantissaArgument, int USizeExponentArgument, typename TypeImplementationArgument>
   friend thisType operator+(const TInstrumentedFloatZonotope<USizeMantissaArgument, USizeExponentArgument, TypeImplementationArgument>& fst, const thisType& snd)
      {  thisType fstConvert(fst); fstConvert.plusAssign(snd, Equation::PCSourceRValue); return fstConvert; }
   template <int USizeMantissaArgument, int USizeExponentArgument, typename TypeImplementationArgument>
   friend thisType operator+(const TInstrumentedFloatZonotope<USizeMantissaArgument, USizeExponentArgument, TypeImplementationArgument>& fst, thisType&& snd)
      {  thisType fstConvert(fst); fstConvert.plusAssign(snd, Equation::PCSourceXValue); return fstConvert; }
   template <int USizeMantissaArgument, int USizeExponentArgument, typename TypeImplementationArgument>
   friend thisType operator+(TInstrumentedFloatZonotope<USizeMantissaArgument, USizeExponentArgument, TypeImplementationArgument>&& fst, const thisType& snd)
      {  thisType fstConvert(std::forward<TInstrumentedFloatZonotope<USizeMantissaArgument, USizeExponentArgument, TypeImplementationArgument> >(fst)); fstConvert.plusAssign(snd, Equation::PCSourceRValue); return fstConvert; }
   template <int USizeMantissaArgument, int USizeExponentArgument, typename TypeImplementationArgument>
   friend thisType operator+(TInstrumentedFloatZonotope<USizeMantissaArgument, USizeExponentArgument, TypeImplementationArgument>&& fst, thisType&& snd)
      {  thisType fstConvert(std::forward<TInstrumentedFloatZonotope<USizeMantissaArgument, USizeExponentArgument, TypeImplementationArgument> >(fst)); fstConvert.plusAssign(snd, Equation::PCSourceXValue); return fstConvert; }
   friend thisType operator+(long double fst, const thisType& snd)
      {  thisType fstConvert(fst); fstConvert.plusAssign(snd, Equation::PCSourceRValue); return fstConvert; }
   friend thisType operator+(double fst, const thisType& snd)
      {  thisType fstConvert(fst); fstConvert.plusAssign(snd, Equation::PCSourceRValue); return fstConvert; }
   friend thisType operator+(float fst, const thisType& snd)
      {  thisType fstConvert(fst); fstConvert.plusAssign(snd, Equation::PCSourceRValue); return fstConvert; }
   friend thisType operator+(unsigned long fst, const thisType& snd)
      {  thisType fstConvert(fst); fstConvert.plusAssign(snd, Equation::PCSourceRValue); return fstConvert; }
   friend thisType operator+(long fst, const thisType& snd)
      {  thisType fstConvert(fst); fstConvert.plusAssign(snd, Equation::PCSourceRValue); return fstConvert; }
   friend thisType operator+(unsigned int fst, const thisType& snd)
      {  thisType fstConvert(fst); fstConvert.plusAssign(snd, Equation::PCSourceRValue); return fstConvert; }
   friend thisType operator+(int fst, const thisType& snd)
      {  thisType fstConvert(fst); fstConvert.plusAssign(snd, Equation::PCSourceRValue); return fstConvert; }
   friend thisType operator+(unsigned short fst, const thisType& snd)
      {  thisType fstConvert(fst); fstConvert.plusAssign(snd, Equation::PCSourceRValue); return fstConvert; }
   friend thisType operator+(short fst, const thisType& snd)
      {  thisType fstConvert(fst); fstConvert.plusAssign(snd, Equation::PCSourceRValue); return fstConvert; }
   friend thisType operator+(long double fst, thisType&& snd)
      {  thisType fstConvert(fst); fstConvert.plusAssign(snd, Equation::PCSourceXValue); return fstConvert; }
   friend thisType operator+(double fst, thisType&& snd)
      {  thisType fstConvert(fst); fstConvert.plusAssign(snd, Equation::PCSourceXValue); return fstConvert; }
   friend thisType operator+(float fst, thisType&& snd)
      {  thisType fstConvert(fst); fstConvert.plusAssign(snd, Equation::PCSourceXValue); return fstConvert; }
   friend thisType operator+(unsigned long fst, thisType&& snd)
      {  thisType fstConvert(fst); fstConvert.plusAssign(snd, Equation::PCSourceXValue); return fstConvert; }
   friend thisType operator+(long fst, thisType&& snd)
      {  thisType fstConvert(fst); fstConvert.plusAssign(snd, Equation::PCSourceXValue); return fstConvert; }
   friend thisType operator+(unsigned int fst, thisType&& snd)
      {  thisType fstConvert(fst); fstConvert.plusAssign(snd, Equation::PCSourceXValue); return fstConvert; }
   friend thisType operator+(int fst, thisType&& snd)
      {  thisType fstConvert(fst); fstConvert.plusAssign(snd, Equation::PCSourceXValue); return fstConvert; }
   friend thisType operator+(unsigned short fst, thisType&& snd)
      {  thisType fstConvert(fst); fstConvert.plusAssign(snd, Equation::PCSourceXValue); return fstConvert; }
   friend thisType operator+(short fst, thisType&& snd)
      {  thisType fstConvert(fst); fstConvert.plusAssign(snd, Equation::PCSourceXValue); return fstConvert; }
   template <int USizeMantissaArgument, int USizeExponentArgument, typename TypeImplementationArgument>
   friend thisType operator-(const TInstrumentedFloatZonotope<USizeMantissaArgument, USizeExponentArgument, TypeImplementationArgument>& fst, const thisType& snd)
      {  thisType fstConvert(fst); fstConvert.minusAssign(snd, Equation::PCSourceRValue); return fstConvert; }
   template <int USizeMantissaArgument, int USizeExponentArgument, typename TypeImplementationArgument>
   friend thisType operator-(const TInstrumentedFloatZonotope<USizeMantissaArgument, USizeExponentArgument, TypeImplementationArgument>& fst, thisType&& snd)
      {  thisType fstConvert(fst); fstConvert.minusAssign(snd, Equation::PCSourceXValue); return fstConvert; }
   template <int USizeMantissaArgument, int USizeExponentArgument, typename TypeImplementationArgument>
   friend thisType operator-(TInstrumentedFloatZonotope<USizeMantissaArgument, USizeExponentArgument, TypeImplementationArgument>&& fst, const thisType& snd)
      {  thisType fstConvert(std::forward<TInstrumentedFloatZonotope<USizeMantissaArgument, USizeExponentArgument, TypeImplementationArgument> >(fst)); fstConvert.minusAssign(snd, Equation::PCSourceRValue); return fstConvert; }
   template <int USizeMantissaArgument, int USizeExponentArgument, typename TypeImplementationArgument>
   friend thisType operator-(TInstrumentedFloatZonotope<USizeMantissaArgument, USizeExponentArgument, TypeImplementationArgument>&& fst, thisType&& snd)
      {  thisType fstConvert(std::forward<TInstrumentedFloatZonotope<USizeMantissaArgument, USizeExponentArgument, TypeImplementationArgument> >(fst)); fstConvert.minusAssign(snd, Equation::PCSourceXValue); return fstConvert; }
   friend thisType operator-(long double fst, const thisType& snd)
      {  thisType fstConvert(fst); fstConvert.minusAssign(snd, Equation::PCSourceRValue); return fstConvert; }
   friend thisType operator-(double fst, const thisType& snd)
      {  thisType fstConvert(fst); fstConvert.minusAssign(snd, Equation::PCSourceRValue); return fstConvert; }
   friend thisType operator-(float fst, const thisType& snd)
      {  thisType fstConvert(fst); fstConvert.minusAssign(snd, Equation::PCSourceRValue); return fstConvert; }
   friend thisType operator-(unsigned long fst, const thisType& snd)
      {  thisType fstConvert(fst); fstConvert.minusAssign(snd, Equation::PCSourceRValue); return fstConvert; }
   friend thisType operator-(long fst, const thisType& snd)
      {  thisType fstConvert(fst); fstConvert.minusAssign(snd, Equation::PCSourceRValue); return fstConvert; }
   friend thisType operator-(unsigned int fst, const thisType& snd)
      {  thisType fstConvert(fst); fstConvert.minusAssign(snd, Equation::PCSourceRValue); return fstConvert; }
   friend thisType operator-(int fst, const thisType& snd)
      {  thisType fstConvert(fst); fstConvert.minusAssign(snd, Equation::PCSourceRValue); return fstConvert; }
   friend thisType operator-(unsigned short fst, const thisType& snd)
      {  thisType fstConvert(fst); fstConvert.minusAssign(snd, Equation::PCSourceRValue); return fstConvert; }
   friend thisType operator-(short fst, const thisType& snd)
      {  thisType fstConvert(fst); fstConvert.minusAssign(snd, Equation::PCSourceRValue); return fstConvert; }
   friend thisType operator-(long double fst, thisType&& snd)
      {  thisType fstConvert(fst); fstConvert.minusAssign(snd, Equation::PCSourceXValue); return fstConvert; }
   friend thisType operator-(double fst, thisType&& snd)
      {  thisType fstConvert(fst); fstConvert.minusAssign(snd, Equation::PCSourceXValue); return fstConvert; }
   friend thisType operator-(float fst, thisType&& snd)
      {  thisType fstConvert(fst); fstConvert.minusAssign(snd, Equation::PCSourceXValue); return fstConvert; }
   friend thisType operator-(unsigned long fst, thisType&& snd)
      {  thisType fstConvert(fst); fstConvert.minusAssign(snd, Equation::PCSourceXValue); return fstConvert; }
   friend thisType operator-(long fst, thisType&& snd)
      {  thisType fstConvert(fst); fstConvert.minusAssign(snd, Equation::PCSourceXValue); return fstConvert; }
   friend thisType operator-(unsigned int fst, thisType&& snd)
      {  thisType fstConvert(fst); fstConvert.minusAssign(snd, Equation::PCSourceXValue); return fstConvert; }
   friend thisType operator-(int fst, thisType&& snd)
      {  thisType fstConvert(fst); fstConvert.minusAssign(snd, Equation::PCSourceXValue); return fstConvert; }
   friend thisType operator-(unsigned short fst, thisType&& snd)
      {  thisType fstConvert(fst); fstConvert.minusAssign(snd, Equation::PCSourceXValue); return fstConvert; }
   friend thisType operator-(short fst, thisType&& snd)
      {  thisType fstConvert(fst); fstConvert.minusAssign(snd, Equation::PCSourceXValue); return fstConvert; }
   template <int USizeMantissaArgument, int USizeExponentArgument, typename TypeImplementationArgument>
   friend thisType operator*(const TInstrumentedFloatZonotope<USizeMantissaArgument, USizeExponentArgument, TypeImplementationArgument>& fst, const thisType& snd)
      {  thisType fstConvert(fst); fstConvert.multAssign(snd, Equation::PCSourceRValue); return fstConvert; }
   template <int USizeMantissaArgument, int USizeExponentArgument, typename TypeImplementationArgument>
   friend thisType operator*(const TInstrumentedFloatZonotope<USizeMantissaArgument, USizeExponentArgument, TypeImplementationArgument>& fst, thisType&& snd)
      {  thisType fstConvert(fst); fstConvert.multAssign(snd, Equation::PCSourceXValue); return fstConvert; }
   template <int USizeMantissaArgument, int USizeExponentArgument, typename TypeImplementationArgument>
   friend thisType operator*(TInstrumentedFloatZonotope<USizeMantissaArgument, USizeExponentArgument, TypeImplementationArgument>&& fst, const thisType& snd)
      {  thisType fstConvert(std::forward<TInstrumentedFloatZonotope<USizeMantissaArgument, USizeExponentArgument, TypeImplementationArgument> >(fst)); fstConvert.multAssign(snd, Equation::PCSourceRValue); return fstConvert; }
   template <int USizeMantissaArgument, int USizeExponentArgument, typename TypeImplementationArgument>
   friend thisType operator*(TInstrumentedFloatZonotope<USizeMantissaArgument, USizeExponentArgument, TypeImplementationArgument>&& fst, thisType&& snd)
      {  thisType fstConvert(std::forward<TInstrumentedFloatZonotope<USizeMantissaArgument, USizeExponentArgument, TypeImplementationArgument> >(fst)); fstConvert.multAssign(snd, Equation::PCSourceXValue); return fstConvert; }
   friend thisType operator*(long double fst, const thisType& snd)
      {  thisType fstConvert(fst); fstConvert.multAssign(snd, Equation::PCSourceRValue); return fstConvert; }
   friend thisType operator*(double fst, const thisType& snd)
      {  thisType fstConvert(fst); fstConvert.multAssign(snd, Equation::PCSourceRValue); return fstConvert; }
   friend thisType operator*(float fst, const thisType& snd)
      {  thisType fstConvert(fst); fstConvert.multAssign(snd, Equation::PCSourceRValue); return fstConvert; }
   friend thisType operator*(unsigned long fst, const thisType& snd)
      {  thisType fstConvert(fst); fstConvert.multAssign(snd, Equation::PCSourceRValue); return fstConvert; }
   friend thisType operator*(long fst, const thisType& snd)
      {  thisType fstConvert(fst); fstConvert.multAssign(snd, Equation::PCSourceRValue); return fstConvert; }
   friend thisType operator*(unsigned int fst, const thisType& snd)
      {  thisType fstConvert(fst); fstConvert.multAssign(snd, Equation::PCSourceRValue); return fstConvert; }
   friend thisType operator*(int fst, const thisType& snd)
      {  thisType fstConvert(fst); fstConvert.multAssign(snd, Equation::PCSourceRValue); return fstConvert; }
   friend thisType operator*(unsigned short fst, const thisType& snd)
      {  thisType fstConvert(fst); fstConvert.multAssign(snd, Equation::PCSourceRValue); return fstConvert; }
   friend thisType operator*(short fst, const thisType& snd)
      {  thisType fstConvert(fst); fstConvert.multAssign(snd, Equation::PCSourceRValue); return fstConvert; }
   friend thisType operator*(long double fst, thisType&& snd)
      {  thisType fstConvert(fst); fstConvert.multAssign(snd, Equation::PCSourceXValue); return fstConvert; }
   friend thisType operator*(double fst, thisType&& snd)
      {  thisType fstConvert(fst); fstConvert.multAssign(snd, Equation::PCSourceXValue); return fstConvert; }
   friend thisType operator*(float fst, thisType&& snd)
      {  thisType fstConvert(fst); fstConvert.multAssign(snd, Equation::PCSourceXValue); return fstConvert; }
   friend thisType operator*(unsigned long fst, thisType&& snd)
      {  thisType fstConvert(fst); fstConvert.multAssign(snd, Equation::PCSourceXValue); return fstConvert; }
   friend thisType operator*(long fst, thisType&& snd)
      {  thisType fstConvert(fst); fstConvert.multAssign(snd, Equation::PCSourceXValue); return fstConvert; }
   friend thisType operator*(unsigned int fst, thisType&& snd)
      {  thisType fstConvert(fst); fstConvert.multAssign(snd, Equation::PCSourceXValue); return fstConvert; }
   friend thisType operator*(int fst, thisType&& snd)
      {  thisType fstConvert(fst); fstConvert.multAssign(snd, Equation::PCSourceXValue); return fstConvert; }
   friend thisType operator*(unsigned short fst, thisType&& snd)
      {  thisType fstConvert(fst); fstConvert.multAssign(snd, Equation::PCSourceXValue); return fstConvert; }
   friend thisType operator*(short fst, thisType&& snd)
      {  thisType fstConvert(fst); fstConvert.multAssign(snd, Equation::PCSourceXValue); return fstConvert; }
   template <int USizeMantissaArgument, int USizeExponentArgument, typename TypeImplementationArgument>
   friend thisType operator/(const TInstrumentedFloatZonotope<USizeMantissaArgument, USizeExponentArgument, TypeImplementationArgument>& fst, const thisType& snd)
      {  thisType fstConvert(fst); fstConvert.divAssign(snd, Equation::PCSourceRValue); return fstConvert; }
   template <int USizeMantissaArgument, int USizeExponentArgument, typename TypeImplementationArgument>
   friend thisType operator/(const TInstrumentedFloatZonotope<USizeMantissaArgument, USizeExponentArgument, TypeImplementationArgument>& fst, thisType&& snd)
      {  thisType fstConvert(fst); fstConvert.divAssign(snd, Equation::PCSourceXValue); return fstConvert; }
   template <int USizeMantissaArgument, int USizeExponentArgument, typename TypeImplementationArgument>
   friend thisType operator/(TInstrumentedFloatZonotope<USizeMantissaArgument, USizeExponentArgument, TypeImplementationArgument>&& fst, const thisType& snd)
      {  thisType fstConvert(std::forward<TInstrumentedFloatZonotope<USizeMantissaArgument, USizeExponentArgument, TypeImplementationArgument> >(fst)); fstConvert.divAssign(snd, Equation::PCSourceRValue); return fstConvert; }
   template <int USizeMantissaArgument, int USizeExponentArgument, typename TypeImplementationArgument>
   friend thisType operator/(TInstrumentedFloatZonotope<USizeMantissaArgument, USizeExponentArgument, TypeImplementationArgument>&& fst, thisType&& snd)
      {  thisType fstConvert(std::forward<TInstrumentedFloatZonotope<USizeMantissaArgument, USizeExponentArgument, TypeImplementationArgument> >(fst)); fstConvert.divAssign(snd, Equation::PCSourceXValue); return fstConvert; }
   friend thisType operator/(long double fst, const thisType& snd)
      {  thisType fstConvert(fst); fstConvert.divAssign(snd, Equation::PCSourceRValue); return fstConvert; }
   friend thisType operator/(double fst, const thisType& snd)
      {  thisType fstConvert(fst); fstConvert.divAssign(snd, Equation::PCSourceRValue); return fstConvert; }
   friend thisType operator/(float fst, const thisType& snd)
      {  thisType fstConvert(fst); fstConvert.divAssign(snd, Equation::PCSourceRValue); return fstConvert; }
   friend thisType operator/(unsigned long fst, const thisType& snd)
      {  thisType fstConvert(fst); fstConvert.divAssign(snd, Equation::PCSourceRValue); return fstConvert; }
   friend thisType operator/(long fst, const thisType& snd)
      {  thisType fstConvert(fst); fstConvert.divAssign(snd, Equation::PCSourceRValue); return fstConvert; }
   friend thisType operator/(unsigned int fst, const thisType& snd)
      {  thisType fstConvert(fst); fstConvert.divAssign(snd, Equation::PCSourceRValue); return fstConvert; }
   friend thisType operator/(int fst, const thisType& snd)
      {  thisType fstConvert(fst); fstConvert.divAssign(snd, Equation::PCSourceRValue); return fstConvert; }
   friend thisType operator/(unsigned short fst, const thisType& snd)
      {  thisType fstConvert(fst); fstConvert.divAssign(snd, Equation::PCSourceRValue); return fstConvert; }
   friend thisType operator/(short fst, const thisType& snd)
      {  thisType fstConvert(fst); fstConvert.divAssign(snd, Equation::PCSourceRValue); return fstConvert; }
   friend thisType operator/(long double fst, thisType&& snd)
      {  thisType fstConvert(fst); fstConvert.divAssign(snd, Equation::PCSourceXValue); return fstConvert; }
   friend thisType operator/(double fst, thisType&& snd)
      {  thisType fstConvert(fst); fstConvert.divAssign(snd, Equation::PCSourceXValue); return fstConvert; }
   friend thisType operator/(float fst, thisType&& snd)
      {  thisType fstConvert(fst); fstConvert.divAssign(snd, Equation::PCSourceXValue); return fstConvert; }
   friend thisType operator/(unsigned long fst, thisType&& snd)
      {  thisType fstConvert(fst); fstConvert.divAssign(snd, Equation::PCSourceXValue); return fstConvert; }
   friend thisType operator/(long fst, thisType&& snd)
      {  thisType fstConvert(fst); fstConvert.divAssign(snd, Equation::PCSourceXValue); return fstConvert; }
   friend thisType operator/(unsigned int fst, thisType&& snd)
      {  thisType fstConvert(fst); fstConvert.divAssign(snd, Equation::PCSourceXValue); return fstConvert; }
   friend thisType operator/(int fst, thisType&& snd)
      {  thisType fstConvert(fst); fstConvert.divAssign(snd, Equation::PCSourceXValue); return fstConvert; }
   friend thisType operator/(unsigned short fst, thisType&& snd)
      {  thisType fstConvert(fst); fstConvert.divAssign(snd, Equation::PCSourceXValue); return fstConvert; }
   friend thisType operator/(short fst, thisType&& snd)
      {  thisType fstConvert(fst); fstConvert.divAssign(snd, Equation::PCSourceXValue); return fstConvert; }
   friend thisType floor(const thisType& afst)
      {  thisType result, fst(afst); // afst should gain back its original value
         result.continuousFlow(
               [](thisType& result, const thisType& fst)
                  {  result = thisType(fst.asInt(inherited::ReadParametersBase::RMLowest)); },
               fst);
         return result;
      }
   friend thisType floor(thisType&& fst)
      {  thisType result;
         result.continuousFlow(
               [](thisType& result, const thisType& fst)
                  {  result = thisType(fst.asInt(inherited::ReadParametersBase::RMLowest)); },
               fst);
         return result;
      }
   friend thisType ceil(const thisType& afst)
      {  thisType result, fst(afst);
         result.continuousFlow(
               [](thisType& result, const thisType& fst)
                  {  result = thisType(fst.asInt(thisType::ReadParametersBase::RMHighest)); },
               fst);
         return result;
      }
   friend thisType ceil(thisType&& fst)
      {  thisType result;
         result.continuousFlow(
               [](thisType& result, const thisType& fst)
                  {  result = thisType(fst.asInt(thisType::ReadParametersBase::RMHighest)); },
               fst);
         return result;
      }
   friend thisType trunc(const thisType& afst)
      {  thisType result, fst(afst);
         result.continuousFlow(
               [](thisType& result, const thisType& fst)
                  {  result = thisType(fst.asInt(thisType::ReadParametersBase::RMZero)); },
               fst);
         return result;
      }
   friend thisType trunc(thisType&& fst)
      {  thisType result;
         result.continuousFlow(
               [](thisType& result, const thisType& fst)
                  {  result = thisType(fst.asInt(thisType::ReadParametersBase::RMZero)); },
               fst);
         return result;
      }
   friend thisType round(const thisType& afst)
      {  thisType result, fst(afst);
         result.continuousFlow(
               [](thisType& result, const thisType& fst)
                  {  result = thisType(fst.asInt(thisType::ReadParametersBase::RMNearest)); },
               fst);
         return result;
      }
   friend thisType round(thisType&& fst)
      {  thisType result;
         result.continuousFlow(
               [](thisType& result, const thisType& fst)
                  {  result = thisType(fst.asInt(thisType::ReadParametersBase::RMNearest)); },
               fst);
         return result;
      }
   friend thisType rint(const thisType& afst)
      {  thisType result, fst(afst);
         result.continuousFlow(
               [](thisType& result, const thisType& fst)
                  {  result = thisType(fst.asInt(thisType::ReadParametersBase::RMNearest)); },
               fst);
         return result;
      }
   friend thisType rint(thisType&& fst)
      {  thisType result;
         result.continuousFlow(
               [](thisType& result, const thisType& fst)
                  {  result = thisType(fst.asInt(thisType::ReadParametersBase::RMNearest)); },
               fst);
         return result;
      }
   friend thisType rintf(const thisType& afst)
      {  thisType result, fst(afst);
         result.continuousFlow(
               [](thisType& result, const thisType& fst)
                  {  result = thisType(fst.asInt(thisType::ReadParametersBase::RMNearest)); },
               fst);
         return result;
      }
   friend thisType rintf(thisType&& fst)
      {  thisType result;
         result.continuousFlow(
               [](thisType& result, const thisType& fst)
                  {  result = thisType(fst.asInt(thisType::ReadParametersBase::RMNearest)); },
               fst);
         return result;
      }
   friend thisType dis_floor(const thisType& fst)
      {  return thisType(fst.asInt(thisType::ReadParametersBase::RMLowest)); }
   friend thisType dis_ceil(const thisType& fst)
      {  return thisType(fst.asInt(thisType::ReadParametersBase::RMHighest)); }
   friend thisType dis_trunc(const thisType& fst)
      {  return thisType(fst.asInt(thisType::ReadParametersBase::RMZero)); }
   friend thisType dis_round(const thisType& fst)
      {  return thisType(fst.asInt(thisType::ReadParametersBase::RMNearest)); }
   friend thisType dis_rint(const thisType& fst)
      {  return thisType(fst.asInt(thisType::ReadParametersBase::RMNearest /* fegetround */)); }
   friend thisType dis_rintf(const thisType& fst)
      {  return thisType(fst.asInt(thisType::ReadParametersBase::RMNearest /* fegetround */)); }
   friend thisType fabs(const thisType& fst)
      {  auto result(fst);
         result.continuousFlow([](thisType& val){ if (val < 0) val.oppositeAssign(); });
         return result;
      }
   friend thisType fabs(thisType&& fst)
      {  fst.continuousFlow([](thisType& val){ if (val < 0) val.oppositeAssign(); });
         return fst;
      }
   friend thisType abs(const thisType& fst) { return fabs(fst); }
   friend thisType abs(thisType&& fst) { return fabs(std::forward<thisType>(fst)); }

   friend thisType fmod(const thisType& source, const thisType& value)
      {  thisType multResult;
         multResult.continuousFlow(
               [](thisType& result, const thisType& source, const thisType& value)
               {  auto divResult(source); divResult /= value;
                  result = thisType(divResult.asInt(thisType::ReadParametersBase::RMZero));
                  result *= value;
                  result -= source;
                  result.oppositeAssign();
               },
               source, value);
         return multResult;
      }
   template <int USizeMantissaArgument, int USizeExponentArgument, typename TypeImplementationArgument>
   friend thisType fmod(const TInstrumentedFloatZonotope<USizeMantissaArgument, USizeExponentArgument, TypeImplementationArgument>& asource, const thisType& value)
      {  thisType source(asource);
         source.continuousFlow(
               [](thisType& source, const thisType& value)
               {  auto divResult(source); divResult /= value;
                  thisType multResult(divResult.asInt(thisType::ReadParametersBase::RMZero));
                  multResult *= value; source -= multResult;
               },
               source, value);
         return source;
      }
   friend thisType fmod(long double asource, const thisType& value)
      {  thisType source(asource);
         source.continuousFlow(
               [](thisType& source, const thisType& value)
               {  auto divResult(source); divResult /= value;
                  thisType multResult(divResult.asInt(thisType::ReadParametersBase::RMZero));
                  multResult *= value; source -= multResult;
               },
               source, value);
         return source;
      }
   friend thisType fmod(double asource, const thisType& value)
      {  thisType source(asource);
         source.continuousFlow(
               [](thisType& source, const thisType& value)
               {  auto divResult(source); divResult /= value;
                  thisType multResult(divResult.asInt(thisType::ReadParametersBase::RMZero));
                  multResult *= value; source -= multResult;
               },
               source, value);
         return source;
      }
   friend thisType fmod(float asource, const thisType& value)
      {  thisType source(asource);
         source.continuousFlow(
               [](thisType& source, const thisType& value)
               {  auto divResult(source); divResult /= value;
                  thisType multResult(divResult.asInt(thisType::ReadParametersBase::RMZero));
                  multResult *= value; source -= multResult;
               },
               source, value);
         return source;
      }
   friend thisType fmod(unsigned long asource, const thisType& value)
      {  thisType source(asource);
         source.continuousFlow(
               [](thisType& source, const thisType& value)
               {  auto divResult(source); divResult /= value;
                  thisType multResult(divResult.asInt(thisType::ReadParametersBase::RMZero));
                  multResult *= value; source -= multResult;
               },
               source, value);
         return source;
      }
   friend thisType fmod(long asource, const thisType& value)
      {  thisType source(asource);
         source.continuousFlow(
               [](thisType& source, const thisType& value)
               {  auto divResult(source); divResult /= value;
                  thisType multResult(divResult.asInt(thisType::ReadParametersBase::RMZero));
                  multResult *= value; source -= multResult;
               },
               source, value);
         return source;
      }
   friend thisType fmod(unsigned int asource, const thisType& value)
      {  thisType source(asource);
         source.continuousFlow(
               [](thisType& source, const thisType& value)
               {  auto divResult(source); divResult /= value;
                  thisType multResult(divResult.asInt(thisType::ReadParametersBase::RMZero));
                  multResult *= value; source -= multResult;
               },
               source, value);
         return source;
      }
   friend thisType fmod(int asource, const thisType& value)
      {  thisType source(asource);
         source.continuousFlow(
               [](thisType& source, const thisType& value)
               {  auto divResult(source); divResult /= value;
                  thisType multResult(divResult.asInt(thisType::ReadParametersBase::RMZero));
                  multResult *= value; source -= multResult;
               },
               source, value);
         return source;
      }
   friend thisType fmod(unsigned short asource, const thisType& value)
      {  thisType source(asource);
         source.continuousFlow(
               [](thisType& source, const thisType& value)
               {  auto divResult(source); divResult /= value;
                  thisType multResult(divResult.asInt(thisType::ReadParametersBase::RMZero));
                  multResult *= value; source -= multResult;
               },
               source, value);
         return source;
      }
   friend thisType fmod(short asource, const thisType& value)
      {  thisType source(asource);
         source.continuousFlow(
               [](thisType& source, const thisType& value)
               {  auto divResult(source); divResult /= value;
                  thisType multResult(divResult.asInt(thisType::ReadParametersBase::RMZero));
                  multResult *= value; source -= multResult;
               },
               source, value);
         return source;
      }
   template <int USizeMantissaArgument, int USizeExponentArgument, typename TypeImplementationArgument>
   friend thisType fmod(const thisType& source, const TInstrumentedFloatZonotope<USizeMantissaArgument, USizeExponentArgument, TypeImplementationArgument>& avalue)
      {  thisType value(avalue);
         thisType result;
         result.continuousFlow(
               [](thisType& multResult, const thisType& source, const thisType& value)
               {  auto divResult(source); divResult /= value;
                  multResult = thisType(divResult.asInt(thisType::ReadParametersBase::RMZero));
                  multResult *= value;
                  multResult -= source;
                  multResult.oppositeAssign();
               },
               source, value);
         return result;
      }
   friend thisType fmod(const thisType& source, long double avalue)
      {  thisType value(avalue);
         thisType result;
         result.continuousFlow(
               [](thisType& multResult, const thisType& source, const thisType& value)
               {  auto divResult(source); divResult /= value;
                  multResult = thisType(divResult.asInt(thisType::ReadParametersBase::RMZero));
                  multResult *= value;
                  multResult -= source;
                  multResult.oppositeAssign();
               },
               source, value);
         return result;
      }
   friend thisType fmod(const thisType& source, double avalue)
      {  thisType value(avalue);
         thisType result;
         result.continuousFlow(
               [](thisType& multResult, const thisType& source, const thisType& value)
               {  auto divResult(source); divResult /= value;
                  multResult = thisType(divResult.asInt(thisType::ReadParametersBase::RMZero));
                  multResult *= value;
                  multResult -= source;
                  multResult.oppositeAssign();
               },
               source, value);
         return result;
      }
   friend thisType fmod(const thisType& source, float avalue)
      {  thisType value(avalue);
         thisType result;
         result.continuousFlow(
               [](thisType& multResult, const thisType& source, const thisType& value)
               {  auto divResult(source); divResult /= value;
                  multResult = thisType(divResult.asInt(thisType::ReadParametersBase::RMZero));
                  multResult *= value;
                  multResult -= source;
                  multResult.oppositeAssign();
               },
               source, value);
         return result;
      }
   friend thisType fmod(const thisType& source, unsigned long avalue)
      {  thisType value(avalue);
         thisType result;
         result.continuousFlow(
               [](thisType& multResult, const thisType& source, const thisType& value)
               {  auto divResult(source); divResult /= value;
                  multResult = thisType(divResult.asInt(thisType::ReadParametersBase::RMZero));
                  multResult *= value;
                  multResult -= source;
                  multResult.oppositeAssign();
               },
               source, value);
         return result;
      }
   friend thisType fmod(const thisType& source, long avalue)
      {  thisType value(avalue);
         thisType result;
         result.continuousFlow(
               [](thisType& multResult, const thisType& source, const thisType& value)
               {  auto divResult(source); divResult /= value;
                  multResult = thisType(divResult.asInt(thisType::ReadParametersBase::RMZero));
                  multResult *= value;
                  multResult -= source;
                  multResult.oppositeAssign();
               },
               source, value);
         return result;
      }
   friend thisType fmod(const thisType& source, unsigned int avalue)
      {  thisType value(avalue);
         thisType result;
         result.continuousFlow(
               [](thisType& multResult, const thisType& source, const thisType& value)
               {  auto divResult(source); divResult /= value;
                  multResult = thisType(divResult.asInt(thisType::ReadParametersBase::RMZero));
                  multResult *= value;
                  multResult -= source;
                  multResult.oppositeAssign();
               },
               source, value);
         return result;
      }
   friend thisType fmod(const thisType& source, int avalue)
      {  thisType value(avalue);
         thisType result;
         result.continuousFlow(
               [](thisType& multResult, const thisType& source, const thisType& value)
               {  auto divResult(source); divResult /= value;
                  multResult = thisType(divResult.asInt(thisType::ReadParametersBase::RMZero));
                  multResult *= value;
                  multResult -= source;
                  multResult.oppositeAssign();
               },
               source, value);
         return result;
      }
   friend thisType fmod(const thisType& source, unsigned short avalue)
      {  thisType value(avalue);
         thisType result;
         result.continuousFlow(
               [](thisType& multResult, const thisType& source, const thisType& value)
               {  auto divResult(source); divResult /= value;
                  multResult = thisType(divResult.asInt(thisType::ReadParametersBase::RMZero));
                  multResult *= value;
                  multResult -= source;
                  multResult.oppositeAssign();
               },
               source, value);
         return result;
      }
   friend thisType fmod(const thisType& source, short avalue)
      {  thisType value(avalue);
         thisType result;
         result.continuousFlow(
               [](thisType& multResult, const thisType& source, const thisType& value)
               {  auto divResult(source); divResult /= value;
                  multResult = thisType(divResult.asInt(thisType::ReadParametersBase::RMZero));
                  multResult *= value;
                  multResult -= source;
                  multResult.oppositeAssign();
               },
               source, value);
         return result;
      }
   friend int fld_finite(const thisType& source) { return tfinite(source.asImplementation()); }
   friend int fld_isfinite(const thisType& source) { return tisfinite(source.asImplementation()); }
   friend int fld_isnan(const thisType& source) { return tisnan(source.asImplementation()); }
   friend int fld_isinf(const thisType& source) { return tisinf(source.asImplementation()); }
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

class ParseFloatZonotope : public FloatZonotope {
  public:
   ParseFloatZonotope(const char* value) : FloatZonotope(value, ValueFromString()) {}
};

class ParseDoubleZonotope : public DoubleZonotope {
  public:
   ParseDoubleZonotope(const char* value) : DoubleZonotope(value, ValueFromString()) {}
};

class ParseLongDoubleZonotope : public LongDoubleZonotope {
  public:
   ParseLongDoubleZonotope(const char* value) : LongDoubleZonotope(value, ValueFromString()) {}
};

} // end of namespace NumericalDomains

#endif // FloatInstrumentation_FloatAffineH

