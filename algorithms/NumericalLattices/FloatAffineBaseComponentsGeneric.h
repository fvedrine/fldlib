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
// File      : FloatAffineBaseComponentsGeneric.h
// Description :
//   Definition of a class of affine relations.
//

#ifndef NumericalDomains_FloatAffineBaseComponentsGenericH
#define NumericalDomains_FloatAffineBaseComponentsGenericH

#include "NumericalLattices/FloatAffineBaseCommon.h"

#include "ForwardNumerics/BaseFloatingGeneric.h"
#include "BackwardNumerics/BaseProjectionFloating.h"
#include <math.h>

namespace NumericalDomains {

#define DefineGeneric
#include "NumericalLattices/FloatAffineBaseComponents.inch"
#undef DefineGeneric

} // end of namespace NumericalDomains::DAffine

#endif // NumericalDomains_FloatAffineBaseComponentsGenericH

