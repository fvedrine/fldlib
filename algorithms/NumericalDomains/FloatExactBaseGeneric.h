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
// File      : FloatExactBaseGeneric.h
// Description :
//   Definition of a class of floating point comparison
//

#ifndef NumericalDomains_FloatExactBaseGenericH
#define NumericalDomains_FloatExactBaseGenericH

#include "NumericalLattices/FloatExactBaseTypesGeneric.h"

namespace NumericalDomains {

#define DefineGeneric
#include "NumericalDomains/FloatExactBase.inch"
#undef DefineGeneric

} // end of namespace NumericalDomains

#endif // NumericalDomains_FloatExactBaseGenericH

