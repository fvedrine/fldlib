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
// File      : FloatIntervalInterface.h
// Description :
//   Definition of a class of floating point intervals
//

#ifndef FloatInstrumentation_FloatIntervalH
#define FloatInstrumentation_FloatIntervalH

// for log
#include <math.h>
#include "FloatInstrumentation/IntervalTypesSize.h"

#include <iostream>

namespace NumericalDomains {
   
namespace DDoubleInterval {

class PathExplorer;
class DiagnosisReadStream;

}

namespace DDoubleIntervalInterface {

class end {};

class ExecutionPath;
class PathExplorer {
  private:
   typedef void* /* size_t */ AlignType;
   static const int UPathExplorerSizeInBytes = NumericalDomains::DDoubleIntervalInterface::PathExplorerSize;
   static const int UPathExplorerSize = (UPathExplorerSizeInBytes + sizeof(AlignType)-1)/(sizeof(AlignType) - 1);
   AlignType content[UPathExplorerSize];
   friend class ExecutionPath;

  public:
   PathExplorer();
   PathExplorer(const PathExplorer& source);
   PathExplorer(PathExplorer&& source);
   ~PathExplorer();
   PathExplorer& operator=(const PathExplorer& source);
   PathExplorer& operator=(PathExplorer&& source);

   bool isFinished();
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
   SaveMemory& operator<<(end) { return *this; }
   SaveMemory& setCurrentResult(bool result) { fResult = result; return *this; }
   bool getResult() const { return fResult; }
   bool operator>>(end)
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
   TMergeMemory<T1, TypeMergeMemory>& operator>>(end) { return *this; }
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
   MergeMemory& operator>>(end) { return *this; }
   bool isFirst() const { return fFirst; }
   bool operator<<(end)
      {  fFirst = false;
         return true;
      }
};

class ExecutionPath {
  public:
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
   static void flushOut();
   static void setSourceLine(const char* file, int line);
   static void writeCurrentPath(std::ostream& out);
   static DDoubleInterval::PathExplorer* getCurrentPathExplorer();
   static void setCurrentPathExplorer(PathExplorer* pathExplorer);
   static void setCurrentPathExplorer(DDoubleInterval::PathExplorer* pathExplorer);

   static DDoubleInterval::DiagnosisReadStream* inputTraceFile();
   static bool doesFollowFlow();
   static void clearFollowFlow();
   static void setFollowFlow(bool doesFollowFlow, DDoubleInterval::DiagnosisReadStream* inputTraceFile);
   static void setFollowFlow();
};

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
class TFloatInterval {
  private:
   typedef TFloatInterval<USizeMantissa, USizeExponent, TypeImplementation> thisType;

   typedef void* /* size_t */ AlignType;
   static const int UFloatIntervalSizeInBytes = NumericalDomains::DDoubleIntervalInterface
      ::TFloatIntervalSizeTraits<USizeMantissa, USizeExponent, TypeImplementation>::USize;
   static const int UFloatIntervalSize = (UFloatIntervalSizeInBytes + sizeof(AlignType)-1)/(sizeof(AlignType) - 1);
   AlignType content[UFloatIntervalSize];

   template <int USizeMantissaArgument, int USizeExponentArgument, typename TypeImplementationArgument>
   friend class TFloatInterval;
   friend class MergeBranches;

  public:
   const char* queryDebugValue() const;
   const char* queryLightDebugValue() const;
   typedef ExecutionPath::Initialization Initialization;
   typedef DDoubleIntervalInterface::end end;
   static void flushOut() { ExecutionPath::flushOut(); }

  public:
   TFloatInterval();
   TFloatInterval(float value);
   TFloatInterval(double value);
   TFloatInterval(long double value);
   TFloatInterval(TypeImplementation min, TypeImplementation max);
   TFloatInterval(TypeImplementation min, TypeImplementation max,
         TypeImplementation errmin, TypeImplementation errmax);
   TFloatInterval(int value);
   TFloatInterval(long int value);
   // TFloatInterval(unsigned value);
   // TFloatInterval(unsigned long value);
   TFloatInterval(const thisType& source);
   TFloatInterval(thisType&& source);
   ~TFloatInterval();

   TFloatInterval& operator=(const thisType& source);
   TFloatInterval& operator=(thisType&& source);
   void mergeWith(const thisType& source);
   void mergeWith(thisType&& source);

   template <int USizeMantissaArgument, int USizeExponentArgument, typename TypeImplementationArgument>
   TFloatInterval(const TFloatInterval<USizeMantissaArgument, USizeExponentArgument, TypeImplementationArgument>& source);
   template <int USizeMantissaArgument, int USizeExponentArgument, typename TypeImplementationArgument>
   thisType& operator=(const TFloatInterval<USizeMantissaArgument, USizeExponentArgument, TypeImplementationArgument>& source);

   void readImplementation(std::istream& in);
   void writeImplementation(std::ostream& out) const;

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

   thisType& operator+=(const thisType& source);
   thisType& operator-=(const thisType& source);
   thisType& operator*=(const thisType& source);
   thisType& operator/=(const thisType& source);

   void oppositeAssign();
   thisType operator+() const { return *this; }
   thisType operator-() const
      {  thisType result(*this); result.oppositeAssign(); return result; }
   thisType operator+(const thisType& source) const
      {  thisType result(*this); result.operator+=(source); return result; }
   thisType operator+(TypeImplementation source) const
      {  thisType result(*this); result.operator+=(thisType(source)); return result; }
   thisType operator-(const thisType& source) const
      {  thisType result(*this); result.operator-=(source); return result; }
   thisType operator-(TypeImplementation source) const
      {  thisType result(*this); result.operator-=(thisType(source)); return result; }
   thisType operator*(const thisType& source) const
      {  thisType result(*this); result.operator*=(source); return result; }
   thisType operator*(TypeImplementation source) const
      {  thisType result(*this); result.operator*=(thisType(source)); return result; }
   thisType operator/(const thisType& source) const
      {  thisType result(*this); result.operator/=(source); return result; }
   thisType operator/(TypeImplementation source) const
      {  thisType result(*this); result.operator/=(thisType(source)); return result; }

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

   thisType abs() const
      {  if (*this < 0)
            return -(*this);
         return *this;
      }
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

   void persist(const char* prefix) const;
};

} // end of namespace DDoubleIntervalInterface

typedef DDoubleIntervalInterface::TFloatInterval<23, 8, float> FloatInterval;
typedef DDoubleIntervalInterface::TFloatInterval<52, 11, double> DoubleInterval;
typedef DDoubleIntervalInterface::TFloatInterval<80, 15, long double> LongDoubleInterval;

} // end of namespace NumericalDomains

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
inline NumericalDomains::DDoubleIntervalInterface::TFloatInterval<USizeMantissa, USizeExponent, TypeImplementation>
sqrt(const NumericalDomains::DDoubleIntervalInterface::TFloatInterval<USizeMantissa, USizeExponent, TypeImplementation>& source)
   {  auto result(std::move(source)); result.sqrtAssign(); return result; }

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
inline NumericalDomains::DDoubleIntervalInterface::TFloatInterval<USizeMantissa, USizeExponent, TypeImplementation>
sin(const NumericalDomains::DDoubleIntervalInterface::TFloatInterval<USizeMantissa, USizeExponent, TypeImplementation>& source)
   {  auto result(std::move(source)); result.sinAssign(); return result; }

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
inline NumericalDomains::DDoubleIntervalInterface::TFloatInterval<USizeMantissa, USizeExponent, TypeImplementation>
cos(const NumericalDomains::DDoubleIntervalInterface::TFloatInterval<USizeMantissa, USizeExponent, TypeImplementation>& source)
   {  auto result(std::move(source)); result.cosAssign(); return result; }

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
inline NumericalDomains::DDoubleIntervalInterface::TFloatInterval<USizeMantissa, USizeExponent, TypeImplementation>
asin(const NumericalDomains::DDoubleIntervalInterface::TFloatInterval<USizeMantissa, USizeExponent, TypeImplementation>& source)
   {  auto result(std::move(source)); result.asinAssign(); return result; }

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
inline NumericalDomains::DDoubleIntervalInterface::TFloatInterval<USizeMantissa, USizeExponent, TypeImplementation>
acos(const NumericalDomains::DDoubleIntervalInterface::TFloatInterval<USizeMantissa, USizeExponent, TypeImplementation>& source)
   {  auto result(std::move(source)); result.acosAssign(); return result; }

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
inline NumericalDomains::DDoubleIntervalInterface::TFloatInterval<USizeMantissa, USizeExponent, TypeImplementation>
tan(const NumericalDomains::DDoubleIntervalInterface::TFloatInterval<USizeMantissa, USizeExponent, TypeImplementation>& source)
   {  auto result(std::move(source)); result.tanAssign(); return result; }

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
inline NumericalDomains::DDoubleIntervalInterface::TFloatInterval<USizeMantissa, USizeExponent, TypeImplementation>
atan(const NumericalDomains::DDoubleIntervalInterface::TFloatInterval<USizeMantissa, USizeExponent, TypeImplementation>& source)
   {  auto result(std::move(source)); result.atanAssign(); return result; }

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
inline NumericalDomains::DDoubleIntervalInterface::TFloatInterval<USizeMantissa, USizeExponent, TypeImplementation>
exp(const NumericalDomains::DDoubleIntervalInterface::TFloatInterval<USizeMantissa, USizeExponent, TypeImplementation>& source)
   {  auto result(std::move(source)); result.expAssign(); return result; }

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
inline NumericalDomains::DDoubleIntervalInterface::TFloatInterval<USizeMantissa, USizeExponent, TypeImplementation>
log(const NumericalDomains::DDoubleIntervalInterface::TFloatInterval<USizeMantissa, USizeExponent, TypeImplementation>& source)
   {  auto result(std::move(source)); result.logAssign(); return result; }

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
inline NumericalDomains::DDoubleIntervalInterface::TFloatInterval<USizeMantissa, USizeExponent, TypeImplementation>
log10(const NumericalDomains::DDoubleIntervalInterface::TFloatInterval<USizeMantissa, USizeExponent, TypeImplementation>& source)
   {  auto result(std::move(source)); result.log10Assign(); return result; }

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
inline NumericalDomains::DDoubleIntervalInterface::TFloatInterval<USizeMantissa, USizeExponent, TypeImplementation>
pow(const NumericalDomains::DDoubleIntervalInterface::TFloatInterval<USizeMantissa, USizeExponent, TypeImplementation>& source,
      const NumericalDomains::DDoubleIntervalInterface::TFloatInterval<USizeMantissa, USizeExponent, TypeImplementation>& value)
   {  auto result(std::move(source)); result.powAssign(value); return result; }

template <class TypeFst, int USizeMantissa, int USizeExponent, typename TypeImplementation>
inline NumericalDomains::DDoubleIntervalInterface::TFloatInterval<USizeMantissa, USizeExponent, TypeImplementation>
pow(TypeFst source, const NumericalDomains::DDoubleIntervalInterface::TFloatInterval<USizeMantissa, USizeExponent, TypeImplementation>& value)
   {  NumericalDomains::DDoubleIntervalInterface::TFloatInterval<USizeMantissa, USizeExponent, TypeImplementation> result(source);
      result.powAssign(value);
      return result;
   }

template <class TypeSnd, int USizeMantissa, int USizeExponent, typename TypeImplementation>
inline NumericalDomains::DDoubleIntervalInterface::TFloatInterval<USizeMantissa, USizeExponent, TypeImplementation>
pow(const NumericalDomains::DDoubleIntervalInterface::TFloatInterval<USizeMantissa, USizeExponent, TypeImplementation>& source,
      TypeSnd value)
   {  auto result(std::move(source));
      result.powAssign(NumericalDomains::DDoubleIntervalInterface::TFloatInterval<USizeMantissa, USizeExponent, TypeImplementation>(value));
      return result;
   }

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
inline NumericalDomains::DDoubleIntervalInterface::TFloatInterval<USizeMantissa, USizeExponent, TypeImplementation>
powf(const NumericalDomains::DDoubleIntervalInterface::TFloatInterval<USizeMantissa, USizeExponent, TypeImplementation>& source,
      const NumericalDomains::DDoubleIntervalInterface::TFloatInterval<USizeMantissa, USizeExponent, TypeImplementation>& value)
   {  auto result(std::move(source)); result.powAssign(value); return result; }

template <class TypeFst, int USizeMantissa, int USizeExponent, typename TypeImplementation>
inline NumericalDomains::DDoubleIntervalInterface::TFloatInterval<USizeMantissa, USizeExponent, TypeImplementation>
powf(TypeFst source, const NumericalDomains::DDoubleIntervalInterface::TFloatInterval<USizeMantissa, USizeExponent, TypeImplementation>& value)
   {  NumericalDomains::DDoubleIntervalInterface::TFloatInterval<USizeMantissa, USizeExponent, TypeImplementation> result(source);
      result.powAssign(value);
      return result;
   }

template <class TypeSnd, int USizeMantissa, int USizeExponent, typename TypeImplementation>
inline NumericalDomains::DDoubleIntervalInterface::TFloatInterval<USizeMantissa, USizeExponent, TypeImplementation>
powf(const NumericalDomains::DDoubleIntervalInterface::TFloatInterval<USizeMantissa, USizeExponent, TypeImplementation>& source,
      TypeSnd value)
   {  auto result(std::move(source));
      result.powAssign(NumericalDomains::DDoubleIntervalInterface::TFloatInterval<USizeMantissa, USizeExponent, TypeImplementation>(value));
      return result;
   }

template <typename TypeFst, int USizeMantissa, int USizeExponent, typename TypeImplementation>
inline bool
operator<(TypeFst fst, const NumericalDomains::DDoubleIntervalInterface::TFloatInterval<USizeMantissa, USizeExponent, TypeImplementation>& snd)
   {  return NumericalDomains::DDoubleIntervalInterface::TFloatInterval<USizeMantissa, USizeExponent, TypeImplementation>(fst).operator<(snd); }

template <typename TypeFst, int USizeMantissa, int USizeExponent, typename TypeImplementation>
inline bool
operator<=(TypeFst fst, const NumericalDomains::DDoubleIntervalInterface::TFloatInterval<USizeMantissa, USizeExponent, TypeImplementation>& snd)
   {  return NumericalDomains::DDoubleIntervalInterface::TFloatInterval<USizeMantissa, USizeExponent, TypeImplementation>(fst).operator<=(snd); }

template <typename TypeFst, int USizeMantissa, int USizeExponent, typename TypeImplementation>
inline bool
operator==(TypeFst fst, const NumericalDomains::DDoubleIntervalInterface::TFloatInterval<USizeMantissa, USizeExponent, TypeImplementation>& snd)
   {  return NumericalDomains::DDoubleIntervalInterface::TFloatInterval<USizeMantissa, USizeExponent, TypeImplementation>(fst).operator==(snd); }

template <typename TypeFst, int USizeMantissa, int USizeExponent, typename TypeImplementation>
inline bool
operator!=(TypeFst fst, const NumericalDomains::DDoubleIntervalInterface::TFloatInterval<USizeMantissa, USizeExponent, TypeImplementation>& snd)
   {  return NumericalDomains::DDoubleIntervalInterface::TFloatInterval<USizeMantissa, USizeExponent, TypeImplementation>(fst).operator!=(snd); }

template <typename TypeFst, int USizeMantissa, int USizeExponent, typename TypeImplementation>
inline bool
operator>=(TypeFst fst, const NumericalDomains::DDoubleIntervalInterface::TFloatInterval<USizeMantissa, USizeExponent, TypeImplementation>& snd)
   {  return NumericalDomains::DDoubleIntervalInterface::TFloatInterval<USizeMantissa, USizeExponent, TypeImplementation>(fst).operator>=(snd); }

template <typename TypeFst, int USizeMantissa, int USizeExponent, typename TypeImplementation>
inline bool
operator>(TypeFst fst, const NumericalDomains::DDoubleIntervalInterface::TFloatInterval<USizeMantissa, USizeExponent, TypeImplementation>& snd)
   {  return NumericalDomains::DDoubleIntervalInterface::TFloatInterval<USizeMantissa, USizeExponent, TypeImplementation>(fst).operator>(snd); }

template <typename TypeFst, int USizeMantissa, int USizeExponent, typename TypeImplementation>
inline NumericalDomains::DDoubleIntervalInterface::TFloatInterval<USizeMantissa, USizeExponent, TypeImplementation>
operator+(TypeFst fst, const NumericalDomains::DDoubleIntervalInterface::TFloatInterval<USizeMantissa, USizeExponent, TypeImplementation>& snd)
   {  return NumericalDomains::DDoubleIntervalInterface::TFloatInterval<USizeMantissa, USizeExponent, TypeImplementation>(fst).operator+(snd); }

template <typename TypeFst, int USizeMantissa, int USizeExponent, typename TypeImplementation>
inline NumericalDomains::DDoubleIntervalInterface::TFloatInterval<USizeMantissa, USizeExponent, TypeImplementation>
operator-(TypeFst fst, const NumericalDomains::DDoubleIntervalInterface::TFloatInterval<USizeMantissa, USizeExponent, TypeImplementation>& snd)
   {  return NumericalDomains::DDoubleIntervalInterface::TFloatInterval<USizeMantissa, USizeExponent, TypeImplementation>(fst).operator-(snd); }

template <typename TypeFst, int USizeMantissa, int USizeExponent, typename TypeImplementation>
inline NumericalDomains::DDoubleIntervalInterface::TFloatInterval<USizeMantissa, USizeExponent, TypeImplementation>
operator*(TypeFst fst, const NumericalDomains::DDoubleIntervalInterface::TFloatInterval<USizeMantissa, USizeExponent, TypeImplementation>& snd)
   {  return NumericalDomains::DDoubleIntervalInterface::TFloatInterval<USizeMantissa, USizeExponent, TypeImplementation>(fst).operator*(snd); }

template <typename TypeFst, int USizeMantissa, int USizeExponent, typename TypeImplementation>
inline NumericalDomains::DDoubleIntervalInterface::TFloatInterval<USizeMantissa, USizeExponent, TypeImplementation>
operator/(TypeFst fst, const NumericalDomains::DDoubleIntervalInterface::TFloatInterval<USizeMantissa, USizeExponent, TypeImplementation>& snd)
   {  return NumericalDomains::DDoubleIntervalInterface::TFloatInterval<USizeMantissa, USizeExponent, TypeImplementation>(fst).operator/(snd); }

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
inline NumericalDomains::DDoubleIntervalInterface::TFloatInterval<USizeMantissa, USizeExponent, TypeImplementation>
log2(const NumericalDomains::DDoubleIntervalInterface::TFloatInterval<USizeMantissa, USizeExponent, TypeImplementation>& source)
   {  typedef NumericalDomains::DDoubleIntervalInterface::TFloatInterval<USizeMantissa, USizeExponent, TypeImplementation> thisType;
      thisType result(std::move(source));
      result.logAssign();
      result.divAssign(thisType(::log(2.0)));
      return result;
   }

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
inline NumericalDomains::DDoubleIntervalInterface::TFloatInterval<USizeMantissa, USizeExponent, TypeImplementation>
exp2(const NumericalDomains::DDoubleIntervalInterface::TFloatInterval<USizeMantissa, USizeExponent, TypeImplementation>& source)
   {  typedef NumericalDomains::DDoubleIntervalInterface::TFloatInterval<USizeMantissa, USizeExponent, TypeImplementation> thisType;
      thisType result = 2.0;
      result.powAssign(source);
      return result;
   }

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
inline NumericalDomains::DDoubleIntervalInterface::TFloatInterval<USizeMantissa, USizeExponent, TypeImplementation>
floor(const NumericalDomains::DDoubleIntervalInterface::TFloatInterval<USizeMantissa, USizeExponent, TypeImplementation>& fst)
{  typedef NumericalDomains::DDoubleIntervalInterface::TFloatInterval<USizeMantissa, USizeExponent, TypeImplementation> thisType;
   return thisType(fst.operator int());
}

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
inline NumericalDomains::DDoubleIntervalInterface::TFloatInterval<USizeMantissa, USizeExponent, TypeImplementation>
floor(NumericalDomains::DDoubleIntervalInterface::TFloatInterval<USizeMantissa, USizeExponent, TypeImplementation>&& fst)
{  typedef NumericalDomains::DDoubleIntervalInterface::TFloatInterval<USizeMantissa, USizeExponent, TypeImplementation> thisType;
   return thisType(fst.operator int());
}

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
inline int
rintf(const NumericalDomains::DDoubleIntervalInterface::TFloatInterval<USizeMantissa, USizeExponent, TypeImplementation>& fst)
{  return fst.operator int(); }

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
inline int
rintf(NumericalDomains::DDoubleIntervalInterface::TFloatInterval<USizeMantissa, USizeExponent, TypeImplementation>&& fst)
{  return fst.operator int(); }

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
inline int
rint(const NumericalDomains::DDoubleIntervalInterface::TFloatInterval<USizeMantissa, USizeExponent, TypeImplementation>& fst)
{  return fst.operator int(); }

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
inline int
rint(NumericalDomains::DDoubleIntervalInterface::TFloatInterval<USizeMantissa, USizeExponent, TypeImplementation>&& fst)
{  return fst.operator int(); }

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
inline NumericalDomains::DDoubleIntervalInterface::TFloatInterval<USizeMantissa, USizeExponent, TypeImplementation>
fabs(const NumericalDomains::DDoubleIntervalInterface::TFloatInterval<USizeMantissa, USizeExponent, TypeImplementation>& fst)
{  typedef NumericalDomains::DDoubleIntervalInterface::TFloatInterval<USizeMantissa, USizeExponent, TypeImplementation> thisType;
   thisType result = fst;
   if (result < 0)
      result.oppositeAssign();
   return result;
}

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
inline NumericalDomains::DDoubleIntervalInterface::TFloatInterval<USizeMantissa, USizeExponent, TypeImplementation>
fabs(NumericalDomains::DDoubleIntervalInterface::TFloatInterval<USizeMantissa, USizeExponent, TypeImplementation>&& fst)
{  typedef NumericalDomains::DDoubleIntervalInterface::TFloatInterval<USizeMantissa, USizeExponent, TypeImplementation> thisType;
   thisType result(std::forward<thisType>(fst));
   if (result < 0)
      result.oppositeAssign();
   return result;
}

#endif // FloatInstrumentation_FloatIntervalH

