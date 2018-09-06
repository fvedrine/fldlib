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
// File      : Integer.cpp
// Description :
//   Implementation of a class of integers with unbound size.
//

#include "Numerics/Integer.h"

namespace Numerics {}

#include "Pointer/Vector.template"
#include "Numerics/Integer.template"

namespace Numerics {

void
BigInteger::_read(ISBase& in, const IOObject::FormatParameters& aparams) {
   const FormatParameters& params = (const FormatParameters&) aparams;
   inheritedImplementation::clear();
   int achar;
   if (params.isText()) {
      while (isspace(achar = in.get()));
      if (!params.isDecimal() && !params.isHexaDecimal()) {
         if (params.isBinary()) {
            while (achar == '0')
               achar = in.get();
            unsigned int value = 0;
            if (achar == '1') {
               while ((achar == '0') || (achar == '1')) {
                  value = ((achar == '1') ? 1U : 0U);
                  int bitIndex = 0;
                  while ((++bitIndex < (int) sizeof(unsigned int)*8)
                           && (((achar = in.get()) == '0') || (achar == '1'))) {
                     value <<= 1;
                     if (achar == '1')
                        value |= 1U;
                  };
                  *this <<= bitIndex;
                  inheritedImplementation::operator[](0) |= value;
                  if (bitIndex == (int) sizeof(unsigned int)*8)
                     achar = in.get();
               };
            };
         }
         else { // !params.isBinary() -> params.isIntegerCell()
            while ((achar == ' ') || ((achar >= '0') && (achar <= '9'))) {
               if (achar == ' ')
                  achar = in.get();
               if ((achar >= '0') && (achar <= '9')) {
                  unsigned int value = achar - '0';
                  while (((achar = in.get()) >= '0') && (achar <= '9')) {
                     value *= 10;
                     value += (achar-'0');
                  };
                  *this <<= sizeof(unsigned int)*8;
                  inheritedImplementation::operator[](0) = value;
               };
            };
         };
      }
      else if (params.isDecimal()) {
         while ((achar >= '0') && (achar <= '9')) {
            *this *= 10;
            *this += (achar-'0');
            achar = in.get();
         };
      }
      else if (params.isHexaDecimal()) {
         while (((achar >= '0') && (achar <= '9')) || ((achar >= 'a') && (achar <= 'f'))
               || ((achar >= 'A') && (achar <= 'F'))) {
            *this <<= 4;
            if ((achar >= '0') && (achar <= '9'))
               *this += (achar-'0');
            else if ((achar >= 'a') && (achar <= 'f'))
               *this += (achar-'a') + 10;
            else if ((achar >= 'A') && (achar <= 'F'))
               *this += (achar-'A') + 10;
            achar = in.get();
         };
      };
      if (achar != EOF)
         in.unget();
   }
   else { // params.isRaw()
      bool isRaw = true;
      in.read(achar, isRaw);
      for (int index = 0; index < achar; ++index)
         in.read(inheritedImplementation::operator[](index), isRaw);
   };
}

void
BigInteger::writeFullBinary(OSBase& out, const FormatParameters& params) const {
   int index = (int) ((params.getLength()+sizeof(unsigned int)*8-1) / (sizeof(unsigned int)*8));
   int bitIndex = (int) ((params.getLength()+sizeof(unsigned int)*8-1) % (sizeof(unsigned int)*8))+1;
   
   while (--index >= 0) {
      unsigned int value = array(index) << (sizeof(unsigned int)*8 - bitIndex);
      while (--bitIndex >= 0) {
         out.put(((value & (1U << (sizeof(unsigned int)*8-1))) == 0) ? '0' : '1');
         value <<= 1;
      };
      bitIndex = sizeof(unsigned int)*8;
   };
}

void
BigInteger::writeFullHexaDecimal(OSBase& out, const FormatParameters& params) const {
   int index = (int) ((params.getLength()+sizeof(unsigned int)*8-1) / (sizeof(unsigned int)*8));
   int hexaIndex = (int) ((params.getLength()+sizeof(unsigned int)*8-1) % (sizeof(unsigned int)*2))+1;
   
   while (--index >= 0) {
      unsigned int value = array(index) << (4*(sizeof(unsigned int)*2 - hexaIndex));
      while (--hexaIndex >= 0) {
         unsigned int uShow = (value >> (sizeof(unsigned int)*8-4)) & 15U;
         out.put((uShow < 10) ? (char) (uShow + '0') : (char) (uShow-10 + 'a'));
         value <<= 4;
      };
      hexaIndex = sizeof(unsigned int)*2;
   };
}

void
BigInteger::_write(OSBase& out, const IOObject::FormatParameters& aparams) const {
   const FormatParameters& params = (const FormatParameters&) aparams;
   if (params.isText()) {
      if (params.isDecimal()) {
         unsigned int* decimal = new unsigned int[(log_base_2()-1)/3+1];
         AssumeAllocation(decimal)
         int index = -1;
         thisType thisCopy(*this);
         while (thisCopy != 0U)
            decimal[++index] = thisCopy.divAssign(10).remainder();
         if (index == -1)
            out.put('0');
         ++index;
         while (--index >= 0)
            out.put((char) ('0' + decimal[index]));
         if (decimal)
            delete [] decimal;
      }
      else if (params.isFullFormat()) {
         if (params.isFullBinary())
            writeFullBinary(out, params);
         else if (params.isFullHexaDecimal())
            writeFullHexaDecimal(out, params);
      }
      else if (params.isHexaDecimal()) {
         unsigned int* decimal = new unsigned int[(log_base_2()-1)/4+1];
         AssumeAllocation(decimal)
         int index = -1;
         thisType thisCopy(*this);
         while (thisCopy != 0U)
            decimal[++index] = thisCopy.divAssign(16).remainder();
         if (index == 0)
            out.put('0');
         ++index;
         while (--index >= 0) {
            int value = decimal[index];
            if (0 <= value && value <= 9)
               out.put((char) ('0' + value));
            else
               out.put((char) ('a' + (value-10)));
         };
         if (decimal)
            delete [] decimal;
      }
      else { // !params.isDecimal()
         int index = (int) ((log_base_2()-1)/(sizeof(unsigned int)*8)) + 1;
         while ((index > 0) && (inheritedImplementation::operator[](--index) == 0U));
         if (inheritedImplementation::operator[](index) == 0U)
            out.put('0');
         else {
            if (params.isBinary()) {
               unsigned int value = inheritedImplementation::operator[](index);
               int bitIndex = 0;
               while ((value & (1U << (sizeof(unsigned int)*8-1))) == 0) {
                  value <<= 1;
                  ++bitIndex;
               };
               out.put('1');
               value <<= 1;
               
               while (++bitIndex < (int) sizeof(unsigned int)*8) {
                  out.put(((value & (1U << (sizeof(unsigned int)*8-1))) == 0) ? '0' : '1');
                  value <<= 1;
               };
            }
            else
               out.write(inheritedImplementation::operator[](index), false);
            while (--index >= 0) {
               if (params.isBinary()) {
                  unsigned int value = inheritedImplementation::operator[](index);
                  int bitIndex = sizeof(unsigned int)*8;
                  while (--bitIndex >= 0) {
                     out.put(((value & (1U << (sizeof(unsigned int)*8-1))) == 0) ? '0' : '1');
                     value <<= 1;
                  };
               }
               else
                  out.write(inheritedImplementation::operator[](index), true);
            };
         };
      };
   }
   else { // params.isRaw()
      bool isRaw = true;
      int size = inheritedImplementation::getSize();
      out.write(size, isRaw);
      for (int index = 0; index < size; ++index)
         out.write(inheritedImplementation::operator[](index), isRaw);
   };
}

PNT::PassPointer<BigInteger>
BigInteger::mult(const thisType& source) const {
   Implementation::MultResult multResult;
   inheritedImplementation::mult(source, multResult);
   PNT::PassPointer<BigInteger> result(new BigInteger(), PNT::Pointer::Init());
   result->inheritedImplementation::implementation().swap(multResult);
   result->inheritedImplementation::implementation().normalize();
   return result;
}

BigInteger&
BigInteger::multAssign(const thisType& source) {
   Implementation::MultResult result;
   inheritedImplementation::mult(source, result);
   inheritedImplementation::implementation().swap(result);
   return *this;
}

void
BigInteger::divNormalized(const BigInteger& source, int abitSize, NormalizedDivisionResult& result) const {
   AssumeCondition(abitSize >= 0);
   unsigned bitSize = abitSize;
   AssumeCondition(log_base_2() <= bitSize && source.log_base_2() <= bitSize)
   if (!((bitSize % (8*sizeof(unsigned int))) == 0)) {
      Implementation::DivisionResult atomicResult;
      int quotientCellSize = (int) ((bitSize + 8*sizeof(unsigned int) -1)/(8*sizeof(unsigned int)));
      atomicResult.quotient().adjustSize(quotientCellSize);
      atomicResult.remainder().assertSize((int) (bitSize/(8*sizeof(unsigned int))+1));
      BigIntegerImplementation thisCells, sourceCells;
      int sourceCellSize = (int) (bitSize/(8*sizeof(unsigned int))+1);
      thisCells.assertSize(sourceCellSize);
      sourceCells.assertSize(sourceCellSize);
      for (int numeratorIndex = 0; numeratorIndex < implementation().getSize(); ++numeratorIndex)
         thisCells[numeratorIndex] = implementation()[numeratorIndex];
      thisCells.setTrueBitArray(bitSize);
      for (int denominatorIndex = 0; denominatorIndex < source.implementation().getSize(); ++denominatorIndex)
         sourceCells[denominatorIndex] = source[denominatorIndex];
      sourceCells.setTrueBitArray(bitSize);

      thisCells.div(sourceCells, atomicResult);
      int shift = (int) ((8*sizeof(unsigned int)- ((bitSize+8*sizeof(unsigned int)-1) % (8*sizeof(unsigned int))+1)));
      unsigned int divLeft = atomicResult.quotient()[0] & ~(~0U << shift);
      ((BigIntegerImplementation&) atomicResult.quotient()) >>= shift;

      BigIntegerImplementation::Carry mult = sourceCells.multAssign(divLeft);
      if (sourceCells.add((const BigIntegerImplementation&) atomicResult.remainder()).hasCarry())
         ++mult.carry();
      AssumeCondition(((sourceCells[0] & ~(~0U << shift)) == 0) && (mult.carry() <= 1))
      sourceCells >>= shift;
      if (mult.hasCarry())
         sourceCells[sourceCellSize-1] |= (mult.carry() << ((source.log_base_2()-1) % (8*sizeof(unsigned int) + 1)));

      int additionalShift = implementation().getSize() - quotientCellSize;
      while (--additionalShift >= 0) {
         divLeft = atomicResult.quotient()[0];
         ((BigIntegerImplementation&) atomicResult.quotient()) >>= 8*sizeof(unsigned int);

         BigIntegerImplementation newSource;
         newSource.assertSize(sourceCellSize);
         for (int denominatorIndex = 0; denominatorIndex <= source.implementation().getSize(); ++denominatorIndex)
            newSource[denominatorIndex] = source.implementation()[denominatorIndex];
         newSource.setTrueBitArray(bitSize);
            
         mult = newSource.multAssign(divLeft);
         if (sourceCells.add(newSource).hasCarry())
            ++mult.carry();
         AssumeCondition((sourceCells[0] == 0) && (mult.carry() <= 1))
         sourceCells >>= 8*sizeof(unsigned int);
         if (mult.hasCarry())
            sourceCells[sourceCellSize-1] |= (mult.carry() << ((source.log_base_2()-1) % (8*sizeof(unsigned int) + 1)));
      };
      for (int quotientIndex = 0; quotientIndex < atomicResult.quotient().getSize(); ++quotientIndex)
         result.quotient()[quotientIndex] = (unsigned) atomicResult.quotient()[quotientIndex];
      for (int remainderIndex = 0; remainderIndex < sourceCells.getSize(); ++remainderIndex)
         result.remainder()[remainderIndex] = (unsigned) sourceCells[remainderIndex];
      result.comma() = atomicResult.comma();
   }
   else {
      inheritedImplementation::NormalizedDivisionResult atomicResult;
      int quotientCellSize = (int) ((bitSize + 8*sizeof(unsigned int) -1)/(8*sizeof(unsigned int)));
      atomicResult.quotient().adjustSize(quotientCellSize);
      atomicResult.remainder().assertSize((int) (bitSize/(8*sizeof(unsigned int))+1));
      implementation().divNormalized(source, atomicResult);
      for (int quotientIndex = 0; quotientIndex < atomicResult.quotient().getSize(); ++quotientIndex)
         result.quotient()[quotientIndex] = (unsigned) atomicResult.quotient()[quotientIndex];
      for (int remainderIndex = 0; remainderIndex < atomicResult.remainder().getSize(); ++remainderIndex)
         result.remainder()[remainderIndex] = (unsigned) atomicResult.remainder()[remainderIndex];
      result.comma() = atomicResult.comma();
   };
}

void
BigInteger::div(const thisType& source, DivisionResult& result) const {
   if (inheritedImplementation::operator>=(source)) {
      if ((source.inheritedImplementation::getSize() <= 1)
            && (source.inheritedImplementation::operator[](0) < (1U << 4*sizeof(unsigned int)))) {
         Implementation copy = *this;
         Implementation::AtomicDivisionResult atomicResult = copy.divAssign(source.inheritedImplementation::operator[](0));
         result.ppbiQuotient.absorbElement(new BigInteger());
         copy.implementation().swap(result.ppbiQuotient->inheritedImplementation::implementation());
         if (!result.hasPartialField())
            result.ppbiRemainder.absorbElement(new BigInteger(atomicResult.remainder()));
         if (atomicResult.remainder() != 0U)
            result.mergeRemainderField(1);
         return;
      };
      
      Implementation::DivisionResult implementationResult;
      int size = (int) ((inheritedImplementation::log_base_2()
            - source.inheritedImplementation::log_base_2())/(sizeof(unsigned int)*8)+1);
      implementationResult.quotient().adjustSize(size);
      inheritedImplementation::div(source, implementationResult);
      int shift = (int) (implementationResult.quotient().getSize()*sizeof(unsigned int)*8 - implementationResult.comma());
      AssumeCondition(shift <= (int) (sizeof(unsigned int)*8 + 1))
      if (((const Implementation&) implementationResult.remainder()).isZero()
            && ((const Implementation&) implementationResult.quotient()).hasZero(shift)) {
         ((Implementation&) implementationResult.quotient()) >>= shift;
         ((Implementation&) implementationResult.quotient()).bitArray((int) (size*sizeof(unsigned int)*8-shift)) = true;
      }
      else if (shift > 0) {
         result.mergeRemainderField(1);
         if (!result.hasPartialField()) {
            unsigned int left = implementationResult.quotient()[0];
            if (shift <= (int) sizeof(unsigned int)*8) {
               if (shift < (int) sizeof(unsigned int)*8)
                  left &= ~(~0U << shift);
               Implementation mult(source);
               mult.multAssign(left);
               ((Implementation&) implementationResult.remainder()) += mult;
            }
            else {
               Implementation mult(source);
               mult.multAssign(left);
               ((Implementation&) implementationResult.remainder()) += mult;
               if (implementationResult.quotient()[1] & 1)
                  ((Implementation&) implementationResult.remainder())
                     += (Implementation(source) <<= sizeof(unsigned int)*8);
            };
            AssumeCondition((Implementation().neg(shift)
                  &= ((Implementation&) implementationResult.remainder())).isZero())
            ((Implementation&) implementationResult.remainder()) >>= shift;
         }
         // else {
         //    Implementation newRemainder;
         //    newRemainder.neg(shift);
         //    newRemainder &= (Implementation&) implementationResult.quotient();
         //    newRemainder *= source;
         //    newRemainder += (Implementation&) implementationResult.remainder();
         //    AssumeCondition((Implementation().neg(shift) &= newRemainder).isZero())
         //    newRemainder >>= shift;
         //    ((Implementation&) implementationResult.remainder()) = newRemainder;
         // };
         ((Implementation&) implementationResult.quotient()) >>= shift;
         ((Implementation&) implementationResult.quotient()).bitArray((int) (size*sizeof(unsigned int)*8-shift)) = true;
      };

      result.ppbiQuotient.absorbElement(new BigInteger());
      implementationResult.quotient().swap(result.ppbiQuotient->inheritedImplementation::implementation());
      if (!result.hasPartialField()) {
         result.ppbiRemainder.absorbElement(new BigInteger());
         implementationResult.remainder().swap(result.ppbiRemainder->inheritedImplementation::implementation());
      };
   }
   else {
      result.setRemainderField(!isZero() ? 1 : 0);
      result.ppbiQuotient.absorbElement(new BigInteger());
      if (!result.hasPartialField())
         result.ppbiRemainder.absorbElement(new BigInteger(*this));
   };
}

PNT::PassPointer<BigInteger>
BigInteger::pgcd(const thisType& source) const {
   DivisionResult result;
   PNT::PassPointer<BigInteger> fst, snd;
   if (*this >= source) {
      div(source, result);
      if (!result.hasRemainder())
         return source;
      fst.absorbElement(new BigInteger(source));
      snd = result.ppbiRemainder;
   }
   else {
      source.div(*this, result);
      if (!result.hasRemainder())
         return *this;
      fst.absorbElement(new BigInteger(*this));
      snd = result.ppbiRemainder;
   };

   do {
      do {
         *fst -= *snd;
      } while (*fst >= *snd);
      fst.swap(snd);
   } while (!snd->isZero());
   return fst;
}

void
BigInteger::retrieveBezout(const thisType& source, BezoutResult& result) const { // pgcd = a*u+b*v
   DivisionResult divisionResult;
   PNT::PassPointer<BigInteger> fst, snd;
   BigInteger i,j(1),k(1),l; // u = j et v = l
   // i_{n+1} <- k_n and j_{n+1} <- l_n
   // k_{n+1} <- i_n - q*k_n and l_{n+1} <- j_n - q*l_n

   bool fi=false, fj=true,fk=true,fl=false; // = signes de u et de i,j,k,l
   bool isInverse=false;

   if (*this >= source) {
      div(source, divisionResult);
      if (!divisionResult.hasRemainder()) {
         result.biPgcd = source;
         result.biThisMultiplicand.clear();
         result.biSourceMultiplicand.clear();
         result.biSourceMultiplicand[0] = 1;
         result.fThisPositive = false;
         return;
      };
      fst.absorbElement(new BigInteger(source));
   }
   else {
      source.div(*this, divisionResult);
      if (!divisionResult.hasRemainder()) {
         result.biPgcd = *this;
         result.biSourceMultiplicand.clear();
         result.biThisMultiplicand.clear();
         result.biThisMultiplicand[0] = 1;
         result.fThisPositive = true;
         return;
      };
      fst.absorbElement(new BigInteger(*this));
      isInverse = true;
   };
   snd = divisionResult.ppbiRemainder;
   divisionResult.ppbiQuotient->swap(l);

   do {
      i.swap(k);
      j.swap(l);
      bool temp = fk; fk = fi; fi = temp;
      temp = fj; fj = fl; fl = temp;
      // k_{n+1} <- k_n - i_n and l_{n+1} <- l_n - j_n
      do {
         *fst -= *snd;
         if (!snd->isZero()) {
            if (fk == fi) {
               if (k > i)
                  k -= i;
               else {
                  fk = !fk;
                  BigInteger temp(i);
                  temp -= k;
                  temp.swap(k);
               };
            }
            else
               k += i;
            if (fj == fl) {
               if (l > j)
                  l -= j;
               else {
                  fl = !fl;
                  BigInteger temp(j);
                  temp -= l;
                  temp.swap(l);
               };
            }
            else
               l += j;
         };
      } while (*fst >= *snd);
      fst.swap(snd);
   } while (!snd->isZero());
   result.biPgcd.swap(*fst);
   result.biThisMultiplicand.swap(!isInverse ? i : j);
   result.biSourceMultiplicand.swap(!isInverse ? j : i);
   result.fThisPositive = !isInverse ? fi : !fi;
}

} // end of namespace Numerics

template class Numerics::TBigCellInt<Numerics::DInteger::ExtensibleCellIntegerTraits>;
template class Numerics::DInteger::TBigCellInt<Numerics::DInteger::ExtensibleCellIntegerTraits>;
// template Numerics::DInteger::TBigCellInt<Numerics::DInteger::ExtensibleCellIntegerTraits>::verifyAtomicity() const;
// template Numerics::DInteger::TBigCellInt<Numerics::DInteger::ExtensibleCellIntegerTraits>::operator |=(const Numerics::DInteger::TBigCellInt<Numerics::DInteger::ExtensibleCellIntegerTraits>&);

template class COL::TVector<unsigned int, COL::DVector::TSystemElementTraits<unsigned int> >;

