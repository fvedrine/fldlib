/**************************************************************************/
/*                                                                        */
/*  This file is part of FLDLib                                           */
/*                                                                        */
/*  Copyright (C) 2014-2017                                               */
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
// Library   : Collection
// Unit      : Concrete collections
// File      : BasicList.cpp
// Description :
//   Implementation of a list of elements inheriting from VirtualCollection.
//   No inheritance relationship is assumed on the elements.
//

#include "Collection/ConcreteCollection/BasicList.h"
// #include "Collection/Collection.hpp"

namespace COL {}

#include "Pointer/ImplList.template"

namespace COL {

/*****************************************/
/* Implementation of the class BasicList */
/*****************************************/

BasicList::BasicList(const BasicList& source, AddMode dupMode,
      const VirtualCast* retrieveRegistrationFromCopy)
   :  GenericList(source, AMDuplicate) {
   if (dupMode == AMDuplicate) {
      try {
         std::function<bool(BasicListElement&)> copy(
            [retrieveRegistrationFromCopy](BasicListElement& element)
            {  element.copyElement(retrieveRegistrationFromCopy); return true; });
         GenericList::foreachDo(TemplateElementCastParameters<BasicListElement, SimpleCast>(), copy);
      }
      catch (...) { // [TODO] should handle allocation errors without any new allocation
         PPCursor cursor(newCursor()), sourceCursor(source.newCursor());
         for (bool doesContinue = cursor->setToFirst() && sourceCursor->setToFirst(); doesContinue;
              doesContinue = doesContinue && cursor->setToNext() && sourceCursor->setToNext())
            if ((doesContinue = (cursor->element() != sourceCursor->element())) != false)
               cursor->element()->releaseElement();
         GenericList::_removeAll(ExtendedSuppressParameters().setFree());
         throw;
      };
   };
}

void
BasicList::_fullAssign(const BasicList& source, const ExtendedReplaceParameters& parameters) {
   if (this != &source) {
      if (!parameters.isFree() && !parameters.isDuplicate()) {
         PNT::AutoPointer<ExtendedReplaceParameters> paramsCopy(parameters);
         paramsCopy->setFree().setDuplicate();
         GenericList::_fullAssign(source, *paramsCopy);
         return;
      };

      VirtualCollection::_fullAssign(source, parameters);
      if (parameters.isFree() && !isEmpty()) {
         std::function<bool(BasicListElement&)> release([](BasicListElement& element)
            {  element.releaseElement(); return true; });
         GenericList::foreachDo(TemplateElementCastParameters<BasicListElement, SimpleCast>(), release);
      };

      {  ExtendedReplaceParameters params(parameters);
         GenericList::_fullAssign(source, parameters.isDuplicate()
            ? params.setLocal()
            : params.setDuplicate());
      };

      if (parameters.isDuplicate()) {
         try {
            auto retrieveRegistrationFromCopy = parameters.getSCast();
            std::function<bool(BasicListElement&)> copy(
               [retrieveRegistrationFromCopy](BasicListElement& element)
               {  element.copyElement(retrieveRegistrationFromCopy); return true; });
            GenericList::foreachDo(TemplateElementCastParameters<BasicListElement, SimpleCast>(), copy);
         }
         catch (...) { // [TODO] should handle allocation errors without any new allocation
            PPCursor cursor(newCursor()), sourceCursor(source.newCursor());
            for (bool doesContinue = cursor->setToFirst() && sourceCursor->setToFirst(); doesContinue;
                 doesContinue = doesContinue && cursor->setToNext() && sourceCursor->setToNext())
               if ((doesContinue = (cursor->element() != sourceCursor->element())) != false)
                  cursor->element()->releaseElement();
            GenericList::_removeAll(ExtendedSuppressParameters().setFree());
            throw;
         };
      };
   };
}

/* insertion methods */
void
BasicList::_add(EnhancedObject* anObject, const ExtendedInsertionParameters& parameters,
      BasicListCursor* cursor) {
   VirtualCollection::_add(anObject, parameters, cursor);
   AddGuardian<EnhancedObject> guard(*this, anObject, parameters);
   ExtendedInsertionParameters paramsCopy(parameters);
   anObject = guard.handleDuplication();
   paramsCopy.releaseDuplicate().setFreeOnError();
   GenericList::_add(new BasicListElement(*anObject), paramsCopy, cursor);
   guard.release();
}

void
BasicList::_addAll(const BasicList& source, const ExtendedInsertionParameters& parameters,
      BasicListCursor* cursor, const BasicListCursor* startSource,
      const BasicListCursor* endSource) {
   if (parameters.isDuplicate()) {
      ImplListElement *prevStart=nullptr, *nextEnd=nullptr;
      GenericList::retrieveBoundInsertion(parameters, cursor, prevStart, nextEnd);
      GenericList::_addAll(source, ExtendedInsertionParameters(parameters).setLocal(), cursor,
            startSource, endSource);

      try {
         auto retrieveRegistrationFromCopy = parameters.getSCast();
         std::function<bool(BasicListElement&)> copy(
            [retrieveRegistrationFromCopy](BasicListElement& element)
            {  element.copyElement(retrieveRegistrationFromCopy); return true; });
         GenericList::foreachDo(TemplateElementCastParameters<BasicListElement, SimpleCast>(),
            copy, prevStart, nextEnd);
      }
      catch (...) {
         PPCursor cursor(newCursor()), sourceCursor(source.newCursor());
         bool doesContinue = true;
         if (prevStart) {
            cursor->setElement((BasicListElement*) prevStart);
            doesContinue = cursor->setToNext();
         }
         else
            doesContinue = cursor->setToFirst();
         if (startSource)
            sourceCursor->setElement(startSource->element());
         else
            sourceCursor->setToFirst();

         for (; doesContinue; doesContinue = (doesContinue && cursor->setToNext() && sourceCursor->setToNext()))
            if ((doesContinue = (cursor->element() != sourceCursor->element())) != false)
               cursor->element()->releaseElement();
         GenericList::_removeAll(ExtendedSuppressParameters().setFree());
         throw;
      };
   }
   else {
      GenericList::_addAll(source, ExtendedInsertionParameters(parameters).setLocal()
            .setDuplicate().setFreeOnError(), cursor, startSource, endSource);
      return;
   };
}

/* suppression methods */
void
BasicList::_remove(const ExtendedSuppressParameters& parameters, BasicListCursor* cursor) {
   if (parameters.isFree()) {
      BasicListElement* element = (BasicListElement*) GenericList::_getElement(parameters, cursor);
      if (element)
         element->freeElement();
      GenericList::_remove(parameters, cursor);
   }
   else {
      PNT::AutoPointer<ExtendedSuppressParameters> paramsCopy(parameters);
      paramsCopy->setFree();
      GenericList::_remove(*paramsCopy, cursor);
   };
}

void
BasicList::_removeAll(const ExtendedSuppressParameters& parameters,
      const BasicListCursor* start, const BasicListCursor* end) {
   if (parameters.isFree()) {
      ImplListElement *prevStart = nullptr, *nextEnd = nullptr;
      GenericList::retrieveBoundSuppression(parameters, start, end, prevStart, nextEnd);
      std::function<bool(BasicListElement&)> freeElement([](BasicListElement& element)
         {  element.freeElement(); return true; });
      GenericList::foreachDo(TemplateElementCastParameters<BasicListElement, SimpleCast>(),
         freeElement, prevStart, nextEnd);
      GenericList::_removeAll(parameters, start, end);
   }
   else {
      PNT::AutoPointer<ExtendedSuppressParameters> paramsCopy(parameters);
      paramsCopy->setFree();
      GenericList::_removeAll(*paramsCopy, start, end);
   }
}

/* replace method */
void
BasicList::_replace(EnhancedObject* newObject, const ExtendedReplaceParameters& parameters,
      BasicListCursor* cursor) {
   VirtualCollection::_replace(newObject, parameters, cursor);
   AddGuardian<EnhancedObject> guard(*this, newObject, parameters);

   BasicListElement* locateElement = (BasicListElement*) GenericList::_getElement(
         (const ExtendedSuppressParameters&) parameters, cursor);
   if (locateElement->element() != newObject) {
      if (parameters.isFree())
         locateElement->freeElement();
      else
         locateElement->releaseElement();
   };
   newObject = guard.handleDuplication();
   locateElement->setElement(*newObject);
   guard.release();
}

} // end of namespace COL

