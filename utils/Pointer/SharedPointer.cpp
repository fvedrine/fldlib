/**************************************************************************/
/*                                                                        */
/*  This file is part of FLDLib                                           */
/*                                                                        */
/*  Copyright (C) 2013-2017                                               */
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
// Library   : Pointer
// Unit      : Elaborated pointers
// File      : SharedPointer.cpp
// Description :
//   Implementation of the classes SharedPointer, SharedElement and of the template TSharedPointer.
//

#include "Pointer/ExtPointer.hpp"
// #include "Pointer/SharedPointer.h"

#include "Pointer/ImplList.template"

namespace PNT {

void
SharedPointer::release() {
   if (Pointer::isValid()) {
      SharedElement* element = &getElement();
      if (getElement().unlock(*this))
         delete element;
   };
}

SharedPointer&
SharedPointer::operator=(const SharedPointer& source) {
   COL::ImplListElement::operator=((const COL::ImplListElement&) source);
   if (Pointer::key() != source.Pointer::key()) {
      if (Pointer::isValid())
         getElement().unlock(*this);
      if (source.Pointer::isValid())
         source.getElement().lock(*this);
   };
   return *this;
}

void
SharedPointer::fullAssign(const SharedPointer& source) {
   COL::ImplListElement::operator=(source);
   if (Pointer::key() != source.Pointer::key()) {
      SharedElement* oldElement = nullptr;
      bool doesFree = false;
      if (Pointer::isValid()) {
         oldElement = &getElement();
         doesFree = oldElement->unlock(*this);
      };
      try {
         if (source.Pointer::isValid())
            source.getElement().lock(*this);
      }
      catch (...) {
         if (oldElement)
            oldElement->lock(*this);
         throw;
      };
      if (doesFree && oldElement) delete oldElement;
   };
}

void
SharedPointer::setElement(SharedElement& element) {
   if (Pointer::isValid())
      getElement().unlock(*this);
   element.lock(*this);
}

SharedPointer&
SharedPointer::assign(SharedElement* element, Init) {
   if (Pointer::isValid())
      getElement().unlock(*this);
   if (element)
      element->lock(*this);
   return *this;
}

void
SharedPointer::swap(SharedPointer& source) {
   if (Pointer::isValid() && source.Pointer::isValid()) {
      if (&getCElement() != &source.getCElement()) {
         SharedElement* thisElement = &getElement();
         SharedElement* sourceElement = &source.getElement();
         thisElement->unlock(*this);
         sourceElement->lock(*this);
         sourceElement->unlock(source);
         thisElement->lock(source);
      };
   }
   else {
      if (Pointer::isValid()) {
         getElement().lock(source);
         getElement().unlock(*this);
      }
      else if (source.Pointer::isValid()) {
         source.getElement().lock(*this);
         source.getElement().unlock(source);
      };
   };
}

} // end of namespace PNT

template bool COL::TImplList<PNT::SharedPointer, HandlerCast<PNT::SharedPointer, COL::ImplListElement> >::foreachDo<PNT::TPointerNotification<PNT::SharedPointer> >(PNT::TPointerNotification<PNT::SharedPointer>&, PNT::SharedPointer*, PNT::SharedPointer*) const;
template bool COL::ImplList::foreachDo<PNT::TPointerNotification<PNT::SharedPointer>, TemplateElementCastParameters<PNT::SharedPointer, HandlerCast<PNT::SharedPointer, COL::ImplListElement> > >(TemplateElementCastParameters<PNT::SharedPointer, HandlerCast<PNT::SharedPointer, COL::ImplListElement> >, PNT::TPointerNotification<PNT::SharedPointer>&, COL::ImplListElement*, COL::ImplListElement*) const;
template bool COL::ImplList::foreachDo<std::function<bool (PNT::SharedPointer&)>, TemplateElementCastParameters<PNT::SharedPointer, HandlerCast<PNT::SharedPointer, COL::ImplListElement> > >(TemplateElementCastParameters<PNT::SharedPointer, HandlerCast<PNT::SharedPointer, COL::ImplListElement> >, std::function<bool (PNT::SharedPointer&)>&, COL::ImplListElement*, COL::ImplListElement*) const;
