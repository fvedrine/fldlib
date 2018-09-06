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
// File      : BaseIntegerGeneric.h
// Description :
//   Definition of a class of integers with unbound size.
//

#ifndef Numerics_BaseIntegerGenericH
#define Numerics_BaseIntegerGenericH

#include "ForwardNumerics/BaseIntegerCommon.h"

namespace Numerics {

class UnsignedLongBaseStoreTraits {
  public:
   static const int USizeBaseInBits = sizeof(unsigned long)*8;
   typedef unsigned long BaseType;
   typedef unsigned long* BaseTypePointer;
   typedef unsigned long& BaseTypeReference;
   typedef unsigned long BaseTypeConstReference;
   static int log_base_2(unsigned long value)
      {  int result = 1;
         while ((value >>= 1) != 0)
            ++result;
         return result;
      }
   static void clearArray(unsigned long* array, int count)
      {  memset(array, 0, count*sizeof(unsigned long)); }
   static void copyArray(unsigned long* target, const unsigned long* source, int count)
      {  memcpy(target, source, count*sizeof(unsigned long)); }
   static int sizeBaseInBits() { return 8*sizeof(unsigned long); }
   static int minCellsCountToStoreBits(int bitsNumber)
      {  return (int) ((bitsNumber + 8*sizeof(unsigned long) - 1) / (8*sizeof(unsigned long))); } 
   static void swapArray(unsigned long* target, unsigned long* source, int count)
      {  unsigned long temp;
         for (int i = 0; i < count; ++i) {
            temp = target[i];
            target[i] = source[i];
            source[i] = temp;
         };
      }
   static unsigned long detectCarryAfterAddition(unsigned long result, unsigned long operand)
      {  return (result < operand) ? 1UL : 0UL; }
   static unsigned long detectCarryBeforeSubstraction(unsigned long first, unsigned long second)
      {  return (first < second) ? 1UL : 0UL; }
   static unsigned long getStoreMidHighPart(unsigned long value, unsigned long store)
      {  return ((value << (4*sizeof(unsigned long))) | (store & ~(~0UL << 4*sizeof(unsigned long)))); }
   static void storeIntoMidHighPart(unsigned long value, unsigned long& store)
      {  store &= ~(~0UL << 4*sizeof(unsigned long));
         store |= (value << (4*sizeof(unsigned long)));
      }
   static unsigned long getStoreMidLowPart(unsigned long value, unsigned long store)
      {  return ((store & (~0UL << 4*sizeof(unsigned long))) | value); }
   static void storeIntoMidLowPart(unsigned long value, unsigned long& store)
      {  store &= (~0UL << 4*sizeof(unsigned long));
         store |= value;
      }
   static unsigned long getMidHighPart(unsigned long value)
      {  return value >> 4*sizeof(unsigned long); }
   static unsigned long getMidLowPart(unsigned long value)
      {  return value & ~(~0UL << 4*sizeof(unsigned long)); }
   static unsigned long getLowPart(unsigned long value, int shift)
      {  return value & ~(~0UL << shift); }
   static unsigned long getHighPart(unsigned long value, int shift)
      {  return (value >> shift); }
   static unsigned long getMiddlePart(unsigned long value, int lowBit, int sizeInBits)
      {  return (value >> lowBit) & ~(~0UL << sizeInBits); }
   static unsigned long getStoreHighPart(unsigned long value, int shift)
      {  return (value << shift); }
   static unsigned long getStoreHighPart(unsigned long value, int shift, unsigned long store)
      {  return (value << shift) | (store & ~(~0UL << shift)); }
   static void storeIntoHighPart(unsigned long value, int shift, unsigned long& store)
      {  store &= ~(~0UL << shift);
         store |= (value << shift);
      }
   static unsigned long getStoreLowPart(unsigned long value, int shift)
      {  return value & ~(~0UL << shift); }
   static unsigned long getStoreLowPart(unsigned long value, int shift, unsigned long store)
      {  return (store & (~0UL << shift)) | (value & ~(~0UL << shift)); }
   static void storeIntoLowPart(unsigned long value, int shift, unsigned long& store)
      {  store &= (~0UL << shift);
         store |= (value & ~(~0UL << shift));
      }
   static void setTrueBit(unsigned long& result, int index) { result |= (1UL << index); }
   static void setFalseBit(unsigned long& result, int index) { result &= ~(1UL << index); }
   static void setBit(unsigned long& result, int index, bool value)
      {  if (value)
            result |= (1UL << index);
         else
            result &= ~(1UL << index);
      }
   static bool getBit(unsigned long value, int index)
      {  return (value & (1UL << index)) ? true : false; }
   static void leftShiftLocal(unsigned long& value, int index, int shift)
      {  unsigned long temp = value;
         temp &= (~0UL << index);
         temp <<= shift;
         value = temp | (value & ~(~0UL << index));
      }
   static void rightShiftLocal(unsigned long& value, int index, int shift)
      {  unsigned long temp = value;
         temp &= (~0UL << index);
         temp >>= shift;
         value = temp | (value & ~(~0UL << (index-shift)));
      }
   static void leftShiftAndClearLow(unsigned long& value, int shift, int low)
      {  value <<= shift;
         value &= ~0UL << low;
      }
   static void rightShiftAndClearHigh(unsigned long& value, int shift, int high)
      {  value >>= shift;
         value &= ~(~0UL << high);
      }
   static void rightShift(unsigned long& value, int shift) { value >>= shift; }
   static void leftShift(unsigned long& value, int shift) { value <<= shift; }
   static void negLowValuePart(unsigned long& value, int shift)
      {  if (shift > 0)
            value = (value & (~0UL << shift)) | (~value & ~(~0UL << shift));
      }
   static void clearLowValuePart(unsigned long& value, int shift)
      {  if (shift > 0)
            value &= (~0UL << shift);
      }
   static void clearHighValuePart(unsigned long& value, int shift)
      {  if (shift < (int) (8*sizeof(unsigned long)))
            value &= ~(~0UL << shift);
      }
   static unsigned long getSaturation() { return ~0UL; }
   static void saturateLowValuePart(unsigned long& value, int shift)
      {  if (shift > 0)
            value |= ~(~0UL << shift);
      }
   static bool isZeroValue(unsigned long value) { return value == 0UL; }
   static bool hasZeroValue(unsigned long value, int shift)
      {  AssumeCondition(shift <= (int) (8*sizeof(unsigned long)))
         return (shift <= 0) || ((value << (8*sizeof(unsigned long)-shift)) == 0UL);
      }
   static bool isOneValue(unsigned long value) { return value == 1UL; }
   static void writeValue(STG::IOObject::OSBase& out, unsigned long value, bool isRaw)
#ifdef StandardClassesHPP
      {  out.write(value, isRaw); }
#else
      {  if (!isRaw)
            out << value;
         else
            out.write(reinterpret_cast<char*>(&value), sizeof(value));
      }
#endif
   static void readValue(STG::IOObject::ISBase& in, unsigned long& value, bool isRaw)
      {
#ifdef StandardClassesHPP
         in.read(value, isRaw);
#else
         if (isRaw) {
            in.read(reinterpret_cast<char*>(&value), sizeof(value));
         }
         else
            in >> value;
#endif
      }
};

/* IntegerTraits should conform to DInteger::CellIntegerTraitsContract */
template <class BaseStoreTraits, class IntegerTraits>
class TGBigCellInt;
template <class BaseStoreTraits, class IntegerTraits>
class TGBigCell;
template <class BaseStoreTraits, class IntegerTraits>
class TGBigInt;

namespace DInteger {

/* BaseStoreTraits should conform to UnsignedBaseStoreTraits with optionally another base type */
template <class BaseStoreTraits>
class GCellIntegerTraitsContract : public BaseStoreTraits {
  public:
   GCellIntegerTraitsContract() {}
   GCellIntegerTraitsContract(const GCellIntegerTraitsContract& source) { AssumeUncalled }
   GCellIntegerTraitsContract& operator=(const GCellIntegerTraitsContract& source) { AssumeUncalled return *this; }

   typedef typename BaseStoreTraits::BaseTypeReference ArrayProperty;
   ArrayProperty array(int index) { AssumeUncalled typename BaseStoreTraits::BaseTypePointer result = nullptr; return *result; }
   typename BaseStoreTraits::BaseType array(int index) const { AssumeUncalled return 0; }
   typename BaseStoreTraits::BaseType carray(int index) const { AssumeUncalled return 0; }
   ArrayProperty operator[](int index) { AssumeUncalled typename BaseStoreTraits::BaseType* result = nullptr; return *result; }
   typename BaseStoreTraits::BaseTypeConstReference operator[](int index) const { AssumeUncalled return 0; }
   typedef GCellIntegerTraitsContract MultResult;

   typedef GCellIntegerTraitsContract QuotientResult;
   typedef GCellIntegerTraitsContract RemainderResult;
   typedef GCellIntegerTraitsContract NormalizedRemainderResult;
   void copyLow(const MultResult& result) { AssumeUncalled }
   int getSize() const { AssumeUncalled return 0; }
   void adjustSize(int newSize) { AssumeUncalled }
   void setSize(int exactSize) { AssumeUncalled }
   void setBitSize(int exactSize) { AssumeUncalled }
   void setCellSize(int exactSize) { AssumeUncalled }
   void normalize() { AssumeUncalled }
   void assertSize(int newSize) { AssumeUncalled }
   void clear() { AssumeUncalled }
   typedef GCellIntegerTraitsContract ExtendedInteger;
};

template <class BaseStoreTraits, int UCellSize>
class TGBasicCellIntegerTraits : public BaseStoreTraits {
  public:
   typedef typename BaseStoreTraits::BaseType BaseType;
   typedef typename BaseStoreTraits::BaseTypeConstReference BaseTypeConstReference;

  private:
   typename BaseStoreTraits::BaseType auArray[UCellSize];
   typedef TGBasicCellIntegerTraits<BaseStoreTraits, UCellSize> thisType;

  protected:
   BaseType* _array() { return auArray; }
   const BaseType* _array() const { return auArray; }

  public:
   TGBasicCellIntegerTraits() { BaseStoreTraits::clearArray(auArray, UCellSize); }
   TGBasicCellIntegerTraits(const thisType& source)
      {  BaseStoreTraits::copyArray(auArray, source.auArray, UCellSize); }
   thisType& operator=(const thisType& source)
      {  BaseStoreTraits::copyArray(auArray, source.auArray, UCellSize);
         return *this;
      }
   thisType& operator=(BaseTypeConstReference source)
      {  if (UCellSize > 1)
            BaseStoreTraits::clearArray(auArray, UCellSize);
         auArray[0] = source;
         return *this;
      }
   typedef BaseType& ArrayProperty;
#ifdef __GNUC__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Warray-bounds"
#endif
   ArrayProperty array(int index)
      {  AssumeCondition(index >= 0 && (index < UCellSize))
         return auArray[index];
      }
   BaseType array(int index) const
      {  AssumeCondition(index >= 0)
         BaseType result;
         if (index < UCellSize)
            result = auArray[index];
         else
            result = 0x0;
         return result;
      }
   BaseTypeConstReference carray(int index) const { return array(index); }
   ArrayProperty operator[](int index)
      {  AssumeCondition(index >= 0 && (index < UCellSize))
         return auArray[index];
      }
#ifdef __GNUC__
#pragma GCC diagnostic pop
#endif
   BaseType operator[](int index) const
      {  AssumeCondition(index >= 0)
         BaseType result;
         if (index < UCellSize)
            result = auArray[index];
         else
            result = 0x0;
         return result;
      }

   static int getSize() { return UCellSize; }
   void normalize() {}
   void adjustSize(int /* newSize */) { AssumeUncalled }
   void assertSize(int newSize) { AssumeCondition(newSize <= UCellSize) }
   void setSize(int exactSize) { AssumeCondition(exactSize == UCellSize) }
   void setBitSize(int exactSize)
      {  AssumeCondition(BaseStoreTraits::minCellsCountToStoreBits(exactSize) == UCellSize) }
   void setCellSize(int exactSize) { AssumeCondition(exactSize == UCellSize) }
   void clear() { BaseStoreTraits::clearArray(auArray, UCellSize); }
   void swap(thisType& source) { BaseStoreTraits::swapArray(auArray, source.auArray, UCellSize); }
   BaseType* arrayStart() { return auArray; }
   const BaseType* arrayStart() const { return auArray; }
};

template <class BaseStoreTraits>
class TGBasicCellIntegerTraits<BaseStoreTraits, 0> : public GCellIntegerTraitsContract<BaseStoreTraits> {
  private:
   typedef TGBasicCellIntegerTraits<BaseStoreTraits, 0> thisType;

  protected:
   typename BaseStoreTraits::BaseType* _array() { return nullptr; }
   const typename BaseStoreTraits::BaseType* _array() const { return nullptr; }

  public:
   TGBasicCellIntegerTraits() {}
   TGBasicCellIntegerTraits(const thisType& source) {}
   thisType& operator=(const thisType& source) { return *this; }
   thisType& operator=(typename BaseStoreTraits::BaseType source) { return *this; }
   typedef typename BaseStoreTraits::BaseTypeReference ArrayProperty;
   ArrayProperty array(int index) { AssumeUncalled typename BaseStoreTraits::BaseType* result = nullptr; return *result; }
   typename BaseStoreTraits::BaseTypeConstReference array(int index) const { AssumeUncalled return 0; }
   typename BaseStoreTraits::BaseTypeConstReference carray(int index) const { AssumeUncalled return 0; }
   ArrayProperty operator[](int index) { AssumeUncalled typename BaseStoreTraits::BaseTypePointer* result = nullptr; return *result; }
   typename BaseStoreTraits::BaseTypeConstReference operator[](int index) const { AssumeUncalled return 0; }

   static int getSize() { return 0; }
   void normalize() {}
   void adjustSize(int newSize) { AssumeUncalled }
   void assertSize(int newSize) { AssumeCondition(newSize == 0) }
   void setSize(int exactSize) { AssumeCondition(exactSize == 0) }
   void setBitSize(int exactSize) { AssumeCondition(exactSize == 0) }
   void setCellSize(int exactSize) { AssumeCondition(exactSize == 0) }
   void clear() {}
   void swap(thisType& source) {}
   typename BaseStoreTraits::BaseType* arrayStart() { return nullptr; }
   const typename BaseStoreTraits::BaseType* arrayStart() const { return nullptr; }
};

} // end of namespace DInteger

#define DefineGeneric
#define DefineBigCellClass
#include "ForwardNumerics/BaseInteger.inch"
#undef DefineGeneric
#undef DefineBigCellClass

template <class BaseStoreTraits>
class TGBigCellInt<BaseStoreTraits, DInteger::TGCellIntegerTraits<BaseStoreTraits, 1> >
   :  public DInteger::TGBigCellInt<BaseStoreTraits, DInteger::TGCellIntegerTraits<BaseStoreTraits, 1> > {
  public:
   typedef typename BaseStoreTraits::BaseType BaseType;
   typedef typename BaseStoreTraits::BaseTypeReference BaseTypeReference;
   typedef typename BaseStoreTraits::BaseTypeConstReference BaseTypeConstReference;

  private:
   typedef TGBigCellInt<BaseStoreTraits, DInteger::TGCellIntegerTraits<BaseStoreTraits, 1> > thisType;
   typedef DInteger::TGBigCellInt<BaseStoreTraits, DInteger::TGCellIntegerTraits<BaseStoreTraits, 1> > inherited;

   BaseTypeReference value() { return inherited::operator[](0); }
   BaseTypeReference svalue() { return inherited::operator[](0); }
   BaseTypeConstReference value() const { return inherited::operator[](0); }

// static BaseType add(BaseTypeReference cell, BaseType value)
//    {  BaseType temp = cell;
//       cell += value;
//       return (cell < temp) ? 1 : 0;
//    }
// static BaseType sub(BaseTypeReference cell, BaseType value)
//    {  BaseType temp = cell;
//       cell -= value;
//       return (cell > temp) ? 1 : 0;
//    }

  protected:
   static int log_base_2(BaseTypeConstReference value) { return BaseStoreTraits::log_base_2(value); }
   BaseTypeReference array(int index) { return inherited::value(); }
   BaseTypeConstReference array(int index) const { return inherited::value(); }
   BaseTypeConstReference carray(int index) const { return inherited::value(); }

  public:
   class MidArray {
     private:
      BaseTypeReference uArrayValue;
      int uIndex;

     public:
      MidArray(thisType& source, int index)
         : uArrayValue(source.value()), uIndex(index) {}
      MidArray(MidArray&&) = default;
      MidArray(const MidArray&) = delete;
      MidArray& operator=(BaseTypeConstReference value)
         {  if (uIndex > 0)
               BaseStoreTraits::storeIntoMidHighPart(value, uArrayValue);
            else
               BaseStoreTraits::storeIntoMidLowPart(value, uArrayValue);
            return *this;
         }
      operator BaseType() const
         {  return (uIndex > 0)
               ? BaseStoreTraits::getMidHighPart(uArrayValue)
               : BaseStoreTraits::getMidLowPart(uArrayValue);
         }
   };
   friend class MidArray;
   MidArray midArray(int index) { return MidArray(*this, index); }
   BaseType midArray(int index) const
      {  return (index > 0)
            ? BaseStoreTraits::getMidHighPart(value())
            : BaseStoreTraits::getMidLowPart(value());
      }
   BaseType cmidArray(int index) const
      {  return (index > 0)
            ? BaseStoreTraits::getMidHighPart(value())
            : BaseStoreTraits::getMidLowPart(value());
      }
   void setMidArray(int index, BaseTypeConstReference midValue)
      {  if (index > 0)
            BaseStoreTraits::storeIntoMidHighPart(midValue, svalue());
         else
            BaseStoreTraits::storeIntoMidLowPart(midValue, svalue());
         return *this;
      }

   class BitArray {
     private:
      BaseType* puArrayValue;
      int uIndex;

     public:
      BitArray(thisType& source, int index) : puArrayValue(&source.svalue()), uIndex(index) {}
      BitArray(BitArray&& source) = default;
      BitArray(const BitArray& source) = delete;
      BitArray& operator=(const BitArray& source) = delete;
      BitArray& operator=(bool value)
         {  if (value)
               BaseStoreTraits::setTrueBit(*puArrayValue, uIndex);
            else
               BaseStoreTraits::setFalseBit(*puArrayValue, uIndex);
            return *this;
         }
      operator bool() const
         {  return BaseStoreTraits::getBit(*puArrayValue, uIndex); }
   };
   friend class BitArray;
   BitArray bitArray(int index) { return BitArray(*this, index); }
   bool bitArray(int index) const
      {  return BaseStoreTraits::getBit(value(), index); }
   bool cbitArray(int index) const
      {  return BaseStoreTraits::getBit(value(), index); }
   void setBitArray(int index, bool value)
      {  if (value)
            BaseStoreTraits::setTrueBit(svalue(), index);
         else
            BaseStoreTraits::setFalseBit(svalue(), index);
      }
   void setTrueBitArray(int index)
      {  BaseStoreTraits::setTrueBit(svalue(), index); }
   void setFalseBitArray(int index)
      {  BaseStoreTraits::setFalseBit(svalue(), index); }

   TGBigCellInt() : inherited() {}
   TGBigCellInt(BaseTypeConstReference value) : inherited(value) {}
   TGBigCellInt(const thisType& source) = default;
   thisType& operator=(const thisType& source) = default;
   thisType& operator=(BaseTypeConstReference value) { svalue() = value; return *this; }

   BaseTypeConstReference operator[](int index) const { return value(); }
   BaseTypeReference& operator[](int /* index */) { return value(); }
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
   void leftShiftLocal(int index, int shift) { BaseStoreTraits::leftShiftLocal(value(), index, shift); }
   void rightShiftLocal(int index, int shift) { BaseStoreTraits::rightShiftLocal(value(), index, shift); }

   thisType& operator|=(const thisType& source) { value() |= source.value(); return *this; }
   thisType& operator^=(const thisType& source) { value() ^= source.value(); return *this; }
   thisType& operator&=(const thisType& source) { value() &= source.value(); return *this; }
   thisType& neg() { svalue() = ~svalue(); return *this; }
   thisType& neg(int shift)
      {  BaseStoreTraits::negLowValuePart(svalue(), shift); return *this; }
   thisType& clear(int shift)
      {  BaseStoreTraits::clearLowValuePart(svalue(), shift); return *this; }
   thisType& clearHigh(int shift)
      {  BaseStoreTraits::clearHighValuePart(svalue(), shift); return *this; }
   thisType& saturate(int shift)
      {  BaseStoreTraits::saturateLowValuePart(svalue(), shift); return *this; }
   bool isZero() const { return BaseStoreTraits::isZeroValue(value()); }
   bool hasZero(int shift) const { return BaseStoreTraits::hasZeroValue(value(), shift); }
 
   typedef typename inherited::Carry Carry;
   Carry add(const thisType& source)
      {  value() += source.value();
         return Carry(BaseStoreTraits::detectCarryAfterAddition(value(), source.value()));
      }
   Carry sub(const thisType& source)
      {  Carry carry(BaseStoreTraits::detectCarryBeforeSubstraction(value(), source.value()));
         value() -= source.value();
         return carry;
      }
   Carry plusAssign(const thisType& source) { return add(source); }
   Carry minusAssign(const thisType& source) { return sub(source); }
   Carry inc() { ++svalue(); return Carry(BaseStoreTraits::isZeroValue(value()) ? 0x1 : 0x0); }
   Carry dec()
      {  Carry carry(BaseStoreTraits::isZeroValue(value()) ? 0x1 : 0x0);
         --svalue();
         return carry;
      }

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
      {  AssumeCondition(BaseStoreTraits::isZero(source.value()))
         value() /= source.value();
         return *this;
      }
   thisType& operator/=(BaseTypeConstReference source)
      {  AssumeCondition(BaseStoreTraits::isZero(source))
         value() /= source;
         return *this;
      }
   BaseType operator%(BaseTypeConstReference source) const
      {  return value() % source; }
   thisType& operator%=(const thisType& source)
      {  svalue() %= source.value(); return *this; }

   BaseType log_base_2() const { return BaseStoreTraits::log_base_2(value()); }
   BaseTypeConstReference getValue() const { return value(); }
   bool isAtomic() const { return true; }
   void swap(thisType& source) { inherited::swap(source); }
   void clear() { value() = 0x0; }
};

namespace DInteger {

template<int i>
struct genable_if_two {};
 
template<>
struct genable_if_two<2> { static const int value = 2; };

template <>
class TGBasicCellIntegerTraits<UnsignedBaseStoreTraits,
         genable_if_two<sizeof(unsigned long long int)/(sizeof(unsigned))>::value>
   :  public GCellIntegerTraitsContract<UnsignedBaseStoreTraits> {
  private:
   unsigned long long int ulValue;
   typedef TGBasicCellIntegerTraits<UnsignedBaseStoreTraits, 2> thisType;

  protected:
   unsigned long long int& svalue() { return ulValue; }
   unsigned long long int& value() { return ulValue; }
   const unsigned long long int& value() const { return ulValue; }

  public:
   TGBasicCellIntegerTraits() : ulValue(0) {}
   TGBasicCellIntegerTraits(unsigned int value) : ulValue(value) {}
   TGBasicCellIntegerTraits(const thisType& source)
      :  GCellIntegerTraitsContract<UnsignedBaseStoreTraits>(), ulValue(source.ulValue) {}
   thisType& operator=(const thisType& source) { ulValue = source.ulValue; return *this; }
   thisType& operator=(unsigned int source) { ulValue = source; return *this; }
   class ArrayProperty {
     private:
      unsigned long long int* pulValue;
      int uIndex;

     public:
      ArrayProperty(thisType& source, int index) : pulValue(&source.svalue()), uIndex(index) {}
      ArrayProperty(ArrayProperty&& source) = default;
      // ArrayProperty(const ArrayProperty& source) = delete;
      ArrayProperty(const ArrayProperty& source) = default;
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

typedef DInteger::TGBigCellInt<UnsignedBaseStoreTraits,
      DInteger::TGCellIntegerTraits<UnsignedBaseStoreTraits, 2> > GDoubleBigCellInt;

template <>
class TGBigCellInt<UnsignedBaseStoreTraits,
   DInteger::TGCellIntegerTraits<UnsignedBaseStoreTraits,
         DInteger::genable_if_two<sizeof(unsigned long long int)/(sizeof(unsigned))>::value> >
   :  public GDoubleBigCellInt {
  private:
   typedef TGBigCellInt<UnsignedBaseStoreTraits,
         DInteger::TGCellIntegerTraits<UnsignedBaseStoreTraits, 2> > thisType;
   typedef GDoubleBigCellInt inherited;

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
      {  return UnsignedBaseStoreTraits::log_base_2(value); }

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

   TGBigCellInt() : inherited() {}
   TGBigCellInt(unsigned int value) : inherited(value) {}
   TGBigCellInt(const thisType& source) = default;
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

   unsigned int log_base_2() const { return log_base_2(value()); }
   unsigned int getValue() const { return (unsigned int) value(); }
   bool isAtomic() const { return !carray(1); }
   void swap(thisType& source) { inherited::swap(source); }
   void clear() { value() = 0U; }
};

#define DefineGeneric
#define DefineBigIntClass
#include "ForwardNumerics/BaseInteger.inch"
#undef DefineGeneric
#undef DefineBigIntClass

} // end of namespace Numerics

#endif // Numerics_BaseIntegerGenericH

