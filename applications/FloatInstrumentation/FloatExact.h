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
   TSaveMemory<T, TSaveMemory<TypeIterator, TypeSaveMemory> > operator<<(T t)
      {  return TSaveMemory<T, TSaveMemory<TypeIterator, TypeSaveMemory> >(t, *this); }
   template <class TypeIteratorArgument>
   TPackedSaveMemory<TypeIteratorArgument, TPackedSaveMemory<TypeIterator, TypeSaveMemory> >
      operator<<(MergeBranches::TPacker<TypeIteratorArgument> packer)
      {  return TPackedSaveMemory<TypeIteratorArgument, TPackedSaveMemory<TypeIterator, TypeSaveMemory> >
            (packer.iter, packer.end, *this);
      }
   TPackedSaveMemory<TypeIterator, TypeSaveMemory>& operator<<(BaseExecutionPath::end) { return *this; }

   TPackedSaveMemory<TypeIterator, TypeSaveMemory>& setCurrent(bool result)
      {  next.setCurrent(result); return *this; }
   
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
   SaveMemory& setCurrent(bool result) { fResult = result; return *this; }
   bool getResult() const { return fResult; }
   bool operator>>(BaseExecutionPath::end)
      {  bool result = fResult;
         if (fResult)
            fResult = false;
         return result;
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
class TInstrumentedFloat : public TFloatExact<ExecutionPath, TypeBuiltDouble, TypeImplementation> {
  private:
   typedef TFloatExact<ExecutionPath, TypeBuiltDouble, TypeImplementation> inherited;
   typedef TInstrumentedFloat<TypeBuiltDouble, TypeImplementation> thisType;

  public:
   class ErrorParameter {};
   typedef DDoubleExact::MergeBranches MergeBranches;

   TInstrumentedFloat() {}
   TInstrumentedFloat(float value) { inherited::initFrom(value); }
   TInstrumentedFloat(double value) { inherited::initFrom(value); }
   TInstrumentedFloat(long double value); // { inherited::initFrom(value); }
   TInstrumentedFloat(short int value) : inherited(value) {}
   TInstrumentedFloat(int value) : inherited(value) {}
   TInstrumentedFloat(long int value) : inherited(value) {}
   TInstrumentedFloat(unsigned short value) : inherited(value) {}
   TInstrumentedFloat(unsigned int value) : inherited(value) {}
   TInstrumentedFloat(unsigned long value) : inherited(value) {}
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
      {  return inherited::asUnsignedLong(inherited::ReadParametersBase::RMZero); }

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

   void persist(const char* prefix) const { inherited::notifyForPersistence(*this, prefix); }

   friend std::ostream& operator<<(std::ostream& out, const thisType& source)
      {  return out << source.asImplementation(); }
   friend std::istream& operator>>(std::istream& in, thisType& source)
      {  decltype(thisType.asImplementation()) val;
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
      {  auto result(std::move(source)); result.logAssign(); result.operator/=(log(thisType(2.0))); return result; }
   friend thisType exp2(const thisType& source)
      {  thisType result(2.0); result.powAssign(source); return result; }
   friend thisType log10(const thisType& source)
      {  auto result(std::move(source)); result.log10Assign(); return result; }

   friend thisType pow(const thisType& source, const thisType& value)
      {  auto result(std::move(source)); result.powAssign(value); return result; }
   template <class TypeBuiltArgument, typename TypeImplementationArgument>
   friend thisType pow(const TInstrumentedFloat<TypeBuiltArgument, TypeImplementationArgument>& source, const thisType& value)
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
   friend thisType pow(const thisType& source, const TInstrumentedFloat<TypeBuiltArgument, TypeImplementationArgument>& value)
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
   friend thisType powf(const TInstrumentedFloat<TypeBuiltArgument, TypeImplementationArgument>& source, const thisType& value)
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
   friend thisType powf(const thisType& source, const TInstrumentedFloat<TypeBuiltArgument, TypeImplementationArgument>& value)
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
   friend thisType atan2(const TInstrumentedFloat<TypeBuiltArgument, TypeImplementationArgument>& source, const thisType& value)
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
   friend thisType atan2(const thisType& source, const TInstrumentedFloat<TypeBuiltArgument, TypeImplementationArgument>& value)
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
   friend bool operator<(const TInstrumentedFloat<TypeBuiltArgument, TypeImplementationArgument>& fst, const thisType& snd)
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
   friend bool operator<=(const TInstrumentedFloat<TypeBuiltArgument, TypeImplementationArgument>& fst, const thisType& snd)
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
   friend bool operator==(const TInstrumentedFloat<TypeBuiltArgument, TypeImplementationArgument>& fst, const thisType& snd)
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
   friend bool operator!=(const TInstrumentedFloat<TypeBuiltArgument, TypeImplementationArgument>& fst, const thisType& snd)
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
   friend bool operator>=(const TInstrumentedFloat<TypeBuiltArgument, TypeImplementationArgument>& fst, const thisType& snd)
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
   friend bool operator>(const TInstrumentedFloat<TypeBuiltArgument, TypeImplementationArgument>& fst, const thisType& snd)
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
   friend thisType operator+(const TInstrumentedFloat<TypeBuiltArgument, TypeImplementationArgument>& fst, const thisType& snd)
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
   friend thisType operator-(const TInstrumentedFloat<TypeBuiltArgument, TypeImplementationArgument>& fst, const thisType& snd)
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
   friend thisType operator*(const TInstrumentedFloat<TypeBuiltArgument, TypeImplementationArgument>& fst, const thisType& snd)
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
   friend thisType operator/(const TInstrumentedFloat<TypeBuiltArgument, TypeImplementationArgument>& fst, const thisType& snd)
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
      {  auto result(std::move(source)); result.absAssign(); return result; }
   friend thisType fabs(thisType&& source)
      {  auto result(source); result.absAssign(); return result; }
   friend thisType abs(const thisType& source)
      {  auto result(std::move(source)); result.absAssign(); return result; }
   friend thisType abs(thisType&& source)
      {  auto result(source); result.absAssign(); return result; }
   friend thisType fmod(const thisType& source, const thisType& value)
      {  auto divResult(source); divResult /= value;
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
TInstrumentedFloat<TypeBuiltDouble, TypeImplementation>::TInstrumentedFloat(long double value) {
   TFloatExact<ExecutionPath, BuiltLongDouble, long double> receiver;
   receiver.initFrom(value);
   inherited::operator=(std::move(receiver));
}

} // end of namespace DDoubleExact

typedef DDoubleExact::TInstrumentedFloat<DDoubleExact::BuiltFloat, float> FloatExact;
typedef DDoubleExact::TInstrumentedFloat<DDoubleExact::BuiltDouble, double> DoubleExact;
typedef DDoubleExact::TInstrumentedFloat<DDoubleExact::BuiltLongDouble, long double> LongDoubleExact;

} // end of namespace NumericalDomains

#endif // FloatInstrumentation_FloatExactH

