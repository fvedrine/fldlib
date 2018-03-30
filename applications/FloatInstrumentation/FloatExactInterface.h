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
#include <vector>

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

   TPackedSaveMemory<TypeIterator, TypeSaveMemory>& setCurrent(bool result)
      {  next.setCurrent(result); return *this; }
   
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
   friend class DDoubleExactInterface::MergeBranches;

   enum RoundMode { RMNearest, RMLowest, RMHighest, RMZero };
   int asInt(RoundMode mode) const;

   int sfinite() const;
   int sisfinite() const;
   int sisnan() const;
   int sisinf() const;

  public:
   typedef DDoubleExactInterface::MergeBranches MergeBranches;
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
   TFloatExact(short int value);
   TFloatExact(int value);
   TFloatExact(long int value);
   TFloatExact(unsigned short value);
   TFloatExact(unsigned value);
   TFloatExact(unsigned long value);
   TFloatExact(const thisType& source);
   TFloatExact& operator=(const thisType& source);
   ~TFloatExact();

   template <int USizeMantissaArgument, int USizeExponentArgument, typename TypeImplementationArgument>
   TFloatExact(const TFloatExact<USizeMantissaArgument, USizeExponentArgument, TypeImplementationArgument>& source);
   template <int USizeMantissaArgument, int USizeExponentArgument, typename TypeImplementationArgument>
   thisType& operator=(const TFloatExact<USizeMantissaArgument, USizeExponentArgument, TypeImplementationArgument>& source);

   TypeImplementation asImplementation() const;
   void writeImplementation(std::ostream& out) const;
   void readImplementation(std::istream& in);
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
      {  auto result(std::move(source)); result.powAssign(value); return result; }
   template <typename TypeFst>
   friend thisType pow(TypeFst source, const thisType& value)
      {  thisType result(source); result.powAssign(value); return result; }
   template <typename TypeSnd>
   friend thisType pow(const thisType& source, TypeSnd value)
      {  auto result(std::move(source)); result.powAssign(thisType(value)); return result; }
   friend thisType powf(const thisType& source, const thisType& value)
      {  auto result(std::move(source)); result.powAssign(value); return result; }
   template <typename TypeFst>
   friend thisType powf(TypeFst source, const thisType& value)
      {  thisType result(source); result.powAssign(value); return result; }
   template <typename TypeSnd>
   friend thisType powf(const thisType& source, TypeSnd value)
      {  auto result(std::move(source)); result.powAssign(thisType(value)); return result; }

   friend thisType atan2(const thisType& source, const thisType& value)
      {  auto result(std::move(source)); result.atan2Assign(value); return result; }
   template <typename TypeFst>
   friend thisType atan2(TypeFst source, const thisType& value)
      {  thisType result(source); result.atan2Assign(value); return result; }
   template <typename TypeSnd>
   friend thisType atan2(const thisType& source, TypeSnd value)
      {  auto result(std::move(source)); result.atan2Assign(thisType(value)); return result; }

   template <typename TypeFst>
   friend bool operator<(TypeFst fst, const thisType& snd)
      {  return thisType(fst).operator<(snd); }
   template <typename TypeFst>
   friend bool operator<=(TypeFst fst, const thisType& snd)
      {  return thisType(fst).operator<=(snd); }
   template <typename TypeFst>
   friend bool operator==(TypeFst fst, const thisType& snd)
      {  return thisType(fst).operator==(snd); }
   template <typename TypeFst>
   friend bool operator!=(TypeFst fst, const thisType& snd)
      {  return thisType(fst).operator!=(snd); }
   template <typename TypeFst>
   friend bool operator>=(TypeFst fst, const thisType& snd)
      {  return thisType(fst).operator>=(snd); }
   template <typename TypeFst>
   friend bool operator>(TypeFst fst, const thisType& snd)
      {  return thisType(fst).operator>(snd); }
   template <typename TypeFst>
   friend thisType operator+(TypeFst fst, const thisType& snd)
      {  return thisType(fst).operator+(snd); }
   template <typename TypeFst>
   friend thisType operator-(TypeFst fst, const thisType& snd)
      {  return thisType(fst).operator-(snd); }
   template <typename TypeFst>
   friend thisType operator*(TypeFst fst, const thisType& snd)
      {  return thisType(fst).operator*(snd); }
   template <typename TypeFst>
   friend thisType operator/(TypeFst fst, const thisType& snd)
      {  return thisType(fst).operator/(snd); }
   friend thisType floor(const thisType& fst) { return thisType(fst.asInt(RMLowest)); }
   friend thisType floor(thisType&& fst) { return thisType(fst.asInt(RMLowest)); }
   friend thisType ceil(const thisType& fst) { return thisType(fst.asInt(RMHighest)); }
   friend thisType ceil(thisType&& fst) { return thisType(fst.asInt(RMHighest)); }
   friend thisType trunc(const thisType& fst) { return thisType(fst.asInt(RMZero)); }
   friend thisType trunc(thisType&& fst) { return thisType(fst.asInt(RMZero)); }
   friend thisType round(const thisType& fst) { return thisType(fst.asInt(RMNearest)); } 
   friend thisType round(thisType&& fst) { return thisType(fst.asInt(RMNearest)); } 
   friend thisType rintf(const thisType& fst) { return thisType(fst.asInt(RMNearest /* fegetround */)); } 
   friend thisType rintf(thisType&& fst) { return thisType(fst.asInt(RMNearest /* fegetround */)); } 
   friend thisType rint(const thisType& fst) { return thisType(fst.asInt(RMNearest /* fegetround */)); } 
   friend thisType rint(thisType&& fst) { return thisType(fst.asInt(RMNearest /* fegetround */)); } 
   friend thisType fabs(const thisType& source)
      {  auto result(std::move(source)); result.absAssign(); return result; } 
   friend thisType fabs(thisType&& source)
      {  auto result(std::forward<thisType>(source)); result.absAssign(); return result; }
   friend thisType abs(const thisType& source)
      {  auto result(std::move(source)); result.absAssign(); return result; } 
   friend thisType abs(thisType&& source)
      {  auto result(std::forward<thisType>(source)); result.absAssign(); return result; }

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

} // end of namespace DDoubleExactInterface

typedef DDoubleExactInterface::TFloatExact<23, 8, float> FloatExact;
typedef DDoubleExactInterface::TFloatExact<52, 11, double> DoubleExact;
typedef DDoubleExactInterface::TFloatExact<LDBL_MANT_DIG-1,
      (LDBL_MAX_EXP == (1 << (16-2))) ? 15 /* leading 1 bit */
                                      : sizeof(long double)*8-LDBL_MANT_DIG,
      long double> LongDoubleExact;

} // end of namespace NumericalDomains

#endif // FloatInstrumentation_FloatExactH

