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

template <typename T1, class TypeSaveMemory>
class TSaveMemory {
  public:
   T1 save;
   TypeSaveMemory next;

   TSaveMemory(T1& saveArg, TypeSaveMemory nextArg)
      :  save(saveArg, typename T1::Record()), next(nextArg) {}
   TSaveMemory(const TSaveMemory<T1, TypeSaveMemory>& source)
      :  save(const_cast<T1&>(source.save), typename T1::Record()), next(source.next) {}
   TSaveMemory(TSaveMemory<T1, TypeSaveMemory>&& source)
      :  save(const_cast<T1&>(source.save), typename T1::Record()), next(source.next) {}

   template <typename T>
   TSaveMemory<T, TSaveMemory<T1, TypeSaveMemory> > operator<<(T& t)
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
   TSaveMemory<T, SaveMemory> operator<<(T& t)
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
   TypeMergeMemory& operator>>(const T1& aval)
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

class MergeMemory {
  private:
   bool fFirst;
   bool fComplete;

  public:
   MergeMemory() : fFirst(true), fComplete(false) {}

   template <typename T>
   TMergeMemory<T, MergeMemory> operator>>(T& t)
      {  return TMergeMemory<T, MergeMemory>(t, *this); }
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
   static void setSupportUnstableInLoop();
   static void setSupportVerbose();
   static void setSupportThreshold();
   static void setSupportFirstFollowFloat();
   static void initializeGlobals(const char* fileSuffix);
   static void finalizeGlobals();
   static void setSimplificationTriggerPercent(double percent);
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
class TFloatZonotope;

class MergeBranches {
  private:
   typedef void* /* size_t */ AlignType;
   static const int UMergeBranchesSizeInBytes = NumericalDomains::DAffineInterface::MergeBranchesSize;
   static const int UMergeBranchesSize = (UMergeBranchesSizeInBytes + sizeof(AlignType)-1)/(sizeof(AlignType) - 1);
   AlignType content[UMergeBranchesSize];

  public:
   MergeBranches(const char* file, int line);

   template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
   MergeBranches& operator<<(TFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>& value);
   bool operator<<(end);
};

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
   friend class MergeBranches;

  public:
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
   TFloatZonotope(int value);
   TFloatZonotope(long int value);
   // TFloatZonotope(unsigned value);
   // TFloatZonotope(unsigned long value);
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
   void readImplementation(std::istream& in);
   void writeImplementation(std::ostream& out) const;

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
};

} // end of namespace DAffineInterface

typedef DAffineInterface::TFloatZonotope<23, 8, float> FloatZonotope;
typedef DAffineInterface::TFloatZonotope<52, 11, double> DoubleZonotope;
typedef DAffineInterface::TFloatZonotope<80, 15, long double> LongDoubleZonotope;

} // end of namespace NumericalDomains

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
inline NumericalDomains::DAffineInterface::TFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>
sqrt(const NumericalDomains::DAffineInterface::TFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>& source) {
   NumericalDomains::DAffineInterface::TFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation> result(source);
   result.sqrtAssign();
   return result;
}

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
inline NumericalDomains::DAffineInterface::TFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>
sqrt(NumericalDomains::DAffineInterface::TFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>&& source) {
   typedef NumericalDomains::DAffineInterface::TFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation> thisType;
   thisType result(std::forward<thisType>(source));
   result.sqrtAssign();
   return result;
}

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
inline NumericalDomains::DAffineInterface::TFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>
sin(const NumericalDomains::DAffineInterface::TFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>& source) {
   NumericalDomains::DAffineInterface::TFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation> result(source);
   result.sinAssign();
   return result;
}

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
inline NumericalDomains::DAffineInterface::TFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>
sin(NumericalDomains::DAffineInterface::TFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>&& source) {
   typedef NumericalDomains::DAffineInterface::TFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation> thisType;
   thisType result(std::forward<thisType>(source));
   result.sinAssign();
   return result;
}

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
inline NumericalDomains::DAffineInterface::TFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>
cos(const NumericalDomains::DAffineInterface::TFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>& source) {
   NumericalDomains::DAffineInterface::TFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation> result(source);
   result.cosAssign();
   return result;
}

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
inline NumericalDomains::DAffineInterface::TFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>
cos(NumericalDomains::DAffineInterface::TFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>&& source) {
   typedef NumericalDomains::DAffineInterface::TFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation> thisType;
   thisType result(std::forward<thisType>(source));
   result.cosAssign();
   return result;
}

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
inline NumericalDomains::DAffineInterface::TFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>
asin(const NumericalDomains::DAffineInterface::TFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>& source) {
   NumericalDomains::DAffineInterface::TFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation> result(source);
   result.asinAssign();
   return result;
}

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
inline NumericalDomains::DAffineInterface::TFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>
asin(NumericalDomains::DAffineInterface::TFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>&& source) {
   typedef NumericalDomains::DAffineInterface::TFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation> thisType;
   thisType result(std::forward<thisType>(source));
   result.asinAssign();
   return result;
}

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
inline NumericalDomains::DAffineInterface::TFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>
acos(const NumericalDomains::DAffineInterface::TFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>& source) {
   NumericalDomains::DAffineInterface::TFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation> result(source);
   result.acosAssign();
   return result;
}

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
inline NumericalDomains::DAffineInterface::TFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>
acos(NumericalDomains::DAffineInterface::TFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>&& source) {
   typedef NumericalDomains::DAffineInterface::TFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation> thisType;
   thisType result(std::forward<thisType>(source));
   result.acosAssign();
   return result;
}

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
inline NumericalDomains::DAffineInterface::TFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>
tan(const NumericalDomains::DAffineInterface::TFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>& source) {
   NumericalDomains::DAffineInterface::TFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation> result(source);
   result.tanAssign();
   return result;
}

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
inline NumericalDomains::DAffineInterface::TFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>
tan(NumericalDomains::DAffineInterface::TFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>&& source) {
   typedef NumericalDomains::DAffineInterface::TFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation> thisType;
   thisType result(std::forward<thisType>(source));
   result.tanAssign();
   return result;
}

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
inline NumericalDomains::DAffineInterface::TFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>
atan(const NumericalDomains::DAffineInterface::TFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>& source) {
   NumericalDomains::DAffineInterface::TFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation> result(source);
   result.atanAssign();
   return result;
}

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
inline NumericalDomains::DAffineInterface::TFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>
atan(NumericalDomains::DAffineInterface::TFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>&& source) {
   typedef NumericalDomains::DAffineInterface::TFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation> thisType;
   thisType result(std::forward<thisType>(source));
   result.atanAssign();
   return result;
}

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
inline NumericalDomains::DAffineInterface::TFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>
exp(const NumericalDomains::DAffineInterface::TFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>& source) {
   NumericalDomains::DAffineInterface::TFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation> result(source);
   result.expAssign();
   return result;
}

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
inline NumericalDomains::DAffineInterface::TFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>
exp(NumericalDomains::DAffineInterface::TFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>&& source) {
   typedef NumericalDomains::DAffineInterface::TFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation> thisType;
   thisType result(std::forward<thisType>(source));
   result.expAssign();
   return result;
}

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
inline NumericalDomains::DAffineInterface::TFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>
log(const NumericalDomains::DAffineInterface::TFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>& source) {
   NumericalDomains::DAffineInterface::TFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation> result(source);
   result.logAssign();
   return result;
}

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
inline NumericalDomains::DAffineInterface::TFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>
log(NumericalDomains::DAffineInterface::TFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>&& source) {
   typedef NumericalDomains::DAffineInterface::TFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation> thisType;
   thisType result(std::forward<thisType>(source));
   result.logAssign();
   return result;
}

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
inline NumericalDomains::DAffineInterface::TFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>
log10(const NumericalDomains::DAffineInterface::TFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>& source) {
   NumericalDomains::DAffineInterface::TFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation> result(source);
   result.log10Assign();
   return result;
}

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
inline NumericalDomains::DAffineInterface::TFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>
log10(NumericalDomains::DAffineInterface::TFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>&& source) {
   typedef NumericalDomains::DAffineInterface::TFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation> thisType;
   thisType result(std::forward<thisType>(source));
   result.log10Assign();
   return result;
}

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
inline NumericalDomains::DAffineInterface::TFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>
pow(const NumericalDomains::DAffineInterface::TFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>& source,
      const NumericalDomains::DAffineInterface::TFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>& value) {
   NumericalDomains::DAffineInterface::TFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation> result(source);
   result.powAssign(value);
   return result;
}

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
inline NumericalDomains::DAffineInterface::TFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>
pow(const NumericalDomains::DAffineInterface::TFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>& source,
      NumericalDomains::DAffineInterface::TFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>&& value) {
   typedef NumericalDomains::DAffineInterface::TFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation> thisType;
   thisType result(source);
   result.powAssign(std::forward<thisType>(value));
   return result;
}

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
inline NumericalDomains::DAffineInterface::TFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>
pow(NumericalDomains::DAffineInterface::TFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>&& source,
      const NumericalDomains::DAffineInterface::TFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>& value) {
   typedef NumericalDomains::DAffineInterface::TFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation> thisType;
   thisType result(std::forward<thisType>(source));
   result.powAssign(value);
   return result;
}

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
inline NumericalDomains::DAffineInterface::TFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>
pow(NumericalDomains::DAffineInterface::TFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>&& source,
      NumericalDomains::DAffineInterface::TFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>&& value) {
   typedef NumericalDomains::DAffineInterface::TFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation> thisType;
   thisType result(std::forward<thisType>(source));
   result.powAssign(std::forward<thisType>(value));
   return result;
}

template <typename TypeFst, int USizeMantissa, int USizeExponent, typename TypeImplementation>
inline NumericalDomains::DAffineInterface::TFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>
pow(TypeFst source, const NumericalDomains::DAffineInterface::TFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>& value) {
   NumericalDomains::DAffineInterface::TFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation> result(source);
   result.powAssign(value);
   return result;
}

template <typename TypeFst, int USizeMantissa, int USizeExponent, typename TypeImplementation>
inline NumericalDomains::DAffineInterface::TFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>
pow(TypeFst source, NumericalDomains::DAffineInterface::TFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>&& value) {
   typedef NumericalDomains::DAffineInterface::TFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation> thisType;
   thisType result(source);
   result.powAssign(std::forward<thisType>(value));
   return result;
}

template <typename TypeSnd, int USizeMantissa, int USizeExponent, typename TypeImplementation>
inline NumericalDomains::DAffineInterface::TFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>
pow(const NumericalDomains::DAffineInterface::TFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>& source,
      TypeSnd value) {
   typedef NumericalDomains::DAffineInterface::TFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation> thisType;
   thisType result(source);
   result.powAssign(thisType(value));
   return result;
}

template <typename TypeSnd, int USizeMantissa, int USizeExponent, typename TypeImplementation>
inline NumericalDomains::DAffineInterface::TFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>
pow(NumericalDomains::DAffineInterface::TFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>&& source,
      TypeSnd value) {
   typedef NumericalDomains::DAffineInterface::TFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation> thisType;
   thisType result(std::forward<thisType>(source));
   result.powAssign(thisType(value));
   return result;
}

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
inline NumericalDomains::DAffineInterface::TFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>
powf(const NumericalDomains::DAffineInterface::TFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>& source,
      const NumericalDomains::DAffineInterface::TFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>& value) {
   NumericalDomains::DAffineInterface::TFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation> result(source);
   result.powAssign(value);
   return result;
}

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
inline NumericalDomains::DAffineInterface::TFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>
powf(const NumericalDomains::DAffineInterface::TFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>& source,
      NumericalDomains::DAffineInterface::TFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>&& value) {
   typedef NumericalDomains::DAffineInterface::TFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation> thisType;
   thisType result(source);
   result.powAssign(std::forward<thisType>(value));
   return result;
}

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
inline NumericalDomains::DAffineInterface::TFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>
powf(NumericalDomains::DAffineInterface::TFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>&& source,
      const NumericalDomains::DAffineInterface::TFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>& value) {
   typedef NumericalDomains::DAffineInterface::TFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation> thisType;
   thisType result(std::forward<thisType>(source));
   result.powAssign(value);
   return result;
}

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
inline NumericalDomains::DAffineInterface::TFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>
powf(NumericalDomains::DAffineInterface::TFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>&& source,
      NumericalDomains::DAffineInterface::TFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>&& value) {
   typedef NumericalDomains::DAffineInterface::TFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation> thisType;
   thisType result(std::forward<thisType>(source));
   result.powAssign(std::forward<thisType>(value));
   return result;
}

template <typename TypeFst, int USizeMantissa, int USizeExponent, typename TypeImplementation>
inline NumericalDomains::DAffineInterface::TFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>
powf(TypeFst source, const NumericalDomains::DAffineInterface::TFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>& value) {
   NumericalDomains::DAffineInterface::TFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation> result(source);
   result.powAssign(value);
   return result;
}

template <typename TypeFst, int USizeMantissa, int USizeExponent, typename TypeImplementation>
inline NumericalDomains::DAffineInterface::TFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>
powf(TypeFst source, NumericalDomains::DAffineInterface::TFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>&& value) {
   typedef NumericalDomains::DAffineInterface::TFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation> thisType;
   thisType result(source);
   result.powAssign(std::forward<thisType>(value));
   return result;
}

template <typename TypeSnd, int USizeMantissa, int USizeExponent, typename TypeImplementation>
inline NumericalDomains::DAffineInterface::TFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>
powf(const NumericalDomains::DAffineInterface::TFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>& source,
      TypeSnd value) {
   typedef NumericalDomains::DAffineInterface::TFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation> thisType;
   thisType result(source);
   result.powAssign(thisType(value));
   return result;
}

template <typename TypeSnd, int USizeMantissa, int USizeExponent, typename TypeImplementation>
inline NumericalDomains::DAffineInterface::TFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>
powf(NumericalDomains::DAffineInterface::TFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>&& source,
      TypeSnd value) {
   typedef NumericalDomains::DAffineInterface::TFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation> thisType;
   thisType result(std::forward<thisType>(source));
   result.powAssign(thisType(value));
   return result;
}

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
inline NumericalDomains::DAffineInterface::TFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>
atan2(const NumericalDomains::DAffineInterface::TFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>& source,
      const NumericalDomains::DAffineInterface::TFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>& value) {
   NumericalDomains::DAffineInterface::TFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation> result(source);
   result.atan2Assign(value);
   return result;
}

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
inline NumericalDomains::DAffineInterface::TFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>
atan2(const NumericalDomains::DAffineInterface::TFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>& source,
      NumericalDomains::DAffineInterface::TFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>&& value) {
   typedef NumericalDomains::DAffineInterface::TFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation> thisType;
   thisType result(source);
   result.atan2Assign(std::forward<thisType>(value));
   return result;
}

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
inline NumericalDomains::DAffineInterface::TFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>
atan2(NumericalDomains::DAffineInterface::TFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>&& source,
      const NumericalDomains::DAffineInterface::TFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>& value) {
   typedef NumericalDomains::DAffineInterface::TFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation> thisType;
   thisType result(std::forward<thisType>(source));
   result.atan2Assign(value);
   return result;
}

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
inline NumericalDomains::DAffineInterface::TFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>
atan2(NumericalDomains::DAffineInterface::TFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>&& source,
      NumericalDomains::DAffineInterface::TFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>&& value) {
   typedef NumericalDomains::DAffineInterface::TFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation> thisType;
   thisType result(std::forward<thisType>(source));
   result.atan2Assign(std::forward<thisType>(value));
   return result;
}

template <typename TypeFst, int USizeMantissa, int USizeExponent, typename TypeImplementation>
inline NumericalDomains::DAffineInterface::TFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>
atan2(TypeFst source, const NumericalDomains::DAffineInterface::TFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>& value) {
   NumericalDomains::DAffineInterface::TFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation> result(source);
   result.atan2Assign(value);
   return result;
}

template <typename TypeFst, int USizeMantissa, int USizeExponent, typename TypeImplementation>
inline NumericalDomains::DAffineInterface::TFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>
atan2(TypeFst source, NumericalDomains::DAffineInterface::TFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>&& value) {
   typedef NumericalDomains::DAffineInterface::TFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation> thisType;
   thisType result(source);
   result.atan2Assign(std::forward<thisType>(value));
   return result;
}

template <typename TypeSnd, int USizeMantissa, int USizeExponent, typename TypeImplementation>
inline NumericalDomains::DAffineInterface::TFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>
atan2(const NumericalDomains::DAffineInterface::TFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>& source,
      TypeSnd value) {
   typedef NumericalDomains::DAffineInterface::TFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation> thisType;
   thisType result(source);
   result.atan2Assign(thisType(value));
   return result;
}

template <typename TypeSnd, int USizeMantissa, int USizeExponent, typename TypeImplementation>
inline NumericalDomains::DAffineInterface::TFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>
atan2(NumericalDomains::DAffineInterface::TFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>&& source,
      TypeSnd value) {
   typedef NumericalDomains::DAffineInterface::TFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation> thisType;
   thisType result(std::forward<thisType>(source));
   result.atan2Assign(thisType(value));
   return result;
}

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
inline NumericalDomains::DAffineInterface::TFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>
log2(const NumericalDomains::DAffineInterface::TFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>& source) {
   typedef NumericalDomains::DAffineInterface::TFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation> thisType;
   thisType result(source);
   result.logAssign();
   result.divAssign(thisType(::log(2.0)));
   return result;
}

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
inline NumericalDomains::DAffineInterface::TFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>
log2(NumericalDomains::DAffineInterface::TFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>&& source) {
   typedef NumericalDomains::DAffineInterface::TFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation> thisType;
   thisType result(std::forward<thisType>(source));
   result.logAssign();
   result.divAssign(thisType(::log(2.0)));
   return result;
}

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
inline NumericalDomains::DAffineInterface::TFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>
exp2(const NumericalDomains::DAffineInterface::TFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>& source) {
   NumericalDomains::DAffineInterface::TFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>
      result = 2.0;
   result.powAssign(source);
   return result;
}

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
inline NumericalDomains::DAffineInterface::TFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>
exp2(NumericalDomains::DAffineInterface::TFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>&& source) {
   typedef NumericalDomains::DAffineInterface::TFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation> thisType;
   thisType result = 2.0;
   result.powAssign(std::forward<thisType>(source));
   return result;
}

template <typename TypeFst, int USizeMantissa, int USizeExponent, typename TypeImplementation>
inline bool
operator<(TypeFst fst, const NumericalDomains::DAffineInterface::TFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>& snd)
   {  return NumericalDomains::DAffineInterface::TFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>(fst).operator<(snd); }

template <typename TypeFst, int USizeMantissa, int USizeExponent, typename TypeImplementation>
inline bool
operator<=(TypeFst fst, const NumericalDomains::DAffineInterface::TFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>& snd)
   {  return NumericalDomains::DAffineInterface::TFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>(fst).operator<=(snd); }

template <typename TypeFst, int USizeMantissa, int USizeExponent, typename TypeImplementation>
inline bool
operator==(TypeFst fst, const NumericalDomains::DAffineInterface::TFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>& snd)
   {  return NumericalDomains::DAffineInterface::TFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>(fst).operator==(snd); }

template <typename TypeFst, int USizeMantissa, int USizeExponent, typename TypeImplementation>
inline bool
operator!=(TypeFst fst, const NumericalDomains::DAffineInterface::TFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>& snd)
   {  return NumericalDomains::DAffineInterface::TFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>(fst).operator!=(snd); }

template <typename TypeFst, int USizeMantissa, int USizeExponent, typename TypeImplementation>
inline bool
operator>=(TypeFst fst, const NumericalDomains::DAffineInterface::TFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>& snd)
   {  return NumericalDomains::DAffineInterface::TFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>(fst).operator>=(snd); }

template <typename TypeFst, int USizeMantissa, int USizeExponent, typename TypeImplementation>
inline bool
operator>(TypeFst fst, const NumericalDomains::DAffineInterface::TFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>& snd)
   {  return NumericalDomains::DAffineInterface::TFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>(fst).operator>(snd); }

template <typename TypeFst, int USizeMantissa, int USizeExponent, typename TypeImplementation>
inline NumericalDomains::DAffineInterface::TFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>
operator+(TypeFst fst, const NumericalDomains::DAffineInterface::TFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>& snd)
{  typedef NumericalDomains::DAffineInterface::TFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation> thisType;
   thisType fstConvert(fst);
   fstConvert += snd;
   return fstConvert;
}

template <typename TypeFst, int USizeMantissa, int USizeExponent, typename TypeImplementation>
inline NumericalDomains::DAffineInterface::TFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>
operator+(TypeFst fst, NumericalDomains::DAffineInterface::TFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>&& snd)
{  typedef NumericalDomains::DAffineInterface::TFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation> thisType;
   thisType fstConvert(fst);
   fstConvert += std::forward<thisType>(snd);
   return fstConvert;
}

template <typename TypeFst, int USizeMantissa, int USizeExponent, typename TypeImplementation>
inline NumericalDomains::DAffineInterface::TFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>
operator-(TypeFst fst, const NumericalDomains::DAffineInterface::TFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>& snd)
{  typedef NumericalDomains::DAffineInterface::TFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation> thisType;
   thisType fstConvert(fst);
   fstConvert -= snd;
   return fstConvert;
}

template <typename TypeFst, int USizeMantissa, int USizeExponent, typename TypeImplementation>
inline NumericalDomains::DAffineInterface::TFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>
operator-(TypeFst fst, NumericalDomains::DAffineInterface::TFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>&& snd)
{  typedef NumericalDomains::DAffineInterface::TFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation> thisType;
   thisType fstConvert(fst);
   fstConvert -= std::forward<thisType>(snd);
   return fstConvert;
}

template <typename TypeFst, int USizeMantissa, int USizeExponent, typename TypeImplementation>
inline NumericalDomains::DAffineInterface::TFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>
operator*(TypeFst fst, const NumericalDomains::DAffineInterface::TFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>& snd)
{  typedef NumericalDomains::DAffineInterface::TFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation> thisType;
   thisType fstConvert(fst);
   fstConvert *= snd;
   return fstConvert;
}

template <typename TypeFst, int USizeMantissa, int USizeExponent, typename TypeImplementation>
inline NumericalDomains::DAffineInterface::TFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>
operator*(TypeFst fst, NumericalDomains::DAffineInterface::TFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>&& snd)
{  typedef NumericalDomains::DAffineInterface::TFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation> thisType;
   thisType fstConvert(fst);
   fstConvert *= std::forward<thisType>(snd);
   return fstConvert;
}

template <typename TypeFst, int USizeMantissa, int USizeExponent, typename TypeImplementation>
inline NumericalDomains::DAffineInterface::TFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>
operator/(TypeFst fst, const NumericalDomains::DAffineInterface::TFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>& snd)
{  typedef NumericalDomains::DAffineInterface::TFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation> thisType;
   thisType fstConvert(fst);
   fstConvert /= snd;
   return fstConvert;
}

template <typename TypeFst, int USizeMantissa, int USizeExponent, typename TypeImplementation>
inline NumericalDomains::DAffineInterface::TFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>
operator/(TypeFst fst, NumericalDomains::DAffineInterface::TFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>&& snd)
{  typedef NumericalDomains::DAffineInterface::TFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation> thisType;
   thisType fstConvert(fst);
   fstConvert /= std::forward<thisType>(snd);
   return fstConvert;
}

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
inline NumericalDomains::DAffineInterface::TFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>
floor(const NumericalDomains::DAffineInterface::TFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>& fst)
{  typedef NumericalDomains::DAffineInterface::TFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation> thisType;
   return thisType(std::forward<thisType>(thisType(fst)).operator int());
}

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
inline NumericalDomains::DAffineInterface::TFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>
floor(NumericalDomains::DAffineInterface::TFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>&& fst)
{  typedef NumericalDomains::DAffineInterface::TFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation> thisType;
   return thisType(std::forward<thisType>(thisType(fst)).operator int());
}

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
inline int
rintf(const NumericalDomains::DAffineInterface::TFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>& fst)
{  typedef NumericalDomains::DAffineInterface::TFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation> thisType;
   return std::forward<thisType>(thisType(fst)).operator int();
}

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
inline int
rintf(NumericalDomains::DAffineInterface::TFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>&& fst)
{  typedef NumericalDomains::DAffineInterface::TFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation> thisType;
   return std::forward<thisType>(thisType(fst)).operator int();
}

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
inline int
rint(const NumericalDomains::DAffineInterface::TFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>& fst)
{  typedef NumericalDomains::DAffineInterface::TFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation> thisType;
   return std::forward<thisType>(thisType(fst)).operator int();
}

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
inline int
rint(NumericalDomains::DAffineInterface::TFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>&& fst)
{  typedef NumericalDomains::DAffineInterface::TFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation> thisType;
   return std::forward<thisType>(thisType(fst)).operator int();
}

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
inline NumericalDomains::DAffineInterface::TFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>
fabs(const NumericalDomains::DAffineInterface::TFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>& fst)
{  typedef NumericalDomains::DAffineInterface::TFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation> thisType;
   thisType result = fst;
   if (result < 0)
      result.oppositeAssign();
   return result;
}

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
inline NumericalDomains::DAffineInterface::TFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>
fabs(NumericalDomains::DAffineInterface::TFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation>&& fst)
{  typedef NumericalDomains::DAffineInterface::TFloatZonotope<USizeMantissa, USizeExponent, TypeImplementation> thisType;
   thisType result(std::forward<thisType>(fst));
   if (result < 0)
      result.oppositeAssign();
   return result;
}

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
inline std::ostream&
operator<<(std::ostream& out, const NumericalDomains::DAffineInterface::TFloatZonotope
      <USizeMantissa, USizeExponent, TypeImplementation>& value) {
   value.writeImplementation(out);
   return out;
}

template <int USizeMantissa, int USizeExponent, typename TypeImplementation>
inline std::istream&
operator>>(std::istream& in, NumericalDomains::DAffineInterface::TFloatZonotope
      <USizeMantissa, USizeExponent, TypeImplementation>& value) {
   value.readImplementation(in);
   return in;
}

#endif // FloatInstrumentation_FloatAffineH

