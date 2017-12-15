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
// File      : FloatExact.h
// Description :
//   Definition of a class of floating point comparison
//

#ifndef FloatInstrumentation_FloatExactH
#define FloatInstrumentation_FloatExactH

#include "NumericalAnalysis/FloatExactExecutionPath.h"
#include "FloatInstrumentation/FloatExact.inch"

namespace NumericalDomains { namespace DDoubleExact {

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
   TSaveMemory<T1, TypeSaveMemory>& setCurrent(bool result)
      {  next.setCurrent(result); return *this; }
   TypeSaveMemory& operator>>(T1& val)
      {  if (!next.getResult())
            val = save;
         return next;
      }
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
   SaveMemory& setCurrent(bool result) { fResult = result; return *this; }
   bool getResult() const { return fResult; }
   bool operator>>(BaseExecutionPath::end)
      {  bool result = fResult;
         if (fResult)
            fResult = false;
         return result;
      }
};

template <class TypeBuiltDouble, typename TypeImplementation>
class TInstrumentedFloat : public TFloatExact<ExecutionPath, TypeBuiltDouble, TypeImplementation> {
  private:
   typedef TFloatExact<ExecutionPath, TypeBuiltDouble, TypeImplementation> inherited;
   typedef TInstrumentedFloat<TypeBuiltDouble, TypeImplementation> thisType;

  public:
   class ErrorParameter {};

   TInstrumentedFloat() {}
   TInstrumentedFloat(float value) { inherited::initFrom(value); }
   TInstrumentedFloat(double value) { inherited::initFrom(value); }
   TInstrumentedFloat(long double value) { inherited::initFrom(value); }
   TInstrumentedFloat(int value) : inherited(value) {}
   TInstrumentedFloat(long int value) : inherited(value) {}
   // TInstrumentedFloat(unsigned value) : inherited(value) {}
   // TInstrumentedFloat(unsigned long value) : inherited(value) {}
   TInstrumentedFloat(TypeImplementation value, TypeImplementation error, ErrorParameter)
      {  inherited::initFrom(value);
         inherited addition;
         addition.initFrom(error);
         addition.clearImplementation();
         inherited::operator+=(addition);
      }
   TInstrumentedFloat(const thisType& source) = default;
   TInstrumentedFloat& operator=(const thisType& source) = default;

   template <class TypeBuiltArgument, typename TypeImplementationArgument>
   TInstrumentedFloat(const TInstrumentedFloat<TypeBuiltArgument, TypeImplementationArgument>& source)
      :  inherited(source) {}
   template <class TypeBuiltArgument, typename TypeImplementationArgument>
   thisType& operator=(const TInstrumentedFloat<TypeBuiltArgument, TypeImplementationArgument>& source)
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

   thisType operator+() const
      {  return *this; }
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
   explicit operator int() const { return inherited::asInt(); }
   explicit operator short int() const { return inherited::asInt(); }
   explicit operator unsigned() const { return inherited::asUnsigned(); }
   explicit operator short unsigned() const { return inherited::asUnsigned(); }
   explicit operator long int() const { return inherited::asLongInt(); }
   explicit operator unsigned long() const { return inherited::asUnsignedLong(); }

   thisType abs() const
      {  auto result(*this); result.absAssign(); return result; }
   thisType min(const thisType& source) const
      {  auto result(*this); result.minAssign(source); return result; }
   thisType min(TypeImplementation source) const
      {  auto result(*this); result.minAssign(thisType(source)); return result; }
   thisType max(const thisType& source) const
      {  auto result(*this); result.maxAssign(source); return result; }
   thisType max(TypeImplementation source) const
      {  auto result(*this); result.maxAssign(thisType(source)); return result; }
   thisType median(const thisType& fst, const thisType& snd) const
      {  auto result(*this); result.medianAssign(fst, snd); return result; }
   thisType median(TypeImplementation fst, const thisType& snd) const
      {  auto result(*this); result.medianAssign(thisType(fst), snd); return result; }
   thisType median(const thisType& fst, TypeImplementation snd) const
      {  auto result(*this); result.medianAssign(fst, thisType(snd)); return result; }
   thisType median(TypeImplementation fst, TypeImplementation snd) const
      {  auto result(*this); result.medianAssign(thisType(fst), thisType(snd)); return result; }

   void persist(const char* prefix) { inherited::notifyForPersistence(*this, prefix); }
};

} // end of namespace DDoubleExact

typedef DDoubleExact::TInstrumentedFloat<DDoubleExact::BuiltFloat, float> FloatExact;
typedef DDoubleExact::TInstrumentedFloat<DDoubleExact::BuiltDouble, double> DoubleExact;
typedef DDoubleExact::TInstrumentedFloat<DDoubleExact::BuiltLongDouble, long double> LongDoubleExact;

} // end of namespace NumericalDomains

template <class TypeBuiltDouble, typename TypeImplementation>
inline NumericalDomains::DDoubleExact::TInstrumentedFloat<TypeBuiltDouble, TypeImplementation>
sqrt(const NumericalDomains::DDoubleExact::TInstrumentedFloat<TypeBuiltDouble, TypeImplementation>& source)
   {  auto result(std::move(source)); result.sqrtAssign(); return result; }

template <class TypeBuiltDouble, typename TypeImplementation>
inline NumericalDomains::DDoubleExact::TInstrumentedFloat<TypeBuiltDouble, TypeImplementation>
sin(const NumericalDomains::DDoubleExact::TInstrumentedFloat<TypeBuiltDouble, TypeImplementation>& source)
   {  auto result(std::move(source)); result.sinAssign(); return result; }

template <class TypeBuiltDouble, typename TypeImplementation>
inline NumericalDomains::DDoubleExact::TInstrumentedFloat<TypeBuiltDouble, TypeImplementation>
cos(const NumericalDomains::DDoubleExact::TInstrumentedFloat<TypeBuiltDouble, TypeImplementation>& source)
   {  auto result(std::move(source)); result.cosAssign(); return result; }

template <class TypeBuiltDouble, typename TypeImplementation>
inline NumericalDomains::DDoubleExact::TInstrumentedFloat<TypeBuiltDouble, TypeImplementation>
asin(const NumericalDomains::DDoubleExact::TInstrumentedFloat<TypeBuiltDouble, TypeImplementation>& source)
   {  auto result(std::move(source)); result.asinAssign(); return result; }

template <class TypeBuiltDouble, typename TypeImplementation>
inline NumericalDomains::DDoubleExact::TInstrumentedFloat<TypeBuiltDouble, TypeImplementation>
acos(const NumericalDomains::DDoubleExact::TInstrumentedFloat<TypeBuiltDouble, TypeImplementation>& source)
   {  auto result(std::move(source)); result.acosAssign(); return result; }

template <class TypeBuiltDouble, typename TypeImplementation>
inline NumericalDomains::DDoubleExact::TInstrumentedFloat<TypeBuiltDouble, TypeImplementation>
tan(const NumericalDomains::DDoubleExact::TInstrumentedFloat<TypeBuiltDouble, TypeImplementation>& source)
   {  auto result(std::move(source)); result.tanAssign(); return result; }

template <class TypeBuiltDouble, typename TypeImplementation>
inline NumericalDomains::DDoubleExact::TInstrumentedFloat<TypeBuiltDouble, TypeImplementation>
atan(const NumericalDomains::DDoubleExact::TInstrumentedFloat<TypeBuiltDouble, TypeImplementation>& source)
   {  auto result(std::move(source)); result.atanAssign(); return result; }

template <class TypeBuiltDouble, typename TypeImplementation>
inline NumericalDomains::DDoubleExact::TInstrumentedFloat<TypeBuiltDouble, TypeImplementation>
exp(const NumericalDomains::DDoubleExact::TInstrumentedFloat<TypeBuiltDouble, TypeImplementation>& source)
   {  auto result(std::move(source)); result.expAssign(); return result; }

template <class TypeBuiltDouble, typename TypeImplementation>
inline NumericalDomains::DDoubleExact::TInstrumentedFloat<TypeBuiltDouble, TypeImplementation>
log(const NumericalDomains::DDoubleExact::TInstrumentedFloat<TypeBuiltDouble, TypeImplementation>& source)
   {  auto result(std::move(source)); result.logAssign(); return result; }

template <class TypeBuiltDouble, typename TypeImplementation>
inline NumericalDomains::DDoubleExact::TInstrumentedFloat<TypeBuiltDouble, TypeImplementation>
log2(const NumericalDomains::DDoubleExact::TInstrumentedFloat<TypeBuiltDouble, TypeImplementation>& source)
   {  typedef NumericalDomains::DDoubleExact::TInstrumentedFloat<TypeBuiltDouble, TypeImplementation> thisType;
      auto result(std::move(source)); result.logAssign(); result.operator/=(thisType(log(2))); return result;
   }

template <class TypeBuiltDouble, typename TypeImplementation>
inline NumericalDomains::DDoubleExact::TInstrumentedFloat<TypeBuiltDouble, TypeImplementation>
exp2(const NumericalDomains::DDoubleExact::TInstrumentedFloat<TypeBuiltDouble, TypeImplementation>& source)
   {  NumericalDomains::DDoubleExact::TInstrumentedFloat<TypeBuiltDouble, TypeImplementation> result(2.0);
      result.powAssign(source);
      return result;
   }

template <class TypeBuiltDouble, typename TypeImplementation>
inline NumericalDomains::DDoubleExact::TInstrumentedFloat<TypeBuiltDouble, TypeImplementation>
log10(const NumericalDomains::DDoubleExact::TInstrumentedFloat<TypeBuiltDouble, TypeImplementation>& source)
   {  auto result(std::move(source)); result.log10Assign(); return result; }

template <class TypeBuiltDouble, typename TypeImplementation>
inline NumericalDomains::DDoubleExact::TInstrumentedFloat<TypeBuiltDouble, TypeImplementation>
pow(const NumericalDomains::DDoubleExact::TInstrumentedFloat<TypeBuiltDouble, TypeImplementation>& source,
      const NumericalDomains::DDoubleExact::TInstrumentedFloat<TypeBuiltDouble, TypeImplementation>& value)
   {  auto result(std::move(source)); result.powAssign(value); return result; }

template <typename TypeFst, class TypeBuiltDouble, typename TypeImplementation>
inline NumericalDomains::DDoubleExact::TInstrumentedFloat<TypeBuiltDouble, TypeImplementation>
pow(TypeFst source, const NumericalDomains::DDoubleExact::TInstrumentedFloat<TypeBuiltDouble, TypeImplementation>& value)
   {  NumericalDomains::DDoubleExact::TInstrumentedFloat<TypeBuiltDouble, TypeImplementation> result(source);
      result.powAssign(value);
      return result;
   }

template <typename TypeSnd, class TypeBuiltDouble, typename TypeImplementation>
inline NumericalDomains::DDoubleExact::TInstrumentedFloat<TypeBuiltDouble, TypeImplementation>
pow(const NumericalDomains::DDoubleExact::TInstrumentedFloat<TypeBuiltDouble, TypeImplementation>& source,
      TypeSnd value)
   {  auto result(std::move(source));
      result.powAssign(NumericalDomains::DDoubleExact::TInstrumentedFloat<TypeBuiltDouble, TypeImplementation>(value));
      return result;
   }

template <class TypeBuiltDouble, typename TypeImplementation>
inline NumericalDomains::DDoubleExact::TInstrumentedFloat<TypeBuiltDouble, TypeImplementation>
powf(const NumericalDomains::DDoubleExact::TInstrumentedFloat<TypeBuiltDouble, TypeImplementation>& source,
      const NumericalDomains::DDoubleExact::TInstrumentedFloat<TypeBuiltDouble, TypeImplementation>& value)
   {  auto result(std::move(source)); result.powAssign(value); return result; }

template <typename TypeFst, class TypeBuiltDouble, typename TypeImplementation>
inline NumericalDomains::DDoubleExact::TInstrumentedFloat<TypeBuiltDouble, TypeImplementation>
powf(TypeFst source, const NumericalDomains::DDoubleExact::TInstrumentedFloat<TypeBuiltDouble, TypeImplementation>& value)
   {  NumericalDomains::DDoubleExact::TInstrumentedFloat<TypeBuiltDouble, TypeImplementation> result(source);
      result.powAssign(value);
      return result;
   }

template <typename TypeSnd, class TypeBuiltDouble, typename TypeImplementation>
inline NumericalDomains::DDoubleExact::TInstrumentedFloat<TypeBuiltDouble, TypeImplementation>
powf(const NumericalDomains::DDoubleExact::TInstrumentedFloat<TypeBuiltDouble, TypeImplementation>& source,
      TypeSnd value)
   {  auto result(std::move(source));
      result.powAssign(NumericalDomains::DDoubleExact::TInstrumentedFloat<TypeBuiltDouble, TypeImplementation>(value));
      return result;
   }

template <typename TypeFst, class TypeBuiltDouble, typename TypeImplementation>
inline bool
operator<(TypeFst fst, const NumericalDomains::DDoubleExact::TInstrumentedFloat<TypeBuiltDouble, TypeImplementation>& snd)
   {  return NumericalDomains::DDoubleExact::TInstrumentedFloat<TypeBuiltDouble, TypeImplementation>(fst).operator<(snd); }

template <typename TypeFst, class TypeBuiltDouble, typename TypeImplementation>
inline bool
operator<=(TypeFst fst, const NumericalDomains::DDoubleExact::TInstrumentedFloat<TypeBuiltDouble, TypeImplementation>& snd)
   {  return NumericalDomains::DDoubleExact::TInstrumentedFloat<TypeBuiltDouble, TypeImplementation>(fst).operator<=(snd); }

template <typename TypeFst, class TypeBuiltDouble, typename TypeImplementation>
inline bool
operator==(TypeFst fst, const NumericalDomains::DDoubleExact::TInstrumentedFloat<TypeBuiltDouble, TypeImplementation>& snd)
   {  return NumericalDomains::DDoubleExact::TInstrumentedFloat<TypeBuiltDouble, TypeImplementation>(fst).operator==(snd); }

template <typename TypeFst, class TypeBuiltDouble, typename TypeImplementation>
inline bool
operator!=(TypeFst fst, const NumericalDomains::DDoubleExact::TInstrumentedFloat<TypeBuiltDouble, TypeImplementation>& snd)
   {  return NumericalDomains::DDoubleExact::TInstrumentedFloat<TypeBuiltDouble, TypeImplementation>(fst).operator!=(snd); }

template <typename TypeFst, class TypeBuiltDouble, typename TypeImplementation>
inline bool
operator>=(TypeFst fst, const NumericalDomains::DDoubleExact::TInstrumentedFloat<TypeBuiltDouble, TypeImplementation>& snd)
   {  return NumericalDomains::DDoubleExact::TInstrumentedFloat<TypeBuiltDouble, TypeImplementation>(fst).operator>=(snd); }

template <typename TypeFst, class TypeBuiltDouble, typename TypeImplementation>
inline bool
operator>(TypeFst fst, const NumericalDomains::DDoubleExact::TInstrumentedFloat<TypeBuiltDouble, TypeImplementation>& snd)
   {  return NumericalDomains::DDoubleExact::TInstrumentedFloat<TypeBuiltDouble, TypeImplementation>(fst).operator>(snd); }

template <typename TypeFst, class TypeBuiltDouble, typename TypeImplementation>
inline NumericalDomains::DDoubleExact::TInstrumentedFloat<TypeBuiltDouble, TypeImplementation>
operator+(TypeFst fst, const NumericalDomains::DDoubleExact::TInstrumentedFloat<TypeBuiltDouble, TypeImplementation>& snd)
   {  return NumericalDomains::DDoubleExact::TInstrumentedFloat<TypeBuiltDouble, TypeImplementation>(fst).operator+(snd); }

template <typename TypeFst, class TypeBuiltDouble, typename TypeImplementation>
inline NumericalDomains::DDoubleExact::TInstrumentedFloat<TypeBuiltDouble, TypeImplementation>
operator-(TypeFst fst, const NumericalDomains::DDoubleExact::TInstrumentedFloat<TypeBuiltDouble, TypeImplementation>& snd)
   {  return NumericalDomains::DDoubleExact::TInstrumentedFloat<TypeBuiltDouble, TypeImplementation>(fst).operator-(snd); }

template <typename TypeFst, class TypeBuiltDouble, typename TypeImplementation>
inline NumericalDomains::DDoubleExact::TInstrumentedFloat<TypeBuiltDouble, TypeImplementation>
operator*(TypeFst fst, const NumericalDomains::DDoubleExact::TInstrumentedFloat<TypeBuiltDouble, TypeImplementation>& snd)
   {  return NumericalDomains::DDoubleExact::TInstrumentedFloat<TypeBuiltDouble, TypeImplementation>(fst).operator*(snd); }

template <typename TypeFst, class TypeBuiltDouble, typename TypeImplementation>
inline NumericalDomains::DDoubleExact::TInstrumentedFloat<TypeBuiltDouble, TypeImplementation>
operator/(TypeFst fst, const NumericalDomains::DDoubleExact::TInstrumentedFloat<TypeBuiltDouble, TypeImplementation>& snd)
   {  return NumericalDomains::DDoubleExact::TInstrumentedFloat<TypeBuiltDouble, TypeImplementation>(fst).operator/(snd); }

template <class TypeBuiltDouble, typename TypeImplementation>
inline NumericalDomains::DDoubleExact::TInstrumentedFloat<TypeBuiltDouble, TypeImplementation>
floor(const NumericalDomains::DDoubleExact::TInstrumentedFloat<TypeBuiltDouble, TypeImplementation>& fst)
   {  typedef NumericalDomains::DDoubleExact::TInstrumentedFloat<TypeBuiltDouble, TypeImplementation> thisType;
      return thisType(std::forward<thisType>(thisType(fst)).operator int());
   }

template <class TypeBuiltDouble, typename TypeImplementation>
inline NumericalDomains::DDoubleExact::TInstrumentedFloat<TypeBuiltDouble, TypeImplementation>
floor(NumericalDomains::DDoubleExact::TInstrumentedFloat<TypeBuiltDouble, TypeImplementation>&& fst)
   {  typedef NumericalDomains::DDoubleExact::TInstrumentedFloat<TypeBuiltDouble, TypeImplementation> thisType;
      return thisType(std::forward<thisType>(thisType(fst)).operator int());
   }

template <class TypeBuiltDouble, typename TypeImplementation>
inline int
rint(const NumericalDomains::DDoubleExact::TInstrumentedFloat<TypeBuiltDouble, TypeImplementation>& fst)
   {  // typedef NumericalDomains::DDoubleExact::TInstrumentedFloat<TypeBuiltDouble, TypeImplementation> thisType;
      return fst.operator int();
   }

template <class TypeBuiltDouble, typename TypeImplementation>
inline int
rintf(const NumericalDomains::DDoubleExact::TInstrumentedFloat<TypeBuiltDouble, TypeImplementation>& fst)
   {  // typedef NumericalDomains::DDoubleExact::TInstrumentedFloat<TypeBuiltDouble, TypeImplementation> thisType;
      return fst.operator int();
   }

template <class TypeBuiltDouble, typename TypeImplementation>
inline NumericalDomains::DDoubleExact::TInstrumentedFloat<TypeBuiltDouble, TypeImplementation>
fabs(const NumericalDomains::DDoubleExact::TInstrumentedFloat<TypeBuiltDouble, TypeImplementation>& source)
   {  auto result(std::move(source)); result.absAssign(); return result; }

template <class TypeBuiltDouble, typename TypeImplementation>
inline NumericalDomains::DDoubleExact::TInstrumentedFloat<TypeBuiltDouble, TypeImplementation>
fabs(NumericalDomains::DDoubleExact::TInstrumentedFloat<TypeBuiltDouble, TypeImplementation>&& source)
   {  auto result(source); result.absAssign(); return result; }

#endif // FloatInstrumentation_FloatExactH

