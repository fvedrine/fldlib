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
// File      : FloatInterval.h
// Description :
//   Definition of a class of floating point intervals
//

#ifndef FloatInstrumentation_FloatIntervalH
#define FloatInstrumentation_FloatIntervalH

#include "NumericalAnalysis/FloatIntervalExecutionPath.h"

#include "FloatInstrumentation/FloatInterval.inch"

namespace NumericalDomains { namespace DDoubleInterval {

class MergeBranches {
  public:
   template <class TypeIterator>
   struct TPacker {
      TypeIterator iter, end;
      TPacker(TypeIterator aiter, TypeIterator aend) : iter(aiter), end(aend) {}
   };

   template <class TypeIterator>
   static TPacker<TypeIterator> packer(TypeIterator iter, TypeIterator end)
      {  return TPacker<TypeIterator>(iter, end); }
};

template <typename TypeIterator, class TypeSaveMemory>
class TPackedSaveMemory;

template <typename T1, class TypeSaveMemory>
class TSaveMemory {
  public:
   T1 save;
   TypeSaveMemory next;

   TSaveMemory(T1 saveArg, TypeSaveMemory nextArg) : save(saveArg), next(nextArg) {}
   TSaveMemory(const TSaveMemory<T1, TypeSaveMemory>&) = default;
   TSaveMemory(TSaveMemory<T1, TypeSaveMemory>&&) = default;

   template <typename T>
   TSaveMemory<T, TSaveMemory<T1, TypeSaveMemory> > operator<<(T t)
      {  return TSaveMemory<T, TSaveMemory<T1, TypeSaveMemory> >(t, *this); }
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
         for (; iter != end; ++iter)
            save.insertAtEnd(*iter);
      }
   TPackedSaveMemory(const TPackedSaveMemory<TypeIterator, TypeSaveMemory>&) = default;
   TPackedSaveMemory(TPackedSaveMemory<TypeIterator, TypeSaveMemory>&&) = default;

   template <typename T>
   TSaveMemory<T, TPackedSaveMemory<TypeIterator, TypeSaveMemory> > operator<<(T t)
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
   TSaveMemory<T, SaveMemory> operator<<(T t)
      {  return TSaveMemory<T, SaveMemory>(t, *this); }
   template <class TypeIterator>
   TPackedSaveMemory<TypeIterator, SaveMemory>
      operator<<(MergeBranches::TPacker<TypeIterator> packer)
      {  return TPackedSaveMemory<TypeIterator, SaveMemory>
            (packer.iter, packer.end, *this);
      }
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

   TMergeMemory(const T1&, TypeMergeMemory nextArg) : merge(), next(nextArg) {}
   TMergeMemory(const TMergeMemory<T1, TypeMergeMemory>&) = default;
   TMergeMemory(TMergeMemory<T1, TypeMergeMemory>&&) = default;

   template <typename T>
   TMergeMemory<T, TMergeMemory<T1, TypeMergeMemory> > operator>>(T& t)
      {  return TMergeMemory<T, TMergeMemory<T1, TypeMergeMemory> >(t, *this); }
   template <typename TypeIterator>
   TPackedMergeMemory<TypeIterator, TMergeMemory<T1, TypeMergeMemory> > operator>>(MergeBranches::TPacker<TypeIterator> packer);

   TMergeMemory<T1, TypeMergeMemory>& operator>>(BaseExecutionPath::end) { return *this; }
   TypeMergeMemory& operator<<(T1& val)
      {  if (next.isFirst())
            merge = val;
         else
            merge.mergeWith(val);
         val = merge;
         return next;
      }
   // to remove for the emission of compiler warnings
   TypeMergeMemory& operator<<(const T1& aval)
      {  T1& val = const_cast<T1&>(aval);
         if (next.isFirst())
            merge = val;
         else
            merge.mergeWith(val);
         val = merge;
         return next;
      }
   bool isFirst() const { return next.isFirst(); }
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

   TypeMergeMemory& operator<<(MergeBranches::TPacker<TypeIterator>&& packer)
      {  int count = packer.end - packer.iter;
         auto iter = packer.iter;
         if (next.isFirst()) {
            AssumeCondition(merge.count() == 0)
            merge.bookPlace(count);
            for (; iter != packer.end; ++iter)
               merge.insertAtEnd(*iter);
         }
         else {
            for (int index = 0; index < count; ++index) {
               merge.referenceAt(index).mergeWith(*iter);
               ++iter;
            }
         }
         for (int index = 0; index < count; ++index) {
            *packer.iter = merge[index];
            ++packer.iter;
         }
         return next;
      }
   bool isFirst() const { return next.isFirst(); }
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

  public:
   MergeMemory() : fFirst(true) {}

   template <typename T>
   TMergeMemory<T, MergeMemory> operator>>(T& t)
      {  return TMergeMemory<T, MergeMemory>(t, *this); }
   template <typename TypeIterator>
   TPackedMergeMemory<TypeIterator, MergeMemory> operator>>(MergeBranches::TPacker<TypeIterator>&& packer)
      {  return TPackedMergeMemory<TypeIterator, MergeMemory>(packer.iter, packer.end, *this); }
   MergeMemory& operator>>(BaseExecutionPath::end) { return *this; }
   bool isFirst() const { return fFirst; }
   bool operator<<(BaseExecutionPath::end)
      {  fFirst = false;
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

template <class TypeBuiltDouble, typename TypeImplementation>
class TInstrumentedFloatInterval : public TFloatInterval<BaseFloatInterval, TypeBuiltDouble, TypeImplementation> {
  private:
   typedef TInstrumentedFloatInterval<TypeBuiltDouble, TypeImplementation> thisType;
   typedef TFloatInterval<BaseFloatInterval, TypeBuiltDouble, TypeImplementation> inherited;

  public:
   typedef DDoubleInterval::MergeBranches MergeBranches;
   struct ValueFromString {};

  public:
   TInstrumentedFloatInterval() {}
   TInstrumentedFloatInterval(const char* value, ValueFromString)
      {  STG::IOObject::ISBase* in = BaseFloatInterval::acquireConstantStream(value);
         inherited::initFrom(*in);
         BaseFloatInterval::releaseConstantStream(in);
      }
   TInstrumentedFloatInterval(float value)
      {  if (!inherited::fSupportAtomic)
            inherited::initFrom(value);
         else
            inherited::initFromAtomic(value);
      }
   TInstrumentedFloatInterval(double value)
      {  if (!inherited::fSupportAtomic)
            inherited::initFrom(value);
         else
            inherited::initFromAtomic(value);
      }
   TInstrumentedFloatInterval(long double value);
/*
      {  if (!inherited::fSupportAtomic)
            inherited::initFrom(value);
         else
            inherited::initFromAtomic(value);
      }
*/
   TInstrumentedFloatInterval(TypeImplementation min, TypeImplementation max) : inherited(min, max) {}
   TInstrumentedFloatInterval(TypeImplementation min, TypeImplementation max,
         TypeImplementation errmin, TypeImplementation errmax)
      :  inherited(errmin < 0 ? min+errmin : min,
                   errmax > 0 ? max+errmax : max) {}
   TInstrumentedFloatInterval(short int value) : inherited(value) {}
   TInstrumentedFloatInterval(int value) : inherited(value) {}
   TInstrumentedFloatInterval(long int value) : inherited(value) {}
   TInstrumentedFloatInterval(unsigned short value) : inherited(value) {}
   TInstrumentedFloatInterval(unsigned value) : inherited(value) {}
   TInstrumentedFloatInterval(unsigned long value) : inherited(value) {}
   TInstrumentedFloatInterval(const thisType& source) = default;
   TInstrumentedFloatInterval(thisType&& source) = default; // [TODO] keep symbolic for constraints
   TInstrumentedFloatInterval& operator=(const thisType& source) = default;
   TInstrumentedFloatInterval& operator=(thisType&& source) = default; // [TODO] keep symbolic for constraints

   template <class TypeBuiltArgument, typename TypeImplementationArgument>
   TInstrumentedFloatInterval(const TInstrumentedFloatInterval<TypeBuiltArgument, TypeImplementationArgument>& source)
      :  inherited(source) {}
   template <class TypeBuiltArgument, typename TypeImplementationArgument>
   thisType& operator=(const TInstrumentedFloatInterval<TypeBuiltArgument, TypeImplementationArgument>& source)
      {  return (thisType&) inherited::operator=(source); }

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

   thisType& operator+=(const thisType& source) { return (thisType&) inherited::operator+=(source); }
   thisType& operator-=(const thisType& source) { return (thisType&) inherited::operator-=(source); }
   thisType& operator*=(const thisType& source) { return (thisType&) inherited::operator*=(source); }
   thisType& operator/=(const thisType& source) { return (thisType&) inherited::operator/=(source); }

   thisType operator+() const { return *this; }
   thisType operator-() const
      {  thisType result(*this);
         result.oppositeAssign();
         return result;
      }
   thisType operator+(const thisType& source) const
      {  thisType result(*this);
         result += source;
         return result;
      }
   thisType operator+(TypeImplementation source) const { return operator+(thisType(source)); }
   thisType operator-(const thisType& source) const
      {  thisType result(*this);
         result -= source;
         return result;
      }
   thisType operator-(TypeImplementation source) const { return operator-(thisType(source)); }
   thisType operator*(const thisType& source) const
      {  thisType result(*this);
         result *= source;
         return result;
      }
   thisType operator*(TypeImplementation source) const { return operator*(thisType(source)); }
   thisType operator/(const thisType& source) const
      {  thisType result(*this);
         result /= source;
         return result;
      }
   thisType operator/(TypeImplementation source) const { return operator/(thisType(source)); }

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
      {  return inherited::asLongInt(inherited::ReadParametersBase::RMZero); }
   explicit operator unsigned long() const
      { return inherited::asUnsignedLong(inherited::ReadParametersBase::RMZero); }

   thisType abs() const
      {  if (*this < 0)
            return -(*this);
         return *this;
      }
   const TypeBuiltDouble& min() const { return inherited::min(); }
   const TypeBuiltDouble& max() const { return inherited::max(); }
   thisType min(const thisType& source) const
      {  return (*this <= source) ? *this : source; }
   thisType min(TypeImplementation source) const
      {  thisType affineSource(source);
         return (*this <= affineSource) ? *this : affineSource;
      }
   thisType max(const thisType& source) const
      {  return (*this >= source) ? *this : source; }
   thisType max(TypeImplementation source) const
      {  thisType affineSource(source);
         return (*this >= affineSource) ? *this : affineSource;
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

   void persist(const char* prefix) { inherited::notifyForPersistence(*this, prefix); }

   friend std::ostream& operator<<(std::ostream& out, const thisType& source)
      {  return out << source.asImplementation(); }
   friend std::istream& operator>>(std::istream& in, thisType& source)
      {  TypeImplementation val;
         in >> val;
         source = thisType(val);
         return in;
      }

   friend thisType sqrt(const thisType& source)
      {  auto result(std::move(source)); result.sqrtAssign(); return result; }
   friend thisType sin(const thisType& source)
      {  auto result(std::move(source)); result.sinAssign(); return result; }
   friend thisType cos(const thisType& source)
      {  auto result(std::move(source)); result.cosAssign(); return result; }
   friend thisType asin(const thisType& source)
      {  auto result(std::move(source)); result.asinAssign(); return result; }
   friend thisType acos(const thisType& source)
      {  auto result(std::move(source)); result.acosAssign(); return result; }
   friend thisType tan(const thisType& source)
      {  auto result(std::move(source)); result.tanAssign(); return result; }
   friend thisType atan(const thisType& source)
      {  auto result(std::move(source)); result.atanAssign(); return result; }
   friend thisType exp(const thisType& source)
      {  auto result(std::move(source)); result.expAssign(); return result; }
   friend thisType log(const thisType& source)
      {  auto result(std::move(source)); result.logAssign(); return result; }
   friend thisType log2(const thisType& source)
      {  auto result(std::move(source)); result.logAssign();
         result.operator/=(log(thisType(2.0)));
         return result;
      }
   friend thisType exp2(const thisType& source)
      {  thisType result(2.0);
         result.powAssign(source);
         return result;
      }
   friend thisType log10(const thisType& source)
      {  auto result(std::move(source)); result.log10Assign(); return result; }

   friend thisType pow(const thisType& source, const thisType& value)
      {  auto result(std::move(source)); result.powAssign(value); return result; }
   template <class TypeBuiltArgument, typename TypeImplementationArgument>
   friend thisType pow(const TInstrumentedFloatInterval<TypeBuiltArgument, TypeImplementationArgument>& source, const thisType& value)
      {  thisType result(source); result.powAssign(value); return result; }
   friend thisType pow(long double source, const thisType& value)
      {  thisType result(source); result.powAssign(value); return result; }
   friend thisType pow(double source, const thisType& value)
      {  thisType result(source); result.powAssign(value); return result; }
   friend thisType pow(float source, const thisType& value)
      {  thisType result(source); result.powAssign(value); return result; }
   friend thisType pow(unsigned long source, const thisType& value)
      {  thisType result(source); result.powAssign(value); return result; }
   friend thisType pow(long source, const thisType& value)
      {  thisType result(source); result.powAssign(value); return result; }
   friend thisType pow(unsigned int source, const thisType& value)
      {  thisType result(source); result.powAssign(value); return result; }
   friend thisType pow(int source, const thisType& value)
      {  thisType result(source); result.powAssign(value); return result; }
   friend thisType pow(unsigned short source, const thisType& value)
      {  thisType result(source); result.powAssign(value); return result; }
   friend thisType pow(short source, const thisType& value)
      {  thisType result(source); result.powAssign(value); return result; }

   template <class TypeBuiltArgument, typename TypeImplementationArgument>
   friend thisType pow(const thisType& source, const TInstrumentedFloatInterval<TypeBuiltArgument, TypeImplementationArgument>& value)
      {  thisType result(source); result.powAssign(thisType(value)); return result; }
   friend thisType pow(const thisType& source, long double value)
      {  auto result(std::move(source)); result.powAssign(thisType(value)); return result; }
   friend thisType pow(const thisType& source, double value)
      {  auto result(std::move(source)); result.powAssign(thisType(value)); return result; }
   friend thisType pow(const thisType& source, float value)
      {  auto result(std::move(source)); result.powAssign(thisType(value)); return result; }
   friend thisType pow(const thisType& source, unsigned long value)
      {  auto result(std::move(source)); result.powAssign(thisType(value)); return result; }
   friend thisType pow(const thisType& source, long value)
      {  auto result(std::move(source)); result.powAssign(thisType(value)); return result; }
   friend thisType pow(const thisType& source, unsigned int value)
      {  auto result(std::move(source)); result.powAssign(thisType(value)); return result; }
   friend thisType pow(const thisType& source, int value)
      {  auto result(std::move(source)); result.powAssign(thisType(value)); return result; }
   friend thisType pow(const thisType& source, unsigned short value)
      {  auto result(std::move(source)); result.powAssign(thisType(value)); return result; }
   friend thisType pow(const thisType& source, short value)
      {  auto result(std::move(source)); result.powAssign(thisType(value)); return result; }

   friend thisType powf(const thisType& source, const thisType& value)
      {  auto result(std::move(source)); result.powAssign(value); return result; }
   template <class TypeBuiltArgument, typename TypeImplementationArgument>
   friend thisType powf(const TInstrumentedFloatInterval<TypeBuiltArgument, TypeImplementationArgument>& source, const thisType& value)
      {  thisType result(source); result.powAssign(value); return result; }
   friend thisType powf(long double source, const thisType& value)
      {  thisType result(source); result.powAssign(value); return result; }
   friend thisType powf(double source, const thisType& value)
      {  thisType result(source); result.powAssign(value); return result; }
   friend thisType powf(float source, const thisType& value)
      {  thisType result(source); result.powAssign(value); return result; }
   friend thisType powf(unsigned long source, const thisType& value)
      {  thisType result(source); result.powAssign(value); return result; }
   friend thisType powf(long source, const thisType& value)
      {  thisType result(source); result.powAssign(value); return result; }
   friend thisType powf(unsigned int source, const thisType& value)
      {  thisType result(source); result.powAssign(value); return result; }
   friend thisType powf(int source, const thisType& value)
      {  thisType result(source); result.powAssign(value); return result; }
   friend thisType powf(unsigned short source, const thisType& value)
      {  thisType result(source); result.powAssign(value); return result; }
   friend thisType powf(short source, const thisType& value)
      {  thisType result(source); result.powAssign(value); return result; }

   template <class TypeBuiltArgument, typename TypeImplementationArgument>
   friend thisType powf(const thisType& source, const TInstrumentedFloatInterval<TypeBuiltArgument, TypeImplementationArgument>& value)
      {  thisType result(source); result.powAssign(thisType(value)); return result; }
   friend thisType powf(const thisType& source, long double value)
      {  auto result(std::move(source)); result.powAssign(thisType(value)); return result; }
   friend thisType powf(const thisType& source, double value)
      {  auto result(std::move(source)); result.powAssign(thisType(value)); return result; }
   friend thisType powf(const thisType& source, float value)
      {  auto result(std::move(source)); result.powAssign(thisType(value)); return result; }
   friend thisType powf(const thisType& source, unsigned long value)
      {  auto result(std::move(source)); result.powAssign(thisType(value)); return result; }
   friend thisType powf(const thisType& source, long value)
      {  auto result(std::move(source)); result.powAssign(thisType(value)); return result; }
   friend thisType powf(const thisType& source, unsigned int value)
      {  auto result(std::move(source)); result.powAssign(thisType(value)); return result; }
   friend thisType powf(const thisType& source, int value)
      {  auto result(std::move(source)); result.powAssign(thisType(value)); return result; }
   friend thisType powf(const thisType& source, unsigned short value)
      {  auto result(std::move(source)); result.powAssign(thisType(value)); return result; }
   friend thisType powf(const thisType& source, short value)
      {  auto result(std::move(source)); result.powAssign(thisType(value)); return result; }

   friend thisType atan2(const thisType& source, const thisType& value)
      {  auto result(std::move(source)); result.atan2Assign(value); return result; }
   template <class TypeBuiltArgument, typename TypeImplementationArgument>
   friend thisType atan2(const TInstrumentedFloatInterval<TypeBuiltArgument, TypeImplementationArgument>& source, const thisType& value)
      {  thisType result(source); result.atan2Assign(value); return result; }
   friend thisType atan2(long double source, const thisType& value)
      {  thisType result(source); result.atan2Assign(value); return result; }
   friend thisType atan2(double source, const thisType& value)
      {  thisType result(source); result.atan2Assign(value); return result; }
   friend thisType atan2(float source, const thisType& value)
      {  thisType result(source); result.atan2Assign(value); return result; }
   friend thisType atan2(unsigned long source, const thisType& value)
      {  thisType result(source); result.atan2Assign(value); return result; }
   friend thisType atan2(long source, const thisType& value)
      {  thisType result(source); result.atan2Assign(value); return result; }
   friend thisType atan2(unsigned int source, const thisType& value)
      {  thisType result(source); result.atan2Assign(value); return result; }
   friend thisType atan2(int source, const thisType& value)
      {  thisType result(source); result.atan2Assign(value); return result; }
   friend thisType atan2(unsigned short source, const thisType& value)
      {  thisType result(source); result.atan2Assign(value); return result; }
   friend thisType atan2(short source, const thisType& value)
      {  thisType result(source); result.atan2Assign(value); return result; }

   template <class TypeBuiltArgument, typename TypeImplementationArgument>
   friend thisType atan2(const thisType& source, const TInstrumentedFloatInterval<TypeBuiltArgument, TypeImplementationArgument>& value)
      {  thisType result(source); result.atan2Assign(thisType(value)); return result; }
   friend thisType atan2(const thisType& source, long double value)
      {  auto result(std::move(source)); result.atan2Assign(thisType(value)); return result; }
   friend thisType atan2(const thisType& source, double value)
      {  auto result(std::move(source)); result.atan2Assign(thisType(value)); return result; }
   friend thisType atan2(const thisType& source, float value)
      {  auto result(std::move(source)); result.atan2Assign(thisType(value)); return result; }
   friend thisType atan2(const thisType& source, unsigned long value)
      {  auto result(std::move(source)); result.atan2Assign(thisType(value)); return result; }
   friend thisType atan2(const thisType& source, long value)
      {  auto result(std::move(source)); result.atan2Assign(thisType(value)); return result; }
   friend thisType atan2(const thisType& source, unsigned int value)
      {  auto result(std::move(source)); result.atan2Assign(thisType(value)); return result; }
   friend thisType atan2(const thisType& source, int value)
      {  auto result(std::move(source)); result.atan2Assign(thisType(value)); return result; }
   friend thisType atan2(const thisType& source, unsigned short value)
      {  auto result(std::move(source)); result.atan2Assign(thisType(value)); return result; }
   friend thisType atan2(const thisType& source, short value)
      {  auto result(std::move(source)); result.atan2Assign(thisType(value)); return result; }

   template <class TypeBuiltArgument, typename TypeImplementationArgument>
   friend bool operator<(const TInstrumentedFloatInterval<TypeBuiltArgument, TypeImplementationArgument>& fst, const thisType& snd)
      {  return thisType(fst).operator<(snd); }    
   friend bool operator<(long double fst, const thisType& snd) { return thisType(fst).operator<(snd); }
   friend bool operator<(double fst, const thisType& snd) { return thisType(fst).operator<(snd); }
   friend bool operator<(float fst, const thisType& snd) { return thisType(fst).operator<(snd); }
   friend bool operator<(unsigned long fst, const thisType& snd) { return thisType(fst).operator<(snd); }
   friend bool operator<(long fst, const thisType& snd) { return thisType(fst).operator<(snd); }
   friend bool operator<(unsigned int fst, const thisType& snd) { return thisType(fst).operator<(snd); }
   friend bool operator<(int fst, const thisType& snd) { return thisType(fst).operator<(snd); }
   friend bool operator<(unsigned short fst, const thisType& snd) { return thisType(fst).operator<(snd); }
   friend bool operator<(short fst, const thisType& snd) { return thisType(fst).operator<(snd); }

   template <class TypeBuiltArgument, typename TypeImplementationArgument>
   friend bool operator<=(const TInstrumentedFloatInterval<TypeBuiltArgument, TypeImplementationArgument>& fst, const thisType& snd)
      {  return thisType(fst).operator<=(snd); }    
   friend bool operator<=(long double fst, const thisType& snd) { return thisType(fst).operator<=(snd); }
   friend bool operator<=(double fst, const thisType& snd) { return thisType(fst).operator<=(snd); }
   friend bool operator<=(float fst, const thisType& snd) { return thisType(fst).operator<=(snd); }
   friend bool operator<=(unsigned long fst, const thisType& snd) { return thisType(fst).operator<=(snd); }
   friend bool operator<=(long fst, const thisType& snd) { return thisType(fst).operator<=(snd); }
   friend bool operator<=(unsigned int fst, const thisType& snd) { return thisType(fst).operator<=(snd); }
   friend bool operator<=(int fst, const thisType& snd) { return thisType(fst).operator<=(snd); }
   friend bool operator<=(unsigned short fst, const thisType& snd) { return thisType(fst).operator<=(snd); }
   friend bool operator<=(short fst, const thisType& snd) { return thisType(fst).operator<=(snd); }

   template <class TypeBuiltArgument, typename TypeImplementationArgument>
   friend bool operator==(const TInstrumentedFloatInterval<TypeBuiltArgument, TypeImplementationArgument>& fst, const thisType& snd)
      {  return thisType(fst).operator==(snd); }    
   friend bool operator==(long double fst, const thisType& snd) { return thisType(fst).operator==(snd); }
   friend bool operator==(double fst, const thisType& snd) { return thisType(fst).operator==(snd); }
   friend bool operator==(float fst, const thisType& snd) { return thisType(fst).operator==(snd); }
   friend bool operator==(unsigned long fst, const thisType& snd) { return thisType(fst).operator==(snd); }
   friend bool operator==(long fst, const thisType& snd) { return thisType(fst).operator==(snd); }
   friend bool operator==(unsigned int fst, const thisType& snd) { return thisType(fst).operator==(snd); }
   friend bool operator==(int fst, const thisType& snd) { return thisType(fst).operator==(snd); }
   friend bool operator==(unsigned short fst, const thisType& snd) { return thisType(fst).operator==(snd); }
   friend bool operator==(short fst, const thisType& snd) { return thisType(fst).operator==(snd); }

   template <class TypeBuiltArgument, typename TypeImplementationArgument>
   friend bool operator!=(const TInstrumentedFloatInterval<TypeBuiltArgument, TypeImplementationArgument>& fst, const thisType& snd)
      {  return thisType(fst).operator!=(snd); }    
   friend bool operator!=(long double fst, const thisType& snd) { return thisType(fst).operator!=(snd); }
   friend bool operator!=(double fst, const thisType& snd) { return thisType(fst).operator!=(snd); }
   friend bool operator!=(float fst, const thisType& snd) { return thisType(fst).operator!=(snd); }
   friend bool operator!=(unsigned long fst, const thisType& snd) { return thisType(fst).operator!=(snd); }
   friend bool operator!=(long fst, const thisType& snd) { return thisType(fst).operator!=(snd); }
   friend bool operator!=(unsigned int fst, const thisType& snd) { return thisType(fst).operator!=(snd); }
   friend bool operator!=(int fst, const thisType& snd) { return thisType(fst).operator!=(snd); }
   friend bool operator!=(unsigned short fst, const thisType& snd) { return thisType(fst).operator!=(snd); }
   friend bool operator!=(short fst, const thisType& snd) { return thisType(fst).operator!=(snd); }

   template <class TypeBuiltArgument, typename TypeImplementationArgument>
   friend bool operator>=(const TInstrumentedFloatInterval<TypeBuiltArgument, TypeImplementationArgument>& fst, const thisType& snd)
      {  return thisType(fst).operator>=(snd); }    
   friend bool operator>=(long double fst, const thisType& snd) { return thisType(fst).operator>=(snd); }
   friend bool operator>=(double fst, const thisType& snd) { return thisType(fst).operator>=(snd); }
   friend bool operator>=(float fst, const thisType& snd) { return thisType(fst).operator>=(snd); }
   friend bool operator>=(unsigned long fst, const thisType& snd) { return thisType(fst).operator>=(snd); }
   friend bool operator>=(long fst, const thisType& snd) { return thisType(fst).operator>=(snd); }
   friend bool operator>=(unsigned int fst, const thisType& snd) { return thisType(fst).operator>=(snd); }
   friend bool operator>=(int fst, const thisType& snd) { return thisType(fst).operator>=(snd); }
   friend bool operator>=(unsigned short fst, const thisType& snd) { return thisType(fst).operator>=(snd); }
   friend bool operator>=(short fst, const thisType& snd) { return thisType(fst).operator>=(snd); }

   template <class TypeBuiltArgument, typename TypeImplementationArgument>
   friend bool operator>(const TInstrumentedFloatInterval<TypeBuiltArgument, TypeImplementationArgument>& fst, const thisType& snd)
      {  return thisType(fst).operator>(snd); }    
   friend bool operator>(long double fst, const thisType& snd) { return thisType(fst).operator>(snd); }
   friend bool operator>(double fst, const thisType& snd) { return thisType(fst).operator>(snd); }
   friend bool operator>(float fst, const thisType& snd) { return thisType(fst).operator>(snd); }
   friend bool operator>(unsigned long fst, const thisType& snd) { return thisType(fst).operator>(snd); }
   friend bool operator>(long fst, const thisType& snd) { return thisType(fst).operator>(snd); }
   friend bool operator>(unsigned int fst, const thisType& snd) { return thisType(fst).operator>(snd); }
   friend bool operator>(int fst, const thisType& snd) { return thisType(fst).operator>(snd); }
   friend bool operator>(unsigned short fst, const thisType& snd) { return thisType(fst).operator>(snd); }
   friend bool operator>(short fst, const thisType& snd) { return thisType(fst).operator>(snd); }

   template <class TypeBuiltArgument, typename TypeImplementationArgument>
   friend thisType operator+(const TInstrumentedFloatInterval<TypeBuiltArgument, TypeImplementationArgument>& fst, const thisType& snd)
      {  return thisType(fst).operator+(snd); }    
   friend thisType operator+(long double fst, const thisType& snd) { return thisType(fst).operator+(snd); }
   friend thisType operator+(double fst, const thisType& snd) { return thisType(fst).operator+(snd); }
   friend thisType operator+(float fst, const thisType& snd) { return thisType(fst).operator+(snd); }
   friend thisType operator+(unsigned long fst, const thisType& snd) { return thisType(fst).operator+(snd); }
   friend thisType operator+(long fst, const thisType& snd) { return thisType(fst).operator+(snd); }
   friend thisType operator+(unsigned int fst, const thisType& snd) { return thisType(fst).operator+(snd); }
   friend thisType operator+(int fst, const thisType& snd) { return thisType(fst).operator+(snd); }
   friend thisType operator+(unsigned short fst, const thisType& snd) { return thisType(fst).operator+(snd); }
   friend thisType operator+(short fst, const thisType& snd) { return thisType(fst).operator+(snd); }

   template <class TypeBuiltArgument, typename TypeImplementationArgument>
   friend thisType operator-(const TInstrumentedFloatInterval<TypeBuiltArgument, TypeImplementationArgument>& fst, const thisType& snd)
      {  return thisType(fst).operator-(snd); }    
   friend thisType operator-(long double fst, const thisType& snd) { return thisType(fst).operator-(snd); }
   friend thisType operator-(double fst, const thisType& snd) { return thisType(fst).operator-(snd); }
   friend thisType operator-(float fst, const thisType& snd) { return thisType(fst).operator-(snd); }
   friend thisType operator-(unsigned long fst, const thisType& snd) { return thisType(fst).operator-(snd); }
   friend thisType operator-(long fst, const thisType& snd) { return thisType(fst).operator-(snd); }
   friend thisType operator-(unsigned int fst, const thisType& snd) { return thisType(fst).operator-(snd); }
   friend thisType operator-(int fst, const thisType& snd) { return thisType(fst).operator-(snd); }
   friend thisType operator-(unsigned short fst, const thisType& snd) { return thisType(fst).operator-(snd); }
   friend thisType operator-(short fst, const thisType& snd) { return thisType(fst).operator-(snd); }

   template <class TypeBuiltArgument, typename TypeImplementationArgument>
   friend thisType operator*(const TInstrumentedFloatInterval<TypeBuiltArgument, TypeImplementationArgument>& fst, const thisType& snd)
      {  return thisType(fst).operator*(snd); }    
   friend thisType operator*(long double fst, const thisType& snd) { return thisType(fst).operator*(snd); }
   friend thisType operator*(double fst, const thisType& snd) { return thisType(fst).operator*(snd); }
   friend thisType operator*(float fst, const thisType& snd) { return thisType(fst).operator*(snd); }
   friend thisType operator*(unsigned long fst, const thisType& snd) { return thisType(fst).operator*(snd); }
   friend thisType operator*(long fst, const thisType& snd) { return thisType(fst).operator*(snd); }
   friend thisType operator*(unsigned int fst, const thisType& snd) { return thisType(fst).operator*(snd); }
   friend thisType operator*(int fst, const thisType& snd) { return thisType(fst).operator*(snd); }
   friend thisType operator*(unsigned short fst, const thisType& snd) { return thisType(fst).operator*(snd); }
   friend thisType operator*(short fst, const thisType& snd) { return thisType(fst).operator*(snd); }

   template <class TypeBuiltArgument, typename TypeImplementationArgument>
   friend thisType operator/(const TInstrumentedFloatInterval<TypeBuiltArgument, TypeImplementationArgument>& fst, const thisType& snd)
      {  return thisType(fst).operator/(snd); }    
   friend thisType operator/(long double fst, const thisType& snd) { return thisType(fst).operator/(snd); }
   friend thisType operator/(double fst, const thisType& snd) { return thisType(fst).operator/(snd); }
   friend thisType operator/(float fst, const thisType& snd) { return thisType(fst).operator/(snd); }
   friend thisType operator/(unsigned long fst, const thisType& snd) { return thisType(fst).operator/(snd); }
   friend thisType operator/(long fst, const thisType& snd) { return thisType(fst).operator/(snd); }
   friend thisType operator/(unsigned int fst, const thisType& snd) { return thisType(fst).operator/(snd); }
   friend thisType operator/(int fst, const thisType& snd) { return thisType(fst).operator/(snd); }
   friend thisType operator/(unsigned short fst, const thisType& snd) { return thisType(fst).operator/(snd); }
   friend thisType operator/(short fst, const thisType& snd) { return thisType(fst).operator/(snd); }

   friend thisType floor(const thisType& fst)
      {  return thisType(std::forward<thisType>(thisType(fst)).asInt(thisType::ReadParametersBase::RMLowest)); }
   friend thisType floor(thisType&& fst)
      {  return thisType(std::forward<thisType>(thisType(fst)).asInt(thisType::ReadParametersBase::RMLowest)); }
   friend thisType ceil(const thisType& fst)
      {  return thisType(std::forward<thisType>(thisType(fst)).asInt(thisType::ReadParametersBase::RMHighest)); }
   friend thisType ceil(thisType&& fst)
      {  return thisType(std::forward<thisType>(thisType(fst)).asInt(thisType::ReadParametersBase::RMHighest)); }
   friend thisType trunc(const thisType& fst)
      {  return thisType(std::forward<thisType>(thisType(fst)).asInt(thisType::ReadParametersBase::RMZero)); }
   friend thisType trunc(thisType&& fst)
      {  return thisType(std::forward<thisType>(thisType(fst)).asInt(thisType::ReadParametersBase::RMZero)); }
   friend thisType round(const thisType& fst)
      {  return thisType(std::forward<thisType>(thisType(fst)).asInt(thisType::ReadParametersBase::RMNearest)); }
   friend thisType round(thisType&& fst)
      {  return thisType(std::forward<thisType>(thisType(fst)).asInt(thisType::ReadParametersBase::RMNearest)); }
   friend thisType rint(const thisType& fst)
      {  return thisType(fst.asInt(thisType::ReadParametersBase::RMNearest /* fegetround */)); }
   friend thisType rintf(const thisType& fst)
      {  return thisType(fst.asInt(thisType::ReadParametersBase::RMNearest /* fegetround */)); }
   friend thisType fabs(const thisType& source)
      {  thisType result = source;
         // see float_diagnosis.h FLOAT_SPLIT_ALL FLOAT_MERGE_ALL
         auto* oldPathExplorer = NumericalDomains::DDoubleInterval::ExecutionPath::getCurrentPathExplorer();
         bool oldDoesFollow = NumericalDomains::DDoubleInterval::ExecutionPath::doesFollowFlow();
         NumericalDomains::DDoubleInterval::ExecutionPath::clearFollowFlow();
         auto* oldInputTraceFile = NumericalDomains::DDoubleInterval::ExecutionPath::inputTraceFile();
         NumericalDomains::DDoubleInterval::PathExplorer pathExplorer;
         NumericalDomains::DDoubleInterval::ExecutionPath::setCurrentPathExplorer(&pathExplorer);
         auto mergeMemory = NumericalDomains::DDoubleInterval::MergeMemory() >> result;
         auto saveMemory = NumericalDomains::DDoubleInterval::SaveMemory() << result;
         do {
            if (result < 0)
               result.oppositeAssign();
            NumericalDomains::DDoubleInterval::ExecutionPath::setFollowFlow();
         } while ((mergeMemory << result)
               && !(saveMemory.setCurrentResult(pathExplorer.isFinished()) >> result));
         NumericalDomains::DDoubleInterval::ExecutionPath::setFollowFlow(oldDoesFollow, oldInputTraceFile);
         NumericalDomains::DDoubleInterval::ExecutionPath::setCurrentPathExplorer(oldPathExplorer);
         return result;
      }
   friend thisType fabs(thisType&& source)
      {  thisType result(std::forward<thisType>(source));
         // see float_diagnosis.h FLOAT_SPLIT_ALL FLOAT_MERGE_ALL
         auto* oldPathExplorer = NumericalDomains::DDoubleInterval::ExecutionPath::getCurrentPathExplorer();
         bool oldDoesFollow = NumericalDomains::DDoubleInterval::ExecutionPath::doesFollowFlow();
         NumericalDomains::DDoubleInterval::ExecutionPath::clearFollowFlow();
         auto* oldInputTraceFile = NumericalDomains::DDoubleInterval::ExecutionPath::inputTraceFile();
         NumericalDomains::DDoubleInterval::PathExplorer pathExplorer;
         NumericalDomains::DDoubleInterval::ExecutionPath::setCurrentPathExplorer(&pathExplorer);
         auto mergeMemory = NumericalDomains::DDoubleInterval::MergeMemory() >> result;
         auto saveMemory = NumericalDomains::DDoubleInterval::SaveMemory() << result;
         do {
            if (result < 0)
               result.oppositeAssign();
            NumericalDomains::DDoubleInterval::ExecutionPath::setFollowFlow();
         } while ((mergeMemory << result)
               && !(saveMemory.setCurrentResult(pathExplorer.isFinished()) >> result));
         NumericalDomains::DDoubleInterval::ExecutionPath::setFollowFlow(oldDoesFollow, oldInputTraceFile);
         NumericalDomains::DDoubleInterval::ExecutionPath::setCurrentPathExplorer(oldPathExplorer);

         return result;
      }
   friend thisType abs(const thisType& source)
      {  thisType result = source;
         // see float_diagnosis.h FLOAT_SPLIT_ALL FLOAT_MERGE_ALL
         auto* oldPathExplorer = NumericalDomains::DDoubleInterval::ExecutionPath::getCurrentPathExplorer();
         bool oldDoesFollow = NumericalDomains::DDoubleInterval::ExecutionPath::doesFollowFlow();
         NumericalDomains::DDoubleInterval::ExecutionPath::clearFollowFlow();
         auto* oldInputTraceFile = NumericalDomains::DDoubleInterval::ExecutionPath::inputTraceFile();
         NumericalDomains::DDoubleInterval::PathExplorer pathExplorer;
         NumericalDomains::DDoubleInterval::ExecutionPath::setCurrentPathExplorer(&pathExplorer);
         auto mergeMemory = NumericalDomains::DDoubleInterval::MergeMemory() >> result;
         auto saveMemory = NumericalDomains::DDoubleInterval::SaveMemory() << result;
         do {
            if (result < 0)
               result.oppositeAssign();
            NumericalDomains::DDoubleInterval::ExecutionPath::setFollowFlow();
         } while ((mergeMemory << result)
               && !(saveMemory.setCurrentResult(pathExplorer.isFinished()) >> result));
         NumericalDomains::DDoubleInterval::ExecutionPath::setFollowFlow(oldDoesFollow, oldInputTraceFile);
         NumericalDomains::DDoubleInterval::ExecutionPath::setCurrentPathExplorer(oldPathExplorer);
         return result;
      }
   friend thisType abs(thisType&& source)
      {  thisType result(std::forward<thisType>(source));
         // see float_diagnosis.h FLOAT_SPLIT_ALL FLOAT_MERGE_ALL
         auto* oldPathExplorer = NumericalDomains::DDoubleInterval::ExecutionPath::getCurrentPathExplorer();
         bool oldDoesFollow = NumericalDomains::DDoubleInterval::ExecutionPath::doesFollowFlow();
         NumericalDomains::DDoubleInterval::ExecutionPath::clearFollowFlow();
         auto* oldInputTraceFile = NumericalDomains::DDoubleInterval::ExecutionPath::inputTraceFile();
         NumericalDomains::DDoubleInterval::PathExplorer pathExplorer;
         NumericalDomains::DDoubleInterval::ExecutionPath::setCurrentPathExplorer(&pathExplorer);
         auto mergeMemory = NumericalDomains::DDoubleInterval::MergeMemory() >> result;
         auto saveMemory = NumericalDomains::DDoubleInterval::SaveMemory() << result;
         do {
            if (result < 0)
               result.oppositeAssign();
            NumericalDomains::DDoubleInterval::ExecutionPath::setFollowFlow();
         } while ((mergeMemory << result)
               && !(saveMemory.setCurrentResult(pathExplorer.isFinished()) >> result));
         NumericalDomains::DDoubleInterval::ExecutionPath::setFollowFlow(oldDoesFollow, oldInputTraceFile);
         NumericalDomains::DDoubleInterval::ExecutionPath::setCurrentPathExplorer(oldPathExplorer);

         return result;
      }
   friend thisType fmod(const thisType& source, const thisType& value)
      {  auto divResult(source);
         divResult /= value;
         thisType multResult(divResult.asInt(thisType::ReadParametersBase::RMZero));
         multResult *= value;
         multResult -= source;
         multResult.oppositeAssign();
         return multResult;
      }
   friend thisType fmod(long double source, const thisType& value)
      {  thisType fst(source);
         auto divResult(fst); divResult /= value;
         thisType multResult(divResult.asInt(thisType::ReadParametersBase::RMZero));
         multResult *= value; fst -= multResult;
         return fst;
      }
   friend thisType fmod(double source, const thisType& value)
      {  thisType fst(source);
         auto divResult(fst); divResult /= value;
         thisType multResult(divResult.asInt(thisType::ReadParametersBase::RMZero));
         multResult *= value; fst -= multResult;
         return fst;
      }
   friend thisType fmod(float source, const thisType& value)
      {  thisType fst(source);
         auto divResult(fst); divResult /= value;
         thisType multResult(divResult.asInt(thisType::ReadParametersBase::RMZero));
         multResult *= value; fst -= multResult;
         return fst;
      }
   friend thisType fmod(unsigned long source, const thisType& value)
      {  thisType fst(source);
         auto divResult(fst); divResult /= value;
         thisType multResult(divResult.asInt(thisType::ReadParametersBase::RMZero));
         multResult *= value; fst -= multResult;
         return fst;
      }
   friend thisType fmod(long source, const thisType& value)
      {  thisType fst(source);
         auto divResult(fst); divResult /= value;
         thisType multResult(divResult.asInt(thisType::ReadParametersBase::RMZero));
         multResult *= value; fst -= multResult;
         return fst;
      }
   friend thisType fmod(unsigned int source, const thisType& value)
      {  thisType fst(source);
         auto divResult(fst); divResult /= value;
         thisType multResult(divResult.asInt(thisType::ReadParametersBase::RMZero));
         multResult *= value; fst -= multResult;
         return fst;
      }
   friend thisType fmod(int source, const thisType& value)
      {  thisType fst(source);
         auto divResult(fst); divResult /= value;
         thisType multResult(divResult.asInt(thisType::ReadParametersBase::RMZero));
         multResult *= value; fst -= multResult;
         return fst;
      }
   friend thisType fmod(unsigned short source, const thisType& value)
      {  thisType fst(source);
         auto divResult(fst); divResult /= value;
         thisType multResult(divResult.asInt(thisType::ReadParametersBase::RMZero));
         multResult *= value; fst -= multResult;
         return fst;
      }
   friend thisType fmod(short source, const thisType& value)
      {  thisType fst(source);
         auto divResult(fst); divResult /= value;
         thisType multResult(divResult.asInt(thisType::ReadParametersBase::RMZero));
         multResult *= value; fst -= multResult;
         return fst;
      }
   friend thisType fmod(const thisType& source, long double value)
      {  auto divResult(source); thisType snd(value); divResult /= snd;
         thisType multResult(divResult.asInt(thisType::ReadParametersBase::RMZero));
         multResult *= snd; multResult -= source; multResult.oppositeAssign();
         return multResult;
      }
   friend thisType fmod(const thisType& source, double value)
      {  auto divResult(source); thisType snd(value); divResult /= snd;
         thisType multResult(divResult.asInt(thisType::ReadParametersBase::RMZero));
         multResult *= snd; multResult -= source; multResult.oppositeAssign();
         return multResult;
      }
   friend thisType fmod(const thisType& source, float value)
      {  auto divResult(source); thisType snd(value); divResult /= snd;
         thisType multResult(divResult.asInt(thisType::ReadParametersBase::RMZero));
         multResult *= snd; multResult -= source; multResult.oppositeAssign();
         return multResult;
      }
   friend thisType fmod(const thisType& source, unsigned long value)
      {  auto divResult(source); thisType snd(value); divResult /= snd;
         thisType multResult(divResult.asInt(thisType::ReadParametersBase::RMZero));
         multResult *= snd; multResult -= source; multResult.oppositeAssign();
         return multResult;
      }
   friend thisType fmod(const thisType& source, long value)
      {  auto divResult(source); thisType snd(value); divResult /= snd;
         thisType multResult(divResult.asInt(thisType::ReadParametersBase::RMZero));
         multResult *= snd; multResult -= source; multResult.oppositeAssign();
         return multResult;
      }
   friend thisType fmod(const thisType& source, unsigned int value)
      {  auto divResult(source); thisType snd(value); divResult /= snd;
         thisType multResult(divResult.asInt(thisType::ReadParametersBase::RMZero));
         multResult *= snd; multResult -= source; multResult.oppositeAssign();
         return multResult;
      }
   friend thisType fmod(const thisType& source, int value)
      {  auto divResult(source); thisType snd(value); divResult /= snd;
         thisType multResult(divResult.asInt(thisType::ReadParametersBase::RMZero));
         multResult *= snd; multResult -= source; multResult.oppositeAssign();
         return multResult;
      }
   friend thisType fmod(const thisType& source, unsigned short value)
      {  auto divResult(source); thisType snd(value); divResult /= snd;
         thisType multResult(divResult.asInt(thisType::ReadParametersBase::RMZero));
         multResult *= snd; multResult -= source; multResult.oppositeAssign();
         return multResult;
      }
   friend thisType fmod(const thisType& source, short value)
      {  auto divResult(source); thisType snd(value); divResult /= snd;
         thisType multResult(divResult.asInt(thisType::ReadParametersBase::RMZero));
         multResult *= snd; multResult -= source; multResult.oppositeAssign();
         return multResult;
      }

   friend int fld_finite(const thisType& source) { return tfinite(source.asImplementation()); }
   friend int fld_isfinite(const thisType& source) { return tisfinite(source.asImplementation()); }
   friend int fld_isnan(const thisType& source) { return tisnan(source.asImplementation()); }
   friend int fld_isinf(const thisType& source) { return tisinf(source.asImplementation()); }
};

template <class TypeBuiltDouble, typename TypeImplementation>
inline
TInstrumentedFloatInterval<TypeBuiltDouble, TypeImplementation>::TInstrumentedFloatInterval(
      long double value) {
   TFloatInterval<BaseFloatInterval, DDoubleInterval::BuiltLongDouble, long double> receiver;
   if (!inherited::fSupportAtomic)
      receiver.initFrom(value);
   else
      receiver.initFromAtomic(value);
   inherited::operator=(std::move(receiver));
}

} // end of namespace DDoubleInterval

typedef DDoubleInterval::TInstrumentedFloatInterval<DDoubleInterval::BuiltFloat, float> FloatInterval;
typedef DDoubleInterval::TInstrumentedFloatInterval<DDoubleInterval::BuiltDouble, double> DoubleInterval;
typedef DDoubleInterval::TInstrumentedFloatInterval<DDoubleInterval::BuiltLongDouble, long double> LongDoubleInterval;

class ParseFloatInterval : public FloatInterval {
  public:
   ParseFloatInterval(const char* value) : FloatInterval(value, ValueFromString()) {}
};

class ParseDoubleInterval : public DoubleInterval {
  public:
   ParseDoubleInterval(const char* value) : DoubleInterval(value, ValueFromString()) {}
};

class ParseLongDoubleInterval : public LongDoubleInterval {
  public:
   ParseLongDoubleInterval(const char* value) : LongDoubleInterval(value, ValueFromString()) {}
};

} // end of namespace NumericalDomains

#endif // FloatInstrumentation_FloatIntervalH

