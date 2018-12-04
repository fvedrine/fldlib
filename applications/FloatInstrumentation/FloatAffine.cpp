/**************************************************************************/
/*                                                                        */
/*  This file is part of FLDLib                                           */
/*                                                                        */
/*  Copyright (C) 2011-2017                                               */
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
// File      : FloatAffine.cpp
// Description :
//   Implementation of a class of affine relations.
//

#include "FloatInstrumentation/FloatAffine.h"

#include "NumericalAnalysis/FloatAffineExecutionPath.template"

#include <fstream>
#include <sstream>
#include <vector>
#include <string>

#include "Pointer/Vector.template"

#include "FloatInstrumentation/FloatAffine.incc"

namespace NumericalDomains { namespace DAffine {

template <> int tfinite(long double val) { return finite(val); }
template <> int tfinite(double val) { return finite(val); }
template <> int tfinite(float val) { return finite(val); }

template <> int tisfinite(long double val) { return isfinite(val); }
template <> int tisfinite(double val) { return isfinite(val); }
template <> int tisfinite(float val) { return isfinite(val); }

template <> int tisnan(long double val) { return isnan(val); }
template <> int tisnan(double val) { return isnan(val); }
template <> int tisnan(float val) { return isnan(val); }

template <> int tisinf(long double val) { return isinf(val); }
template <> int tisinf(double val) { return isinf(val); }
template <> int tisinf(float val) { return isinf(val); }

#ifndef LDBL_EXPONENT_DIG
#define LDBL_EXPONENT_DIG \
      ((LDBL_MAX_EXP == (1 << (16-2))) ? 15 : sizeof(long double)*8-LDBL_MANT_DIG)
#endif

template class TInstrumentedFloatZonotope<23, 8, float>;
template class TInstrumentedFloatZonotope<52, 11, double>;
template class TInstrumentedFloatZonotope<LDBL_MANT_DIG-1, LDBL_EXPONENT_DIG, long double>;
template class TBaseFloatAffine<ExecutionPath>;

template void TBaseFloatAffine<ExecutionPath>::writeCompare<TInstrumentedFloatZonotope<23, 8, float> >(TInstrumentedFloatZonotope<23, 8, float> const&, bool) const;
template void TBaseFloatAffine<ExecutionPath>::writeCompare<TInstrumentedFloatZonotope<52, 11, double> >(TInstrumentedFloatZonotope<52, 11, double> const&, bool) const;
template void TBaseFloatAffine<ExecutionPath>::writeCompare<TInstrumentedFloatZonotope<LDBL_MANT_DIG-1, LDBL_EXPONENT_DIG, long double> >(TInstrumentedFloatZonotope<LDBL_MANT_DIG-1, LDBL_EXPONENT_DIG, long double> const&, bool) const;

template void TBaseFloatAffine<ExecutionPath>::assumeCompare<TInstrumentedFloatZonotope<23, 8, float> >(TInstrumentedFloatZonotope<23, 8, float> const&, bool) const;
template void TBaseFloatAffine<ExecutionPath>::assumeCompare<TInstrumentedFloatZonotope<52, 11, double> >(TInstrumentedFloatZonotope<52, 11, double> const&, bool) const;
template void TBaseFloatAffine<ExecutionPath>::assumeCompare<TInstrumentedFloatZonotope<LDBL_MANT_DIG-1, LDBL_EXPONENT_DIG, long double> >(TInstrumentedFloatZonotope<LDBL_MANT_DIG-1, LDBL_EXPONENT_DIG, long double> const&, bool) const;

#if !defined(FLOAT_GENERIC_BASE_UNSIGNED) && !defined(FLOAT_GENERIC_BASE_LONG)
template class TEquation<FLOAT_REAL_BITS_NUMBER, TBaseFloatAffine<ExecutionPath> >;
template class TEquationServices<FLOAT_REAL_BITS_NUMBER, TBaseFloatAffine<ExecutionPath> >;
template class TBasicFloatZonotope<FLOAT_REAL_BITS_NUMBER, TBaseFloatAffine<ExecutionPath> , 23, 8, float>;
template class TBasicFloatZonotope<FLOAT_REAL_BITS_NUMBER, TBaseFloatAffine<ExecutionPath> , 52, 11, double>;
template class TBasicFloatZonotope<FLOAT_REAL_BITS_NUMBER, TBaseFloatAffine<ExecutionPath> , LDBL_MANT_DIG-1, LDBL_EXPONENT_DIG, long double>;
template void TBaseFloatAffine<ExecutionPath>::writeCompare<TBasicFloatZonotope<FLOAT_REAL_BITS_NUMBER, TBaseFloatAffine<ExecutionPath> , 23, 8, float> >(TBasicFloatZonotope<FLOAT_REAL_BITS_NUMBER, TBaseFloatAffine<ExecutionPath> , 23, 8, float> const&, bool) const;
template void TBaseFloatAffine<ExecutionPath>::writeCompare<TBasicFloatZonotope<FLOAT_REAL_BITS_NUMBER, TBaseFloatAffine<ExecutionPath> , 52, 11, double> >(TBasicFloatZonotope<FLOAT_REAL_BITS_NUMBER, TBaseFloatAffine<ExecutionPath> , 52, 11, double> const&, bool) const;
template void TBaseFloatAffine<ExecutionPath>::writeCompare<TBasicFloatZonotope<FLOAT_REAL_BITS_NUMBER, TBaseFloatAffine<ExecutionPath> , LDBL_MANT_DIG-1, LDBL_EXPONENT_DIG, long double> >(TBasicFloatZonotope<FLOAT_REAL_BITS_NUMBER, TBaseFloatAffine<ExecutionPath> , LDBL_MANT_DIG-1, LDBL_EXPONENT_DIG, long double> const&, bool) const;
template void TBaseFloatAffine<ExecutionPath>::assumeCompare<TBasicFloatZonotope<FLOAT_REAL_BITS_NUMBER, TBaseFloatAffine<ExecutionPath> , 23, 8, float> >(TBasicFloatZonotope<FLOAT_REAL_BITS_NUMBER, TBaseFloatAffine<ExecutionPath> , 23, 8, float> const&, bool) const;
template void TBaseFloatAffine<ExecutionPath>::assumeCompare<TBasicFloatZonotope<FLOAT_REAL_BITS_NUMBER, TBaseFloatAffine<ExecutionPath> , 52, 11, double> >(TBasicFloatZonotope<FLOAT_REAL_BITS_NUMBER, TBaseFloatAffine<ExecutionPath> , 52, 11, double> const&, bool) const;
template void TBaseFloatAffine<ExecutionPath>::assumeCompare<TBasicFloatZonotope<FLOAT_REAL_BITS_NUMBER, TBaseFloatAffine<ExecutionPath> , LDBL_MANT_DIG-1, LDBL_EXPONENT_DIG, long double> >(TBasicFloatZonotope<FLOAT_REAL_BITS_NUMBER, TBaseFloatAffine<ExecutionPath> , LDBL_MANT_DIG-1, LDBL_EXPONENT_DIG, long double> const&, bool) const;
template bool TBasicFloatZonotope<FLOAT_REAL_BITS_NUMBER, TBaseFloatAffine<ExecutionPath>, 23, 8, float>::retrieveConcreteChoicesForConversion<int>(Numerics::DDouble::Access::ReadParameters::RoundMode, ExecutionPath::EquationHolder&, int&, unsigned int&, unsigned int&, unsigned int&, unsigned int&, int&, int&, int&, int&, int&, int&, int&, int&) const;
template bool TBasicFloatZonotope<FLOAT_REAL_BITS_NUMBER, TBaseFloatAffine<ExecutionPath>, 52, 11, double>::retrieveConcreteChoicesForConversion<int>(Numerics::DDouble::Access::ReadParameters::RoundMode, ExecutionPath::EquationHolder&, int&, unsigned int&, unsigned int&, unsigned int&, unsigned int&, int&, int&, int&, int&, int&, int&, int&, int&) const;
template bool TBasicFloatZonotope<FLOAT_REAL_BITS_NUMBER, TBaseFloatAffine<ExecutionPath>, LDBL_MANT_DIG-1, LDBL_EXPONENT_DIG, long double>::retrieveConcreteChoicesForConversion<int>(Numerics::DDouble::Access::ReadParameters::RoundMode, ExecutionPath::EquationHolder&, int&, unsigned int&, unsigned int&, unsigned int&, unsigned int&, int&, int&, int&, int&, int&, int&, int&, int&) const;
template bool TBasicFloatZonotope<FLOAT_REAL_BITS_NUMBER, TBaseFloatAffine<ExecutionPath>, 23, 8, float>::retrieveConcreteChoicesForConversion<unsigned int>(Numerics::DDouble::Access::ReadParameters::RoundMode, ExecutionPath::EquationHolder&, unsigned int&, unsigned int&, unsigned int&, unsigned int&, unsigned int&, unsigned int&, unsigned int&, unsigned int&, unsigned int&, unsigned int&, unsigned int&, unsigned int&, unsigned int&) const;
template bool TBasicFloatZonotope<FLOAT_REAL_BITS_NUMBER, TBaseFloatAffine<ExecutionPath>, 52, 11, double>::retrieveConcreteChoicesForConversion<unsigned int>(Numerics::DDouble::Access::ReadParameters::RoundMode, ExecutionPath::EquationHolder&, unsigned int&, unsigned int&, unsigned int&, unsigned int&, unsigned int&, unsigned int&, unsigned int&, unsigned int&, unsigned int&, unsigned int&, unsigned int&, unsigned int&, unsigned int&) const;
template bool TBasicFloatZonotope<FLOAT_REAL_BITS_NUMBER, TBaseFloatAffine<ExecutionPath>, LDBL_MANT_DIG-1, LDBL_EXPONENT_DIG, long double>::retrieveConcreteChoicesForConversion<unsigned int>(Numerics::DDouble::Access::ReadParameters::RoundMode, ExecutionPath::EquationHolder&, unsigned int&, unsigned int&, unsigned int&, unsigned int&, unsigned int&, unsigned int&, unsigned int&, unsigned int&, unsigned int&, unsigned int&, unsigned int&, unsigned int&, unsigned int&) const;
template int TBasicFloatZonotope<FLOAT_REAL_BITS_NUMBER, TBaseFloatAffine<ExecutionPath>, 23, 8, float>::setConversionResult<int>(Numerics::DDouble::Access::ReadParameters::RoundMode, BaseExecutionPath::Mode, int, bool, bool, int, bool, bool, ExecutionPath::EquationHolder&, int, int, int, int, int, int, int, int);
template int TBasicFloatZonotope<FLOAT_REAL_BITS_NUMBER, TBaseFloatAffine<ExecutionPath>, 52, 11, double>::setConversionResult<int>(Numerics::DDouble::Access::ReadParameters::RoundMode, BaseExecutionPath::Mode, int, bool, bool, int, bool, bool, ExecutionPath::EquationHolder&, int, int, int, int, int, int, int, int);
template int TBasicFloatZonotope<FLOAT_REAL_BITS_NUMBER, TBaseFloatAffine<ExecutionPath>,  LDBL_MANT_DIG-1, LDBL_EXPONENT_DIG, long double>::setConversionResult<int>(Numerics::DDouble::Access::ReadParameters::RoundMode, BaseExecutionPath::Mode, int, bool, bool, int, bool, bool, ExecutionPath::EquationHolder&, int, int, int, int, int, int, int, int);
template unsigned int TBasicFloatZonotope<FLOAT_REAL_BITS_NUMBER, TBaseFloatAffine<ExecutionPath>, 23, 8, float>::setConversionResult<unsigned int>(Numerics::DDouble::Access::ReadParameters::RoundMode, BaseExecutionPath::Mode, unsigned int, bool, bool, unsigned int, bool, bool, ExecutionPath::EquationHolder&, unsigned int, unsigned int, unsigned int, unsigned int, unsigned int, unsigned int, unsigned int, unsigned int);
template unsigned int TBasicFloatZonotope<FLOAT_REAL_BITS_NUMBER, TBaseFloatAffine<ExecutionPath>, 52, 11, double>::setConversionResult<unsigned int>(Numerics::DDouble::Access::ReadParameters::RoundMode, BaseExecutionPath::Mode, unsigned int, bool, bool, unsigned int, bool, bool, ExecutionPath::EquationHolder&, unsigned int, unsigned int, unsigned int, unsigned int, unsigned int, unsigned int, unsigned int, unsigned int);
template unsigned int TBasicFloatZonotope<FLOAT_REAL_BITS_NUMBER, TBaseFloatAffine<ExecutionPath>,  LDBL_MANT_DIG-1, LDBL_EXPONENT_DIG, long double>::setConversionResult<unsigned int>(Numerics::DDouble::Access::ReadParameters::RoundMode, BaseExecutionPath::Mode, unsigned int, bool, bool, unsigned int, bool, bool, ExecutionPath::EquationHolder&, unsigned int, unsigned int, unsigned int, unsigned int, unsigned int, unsigned int, unsigned int, unsigned int);
#elif defined(FLOAT_GENERIC_BASE_LONG)
template class TGEquation<Numerics::UnsignedLongBaseStoreTraits, FLOAT_REAL_BITS_NUMBER, TBaseFloatAffine<ExecutionPath> >;
template class TGEquationServices<Numerics::UnsignedLongBaseStoreTraits, FLOAT_REAL_BITS_NUMBER, TBaseFloatAffine<ExecutionPath> >;
template class TGBasicFloatZonotope<Numerics::UnsignedLongBaseStoreTraits, FLOAT_REAL_BITS_NUMBER, TBaseFloatAffine<ExecutionPath> , 23, 8, float>;
template class TGBasicFloatZonotope<Numerics::UnsignedLongBaseStoreTraits, FLOAT_REAL_BITS_NUMBER, TBaseFloatAffine<ExecutionPath> , 52, 11, double>;
template class TGBasicFloatZonotope<Numerics::UnsignedLongBaseStoreTraits, FLOAT_REAL_BITS_NUMBER, TBaseFloatAffine<ExecutionPath> , LDBL_MANT_DIG-1, LDBL_EXPONENT_DIG, long double>;
template void TBaseFloatAffine<ExecutionPath>::writeCompare<TGBasicFloatZonotope<Numerics::UnsignedLongBaseStoreTraits, FLOAT_REAL_BITS_NUMBER, TBaseFloatAffine<ExecutionPath> , 23, 8, float> >(TGBasicFloatZonotope<Numerics::UnsignedLongBaseStoreTraits, FLOAT_REAL_BITS_NUMBER, TBaseFloatAffine<ExecutionPath> , 23, 8, float> const&, bool) const;
template void TBaseFloatAffine<ExecutionPath>::writeCompare<TGBasicFloatZonotope<Numerics::UnsignedLongBaseStoreTraits, FLOAT_REAL_BITS_NUMBER, TBaseFloatAffine<ExecutionPath> , 52, 11, double> >(TGBasicFloatZonotope<Numerics::UnsignedLongBaseStoreTraits, FLOAT_REAL_BITS_NUMBER, TBaseFloatAffine<ExecutionPath> , 52, 11, double> const&, bool) const;
template void TBaseFloatAffine<ExecutionPath>::writeCompare<TGBasicFloatZonotope<Numerics::UnsignedLongBaseStoreTraits, FLOAT_REAL_BITS_NUMBER, TBaseFloatAffine<ExecutionPath> , LDBL_MANT_DIG-1, LDBL_EXPONENT_DIG, long double> >(TGBasicFloatZonotope<Numerics::UnsignedLongBaseStoreTraits, FLOAT_REAL_BITS_NUMBER, TBaseFloatAffine<ExecutionPath> , LDBL_MANT_DIG-1, LDBL_EXPONENT_DIG, long double> const&, bool) const;
template void TBaseFloatAffine<ExecutionPath>::assumeCompare<TGBasicFloatZonotope<Numerics::UnsignedLongBaseStoreTraits, FLOAT_REAL_BITS_NUMBER, TBaseFloatAffine<ExecutionPath> , 23, 8, float> >(TGBasicFloatZonotope<Numerics::UnsignedLongBaseStoreTraits, FLOAT_REAL_BITS_NUMBER, TBaseFloatAffine<ExecutionPath> , 23, 8, float> const&, bool) const;
template void TBaseFloatAffine<ExecutionPath>::assumeCompare<TGBasicFloatZonotope<Numerics::UnsignedLongBaseStoreTraits, FLOAT_REAL_BITS_NUMBER, TBaseFloatAffine<ExecutionPath> , 52, 11, double> >(TGBasicFloatZonotope<Numerics::UnsignedLongBaseStoreTraits, FLOAT_REAL_BITS_NUMBER, TBaseFloatAffine<ExecutionPath> , 52, 11, double> const&, bool) const;
template void TBaseFloatAffine<ExecutionPath>::assumeCompare<TGBasicFloatZonotope<Numerics::UnsignedLongBaseStoreTraits, FLOAT_REAL_BITS_NUMBER, TBaseFloatAffine<ExecutionPath> , LDBL_MANT_DIG-1, LDBL_EXPONENT_DIG, long double> >(TGBasicFloatZonotope<Numerics::UnsignedLongBaseStoreTraits, FLOAT_REAL_BITS_NUMBER, TBaseFloatAffine<ExecutionPath> , LDBL_MANT_DIG-1, LDBL_EXPONENT_DIG, long double> const&, bool) const;
template bool TGBasicFloatZonotope<Numerics::UnsignedLongBaseStoreTraits, FLOAT_REAL_BITS_NUMBER, TBaseFloatAffine<ExecutionPath>, 23, 8, float>::retrieveConcreteChoicesForConversion<int>(Numerics::DDouble::Access::ReadParameters::RoundMode, ExecutionPath::EquationHolder&, int&, unsigned int&, unsigned int&, unsigned int&, unsigned int&, int&, int&, int&, int&, int&, int&, int&, int&) const;
template bool TGBasicFloatZonotope<Numerics::UnsignedLongBaseStoreTraits, FLOAT_REAL_BITS_NUMBER, TBaseFloatAffine<ExecutionPath>, 52, 11, double>::retrieveConcreteChoicesForConversion<int>(Numerics::DDouble::Access::ReadParameters::RoundMode, ExecutionPath::EquationHolder&, int&, unsigned int&, unsigned int&, unsigned int&, unsigned int&, int&, int&, int&, int&, int&, int&, int&, int&) const;
template bool TGBasicFloatZonotope<Numerics::UnsignedLongBaseStoreTraits, FLOAT_REAL_BITS_NUMBER, TBaseFloatAffine<ExecutionPath>, LDBL_MANT_DIG-1, LDBL_EXPONENT_DIG, long double>::retrieveConcreteChoicesForConversion<int>(Numerics::DDouble::Access::ReadParameters::RoundMode, ExecutionPath::EquationHolder&, int&, unsigned int&, unsigned int&, unsigned int&, unsigned int&, int&, int&, int&, int&, int&, int&, int&, int&) const;
template bool TGBasicFloatZonotope<Numerics::UnsignedLongBaseStoreTraits, FLOAT_REAL_BITS_NUMBER, TBaseFloatAffine<ExecutionPath>, 23, 8, float>::retrieveConcreteChoicesForConversion<unsigned int>(Numerics::DDouble::Access::ReadParameters::RoundMode, ExecutionPath::EquationHolder&, unsigned int&, unsigned int&, unsigned int&, unsigned int&, unsigned int&, unsigned int&, unsigned int&, unsigned int&, unsigned int&, unsigned int&, unsigned int&, unsigned int&, unsigned int&) const;
template bool TGBasicFloatZonotope<Numerics::UnsignedLongBaseStoreTraits, FLOAT_REAL_BITS_NUMBER, TBaseFloatAffine<ExecutionPath>, 52, 11, double>::retrieveConcreteChoicesForConversion<unsigned int>(Numerics::DDouble::Access::ReadParameters::RoundMode, ExecutionPath::EquationHolder&, unsigned int&, unsigned int&, unsigned int&, unsigned int&, unsigned int&, unsigned int&, unsigned int&, unsigned int&, unsigned int&, unsigned int&, unsigned int&, unsigned int&, unsigned int&) const;
template bool TGBasicFloatZonotope<Numerics::UnsignedLongBaseStoreTraits, FLOAT_REAL_BITS_NUMBER, TBaseFloatAffine<ExecutionPath>, LDBL_MANT_DIG-1, LDBL_EXPONENT_DIG, long double>::retrieveConcreteChoicesForConversion<unsigned int>(Numerics::DDouble::Access::ReadParameters::RoundMode, ExecutionPath::EquationHolder&, unsigned int&, unsigned int&, unsigned int&, unsigned int&, unsigned int&, unsigned int&, unsigned int&, unsigned int&, unsigned int&, unsigned int&, unsigned int&, unsigned int&, unsigned int&) const;
template int TGBasicFloatZonotope<Numerics::UnsignedLongBaseStoreTraits, FLOAT_REAL_BITS_NUMBER, TBaseFloatAffine<ExecutionPath>, 23, 8, float>::setConversionResult<int>(Numerics::DDouble::Access::ReadParameters::RoundMode, BaseExecutionPath::Mode, int, bool, bool, int, bool, bool, ExecutionPath::EquationHolder&, int, int, int, int, int, int, int, int);
template int TGBasicFloatZonotope<Numerics::UnsignedLongBaseStoreTraits, FLOAT_REAL_BITS_NUMBER, TBaseFloatAffine<ExecutionPath>, 52, 11, double>::setConversionResult<int>(Numerics::DDouble::Access::ReadParameters::RoundMode, BaseExecutionPath::Mode, int, bool, bool, int, bool, bool, ExecutionPath::EquationHolder&, int, int, int, int, int, int, int, int);
template int TGBasicFloatZonotope<Numerics::UnsignedLongBaseStoreTraits, FLOAT_REAL_BITS_NUMBER, TBaseFloatAffine<ExecutionPath>,  LDBL_MANT_DIG-1, LDBL_EXPONENT_DIG, long double>::setConversionResult<int>(Numerics::DDouble::Access::ReadParameters::RoundMode, BaseExecutionPath::Mode, int, bool, bool, int, bool, bool, ExecutionPath::EquationHolder&, int, int, int, int, int, int, int, int);
template unsigned int TGBasicFloatZonotope<Numerics::UnsignedLongBaseStoreTraits, FLOAT_REAL_BITS_NUMBER, TBaseFloatAffine<ExecutionPath>, 23, 8, float>::setConversionResult<unsigned int>(Numerics::DDouble::Access::ReadParameters::RoundMode, BaseExecutionPath::Mode, unsigned int, bool, bool, unsigned int, bool, bool, ExecutionPath::EquationHolder&, unsigned int, unsigned int, unsigned int, unsigned int, unsigned int, unsigned int, unsigned int, unsigned int);
template unsigned int TGBasicFloatZonotope<Numerics::UnsignedLongBaseStoreTraits, FLOAT_REAL_BITS_NUMBER, TBaseFloatAffine<ExecutionPath>, 52, 11, double>::setConversionResult<unsigned int>(Numerics::DDouble::Access::ReadParameters::RoundMode, BaseExecutionPath::Mode, unsigned int, bool, bool, unsigned int, bool, bool, ExecutionPath::EquationHolder&, unsigned int, unsigned int, unsigned int, unsigned int, unsigned int, unsigned int, unsigned int, unsigned int);
template unsigned int TGBasicFloatZonotope<Numerics::UnsignedLongBaseStoreTraits, FLOAT_REAL_BITS_NUMBER, TBaseFloatAffine<ExecutionPath>,  LDBL_MANT_DIG-1, LDBL_EXPONENT_DIG, long double>::setConversionResult<unsigned int>(Numerics::DDouble::Access::ReadParameters::RoundMode, BaseExecutionPath::Mode, unsigned int, bool, bool, unsigned int, bool, bool, ExecutionPath::EquationHolder&, unsigned int, unsigned int, unsigned int, unsigned int, unsigned int, unsigned int, unsigned int, unsigned int);
#else // defined(FLOAT_GENERIC_BASE_UNSIGNED)
template class TGEquation<Numerics::UnsignedBaseStoreTraits, FLOAT_REAL_BITS_NUMBER, TBaseFloatAffine<ExecutionPath> >;
template class TGEquationServices<Numerics::UnsignedBaseStoreTraits, FLOAT_REAL_BITS_NUMBER, TBaseFloatAffine<ExecutionPath> >;
template class TGBasicFloatZonotope<Numerics::UnsignedBaseStoreTraits, FLOAT_REAL_BITS_NUMBER, TBaseFloatAffine<ExecutionPath> , 23, 8, float>;
template class TGBasicFloatZonotope<Numerics::UnsignedBaseStoreTraits, FLOAT_REAL_BITS_NUMBER, TBaseFloatAffine<ExecutionPath> , 52, 11, double>;
template class TGBasicFloatZonotope<Numerics::UnsignedBaseStoreTraits, FLOAT_REAL_BITS_NUMBER, TBaseFloatAffine<ExecutionPath> , LDBL_MANT_DIG-1, LDBL_EXPONENT_DIG, long double>;
template void TBaseFloatAffine<ExecutionPath>::writeCompare<TGBasicFloatZonotope<Numerics::UnsignedBaseStoreTraits, FLOAT_REAL_BITS_NUMBER, TBaseFloatAffine<ExecutionPath> , 23, 8, float> >(TGBasicFloatZonotope<Numerics::UnsignedBaseStoreTraits, FLOAT_REAL_BITS_NUMBER, TBaseFloatAffine<ExecutionPath> , 23, 8, float> const&, bool) const;
template void TBaseFloatAffine<ExecutionPath>::writeCompare<TGBasicFloatZonotope<Numerics::UnsignedBaseStoreTraits, FLOAT_REAL_BITS_NUMBER, TBaseFloatAffine<ExecutionPath> , 52, 11, double> >(TGBasicFloatZonotope<Numerics::UnsignedBaseStoreTraits, FLOAT_REAL_BITS_NUMBER, TBaseFloatAffine<ExecutionPath> , 52, 11, double> const&, bool) const;
template void TBaseFloatAffine<ExecutionPath>::writeCompare<TGBasicFloatZonotope<Numerics::UnsignedBaseStoreTraits, FLOAT_REAL_BITS_NUMBER, TBaseFloatAffine<ExecutionPath> , LDBL_MANT_DIG-1, LDBL_EXPONENT_DIG, long double> >(TGBasicFloatZonotope<Numerics::UnsignedBaseStoreTraits, FLOAT_REAL_BITS_NUMBER, TBaseFloatAffine<ExecutionPath> , LDBL_MANT_DIG-1, LDBL_EXPONENT_DIG, long double> const&, bool) const;
template void TBaseFloatAffine<ExecutionPath>::assumeCompare<TGBasicFloatZonotope<Numerics::UnsignedBaseStoreTraits, FLOAT_REAL_BITS_NUMBER, TBaseFloatAffine<ExecutionPath> , 23, 8, float> >(TGBasicFloatZonotope<Numerics::UnsignedBaseStoreTraits, FLOAT_REAL_BITS_NUMBER, TBaseFloatAffine<ExecutionPath> , 23, 8, float> const&, bool) const;
template void TBaseFloatAffine<ExecutionPath>::assumeCompare<TGBasicFloatZonotope<Numerics::UnsignedBaseStoreTraits, FLOAT_REAL_BITS_NUMBER, TBaseFloatAffine<ExecutionPath> , 52, 11, double> >(TGBasicFloatZonotope<Numerics::UnsignedBaseStoreTraits, FLOAT_REAL_BITS_NUMBER, TBaseFloatAffine<ExecutionPath> , 52, 11, double> const&, bool) const;
template void TBaseFloatAffine<ExecutionPath>::assumeCompare<TGBasicFloatZonotope<Numerics::UnsignedBaseStoreTraits, FLOAT_REAL_BITS_NUMBER, TBaseFloatAffine<ExecutionPath> , LDBL_MANT_DIG-1, LDBL_EXPONENT_DIG, long double> >(TGBasicFloatZonotope<Numerics::UnsignedBaseStoreTraits, FLOAT_REAL_BITS_NUMBER, TBaseFloatAffine<ExecutionPath> , LDBL_MANT_DIG-1, LDBL_EXPONENT_DIG, long double> const&, bool) const;
template bool TGBasicFloatZonotope<Numerics::UnsignedBaseStoreTraits, FLOAT_REAL_BITS_NUMBER, TBaseFloatAffine<ExecutionPath>, 23, 8, float>::retrieveConcreteChoicesForConversion<int>(Numerics::DDouble::Access::ReadParameters::RoundMode, ExecutionPath::EquationHolder&, int&, unsigned int&, unsigned int&, unsigned int&, unsigned int&, int&, int&, int&, int&, int&, int&, int&, int&) const;
template bool TGBasicFloatZonotope<Numerics::UnsignedBaseStoreTraits, FLOAT_REAL_BITS_NUMBER, TBaseFloatAffine<ExecutionPath>, 52, 11, double>::retrieveConcreteChoicesForConversion<int>(Numerics::DDouble::Access::ReadParameters::RoundMode, ExecutionPath::EquationHolder&, int&, unsigned int&, unsigned int&, unsigned int&, unsigned int&, int&, int&, int&, int&, int&, int&, int&, int&) const;
template bool TGBasicFloatZonotope<Numerics::UnsignedBaseStoreTraits, FLOAT_REAL_BITS_NUMBER, TBaseFloatAffine<ExecutionPath>, LDBL_MANT_DIG-1, LDBL_EXPONENT_DIG, long double>::retrieveConcreteChoicesForConversion<int>(Numerics::DDouble::Access::ReadParameters::RoundMode, ExecutionPath::EquationHolder&, int&, unsigned int&, unsigned int&, unsigned int&, unsigned int&, int&, int&, int&, int&, int&, int&, int&, int&) const;
template bool TGBasicFloatZonotope<Numerics::UnsignedBaseStoreTraits, FLOAT_REAL_BITS_NUMBER, TBaseFloatAffine<ExecutionPath>, 23, 8, float>::retrieveConcreteChoicesForConversion<unsigned int>(Numerics::DDouble::Access::ReadParameters::RoundMode, ExecutionPath::EquationHolder&, unsigned int&, unsigned int&, unsigned int&, unsigned int&, unsigned int&, unsigned int&, unsigned int&, unsigned int&, unsigned int&, unsigned int&, unsigned int&, unsigned int&, unsigned int&) const;
template bool TGBasicFloatZonotope<Numerics::UnsignedBaseStoreTraits, FLOAT_REAL_BITS_NUMBER, TBaseFloatAffine<ExecutionPath>, 52, 11, double>::retrieveConcreteChoicesForConversion<unsigned int>(Numerics::DDouble::Access::ReadParameters::RoundMode, ExecutionPath::EquationHolder&, unsigned int&, unsigned int&, unsigned int&, unsigned int&, unsigned int&, unsigned int&, unsigned int&, unsigned int&, unsigned int&, unsigned int&, unsigned int&, unsigned int&, unsigned int&) const;
template bool TGBasicFloatZonotope<Numerics::UnsignedBaseStoreTraits, FLOAT_REAL_BITS_NUMBER, TBaseFloatAffine<ExecutionPath>, LDBL_MANT_DIG-1, LDBL_EXPONENT_DIG, long double>::retrieveConcreteChoicesForConversion<unsigned int>(Numerics::DDouble::Access::ReadParameters::RoundMode, ExecutionPath::EquationHolder&, unsigned int&, unsigned int&, unsigned int&, unsigned int&, unsigned int&, unsigned int&, unsigned int&, unsigned int&, unsigned int&, unsigned int&, unsigned int&, unsigned int&, unsigned int&) const;
template int TGBasicFloatZonotope<Numerics::UnsignedBaseStoreTraits, FLOAT_REAL_BITS_NUMBER, TBaseFloatAffine<ExecutionPath>, 23, 8, float>::setConversionResult<int>(Numerics::DDouble::Access::ReadParameters::RoundMode, BaseExecutionPath::Mode, int, bool, bool, int, bool, bool, ExecutionPath::EquationHolder&, int, int, int, int, int, int, int, int);
template int TGBasicFloatZonotope<Numerics::UnsignedBaseStoreTraits, FLOAT_REAL_BITS_NUMBER, TBaseFloatAffine<ExecutionPath>, 52, 11, double>::setConversionResult<int>(Numerics::DDouble::Access::ReadParameters::RoundMode, BaseExecutionPath::Mode, int, bool, bool, int, bool, bool, ExecutionPath::EquationHolder&, int, int, int, int, int, int, int, int);
template int TGBasicFloatZonotope<Numerics::UnsignedBaseStoreTraits, FLOAT_REAL_BITS_NUMBER, TBaseFloatAffine<ExecutionPath>,  LDBL_MANT_DIG-1, LDBL_EXPONENT_DIG, long double>::setConversionResult<int>(Numerics::DDouble::Access::ReadParameters::RoundMode, BaseExecutionPath::Mode, int, bool, bool, int, bool, bool, ExecutionPath::EquationHolder&, int, int, int, int, int, int, int, int);
template unsigned int TGBasicFloatZonotope<Numerics::UnsignedBaseStoreTraits, FLOAT_REAL_BITS_NUMBER, TBaseFloatAffine<ExecutionPath>, 23, 8, float>::setConversionResult<unsigned int>(Numerics::DDouble::Access::ReadParameters::RoundMode, BaseExecutionPath::Mode, unsigned int, bool, bool, unsigned int, bool, bool, ExecutionPath::EquationHolder&, unsigned int, unsigned int, unsigned int, unsigned int, unsigned int, unsigned int, unsigned int, unsigned int);
template unsigned int TGBasicFloatZonotope<Numerics::UnsignedBaseStoreTraits, FLOAT_REAL_BITS_NUMBER, TBaseFloatAffine<ExecutionPath>, 52, 11, double>::setConversionResult<unsigned int>(Numerics::DDouble::Access::ReadParameters::RoundMode, BaseExecutionPath::Mode, unsigned int, bool, bool, unsigned int, bool, bool, ExecutionPath::EquationHolder&, unsigned int, unsigned int, unsigned int, unsigned int, unsigned int, unsigned int, unsigned int, unsigned int);
template unsigned int TGBasicFloatZonotope<Numerics::UnsignedBaseStoreTraits, FLOAT_REAL_BITS_NUMBER, TBaseFloatAffine<ExecutionPath>,  LDBL_MANT_DIG-1, LDBL_EXPONENT_DIG, long double>::setConversionResult<unsigned int>(Numerics::DDouble::Access::ReadParameters::RoundMode, BaseExecutionPath::Mode, unsigned int, bool, bool, unsigned int, bool, bool, ExecutionPath::EquationHolder&, unsigned int, unsigned int, unsigned int, unsigned int, unsigned int, unsigned int, unsigned int, unsigned int);
#endif
template class TFloatZonotope<ExecutionPath, 23, 8, float>;
template class TFloatZonotope<ExecutionPath, 52, 11, double>;
template class TFloatZonotope<ExecutionPath, LDBL_MANT_DIG-1, LDBL_EXPONENT_DIG, long double>;

template class TMergeBranches<ExecutionPath>;

} // end of namespace DAffine

template DAffine::TMergeBranches<DAffine::ExecutionPath>&
DAffine::TMergeBranches<DAffine::ExecutionPath>::operator<<(DAffine::TFloatZonotope<ExecutionPath, 23, 8, float>&);

template DAffine::TMergeBranches<DAffine::ExecutionPath>&
DAffine::TMergeBranches<DAffine::ExecutionPath>::operator<<(DAffine::TFloatZonotope<ExecutionPath, 52, 11, double>&);

template DAffine::TMergeBranches<DAffine::ExecutionPath>&
DAffine::TMergeBranches<DAffine::ExecutionPath>::operator<<(DAffine::TFloatZonotope<ExecutionPath, LDBL_MANT_DIG-1, LDBL_EXPONENT_DIG, long double>&);

} // end of namespace NumericalDomains

template class COL::TVector<NumericalDomains::DAffine::TMergeBranches<NumericalDomains::DAffine::ExecutionPath>::HighLevelUpdate, COL::DVector::TElementTraits<NumericalDomains::DAffine::TMergeBranches<NumericalDomains::DAffine::ExecutionPath>::HighLevelUpdate>, COL::DVector::ReallocTraits>;

template class COL::TVector<NumericalDomains::FloatZonotope>;
template class COL::TVector<NumericalDomains::DoubleZonotope>;
template class COL::TVector<NumericalDomains::LongDoubleZonotope>;

template class COL::TVector<NumericalDomains::DAffine::Equation>;
template class COL::TVector<NumericalDomains::FloatZonotope::BuiltDouble>;
template class COL::TVector<NumericalDomains::DoubleZonotope::BuiltDouble>;
template class COL::TVector<NumericalDomains::LongDoubleZonotope::BuiltDouble>;
