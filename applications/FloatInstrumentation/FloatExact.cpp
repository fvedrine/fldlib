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
// Unit      : FloatExact
// File      : FloatExact.cpp
// Description :
//   Implementation of a class of comparison between floating points
//

#include "FloatInstrumentation/FloatExact.h"

#include "NumericalAnalysis/FloatExactExecutionPath.template"

#if !defined(FLOAT_GENERIC_BASE_UNSIGNED) && !defined(FLOAT_GENERIC_BASE_LONG)
#include "NumericalDomains/FloatExactBase.template"
#else
#include "NumericalDomains/FloatExactBaseGeneric.template"
#endif

#include <fstream>
#include <sstream>

#ifndef FLOAT_LONG_WRITE
#if !defined(FLOAT_GENERIC_BASE_UNSIGNED) && !defined(FLOAT_GENERIC_BASE_LONG)
#include "NumericalLattices/FloatExactBaseTypes.template"
#else
#include "NumericalLattices/FloatExactBaseTypesGeneric.template"
#endif
#endif

#include "Pointer/Vector.template"

#include "FloatInstrumentation/FloatExact.incc"

namespace NumericalDomains { namespace DDoubleExact {

template <> int tfinite(long double val) { return finite((double) val); }
template <> int tfinite(double val) { return finite(val); }
template <> int tfinite(float val) { return finite(val); }

template <> int tisfinite(long double val) { return isfinite((double) val); }
template <> int tisfinite(double val) { return isfinite(val); }
template <> int tisfinite(float val) { return isfinite(val); }

template <> int tisnan(long double val) { return isnan(val); }
template <> int tisnan(double val) { return isnan(val); }
template <> int tisnan(float val) { return isnan(val); }

template <> int tisinf(long double val) { return isinf(val); }
template <> int tisinf(double val) { return isinf(val); }
template <> int tisinf(float val) { return isinf(val); }

#if !defined(FLOAT_GENERIC_BASE_UNSIGNED) && !defined(FLOAT_GENERIC_BASE_LONG)
template class TCompareFloatExact<FLOAT_REAL_BITS_NUMBER, TBaseFloatExact<ExecutionPath>, BuiltFloat, float>;
template class TCompareFloatExact<FLOAT_REAL_BITS_NUMBER, TBaseFloatExact<ExecutionPath>, BuiltDouble, double>;
template class TCompareFloatExact<FLOAT_REAL_BITS_NUMBER, TBaseFloatExact<ExecutionPath>, BuiltLongDouble, long double>;
template void TBaseFloatExact<ExecutionPath>::assumeCompare<TCompareFloatExact<FLOAT_REAL_BITS_NUMBER, TBaseFloatExact<ExecutionPath>, BuiltFloat, float> >(TCompareFloatExact<FLOAT_REAL_BITS_NUMBER, TBaseFloatExact<ExecutionPath>, BuiltFloat, float> const&, BuiltReal const&) const;
template void TBaseFloatExact<ExecutionPath>::assumeCompare<TCompareFloatExact<FLOAT_REAL_BITS_NUMBER, TBaseFloatExact<ExecutionPath>, BuiltDouble, double> >(TCompareFloatExact<FLOAT_REAL_BITS_NUMBER, TBaseFloatExact<ExecutionPath>, BuiltDouble, double> const&, BuiltReal const&) const;
template void TBaseFloatExact<ExecutionPath>::assumeCompare<TCompareFloatExact<FLOAT_REAL_BITS_NUMBER, TBaseFloatExact<ExecutionPath>, BuiltLongDouble, long double> >(TCompareFloatExact<FLOAT_REAL_BITS_NUMBER, TBaseFloatExact<ExecutionPath>, BuiltLongDouble, long double> const&, BuiltReal const&) const;
template void TBaseFloatExact<ExecutionPath>::writeCompare<TCompareFloatExact<FLOAT_REAL_BITS_NUMBER, TBaseFloatExact<ExecutionPath>, BuiltFloat, float> >(TCompareFloatExact<FLOAT_REAL_BITS_NUMBER, TBaseFloatExact<ExecutionPath>, BuiltFloat, float> const&, BuiltReal const&) const;
template void TBaseFloatExact<ExecutionPath>::writeCompare<TCompareFloatExact<FLOAT_REAL_BITS_NUMBER, TBaseFloatExact<ExecutionPath>, BuiltDouble, double> >(TCompareFloatExact<FLOAT_REAL_BITS_NUMBER, TBaseFloatExact<ExecutionPath>, BuiltDouble, double> const&, BuiltReal const&) const;
template void TBaseFloatExact<ExecutionPath>::writeCompare<TCompareFloatExact<FLOAT_REAL_BITS_NUMBER, TBaseFloatExact<ExecutionPath>, BuiltLongDouble, long double> >(TCompareFloatExact<FLOAT_REAL_BITS_NUMBER, TBaseFloatExact<ExecutionPath>, BuiltLongDouble, long double> const&, BuiltReal const&) const;
#else
template class TGCompareFloatExact<FLOAT_REAL_BITS_NUMBER, TBaseFloatExact<ExecutionPath>, BuiltFloat, float>;
template class TGCompareFloatExact<FLOAT_REAL_BITS_NUMBER, TBaseFloatExact<ExecutionPath>, BuiltDouble, double>;
template class TGCompareFloatExact<FLOAT_REAL_BITS_NUMBER, TBaseFloatExact<ExecutionPath>, BuiltLongDouble, long double>;
template void TBaseFloatExact<ExecutionPath>::assumeCompare<TGCompareFloatExact<FLOAT_REAL_BITS_NUMBER, TBaseFloatExact<ExecutionPath>, BuiltFloat, float> >(TGCompareFloatExact<FLOAT_REAL_BITS_NUMBER, TBaseFloatExact<ExecutionPath>, BuiltFloat, float> const&, BuiltReal const&) const;
template void TBaseFloatExact<ExecutionPath>::assumeCompare<TGCompareFloatExact<FLOAT_REAL_BITS_NUMBER, TBaseFloatExact<ExecutionPath>, BuiltDouble, double> >(TGCompareFloatExact<FLOAT_REAL_BITS_NUMBER, TBaseFloatExact<ExecutionPath>, BuiltDouble, double> const&, BuiltReal const&) const;
template void TBaseFloatExact<ExecutionPath>::assumeCompare<TGCompareFloatExact<FLOAT_REAL_BITS_NUMBER, TBaseFloatExact<ExecutionPath>, BuiltLongDouble, long double> >(TGCompareFloatExact<FLOAT_REAL_BITS_NUMBER, TBaseFloatExact<ExecutionPath>, BuiltLongDouble, long double> const&, BuiltReal const&) const;
template void TBaseFloatExact<ExecutionPath>::writeCompare<TGCompareFloatExact<FLOAT_REAL_BITS_NUMBER, TBaseFloatExact<ExecutionPath>, BuiltFloat, float> >(TGCompareFloatExact<FLOAT_REAL_BITS_NUMBER, TBaseFloatExact<ExecutionPath>, BuiltFloat, float> const&, BuiltReal const&) const;
template void TBaseFloatExact<ExecutionPath>::writeCompare<TGCompareFloatExact<FLOAT_REAL_BITS_NUMBER, TBaseFloatExact<ExecutionPath>, BuiltDouble, double> >(TGCompareFloatExact<FLOAT_REAL_BITS_NUMBER, TBaseFloatExact<ExecutionPath>, BuiltDouble, double> const&, BuiltReal const&) const;
template void TBaseFloatExact<ExecutionPath>::writeCompare<TGCompareFloatExact<FLOAT_REAL_BITS_NUMBER, TBaseFloatExact<ExecutionPath>, BuiltLongDouble, long double> >(TGCompareFloatExact<FLOAT_REAL_BITS_NUMBER, TBaseFloatExact<ExecutionPath>, BuiltLongDouble, long double> const&, BuiltReal const&) const;
#endif

template void TBaseFloatExact<ExecutionPath>::assumeCompare<TInstrumentedFloat<BuiltFloat, float> >(TInstrumentedFloat<BuiltFloat, float> const&, BuiltReal const&) const;
template void TBaseFloatExact<ExecutionPath>::assumeCompare<TInstrumentedFloat<BuiltDouble, double> >(TInstrumentedFloat<BuiltDouble, double> const&, BuiltReal const&) const;
template void TBaseFloatExact<ExecutionPath>::assumeCompare<TInstrumentedFloat<BuiltLongDouble, long double> >(TInstrumentedFloat<BuiltLongDouble, long double> const&, BuiltReal const&) const;
template void TBaseFloatExact<ExecutionPath>::writeCompare<TInstrumentedFloat<BuiltFloat, float> >(TInstrumentedFloat<BuiltFloat, float> const&, BuiltReal const&) const;
template void TBaseFloatExact<ExecutionPath>::writeCompare<TInstrumentedFloat<BuiltDouble, double> >(TInstrumentedFloat<BuiltDouble, double> const&, BuiltReal const&) const;
template void TBaseFloatExact<ExecutionPath>::writeCompare<TInstrumentedFloat<BuiltLongDouble, long double> >(TInstrumentedFloat<BuiltLongDouble, long double> const&, BuiltReal const&) const;
template void TBaseFloatExact<ExecutionPath>::writeBranchCompare<TInstrumentedFloat<BuiltFloat, float> >(TInstrumentedFloat<BuiltFloat, float> const&, BuiltReal const&) const;
template void TBaseFloatExact<ExecutionPath>::writeBranchCompare<TInstrumentedFloat<BuiltDouble, double> >(TInstrumentedFloat<BuiltDouble, double> const&, BuiltReal const&) const;
template void TBaseFloatExact<ExecutionPath>::writeBranchCompare<TInstrumentedFloat<BuiltLongDouble, long double> >(TInstrumentedFloat<BuiltLongDouble, long double> const&, BuiltReal const&) const;

template class TFloatExact<ExecutionPath, BuiltFloat, float>;
template class TFloatExact<ExecutionPath, BuiltDouble, double>;
template class TFloatExact<ExecutionPath, BuiltLongDouble, long double>;

} // end of namespace DDoubleExact

template DDoubleExact::TMergeBranches<DDoubleExact::ExecutionPath>&
DDoubleExact::TMergeBranches<DDoubleExact::ExecutionPath>::operator<<(DDoubleExact::TFloatExact<DDoubleExact::ExecutionPath, DDoubleExact::BuiltFloat, float>&);

template DDoubleExact::TMergeBranches<DDoubleExact::ExecutionPath>&
DDoubleExact::TMergeBranches<DDoubleExact::ExecutionPath>::operator<<(DDoubleExact::TFloatExact<DDoubleExact::ExecutionPath, DDoubleExact::BuiltDouble, double>&);

template DDoubleExact::TMergeBranches<DDoubleExact::ExecutionPath>&
DDoubleExact::TMergeBranches<DDoubleExact::ExecutionPath>::operator<<(DDoubleExact::TFloatExact<DDoubleExact::ExecutionPath, DDoubleExact::BuiltLongDouble, long double>&);

} // end of namespace NumericalDomains

template class COL::TVector<NumericalDomains::DDoubleExact::TBaseFloatExact<NumericalDomains::DDoubleExact::ExecutionPath> >;
template class COL::TVector<NumericalDomains::DDoubleExact::BuiltFloat>;
template class COL::TVector<NumericalDomains::DDoubleExact::BuiltDouble>;
template class COL::TVector<NumericalDomains::DDoubleExact::BuiltLongDouble>;
template class COL::TVector<NumericalDomains::DDoubleExact::TInstrumentedFloat<NumericalDomains::DDoubleExact::BuiltFloat, float> >;
template class COL::TVector<NumericalDomains::DDoubleExact::TInstrumentedFloat<NumericalDomains::DDoubleExact::BuiltDouble, double> >;
template class COL::TVector<NumericalDomains::DDoubleExact::TInstrumentedFloat<NumericalDomains::DDoubleExact::BuiltLongDouble, long double> >;
