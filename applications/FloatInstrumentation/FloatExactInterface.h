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
// File      : FloatExactInterface.h
// Description :
//   Definition of a class of floating point comparison
//

#ifndef FloatInstrumentation_FloatExactH
#define FloatInstrumentation_FloatExactH

// for log
#include <math.h>
#include "FloatInstrumentation/ExactTypesSize.h"

// #include <iosfwd>
#include <iostream>

namespace NumericalDomains { namespace DDoubleExactInterface {

class end {};

class ExecutionPath {
  public:
   static void splitBranches(const char* file, int line);
   static void flushOut();
   static void setSourceLine(const char* file, int line);
   static int getCurrentUnstableBranch();
   static void writeCurrentPath(std::ostream& out);
   // float loop unstable
   static bool doesFollowFlow();
   static bool setFollowFlow(bool doesFollow=true);
   static void clearFollowFlow();
   // end of float loop unstable

   static void setSupportAtomic();
   static void setSupportUnstableInLoop();
   static void setSupportVerbose();
   static void setSupportThreshold();
   static void setSupportFirstFollowFloat();
   static void initializeGlobals(const char* fileSuffix);
   static void finalizeGlobals();
   class Initialization {
     public:
      Initialization() {}
      void setSupportAtomic() { ExecutionPath::setSupportAtomic(); }
      void setSupportUnstableInLoop() { ExecutionPath::setSupportUnstableInLoop(); }
      void setSupportVerbose() { ExecutionPath::setSupportVerbose(); }
      void setSupportThreshold() { ExecutionPath::setSupportThreshold(); }
      void setSupportFirstFollowFloat() { ExecutionPath::setSupportFirstFollowFloat(); }
      void setResultFile(const char* fileSuffix) { initializeGlobals(fileSuffix); }
      ~Initialization() { finalizeGlobals(); }
   };

   class anticipated_termination { public: anticipated_termination() {} };
   class read_error {
     public:
      const char* message;
      read_error(const char* msg) : message(msg) {}
      const char* getMessage() const { return message; }
   };
   class precondition_error {
     public:
      const char* message;
      precondition_error(const char* msg) : message(msg) {}
      const char* getMessage() const { return message; }
      void print(std::ostream& out) { out << message << std::endl; }
   };
};

typedef ExecutionPath BaseFloatExact; // for splitBranches

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
class TFloatExact;

class MergeBranches {
  private:
   typedef void* /* size_t */ AlignType;
   static const int UMergeBranchesSizeInBytes = NumericalDomains::DDoubleExactInterface::MergeBranchesSize;
   static const int UMergeBranchesSize = (UMergeBranchesSizeInBytes + sizeof(AlignType)-1)/(sizeof(AlignType) - 1);
   AlignType content[UMergeBranchesSize];

  public:
   MergeBranches(const char* file, int line);

   template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
   MergeBranches& operator<<(TFloatExact<USizeMantissa, USizeExponent, TypeImplementation>& value);
   bool operator<<(end);
};

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
   TSaveMemory<T1, TypeSaveMemory>& operator<<(end) { return *this; }
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
   SaveMemory& operator<<(end) { return *this; }
   SaveMemory& setCurrent(bool result) { fResult = result; return *this; }
   bool getResult() const { return fResult; }
   bool operator>>(end)
      {  bool result = fResult;
         if (fResult)
            fResult = false;
         return result;
      }
};

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
class TFloatExact {
  private:
   typedef TFloatExact<USizeMantissa, USizeExponent, TypeImplementation> thisType;

   typedef void* /* size_t */ AlignType;
   static const int UFloatExactSizeInBytes = NumericalDomains::DDoubleExactInterface
      ::TFloatExactSizeTraits<USizeMantissa, USizeExponent, TypeImplementation>::USize;
   static const int UFloatExactSize = (UFloatExactSizeInBytes + sizeof(AlignType)-1)/(sizeof(AlignType) - 1);
   AlignType content[UFloatExactSize];

   template <int USizeMantissaArgument, int USizeExponentArgument, typename TypeImplementationArgument>
   friend class TFloatExact;
   friend class MergeBranches;

  public:
   const char* queryDebugValue() const;
   typedef ExecutionPath::Initialization Initialization;
   typedef ExecutionPath::anticipated_termination anticipated_termination;
   typedef DDoubleExactInterface::end end;
   static void flushOut() { ExecutionPath::flushOut(); }

  public:
   class ErrorParameter {};
   TFloatExact();
   TFloatExact(float value);
   TFloatExact(float value, float error, ErrorParameter);
   TFloatExact(double value);
   TFloatExact(double value, double error, ErrorParameter);
   TFloatExact(long double value);
   TFloatExact(long double value, long double error, ErrorParameter);
   TFloatExact(int value);
   TFloatExact(long int value);
   // TFloatExact(unsigned value);
   // TFloatExact(unsigned long value);
   TFloatExact(const thisType& source);
   TFloatExact& operator=(const thisType& source);
   ~TFloatExact();

   template <int USizeMantissaArgument, int USizeExponentArgument, typename TypeImplementationArgument>
   TFloatExact(const TFloatExact<USizeMantissaArgument, USizeExponentArgument, TypeImplementationArgument>& source);
   template <int USizeMantissaArgument, int USizeExponentArgument, typename TypeImplementationArgument>
   thisType& operator=(const TFloatExact<USizeMantissaArgument, USizeExponentArgument, TypeImplementationArgument>& source);

   void writeImplementation(std::ostream& out) const;
   void readImplementation(std::istream& in);

   bool operator<(const thisType& source) const;
   bool operator<(TypeImplementation source) const;
   bool operator<=(const thisType& source) const;
   bool operator<=(TypeImplementation source) const;
   bool operator==(const thisType& source) const;
   bool operator==(TypeImplementation source) const;
   bool operator!=(const thisType& source) const;
   bool operator!=(TypeImplementation source) const;
   bool operator>=(const thisType& source) const;
   bool operator>=(TypeImplementation source) const;
   bool operator>(const thisType& source) const;
   bool operator>(TypeImplementation source) const;

   void oppositeAssign();
   thisType& operator+=(const thisType& source);
   thisType& operator-=(const thisType& source);
   thisType& operator*=(const thisType& source);
   thisType& operator/=(const thisType& source);

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

   explicit operator TypeImplementation() const;
   explicit operator int() const;
   explicit operator short int() const;
   explicit operator unsigned() const;
   explicit operator short unsigned() const;
   explicit operator long int() const;
   explicit operator unsigned long() const;

   void sqrtAssign();
   void sinAssign();
   void cosAssign();
   void asinAssign();
   void acosAssign();
   void tanAssign();
   void atanAssign();
   void expAssign();
   void logAssign();
   void log10Assign();
   void powAssign(const thisType& value);

   void absAssign();
   void minAssign(const thisType& source);
   void maxAssign(const thisType& source);
   void medianAssign(const thisType& fst, const thisType& snd);

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

   void persist(const char* prefix) const;
};

} // end of namespace DDoubleExactInterface

typedef DDoubleExactInterface::TFloatExact<23, 8, float> FloatExact;
typedef DDoubleExactInterface::TFloatExact<52, 11, double> DoubleExact;
typedef DDoubleExactInterface::TFloatExact<80, 15, long double> LongDoubleExact;

} // end of namespace NumericalDomains

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
inline NumericalDomains::DDoubleExactInterface::TFloatExact<USizeMantissa, USizeExponent, TypeImplementation>
sqrt(const NumericalDomains::DDoubleExactInterface::TFloatExact<USizeMantissa, USizeExponent, TypeImplementation>& source)
   {  auto result(std::move(source)); result.sqrtAssign(); return result; }

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
inline NumericalDomains::DDoubleExactInterface::TFloatExact<USizeMantissa, USizeExponent, TypeImplementation>
sin(const NumericalDomains::DDoubleExactInterface::TFloatExact<USizeMantissa, USizeExponent, TypeImplementation>& source)
   {  auto result(std::move(source)); result.sinAssign(); return result; }

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
inline NumericalDomains::DDoubleExactInterface::TFloatExact<USizeMantissa, USizeExponent, TypeImplementation>
cos(const NumericalDomains::DDoubleExactInterface::TFloatExact<USizeMantissa, USizeExponent, TypeImplementation>& source)
   {  auto result(std::move(source)); result.cosAssign(); return result; }

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
inline NumericalDomains::DDoubleExactInterface::TFloatExact<USizeMantissa, USizeExponent, TypeImplementation>
asin(const NumericalDomains::DDoubleExactInterface::TFloatExact<USizeMantissa, USizeExponent, TypeImplementation>& source)
   {  auto result(std::move(source)); result.asinAssign(); return result; }

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
inline NumericalDomains::DDoubleExactInterface::TFloatExact<USizeMantissa, USizeExponent, TypeImplementation>
acos(const NumericalDomains::DDoubleExactInterface::TFloatExact<USizeMantissa, USizeExponent, TypeImplementation>& source)
   {  auto result(std::move(source)); result.acosAssign(); return result; }

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
inline NumericalDomains::DDoubleExactInterface::TFloatExact<USizeMantissa, USizeExponent, TypeImplementation>
tan(const NumericalDomains::DDoubleExactInterface::TFloatExact<USizeMantissa, USizeExponent, TypeImplementation>& source)
   {  auto result(std::move(source)); result.tanAssign(); return result; }

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
inline NumericalDomains::DDoubleExactInterface::TFloatExact<USizeMantissa, USizeExponent, TypeImplementation>
atan(const NumericalDomains::DDoubleExactInterface::TFloatExact<USizeMantissa, USizeExponent, TypeImplementation>& source)
   {  auto result(std::move(source)); result.atanAssign(); return result; }

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
inline NumericalDomains::DDoubleExactInterface::TFloatExact<USizeMantissa, USizeExponent, TypeImplementation>
exp(const NumericalDomains::DDoubleExactInterface::TFloatExact<USizeMantissa, USizeExponent, TypeImplementation>& source)
   {  auto result(std::move(source)); result.expAssign(); return result; }

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
inline NumericalDomains::DDoubleExactInterface::TFloatExact<USizeMantissa, USizeExponent, TypeImplementation>
log(const NumericalDomains::DDoubleExactInterface::TFloatExact<USizeMantissa, USizeExponent, TypeImplementation>& source)
   {  auto result(std::move(source)); result.logAssign(); return result; }

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
inline NumericalDomains::DDoubleExactInterface::TFloatExact<USizeMantissa, USizeExponent, TypeImplementation>
log10(const NumericalDomains::DDoubleExactInterface::TFloatExact<USizeMantissa, USizeExponent, TypeImplementation>& source)
   {  auto result(std::move(source)); result.log10Assign(); return result; }

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
inline NumericalDomains::DDoubleExactInterface::TFloatExact<USizeMantissa, USizeExponent, TypeImplementation>
pow(const NumericalDomains::DDoubleExactInterface::TFloatExact<USizeMantissa, USizeExponent, TypeImplementation>& source,
      const NumericalDomains::DDoubleExactInterface::TFloatExact<USizeMantissa, USizeExponent, TypeImplementation>& value)
   {  auto result(std::move(source)); result.powAssign(value); return result; }

template <typename TypeFst, int USizeMantissa, int USizeExponent, typename TypeImplementation>
inline NumericalDomains::DDoubleExactInterface::TFloatExact<USizeMantissa, USizeExponent, TypeImplementation>
pow(TypeFst source, const NumericalDomains::DDoubleExactInterface::TFloatExact<USizeMantissa, USizeExponent, TypeImplementation>& value)
   {  NumericalDomains::DDoubleExactInterface::TFloatExact<USizeMantissa, USizeExponent, TypeImplementation> result(source);
      result.powAssign(value);
      return result;
   }

template <typename TypeSnd, int USizeMantissa, int USizeExponent, typename TypeImplementation>
inline NumericalDomains::DDoubleExactInterface::TFloatExact<USizeMantissa, USizeExponent, TypeImplementation>
pow(const NumericalDomains::DDoubleExactInterface::TFloatExact<USizeMantissa, USizeExponent, TypeImplementation>& source,
      TypeSnd value)
   {  auto result(std::move(source));
      result.powAssign(NumericalDomains::DDoubleExactInterface::TFloatExact<USizeMantissa, USizeExponent, TypeImplementation>(value));
      return result;
   }

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
inline NumericalDomains::DDoubleExactInterface::TFloatExact<USizeMantissa, USizeExponent, TypeImplementation>
powf(const NumericalDomains::DDoubleExactInterface::TFloatExact<USizeMantissa, USizeExponent, TypeImplementation>& source,
      const NumericalDomains::DDoubleExactInterface::TFloatExact<USizeMantissa, USizeExponent, TypeImplementation>& value)
   {  auto result(std::move(source)); result.powAssign(value); return result; }

template <typename TypeFst, int USizeMantissa, int USizeExponent, typename TypeImplementation>
inline NumericalDomains::DDoubleExactInterface::TFloatExact<USizeMantissa, USizeExponent, TypeImplementation>
powf(TypeFst source, const NumericalDomains::DDoubleExactInterface::TFloatExact<USizeMantissa, USizeExponent, TypeImplementation>& value)
   {  NumericalDomains::DDoubleExactInterface::TFloatExact<USizeMantissa, USizeExponent, TypeImplementation> result(source);
      result.powAssign(value);
      return result;
   }

template <typename TypeSnd, int USizeMantissa, int USizeExponent, typename TypeImplementation>
inline NumericalDomains::DDoubleExactInterface::TFloatExact<USizeMantissa, USizeExponent, TypeImplementation>
powf(const NumericalDomains::DDoubleExactInterface::TFloatExact<USizeMantissa, USizeExponent, TypeImplementation>& source,
      TypeSnd value)
   {  auto result(std::move(source));
      result.powAssign(NumericalDomains::DDoubleExactInterface::TFloatExact<USizeMantissa, USizeExponent, TypeImplementation>(value));
      return result;
   }

template <typename TypeFst, int USizeMantissa, int USizeExponent, typename TypeImplementation>
inline bool
operator<(TypeFst fst, const NumericalDomains::DDoubleExactInterface::TFloatExact<USizeMantissa, USizeExponent, TypeImplementation>& snd)
   {  return NumericalDomains::DDoubleExactInterface::TFloatExact<USizeMantissa, USizeExponent, TypeImplementation>(fst).operator<(snd); }

template <typename TypeFst, int USizeMantissa, int USizeExponent, typename TypeImplementation>
inline bool
operator<=(TypeFst fst, const NumericalDomains::DDoubleExactInterface::TFloatExact<USizeMantissa, USizeExponent, TypeImplementation>& snd)
   {  return NumericalDomains::DDoubleExactInterface::TFloatExact<USizeMantissa, USizeExponent, TypeImplementation>(fst).operator<=(snd); }

template <typename TypeFst, int USizeMantissa, int USizeExponent, typename TypeImplementation>
inline bool
operator==(TypeFst fst, const NumericalDomains::DDoubleExactInterface::TFloatExact<USizeMantissa, USizeExponent, TypeImplementation>& snd)
   {  return NumericalDomains::DDoubleExactInterface::TFloatExact<USizeMantissa, USizeExponent, TypeImplementation>(fst).operator==(snd); }

template <typename TypeFst, int USizeMantissa, int USizeExponent, typename TypeImplementation>
inline bool
operator!=(TypeFst fst, const NumericalDomains::DDoubleExactInterface::TFloatExact<USizeMantissa, USizeExponent, TypeImplementation>& snd)
   {  return NumericalDomains::DDoubleExactInterface::TFloatExact<USizeMantissa, USizeExponent, TypeImplementation>(fst).operator!=(snd); }

template <typename TypeFst, int USizeMantissa, int USizeExponent, typename TypeImplementation>
inline bool
operator>=(TypeFst fst, const NumericalDomains::DDoubleExactInterface::TFloatExact<USizeMantissa, USizeExponent, TypeImplementation>& snd)
   {  return NumericalDomains::DDoubleExactInterface::TFloatExact<USizeMantissa, USizeExponent, TypeImplementation>(fst).operator>=(snd); }

template <typename TypeFst, int USizeMantissa, int USizeExponent, typename TypeImplementation>
inline bool
operator>(TypeFst fst, const NumericalDomains::DDoubleExactInterface::TFloatExact<USizeMantissa, USizeExponent, TypeImplementation>& snd)
   {  return NumericalDomains::DDoubleExactInterface::TFloatExact<USizeMantissa, USizeExponent, TypeImplementation>(fst).operator>(snd); }

template <typename TypeFst, int USizeMantissa, int USizeExponent, typename TypeImplementation>
inline NumericalDomains::DDoubleExactInterface::TFloatExact<USizeMantissa, USizeExponent, TypeImplementation>
operator+(TypeFst fst, const NumericalDomains::DDoubleExactInterface::TFloatExact<USizeMantissa, USizeExponent, TypeImplementation>& snd)
   {  return NumericalDomains::DDoubleExactInterface::TFloatExact<USizeMantissa, USizeExponent, TypeImplementation>(fst).operator+(snd); }

template <typename TypeFst, int USizeMantissa, int USizeExponent, typename TypeImplementation>
inline NumericalDomains::DDoubleExactInterface::TFloatExact<USizeMantissa, USizeExponent, TypeImplementation>
operator-(TypeFst fst, const NumericalDomains::DDoubleExactInterface::TFloatExact<USizeMantissa, USizeExponent, TypeImplementation>& snd)
   {  return NumericalDomains::DDoubleExactInterface::TFloatExact<USizeMantissa, USizeExponent, TypeImplementation>(fst).operator-(snd); }

template <typename TypeFst, int USizeMantissa, int USizeExponent, typename TypeImplementation>
inline NumericalDomains::DDoubleExactInterface::TFloatExact<USizeMantissa, USizeExponent, TypeImplementation>
operator*(TypeFst fst, const NumericalDomains::DDoubleExactInterface::TFloatExact<USizeMantissa, USizeExponent, TypeImplementation>& snd)
   {  return NumericalDomains::DDoubleExactInterface::TFloatExact<USizeMantissa, USizeExponent, TypeImplementation>(fst).operator*(snd); }

template <typename TypeFst, int USizeMantissa, int USizeExponent, typename TypeImplementation>
inline NumericalDomains::DDoubleExactInterface::TFloatExact<USizeMantissa, USizeExponent, TypeImplementation>
operator/(TypeFst fst, const NumericalDomains::DDoubleExactInterface::TFloatExact<USizeMantissa, USizeExponent, TypeImplementation>& snd)
   {  return NumericalDomains::DDoubleExactInterface::TFloatExact<USizeMantissa, USizeExponent, TypeImplementation>(fst).operator/(snd); }

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
inline NumericalDomains::DDoubleExactInterface::TFloatExact<USizeMantissa, USizeExponent, TypeImplementation>
log2(const NumericalDomains::DDoubleExactInterface::TFloatExact<USizeMantissa, USizeExponent, TypeImplementation>& source)
   {  typedef NumericalDomains::DDoubleExactInterface::TFloatExact<USizeMantissa, USizeExponent, TypeImplementation> thisType;
      thisType result(std::move(source));
      result.logAssign();
      result.divAssign(thisType(::log(2.0)));
      return result;
   }

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
inline NumericalDomains::DDoubleExactInterface::TFloatExact<USizeMantissa, USizeExponent, TypeImplementation>
exp2(const NumericalDomains::DDoubleExactInterface::TFloatExact<USizeMantissa, USizeExponent, TypeImplementation>& source)
   {  NumericalDomains::DDoubleExactInterface::TFloatExact<USizeMantissa, USizeExponent, TypeImplementation>
            result = 2.0;
      result.powAssign(source);
      return result;
   }

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
inline NumericalDomains::DDoubleExactInterface::TFloatExact<USizeMantissa, USizeExponent, TypeImplementation>
floor(const NumericalDomains::DDoubleExactInterface::TFloatExact<USizeMantissa, USizeExponent, TypeImplementation>& fst)
   {  typedef NumericalDomains::DDoubleExactInterface::TFloatExact<USizeMantissa, USizeExponent, TypeImplementation> thisType;
      return thisType(fst.operator int());
   }

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
inline NumericalDomains::DDoubleExactInterface::TFloatExact<USizeMantissa, USizeExponent, TypeImplementation>
floor(NumericalDomains::DDoubleExactInterface::TFloatExact<USizeMantissa, USizeExponent, TypeImplementation>&& fst)
   {  typedef NumericalDomains::DDoubleExactInterface::TFloatExact<USizeMantissa, USizeExponent, TypeImplementation> thisType;
      return thisType(fst.operator int());
   }

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
inline int
rintf(const NumericalDomains::DDoubleExactInterface::TFloatExact<USizeMantissa, USizeExponent, TypeImplementation>& fst)
   {  return fst.operator int(); }

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
inline int
rintf(NumericalDomains::DDoubleExactInterface::TFloatExact<USizeMantissa, USizeExponent, TypeImplementation>&& fst)
   {  return fst.operator int(); }

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
inline int
rint(const NumericalDomains::DDoubleExactInterface::TFloatExact<USizeMantissa, USizeExponent, TypeImplementation>& fst)
   {  return fst.operator int(); }

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
inline int
rint(NumericalDomains::DDoubleExactInterface::TFloatExact<USizeMantissa, USizeExponent, TypeImplementation>&& fst)
   {  return fst.operator int(); }

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
inline NumericalDomains::DDoubleExactInterface::TFloatExact<USizeMantissa, USizeExponent, TypeImplementation>
fabs(const NumericalDomains::DDoubleExactInterface::TFloatExact<USizeMantissa, USizeExponent, TypeImplementation>& source)
   {  auto result(std::move(source)); result.absAssign(); return result; }

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
inline NumericalDomains::DDoubleExactInterface::TFloatExact<USizeMantissa, USizeExponent, TypeImplementation>
fabs(NumericalDomains::DDoubleExactInterface::TFloatExact<USizeMantissa, USizeExponent, TypeImplementation>&& source)
   {  auto result(source); result.absAssign(); return result; }

#endif // FloatInstrumentation_FloatExactH

