/**************************************************************************/
/*                                                                        */
/*  This file is part of FLDLib                                           */
/*                                                                        */
/*  Copyright (C) 2005-2017                                               */
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
// Library   : BackwardNumerics
// Unit      : Floating
// File      : BaseProjectionFloating.h
// Description :
//   Definition of a class of floating points with unbound size.
//

#ifndef Numerics_BaseProjectionFloatingH
#define Numerics_BaseProjectionFloatingH

namespace Numerics {

template <class TypeDouble, class TypeDouble_1>
class TProjectionDouble : public TypeDouble {
  private:
   typedef TProjectionDouble<TypeDouble, TypeDouble_1> thisType;
   typedef TypeDouble inherited;

  public:
   class EnhancedDouble : public TypeDouble_1 {
     private:
      typedef TypeDouble_1 inherited;

     public:
      EnhancedDouble() {}
      EnhancedDouble(const inherited& source) : inherited(source) {}
      EnhancedDouble(const TypeDouble& source)
         {  int lastCellIndex = source.getMantissa().lastCellIndex();
            for (int index = 0; index <= lastCellIndex; ++index)
               inherited::getSMantissa()[index] = source.getMantissa()[index];
            inherited::getSMantissa() <<= 1;
            lastCellIndex = source.getBasicExponent().lastCellIndex();
            for (int index = 0; index <= lastCellIndex; ++index)
               inherited::getSBasicExponent()[index] = source.getBasicExponent()[index];
            inherited::setSign(source.isPositive());
         }
      EnhancedDouble& operator=(const TypeDouble& source)
         {  int lastCellIndex = source.getMantissa().lastCellIndex();
            for (int index = 0; index <= lastCellIndex; ++index)
               inherited::getSMantissa()[index] = source.getMantissa()[index];
            inherited::getSMantissa() <<= 1;
            lastCellIndex = source.getBasicExponent().lastCellIndex();
            for (int index = 0; index <= lastCellIndex; ++index)
               inherited::getSBasicExponent()[index] = source.getBasicExponent()[index];
            inherited::setSign(source.isPositive());
            return *this;
         }
      EnhancedDouble& operator=(const inherited& source)
         {  return (EnhancedDouble&) inherited::operator=(source); }

      void retrieveDouble(TypeDouble& source, typename TypeDouble::ReadParameters& params) const
         {  typename inherited::Mantissa mantissa(inherited::getMantissa());
            typename inherited::Exponent exponent(inherited::getBasicExponent());
            bool isMinor = mantissa.cbitArray(0);
            if (isMinor) {
               bool doesAdd = false;
               if (params.isNearestRound()) {
                  if (params.isDownApproximate())
                     doesAdd = true;
                  else if (!params.isApproximate())
                     doesAdd = params.isRoundToEven() ? mantissa.cbitArray(1) : true;
               }
               else if (params.isHighestRound())
                  doesAdd = inherited::isPositive();
               else if (params.isLowestRound())
                  doesAdd = !inherited::isPositive();

               mantissa >>= 1;
               if (doesAdd) {
                  mantissa.inc();
                  if (mantissa.cbitArray(inherited::bitSizeMantissa()-1))
                     exponent.inc();
               };
               if (inherited::isPositive())
                  params.setApproximate(doesAdd ? TypeDouble::ReadParameters::Up : TypeDouble::ReadParameters::Down);
               else
                  params.setApproximate(doesAdd ? TypeDouble::ReadParameters::Down : TypeDouble::ReadParameters::Up);
            }
            else
               mantissa >>= 1;

            int lastCellIndex = source.getMantissa().lastCellIndex();
            for (int index = 0; index <= lastCellIndex; ++index)
               source.getSMantissa()[index] = (typename TypeDouble::BaseStoreTraits::BaseType) mantissa[index];
            source.getSMantissa().normalize();
            lastCellIndex = source.getBasicExponent().lastCellIndex();
            for (int index = 0; index <= lastCellIndex; ++index)
               source.getSBasicExponent()[index] = (typename TypeDouble::BaseStoreTraits::BaseType) exponent[index];
            source.getSBasicExponent().normalize();
            source.setSign((params.acceptMinusZero() || !source.isZero())
                  ? inherited::isPositive() : true);
         }
   };

  public:
   typedef typename inherited::ReadParameters ReadParameters;

  private:
   bool minusAcceptInfty(const thisType& source, ReadParameters& params, bool& isHandled);
   bool plusAcceptInfty(const thisType& source, ReadParameters& params, bool& isHandled);
   bool divAcceptInfty(const thisType& source, ReadParameters& params, bool& isHandled);
   bool multAcceptInfty(const thisType& source, ReadParameters& params, bool& isHandled);
   
  public:
   TProjectionDouble() {}
   TProjectionDouble(typename TypeDouble::BaseStoreTraits::BaseTypeConstReference value) : inherited(value) {}

   TProjectionDouble(const typename inherited::IntConversion& value, ReadParameters& params)
      :  inherited(value, params) {}
   TProjectionDouble(const typename inherited::FloatConversion& value, ReadParameters& params)
      :  inherited(value, params) {}
   TProjectionDouble(const typename inherited::BuiltDouble& source) : inherited(source) {}

   bool minusAccept(const thisType& source, ReadParameters& params);
   bool plusAccept(const thisType& source, ReadParameters& params);
   bool divAccept(const thisType& source, ReadParameters& params);
   bool multAccept(const thisType& source, ReadParameters& params);

   thisType& sqrtAssign(ReadParameters& params);
   bool squareAccept(ReadParameters& params);
   bool sqrtAccept(ReadParameters& params);
};

} // end of namespace Numerics

#endif // Numerics_BaseProjectionFloatingH

