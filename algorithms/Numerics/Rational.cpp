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
// Unit      : Rational
// File      : Rational.cpp
// Description :
//   Implementation of a class of rationals.
//

#include "Numerics/Rational.h"
// #include "Numerics/Numerics.hpp"

namespace Numerics {

bool
Rational::simplify(const BigInteger& numerator,
      const BigInteger& denominator,
      BigInteger::DivisionResult& result) {
   bool hasSimplifiedAndIsExact = false;
   if ((denominator >= 2) && (numerator >= 2)
         && (numerator.log_base_2() >= denominator.log_base_2())) {
      result = numerator.div(denominator);
      if (result.remainder().isZero())
         hasSimplifiedAndIsExact = true;
   };
   return hasSimplifiedAndIsExact;
}

ComparisonResult
Rational::_compare(const EnhancedObject& asource) const {
   ComparisonResult result = inherited::_compare(asource);
   if (result == CREqual) {
      const Rational& source = (const Rational&) asource;
      if (fNegative != source.fNegative) {
         if (biNumerator.isZero() && source.biNumerator.isZero())
            result = CREqual;
         else
            result = fNegative ? CRLess : CRGreater;
      };

      PNT::PassPointer<BigInteger> fst = biNumerator.mult(source.biDenominator);
      PNT::PassPointer<BigInteger> snd = source.biNumerator.mult(biDenominator);
      result = !fNegative ? fst->compare(*snd) : snd->compare(*fst);
   };
   return result;
}

void
Rational::_read(ISBase& in, const FormatParameters& params) {
   BigInteger numerator, denominator;
   if (params.isText()) {
      int achar;
      while (isspace(achar = in.get()));
      if (achar == '-')
         fNegative = true;
      else if (achar != '+')
         in.unget();
      numerator.read(in, params);
      while (isspace(achar = in.get()));
      if (achar != '/')
         in.unget();
      denominator.read(in, params);
   }
   else { // params.isRaw()
      int achar = in.get();
      if (achar)
         fNegative = true;
      numerator.read(in, params);
      denominator.read(in, params);
   };
   if (denominator.isZero())
      throw STG::EReadError();
   biNumerator.swap(numerator);
   biDenominator.swap(denominator);
   simplify();
}

void
Rational::_write(OSBase& out, const FormatParameters& params) const {
   const_cast<Rational&>(*this).normalize();
   if (params.isText()) {
      if (fNegative)
         out.put('-');
      biNumerator.write(out, params);
      out.put('/');
      biDenominator.write(out, params);
   }
   else { // params.isRaw()
      out.put(fNegative ? (char) 1 : (char) 0);
      biNumerator.write(out, params);
      biDenominator.write(out, params);
   };
}

void
Rational::simplify() {
   BigInteger::DivisionResult result;
   if (simplify(biNumerator, biDenominator, result)) {
      biNumerator = result.quotient();
      biDenominator = 1U;
   };
}

Rational&
Rational::operator+=(const Rational& source) {
   if (fNegative == source.fNegative) {
      if (biDenominator == source.biDenominator)
         biNumerator += source.biNumerator;
      else {
         biNumerator *= source.biDenominator;
         biNumerator += source.biNumerator*biDenominator;
         biDenominator *= source.biDenominator;
      };
   }
   else {
      if (biDenominator == source.biDenominator)
         biNumerator -= source.biNumerator;
      else {
         biNumerator *= source.biDenominator;
         BigInteger temp(source.biNumerator);
         temp *= biDenominator;
         if (biNumerator > temp)
            biNumerator -= source.biNumerator*biDenominator;
         else {
            temp -= biNumerator;
            temp.swap(biNumerator);
            fNegative = !fNegative;
         };
         biDenominator *= source.biDenominator;
      };
   };
   simplify();
   return *this;
}

Rational&
Rational::operator-=(const Rational& source) {
   if (fNegative != source.fNegative) {
      if (biDenominator == source.biDenominator)
         biNumerator += source.biNumerator;
      else {
         biNumerator *= source.biDenominator;
         biNumerator += source.biNumerator*biDenominator;
         biDenominator *= source.biDenominator;
      };
   }
   else {
      if (biDenominator == source.biDenominator)
         biNumerator -= source.biNumerator;
      else {
         biNumerator *= source.biDenominator;
         BigInteger temp(source.biNumerator);
         temp *= biDenominator;
         if (biNumerator > temp)
            biNumerator -= source.biNumerator*biDenominator;
         else {
            temp -= biNumerator;
            temp.swap(biNumerator);
            fNegative = !fNegative;
         };
         biDenominator *= source.biDenominator;
      };
   };
   simplify();
   return *this;
}

Rational&
Rational::operator*=(const Rational& source) {
   BigInteger::DivisionResult divisionResult;
   if (simplify(source.biNumerator, biDenominator, divisionResult)) {
      biDenominator = source.biDenominator;
      biDenominator *= divisionResult.quotient();
      simplify();
   }
   else if (simplify(biNumerator, source.biDenominator, divisionResult)) {
      biNumerator = source.biNumerator;
      biDenominator *= divisionResult.quotient();
   }
   else {
      biNumerator *= source.biNumerator;
      biDenominator *= source.biDenominator;
   };
   fNegative = (fNegative != source.fNegative);
   return *this;
}

Rational&
Rational::operator/=(const Rational& source) {
   AssumeCondition(!source.biNumerator.isZero())
   BigInteger::DivisionResult divisionResult;
   if (simplify(source.biDenominator, biDenominator, divisionResult)) {
      biDenominator = source.biNumerator;
      biDenominator *= divisionResult.quotient();
      simplify();
   }
   else if (simplify(biNumerator, source.biNumerator, divisionResult)) {
      biNumerator = source.biDenominator;
      biDenominator *= divisionResult.quotient();
   }
   else {
      biNumerator *= source.biDenominator;
      biDenominator *= source.biNumerator;
   };
   fNegative = (fNegative != source.fNegative);
   return *this;
}

} // end of namespace Numerics

