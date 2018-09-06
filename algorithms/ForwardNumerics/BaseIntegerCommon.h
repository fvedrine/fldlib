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
// File      : BaseIntegerCommon.h
// Description :
//   Definition of a common part between BaseInteger.h and BaseIntegerGeneric.h
//

#ifndef Numerics_BaseIntegerCommonH
#define Numerics_BaseIntegerCommonH

#ifndef StandardClassesHPP

#include <iostream>
#include <cstring>
#include <assert.h>
#define AssumeUncalled assert(false);
#define AssumeCondition(cond) assert(cond);

namespace STG { namespace IOObject { typedef std::ostream OSBase; typedef std::istream ISBase; }}

enum ComparisonResult { CRLess=-1, CREqual=0, CRGreater=1, CRNonComparable=2 };

#endif

namespace Numerics {

namespace DInteger {

class Access {
  public:
   static int log_base_2(unsigned int value)
      {  int result = 1;
         while ((value >>= 1) != 0)
            ++result;
         return result;
      }
};

} // end of namespace DInteger

class UnsignedBaseStoreTraits {
  public:
   static const int USizeBaseInBits = sizeof(unsigned int)*8;
   typedef unsigned int BaseType;
   typedef unsigned int* BaseTypePointer;
   typedef unsigned int& BaseTypeReference;
   typedef unsigned int BaseTypeConstReference;
   static int log_base_2(unsigned int value) { return DInteger::Access::log_base_2(value); }
   static void clearArray(unsigned int* array, int count)
      {  memset(array, 0, count*sizeof(unsigned int)); }
   static void copyArray(unsigned int* target, const unsigned int* source, int count)
      {  memcpy(target, source, count*sizeof(unsigned int)); }
   template <typename TypeFst, typename TypeSnd>
   static void copyArray(TypeFst* target, const TypeSnd* source, int count)
      {  memcpy(target, source, count*sizeof(unsigned int)); }
   static int sizeBaseInBits() { return 8*sizeof(unsigned); }
   static int minCellsCountToStoreBits(int bitsNumber)
      {  return (int) ((bitsNumber + 8*sizeof(unsigned) - 1) / (8*sizeof(unsigned))); } 

   static void swapArray(unsigned int* target, unsigned int* source, int count)
      {  unsigned int temp;
         for (int i = 0; i < count; ++i) {
            temp = target[i];
            target[i] = source[i];
            source[i] = temp;
         };
      }
   static unsigned int detectCarryAfterAddition(unsigned int result, unsigned int operand)
      {  return (result < operand) ? 1U : 0U; }
   static unsigned int detectCarryBeforeSubstraction(unsigned int first, unsigned int second)
      {  return (first < second) ? 1U : 0U; }
   static unsigned int getStoreMidHighPart(unsigned int value, unsigned int store)
      {  return ((value << (4*sizeof(unsigned int))) | (store & ~(~0U << 4*sizeof(unsigned int)))); }
   static void storeIntoMidHighPart(unsigned int value, unsigned int& store)
      {  store &= ~(~0U << 4*sizeof(unsigned int));
         store |= (value << (4*sizeof(unsigned int)));
      }
   template <typename TypeProperty>
   static void storeIntoMidHighPart(unsigned int value, TypeProperty store)
      {  store &= ~(~0U << 4*sizeof(unsigned int));
         store |= (value << (4*sizeof(unsigned int)));
      }
   static unsigned int getStoreMidLowPart(unsigned int value, unsigned int store)
      {  return ((store & (~0U << 4*sizeof(unsigned int))) | value); }
   static void storeIntoMidLowPart(unsigned int value, unsigned int& store)
      {  store &= (~0U << 4*sizeof(unsigned int));
         store |= value;
      }
   template <typename TypeProperty>
   static void storeIntoMidLowPart(unsigned int value, TypeProperty store)
      {  store &= (~0U << 4*sizeof(unsigned int));
         store |= value;
      }
   static unsigned int getMidHighPart(unsigned int value)
      {  return value >> 4*sizeof(unsigned int); }
   static unsigned int getMidLowPart(unsigned int value)
      {  return value & ~(~0U << 4*sizeof(unsigned int)); }
   static unsigned int getLowPart(unsigned int value, int shift)
      {  return value & ~(~0U << shift); }
   static unsigned int getHighPart(unsigned int value, int shift)
      {  return (value >> shift); }
   static unsigned int getMiddlePart(unsigned int value, int lowBit, int sizeInBits)
      {  return (value >> lowBit) & ~(~0U << sizeInBits); }
   static unsigned int getStoreHighPart(unsigned int value, int shift)
      {  return (value << shift); }
   static unsigned int getStoreHighPart(unsigned int value, int shift, unsigned int store)
      {  return (value << shift) | (store & ~(~0U << shift)); }
   static void storeIntoHighPart(unsigned int value, int shift, unsigned int& store)
      {  store &= ~(~0U << shift);
         store |= (value << shift);
      }
   template <typename TypeProperty>
   static void storeIntoHighPart(unsigned int value, int shift, TypeProperty store)
      {  store &= ~(~0U << shift);
         store |= (value << shift);
      }
   static unsigned int getStoreLowPart(unsigned int value, int shift)
      {  return value & ~(~0U << shift); }
   static unsigned int getStoreLowPart(unsigned int value, int shift, unsigned int store)
      {  return (store & (~0U << shift)) | (value & ~(~0U << shift)); }
   static void storeIntoLowPart(unsigned int value, int shift, unsigned int& store)
      {  store &= (~0U << shift);
         store |= (value & ~(~0U << shift));
      }
   template <typename TypeProperty>
   static void storeIntoLowPart(unsigned int value, int shift, TypeProperty store)
      {  store &= (~0U << shift);
         store |= (value & ~(~0U << shift));
      }
   static void setTrueBit(unsigned int& result, int index) { result |= (1U << index); }
   template <typename TypeProperty>
   static void setTrueBit(TypeProperty result, int index) { result |= (1U << index); }
   static void setFalseBit(unsigned int& result, int index) { result &= ~(1U << index); }
   template <typename TypeProperty>
   static void setFalseBit(TypeProperty result, int index) { result &= ~(1U << index); }
   static void setBit(unsigned int& result, int index, bool value)
      {  if (value)
            result |= (1U << index);
         else
            result &= ~(1U << index);
      }
   template <typename TypeProperty>
   static void setBit(TypeProperty result, int index, bool value)
      {  if (value)
            result |= (1U << index);
         else
            result &= ~(1U << index);
      }
   static bool getBit(unsigned int value, int index)
      {  return (value & (1U << index)) ? true : false; }
   static void leftShiftLocal(unsigned int& value, int index, int shift)
      {  unsigned int temp = value;
         temp &= (~0U << index);
         temp <<= shift;
         value = temp | (value & ~(~0U << index));
      }
   template <typename TypeProperty>
   static void leftShiftLocal(TypeProperty value, int index, int shift)
      {  unsigned int temp = value;
         temp &= (~0U << index);
         temp <<= shift;
         value = temp | (value & ~(~0U << index));
      }
   static void rightShiftLocal(unsigned int& value, int index, int shift)
      {  unsigned int temp = value;
         temp &= (~0U << index);
         temp >>= shift;
         value = temp | (value & ~(~0U << (index-shift)));
      }
   template <typename TypeProperty>
   static void rightShiftLocal(TypeProperty value, int index, int shift)
      {  unsigned int temp = value;
         temp &= (~0U << index);
         temp >>= shift;
         value = temp | (value & ~(~0U << (index-shift)));
      }
   static void leftShiftAndClearLow(unsigned int& value, int shift, int low)
      {  value <<= shift;
         value &= ~0U << low;
      }
   template <typename TypeProperty>
   static void leftShiftAndClearLow(TypeProperty value, int shift, int low)
      {  value <<= shift;
         value &= ~0U << low;
      }
   static void rightShiftAndClearHigh(unsigned int& value, int shift, int high)
      {  value >>= shift;
         value &= ~(~0U << high);
      }
   template <typename TypeProperty>
   static void rightShiftAndClearHigh(TypeProperty value, int shift, int high)
      {  value >>= shift;
         value &= ~(~0U << high);
      }
   static void rightShift(unsigned int& value, int shift) { value >>= shift; }
   template <typename TypeProperty>
   static void rightShift(TypeProperty value, int shift) { value >>= shift; }
   static void leftShift(unsigned int& value, int shift) { value <<= shift; }
   template <typename TypeProperty>
   static void leftShift(TypeProperty value, int shift) { value <<= shift; }
   static void negLowValuePart(unsigned int& value, int shift)
      {  if (shift > 0)
            value = (value & (~0U << shift)) | (~value & ~(~0U << shift));
      }
   template <typename TypeProperty>
   static void negLowValuePart(TypeProperty value, int shift)
      {  if (shift > 0)
            value = (value & (~0U << shift)) | (~value & ~(~0U << shift));
      }
   static void clearLowValuePart(unsigned int& value, int shift)
      {  if (shift > 0)
            value &= (~0U << shift);
      }
   template <typename TypeProperty>
   static void clearLowValuePart(TypeProperty value, int shift)
      {  if (shift > 0)
            value &= (~0U << shift);
      }
   static void clearHighValuePart(unsigned int& value, int shift)
      {  if (shift < (int) (8*sizeof(unsigned)))
            value &= ~(~0U << shift);
      }
   template <typename TypeProperty>
   static void clearHighValuePart(TypeProperty value, int shift)
      {  if (shift < (int) (8*sizeof(unsigned)))
            value &= ~(~0U << shift);
      }
   static unsigned int getSaturation() { return ~0U; }
   static void saturateLowValuePart(unsigned int& value, int shift)
      {  if (shift > 0)
            value |= ~(~0U << shift);
      }
   template <typename TypeProperty>
   static void saturateLowValuePart(TypeProperty value, int shift)
      {  if (shift > 0)
            value |= ~(~0U << shift);
      }
   static bool isZeroValue(unsigned int value) { return value == 0U; }
   static bool hasZeroValue(unsigned int value, int shift)
      {  AssumeCondition(shift <= (int) (8*sizeof(unsigned int)))
         return (shift <= 0) || ((value << (8*sizeof(unsigned int)-shift)) == 0U);
      }
   static bool isOneValue(unsigned int value) { return value == 1U; }
   static void writeValue(STG::IOObject::OSBase& out, unsigned int value, bool isRaw)
#ifdef StandardClassesHPP
      {  out.write(value, isRaw); }
#else
      {  if (!isRaw)
            out << value;
         else
            out.write(reinterpret_cast<char*>(&value), sizeof(value));
      }
#endif
   static void readValue(STG::IOObject::ISBase& in, unsigned int& value, bool isRaw)
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

} // end of namespace Numerics

#endif // Numerics_BaseIntegerCommonH

