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
// Library   : Numerics
// Unit      : Integer
// File      : Integer.h
// Description :
//   Definition of a class of integers with unbound size.
//

#ifndef Numerics_IntegerH
#define Numerics_IntegerH

#include "StandardClasses/Persistence.h"
#include "Pointer/Vector.h"
#include "Pointer/Pointer.hpp"

#include "ForwardNumerics/BaseInteger.h"

namespace Numerics {

/***********************************/
/* Definition of the class Integer */
/***********************************/

namespace DInteger {

class ExtensibleCellIntegerTraits {
  private:
   typedef ExtensibleCellIntegerTraits thisType;
   typedef COL::TVector<unsigned int, COL::DVector::TSystemElementTraits<unsigned int> > UnsignedIntVector;
   UnsignedIntVector viIntArray;

   void bookPlace(int places=1) const
      {  if (places > 0) {
            const_cast<UnsignedIntVector&>(viIntArray).bookPlace(places);
            while (--places >= 0)
               const_cast<UnsignedIntVector&>(viIntArray).insertAtEnd(0);
         };
      }
   
  public:
   ExtensibleCellIntegerTraits() : viIntArray() {}
   ExtensibleCellIntegerTraits(ExtensibleCellIntegerTraits&&) = default;
   ExtensibleCellIntegerTraits(const ExtensibleCellIntegerTraits&) = default;
   ExtensibleCellIntegerTraits& operator=(ExtensibleCellIntegerTraits&& source)
      {  viIntArray = source.viIntArray; return *this; }
   ExtensibleCellIntegerTraits& operator=(const ExtensibleCellIntegerTraits& source)
      {  viIntArray = source.viIntArray;
         normalize();
         return *this;
      }

   void assignCells(const ExtensibleCellIntegerTraits& source) { operator=(source); }
   void assignCells(ExtensibleCellIntegerTraits&& source) { operator=(source); }
   typedef unsigned int& ArrayProperty;
   ArrayProperty array(int index)
      {  if (index >= viIntArray.count())
            bookPlace(index-viIntArray.count()+1);
         return viIntArray.referenceAt(index);
      }
   unsigned int array(int index) const
      {  return (index < viIntArray.count()) ? viIntArray[index] : 0U; }
   unsigned int carray(int index) const { return array(index); }
   ArrayProperty operator[](int index)
      {  if (index >= viIntArray.count())
            bookPlace(index-viIntArray.count()+1);
         return viIntArray.referenceAt(index);
      }
   unsigned int operator[](int index) const
      {  return (index < viIntArray.count()) ? viIntArray[index] : 0U; }
   typedef thisType MultResult;

   typedef thisType QuotientResult;
   typedef thisType RemainderResult;
   typedef thisType NormalizedRemainderResult;
   void copyLow(const MultResult& result) { viIntArray = result.viIntArray; }
   int getSize() const { return viIntArray.count(); }
   void adjustSize(int newSize)
      {  int newCells = newSize-viIntArray.count();
         if (newCells > 0)
            bookPlace(newCells);
         else if (newCells < 0) {
            while (++newCells <= 0)
               viIntArray.removeAtEnd();
         };
      }
   void normalize()
      {  int count = viIntArray.count();
         while ((--count >= 0) && (viIntArray[count] == 0))
            viIntArray.removeAtEnd();
      }
   void assertSize(int newSize) { adjustSize(newSize); }
   void setSize(int exactSize) { if (exactSize < viIntArray.count()) viIntArray.removeAllBetween(exactSize, -1); }
   void setBitSize(int exactSize)
      {  int size = (int) ((exactSize + 8*sizeof(unsigned)-1)/(8*sizeof(unsigned)));
         int count = viIntArray.count();
         if (size > count)
            bookPlace(size-count);
         else if (size < count)
            viIntArray.removeAllBetween(size, -1);
         if ((exactSize % (8*sizeof(unsigned)) != 0) && viIntArray.elementAt(size-1))
            viIntArray.referenceAt(size-1) &= ~(~0U << (exactSize % (8*sizeof(unsigned))));
      }
   void setCellSize(int exactSize) { if (exactSize < viIntArray.count()) viIntArray.removeAllBetween(exactSize, -1); }
   void clear() { viIntArray.removeAll(); }
   void swap(thisType& source) { viIntArray.swap(source.viIntArray); }
   typedef thisType ExtendedInteger;
};

class BigIntegerImplementation : public TBigCellInt<ExtensibleCellIntegerTraits> {
  private:
   typedef TBigCellInt<ExtensibleCellIntegerTraits> inherited;

  public:
   BigIntegerImplementation() : inherited() {}
   BigIntegerImplementation(unsigned int value) : inherited(value) {}
   BigIntegerImplementation(BigIntegerImplementation&& source) = default;
   BigIntegerImplementation(const BigIntegerImplementation& source) = default;
   BigIntegerImplementation& operator=(BigIntegerImplementation&& source)
      {  return (BigIntegerImplementation&) inherited::operator=(source); }
   BigIntegerImplementation& operator=(const BigIntegerImplementation& source)
      {  return (BigIntegerImplementation&) inherited::operator=(source); }
   BigIntegerImplementation& operator=(unsigned int value)
      {  return (BigIntegerImplementation&) inherited::operator=(value); }

   void adjustSize(int newSize) { inherited::adjustSize(newSize); }
   void assertSize(int newSize) { inherited::adjustSize(newSize); }
   void normalize() { inherited::normalize(); }
   void setSize(int exactSize) { inherited::setSize(exactSize); }
   void setBitSize(int exactSize) { inherited::setBitSize(exactSize); }
   void setCellSize(int exactSize) { inherited::setCellSize(exactSize); }
};

} // end of namespace DInteger

class BigInteger : public STG::IOObject, public DInteger::BigIntegerImplementation {
  public:
   typedef DInteger::BigIntegerImplementation Implementation;
   typedef UnsignedBaseStoreTraits BaseStoreTraits;

  private:
   typedef STG::IOObject inherited;
   typedef DInteger::BigIntegerImplementation inheritedImplementation;
   typedef BigInteger thisType;

  protected:
   virtual ComparisonResult _compare(const EnhancedObject& asource) const override
      {  ComparisonResult result = inherited::_compare(asource);
         return (result == CREqual)
            ? inheritedImplementation::compare(((const BigInteger&) asource)) : result;
      }

  public:
   class FormatParameters : public STG::IOObject::FormatParameters {
     private:
      typedef STG::IOObject::FormatParameters inherited;
      enum Type { TDecimal, THexaDecimal, TFullHexaDecimal, TIntegerCell, TBinary, TFullBinary };
      int uLength;

     protected:
      DefineExtendedParameters(3, inherited)

     public:
      FormatParameters() : uLength(0) {}
      FormatParameters(const FormatParameters& source) = default;
      FormatParameters& operator=(const FormatParameters& source) = default;

      bool isLightBinary() const { return isText() && (queryOwnField() == TBinary); }
      bool isFullBinary() const { return isText() && (queryOwnField() == TFullBinary); }
      bool isBinary() const
         {  Type type = (Type) queryOwnField();
            return isText() && (type == TBinary || type == TFullBinary);
         }
      bool isInternal() const { return isRaw(); }
      bool isIntegerCell() const { return isText() && (queryOwnField() == TIntegerCell); }
      bool isDecimal() const { return isText() && (queryOwnField() == TDecimal); }
      bool isLightHexaDecimal() const { return isText() && (queryOwnField() == THexaDecimal); }
      bool isFullHexaDecimal() const { return isText() && (queryOwnField() == TFullHexaDecimal); }
      bool isHexaDecimal() const
         {  Type type = (Type) queryOwnField();
            return isText() && (type == THexaDecimal || type == TFullHexaDecimal);
         }
      bool isFullFormat() const
         {  Type type = (Type) queryOwnField();
            return (type == TFullBinary || type == TFullHexaDecimal);
         }
      const int& getLength() const { AssumeCondition(isFullFormat()) return uLength; }

      FormatParameters& setText() { return (FormatParameters&) inherited::setText(); }
      FormatParameters& setRaw() { clearOwnField(); return (FormatParameters&) inherited::setRaw(); }
      FormatParameters& setInternal() { clearOwnField(); return (FormatParameters&) inherited::setRaw(); }
      FormatParameters& setBinary() { setText(); setOwnField(TBinary); return *this; }
      FormatParameters& setIntegerCell() { setText(); setOwnField(TIntegerCell); return *this; }
      FormatParameters& setDecimal() { setText(); setOwnField(TDecimal); return *this; }
      FormatParameters& setHexaDecimal() { setText(); setOwnField(THexaDecimal); return *this; }
   };

  protected:
   void writeFullBinary(STG::IOObject::OSBase& out, const FormatParameters& params) const;
   void writeFullHexaDecimal(STG::IOObject::OSBase& out, const FormatParameters& params) const;
   virtual void _read(ISBase& in, const IOObject::FormatParameters& params) override;
   virtual void _write(OSBase& out, const IOObject::FormatParameters& params) const override;

  public:
   BigInteger() : inheritedImplementation() {}
   BigInteger(unsigned int value) : inheritedImplementation(value) {}
   BigInteger(BigInteger&) = default;
   BigInteger(const BigInteger& source) = default;
   DefineCopy(BigInteger)
   DDefineAssign(BigInteger)
   DCompare(BigInteger)
   BigInteger& operator=(unsigned int value) { inheritedImplementation::operator=(value); return *this; }
   BigInteger& operator=(BigInteger&& source) = default;
   BigInteger& operator=(const BigInteger& source) = default;
 
   void read(ISBase& in, const STG::IOObject::FormatParameters& params) { _read(in, params); }
   void write(OSBase& out, const STG::IOObject::FormatParameters& params) const { _write(out, params); }
   void read(std::istream& in, const STG::IOObject::FormatParameters& params) { ISStream instream(in); _read(instream, params); }
   void read(std::istream& in) { ISStream instream(in); _read(instream, FormatParameters()); }
   void write(std::ostream& out, const STG::IOObject::FormatParameters& params) const { OSStream outstream(out); _write(outstream, params); }
   void write(std::ostream& out) const { OSStream outstream(out); _write(outstream, FormatParameters()); }

   void add(const thisType& source)
      {  Implementation::Carry carry = inheritedImplementation::add(source);
         if (carry.hasCarry())
            inheritedImplementation::operator[](inheritedImplementation::getSize()) = 1U;
      }
   void sub(const thisType& source)
      {  AssumeCondition(inheritedImplementation::operator>=(source))
         inheritedImplementation::sub(source);
         inheritedImplementation::implementation().normalize();
      }
   thisType& plusAssign(const thisType& source) { add(source); return *this; }
   thisType& minusAssign(const thisType& source) { sub(source); return *this; }
   thisType& inc()
      {  Implementation::Carry carry = inheritedImplementation::inc();
         if (carry.hasCarry())
            inheritedImplementation::operator[](inheritedImplementation::getSize()) = 1U;
         return *this;
      }
   thisType& dec()
      {  AssumeCondition(!inheritedImplementation::isZero())
         inheritedImplementation::dec();
         inheritedImplementation::implementation().normalize();
         return *this;
      }
   thisType& neg(int shift)
      {  inheritedImplementation::assertSize((int) ((shift+8*sizeof(unsigned int)-1)/(8*sizeof(unsigned int))));
         inheritedImplementation::neg(shift);
         return *this;
      }
   thisType& clear(int shift)
      {  inheritedImplementation::assertSize((int) ((shift+8*sizeof(unsigned int)-1)/(8*sizeof(unsigned int))));
         inheritedImplementation::clear(shift);
         return *this;
      }
   thisType& clearHigh(int shift)
      {  inheritedImplementation::setBitSize(shift);
         return *this;
      }
   thisType& saturate(int shift)
      {  inheritedImplementation::assertSize((int) ((shift+8*sizeof(unsigned int)-1)/(8*sizeof(unsigned int))));
         inheritedImplementation::saturate(shift);
         return *this;
      }

   thisType& operator<<=(int shift)
      {  inheritedImplementation::assertSize((int) ((log_base_2()+shift+8*sizeof(unsigned int)-1)/(8*sizeof(unsigned int))));
         inheritedImplementation::operator<<=(shift);
         return *this;
      }
   thisType& operator>>=(int shift) { inheritedImplementation::operator>>=(shift); return *this; }
   void leftShiftLocal(int index, int shift)
      {  inheritedImplementation::assertSize((int) ((log_base_2()+shift+8*sizeof(unsigned int)-1)/(8*sizeof(unsigned int))));
         inheritedImplementation::leftShiftLocal(shift, index);
      }
   void rightShiftLocal(int index, int shift)
      {  inheritedImplementation::rightShiftLocal(shift, index); }
   thisType operator<<(int shift)
      {  BigInteger result(*this); result <<= shift; return result; }
   thisType operator>>(int shift)
      {  BigInteger result(*this); result >>= shift; return result; }
   thisType& operator|=(const thisType& source) { inheritedImplementation::operator|=(source); return *this; }
   thisType& operator&=(const thisType& source) { inheritedImplementation::operator&=(source); return *this; }
   thisType& operator^=(const thisType& source) { inheritedImplementation::operator^=(source); return *this; }
   
   thisType& operator+=(const thisType& source) { add(source); return *this; }
   thisType& operator-=(const thisType& source) { sub(source); return *this; }
   thisType& operator--() { dec(); return *this; }
   thisType& operator++() { inc(); return *this; }
   PNT::PassPointer<BigInteger> mult(const thisType& source) const;
   thisType& multAssign(unsigned int source)
      {  Implementation::Carry carry = inheritedImplementation::multAssign(source);
         if (carry.hasCarry())
            inheritedImplementation::operator[](inheritedImplementation::getSize()) = carry.carry();
         return *this;
      }
   thisType& multAssign(const thisType& source);
   thisType& operator*=(const thisType& source) { return multAssign(source); }
   thisType& operator*=(unsigned int source) { return multAssign(source); }
   const Implementation& implementation() const { return *this; }
   Implementation& implementation() { return *this; }

   class AtomicDivisionResult {
     private:
      unsigned int uRemainder;
      friend class BigInteger;

     public:
      AtomicDivisionResult() : uRemainder(0) {}
      AtomicDivisionResult(const AtomicDivisionResult&) = default;
      const unsigned int& remainder() const { return uRemainder; }
   };

   class DivisionResult : protected ExtendedParameters {
     private:
      typedef ExtendedParameters inherited;
      PNT::PassPointer<BigInteger> ppbiQuotient;
      PNT::PassPointer<BigInteger> ppbiRemainder;
      friend class BigInteger;

     protected:
      DefineExtendedParameters(2, ExtendedParameters)
      DefineSubExtendedParameters(Partial, 1, INHERITED)
      DefineSubExtendedParameters(Remainder, 1, Partial)

     public:
      DivisionResult() : inherited() {}
      DivisionResult(const DivisionResult& source) = default;
      DivisionResult& setPartial() { mergePartialField(1); return *this; }
      DivisionResult& setTotal() { clearPartialField(); return *this; }

      bool hasRemainder() const { return hasRemainderField(); }
      bool isValid() const
         { return ppbiQuotient.isValid() && (hasPartialField() || ppbiRemainder.isValid()); }

      BigInteger& remainder() const { return *ppbiRemainder; }
      BigInteger& quotient() const { return *ppbiQuotient; }
   };
  public:
   void divNormalized(const thisType& source, int bitSize, NormalizedDivisionResult& result) const;
   void div(const thisType& source, DivisionResult& result) const;
   DivisionResult div(const thisType& source) const
      {  DivisionResult result;
         div(source, result); return result;
      }
   AtomicDivisionResult divAssign(unsigned int source)
      {  Implementation::AtomicDivisionResult iresult = inheritedImplementation::divAssign(source);
         AtomicDivisionResult result;
         result.uRemainder = iresult.remainder();
         return result;
      }
   thisType& divAssign(const thisType& source)
      {  DivisionResult result;
         div(source, result.setPartial());
         inheritedImplementation::swap(result.quotient());
         return *this;
      }
   thisType& operator/=(const thisType& source) { return divAssign(source); }
   thisType& operator/=(unsigned int source) { divAssign(source); return *this; }
   unsigned int operator%(unsigned int source) const
      { return thisType(*this).divAssign(source).remainder(); }
   thisType& operator%=(const thisType& source)
      {  DivisionResult result;
         div(source, result);
         inheritedImplementation::swap(result.remainder());
         return *this;
      }
   unsigned int log_base_2() const { return inheritedImplementation::log_base_2(); }
   PNT::PassPointer<BigInteger> pgcd(const thisType& source) const;
   PNT::PassPointer<BigInteger> ppcm(const thisType& source) const
      {  BigInteger mult(*this);
         mult *= source;
         PNT::PassPointer<BigInteger> result = pgcd(source);
         DivisionResult divisionResult;
         mult.div(*result, divisionResult.setPartial());
         return divisionResult.ppbiQuotient;
      }
   class BezoutResult;
   void retrieveBezout(const thisType& source, BezoutResult& result) const;
   
   thisType operator+(const thisType& source) const
      {  thisType result(*this); result.add(source); return result; }
   thisType operator-(const thisType& source) const
      {  thisType result(*this); result.sub(source); return result; }
   thisType operator*(const thisType& source) const { return *mult(source); }
   thisType operator/(const thisType& source) const
      {  DivisionResult result;
         div(source, result.setPartial());
         return result.quotient();
      }

   unsigned int getValue() const { return inheritedImplementation::getValue(); }
   bool isAtomic() const { return inheritedImplementation::isAtomic(); }
   bool isZero() const { return inheritedImplementation::isZero(); }
   bool hasZero(unsigned int nb) const { return inheritedImplementation::hasZero(nb); }
   void swap(thisType& source) { inheritedImplementation::swap(source); }

   int getBitSize() const { return (int) (inheritedImplementation::getSize()*8*sizeof(unsigned int)); }
   void adjustBitSize(int newSize) { inheritedImplementation::adjustSize((int) ((newSize + 8*sizeof(unsigned int) - 1)/(8*sizeof(unsigned int)))); }
   void assertBitSize(int newSize) { inheritedImplementation::assertSize((int) ((newSize + 8*sizeof(unsigned int) - 1)/(8*sizeof(unsigned int)))); }
   void setBitSize(int exactSize) { inheritedImplementation::setBitSize(exactSize); }
   unsigned int getValueAt(int bitFirst, int bitLength);
   void setValueAt(int bitFirst, int bitLength, unsigned int value);
   
   template <class TypeInt> void retrieveBValueAt(int bitFirst, int bitLength, TypeInt& value) const;
   template <class TypeInt> void setBValueAt(int bitFirst, int bitLength, const TypeInt& value);

   int getSize() const { return inheritedImplementation::getSize(); }
   void adjustSize(int newSize) { inheritedImplementation::adjustSize(newSize); }
   void normalize() { inheritedImplementation::normalize(); }
   void assertSize(int newSize) { inheritedImplementation::assertSize(newSize); }
   void setSize(int exactSize) { inheritedImplementation::setSize(exactSize); }
   void setCellSize(int exactSize) { inheritedImplementation::setCellSize(exactSize); }
   void clear() { inheritedImplementation::clear(); }

   typedef Implementation::ArrayProperty ArrayProperty;
   ArrayProperty array(int index) { return inheritedImplementation::array(index); }
   unsigned int array(int index) const { return inheritedImplementation::array(index); }
   unsigned int carray(int index) const { return inheritedImplementation::carray(index); }
   ArrayProperty operator[](int index) { return inheritedImplementation::operator[](index); }
   unsigned int operator[](int index) const { return inheritedImplementation::operator[](index); }

   typedef Implementation::MidArray MidArray;
   MidArray midArray(int index) { return inheritedImplementation::midArray(index); }
   unsigned int midArray(int index) const { return inheritedImplementation::midArray(index); }
   unsigned int cmidArray(int index) const { return midArray(index); }
   void setMidArray(int index, unsigned int value) { inheritedImplementation::setMidArray(index, value); }

   typedef Implementation::BitArray BitArray;
   BitArray bitArray(int index) { return inheritedImplementation::bitArray(index); }
   bool bitArray(int index) const { return inheritedImplementation::bitArray(index); }
   bool cbitArray(int index) const { return bitArray(index); }

   void setBitArray(int index, bool value) { inheritedImplementation::setBitArray(index, value); }
   void setTrueBitArray(int index) { inheritedImplementation::setTrueBitArray(index); }
   void setFalseBitArray(int index) { inheritedImplementation::setFalseBitArray(index); }
};

inline unsigned int
BigInteger::getValueAt(int bitFirst, int bitLength) {
   TBigCellInt<DInteger::TCellIntegerTraits<2> > value;
   int arrayIndex = (int) (bitFirst/(8*sizeof(unsigned int)));
   value[0] = carray(arrayIndex);
   value[1] = carray(arrayIndex+1);
   value >>= (int) ((bitFirst - arrayIndex*(8*sizeof(unsigned int))));
   TBigCellInt<DInteger::TCellIntegerTraits<2> > mask = 0U;
   (mask.neg() <<= bitLength).neg();
   value &= mask;
   return value[0];
}

inline void
BigInteger::setValueAt(int bitFirst, int bitLength, unsigned int value) {
   assertBitSize(bitFirst + bitLength);
   TBigCellInt<DInteger::TCellIntegerTraits<2> > valueCells;
   int arrayIndex = (int) (bitFirst/(8*sizeof(unsigned int)));
   valueCells[0] = carray(arrayIndex);
   valueCells[1] = carray(arrayIndex+1);
   TBigCellInt<DInteger::TCellIntegerTraits<2> > mask = 0U;
   int shift = (int) ((bitFirst - arrayIndex*(8*sizeof(unsigned int))));
   ((mask.neg() <<= bitLength).neg() <<= shift).neg();
   valueCells &= mask;
   (mask = value) <<= shift;
   valueCells |= mask;
   array(arrayIndex) = valueCells[0];
   array(arrayIndex+1) = valueCells[1];
}

class BigInteger::BezoutResult {
  private:
   BigInteger biPgcd;
   BigInteger biThisMultiplicand;
   BigInteger biSourceMultiplicand;
   bool fThisPositive;

  public:
   BezoutResult() : fThisPositive(true) {}
   BezoutResult(BezoutResult&&) = default;
   BezoutResult(const BezoutResult&) = default;

   BigInteger& pgcd() { return biPgcd; }
   BigInteger& sourceMultiplicand() { return biSourceMultiplicand; }
   BigInteger& multiplicand() { return biThisMultiplicand; }
   bool isThisPositive() const { return fThisPositive; }
   bool isSourcePositive() const { return !fThisPositive; }
   friend class BigInteger;
};

} // end of namespace Numerics

#endif // Numerics_IntegerH

