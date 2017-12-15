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
// File      : Rational.h
// Description :
//   Definition of a class of rationals.
//

#ifndef Numerics_RationalH
#define Numerics_RationalH

#include "Numerics/Integer.h"

namespace Numerics {

class Rational : public STG::IOObject {
  private:
   typedef STG::IOObject inherited;
   bool fNegative;
   BigInteger biNumerator;
   BigInteger biDenominator;

   static bool simplify(const BigInteger& numerator, const BigInteger& denominator,
         BigInteger::DivisionResult& result);
   void simplify();

  protected:
   virtual ComparisonResult _compare(const EnhancedObject& asource) const override;
   virtual void _read(ISBase& in, const FormatParameters& params) override;
   virtual void _write(OSBase& out, const FormatParameters& params) const override;

  public:
   Rational(int number=0) : fNegative(false), biNumerator(number), biDenominator(1) {}
   Rational(Rational&& source) = default;
   Rational(const Rational& source) = default;
   Rational& operator=(Rational&&) = default;
   Rational& operator=(const Rational&) = default;
   DefineCopy(Rational)
   DDefineAssign(Rational)
   DCompare(Rational)

   Rational operator-() const
      {  Rational result(*this); result.opposite(); return result; }
   Rational operator*(const Rational& source) const
      {  Rational result(*this); result *= source; return result; }
   Rational operator/(const Rational& source) const
      {  Rational result(*this); result /= source; return result; }
   Rational operator+(const Rational& source) const
      {  Rational result(*this); result += source; return result; }
   Rational operator-(const Rational& source) const
      {  Rational result(*this); result -= source; return result; }

   Rational& opposite() { fNegative = !fNegative; return *this; }
   bool isPositive() const { return !fNegative; }
   const BigInteger& numerator() const { return biNumerator; }
   const BigInteger& denominator() const { return biDenominator; }
   Rational& inverse()
      {  AssumeCondition(!biNumerator.isZero());
         biNumerator.swap(biDenominator);
         simplify();
         return *this;
      }
   Rational& operator+=(const Rational& source);
   Rational& operator-=(const Rational& source);
   Rational& operator*=(const Rational& source);
   Rational& operator/=(const Rational& source);

   bool isZero() const { return biNumerator.isZero(); }
   bool isOne() const { return (biNumerator == biDenominator) && !fNegative; }
   bool isInteger() const { const_cast<Rational&>(*this).simplify(); return (biDenominator == 1); }
   int getInteger() const { AssumeCondition(biDenominator == 1) return biNumerator.getValue(); }
   int castToInteger() const
      {  int result = 0;
         if (!biNumerator.isZero()) {
            BigInteger::DivisionResult divisionResult;
            biNumerator.div(biDenominator, divisionResult.setPartial());
            result = divisionResult.quotient().getValue();
         };
         return fNegative ? -result : result;
      }
   void normalize()
      {  if (!biNumerator.isZero()) {
            PNT::PassPointer<BigInteger> pgcd = biNumerator.pgcd(biDenominator);
            biNumerator /= *pgcd;
            biDenominator /= *pgcd;
         };
      }
   virtual bool isValid() const override
      {  return biNumerator.isValid() && biDenominator.isValid() && !biDenominator.isZero(); }
   void swap(Rational& source)
      {  bool temp = fNegative;
         fNegative = source.fNegative;
         source.fNegative = temp;
         biNumerator.swap(source.biNumerator);
         biDenominator.swap(source.biDenominator);
      }
};

} // end of namespace Numerics

#endif // Numerics_RationalH

