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
   TSaveMemory<T, SaveMemory> operator<<(T t)
      {  return TSaveMemory<T, SaveMemory>(t, *this); }
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

   TMergeMemory(const T1&, TypeMergeMemory nextArg) : merge(), next(nextArg) {}
   TMergeMemory(const TMergeMemory<T1, TypeMergeMemory>&) = default;
   TMergeMemory(TMergeMemory<T1, TypeMergeMemory>&&) = default;

   template <typename T>
   TMergeMemory<T, TMergeMemory<T1, TypeMergeMemory> > operator>>(T& t)
      {  return TMergeMemory<T, TMergeMemory<T1, TypeMergeMemory> >(t, *this); }
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

class MergeMemory {
  private:
   bool fFirst;

  public:
   MergeMemory() : fFirst(true) {}

   template <typename T>
   TMergeMemory<T, MergeMemory> operator>>(T& t)
      {  return TMergeMemory<T, MergeMemory>(t, *this); }
   MergeMemory& operator>>(BaseExecutionPath::end) { return *this; }
   bool isFirst() const { return fFirst; }
   bool operator<<(BaseExecutionPath::end)
      {  fFirst = false;
         return true;
      }
};

template <class TypeBuiltDouble, typename TypeImplementation>
class TInstrumentedFloatInterval : public TFloatInterval<BaseFloatInterval, TypeBuiltDouble, TypeImplementation> {
  private:
   typedef TInstrumentedFloatInterval<TypeBuiltDouble, TypeImplementation> thisType;
   typedef TFloatInterval<BaseFloatInterval, TypeBuiltDouble, TypeImplementation> inherited;

  public:
   TInstrumentedFloatInterval() {}
   TInstrumentedFloatInterval(float value)
      {
         if (!inherited::fSupportAtomic)
            inherited::initFrom(value);
         else
            inherited::initFromAtomic(value);
      }
   TInstrumentedFloatInterval(double value)
      {
         if (!inherited::fSupportAtomic)
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
   TInstrumentedFloatInterval(int value) : inherited(value) {}
   TInstrumentedFloatInterval(long int value) : inherited(value) {}
   // TInstrumentedFloatInterval(unsigned value) : inherited(value) {}
   // TInstrumentedFloatInterval(unsigned long value) : inherited(value) {}
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

} // end of namespace NumericalDomains

template <class TypeBuiltDouble, typename TypeImplementation>
inline NumericalDomains::DDoubleInterval::TInstrumentedFloatInterval<TypeBuiltDouble, TypeImplementation>
sqrt(const NumericalDomains::DDoubleInterval::TInstrumentedFloatInterval<TypeBuiltDouble, TypeImplementation>& source)
   {  auto result(std::move(source)); result.sqrtAssign(); return result; }

template <class TypeBuiltDouble, typename TypeImplementation>
inline NumericalDomains::DDoubleInterval::TInstrumentedFloatInterval<TypeBuiltDouble, TypeImplementation>
sin(const NumericalDomains::DDoubleInterval::TInstrumentedFloatInterval<TypeBuiltDouble, TypeImplementation>& source)
   {  auto result(std::move(source)); result.sinAssign(); return result; }

template <class TypeBuiltDouble, typename TypeImplementation>
inline NumericalDomains::DDoubleInterval::TInstrumentedFloatInterval<TypeBuiltDouble, TypeImplementation>
cos(const NumericalDomains::DDoubleInterval::TInstrumentedFloatInterval<TypeBuiltDouble, TypeImplementation>& source)
   {  auto result(std::move(source)); result.cosAssign(); return result; }

template <class TypeBuiltDouble, typename TypeImplementation>
inline NumericalDomains::DDoubleInterval::TInstrumentedFloatInterval<TypeBuiltDouble, TypeImplementation>
asin(const NumericalDomains::DDoubleInterval::TInstrumentedFloatInterval<TypeBuiltDouble, TypeImplementation>& source)
   {  auto result(std::move(source)); result.asinAssign(); return result; }

template <class TypeBuiltDouble, typename TypeImplementation>
inline NumericalDomains::DDoubleInterval::TInstrumentedFloatInterval<TypeBuiltDouble, TypeImplementation>
acos(const NumericalDomains::DDoubleInterval::TInstrumentedFloatInterval<TypeBuiltDouble, TypeImplementation>& source)
   {  auto result(std::move(source)); result.acosAssign(); return result; }

template <class TypeBuiltDouble, typename TypeImplementation>
inline NumericalDomains::DDoubleInterval::TInstrumentedFloatInterval<TypeBuiltDouble, TypeImplementation>
tan(const NumericalDomains::DDoubleInterval::TInstrumentedFloatInterval<TypeBuiltDouble, TypeImplementation>& source)
   {  auto result(std::move(source)); result.tanAssign(); return result; }

template <class TypeBuiltDouble, typename TypeImplementation>
inline NumericalDomains::DDoubleInterval::TInstrumentedFloatInterval<TypeBuiltDouble, TypeImplementation>
atan(const NumericalDomains::DDoubleInterval::TInstrumentedFloatInterval<TypeBuiltDouble, TypeImplementation>& source)
   {  auto result(std::move(source)); result.atanAssign(); return result; }

template <class TypeBuiltDouble, typename TypeImplementation>
inline NumericalDomains::DDoubleInterval::TInstrumentedFloatInterval<TypeBuiltDouble, TypeImplementation>
exp(const NumericalDomains::DDoubleInterval::TInstrumentedFloatInterval<TypeBuiltDouble, TypeImplementation>& source)
   {  auto result(std::move(source)); result.expAssign(); return result; }

template <class TypeBuiltDouble, typename TypeImplementation>
inline NumericalDomains::DDoubleInterval::TInstrumentedFloatInterval<TypeBuiltDouble, TypeImplementation>
log(const NumericalDomains::DDoubleInterval::TInstrumentedFloatInterval<TypeBuiltDouble, TypeImplementation>& source)
   {  auto result(std::move(source)); result.logAssign(); return result; }

template <class TypeBuiltDouble, typename TypeImplementation>
inline NumericalDomains::DDoubleInterval::TInstrumentedFloatInterval<TypeBuiltDouble, TypeImplementation>
log10(const NumericalDomains::DDoubleInterval::TInstrumentedFloatInterval<TypeBuiltDouble, TypeImplementation>& source)
   {  auto result(std::move(source)); result.log10Assign(); return result; }

template <class TypeBuiltDouble, typename TypeImplementation>
inline NumericalDomains::DDoubleInterval::TInstrumentedFloatInterval<TypeBuiltDouble, TypeImplementation>
pow(const NumericalDomains::DDoubleInterval::TInstrumentedFloatInterval<TypeBuiltDouble, TypeImplementation>& source,
      const NumericalDomains::DDoubleInterval::TInstrumentedFloatInterval<TypeBuiltDouble, TypeImplementation>& value)
   {  auto result(std::move(source)); result.powAssign(value); return result; }

template <class TypeFst, class TypeBuiltDouble, typename TypeImplementation>
inline NumericalDomains::DDoubleInterval::TInstrumentedFloatInterval<TypeBuiltDouble, TypeImplementation>
pow(TypeFst source, const NumericalDomains::DDoubleInterval::TInstrumentedFloatInterval<TypeBuiltDouble, TypeImplementation>& value)
   {  NumericalDomains::DDoubleInterval::TInstrumentedFloatInterval<TypeBuiltDouble, TypeImplementation> result(source);
      result.powAssign(value);
      return result;
   }

template <class TypeSnd, class TypeBuiltDouble, typename TypeImplementation>
inline NumericalDomains::DDoubleInterval::TInstrumentedFloatInterval<TypeBuiltDouble, TypeImplementation>
pow(const NumericalDomains::DDoubleInterval::TInstrumentedFloatInterval<TypeBuiltDouble, TypeImplementation>& source,
      TypeSnd value)
   {  auto result(std::move(source));
      result.powAssign(NumericalDomains::DDoubleInterval::TInstrumentedFloatInterval<TypeBuiltDouble, TypeImplementation>(value));
      return result;
   }

template <class TypeBuiltDouble, typename TypeImplementation>
inline NumericalDomains::DDoubleInterval::TInstrumentedFloatInterval<TypeBuiltDouble, TypeImplementation>
powf(const NumericalDomains::DDoubleInterval::TInstrumentedFloatInterval<TypeBuiltDouble, TypeImplementation>& source,
      const NumericalDomains::DDoubleInterval::TInstrumentedFloatInterval<TypeBuiltDouble, TypeImplementation>& value)
   {  auto result(std::move(source)); result.powAssign(value); return result; }

template <class TypeFst, class TypeBuiltDouble, typename TypeImplementation>
inline NumericalDomains::DDoubleInterval::TInstrumentedFloatInterval<TypeBuiltDouble, TypeImplementation>
powf(TypeFst source, const NumericalDomains::DDoubleInterval::TInstrumentedFloatInterval<TypeBuiltDouble, TypeImplementation>& value)
   {  NumericalDomains::DDoubleInterval::TInstrumentedFloatInterval<TypeBuiltDouble, TypeImplementation> result(source);
      result.powAssign(value);
      return result;
   }

template <class TypeSnd, class TypeBuiltDouble, typename TypeImplementation>
inline NumericalDomains::DDoubleInterval::TInstrumentedFloatInterval<TypeBuiltDouble, TypeImplementation>
powf(const NumericalDomains::DDoubleInterval::TInstrumentedFloatInterval<TypeBuiltDouble, TypeImplementation>& source,
      TypeSnd value)
   {  auto result(std::move(source));
      result.powAssign(NumericalDomains::DDoubleInterval::TInstrumentedFloatInterval<TypeBuiltDouble, TypeImplementation>(value));
      return result;
   }

template <typename TypeFst, class TypeBuiltDouble, typename TypeImplementation>
inline bool
operator<(TypeFst fst, const NumericalDomains::DDoubleInterval::TInstrumentedFloatInterval<TypeBuiltDouble, TypeImplementation>& snd)
   {  return NumericalDomains::DDoubleInterval::TInstrumentedFloatInterval<TypeBuiltDouble, TypeImplementation>(fst).operator<(snd); }

template <typename TypeFst, class TypeBuiltDouble, typename TypeImplementation>
inline bool
operator<=(TypeFst fst, const NumericalDomains::DDoubleInterval::TInstrumentedFloatInterval<TypeBuiltDouble, TypeImplementation>& snd)
   {  return NumericalDomains::DDoubleInterval::TInstrumentedFloatInterval<TypeBuiltDouble, TypeImplementation>(fst).operator<=(snd); }

template <typename TypeFst, class TypeBuiltDouble, typename TypeImplementation>
inline bool
operator==(TypeFst fst, const NumericalDomains::DDoubleInterval::TInstrumentedFloatInterval<TypeBuiltDouble, TypeImplementation>& snd)
   {  return NumericalDomains::DDoubleInterval::TInstrumentedFloatInterval<TypeBuiltDouble, TypeImplementation>(fst).operator==(snd); }

template <typename TypeFst, class TypeBuiltDouble, typename TypeImplementation>
inline bool
operator!=(TypeFst fst, const NumericalDomains::DDoubleInterval::TInstrumentedFloatInterval<TypeBuiltDouble, TypeImplementation>& snd)
   {  return NumericalDomains::DDoubleInterval::TInstrumentedFloatInterval<TypeBuiltDouble, TypeImplementation>(fst).operator!=(snd); }

template <typename TypeFst, class TypeBuiltDouble, typename TypeImplementation>
inline bool
operator>=(TypeFst fst, const NumericalDomains::DDoubleInterval::TInstrumentedFloatInterval<TypeBuiltDouble, TypeImplementation>& snd)
   {  return NumericalDomains::DDoubleInterval::TInstrumentedFloatInterval<TypeBuiltDouble, TypeImplementation>(fst).operator>=(snd); }

template <typename TypeFst, class TypeBuiltDouble, typename TypeImplementation>
inline bool
operator>(TypeFst fst, const NumericalDomains::DDoubleInterval::TInstrumentedFloatInterval<TypeBuiltDouble, TypeImplementation>& snd)
   {  return NumericalDomains::DDoubleInterval::TInstrumentedFloatInterval<TypeBuiltDouble, TypeImplementation>(fst).operator>(snd); }

template <typename TypeFst, class TypeBuiltDouble, typename TypeImplementation>
inline NumericalDomains::DDoubleInterval::TInstrumentedFloatInterval<TypeBuiltDouble, TypeImplementation>
operator+(TypeFst fst, const NumericalDomains::DDoubleInterval::TInstrumentedFloatInterval<TypeBuiltDouble, TypeImplementation>& snd)
   {  return NumericalDomains::DDoubleInterval::TInstrumentedFloatInterval<TypeBuiltDouble, TypeImplementation>(fst).operator+(snd); }

template <typename TypeFst, class TypeBuiltDouble, typename TypeImplementation>
inline NumericalDomains::DDoubleInterval::TInstrumentedFloatInterval<TypeBuiltDouble, TypeImplementation>
operator-(TypeFst fst, const NumericalDomains::DDoubleInterval::TInstrumentedFloatInterval<TypeBuiltDouble, TypeImplementation>& snd)
   {  return NumericalDomains::DDoubleInterval::TInstrumentedFloatInterval<TypeBuiltDouble, TypeImplementation>(fst).operator-(snd); }

template <typename TypeFst, class TypeBuiltDouble, typename TypeImplementation>
inline NumericalDomains::DDoubleInterval::TInstrumentedFloatInterval<TypeBuiltDouble, TypeImplementation>
operator*(TypeFst fst, const NumericalDomains::DDoubleInterval::TInstrumentedFloatInterval<TypeBuiltDouble, TypeImplementation>& snd)
   {  return NumericalDomains::DDoubleInterval::TInstrumentedFloatInterval<TypeBuiltDouble, TypeImplementation>(fst).operator*(snd); }

template <typename TypeFst, class TypeBuiltDouble, typename TypeImplementation>
inline NumericalDomains::DDoubleInterval::TInstrumentedFloatInterval<TypeBuiltDouble, TypeImplementation>
operator/(TypeFst fst, const NumericalDomains::DDoubleInterval::TInstrumentedFloatInterval<TypeBuiltDouble, TypeImplementation>& snd)
   {  return NumericalDomains::DDoubleInterval::TInstrumentedFloatInterval<TypeBuiltDouble, TypeImplementation>(fst).operator/(snd); }

template <class TypeBuiltDouble, typename TypeImplementation>
inline NumericalDomains::DDoubleInterval::TInstrumentedFloatInterval<TypeBuiltDouble, TypeImplementation>
log2(const NumericalDomains::DDoubleInterval::TInstrumentedFloatInterval<TypeBuiltDouble, TypeImplementation>& source)
   {  typedef NumericalDomains::DDoubleInterval::TInstrumentedFloatInterval<TypeBuiltDouble, TypeImplementation> thisType;
      thisType result(std::move(source));
      result.logAssign();
      result.divAssign(thisType(::log(2.0)));
      return result;
   }

template <class TypeBuiltDouble, typename TypeImplementation>
inline NumericalDomains::DDoubleInterval::TInstrumentedFloatInterval<TypeBuiltDouble, TypeImplementation>
exp2(const NumericalDomains::DDoubleInterval::TInstrumentedFloatInterval<TypeBuiltDouble, TypeImplementation>& source)
   {  NumericalDomains::DDoubleInterval::TInstrumentedFloatInterval<TypeBuiltDouble, TypeImplementation>
         result = 2.0;
      result.powAssign(source);
      return result;
   }

template <class TypeBuiltDouble, typename TypeImplementation>
inline NumericalDomains::DDoubleInterval::TInstrumentedFloatInterval<TypeBuiltDouble, TypeImplementation>
floor(const NumericalDomains::DDoubleInterval::TInstrumentedFloatInterval<TypeBuiltDouble, TypeImplementation>& fst)
{  typedef NumericalDomains::DDoubleInterval::TInstrumentedFloatInterval<TypeBuiltDouble, TypeImplementation> thisType;
   return thisType(fst.asInt(thisType::ReadParametersBase::RMLowest));
}

template <class TypeBuiltDouble, typename TypeImplementation>
inline NumericalDomains::DDoubleInterval::TInstrumentedFloatInterval<TypeBuiltDouble, TypeImplementation>
floor(NumericalDomains::DDoubleInterval::TInstrumentedFloatInterval<TypeBuiltDouble, TypeImplementation>&& fst)
{  typedef NumericalDomains::DDoubleInterval::TInstrumentedFloatInterval<TypeBuiltDouble, TypeImplementation> thisType;
   return thisType(fst.asInt(thisType::ReadParametersBase::RMLowest));
}

template <class TypeBuiltDouble, typename TypeImplementation>
inline NumericalDomains::DDoubleInterval::TInstrumentedFloatInterval<TypeBuiltDouble, TypeImplementation>
ceil(const NumericalDomains::DDoubleInterval::TInstrumentedFloatInterval<TypeBuiltDouble, TypeImplementation>& fst)
{  typedef NumericalDomains::DDoubleInterval::TInstrumentedFloatInterval<TypeBuiltDouble, TypeImplementation> thisType;
   return thisType(fst.asInt(thisType::ReadParametersBase::RMHighest));
}

template <class TypeBuiltDouble, typename TypeImplementation>
inline NumericalDomains::DDoubleInterval::TInstrumentedFloatInterval<TypeBuiltDouble, TypeImplementation>
ceil(NumericalDomains::DDoubleInterval::TInstrumentedFloatInterval<TypeBuiltDouble, TypeImplementation>&& fst)
{  typedef NumericalDomains::DDoubleInterval::TInstrumentedFloatInterval<TypeBuiltDouble, TypeImplementation> thisType;
   return thisType(fst.asInt(thisType::ReadParametersBase::RMHighest));
}

template <class TypeBuiltDouble, typename TypeImplementation>
inline NumericalDomains::DDoubleInterval::TInstrumentedFloatInterval<TypeBuiltDouble, TypeImplementation>
trunc(const NumericalDomains::DDoubleInterval::TInstrumentedFloatInterval<TypeBuiltDouble, TypeImplementation>& fst)
{  typedef NumericalDomains::DDoubleInterval::TInstrumentedFloatInterval<TypeBuiltDouble, TypeImplementation> thisType;
   return thisType(fst.asInt(thisType::ReadParametersBase::RMZero));
}

template <class TypeBuiltDouble, typename TypeImplementation>
inline NumericalDomains::DDoubleInterval::TInstrumentedFloatInterval<TypeBuiltDouble, TypeImplementation>
trunc(NumericalDomains::DDoubleInterval::TInstrumentedFloatInterval<TypeBuiltDouble, TypeImplementation>&& fst)
{  typedef NumericalDomains::DDoubleInterval::TInstrumentedFloatInterval<TypeBuiltDouble, TypeImplementation> thisType;
   return thisType(fst.asInt(thisType::ReadParametersBase::RMZero));
}

template <class TypeBuiltDouble, typename TypeImplementation>
inline NumericalDomains::DDoubleInterval::TInstrumentedFloatInterval<TypeBuiltDouble, TypeImplementation>
round(const NumericalDomains::DDoubleInterval::TInstrumentedFloatInterval<TypeBuiltDouble, TypeImplementation>& fst)
{  typedef NumericalDomains::DDoubleInterval::TInstrumentedFloatInterval<TypeBuiltDouble, TypeImplementation> thisType;
   return thisType(fst.asInt(thisType::ReadParametersBase::RMNearest));
}

template <class TypeBuiltDouble, typename TypeImplementation>
inline NumericalDomains::DDoubleInterval::TInstrumentedFloatInterval<TypeBuiltDouble, TypeImplementation>
round(NumericalDomains::DDoubleInterval::TInstrumentedFloatInterval<TypeBuiltDouble, TypeImplementation>&& fst)
{  typedef NumericalDomains::DDoubleInterval::TInstrumentedFloatInterval<TypeBuiltDouble, TypeImplementation> thisType;
   return thisType(fst.asInt(thisType::ReadParametersBase::RMNearest));
}

template <class TypeBuiltDouble, typename TypeImplementation>
inline NumericalDomains::DDoubleInterval::TInstrumentedFloatInterval<TypeBuiltDouble, TypeImplementation>
rintf(const NumericalDomains::DDoubleInterval::TInstrumentedFloatInterval<TypeBuiltDouble, TypeImplementation>& fst)
{  typedef NumericalDomains::DDoubleInterval::TInstrumentedFloatInterval<TypeBuiltDouble, TypeImplementation> thisType;
   return thisType(fst.asInt(thisType::ReadParametersBase::RMNearest /* fegetround */));
}

template <class TypeBuiltDouble, typename TypeImplementation>
inline NumericalDomains::DDoubleInterval::TInstrumentedFloatInterval<TypeBuiltDouble, TypeImplementation>
rintf(NumericalDomains::DDoubleInterval::TInstrumentedFloatInterval<TypeBuiltDouble, TypeImplementation>&& fst)
{  typedef NumericalDomains::DDoubleInterval::TInstrumentedFloatInterval<TypeBuiltDouble, TypeImplementation> thisType;
   return thisType(fst.asInt(thisType::ReadParametersBase::RMNearest /* fegetround */));
}

template <class TypeBuiltDouble, typename TypeImplementation>
inline NumericalDomains::DDoubleInterval::TInstrumentedFloatInterval<TypeBuiltDouble, TypeImplementation>
rint(const NumericalDomains::DDoubleInterval::TInstrumentedFloatInterval<TypeBuiltDouble, TypeImplementation>& fst)
{  typedef NumericalDomains::DDoubleInterval::TInstrumentedFloatInterval<TypeBuiltDouble, TypeImplementation> thisType;
   return thisType(fst.asInt(thisType::ReadParametersBase::RMNearest /* fegetround */));
}

template <class TypeBuiltDouble, typename TypeImplementation>
inline NumericalDomains::DDoubleInterval::TInstrumentedFloatInterval<TypeBuiltDouble, TypeImplementation>
rint(NumericalDomains::DDoubleInterval::TInstrumentedFloatInterval<TypeBuiltDouble, TypeImplementation>&& fst)
{  typedef NumericalDomains::DDoubleInterval::TInstrumentedFloatInterval<TypeBuiltDouble, TypeImplementation> thisType;
   return thisType(fst.asInt(thisType::ReadParametersBase::RMNearest /* fegetround */));
}

template <class TypeBuiltDouble, typename TypeImplementation>
inline NumericalDomains::DDoubleInterval::TInstrumentedFloatInterval<TypeBuiltDouble, TypeImplementation>
fabs(const NumericalDomains::DDoubleInterval::TInstrumentedFloatInterval<TypeBuiltDouble, TypeImplementation>& fst)
{  typedef NumericalDomains::DDoubleInterval::TInstrumentedFloatInterval<TypeBuiltDouble, TypeImplementation> thisType;
   thisType result = fst;

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

template <class TypeBuiltDouble, typename TypeImplementation>
inline NumericalDomains::DDoubleInterval::TInstrumentedFloatInterval<TypeBuiltDouble, TypeImplementation>
fabs(NumericalDomains::DDoubleInterval::TInstrumentedFloatInterval<TypeBuiltDouble, TypeImplementation>&& fst)
{  typedef NumericalDomains::DDoubleInterval::TInstrumentedFloatInterval<TypeBuiltDouble, TypeImplementation> thisType;
   thisType result(std::forward<thisType>(fst));

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

#endif // FloatInstrumentation_FloatIntervalH

