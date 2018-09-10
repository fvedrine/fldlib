/**************************************************************************/
/*                                                                        */
/*  This file is part of FLDLib                                           */
/*                                                                        */
/*  Copyright (C) 2017-2017                                               */
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
// Unit      : BackTrace
// File      : BackTrace.h
// Description :
//   Definition of a function to write the backtrace.
//

#ifndef FloatInstrumentation_BackTraceH
#define FloatInstrumentation_BackTraceH

#include "StandardClasses/Persistence.h"

namespace NumericalDomains {

void writeSimplifiedBackTrace(STG::IOObject::OSBase& out);
void writeBackTrace(STG::IOObject::OSBase& out);
void readBackTrace(STG::IOObject::ISBase& in);

}

#endif
