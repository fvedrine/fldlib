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
// Unit      : FloatInterval
// File      : FloatInterval.cpp
// Description :
//   Implementation of a class of floating point intervals
//

#include "FloatInstrumentation/FloatInterval.h"

#include "NumericalAnalysis/FloatIntervalExecutionPath.template"

#include <fstream>
#include <sstream>

#include "Pointer/Vector.template"

#include "FloatInstrumentation/FloatInterval.incc"

namespace NumericalDomains { namespace DDoubleInterval {

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

typedef FloatDigitsHelper::TFloatDigits<long double> LongDoubleFloatDigits;

template class TCompareFloatInterval<LongDoubleFloatDigits::UBitSizeMantissa+1,
         TBaseFloatInterval<ExecutionPath>, BuiltFloat, float>;
template class TCompareFloatInterval<LongDoubleFloatDigits::UBitSizeMantissa+1,
         TBaseFloatInterval<ExecutionPath>, BuiltDouble, double>;
template class TCompareFloatInterval<LongDoubleFloatDigits::UBitSizeMantissa+1,
         TBaseFloatInterval<ExecutionPath>, BuiltLongDouble, long double>;

template class TInstrumentedFloatInterval<BuiltFloat, float>;
template class TInstrumentedFloatInterval<BuiltDouble, double>;
template class TInstrumentedFloatInterval<BuiltLongDouble, long double>;

// template class TBaseFloatInterval<TBaseFloatInterval<ExecutionPath> >;
template class TBaseFloatInterval<ExecutionPath>;

template void TBaseFloatInterval<ExecutionPath>::assumeInterval<TCompareFloatInterval<LongDoubleFloatDigits::UBitSizeMantissa+1, TBaseFloatInterval<ExecutionPath>, TBuiltFloat<LongDoubleFloatDigits::UBitSizeMantissa+1, 23, 8>, float> >(TCompareFloatInterval<LongDoubleFloatDigits::UBitSizeMantissa+1, TBaseFloatInterval<ExecutionPath>, TBuiltFloat<LongDoubleFloatDigits::UBitSizeMantissa+1, 23, 8>, float> const&) const;
template void TBaseFloatInterval<ExecutionPath>::assumeInterval<TCompareFloatInterval<LongDoubleFloatDigits::UBitSizeMantissa+1, TBaseFloatInterval<ExecutionPath>, TBuiltFloat<LongDoubleFloatDigits::UBitSizeMantissa+1, 52, 11>, double> >(TCompareFloatInterval<LongDoubleFloatDigits::UBitSizeMantissa+1, TBaseFloatInterval<ExecutionPath>, TBuiltFloat<LongDoubleFloatDigits::UBitSizeMantissa+1, 52, 11>, double> const&) const;
template void TBaseFloatInterval<ExecutionPath>::assumeInterval<TCompareFloatInterval<LongDoubleFloatDigits::UBitSizeMantissa+1, TBaseFloatInterval<ExecutionPath>, TBuiltFloat<LongDoubleFloatDigits::UBitSizeMantissa+1, FloatDigitsHelper::TFloatDigits<long double>::UBitSizeMantissa, FloatDigitsHelper::TFloatDigits<long double>::UBitSizeExponent>, long double> >(TCompareFloatInterval<LongDoubleFloatDigits::UBitSizeMantissa+1, TBaseFloatInterval<ExecutionPath>, TBuiltFloat<LongDoubleFloatDigits::UBitSizeMantissa+1, FloatDigitsHelper::TFloatDigits<long double>::UBitSizeMantissa, FloatDigitsHelper::TFloatDigits<long double>::UBitSizeExponent>, long double> const&) const;
template void TBaseFloatInterval<ExecutionPath>::writeInterval<TCompareFloatInterval<LongDoubleFloatDigits::UBitSizeMantissa+1, TBaseFloatInterval<ExecutionPath>, TBuiltFloat<LongDoubleFloatDigits::UBitSizeMantissa+1, 23, 8>, float> >(TCompareFloatInterval<LongDoubleFloatDigits::UBitSizeMantissa+1, TBaseFloatInterval<ExecutionPath>, TBuiltFloat<LongDoubleFloatDigits::UBitSizeMantissa+1, 23, 8>, float> const&) const;
template void TBaseFloatInterval<ExecutionPath>::writeInterval<TCompareFloatInterval<LongDoubleFloatDigits::UBitSizeMantissa+1, TBaseFloatInterval<ExecutionPath>, TBuiltFloat<LongDoubleFloatDigits::UBitSizeMantissa+1, 52, 11>, double> >(TCompareFloatInterval<LongDoubleFloatDigits::UBitSizeMantissa+1, TBaseFloatInterval<ExecutionPath>, TBuiltFloat<LongDoubleFloatDigits::UBitSizeMantissa+1, 52, 11>, double> const&) const;
template void TBaseFloatInterval<ExecutionPath>::writeInterval<TCompareFloatInterval<LongDoubleFloatDigits::UBitSizeMantissa+1, TBaseFloatInterval<ExecutionPath>, TBuiltFloat<LongDoubleFloatDigits::UBitSizeMantissa+1, FloatDigitsHelper::TFloatDigits<long double>::UBitSizeMantissa, FloatDigitsHelper::TFloatDigits<long double>::UBitSizeExponent>, long double> >(TCompareFloatInterval<LongDoubleFloatDigits::UBitSizeMantissa+1, TBaseFloatInterval<ExecutionPath>, TBuiltFloat<LongDoubleFloatDigits::UBitSizeMantissa+1, FloatDigitsHelper::TFloatDigits<long double>::UBitSizeMantissa, FloatDigitsHelper::TFloatDigits<long double>::UBitSizeExponent>, long double> const&) const;
template void TBaseFloatInterval<ExecutionPath>::assumeInterval<TInstrumentedFloatInterval<BuiltFloat, float> >(TInstrumentedFloatInterval<BuiltFloat, float> const&) const;
template void TBaseFloatInterval<ExecutionPath>::assumeInterval<TInstrumentedFloatInterval<BuiltDouble, double> >(TInstrumentedFloatInterval<BuiltDouble, double> const&) const;
template void TBaseFloatInterval<ExecutionPath>::assumeInterval<TInstrumentedFloatInterval<BuiltLongDouble, long double> >(TInstrumentedFloatInterval<BuiltLongDouble, long double> const&) const;
template void TBaseFloatInterval<ExecutionPath>::writeInterval<TInstrumentedFloatInterval<BuiltFloat, float> >(TInstrumentedFloatInterval<BuiltFloat, float> const&) const;
template void TBaseFloatInterval<ExecutionPath>::writeInterval<TInstrumentedFloatInterval<BuiltDouble, double> >(TInstrumentedFloatInterval<BuiltDouble, double> const&) const;
template void TBaseFloatInterval<ExecutionPath>::writeInterval<TInstrumentedFloatInterval<BuiltLongDouble, long double> >(TInstrumentedFloatInterval<BuiltLongDouble, long double> const&) const;

template class TFloatInterval<ExecutionPath, BuiltFloat, float>;
template class TFloatInterval<ExecutionPath, BuiltDouble, double>;
template class TFloatInterval<ExecutionPath, BuiltLongDouble, long double>;

}} // end of namespace NumericalDomains::DDoubleInterval

template class COL::TVector<NumericalDomains::FloatInterval>;
template class COL::TVector<NumericalDomains::DoubleInterval>;
template class COL::TVector<NumericalDomains::LongDoubleInterval>;

