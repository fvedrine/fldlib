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
// Unit      : Affine relationships
// File      : FloatAffineInterface.h
// Description :
//   Definition of a class of affine relations.
//

#ifndef FloatInstrumentation_FloatAffineH
#define FloatInstrumentation_FloatAffineH

/* for log */
#include <math.h>
#include "FloatInstrumentation/AffineTypesSize.h"
// #include <iosfwd>
#include <iostream>
#include <vector>
#include <cassert>

namespace NumericalDomains {

namespace DAffine {

class PathExplorer;
class DiagnosisReadStream;

}

namespace DAffineInterface {

class end {};

class ExecutionPath;
class PathExplorer {
  private:
   typedef void* /* size_t */ AlignType;
   static const int UPathExplorerSizeInBytes = NumericalDomains::DAffineInterface::PathExplorerSize;
   static const int UPathExplorerSize = (UPathExplorerSizeInBytes + sizeof(AlignType)-1)/(sizeof(AlignType) - 1);
   AlignType content[UPathExplorerSize];
   friend class ExecutionPath;

  public:
   enum Mode { MRealAndImplementation, MOnlyReal, MOnlyImplementation };

   PathExplorer();
   PathExplorer(Mode mode);
   PathExplorer(const PathExplorer& source);
   PathExplorer(PathExplorer&& source);
   ~PathExplorer();
   PathExplorer& operator=(const PathExplorer& source);
   PathExplorer& operator=(PathExplorer&& source);

   Mode mode() const;
   bool isFinished(Mode mode);
   bool isFinished();
};

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
class TFloatZonotope;

class MergeBranches {
  private:
   typedef void* /* size_t */ AlignType;
   static const int UMergeBranchesSizeInBytes = NumericalDomains::DAffineInterface::MergeBranchesSize;
   static const int UMergeBranchesSize = (UMergeBranchesSizeInBytes + sizeof(AlignType)-1)/(sizeof(AlignType) - 1);
   AlignType content[UMergeBranchesSize];

  public:
   MergeBranches(const char* file, int line);

   template <class TypeIterator>
   struct TPacker {
      TypeIterator iter, end;
      TPacker(TypeIterator aiter, TypeIterator aend) : iter(aiter), end(aend) {}
   };

   template <class TypeIterator>
   static TPacker<TypeIterator> packer(TypeIterator iter, TypeIterator end)
      {  return TPacker<TypeIterator>(iter, end); }

   template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
   MergeBranches& operator<<(const TFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>& value);
   template <class TypeIterator>
   MergeBranches& operator<<(TPacker<TypeIterator> packer)
      {  for (; packer.iter != packer.end; ++packer.iter)
            operator<<(*packer.iter);
         return *this;
      }
   bool operator<<(end);
};

template <typename TypeIterator, class TypeSaveMemory>
class TPackedSaveMemory;

template <class T1, class TypeSaveMemory>
class TSaveMemory {
  public:
   T1 save;
   TypeSaveMemory next;

   TSaveMemory(const T1& saveArg, TypeSaveMemory nextArg)
      :  save(saveArg, typename T1::Record()), next(nextArg) {}
   TSaveMemory(const TSaveMemory<T1, TypeSaveMemory>& source)
      :  save(const_cast<T1&>(source.save), typename T1::Record()), next(source.next) {}
   TSaveMemory(TSaveMemory<T1, TypeSaveMemory>&& source)
      :  save(const_cast<T1&>(source.save), typename T1::Record()), next(source.next) {}

   template <class T>
   TSaveMemory<T, TSaveMemory<T1, TypeSaveMemory> > operator<<(const T& t)
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

   template <class T>
   TSaveMemory<T, TSaveMemory<TypeIterator, TypeSaveMemory> > operator<<(T& t)
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
            assert(count == save.size());
            for (int index = 0; index < count; ++index) {
               *packer.iter = save[index];
               ++packer.iter;
            }
         }
         return next;
      }
   bool getResult() const { return next.getResult(); }
};

template <class T1, class TypeSaveMemory>
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

   template <class T>
   TSaveMemory<T, SaveMemory> operator<<(const T& t)
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

template <class T1, class TypeMergeMemory>
class TMergeMemory {
  public:
   T1 merge;
   TypeMergeMemory next;

   TMergeMemory(const T1&, TypeMergeMemory nextArg) : merge(), next(nextArg) {}
   TMergeMemory(const TMergeMemory<T1, TypeMergeMemory>&) = default;
   TMergeMemory(TMergeMemory<T1, TypeMergeMemory>&&) = default;

   template <class T>
   TMergeMemory<T, TMergeMemory<T1, TypeMergeMemory> > operator>>(const T& t)
      {  return TMergeMemory<T, TMergeMemory<T1, TypeMergeMemory> >(t, *this); }
   template <typename TypeIterator>
   TPackedMergeMemory<TypeIterator, TMergeMemory<T1, TypeMergeMemory> > operator>>(MergeBranches::TPacker<TypeIterator> packer);
   TMergeMemory<T1, TypeMergeMemory>& operator>>(end) { return *this; }
   TMergeMemory<T1, TypeMergeMemory>& setCurrentComplete(bool isComplete)
      {  next.setCurrentComplete(isComplete); return *this; }
   TypeMergeMemory& operator<<(T1& val)
      {  if (isComplete()) {
            if (val.optimizeValue()) {
               if (next.isFirst())
                  merge.recordFrom(std::move(val));
               else
                  merge.mergeWith(std::move(val));
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
         if (isComplete()) {
            if (val.optimizeValue()) {
               if (next.isFirst())
                  merge.recordFrom(std::move(val));
               else
                  merge.mergeWith(std::move(val));
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
   std::vector<typename TypeIterator::value_type> merge;
   TypeMergeMemory next;

   TPackedMergeMemory(TypeIterator iter, TypeIterator end, TypeMergeMemory nextArg)
      :  next(nextArg) {}
   TPackedMergeMemory(const TPackedMergeMemory<TypeIterator, TypeMergeMemory>&) = default;
   TPackedMergeMemory(TPackedMergeMemory<TypeIterator, TypeMergeMemory>&&) = default;

   template <class T>
   TMergeMemory<T, TPackedMergeMemory<TypeIterator, TypeMergeMemory> > operator>>(T& t)
      {  return TMergeMemory<T, TPackedMergeMemory<TypeIterator, TypeMergeMemory> >(t, *this); }
   template <class TypeIteratorArgument>
   TPackedMergeMemory<TypeIteratorArgument, TPackedMergeMemory<TypeIterator, TypeMergeMemory> >
      operator>>(MergeBranches::TPacker<TypeIteratorArgument> packer)
      {  return TPackedMergeMemory<TypeIteratorArgument, TPackedMergeMemory<TypeIterator, TypeMergeMemory> >
            (packer.iter, packer.end, *this);
      }
   TPackedMergeMemory<TypeIterator, TypeMergeMemory>& operator>>(end) { return *this; }
   TPackedMergeMemory<TypeIterator, TypeMergeMemory>& setCurrentComplete(bool isComplete)
      {  next.setCurrentComplete(isComplete); return *this; }
   TypeMergeMemory& operator<<(MergeBranches::TPacker<TypeIterator>&& packer)
      {  int count = packer.end - packer.iter;
         if (isComplete()) {
            auto iter = packer.iter;
            if (next.isFirst()) {
               assert(merge.size() == 0);
               merge.reserve(count);
               for (; iter != packer.end; ++iter) {
                  if (iter->optimizeValue()) {
                     typename TypeIterator::value_type val;
                     merge.push_back(val);
                     merge.back().recordFrom(std::move(*iter));
                  }
                  else
                     next.setCurrentComplete(false);
               };
            }
            else {
               for (int index = 0; index < count; ++index) {
                  if (iter->optimizeValue())
                     merge[index].mergeWith(std::move(*iter));
                  else
                     next.setCurrentComplete(false);
                  ++iter;
               }
            }
         };
         if (merge.size() > 0) {
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

template <class T1, class TypeMergeMemory>
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

   template <class T>
   TMergeMemory<T, MergeMemory> operator>>(const T& t)
      {  return TMergeMemory<T, MergeMemory>(t, *this); }
   template <typename TypeIterator>
   TPackedMergeMemory<TypeIterator, MergeMemory> operator>>(MergeBranches::TPacker<TypeIterator>&& packer)
      {  return TPackedMergeMemory<TypeIterator, MergeMemory>(packer.iter, packer.end, *this); }
   MergeMemory& operator>>(end) { return *this; }
   MergeMemory& setCurrentComplete(bool isComplete) { fComplete = isComplete; return *this; }
   bool isFirst() const { return fFirst; }
   bool isComplete() const { return fComplete; }
   bool operator<<(end)
      {  if (fComplete)
            fFirst = false;
         return true;
      }
};

class ExecutionPath {
  public:
   static void splitBranches(const char* file, int line);

   bool hasMultipleBranches() const;
   static void setSupportAtomic();
   static void setSupportUnstableInLoop(bool value=true);
   static void setSupportVerbose();
   static void setSupportThreshold();
   static void setSupportFirstFollowFloat();
   static void initializeGlobals(const char* fileSuffix);
   static void finalizeGlobals();
   static void setSimplificationTriggerPercent(double percent);
   static bool doesSupportUnstableInLoop();
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
      void setSimplificationTriggerPercent(double percent) { ExecutionPath::setSimplificationTriggerPercent(percent); }
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
   static void flushOut();
   static void setSourceLine(const char* file, int line);
   static void writeCurrentPath(std::ostream& out);
   static DAffine::PathExplorer* getCurrentPathExplorer();
   static void setCurrentPathExplorer(PathExplorer* pathExplorer);
   static void setCurrentPathExplorer(DAffine::PathExplorer* pathExplorer);

   // float loop unstable
   static DAffine::DiagnosisReadStream* inputTraceFile();
   static const char* synchronisationFile();
   static int synchronisationLine();
   static bool doesFollowFlow();
   static void clearFollowFlow();
   static void setFollowFlow(bool doesFollowFlow, DAffine::DiagnosisReadStream* inputTraceFile,
         const char* synchronizationFile, int synchronizationLine);
   static void setFollowFlow();
   static PathExplorer::Mode queryMode(DAffine::PathExplorer* pathExplorer);
   // end of float loop unstable

   static void throwEmptyBranch(bool isUnstable);

  public:
   static void clearSynchronizationBranches();
};

typedef ExecutionPath BaseFloatAffine; // for splitBranches

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
class TFloatZonotope {
  private:
   typedef TFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation> thisType;

   typedef void* /* size_t */ AlignType;
   static const int UFloatZonotopeSizeInBytes = NumericalDomains::DAffineInterface
      ::TFloatZonotopeSizeTraits<USizeMantissa, USizeExponent, TypeImplementation>::USize;
   static const int UFloatZonotopeSize = (UFloatZonotopeSizeInBytes + sizeof(AlignType)-1)/(sizeof(AlignType) - 1);
   AlignType content[UFloatZonotopeSize];

   template <int USizeMantissaArgument, int USizeExponentArgument, typename TypeImplementationArgument>
   friend class TFloatZonotope;
   friend class DAffineInterface::MergeBranches;

   enum RoundMode { RMNearest, RMLowest, RMHighest, RMZero };
   int asInt(RoundMode mode) const;

   int sfinite() const;
   int sisfinite() const;
   int sisnan() const;
   int sisinf() const;

  public:
   typedef DAffineInterface::MergeBranches MergeBranches;

   const char* queryDebugValue() const;
   const char* queryLightDebugValue() const;
   typedef ExecutionPath::Initialization Initialization;
   typedef ExecutionPath::anticipated_termination anticipated_termination;
   typedef DAffineInterface::end end;
   static void flushOut() { ExecutionPath::flushOut(); }

  public:
   class Record {};

   TFloatZonotope();
   TFloatZonotope(float value);
   TFloatZonotope(double value);
   TFloatZonotope(long double value);
   TFloatZonotope(TypeImplementation min, TypeImplementation max);
   TFloatZonotope(TypeImplementation min, TypeImplementation max,
         TypeImplementation errmin, TypeImplementation errmax);
   TFloatZonotope(short int value);
   TFloatZonotope(int value);
   TFloatZonotope(long int value);
   TFloatZonotope(unsigned short value);
   TFloatZonotope(unsigned value);
   TFloatZonotope(unsigned long value);
   TFloatZonotope(const thisType& source);
   TFloatZonotope(const thisType& source, Record);
   TFloatZonotope(thisType&& source);
   TFloatZonotope(thisType&& source, Record);

   template <int USizeMantissaArgument, int USizeExponentArgument, typename TypeImplementationArgument>
   TFloatZonotope(const TFloatZonotope<USizeMantissaArgument, USizeExponentArgument, TypeImplementationArgument>& source);
   template <int USizeMantissaArgument, int USizeExponentArgument, typename TypeImplementationArgument>
   TFloatZonotope(TFloatZonotope<USizeMantissaArgument, USizeExponentArgument, TypeImplementationArgument>&& source);

   ~TFloatZonotope();

   thisType& operator=(const thisType& source);
   void recordFrom(const thisType& source);
   void mergeWith(const thisType& source);
   thisType& operator=(thisType&& source);
   void recordFrom(thisType&& source);
   void mergeWith(thisType&& source);
   template <int USizeMantissaArgument, int USizeExponentArgument, typename TypeImplementationArgument>
   thisType& operator=(const TFloatZonotope<USizeMantissaArgument, USizeExponentArgument, TypeImplementationArgument>& source);
   template <int USizeMantissaArgument, int USizeExponentArgument, typename TypeImplementationArgument>
   thisType& operator=(TFloatZonotope<USizeMantissaArgument, USizeExponentArgument, TypeImplementationArgument>&& source);

   TypeImplementation asImplementation() const;
   void readImplementation(std::istream& in);
   void writeImplementation(std::ostream& out) const;
   friend std::ostream& operator<<(std::ostream& out, const thisType& value)
      {  value.writeImplementation(out); return out; }
   friend std::istream& operator>>(std::istream& in, thisType& value)
      {  value.readImplementation(in); return in; }

   bool optimizeValue();
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
   thisType& operator+=(thisType&& source);
   thisType& operator-=(const thisType& source);
   thisType& operator-=(thisType&& source);
   thisType& operator*=(const thisType& source);
   thisType& operator*=(thisType&& source);
   thisType& operator/=(const thisType& source);
   thisType& operator/=(thisType&& source);

   friend thisType operator-(const thisType& first)
      {  thisType result(first); result.oppositeAssign(); return result; }
   friend thisType operator-(thisType&& first)
      {  thisType result(std::forward<thisType>(first)); result.oppositeAssign(); return result; }
   friend thisType operator+(const thisType& first) { return first; }
   friend thisType operator+(thisType&& first) { return first; }
   friend thisType operator+(const thisType& first, const thisType& second)
      {  thisType result(first);
         result.plusAssign(second);
         return result;
      }
   friend thisType operator+(thisType&& first, const thisType& second)
      {  thisType result(std::forward<thisType>(first));
         result.plusAssign(second);
         return result;
      }
   friend thisType operator+(const thisType& first, thisType&& second)
      {  thisType result(first);
         result.plusAssign(std::forward<thisType>(second));
         return result;
      }
   friend thisType operator+(thisType&& first, thisType&& second)
      {  thisType result(std::forward<thisType>(first));
         result.plusAssign(std::forward<thisType>(second));
         return result;
      }
   friend thisType operator+(const thisType&& first, TypeImplementation second)
      {  thisType result(first);
         result.plusAssign(thisType(second));
         return result;
      }
   friend thisType operator+(thisType&& first, TypeImplementation second)
      {  thisType result(std::forward<thisType>(first));
         result.plusAssign(thisType(second));
         return result;
      }
   friend thisType operator-(const thisType& first, const thisType& second)
      {  thisType result(first);
         result.minusAssign(second);
         return result;
      }
   friend thisType operator-(thisType&& first, const thisType& second)
      {  thisType result(std::forward<thisType>(first));
         result.minusAssign(second);
         return result;
      }
   friend thisType operator-(const thisType& first, thisType&& second)
      {  thisType result(first);
         result.minusAssign(std::forward<thisType>(second));
         return result;
      }
   friend thisType operator-(thisType&& first, thisType&& second)
      {  thisType result(std::forward<thisType>(first));
         result.minusAssign(std::forward<thisType>(second));
         return result;
      }
   friend thisType operator-(const thisType&& first, TypeImplementation second)
      {  thisType result(first);
         result.minusAssign(thisType(second));
         return result;
      }
   friend thisType operator-(thisType&& first, TypeImplementation second)
      {  thisType result(std::forward<thisType>(first));
         result.minusAssign(thisType(second));
         return result;
      }
   friend thisType operator*(const thisType& first, const thisType& second)
      {  thisType result(first);
         result.multAssign(second);
         return result;
      }
   friend thisType operator*(thisType&& first, const thisType& second)
      {  thisType result(std::forward<thisType>(first));
         result.multAssign(second);
         return result;
      }
   friend thisType operator*(const thisType& first, thisType&& second)
      {  thisType result(first);
         result.multAssign(std::forward<thisType>(second));
         return result;
      }
   friend thisType operator*(thisType&& first, thisType&& second)
      {  thisType result(std::forward<thisType>(first));
         result.multAssign(std::forward<thisType>(second));
         return result;
      }
   friend thisType operator*(const thisType&& first, TypeImplementation second)
      {  thisType result(first);
         result.multAssign(thisType(second));
         return result;
      }
   friend thisType operator*(thisType&& first, TypeImplementation second)
      {  thisType result(std::forward<thisType>(first));
         result.multAssign(thisType(second));
         return result;
      }
   friend thisType operator/(const thisType& first, const thisType& second)
      {  thisType result(first);
         result.divAssign(second);
         return result;
      }
   friend thisType operator/(thisType&& first, const thisType& second)
      {  thisType result(std::forward<thisType>(first));
         result.divAssign(second);
         return result;
      }
   friend thisType operator/(const thisType& first, thisType&& second)
      {  thisType result(first);
         result.divAssign(std::forward<thisType>(second));
         return result;
      }
   friend thisType operator/(thisType&& first, thisType&& second)
      {  thisType result(std::forward<thisType>(first));
         result.divAssign(std::forward<thisType>(second));
         return result;
      }
   friend thisType operator/(const thisType&& first, TypeImplementation second)
      {  thisType result(first);
         result.divAssign(thisType(second));
         return result;
      }
   friend thisType operator/(thisType&& first, TypeImplementation second)
      {  thisType result(std::forward<thisType>(first));
         result.divAssign(thisType(second));
         return result;
      }

   void oppositeAssign();
   void inverseAssign();
   void plusAssign(const thisType& source);
   void plusAssign(thisType&& source);
   void minusAssign(const thisType& source);
   void minusAssign(thisType&& source);
   void multAssign(const thisType& source);
   void multAssign(thisType&& source);
   void divAssign(const thisType& source);
   void divAssign(thisType&& source);

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
   void powAssign(thisType&& value);
   void atan2Assign(const thisType& value);
   void atan2Assign(thisType&& value);

   explicit operator TypeImplementation() const;
   explicit operator int() const;
   explicit operator short int() const;
   explicit operator unsigned() const;
   explicit operator short unsigned() const;
   explicit operator long int() const;
   explicit operator unsigned long() const;

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

   void lightPersist(const char* prefix) const;
   void persist(const char* prefix) const;

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
      {   thisType result(std::forward<thisType>(source)); result.cosAssign(); return result; }
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
      {  thisType result(source); result.logAssign(); result.divAssign(log(thisType(2.0))); return result; }
   friend thisType log2(thisType&& source)
      {  thisType result(std::forward<thisType>(source)); result.logAssign(); result.divAssign(log(thisType(2.0))); return result; }
   friend thisType exp2(const thisType& source)
      {  thisType result = 2.0; result.powAssign(source); return result; }
   friend thisType exp2(thisType&& source)
      {  thisType result = 2.0; result.powAssign(std::forward<thisType>(source)); return result; }
   friend thisType log10(const thisType& source)
      {  thisType result(source); result.log10Assign(); return result; }
   friend thisType log10(thisType&& source)
      {  thisType result(std::forward<thisType>(source)); result.log10Assign(); return result; }
   friend thisType pow(const thisType& source, const thisType& value)
      {  thisType result(source); result.powAssign(value); return result; }
   friend thisType pow(const thisType& source, thisType&& value)
      {  thisType result(source); result.powAssign(std::forward<thisType>(value)); return result; }
   friend thisType pow(thisType&& source, const thisType& value)
      {  thisType result(std::forward<thisType>(source)); result.powAssign(value); return result; }
   friend thisType pow(thisType&& source, thisType&& value)
      {  thisType result(std::forward<thisType>(source)); result.powAssign(std::forward<thisType>(value)); return result; }

   template <int USizeMantissaArgument, int USizeExponentArgument, typename TypeImplementationArgument>
   friend thisType pow(const TFloatZonotope<USizeMantissaArgument, USizeExponentArgument, TypeImplementationArgument>& source, const thisType& value)
      {  thisType result(source); result.powAssign(value); return result; }
   template <int USizeMantissaArgument, int USizeExponentArgument, typename TypeImplementationArgument>
   friend thisType pow(TFloatZonotope<USizeMantissaArgument, USizeExponentArgument, TypeImplementationArgument>&& source, const thisType& value)
      {  thisType result(std::forward<TFloatZonotope<USizeMantissaArgument, USizeExponentArgument, TypeImplementationArgument>>(source));
         result.powAssign(value); return result;
      }
   template <int USizeMantissaArgument, int USizeExponentArgument, typename TypeImplementationArgument>
   friend thisType pow(const TFloatZonotope<USizeMantissaArgument, USizeExponentArgument, TypeImplementationArgument>& source, thisType&& value)
      {  thisType result(source); result.powAssign(std::forward<thisType>(value)); return result; }
   template <int USizeMantissaArgument, int USizeExponentArgument, typename TypeImplementationArgument>
   friend thisType pow(TFloatZonotope<USizeMantissaArgument, USizeExponentArgument, TypeImplementationArgument>&& source, thisType&& value)
      {  thisType result(std::forward<TFloatZonotope<USizeMantissaArgument, USizeExponentArgument, TypeImplementationArgument>>(source));
         result.powAssign(std::forward<thisType>(value)); return result;
      }
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
   friend thisType pow(long double source, thisType&& value)
      {  thisType result(source); result.powAssign(std::forward<thisType>(value)); return result; }
   friend thisType pow(double source, thisType&& value)
      {  thisType result(source); result.powAssign(std::forward<thisType>(value)); return result; }
   friend thisType pow(float source, thisType&& value)
      {  thisType result(source); result.powAssign(std::forward<thisType>(value)); return result; }
   friend thisType pow(unsigned long source, thisType&& value)
      {  thisType result(source); result.powAssign(std::forward<thisType>(value)); return result; }
   friend thisType pow(long source, thisType&& value)
      {  thisType result(source); result.powAssign(std::forward<thisType>(value)); return result; }
   friend thisType pow(unsigned int source, thisType&& value)
      {  thisType result(source); result.powAssign(std::forward<thisType>(value)); return result; }
   friend thisType pow(int source, thisType&& value)
      {  thisType result(source); result.powAssign(std::forward<thisType>(value)); return result; }
   friend thisType pow(unsigned short source, thisType&& value)
      {  thisType result(source); result.powAssign(std::forward<thisType>(value)); return result; }
   friend thisType pow(short source, thisType&& value)
      {  thisType result(source); result.powAssign(std::forward<thisType>(value)); return result; }
   template <int USizeMantissaArgument, int USizeExponentArgument, typename TypeImplementationArgument>
   friend thisType pow(const thisType& source, const TFloatZonotope<USizeMantissaArgument, USizeExponentArgument, TypeImplementationArgument>& value)
      {  thisType result(source); result.powAssign(thisType(value)); return result; }
   template <int USizeMantissaArgument, int USizeExponentArgument, typename TypeImplementationArgument>
   friend thisType pow(thisType&& source, const TFloatZonotope<USizeMantissaArgument, USizeExponentArgument, TypeImplementationArgument>& value)
      {  thisType result(std::forward<thisType>(source)); result.powAssign(thisType(value)); return result; }
   template <int USizeMantissaArgument, int USizeExponentArgument, typename TypeImplementationArgument>
   friend thisType pow(const thisType& source, TFloatZonotope<USizeMantissaArgument, USizeExponentArgument, TypeImplementationArgument>&& value)
      {  thisType result(source); result.powAssign(thisType(std::forward<TFloatZonotope<USizeMantissaArgument, USizeExponentArgument, TypeImplementationArgument> >(value))); return result; }
   template <int USizeMantissaArgument, int USizeExponentArgument, typename TypeImplementationArgument>
   friend thisType pow(thisType&& source, TFloatZonotope<USizeMantissaArgument, USizeExponentArgument, TypeImplementationArgument>&& value)
      {  thisType result(std::forward<thisType>(source)); result.powAssign(thisType(std::forward<TFloatZonotope<USizeMantissaArgument, USizeExponentArgument, TypeImplementationArgument> >(value))); return result; }
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
   friend thisType pow(thisType&& source, long double value)
      {  thisType result(std::forward<thisType>(source)); result.powAssign(thisType(value)); return result; }
   friend thisType pow(thisType&& source, double value)
      {  thisType result(std::forward<thisType>(source)); result.powAssign(thisType(value)); return result; }
   friend thisType pow(thisType&& source, float value)
      {  thisType result(std::forward<thisType>(source)); result.powAssign(thisType(value)); return result; }
   friend thisType pow(thisType&& source, unsigned long value)
      {  thisType result(std::forward<thisType>(source)); result.powAssign(thisType(value)); return result; }
   friend thisType pow(thisType&& source, long value)
      {  thisType result(std::forward<thisType>(source)); result.powAssign(thisType(value)); return result; }
   friend thisType pow(thisType&& source, unsigned int value)
      {  thisType result(std::forward<thisType>(source)); result.powAssign(thisType(value)); return result; }
   friend thisType pow(thisType&& source, int value)
      {  thisType result(std::forward<thisType>(source)); result.powAssign(thisType(value)); return result; }
   friend thisType pow(thisType&& source, unsigned short value)
      {  thisType result(std::forward<thisType>(source)); result.powAssign(thisType(value)); return result; }
   friend thisType pow(thisType&& source, short value)
      {  thisType result(std::forward<thisType>(source)); result.powAssign(thisType(value)); return result; }

   friend thisType powf(const thisType& source, const thisType& value)
      {  thisType result(source); result.powAssign(value); return result; }
   friend thisType powf(const thisType& source, thisType&& value)
      {  thisType result(source); result.powAssign(std::forward<thisType>(value)); return result; }
   friend thisType powf(thisType&& source, const thisType& value)
      {  thisType result(std::forward<thisType>(source)); result.powAssign(value); return result; }
   friend thisType powf(thisType&& source, thisType&& value)
      {  thisType result(std::forward<thisType>(source)); result.powAssign(std::forward<thisType>(value)); return result; }

   template <int USizeMantissaArgument, int USizeExponentArgument, typename TypeImplementationArgument>
   friend thisType powf(const TFloatZonotope<USizeMantissaArgument, USizeExponentArgument, TypeImplementationArgument>& source, const thisType& value)
      {  thisType result(source); result.powAssign(value); return result; }
   template <int USizeMantissaArgument, int USizeExponentArgument, typename TypeImplementationArgument>
   friend thisType powf(TFloatZonotope<USizeMantissaArgument, USizeExponentArgument, TypeImplementationArgument>&& source, const thisType& value)
      {  thisType result(std::forward<TFloatZonotope<USizeMantissaArgument, USizeExponentArgument, TypeImplementationArgument>>(source));
         result.powAssign(value); return result;
      }
   template <int USizeMantissaArgument, int USizeExponentArgument, typename TypeImplementationArgument>
   friend thisType powf(const TFloatZonotope<USizeMantissaArgument, USizeExponentArgument, TypeImplementationArgument>& source, thisType&& value)
      {  thisType result(source); result.powAssign(std::forward<thisType>(value)); return result; }
   template <int USizeMantissaArgument, int USizeExponentArgument, typename TypeImplementationArgument>
   friend thisType powf(TFloatZonotope<USizeMantissaArgument, USizeExponentArgument, TypeImplementationArgument>&& source, thisType&& value)
      {  thisType result(std::forward<TFloatZonotope<USizeMantissaArgument, USizeExponentArgument, TypeImplementationArgument>>(source));
         result.powAssign(std::forward<thisType>(value)); return result;
      }
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
   friend thisType powf(long double source, thisType&& value)
      {  thisType result(source); result.powAssign(std::forward<thisType>(value)); return result; }
   friend thisType powf(double source, thisType&& value)
      {  thisType result(source); result.powAssign(std::forward<thisType>(value)); return result; }
   friend thisType powf(float source, thisType&& value)
      {  thisType result(source); result.powAssign(std::forward<thisType>(value)); return result; }
   friend thisType powf(unsigned long source, thisType&& value)
      {  thisType result(source); result.powAssign(std::forward<thisType>(value)); return result; }
   friend thisType powf(long source, thisType&& value)
      {  thisType result(source); result.powAssign(std::forward<thisType>(value)); return result; }
   friend thisType powf(unsigned int source, thisType&& value)
      {  thisType result(source); result.powAssign(std::forward<thisType>(value)); return result; }
   friend thisType powf(int source, thisType&& value)
      {  thisType result(source); result.powAssign(std::forward<thisType>(value)); return result; }
   friend thisType powf(unsigned short source, thisType&& value)
      {  thisType result(source); result.powAssign(std::forward<thisType>(value)); return result; }
   friend thisType powf(short source, thisType&& value)
      {  thisType result(source); result.powAssign(std::forward<thisType>(value)); return result; }
   template <int USizeMantissaArgument, int USizeExponentArgument, typename TypeImplementationArgument>
   friend thisType powf(const thisType& source, const TFloatZonotope<USizeMantissaArgument, USizeExponentArgument, TypeImplementationArgument>& value)
      {  thisType result(source); result.powAssign(thisType(value)); return result; }
   template <int USizeMantissaArgument, int USizeExponentArgument, typename TypeImplementationArgument>
   friend thisType powf(thisType&& source, const TFloatZonotope<USizeMantissaArgument, USizeExponentArgument, TypeImplementationArgument>& value)
      {  thisType result(std::forward<thisType>(source)); result.powAssign(thisType(value)); return result; }
   template <int USizeMantissaArgument, int USizeExponentArgument, typename TypeImplementationArgument>
   friend thisType powf(const thisType& source, TFloatZonotope<USizeMantissaArgument, USizeExponentArgument, TypeImplementationArgument>&& value)
      {  thisType result(source); result.powAssign(thisType(std::forward<TFloatZonotope<USizeMantissaArgument, USizeExponentArgument, TypeImplementationArgument> >(value))); return result; }
   template <int USizeMantissaArgument, int USizeExponentArgument, typename TypeImplementationArgument>
   friend thisType powf(thisType&& source, TFloatZonotope<USizeMantissaArgument, USizeExponentArgument, TypeImplementationArgument>&& value)
      {  thisType result(std::forward<thisType>(source)); result.powAssign(thisType(std::forward<TFloatZonotope<USizeMantissaArgument, USizeExponentArgument, TypeImplementationArgument> >(value))); return result; }
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
   friend thisType powf(thisType&& source, long double value)
      {  thisType result(std::forward<thisType>(source)); result.powAssign(thisType(value)); return result; }
   friend thisType powf(thisType&& source, double value)
      {  thisType result(std::forward<thisType>(source)); result.powAssign(thisType(value)); return result; }
   friend thisType powf(thisType&& source, float value)
      {  thisType result(std::forward<thisType>(source)); result.powAssign(thisType(value)); return result; }
   friend thisType powf(thisType&& source, unsigned long value)
      {  thisType result(std::forward<thisType>(source)); result.powAssign(thisType(value)); return result; }
   friend thisType powf(thisType&& source, long value)
      {  thisType result(std::forward<thisType>(source)); result.powAssign(thisType(value)); return result; }
   friend thisType powf(thisType&& source, unsigned int value)
      {  thisType result(std::forward<thisType>(source)); result.powAssign(thisType(value)); return result; }
   friend thisType powf(thisType&& source, int value)
      {  thisType result(std::forward<thisType>(source)); result.powAssign(thisType(value)); return result; }
   friend thisType powf(thisType&& source, unsigned short value)
      {  thisType result(std::forward<thisType>(source)); result.powAssign(thisType(value)); return result; }
   friend thisType powf(thisType&& source, short value)
      {  thisType result(std::forward<thisType>(source)); result.powAssign(thisType(value)); return result; }

   friend thisType atan2(const thisType& source, const thisType& value)
      {  thisType result(source); result.atan2Assign(value); return result; }
   friend thisType atan2(const thisType& source, thisType&& value)
      {  thisType result(source); result.atan2Assign(std::forward<thisType>(value)); return result; }
   friend thisType atan2(thisType&& source, const thisType& value)
      {  thisType result(std::forward<thisType>(source)); result.atan2Assign(value); return result; }
   friend thisType atan2(thisType&& source, thisType&& value)
      {  thisType result(std::forward<thisType>(source)); result.atan2Assign(std::forward<thisType>(value)); return result; }

   template <int USizeMantissaArgument, int USizeExponentArgument, typename TypeImplementationArgument>
   friend thisType atan2(const TFloatZonotope<USizeMantissaArgument, USizeExponentArgument, TypeImplementationArgument>& source, const thisType& value)
      {  thisType result(source); result.atan2Assign(value); return result; }
   template <int USizeMantissaArgument, int USizeExponentArgument, typename TypeImplementationArgument>
   friend thisType atan2(TFloatZonotope<USizeMantissaArgument, USizeExponentArgument, TypeImplementationArgument>&& source, const thisType& value)
      {  thisType result(std::forward<TFloatZonotope<USizeMantissaArgument, USizeExponentArgument, TypeImplementationArgument>>(source));
         result.atan2Assign(value); return result;
      }
   template <int USizeMantissaArgument, int USizeExponentArgument, typename TypeImplementationArgument>
   friend thisType atan2(const TFloatZonotope<USizeMantissaArgument, USizeExponentArgument, TypeImplementationArgument>& source, thisType&& value)
      {  thisType result(source); result.atan2Assign(std::forward<thisType>(value)); return result; }
   template <int USizeMantissaArgument, int USizeExponentArgument, typename TypeImplementationArgument>
   friend thisType atan2(TFloatZonotope<USizeMantissaArgument, USizeExponentArgument, TypeImplementationArgument>&& source, thisType&& value)
      {  thisType result(std::forward<TFloatZonotope<USizeMantissaArgument, USizeExponentArgument, TypeImplementationArgument>>(source));
         result.atan2Assign(std::forward<thisType>(value)); return result;
      }
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
   friend thisType atan2(long double source, thisType&& value)
      {  thisType result(source); result.atan2Assign(std::forward<thisType>(value)); return result; }
   friend thisType atan2(double source, thisType&& value)
      {  thisType result(source); result.atan2Assign(std::forward<thisType>(value)); return result; }
   friend thisType atan2(float source, thisType&& value)
      {  thisType result(source); result.atan2Assign(std::forward<thisType>(value)); return result; }
   friend thisType atan2(unsigned long source, thisType&& value)
      {  thisType result(source); result.atan2Assign(std::forward<thisType>(value)); return result; }
   friend thisType atan2(long source, thisType&& value)
      {  thisType result(source); result.atan2Assign(std::forward<thisType>(value)); return result; }
   friend thisType atan2(unsigned int source, thisType&& value)
      {  thisType result(source); result.atan2Assign(std::forward<thisType>(value)); return result; }
   friend thisType atan2(int source, thisType&& value)
      {  thisType result(source); result.atan2Assign(std::forward<thisType>(value)); return result; }
   friend thisType atan2(unsigned short source, thisType&& value)
      {  thisType result(source); result.atan2Assign(std::forward<thisType>(value)); return result; }
   friend thisType atan2(short source, thisType&& value)
      {  thisType result(source); result.atan2Assign(std::forward<thisType>(value)); return result; }
   template <int USizeMantissaArgument, int USizeExponentArgument, typename TypeImplementationArgument>
   friend thisType atan2(const thisType& source, const TFloatZonotope<USizeMantissaArgument, USizeExponentArgument, TypeImplementationArgument>& value)
      {  thisType result(source); result.atan2Assign(thisType(value)); return result; }
   template <int USizeMantissaArgument, int USizeExponentArgument, typename TypeImplementationArgument>
   friend thisType atan2(thisType&& source, const TFloatZonotope<USizeMantissaArgument, USizeExponentArgument, TypeImplementationArgument>& value)
      {  thisType result(std::forward<thisType>(source)); result.atan2Assign(thisType(value)); return result; }
   template <int USizeMantissaArgument, int USizeExponentArgument, typename TypeImplementationArgument>
   friend thisType atan2(const thisType& source, TFloatZonotope<USizeMantissaArgument, USizeExponentArgument, TypeImplementationArgument>&& value)
      {  thisType result(source); result.atan2Assign(thisType(std::forward<TFloatZonotope<USizeMantissaArgument, USizeExponentArgument, TypeImplementationArgument> >(value))); return result; }
   template <int USizeMantissaArgument, int USizeExponentArgument, typename TypeImplementationArgument>
   friend thisType atan2(thisType&& source, TFloatZonotope<USizeMantissaArgument, USizeExponentArgument, TypeImplementationArgument>&& value)
      {  thisType result(std::forward<thisType>(source)); result.atan2Assign(thisType(std::forward<TFloatZonotope<USizeMantissaArgument, USizeExponentArgument, TypeImplementationArgument> >(value))); return result; }
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
   friend thisType atan2(thisType&& source, long double value)
      {  thisType result(std::forward<thisType>(source)); result.atan2Assign(thisType(value)); return result; }
   friend thisType atan2(thisType&& source, double value)
      {  thisType result(std::forward<thisType>(source)); result.atan2Assign(thisType(value)); return result; }
   friend thisType atan2(thisType&& source, float value)
      {  thisType result(std::forward<thisType>(source)); result.atan2Assign(thisType(value)); return result; }
   friend thisType atan2(thisType&& source, unsigned long value)
      {  thisType result(std::forward<thisType>(source)); result.atan2Assign(thisType(value)); return result; }
   friend thisType atan2(thisType&& source, long value)
      {  thisType result(std::forward<thisType>(source)); result.atan2Assign(thisType(value)); return result; }
   friend thisType atan2(thisType&& source, unsigned int value)
      {  thisType result(std::forward<thisType>(source)); result.atan2Assign(thisType(value)); return result; }
   friend thisType atan2(thisType&& source, int value)
      {  thisType result(std::forward<thisType>(source)); result.atan2Assign(thisType(value)); return result; }
   friend thisType atan2(thisType&& source, unsigned short value)
      {  thisType result(std::forward<thisType>(source)); result.atan2Assign(thisType(value)); return result; }
   friend thisType atan2(thisType&& source, short value)
      {  thisType result(std::forward<thisType>(source)); result.atan2Assign(thisType(value)); return result; }

   template <int USizeMantissaArgument, int USizeExponentArgument, typename TypeImplementationArgument>
   friend bool operator<(const TFloatZonotope<USizeMantissaArgument, USizeExponentArgument, TypeImplementationArgument>& fst, const thisType& snd)
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
   friend bool operator<=(const TFloatZonotope<USizeMantissaArgument, USizeExponentArgument, TypeImplementationArgument>& fst, const thisType& snd)
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
   friend bool operator==(const TFloatZonotope<USizeMantissaArgument, USizeExponentArgument, TypeImplementationArgument>& fst, const thisType& snd)
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
   friend bool operator!=(const TFloatZonotope<USizeMantissaArgument, USizeExponentArgument, TypeImplementationArgument>& fst, const thisType& snd)
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
   friend bool operator>=(const TFloatZonotope<USizeMantissaArgument, USizeExponentArgument, TypeImplementationArgument>& fst, const thisType& snd)
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
   friend bool operator>(const TFloatZonotope<USizeMantissaArgument, USizeExponentArgument, TypeImplementationArgument>& fst, const thisType& snd)
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
   friend thisType operator+(const TFloatZonotope<USizeMantissaArgument, USizeExponentArgument, TypeImplementationArgument>& fst, const thisType& snd)
      {  thisType fstConvert(fst); fstConvert += snd; return fstConvert; }
   template <int USizeMantissaArgument, int USizeExponentArgument, typename TypeImplementationArgument>
   friend thisType operator+(const TFloatZonotope<USizeMantissaArgument, USizeExponentArgument, TypeImplementationArgument>& fst, thisType&& snd)
      {  thisType fstConvert(fst); fstConvert += std::forward<thisType>(snd); return fstConvert; }
   template <int USizeMantissaArgument, int USizeExponentArgument, typename TypeImplementationArgument>
   friend thisType operator+(TFloatZonotope<USizeMantissaArgument, USizeExponentArgument, TypeImplementationArgument>&& fst, const thisType& snd)
      {  thisType fstConvert(std::forward<TFloatZonotope<USizeMantissaArgument, USizeExponentArgument, TypeImplementationArgument> >(fst)); fstConvert += snd; return fstConvert; }
   template <int USizeMantissaArgument, int USizeExponentArgument, typename TypeImplementationArgument>
   friend thisType operator+(TFloatZonotope<USizeMantissaArgument, USizeExponentArgument, TypeImplementationArgument>&& fst, thisType&& snd)
      {  thisType fstConvert(std::forward<TFloatZonotope<USizeMantissaArgument, USizeExponentArgument, TypeImplementationArgument> >(fst)); fstConvert += std::forward<thisType>(snd); return fstConvert; }
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
   friend thisType operator+(long double fst, thisType&& snd)
      {  thisType fstConvert(fst); fstConvert += std::forward<thisType>(snd); return fstConvert; }
   friend thisType operator+(double fst, thisType&& snd)
      {  thisType fstConvert(fst); fstConvert += std::forward<thisType>(snd); return fstConvert; }
   friend thisType operator+(float fst, thisType&& snd)
      {  thisType fstConvert(fst); fstConvert += std::forward<thisType>(snd); return fstConvert; }
   friend thisType operator+(unsigned long fst, thisType&& snd)
      {  thisType fstConvert(fst); fstConvert += std::forward<thisType>(snd); return fstConvert; }
   friend thisType operator+(long fst, thisType&& snd)
      {  thisType fstConvert(fst); fstConvert += std::forward<thisType>(snd); return fstConvert; }
   friend thisType operator+(unsigned int fst, thisType&& snd)
      {  thisType fstConvert(fst); fstConvert += std::forward<thisType>(snd); return fstConvert; }
   friend thisType operator+(int fst, thisType&& snd)
      {  thisType fstConvert(fst); fstConvert += std::forward<thisType>(snd); return fstConvert; }
   friend thisType operator+(unsigned short fst, thisType&& snd)
      {  thisType fstConvert(fst); fstConvert += std::forward<thisType>(snd); return fstConvert; }
   friend thisType operator+(short fst, thisType&& snd)
      {  thisType fstConvert(fst); fstConvert += std::forward<thisType>(snd); return fstConvert; }
   template <int USizeMantissaArgument, int USizeExponentArgument, typename TypeImplementationArgument>
   friend thisType operator-(const TFloatZonotope<USizeMantissaArgument, USizeExponentArgument, TypeImplementationArgument>& fst, const thisType& snd)
      {  thisType fstConvert(fst); fstConvert -= snd; return fstConvert; }
   template <int USizeMantissaArgument, int USizeExponentArgument, typename TypeImplementationArgument>
   friend thisType operator-(const TFloatZonotope<USizeMantissaArgument, USizeExponentArgument, TypeImplementationArgument>& fst, thisType&& snd)
      {  thisType fstConvert(fst); fstConvert -= std::forward<thisType>(snd); return fstConvert; }
   template <int USizeMantissaArgument, int USizeExponentArgument, typename TypeImplementationArgument>
   friend thisType operator-(TFloatZonotope<USizeMantissaArgument, USizeExponentArgument, TypeImplementationArgument>&& fst, const thisType& snd)
      {  thisType fstConvert(std::forward<TFloatZonotope<USizeMantissaArgument, USizeExponentArgument, TypeImplementationArgument> >(fst)); fstConvert -= snd; return fstConvert; }
   template <int USizeMantissaArgument, int USizeExponentArgument, typename TypeImplementationArgument>
   friend thisType operator-(TFloatZonotope<USizeMantissaArgument, USizeExponentArgument, TypeImplementationArgument>&& fst, thisType&& snd)
      {  thisType fstConvert(std::forward<TFloatZonotope<USizeMantissaArgument, USizeExponentArgument, TypeImplementationArgument> >(fst)); fstConvert -= std::forward<thisType>(snd); return fstConvert; }
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
   friend thisType operator-(long double fst, thisType&& snd)
      {  thisType fstConvert(fst); fstConvert -= std::forward<thisType>(snd); return fstConvert; }
   friend thisType operator-(double fst, thisType&& snd)
      {  thisType fstConvert(fst); fstConvert -= std::forward<thisType>(snd); return fstConvert; }
   friend thisType operator-(float fst, thisType&& snd)
      {  thisType fstConvert(fst); fstConvert -= std::forward<thisType>(snd); return fstConvert; }
   friend thisType operator-(unsigned long fst, thisType&& snd)
      {  thisType fstConvert(fst); fstConvert -= std::forward<thisType>(snd); return fstConvert; }
   friend thisType operator-(long fst, thisType&& snd)
      {  thisType fstConvert(fst); fstConvert -= std::forward<thisType>(snd); return fstConvert; }
   friend thisType operator-(unsigned int fst, thisType&& snd)
      {  thisType fstConvert(fst); fstConvert -= std::forward<thisType>(snd); return fstConvert; }
   friend thisType operator-(int fst, thisType&& snd)
      {  thisType fstConvert(fst); fstConvert -= std::forward<thisType>(snd); return fstConvert; }
   friend thisType operator-(unsigned short fst, thisType&& snd)
      {  thisType fstConvert(fst); fstConvert -= std::forward<thisType>(snd); return fstConvert; }
   friend thisType operator-(short fst, thisType&& snd)
      {  thisType fstConvert(fst); fstConvert -= std::forward<thisType>(snd); return fstConvert; }
   template <int USizeMantissaArgument, int USizeExponentArgument, typename TypeImplementationArgument>
   friend thisType operator*(const TFloatZonotope<USizeMantissaArgument, USizeExponentArgument, TypeImplementationArgument>& fst, const thisType& snd)
      {  thisType fstConvert(fst); fstConvert *= snd; return fstConvert; }
   template <int USizeMantissaArgument, int USizeExponentArgument, typename TypeImplementationArgument>
   friend thisType operator*(const TFloatZonotope<USizeMantissaArgument, USizeExponentArgument, TypeImplementationArgument>& fst, thisType&& snd)
      {  thisType fstConvert(fst); fstConvert *= std::forward<thisType>(snd); return fstConvert; }
   template <int USizeMantissaArgument, int USizeExponentArgument, typename TypeImplementationArgument>
   friend thisType operator*(TFloatZonotope<USizeMantissaArgument, USizeExponentArgument, TypeImplementationArgument>&& fst, const thisType& snd)
      {  thisType fstConvert(std::forward<TFloatZonotope<USizeMantissaArgument, USizeExponentArgument, TypeImplementationArgument> >(fst)); fstConvert *= snd; return fstConvert; }
   template <int USizeMantissaArgument, int USizeExponentArgument, typename TypeImplementationArgument>
   friend thisType operator*(TFloatZonotope<USizeMantissaArgument, USizeExponentArgument, TypeImplementationArgument>&& fst, thisType&& snd)
      {  thisType fstConvert(std::forward<TFloatZonotope<USizeMantissaArgument, USizeExponentArgument, TypeImplementationArgument> >(fst)); fstConvert *= std::forward<thisType>(snd); return fstConvert; }
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
   friend thisType operator*(long double fst, thisType&& snd)
      {  thisType fstConvert(fst); fstConvert *= std::forward<thisType>(snd); return fstConvert; }
   friend thisType operator*(double fst, thisType&& snd)
      {  thisType fstConvert(fst); fstConvert *= std::forward<thisType>(snd); return fstConvert; }
   friend thisType operator*(float fst, thisType&& snd)
      {  thisType fstConvert(fst); fstConvert *= std::forward<thisType>(snd); return fstConvert; }
   friend thisType operator*(unsigned long fst, thisType&& snd)
      {  thisType fstConvert(fst); fstConvert *= std::forward<thisType>(snd); return fstConvert; }
   friend thisType operator*(long fst, thisType&& snd)
      {  thisType fstConvert(fst); fstConvert *= std::forward<thisType>(snd); return fstConvert; }
   friend thisType operator*(unsigned int fst, thisType&& snd)
      {  thisType fstConvert(fst); fstConvert *= std::forward<thisType>(snd); return fstConvert; }
   friend thisType operator*(int fst, thisType&& snd)
      {  thisType fstConvert(fst); fstConvert *= std::forward<thisType>(snd); return fstConvert; }
   friend thisType operator*(unsigned short fst, thisType&& snd)
      {  thisType fstConvert(fst); fstConvert *= std::forward<thisType>(snd); return fstConvert; }
   friend thisType operator*(short fst, thisType&& snd)
      {  thisType fstConvert(fst); fstConvert *= std::forward<thisType>(snd); return fstConvert; }
   template <int USizeMantissaArgument, int USizeExponentArgument, typename TypeImplementationArgument>
   friend thisType operator/(const TFloatZonotope<USizeMantissaArgument, USizeExponentArgument, TypeImplementationArgument>& fst, const thisType& snd)
      {  thisType fstConvert(fst); fstConvert /= snd; return fstConvert; }
   template <int USizeMantissaArgument, int USizeExponentArgument, typename TypeImplementationArgument>
   friend thisType operator/(const TFloatZonotope<USizeMantissaArgument, USizeExponentArgument, TypeImplementationArgument>& fst, thisType&& snd)
      {  thisType fstConvert(fst); fstConvert /= std::forward<thisType>(snd); return fstConvert; }
   template <int USizeMantissaArgument, int USizeExponentArgument, typename TypeImplementationArgument>
   friend thisType operator/(TFloatZonotope<USizeMantissaArgument, USizeExponentArgument, TypeImplementationArgument>&& fst, const thisType& snd)
      {  thisType fstConvert(std::forward<TFloatZonotope<USizeMantissaArgument, USizeExponentArgument, TypeImplementationArgument> >(fst)); fstConvert /= snd; return fstConvert; }
   template <int USizeMantissaArgument, int USizeExponentArgument, typename TypeImplementationArgument>
   friend thisType operator/(TFloatZonotope<USizeMantissaArgument, USizeExponentArgument, TypeImplementationArgument>&& fst, thisType&& snd)
      {  thisType fstConvert(std::forward<TFloatZonotope<USizeMantissaArgument, USizeExponentArgument, TypeImplementationArgument> >(fst)); fstConvert /= std::forward<thisType>(snd); return fstConvert; }
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
   friend thisType operator/(long double fst, thisType&& snd)
      {  thisType fstConvert(fst); fstConvert /= std::forward<thisType>(snd); return fstConvert; }
   friend thisType operator/(double fst, thisType&& snd)
      {  thisType fstConvert(fst); fstConvert /= std::forward<thisType>(snd); return fstConvert; }
   friend thisType operator/(float fst, thisType&& snd)
      {  thisType fstConvert(fst); fstConvert /= std::forward<thisType>(snd); return fstConvert; }
   friend thisType operator/(unsigned long fst, thisType&& snd)
      {  thisType fstConvert(fst); fstConvert /= std::forward<thisType>(snd); return fstConvert; }
   friend thisType operator/(long fst, thisType&& snd)
      {  thisType fstConvert(fst); fstConvert /= std::forward<thisType>(snd); return fstConvert; }
   friend thisType operator/(unsigned int fst, thisType&& snd)
      {  thisType fstConvert(fst); fstConvert /= std::forward<thisType>(snd); return fstConvert; }
   friend thisType operator/(int fst, thisType&& snd)
      {  thisType fstConvert(fst); fstConvert /= std::forward<thisType>(snd); return fstConvert; }
   friend thisType operator/(unsigned short fst, thisType&& snd)
      {  thisType fstConvert(fst); fstConvert /= std::forward<thisType>(snd); return fstConvert; }
   friend thisType operator/(short fst, thisType&& snd)
      {  thisType fstConvert(fst); fstConvert /= std::forward<thisType>(snd); return fstConvert; }
   friend thisType floor(const thisType& fst)
      {  return thisType(std::forward<thisType>(thisType(fst)).asInt(RMLowest)); }
   friend thisType floor(thisType&& fst)
      {  return thisType(std::forward<thisType>(thisType(fst)).asInt(RMLowest)); }
   friend thisType ceil(const thisType& fst)
      {  return thisType(std::forward<thisType>(thisType(fst)).asInt(RMHighest)); }
   friend thisType ceil(thisType&& fst)
      {  return thisType(std::forward<thisType>(thisType(fst)).asInt(RMHighest)); }
   friend thisType trunc(const thisType& fst)
      {  return thisType(std::forward<thisType>(thisType(fst)).asInt(RMZero)); }
   friend thisType trunc(thisType&& fst)
      {  return thisType(std::forward<thisType>(thisType(fst)).asInt(RMZero)); }
   friend thisType round(const thisType& fst)
      {  return thisType(std::forward<thisType>(thisType(fst)).asInt(RMNearest)); }
   friend thisType round(thisType&& fst)
      {  return thisType(std::forward<thisType>(thisType(fst)).asInt(RMNearest)); }
   friend thisType rintf(const thisType& fst)
      {  return thisType(std::forward<thisType>(thisType(fst)).asInt(RMNearest /* fegetround */)); }
   friend thisType rintf(thisType&& fst)
      {  return thisType(std::forward<thisType>(thisType(fst)).asInt(RMNearest /* fegetround */)); }
   friend thisType rint(const thisType& fst)
      {  return thisType(std::forward<thisType>(thisType(fst)).asInt(RMNearest /* fegetround */)); }
   friend thisType rint(thisType&& fst)
      {  return thisType(std::forward<thisType>(thisType(fst)).asInt(RMNearest /* fegetround */)); }

   friend thisType fabs(const thisType& fst)
      {  thisType result = fst;
         // see float_diagnosis.h FLOAT_SPLIT_ALL FLOAT_MERGE_ALL
         bool oldSupportUnstableInLoop = ExecutionPath::doesSupportUnstableInLoop();
         ExecutionPath::setSupportUnstableInLoop();
         auto* oldPathExplorer = ExecutionPath::getCurrentPathExplorer();
         bool oldDoesFollow = ExecutionPath::doesFollowFlow();
         ExecutionPath::clearFollowFlow();
         auto* oldInputTraceFile = ExecutionPath::inputTraceFile();
         const char* oldSynchronisationFile = ExecutionPath::synchronisationFile();
         int oldSynchronisationLine = ExecutionPath::synchronisationLine();
         bool isCompleteFlow = true;
         PathExplorer pathExplorer(
              ExecutionPath::queryMode(oldPathExplorer));
         ExecutionPath::setCurrentPathExplorer(&pathExplorer);
         auto mergeMemory = MergeMemory() >> result >> end();
         auto saveMemory = SaveMemory() << result << end();
         const char* sourceFile = __FILE__;
         int sourceLine = __LINE__;
         do {
            try {
               BaseFloatAffine::splitBranches(sourceFile, sourceLine);

               if (result < 0)
                  result.oppositeAssign();

               isCompleteFlow = MergeBranches(sourceFile, sourceLine) << result << end();
            } 
            catch (ExecutionPath::anticipated_termination&) {
               isCompleteFlow = false;
               ExecutionPath::clearSynchronizationBranches();
            }
            catch (ExecutionPath::read_error& error) {
               if (const char* message = error.getMessage())
                  std::cerr << "error: " << message << std::endl;
               else
                  std::cerr << "error while reading input file!" << std::endl;
               isCompleteFlow = false;
               ExecutionPath::clearSynchronizationBranches();
            }
            ExecutionPath::setFollowFlow();
         } while ((mergeMemory.setCurrentComplete(isCompleteFlow) << result << end())
               && !(saveMemory.setCurrentResult(pathExplorer.isFinished(ExecutionPath::queryMode(oldPathExplorer))) >> result >> end()));
         ExecutionPath::setFollowFlow(oldDoesFollow, oldInputTraceFile,
               oldSynchronisationFile, oldSynchronisationLine);
         ExecutionPath::setSupportUnstableInLoop(oldSupportUnstableInLoop);
         ExecutionPath::setCurrentPathExplorer(oldPathExplorer);
         if (mergeMemory.isFirst())
            ExecutionPath::throwEmptyBranch(true);
         return result;
      }

   friend thisType fabs(thisType&& fst)
      {  thisType result(std::forward<thisType>(fst));
         // see float_diagnosis.h FLOAT_SPLIT_ALL FLOAT_MERGE_ALL
         bool oldSupportUnstableInLoop = ExecutionPath::doesSupportUnstableInLoop();
         ExecutionPath::setSupportUnstableInLoop();
         auto* oldPathExplorer = ExecutionPath::getCurrentPathExplorer();
         bool oldDoesFollow = ExecutionPath::doesFollowFlow();
         ExecutionPath::clearFollowFlow();
         auto* oldInputTraceFile = ExecutionPath::inputTraceFile();
         const char* oldSynchronisationFile = ExecutionPath::synchronisationFile();
         int oldSynchronisationLine = ExecutionPath::synchronisationLine();
         bool isCompleteFlow = true;
         PathExplorer pathExplorer(
              ExecutionPath::queryMode(oldPathExplorer));
         ExecutionPath::setCurrentPathExplorer(&pathExplorer);
         auto mergeMemory = MergeMemory() >> result >> end();
         auto saveMemory = SaveMemory() << result << end();
         const char* sourceFile = __FILE__;
         int sourceLine = __LINE__;
         do {
            try {
               BaseFloatAffine::splitBranches(sourceFile, sourceLine);

               if (result < 0)
                  result.oppositeAssign();

               isCompleteFlow = MergeBranches(sourceFile, sourceLine) << result << end();
            } 
            catch (ExecutionPath::anticipated_termination&) {
               isCompleteFlow = false;
               ExecutionPath::clearSynchronizationBranches();
            }
            catch (ExecutionPath::read_error& error) {
               if (const char* message = error.getMessage())
                  std::cerr << "error: " << message << std::endl;
               else
                  std::cerr << "error while reading input file!" << std::endl;
               isCompleteFlow = false;
               ExecutionPath::clearSynchronizationBranches();
            }
            ExecutionPath::setFollowFlow();
         } while ((mergeMemory.setCurrentComplete(isCompleteFlow) << result << end())
               && !(saveMemory.setCurrentResult(pathExplorer.isFinished(ExecutionPath::queryMode(oldPathExplorer))) >> result >> end()));
         ExecutionPath::setFollowFlow(oldDoesFollow, oldInputTraceFile,
               oldSynchronisationFile, oldSynchronisationLine);
         ExecutionPath::setSupportUnstableInLoop(oldSupportUnstableInLoop);
         ExecutionPath::setCurrentPathExplorer(oldPathExplorer);
         if (mergeMemory.isFirst())
            ExecutionPath::throwEmptyBranch(true);
         return result;
      }
   friend thisType abs(const thisType& source) { return fabs(source); }
   friend thisType abs(thisType&& source) { return fabs(std::forward<thisType>(source)); }

   friend thisType fmod(const thisType& source, const thisType& value)
      {  thisType divResult(source);
         divResult /= value;
         thisType multResult(divResult.asInt(RMZero));
         multResult *= value;
         multResult -= source;
         multResult.oppositeAssign();
         return multResult;
      }
   template <typename TypeFst>
   friend thisType fmod(TypeFst source, const thisType& value)
      {  thisType fst(source);
         thisType divResult(fst); divResult /= value;
         thisType multResult(divResult.asInt(RMZero));
         multResult *= value; fst -= multResult;
         return fst;
      }
   template <typename TypeSnd>
   friend thisType fmod(const thisType& source, TypeSnd value)
      {  auto divResult(source); thisType snd(value); divResult /= snd;
         thisType multResult(divResult.asInt(RMZero));
         multResult *= snd; multResult -= source; multResult.oppositeAssign();
         return multResult;
      }
   friend int fld_finite(const thisType& source) { return source.sfinite(); }
   friend int fld_isfinite(const thisType& source) { return source.sisfinite(); }
   friend int fld_isnan(const thisType& source) { return source.sisnan(); }
   friend int fld_isinf(const thisType& source) { return source.sisinf(); }
};

} // end of namespace DAffineInterface

typedef DAffineInterface::TFloatZonotope<23, 8, float> FloatZonotope;
typedef DAffineInterface::TFloatZonotope<52, 11, double> DoubleZonotope;
typedef DAffineInterface::TFloatZonotope<LDBL_MANT_DIG-1,
      (LDBL_MAX_EXP == (1 << (16-2))) ? 15 /* leading 1 bit */
                                      : sizeof(long double)*8-LDBL_MANT_DIG,
      long double> LongDoubleZonotope;

} // end of namespace NumericalDomains

#endif // FloatInstrumentation_FloatAffineH

