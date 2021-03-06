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
#include <vector>
#include <functional>
#include <cassert>

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

   template <class TypeIterator>
   MergeBranches& operator<<(TPacker<TypeIterator> packer)
      {  for (; packer.iter != packer.end; ++packer.iter)
            operator<<(*packer.iter);
         return *this;
      }
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

template <typename TypeIterator, class TypeSaveMemory>
class TPackedSaveMemory {
  public:
   std::vector<typename TypeIterator::value_type> save;
   TypeSaveMemory next;

   TPackedSaveMemory(TypeIterator iter, TypeIterator end, TypeSaveMemory nextArg)
      :  next(nextArg)
      {  int count = end - iter;
         save.reserve(count);
         for (; iter != end; ++iter)
            save.push_back(*iter);
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
   TPackedSaveMemory<TypeIterator, TypeSaveMemory>& operator<<(end) { return *this; }

   TPackedSaveMemory<TypeIterator, TypeSaveMemory>& setCurrentResult(bool result)
      {  next.setCurrentResult(result); return *this; }
   
   TypeSaveMemory& operator>>(MergeBranches::TPacker<TypeIterator>&& packer)
      {  if (!next.getResult()) {
            int count = packer.end - packer.iter;
            assert(count == (int) save.size());
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
   TPackedSaveMemory<TypeIterator, SaveMemory> operator<<(MergeBranches::TPacker<TypeIterator> packer)
      {  return TPackedSaveMemory<TypeIterator, SaveMemory>(packer.iter, packer.end, *this); }
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

template <typename TypeIterator, class TypeMergeMemory>
class TPackedMergeMemory {
  public:
   std::vector<typename TypeIterator::value_type> merge;
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
   TPackedMergeMemory<TypeIterator, TypeMergeMemory>& operator>>(end) { return *this; }

   TypeMergeMemory& operator<<(MergeBranches::TPacker<TypeIterator>&& packer)
      {  int count = packer.end - packer.iter;
         auto iter = packer.iter;
         if (next.isFirst()) {
            assert(merge.size() == 0);
            merge.reserve(count);
            for (; iter != packer.end; ++iter)
               merge.push_back(*iter);
         }
         else {
            for (int index = 0; index < count; ++index) {
               merge[index].mergeWith(*iter);
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
   static void setSupportUnstableInLoop(bool value = true);
   static void setSupportBacktrace();
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
      void setSupportBacktrace() { ExecutionPath::setSupportBacktrace(); }
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
   static bool doesSupportUnstableInLoop();
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
   friend class DDoubleIntervalInterface::MergeBranches;

   enum RoundMode { RMNearest, RMLowest, RMHighest, RMZero };
   int asInt(RoundMode mode) const;

   int sfinite() const;
   int sisfinite() const;
   int sisnan() const;
   int sisinf() const;

  public:
   const char* queryDebugValue() const;
   const char* queryLightDebugValue() const;
   typedef ExecutionPath::Initialization Initialization;
   typedef DDoubleIntervalInterface::end end;
   static void flushOut() { ExecutionPath::flushOut(); }

   typedef DDoubleIntervalInterface::MergeBranches MergeBranches;

  public:
   struct ValueFromString {};
   TFloatInterval();
   TFloatInterval(const char* value, ValueFromString);
   TFloatInterval(float value);
   TFloatInterval(double value);
   TFloatInterval(long double value);
   TFloatInterval(TypeImplementation min, TypeImplementation max);
   TFloatInterval(TypeImplementation min, TypeImplementation max,
         TypeImplementation errmin, TypeImplementation errmax);
   TFloatInterval(short int value);
   TFloatInterval(int value);
   TFloatInterval(long int value);
   TFloatInterval(unsigned short value);
   TFloatInterval(unsigned value);
   TFloatInterval(unsigned long value);
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

   TypeImplementation asImplementation() const;
   void readImplementation(std::istream& in);
   void writeImplementation(std::ostream& out) const;
   friend std::ostream& operator<<(std::ostream& out, const thisType& source)
      {  return out << source.asImplementation(); }
   friend std::istream& operator>>(std::istream& in, thisType& source)
      {  TypeImplementation val;
         in >> val;
         operator=(thisType(val));
         return in;
      }

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

   void continuousFlow(std::function<void (thisType& val)> funAssign)
      {  bool oldSupportUnstableInLoop = ExecutionPath::doesSupportUnstableInLoop();
         ExecutionPath::setSupportUnstableInLoop();
         auto* oldPathExplorer = ExecutionPath::getCurrentPathExplorer();
         bool oldDoesFollow = ExecutionPath::doesFollowFlow();
         ExecutionPath::clearFollowFlow();
         auto* oldInputTraceFile = ExecutionPath::inputTraceFile();
         PathExplorer pathExplorer;
         ExecutionPath::setCurrentPathExplorer(&pathExplorer);
         auto mergeMemory = MergeMemory() >> *this >> end();
         auto saveMemory = SaveMemory() << *this << end();
         bool doesIterate;
         do {
            funAssign(*this);
            ExecutionPath::setFollowFlow();
            doesIterate = mergeMemory << *this << end();
            if (doesIterate)
               doesIterate = !(saveMemory.setCurrentResult(pathExplorer.isFinished()) >> *this >> end());
         } while (doesIterate);
         ExecutionPath::setFollowFlow(oldDoesFollow, oldInputTraceFile);
         ExecutionPath::setCurrentPathExplorer(oldPathExplorer);
         ExecutionPath::setSupportUnstableInLoop(oldSupportUnstableInLoop);
      }
   void continuousFlow(std::function<void (thisType& val, const thisType& arg)> funAssign, const thisType& anarg)
      {  bool oldSupportUnstableInLoop = ExecutionPath::doesSupportUnstableInLoop();
         ExecutionPath::setSupportUnstableInLoop();
         auto* oldPathExplorer = ExecutionPath::getCurrentPathExplorer();
         bool oldDoesFollow = ExecutionPath::doesFollowFlow();
         ExecutionPath::clearFollowFlow();
         auto* oldInputTraceFile = ExecutionPath::inputTraceFile();
         PathExplorer pathExplorer;
         ExecutionPath::setCurrentPathExplorer(&pathExplorer);
         thisType& arg = const_cast<thisType&>(anarg);
         auto mergeMemory = MergeMemory() >> arg >> *this >> end();
         auto saveMemory = SaveMemory() << *this << arg << end();
         bool doesIterate;
         do {
            funAssign(*this, anarg);
            ExecutionPath::setFollowFlow();
            doesIterate = mergeMemory << *this << arg << end();
            if (doesIterate)
               doesIterate = !(saveMemory.setCurrentResult(pathExplorer.isFinished()) >> arg >> *this >> end());
         } while (doesIterate);
         ExecutionPath::setFollowFlow(oldDoesFollow, oldInputTraceFile);
         ExecutionPath::setCurrentPathExplorer(oldPathExplorer);
         ExecutionPath::setSupportUnstableInLoop(oldSupportUnstableInLoop);
      }
   void continuousFlow(std::function<void (thisType& val, const thisType& fstarg, const thisType& sndarg)> funAssign,
         const thisType& afstarg, const thisType& asndarg)
      {  bool oldSupportUnstableInLoop = ExecutionPath::doesSupportUnstableInLoop();
         ExecutionPath::setSupportUnstableInLoop();
         auto* oldPathExplorer = ExecutionPath::getCurrentPathExplorer();
         bool oldDoesFollow = ExecutionPath::doesFollowFlow();
         ExecutionPath::clearFollowFlow();
         auto* oldInputTraceFile = ExecutionPath::inputTraceFile();
         PathExplorer pathExplorer;
         ExecutionPath::setCurrentPathExplorer(&pathExplorer);
         thisType& fstarg = const_cast<thisType&>(afstarg);
         thisType& sndarg = const_cast<thisType&>(asndarg);
         auto mergeMemory = MergeMemory() >> sndarg >> fstarg >> *this >> end();
         auto saveMemory = SaveMemory() << *this << fstarg << sndarg << end();
         bool doesIterate;
         do {
            funAssign(*this, afstarg, asndarg);
            ExecutionPath::setFollowFlow();
            doesIterate = mergeMemory << *this << fstarg << sndarg << end();
            if (doesIterate)
               doesIterate = !(saveMemory.setCurrentResult(pathExplorer.isFinished()) >> sndarg >> fstarg >> *this >> end());
         } while (doesIterate);
         ExecutionPath::setFollowFlow(oldDoesFollow, oldInputTraceFile);
         ExecutionPath::setCurrentPathExplorer(oldPathExplorer);
         ExecutionPath::setSupportUnstableInLoop(oldSupportUnstableInLoop);
      }

   thisType abs() const
      {  auto result(*this);
         result.continuousFlow([](thisType& val){ if (val < 0) val.oppositeAssign(); });
         return result;
      }
   thisType min() const;
   thisType max() const;
   thisType min(const thisType& source) const
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
   thisType max(const thisType& source) const
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

   thisType median(const thisType& fst, const thisType& snd) const
      {  auto result(*this);
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
   thisType median(TypeImplementation afst, const thisType& snd) const
      {  auto result(*this);
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
   thisType median(const thisType& fst, TypeImplementation asnd) const
      {  auto result(*this);
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

   void persist(const char* prefix) const;

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
      {  thisType result(std::move(source));
         result.logAssign();
         result.divAssign(log(thisType(2.0)));
         return result;
      }
   friend thisType exp2(const thisType& source)
      {  thisType result = 2.0;
         result.powAssign(source);
         return result;
      }
   friend thisType log10(const thisType& source)
      {  auto result(std::move(source)); result.log10Assign(); return result; }

   friend thisType pow(const thisType& source, const thisType& value)
      {  auto result(source); result.powAssign(value); return result; }
   template <int USizeMantissaArgument, int USizeExponentArgument, typename TypeImplementationArgument>
   friend thisType pow(const TFloatInterval<USizeMantissaArgument, USizeExponentArgument, TypeImplementationArgument>& source, const thisType& value)
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
   template <int USizeMantissaArgument, int USizeExponentArgument, typename TypeImplementationArgument>
   friend thisType pow(const thisType& source, const TFloatInterval<USizeMantissaArgument, USizeExponentArgument, TypeImplementationArgument>& value)
      {  thisType result(source); result.powAssign(thisType(value)); return result; }
   friend thisType pow(const thisType& source, long double value)
      {  thisType result(source); result.powAssign(thisType(value)); return result; }
   friend thisType pow(const thisType& source, double value)
      {  thisType result(source); result.powAssign(thisType(value)); return result; }
   friend thisType pow(const thisType& source, float value)
      {  thisType result(source); result.powAssign(thisType(value)); return result; }
   friend thisType pow(const thisType& source, unsigned long value)
      {  thisType result(source); result.powAssign(thisType(value)); return result; }
   friend thisType pow(const thisType& source, long value)
      {  thisType result(source); result.powAssign(thisType(value)); return result; }
   friend thisType pow(const thisType& source, unsigned int value)
      {  thisType result(source); result.powAssign(thisType(value)); return result; }
   friend thisType pow(const thisType& source, int value)
      {  thisType result(source); result.powAssign(thisType(value)); return result; }
   friend thisType pow(const thisType& source, unsigned short value)
      {  thisType result(source); result.powAssign(thisType(value)); return result; }
   friend thisType pow(const thisType& source, short value)
      {  thisType result(source); result.powAssign(thisType(value)); return result; }

   friend thisType powf(const thisType& source, const thisType& value)
      {  auto result(source); result.powAssign(value); return result; }
   template <int USizeMantissaArgument, int USizeExponentArgument, typename TypeImplementationArgument>
   friend thisType powf(const TFloatInterval<USizeMantissaArgument, USizeExponentArgument, TypeImplementationArgument>& source, const thisType& value)
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
   template <int USizeMantissaArgument, int USizeExponentArgument, typename TypeImplementationArgument>
   friend thisType powf(const thisType& source, const TFloatInterval<USizeMantissaArgument, USizeExponentArgument, TypeImplementationArgument>& value)
      {  thisType result(source); result.powAssign(thisType(value)); return result; }
   friend thisType powf(const thisType& source, long double value)
      {  thisType result(source); result.powAssign(thisType(value)); return result; }
   friend thisType powf(const thisType& source, double value)
      {  thisType result(source); result.powAssign(thisType(value)); return result; }
   friend thisType powf(const thisType& source, float value)
      {  thisType result(source); result.powAssign(thisType(value)); return result; }
   friend thisType powf(const thisType& source, unsigned long value)
      {  thisType result(source); result.powAssign(thisType(value)); return result; }
   friend thisType powf(const thisType& source, long value)
      {  thisType result(source); result.powAssign(thisType(value)); return result; }
   friend thisType powf(const thisType& source, unsigned int value)
      {  thisType result(source); result.powAssign(thisType(value)); return result; }
   friend thisType powf(const thisType& source, int value)
      {  thisType result(source); result.powAssign(thisType(value)); return result; }
   friend thisType powf(const thisType& source, unsigned short value)
      {  thisType result(source); result.powAssign(thisType(value)); return result; }
   friend thisType powf(const thisType& source, short value)
      {  thisType result(source); result.powAssign(thisType(value)); return result; }

   friend thisType atan2(const thisType& source, const thisType& value)
      {  auto result(source); result.atan2Assign(value); return result; }
   template <int USizeMantissaArgument, int USizeExponentArgument, typename TypeImplementationArgument>
   friend thisType atan2(const TFloatInterval<USizeMantissaArgument, USizeExponentArgument, TypeImplementationArgument>& source, const thisType& value)
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
   template <int USizeMantissaArgument, int USizeExponentArgument, typename TypeImplementationArgument>
   friend thisType atan2(const thisType& source, const TFloatInterval<USizeMantissaArgument, USizeExponentArgument, TypeImplementationArgument>& value)
      {  thisType result(source); result.atan2Assign(thisType(value)); return result; }
   friend thisType atan2(const thisType& source, long double value)
      {  thisType result(source); result.atan2Assign(thisType(value)); return result; }
   friend thisType atan2(const thisType& source, double value)
      {  thisType result(source); result.atan2Assign(thisType(value)); return result; }
   friend thisType atan2(const thisType& source, float value)
      {  thisType result(source); result.atan2Assign(thisType(value)); return result; }
   friend thisType atan2(const thisType& source, unsigned long value)
      {  thisType result(source); result.atan2Assign(thisType(value)); return result; }
   friend thisType atan2(const thisType& source, long value)
      {  thisType result(source); result.atan2Assign(thisType(value)); return result; }
   friend thisType atan2(const thisType& source, unsigned int value)
      {  thisType result(source); result.atan2Assign(thisType(value)); return result; }
   friend thisType atan2(const thisType& source, int value)
      {  thisType result(source); result.atan2Assign(thisType(value)); return result; }
   friend thisType atan2(const thisType& source, unsigned short value)
      {  thisType result(source); result.atan2Assign(thisType(value)); return result; }
   friend thisType atan2(const thisType& source, short value)
      {  thisType result(source); result.atan2Assign(thisType(value)); return result; }

   template <int USizeMantissaArgument, int USizeExponentArgument, typename TypeImplementationArgument>
   friend bool operator<(const TFloatInterval<USizeMantissaArgument, USizeExponentArgument, TypeImplementationArgument>& fst, const thisType& snd)
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
   friend bool operator<=(const TFloatInterval<USizeMantissaArgument, USizeExponentArgument, TypeImplementationArgument>& fst, const thisType& snd)
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
   friend bool operator==(const TFloatInterval<USizeMantissaArgument, USizeExponentArgument, TypeImplementationArgument>& fst, const thisType& snd)
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
   friend bool operator!=(const TFloatInterval<USizeMantissaArgument, USizeExponentArgument, TypeImplementationArgument>& fst, const thisType& snd)
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
   friend bool operator>=(const TFloatInterval<USizeMantissaArgument, USizeExponentArgument, TypeImplementationArgument>& fst, const thisType& snd)
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
   friend bool operator>(const TFloatInterval<USizeMantissaArgument, USizeExponentArgument, TypeImplementationArgument>& fst, const thisType& snd)
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
   friend thisType operator+(const TFloatInterval<USizeMantissaArgument, USizeExponentArgument, TypeImplementationArgument>& fst, const thisType& snd)
      {  thisType fstConvert(fst); fstConvert += snd; return fstConvert; }
   friend thisType operator+(long double fst, const thisType& snd)
      {  thisType fstConvert(fst); fstConvert += snd; return fstConvert; }
   friend thisType operator+(double fst, const thisType& snd)
      {  thisType fstConvert(fst); fstConvert += snd; return fstConvert; }
   friend thisType operator+(float fst, const thisType& snd)
      {  thisType fstConvert(fst); fstConvert += snd; return fstConvert; }
   friend thisType operator+(unsigned long fst, const thisType& snd)
      {  thisType fstConvert(fst); fstConvert += snd; return fstConvert; }
   friend thisType operator+(long fst, const thisType& snd)
      {  thisType fstConvert(fst); fstConvert += snd; return fstConvert; }
   friend thisType operator+(unsigned int fst, const thisType& snd)
      {  thisType fstConvert(fst); fstConvert += snd; return fstConvert; }
   friend thisType operator+(int fst, const thisType& snd)
      {  thisType fstConvert(fst); fstConvert += snd; return fstConvert; }
   friend thisType operator+(unsigned short fst, const thisType& snd)
      {  thisType fstConvert(fst); fstConvert += snd; return fstConvert; }
   friend thisType operator+(short fst, const thisType& snd)
      {  thisType fstConvert(fst); fstConvert += snd; return fstConvert; }
   template <int USizeMantissaArgument, int USizeExponentArgument, typename TypeImplementationArgument>
   friend thisType operator-(const TFloatInterval<USizeMantissaArgument, USizeExponentArgument, TypeImplementationArgument>& fst, const thisType& snd)
      {  thisType fstConvert(fst); fstConvert -= snd; return fstConvert; }
   friend thisType operator-(long double fst, const thisType& snd)
      {  thisType fstConvert(fst); fstConvert -= snd; return fstConvert; }
   friend thisType operator-(double fst, const thisType& snd)
      {  thisType fstConvert(fst); fstConvert -= snd; return fstConvert; }
   friend thisType operator-(float fst, const thisType& snd)
      {  thisType fstConvert(fst); fstConvert -= snd; return fstConvert; }
   friend thisType operator-(unsigned long fst, const thisType& snd)
      {  thisType fstConvert(fst); fstConvert -= snd; return fstConvert; }
   friend thisType operator-(long fst, const thisType& snd)
      {  thisType fstConvert(fst); fstConvert -= snd; return fstConvert; }
   friend thisType operator-(unsigned int fst, const thisType& snd)
      {  thisType fstConvert(fst); fstConvert -= snd; return fstConvert; }
   friend thisType operator-(int fst, const thisType& snd)
      {  thisType fstConvert(fst); fstConvert -= snd; return fstConvert; }
   friend thisType operator-(unsigned short fst, const thisType& snd)
      {  thisType fstConvert(fst); fstConvert -= snd; return fstConvert; }
   friend thisType operator-(short fst, const thisType& snd)
      {  thisType fstConvert(fst); fstConvert -= snd; return fstConvert; }
   template <int USizeMantissaArgument, int USizeExponentArgument, typename TypeImplementationArgument>
   friend thisType operator*(const TFloatInterval<USizeMantissaArgument, USizeExponentArgument, TypeImplementationArgument>& fst, const thisType& snd)
      {  thisType fstConvert(fst); fstConvert *= snd; return fstConvert; }
   friend thisType operator*(long double fst, const thisType& snd)
      {  thisType fstConvert(fst); fstConvert *= snd; return fstConvert; }
   friend thisType operator*(double fst, const thisType& snd)
      {  thisType fstConvert(fst); fstConvert *= snd; return fstConvert; }
   friend thisType operator*(float fst, const thisType& snd)
      {  thisType fstConvert(fst); fstConvert *= snd; return fstConvert; }
   friend thisType operator*(unsigned long fst, const thisType& snd)
      {  thisType fstConvert(fst); fstConvert *= snd; return fstConvert; }
   friend thisType operator*(long fst, const thisType& snd)
      {  thisType fstConvert(fst); fstConvert *= snd; return fstConvert; }
   friend thisType operator*(unsigned int fst, const thisType& snd)
      {  thisType fstConvert(fst); fstConvert *= snd; return fstConvert; }
   friend thisType operator*(int fst, const thisType& snd)
      {  thisType fstConvert(fst); fstConvert *= snd; return fstConvert; }
   friend thisType operator*(unsigned short fst, const thisType& snd)
      {  thisType fstConvert(fst); fstConvert *= snd; return fstConvert; }
   friend thisType operator*(short fst, const thisType& snd)
      {  thisType fstConvert(fst); fstConvert *= snd; return fstConvert; }
   template <int USizeMantissaArgument, int USizeExponentArgument, typename TypeImplementationArgument>
   friend thisType operator/(const TFloatInterval<USizeMantissaArgument, USizeExponentArgument, TypeImplementationArgument>& fst, const thisType& snd)
      {  thisType fstConvert(fst); fstConvert /= snd; return fstConvert; }
   friend thisType operator/(long double fst, const thisType& snd)
      {  thisType fstConvert(fst); fstConvert /= snd; return fstConvert; }
   friend thisType operator/(double fst, const thisType& snd)
      {  thisType fstConvert(fst); fstConvert /= snd; return fstConvert; }
   friend thisType operator/(float fst, const thisType& snd)
      {  thisType fstConvert(fst); fstConvert /= snd; return fstConvert; }
   friend thisType operator/(unsigned long fst, const thisType& snd)
      {  thisType fstConvert(fst); fstConvert /= snd; return fstConvert; }
   friend thisType operator/(long fst, const thisType& snd)
      {  thisType fstConvert(fst); fstConvert /= snd; return fstConvert; }
   friend thisType operator/(unsigned int fst, const thisType& snd)
      {  thisType fstConvert(fst); fstConvert /= snd; return fstConvert; }
   friend thisType operator/(int fst, const thisType& snd)
      {  thisType fstConvert(fst); fstConvert /= snd; return fstConvert; }
   friend thisType operator/(unsigned short fst, const thisType& snd)
      {  thisType fstConvert(fst); fstConvert /= snd; return fstConvert; }
   friend thisType operator/(short fst, const thisType& snd)
      {  thisType fstConvert(fst); fstConvert /= snd; return fstConvert; }

   friend thisType floor(const thisType& fst)
      {  thisType result;
         result.continuousFlow(
               [](thisType& result, const thisType& fst)
                  {  result = thisType(fst.asInt(RMLowest)); },
               fst);
         return result;
      }
   friend thisType ceil(const thisType& fst)
      {  thisType result;
         result.continuousFlow(
               [](thisType& result, const thisType& fst)
                  {  result = thisType(fst.asInt(RMHighest)); },
               fst);
         return result;
      }
   friend thisType trunc(const thisType& fst)
      {  thisType result;
         result.continuousFlow(
               [](thisType& result, const thisType& fst)
                  {  result = thisType(fst.asInt(RMZero)); },
               fst);
         return result;
      }
   friend thisType round(const thisType& fst)
      {  thisType result;
         result.continuousFlow(
               [](thisType& result, const thisType& fst)
                  {  result = thisType(fst.asInt(RMNearest)); },
               fst);
         return result;
      }
   friend thisType rint(const thisType& fst)
      {  thisType result;
         result.continuousFlow(
               [](thisType& result, const thisType& fst)
                  {  result = thisType(fst.asInt(RMNearest)); },
               fst);
         return result;
      }
   friend thisType rintf(const thisType& fst)
      {  thisType result;
         result.continuousFlow(
               [](thisType& result, const thisType& fst)
                  {  result = thisType(fst.asInt(RMNearest)); },
               fst);
         return result;
      }
   friend thisType dis_floor(const thisType& fst) { return thisType(fst.asInt(RMLowest)); }
   friend thisType dis_ceil(const thisType& fst) { return thisType(fst.asInt(RMHighest)); }
   friend thisType dis_trunc(const thisType& fst) { return thisType(fst.asInt(RMZero)); }
   friend thisType dis_round(const thisType& fst) { return thisType(fst.asInt(RMNearest)); } 
   friend thisType dis_rintf(const thisType& fst) { return thisType(fst.asInt(RMNearest /* fegetround */)); } 
   friend thisType dis_rint(const thisType& fst) { return thisType(fst.asInt(RMNearest /* fegetround */)); } 

   friend thisType fabs(const thisType& source)
      {  auto result(source);
         result.continuousFlow([](thisType& val){ if (val < 0) val.oppositeAssign(); });
         return result;
      }
   friend thisType fabs(thisType&& source)
      {  source.continuousFlow([](thisType& val){ if (val < 0) val.oppositeAssign(); });
         return source;
      }
   friend thisType abs(const thisType& source)
      {  auto result(source);
         result.continuousFlow([](thisType& val){ if (val < 0) val.oppositeAssign(); });
         return result;
      }
   friend thisType abs(thisType&& source)
      {  source.continuousFlow([](thisType& val){ if (val < 0) val.oppositeAssign(); });
         return source;
      }

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
   friend thisType fmod(const TFloatInterval<USizeMantissaArgument, USizeExponentArgument, TypeImplementationArgument>& asource, const thisType& value)
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
   friend thisType fmod(const thisType& source, const TFloatInterval<USizeMantissaArgument, USizeExponentArgument, TypeImplementationArgument>& avalue)
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

   friend int fld_finite(const thisType& source) { return source.sfinite(); }
   friend int fld_isfinite(const thisType& source) { return source.sisfinite(); }
   friend int fld_isnan(const thisType& source) { return source.sisnan(); }
   friend int fld_isinf(const thisType& source) { return source.sisinf(); }
};

} // end of namespace DDoubleIntervalInterface

typedef DDoubleIntervalInterface::TFloatInterval<23, 8, float> FloatInterval;
typedef DDoubleIntervalInterface::TFloatInterval<52, 11, double> DoubleInterval;
typedef DDoubleIntervalInterface::TFloatInterval<LDBL_MANT_DIG-1,
      (LDBL_MAX_EXP == (1 << (16-2))) ? 15 /* leading 1 bit */
                                      : sizeof(long double)*8-LDBL_MANT_DIG,
      long double> LongDoubleInterval;

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

