/**************************************************************************/
/*                                                                        */
/*  This file is part of FLDLib                                           */
/*                                                                        */
/*  Copyright (C) 2014-2017                                               */
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
// Library   : ForwardNumerics
// Unit      : Integer
// File      : BaseInteger.h
// Description :
//   Definition of a class of integers with unbound size.
//

#ifndef Numerics_BaseIntegerH
#define Numerics_BaseIntegerH

#include "ForwardNumerics/BaseIntegerCommon.h"

namespace Numerics {

template <class IntegerTraits>
class TBigCellInt;
template <class IntegerTraits>
class TBigCell;
template <class IntegerTraits>
class TBigInt;

namespace DInteger {

class CellIntegerTraitsContract {
  public:
   CellIntegerTraitsContract() {}
   CellIntegerTraitsContract(const CellIntegerTraitsContract& /* source */) { AssumeUncalled }
   CellIntegerTraitsContract& operator=(const CellIntegerTraitsContract& /* source */) { AssumeUncalled return *this; }
   typedef unsigned int& ArrayProperty;
   ArrayProperty array(int /* index */) { AssumeUncalled unsigned* result = nullptr; return *result; }
   unsigned int array(int /* index */) const { AssumeUncalled return 0; }
   unsigned int carray(int /* index */) const { AssumeUncalled return 0; }
   ArrayProperty operator[](int /* index */) { AssumeUncalled unsigned* result = nullptr; return *result; }
   unsigned int operator[](int /* index */) const { AssumeUncalled return 0; }
   typedef CellIntegerTraitsContract MultResult;

   typedef CellIntegerTraitsContract QuotientResult;
   typedef CellIntegerTraitsContract RemainderResult;
   typedef CellIntegerTraitsContract NormalizedRemainderResult;
   void copyLow(const MultResult& /* result */) { AssumeUncalled }
   int getSize() const { AssumeUncalled return 0; }
   void adjustSize(int /* newSize */) { AssumeUncalled }
   void setSize(int /* exactSize */) { AssumeUncalled }
   void setBitSize(int /* exactSize */) { AssumeUncalled }
   void setCellSize(int /* exactSize */) { AssumeUncalled }
   void normalize() { AssumeUncalled }
   void assertSize(int /* newSize */) { AssumeUncalled }
   void clear() { AssumeUncalled }
   typedef CellIntegerTraitsContract ExtendedInteger;
};

template <int UCellSize>
class TBasicCellIntegerTraits : public CellIntegerTraitsContract {
  private:
   unsigned int auArray[UCellSize];
   typedef TBasicCellIntegerTraits<UCellSize> thisType;

  protected:
   unsigned int* _array() { return auArray; }
   const unsigned int* _array() const { return auArray; }

  public:
   TBasicCellIntegerTraits() { memset(auArray, 0, UCellSize*sizeof(unsigned int)); }
   TBasicCellIntegerTraits(const thisType& source) : CellIntegerTraitsContract()
      {  memcpy(auArray, source.auArray, UCellSize*sizeof(unsigned int)); }
   thisType& operator=(const thisType& source)
      {  memcpy(auArray, source.auArray, UCellSize*sizeof(unsigned int));
         return *this;
      }
   thisType& operator=(unsigned int source)
      {  if (UCellSize > 1)
            memset(auArray, 0, UCellSize*sizeof(unsigned int));
         auArray[0] = source;
         return *this;
      }
   typedef unsigned int& ArrayProperty;
#ifdef __GNUC__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Warray-bounds"
#endif
   ArrayProperty array(int index)
      {  AssumeCondition(index >= 0 && (index < UCellSize))
         return auArray[index];
      }
   unsigned int array(int index) const
      {  AssumeCondition(index >= 0)
         return (index < UCellSize) ? auArray[index] : 0U;
      }
   unsigned int carray(int index) const { return array(index); }
   ArrayProperty operator[](int index)
      {  AssumeCondition(index >= 0 && (index < UCellSize))
         return auArray[index];
      }
#ifdef __GNUC__
#pragma GCC diagnostic pop
#endif
   unsigned int operator[](int index) const
      {  AssumeCondition(index >= 0)
         return (index < UCellSize) ? auArray[index] : 0U;
      }

   static int getSize() { return UCellSize; }
   void normalize() {}
   void adjustSize(int /* newSize */) { AssumeUncalled }
   void assertSize(int newSize) { AssumeCondition(newSize <= UCellSize) }
   void setSize(int exactSize) { AssumeCondition(exactSize == UCellSize) }
   void setBitSize(int exactSize) { AssumeCondition((exactSize + 8*sizeof(unsigned)-1)/(8*sizeof(unsigned)) == UCellSize) }
   void setCellSize(int exactSize) { AssumeCondition(exactSize == UCellSize) }
   void clear() { memset(auArray, 0, UCellSize*sizeof(unsigned int)); }
   void swap(thisType& source)
      {  unsigned int temp[UCellSize];
         memcpy(temp, auArray, UCellSize*sizeof(unsigned int));
         memcpy(auArray, source.auArray, UCellSize*sizeof(unsigned int));
         memcpy(source.auArray, temp, UCellSize*sizeof(unsigned int));
      }
   unsigned int* arrayStart() { return auArray; }
   const unsigned int* arrayStart() const { return auArray; }
};

template <>
class TBasicCellIntegerTraits<0> : public CellIntegerTraitsContract {
  private:
   typedef TBasicCellIntegerTraits<0> thisType;

  protected:
   unsigned int* _array() { return nullptr; }
   const unsigned int* _array() const { return nullptr; }

  public:
   TBasicCellIntegerTraits() {}
   TBasicCellIntegerTraits(const thisType& /* source */) : CellIntegerTraitsContract() {}
   thisType& operator=(const thisType& /* source */) { return *this; }
   thisType& operator=(unsigned int /* source */) { return *this; }
   typedef unsigned int& ArrayProperty;
   ArrayProperty array(int /* index */) { AssumeUncalled unsigned* result = nullptr; return *result; }
   unsigned int array(int /* index */) const { AssumeUncalled return 0; }
   unsigned int carray(int /* index */) const { AssumeUncalled return 0; }
   ArrayProperty operator[](int /* index */) { AssumeUncalled unsigned* result = nullptr; return *result; }
   unsigned int operator[](int /* index */) const { AssumeUncalled return 0; }

   static int getSize() { return 0; }
   void normalize() {}
   void adjustSize(int /* newSize */) { AssumeUncalled }
   void assertSize(int newSize) { AssumeCondition(newSize == 0) }
   void setSize(int exactSize) { AssumeCondition(exactSize == 0) }
   void setBitSize(int exactSize) { AssumeCondition(exactSize == 0) }
   void setCellSize(int exactSize) { AssumeCondition(exactSize == 0) }
   void clear() {}
   void swap(thisType& /* source */) {}
   unsigned int* arrayStart() { return nullptr; }
   const unsigned int* arrayStart() const { return nullptr; }
};

}

#define DefineBigCellClass
#include "ForwardNumerics/BaseInteger.inch"
#undef DefineBigCellClass

typedef DInteger::TBigCellInt<DInteger::TCellIntegerTraits<1> > AloneBigCellInt;

template <>
class TBigCellInt<DInteger::TCellIntegerTraits<1> > : public AloneBigCellInt {
  private:
   typedef TBigCellInt<DInteger::TCellIntegerTraits<1> > thisType;
   typedef AloneBigCellInt inherited;

   unsigned int& value() { return inherited::array(0); }
   unsigned int& svalue() { return inherited::array(0); }
   unsigned int value() const { return inherited::array(0); }

   static unsigned int add(unsigned int& cell, unsigned int value)
      {  auto temp = cell;
         cell += value;
         return (cell < temp) ? 1U : 0U;
      }
   static unsigned int sub(unsigned int& cell, unsigned int value)
      {  auto temp = cell;
         cell -= value;
         return (cell > temp) ? 1U : 0U;
      }

  protected:
   static int log_base_2(unsigned int value) { return DInteger::Access::log_base_2(value); }
   unsigned int& array(int /* index */) { return value(); }
   unsigned int array(int /* index */) const { return value(); }
   unsigned int carray(int /* index */) const { return value(); }

  public:
   class MidArray {
     private:
      unsigned int& uArrayValue;
      int uIndex;

     public:
      MidArray(thisType& source, int index)
         : uArrayValue(source.value()), uIndex(index) {}
      MidArray(MidArray&&) = default;
      MidArray(const MidArray&) = delete;
      MidArray& operator=(unsigned int value)
         {  uArrayValue = (uIndex > 0)
               ? ((value << (4*sizeof(unsigned int)))
                  | (uArrayValue & ~(~0U << 4*sizeof(unsigned int))))
               : ((uArrayValue & (~0U << 4*sizeof(unsigned int))) | value);
            return *this;
         }
      operator unsigned int() const
         {  return (uIndex > 0) ? (uArrayValue >> 4*sizeof(unsigned int))
               :  (uArrayValue & ~(~0U << 4*sizeof(unsigned int)));
         }
   };
   friend class MidArray;
   MidArray midArray(int index) { return MidArray(*this, index); }
   unsigned int midArray(int index) const
      {  return (index > 0) ? (value() >> 4*sizeof(unsigned int))
            :  (value() & ~(~0U << 4*sizeof(unsigned int)));
      }
   unsigned int cmidArray(int index) const { return midArray(index); }
   void setMidArray(int index, unsigned int value)
      {  svalue() = (index > 0)
            ? ((value << (4*sizeof(unsigned int)))
               | (svalue() & ~(~0U << 4*sizeof(unsigned int))))
            : ((svalue() & (~0U << 4*sizeof(unsigned int))) | value);
      }

   class BitArray {
     private:
      unsigned int* puArrayValue;
      int uIndex;

     public:
      BitArray(thisType& source, int index) : puArrayValue(&source.svalue()), uIndex(index) {}
      BitArray(BitArray&& source) = default;
      BitArray(const BitArray& source) = delete;
      // BitArray& operator=(BitArray&& source) = default;
      BitArray& operator=(const BitArray& source) = delete;
      BitArray& operator=(bool value)
         {  if (value)
               *puArrayValue |= (1U << uIndex);
            else
               *puArrayValue &= ~(1U << uIndex);
            return *this;
         }
      operator bool() const
         {  return (*puArrayValue & (1U << uIndex)) ? true : false; }
   };
   friend class BitArray;
   BitArray bitArray(int index) { return BitArray(*this, index); }
   bool bitArray(int index) const { return (value() & (1U << index)) ? true : false; }
   bool cbitArray(int index) const { return bitArray(index); }
   void setBitArray(int index, bool value)
      {  if (value)
            svalue() |= (1U << index);
         else
            svalue() &= ~(1U << index);
      }
   void setTrueBitArray(int index) { svalue() |= (1U << index); }
   void setFalseBitArray(int index) { svalue() &= ~(1U << index); }

   TBigCellInt() : inherited() {}
   TBigCellInt(unsigned int value) : inherited(value) {}
   TBigCellInt(const thisType& source) = default;
   thisType& operator=(const thisType& source) = default;
   thisType& operator=(unsigned int value) { svalue() = value; return *this; }

   unsigned int operator[](int /* index */) const { return value(); }
   unsigned int& operator[](int /* index */) { return value(); }
   ComparisonResult compare(const thisType& source) const
      {  return (value() < source.value()) ? CRLess
            : ((value() > source.value()) ? CRGreater : CREqual);
      }
   bool operator<(const thisType& source) const { return value() < source.value(); }
   bool operator>(const thisType& source) const { return value() > source.value(); }
   bool operator<=(const thisType& source) const { return value() <= source.value(); }
   bool operator>=(const thisType& source) const { return value() >= source.value(); }
   bool operator==(const thisType& source) const { return value() == source.value(); }
   bool operator!=(const thisType& source) const { return value() != source.value(); }
   thisType& operator<<=(int shift) { value() <<= shift; return *this; }
   thisType& operator>>=(int shift) { value() >>= shift; return *this; }
   void leftShiftLocal(int index, int shift)
      {  unsigned int temp = value();
         temp &= (~0U << index);
         temp <<= shift;
         value() = temp | (value() & ~(~0U << index));
      }
   void rightShiftLocal(int index, int shift)
      {  unsigned int temp = value();
         temp &= (~0U << index);
         temp >>= shift;
         value() = temp | (value() & ~(~0U << (index-shift)));
      }

   thisType& operator|=(const thisType& source) { value() |= source.value(); return *this; }
   thisType& operator^=(const thisType& source) { value() ^= source.value(); return *this; }
   thisType& operator&=(const thisType& source) { value() &= source.value(); return *this; }
   thisType& neg() { svalue() = ~svalue(); return *this; }
   thisType& neg(int shift)
      {  if (shift > 0)
            svalue() = (value() & (~0U << shift)) | (~value() & ~(~0U << shift));
         return *this;
      }
   thisType& clear(int shift)
      {  if (shift > 0)
            svalue() = (value() & (~0U << shift));
         return *this;
      }
   thisType& clearHigh(int shift)
      {  if (shift < (int) (8*sizeof(unsigned)))
            svalue() = (value() & ~(~0U << shift));
         return *this;
      }
   thisType& saturate(int shift)
      {  if (shift > 0)
            svalue() = (value() | ~(~0U << shift));
         return *this;
      }
   bool isZero() const { return value() == 0U; }
   bool hasZero(int shift) const
      {  AssumeCondition(shift <= (int) (8*sizeof(unsigned int)))
         return (shift <= 0)
            || ((value() << (8*sizeof(unsigned int)-shift)) == 0U);
      }
 
   typedef typename inherited::Carry Carry;
   Carry add(const thisType& source)
      {  value() += source.value();
         return Carry((value() < source.value()) ? 1U : 0U);
      }
   Carry sub(const thisType& source)
      {  Carry carry((value() < source.value()) ? 1U : 0U);
         value() -= source.value();
         return carry;
      }
   Carry plusAssign(const thisType& source) { return add(source); }
   Carry minusAssign(const thisType& source) { return sub(source); }
   Carry inc() { return Carry(++svalue() == 0U ? 1U : 0U); }
   Carry dec() { return Carry(svalue()-- == 0U ? 1U : 0U); }

   thisType& operator+=(const thisType& source) { add(source); return *this; }
   thisType operator+(const thisType& source) const
      {  thisType result = *this; result += source; return result; }
   thisType& operator-=(const thisType& source) { sub(source); return *this; }
   thisType operator-(const thisType& source) const
      {  thisType result = *this; result -= source; return result; }
   thisType& operator--() { dec(); return *this; }
   thisType& operator++() { inc(); return *this; }

   int getSize() const { return 1; }
   void assertSize(int newSize) { AssumeCondition(newSize <= 1) }
   
   thisType& operator/=(const thisType& source)
      {  AssumeCondition(source.value() != 0U)
         value() /= source.value();
         return *this;
      }
   thisType& operator/=(unsigned int source)
      {  AssumeCondition(source != 0U)
         value() /= source;
         return *this;
      }
   unsigned int operator%(unsigned int source) const
      {  return value() % source; }
   thisType& operator%=(const thisType& source)
      {  value() %= source.value(); return *this; }

   int log_base_2() const { return log_base_2(value()); }
   unsigned int getValue() const { return value(); }
   bool isAtomic() const { return true; }
   void swap(thisType& source) { inherited::swap(source); }
   void clear() { value() = 0U; }
};

namespace DInteger {

template<int i>
struct enable_if_two {};
 
template<>
struct enable_if_two<2> { static const int value = 2; };

template <>
class TBasicCellIntegerTraits<
         enable_if_two<sizeof(unsigned long long int)/(sizeof(unsigned))>::value>
   :  public CellIntegerTraitsContract {
  private:
   unsigned long long int ulValue;
   typedef TBasicCellIntegerTraits<2> thisType;

  protected:
   unsigned long long int& svalue() { return ulValue; }
   unsigned long long int& value() { return ulValue; }
   const unsigned long long int& value() const { return ulValue; }

  public:
   TBasicCellIntegerTraits() : ulValue(0) {}
   TBasicCellIntegerTraits(unsigned int value) : ulValue(value) {}
   TBasicCellIntegerTraits(const thisType& source)
      :  CellIntegerTraitsContract(), ulValue(source.ulValue) {}
   thisType& operator=(const thisType& source) { ulValue = source.ulValue; return *this; }
   thisType& operator=(unsigned int source) { ulValue = source; return *this; }
   class ArrayProperty {
     private:
      unsigned long long int* pulValue;
      int uIndex;

     public:
      ArrayProperty(thisType& source, int index) : pulValue(&source.svalue()), uIndex(index) {}
      ArrayProperty(ArrayProperty&& source) = default;
      ArrayProperty(const ArrayProperty& source) = delete;
      // ArrayProperty& operator=(ArrayProperty&& source) = default;
      ArrayProperty& operator=(const ArrayProperty& source) = delete;
      ArrayProperty& operator=(unsigned value)
         {  unsigned long long int val = value;
            if (uIndex != 0) {
               val <<= (sizeof(unsigned)*8);
               *pulValue &= ~(~0ULL << (sizeof(unsigned)*8));
            }
            else
               *pulValue &= (~0ULL << (sizeof(unsigned)*8));
            *pulValue |= val;
            return *this;
         }
      ArrayProperty& operator|=(unsigned value)
         {  unsigned long long int val = value;
            if (uIndex != 0)
               val <<= (sizeof(unsigned)*8);
            *pulValue |= val;
            return *this;
         }
      ArrayProperty& operator^=(unsigned value)
         {  unsigned long long int val = value;
            if (uIndex != 0)
               val <<= (sizeof(unsigned)*8);
            *pulValue ^= val;
            return *this;
         }
      ArrayProperty& operator&=(unsigned value)
         {  unsigned long long int val = value;
            if (uIndex != 0) {
               val <<= (sizeof(unsigned)*8);
               val |= ~(~0ULL << (sizeof(unsigned)*8));
            }
            else
               val |= (~0ULL << (sizeof(unsigned)*8));
            *pulValue &= val;
            return *this;
         }
      ArrayProperty& operator++()
         {  unsigned long long int val = *pulValue;
            if (uIndex == 0) {
               ++val;
               val &= ~(~0ULL << (sizeof(unsigned)*8));
               *pulValue &= (~0ULL << (sizeof(unsigned)*8));
            }
            else {
               val >>= (sizeof(unsigned)*8);
               ++val;
               val <<= (sizeof(unsigned)*8);
               *pulValue &= ~(~0ULL << (sizeof(unsigned)*8));
            };
            *pulValue |= val;
            return *this;
         }
      ArrayProperty& operator+=(unsigned inc)
         {  unsigned long long int val = *pulValue;
            if (uIndex == 0) {
               val += inc;
               val &= ~(~0ULL << (sizeof(unsigned)*8));
               *pulValue &= (~0ULL << (sizeof(unsigned)*8));
            }
            else {
               val >>= (sizeof(unsigned)*8);
               val += inc;
               val <<= (sizeof(unsigned)*8);
               *pulValue &= ~(~0ULL << (sizeof(unsigned)*8));
            };
            *pulValue |= val;
            return *this;
         }
      ArrayProperty& operator--()
         {  unsigned long long int val = *pulValue;
            if (uIndex == 0) {
               --val;
               val &= ~(~0ULL << (sizeof(unsigned)*8));
               *pulValue &= (~0ULL << (sizeof(unsigned)*8));
            }
            else {
               val >>= (sizeof(unsigned)*8);
               --val;
               val <<= (sizeof(unsigned)*8);
               *pulValue &= ~(~0ULL << (sizeof(unsigned)*8));
            };
            *pulValue |= val;
            return *this;
         }
      ArrayProperty& operator-=(unsigned dec)
         {  unsigned long long int val = *pulValue;
            if (uIndex == 0) {
               val -= dec;
               val &= ~(~0ULL << (sizeof(unsigned)*8));
               *pulValue &= (~0ULL << (sizeof(unsigned)*8));
            }
            else {
               val >>= (sizeof(unsigned)*8);
               val -= dec;
               val <<= (sizeof(unsigned)*8);
               *pulValue &= ~(~0ULL << (sizeof(unsigned)*8));
            };
            *pulValue |= val;
            return *this;
         }
      operator unsigned() const
         {  unsigned result;
            if (uIndex == 0)
               result = (unsigned) *pulValue;
            else {
               unsigned long long int val = *pulValue;
               val >>= (sizeof(unsigned)*8);
               result = (unsigned) val;
            }
            return result;
         }
   };
   ArrayProperty array(int index)
      {  AssumeCondition(!(index & ~1U))
         return ArrayProperty(*this, index);
      }
   unsigned int array(int index) const
      {  AssumeCondition(index >= 0)
         unsigned int result;
         if (index == 0)
            result = (unsigned int) ulValue;
         else if (index == 1) {
            unsigned long long int val = ulValue;
            val >>= (sizeof(unsigned)*8);
            result = (unsigned int) val;
         }
         else
            result = 0;
         return result;
      }
   unsigned int carray(int index) const { return array(index); }
   ArrayProperty operator[](int index)
      {  AssumeCondition(!(index & ~1U))
         return ArrayProperty(*this, index);
      }
   unsigned int operator[](int index) const
      {  AssumeCondition(index >= 0)
         return array(index);
      }

   static int getSize() { return 2; }
   void normalize() {}
   void adjustSize(int /* newSize */) { AssumeUncalled }
   void assertSize(int newSize) { AssumeCondition(newSize <= 2) }
   void setSize(int exactSize) { AssumeCondition(exactSize == 2) }
   void setBitSize(int exactSize) { AssumeCondition((exactSize + 8*sizeof(unsigned)-1)/(8*sizeof(unsigned)) == 2) }
   void setCellSize(int exactSize) { AssumeCondition(exactSize == 2) }
   void clear() { ulValue = 0; }
   void swap(thisType& source)
      {  unsigned long long int temp = ulValue;
         ulValue = source.ulValue;
         source.ulValue = temp;
      }
   unsigned long long int* arrayStart() { return &ulValue; }
   const unsigned long long int* arrayStart() const { return &ulValue; }
};

} // end of namespace DInteger

typedef DInteger::TBigCellInt<DInteger::TCellIntegerTraits<2> > DoubleBigCellInt;

template <>
class TBigCellInt<DInteger::TCellIntegerTraits<
         DInteger::enable_if_two<sizeof(unsigned long long int)/(sizeof(unsigned))>::value> >
   :  public DoubleBigCellInt {
  private:
   typedef TBigCellInt<DInteger::TCellIntegerTraits<2> > thisType;
   typedef DoubleBigCellInt inherited;

   unsigned long long int& value() { return inherited::value(); } // { return *((unsigned long long int*) _array()); }
   unsigned long long int& svalue() { return inherited::value(); } // { return *((unsigned long long int*) _array()); }
   const unsigned long long int& value() const { return inherited::value(); } // { return *((unsigned long long int*) _array()); }

   static unsigned int add(unsigned int& cell, unsigned int value)
      {  unsigned int temp = cell;
         cell += value;
         return (cell < temp) ? 1U : 0U;
      }
   static unsigned int sub(unsigned int& cell, unsigned int value)
      {  unsigned int temp = cell;
         cell -= value;
         return (cell > temp) ? 1U : 0U;
      }

  protected:
   static int log_base_2(unsigned long long int value)
      {  int result = 1;
         while ((value >>= 1) != 0)
            ++result;
         return result;
      }
   static int log_base_2(unsigned int value)
      {  return DInteger::Access::log_base_2(value); }

  public:
   class MidArray {
     private:
      unsigned long long int* plluValue;
      int uIndex;

     public:
      MidArray(thisType& source, int index)
         : plluValue(&source.value()), uIndex(index) { AssumeCondition(uIndex < 4) }
      MidArray(MidArray&&) = default;
      MidArray(const MidArray&) = delete;
      MidArray& operator=(unsigned int value)
         {  *plluValue &= (~(~0ULL << 4*sizeof(unsigned int)) << (uIndex*4*sizeof(unsigned int)));
            *plluValue |= (((unsigned long long int) value) << (uIndex*4*sizeof(unsigned int)));
            return *this;
         }
      operator unsigned int() const
         {  return (unsigned int) ((*plluValue >> (uIndex*4*sizeof(unsigned int)))
               & ~(~0ULL << 4*sizeof(unsigned int)));
         }
   };
   friend class MidArray;
   MidArray midArray(int index) { return MidArray(*this, index); }
   unsigned int midArray(int index) const
      {  AssumeCondition(index < 4)
         return (unsigned int) ((value() >> (index*4*sizeof(unsigned int)))
               & ~(~0ULL << 4*sizeof(unsigned int)));
      }
   unsigned int cmidArray(int index) const { return midArray(index); }
   void setMidArray(int index, unsigned int value)
      {  AssumeCondition(index < 4)
         svalue() &= (~(~0ULL << 4*sizeof(unsigned int)) << (index*4*sizeof(unsigned int)));
         svalue() |= (((unsigned long long int) value) << (index*4*sizeof(unsigned int)));
      }

   class BitArray {
     private:
      unsigned long long int* plluValue;
      int uIndex;

     public:
      BitArray(thisType& source, int index)
         :  plluValue(&source.value()), uIndex(index)
         {  AssumeCondition((unsigned) index < 2*8*sizeof(unsigned int)) }
      BitArray(BitArray&&) = default;
      BitArray(const BitArray&) = delete;
      // BitArray& operator=(BitArray&& source) = default;
      BitArray& operator=(const BitArray& source) = delete;
      BitArray& operator=(bool value)
         {  if (value)
               *plluValue |= (1ULL << uIndex);
            else
               *plluValue &= ~(1ULL << uIndex);
            return *this;
         }
      operator bool() const
         {  return (*plluValue & (1ULL << uIndex)) != 0ULL; }
   };
   friend class BitArray;
   BitArray bitArray(int index)
      {  return BitArray(*this, index); }
   bool bitArray(int index) const
      {  AssumeCondition((unsigned) index < 2*8*sizeof(unsigned int))
         return (value() & (1ULL << index)) != 0ULL;
      }
   bool cbitArray(int index) const { return bitArray(index); }
   void setBitArray(int index, bool value)
      {  AssumeCondition((unsigned) index < 2*8*sizeof(unsigned int))
         if (value)
            svalue() |= (1ULL << index);
         else
            svalue() &= ~(1ULL << index);
      }
   void setTrueBitArray(int index)
      {  AssumeCondition((unsigned) index < 2*8*sizeof(unsigned int))
         value() |= (1ULL << index);
      }
   void setFalseBitArray(int index)
      {  AssumeCondition((unsigned) index < 2*8*sizeof(unsigned int))
         value() &= ~(1ULL << index);
      }

   TBigCellInt() : inherited() {}
   TBigCellInt(unsigned int value) : inherited(value) {}
   TBigCellInt(const thisType& source) = default;
   thisType& operator=(const thisType& source) = default;
   thisType& operator=(unsigned int value) { svalue() = value; return *this; }

   ComparisonResult compare(const thisType& source) const
      {  return (value() < source.value()) ? CRLess
            : ((value() > source.value()) ? CRGreater : CREqual);
      }
   bool operator<(const thisType& source) const { return value() < source.value(); }
   bool operator>(const thisType& source) const { return value() > source.value(); }
   bool operator<=(const thisType& source) const { return value() <= source.value(); }
   bool operator>=(const thisType& source) const { return value() >= source.value(); }
   bool operator==(const thisType& source) const { return value() == source.value(); }
   bool operator!=(const thisType& source) const { return value() != source.value(); }
   thisType& operator<<=(int shift) { value() <<= shift; return *this; }
   thisType& operator>>=(int shift) { value() >>= shift; return *this; }
   void leftShiftLocal(int index, int shift)
      {  unsigned long long int temp = value();
         temp &= (~0ULL << index);
         temp <<= shift;
         value() = temp | (value() & ~(~0ULL << index));
      }
   void rightShiftLocal(int index, int shift)
      {  unsigned long long int temp = value();
         temp &= (~0ULL << index);
         temp >>= shift;
         value() = temp | (value() & ~(~0ULL << (index-shift)));
      }
   thisType& operator|=(const thisType& source) { value() |= source.value(); return *this; }
   thisType& operator^=(const thisType& source) { value() ^= source.value(); return *this; }
   thisType& operator&=(const thisType& source) { value() &= source.value(); return *this; }
   thisType& neg()
      {  value() = ~value();
         return *this;
      }
   thisType& neg(int shift)
      {  if (shift > 0)
            value() = (value() & (~0ULL << shift)) | (~value() & ~(~0ULL << shift));
         return *this;
      }
   thisType& clear(int shift)
      {  if (shift > 0)
            value() = (value() & (~0ULL << shift));
         return *this;
      }
   thisType& clearHigh(int shift)
      {  if (shift < (int) (8*sizeof(unsigned long long)))
            value() = (value() & ~(~0ULL << shift));
         return *this;
      }
   thisType& saturate(int shift)
      {  if (shift > 0)
            value() = (value() | ~(~0ULL << shift));
         return *this;
      }
   bool isZero() const { return value() == 0U; }
   bool hasZero(int shift) const
      {  AssumeCondition((unsigned) shift <= 8*sizeof(unsigned long long int))
         return (shift <= 0)
            || ((value() << (8*sizeof(unsigned long long int)-shift)) == 0ULL);
      }
 
   Carry add(const thisType& source)
      {  value() += source.value();
         return Carry((value() < source.value()) ? 1U : 0U);
      }
   Carry sub(const thisType& source)
      {  Carry carry((value() < source.value()) ? 1U : 0U);
         value() -= source.value();
         return carry;
      }
   Carry plusAssign(const thisType& source) { return add(source); }
   Carry minusAssign(const thisType& source) { return sub(source); }
   Carry inc() { return Carry(++value() == 0U ? 1U : 0U); }
   Carry dec() { return Carry(value()-- == 0U ? 1U : 0U); }

   thisType& operator+=(const thisType& source) { add(source); return *this; }
   thisType operator+(const thisType& source) const
      {  thisType result = *this; result += source; return result; }
   thisType& operator-=(const thisType& source) { sub(source); return *this; }
   thisType operator-(const thisType& source) const
      {  thisType result = *this; result -= source; return result; }
   thisType& operator--() { dec(); return *this; }
   thisType& operator++() { inc(); return *this; }

   int getSize() const { return 2; }
   void assertSize(int newSize) { AssumeCondition(newSize <= 2) }
   
   thisType& operator/=(const thisType& source)
      {  AssumeCondition(source.value() != 0U)
         value() /= source.value();
         return *this;
      }
   thisType& operator/=(unsigned int source)
      {  AssumeCondition(source != 0U)
         value() /= source;
         return *this;
      }
   unsigned long long int operator%(unsigned int source) const
      {  return value() % source; }
   thisType& operator%=(const thisType& source)
      {  value() %= source.value(); return *this; }

   int log_base_2() const { return log_base_2(value()); }
   unsigned int getValue() const { return (unsigned int) value(); }
   bool isAtomic() const { return !carray(1); }
   void swap(thisType& source) { inherited::swap(source); }
   void clear() { value() = 0U; }
};

#define DefineBigIntClass
#include "ForwardNumerics/BaseInteger.inch"
#undef DefineBigIntClass

} // end of namespace Numerics

#endif // Numerics_BaseIntegerH

