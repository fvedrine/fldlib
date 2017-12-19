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

template class TCompareFloatInterval<81, TBaseFloatInterval<ExecutionPath>, BuiltFloat, float>;
template class TCompareFloatInterval<81, TBaseFloatInterval<ExecutionPath>, BuiltDouble, double>;
template class TCompareFloatInterval<81, TBaseFloatInterval<ExecutionPath>, BuiltLongDouble, long double>;

template class TInstrumentedFloatInterval<BuiltFloat, float>;
template class TInstrumentedFloatInterval<BuiltDouble, double>;
template class TInstrumentedFloatInterval<BuiltLongDouble, long double>;

// template class TBaseFloatInterval<TBaseFloatInterval<ExecutionPath> >;
template class TBaseFloatInterval<ExecutionPath>;

template void TBaseFloatInterval<ExecutionPath>::assumeInterval<TCompareFloatInterval<81, TBaseFloatInterval<ExecutionPath>, TBuiltFloat<81, 23, 8>, float> >(TCompareFloatInterval<81, TBaseFloatInterval<ExecutionPath>, TBuiltFloat<81, 23, 8>, float> const&) const;
template void TBaseFloatInterval<ExecutionPath>::assumeInterval<TCompareFloatInterval<81, TBaseFloatInterval<ExecutionPath>, TBuiltFloat<81, 52, 11>, double> >(TCompareFloatInterval<81, TBaseFloatInterval<ExecutionPath>, TBuiltFloat<81, 52, 11>, double> const&) const;
template void TBaseFloatInterval<ExecutionPath>::assumeInterval<TCompareFloatInterval<81, TBaseFloatInterval<ExecutionPath>, TBuiltFloat<81, 80, 15>, long double> >(TCompareFloatInterval<81, TBaseFloatInterval<ExecutionPath>, TBuiltFloat<81, 80, 15>, long double> const&) const;
template void TBaseFloatInterval<ExecutionPath>::writeInterval<TCompareFloatInterval<81, TBaseFloatInterval<ExecutionPath>, TBuiltFloat<81, 23, 8>, float> >(TCompareFloatInterval<81, TBaseFloatInterval<ExecutionPath>, TBuiltFloat<81, 23, 8>, float> const&) const;
template void TBaseFloatInterval<ExecutionPath>::writeInterval<TCompareFloatInterval<81, TBaseFloatInterval<ExecutionPath>, TBuiltFloat<81, 52, 11>, double> >(TCompareFloatInterval<81, TBaseFloatInterval<ExecutionPath>, TBuiltFloat<81, 52, 11>, double> const&) const;
template void TBaseFloatInterval<ExecutionPath>::writeInterval<TCompareFloatInterval<81, TBaseFloatInterval<ExecutionPath>, TBuiltFloat<81, 80, 15>, long double> >(TCompareFloatInterval<81, TBaseFloatInterval<ExecutionPath>, TBuiltFloat<81, 80, 15>, long double> const&) const;
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

