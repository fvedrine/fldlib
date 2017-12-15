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
// File      : SharedCollection.cpp
// Description :
//   Implementation of the classes SharedCollection, SharedCollection::GlobalElement
//   and SharedCollection::LocalElement. These classes manage shared elements that
//   can notify the collections that point onto them. It extends the class
//   SharedPointer with multiple pointed elements.
//

#include "Pointer/ExtPointer.hpp"
// #include "Pointer/SharedCollection.h"

#include "Pointer/ImplList.template"
#include "Pointer/ImplArray.template"

namespace PNT {

namespace DSharedCollection {

void
LocalElement::freeAllCalls() {
   calls().foreachDo([](GlobalElement& call){ call.disconnect(); return true; });
   while (!calls().isEmpty()) {
      GlobalElement* element = calls().getFirst();
      calls().remove(element);
      if (element->pscParent)
         delete element->pscParent;
   };
}

void
LocalElement::removeAllCalls() {
   calls().foreachDo([](GlobalElement& call){ call.disconnect(); return true; });
   while (!calls().isEmpty()) {
      GlobalElement* element = calls().getFirst();
      calls().remove(element);
      AssumeCondition(element->pscParent)
      element->pscParent->free(element);
   };
}

void
LocalElement::moveAllCallsTo(LocalElement& destination, bool doesAvoidDuplication) {
   calls().foreachDo([&destination](GlobalElement& global)
      { global.setPointerElement(&destination); return true; });
   while (!calls().isEmpty()) {
      GlobalElement* element = calls().getFirst();
      calls().remove(element);
      AssumeCondition(element->pscParent)
      bool hasFound = false;
      if (doesAvoidDuplication) {
         GlobalElement *sourceCursor = destination.calls().getFirst(),
                       *sourceFirst = sourceCursor;
         if (sourceCursor)
            hasFound = (sourceCursor->pscParent == element->pscParent);
         if (!hasFound && sourceCursor) {
            do {
               sourceCursor = destination.calls().getNext(sourceCursor);
               hasFound = (sourceCursor->pscParent == element->pscParent);
            } while (!hasFound && (sourceCursor != sourceFirst));
         };
      };
      if (hasFound)
         element->pscParent->free(element);
      else
         destination.calls().addAsLast(element);
   };
}

void
LocalElement::duplicateCalls(const LocalElement& source, UpdateCopyTable& table,
      const VirtualCast* retrieveRegistrationFromCopy) {
   GlobalElement* globalSourceFirst = source.calls().getFirst();
   GlobalElement* globalSource = globalSourceFirst;
   if (globalSource) {
      AssumeCondition(table.tType == UpdateCopyTable::TUndefined || table.tType == UpdateCopyTable::TCollection)
      if (table.tType == UpdateCopyTable::TUndefined)
         table.tType = UpdateCopyTable::TCollection;
      do {
         const SharedCollection* localSource = globalSource->pscParent;
         if (localSource) {
            UpdateCopyTable::Cursor locateCursor(table);
            EnhancedObject* fakeTarget = nullptr;
            UpdateCopyTable::Node::Balance balance = table.locateBefore(
                  UpdateCopyConnection(*localSource, *fakeTarget), locateCursor);
            if (balance == UpdateCopyTable::Node::BEqual)
               ((SharedCollection&) ((UpdateCopyConnection*) locateCursor.elementAt())->target())
                     .addElement((SharedCollection::Element&) *this);
            else {
               AutoPointer<SharedCollection> copy(retrieveRegistrationFromCopy
                  ?  (SharedCollection*) retrieveRegistrationFromCopy->castFrom(localSource->createCopy())
                  :  localSource->createSCopy(), PNT::Pointer::Init());
               copy->removeAll(); // makes only minor perturbations to source.calls() 
               copy->addElement((SharedCollection::Element&) *this);
               SharedCollection* extractedCopy = copy.extractElement();
               table.addAsSon(new UpdateCopyConnection(*localSource, *extractedCopy), locateCursor, balance);
            };
         };
         globalSource = source.calls().getNext(globalSource);
      } while (globalSource && globalSource != globalSourceFirst);
   };
}

} // end of namespace DSharedCollection

} // end of namespace PNT

template int COL::ImplArray::localize<PNT::SharedCollection::GlobalNotification::VisitedCollections::SortedParameters>(PNT::SharedCollection::GlobalNotification::VisitedCollections::SortedParameters::Key::KeyType, PNT::SharedCollection::GlobalNotification::VisitedCollections::SortedParameters const&, int, int) const;
template bool COL::ImplList::foreachDo<PNT::SharedCollection::GlobalNotification, TemplateElementCastParameters<PNT::DSharedCollection::GlobalElement, HandlerMultiCast<PNT::DSharedCollection::GlobalElement, PNT::SharedPointer, HandlerCast<PNT::SharedPointer, COL::ImplListElement> > > >(TemplateElementCastParameters<PNT::DSharedCollection::GlobalElement, HandlerMultiCast<PNT::DSharedCollection::GlobalElement, PNT::SharedPointer, HandlerCast<PNT::SharedPointer, COL::ImplListElement> > >, PNT::SharedCollection::GlobalNotification&, COL::ImplListElement*, COL::ImplListElement*) const;
template bool COL::ImplList::foreachDo<PNT::SharedCollection::LinearGlobalNotification, TemplateElementCastParameters<PNT::DSharedCollection::GlobalElement, HandlerMultiCast<PNT::DSharedCollection::GlobalElement, PNT::SharedPointer, HandlerCast<PNT::SharedPointer, COL::ImplListElement> > > >(TemplateElementCastParameters<PNT::DSharedCollection::GlobalElement, HandlerMultiCast<PNT::DSharedCollection::GlobalElement, PNT::SharedPointer, HandlerCast<PNT::SharedPointer, COL::ImplListElement> > >, PNT::SharedCollection::LinearGlobalNotification&, COL::ImplListElement*, COL::ImplListElement*) const;
template bool COL::ImplList::foreachDo<std::function<bool (PNT::DSharedCollection::GlobalElement&)>, TemplateElementCastParameters<PNT::DSharedCollection::GlobalElement, HandlerMultiCast<PNT::DSharedCollection::GlobalElement, PNT::SharedPointer, HandlerCast<PNT::SharedPointer, COL::ImplListElement> > > >(TemplateElementCastParameters<PNT::DSharedCollection::GlobalElement, HandlerMultiCast<PNT::DSharedCollection::GlobalElement, PNT::SharedPointer, HandlerCast<PNT::SharedPointer, COL::ImplListElement> > >, std::function<bool (PNT::DSharedCollection::GlobalElement&)>&, COL::ImplListElement*, COL::ImplListElement*) const;
template bool COL::ImplList::foreachDo<std::function<bool (PNT::DSharedCollection::GlobalElement&)>, TemplateElementCastParameters<PNT::DSharedCollection::GlobalElement, HandlerIntermediateCast<PNT::DSharedCollection::GlobalElement, PNT::DSharedCollection::Registration, COL::ImplListElement> > >(TemplateElementCastParameters<PNT::DSharedCollection::GlobalElement, HandlerIntermediateCast<PNT::DSharedCollection::GlobalElement, PNT::DSharedCollection::Registration, COL::ImplListElement> >, std::function<bool (PNT::DSharedCollection::GlobalElement&)>&, COL::ImplListElement*, COL::ImplListElement*) const;
