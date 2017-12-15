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
// File      : List.cpp
// Description :
//   Implementation of a list of double linked elements inheriting from VirtualCollection.
//

#include "Collection/ConcreteCollection/List.h"
// #include "Collection/Collection.hpp"

namespace COL {

/*******************************************/
/* Implementation of the class GenericList */
/*******************************************/

void
GenericList::_fullAssign(const GenericList& source, const ExtendedReplaceParameters& parameters) {
   VirtualCollection::_fullAssign(source, parameters);
   if ((this != &source) && parameters.isDuplicate()) {
      InvalidateNotification invalidation;
      notifyCursorUpdate(invalidation);
      if (!parameters.isFree())
         ImplList::removeAll();
      try {
         ImplList::assign((const ImplList&) source, true, parameters.getSCast());
         uCount = source.uCount;
      }
      catch (...) {
         ImplList::freeAll();
         uCount = 0;
         throw;
      };
   }
   else if (this != &source) {
      InvalidateNotification invalidation;
      notifyCursorUpdate(invalidation);
      if (parameters.isFree())
         ImplList::freeAll();
      else
         ImplList::removeAll();
      uCount = 0;
   };
}

// insertion methods
void
GenericList::_add(ImplListElement* newElement, const ExtendedInsertionParameters& parameters,
      GenericListCursor* cursor) {
   VirtualCollection::_add(newElement, parameters, cursor);
   AddGuardian<ImplListElement> guard(*this, newElement, parameters);
   newElement = guard.handleDuplication();
   if (parameters.isInsertion()) {
      if ((!cursor) || (!cursor->element())) {
         if (parameters.getRelativePosition() == RPAfter)
            ImplList::addAsFirst(newElement);
         else
            ImplList::addAsLast(newElement);
      }
      else
         ImplList::addAsNext(newElement, cursor->element(),
            parameters.getRelativePosition() == RPBefore ? ImplList::DPrevious : ImplList::DNext);
   }
   else
      ImplList::addAsLast(newElement);
   guard.release();
   uCount++;
   if (cursor && !parameters.isRemote())
      cursor->setElement(newElement);
}

void
GenericList::_addAll(const GenericList& source, const ExtendedInsertionParameters& parameters,
      GenericListCursor* cursor, const GenericListCursor* startSource, const GenericListCursor* endSource) {
   VirtualCollection::_addAll(source, parameters, cursor, startSource, endSource);
   if (!parameters.isDuplicate() || (source.uCount == 0))
      return;

   ImplListElement *destination=nullptr, *nextDestination=nullptr;
   retrieveBoundInsertion(parameters, cursor, destination, nextDestination);
   try {
      ImplList::addCopyAll((const ImplList&) source,
            startSource ? startSource->element() : nullptr,
            endSource ? endSource->element() : nullptr,
            destination, parameters.getSCast());
      if (!startSource && !endSource)
         uCount += source.uCount;
      else {
         if ((destination && (ImplList::getNext(destination) == nextDestination))
             || (!destination && (ImplList::getFirst() == nextDestination)))
            return;
         uCount += ImplList::queryNumberOfElementsBetween(
               destination ? ImplList::getNext(destination) : ImplList::getFirst(),
               nextDestination ? ImplList::getPrevious(nextDestination) : ImplList::getLast());
      };
   }
   catch (...) {
      uCount = ImplList::queryNumberOfElementsBetween(ImplList::getFirst(), ImplList::getLast());
      if (cursor)
         cursor->setElement((parameters.getRelativePosition() != RPBefore)
            ? (nextDestination ? ImplList::getPrevious(nextDestination) : ImplList::getLast())
            : (destination ? ImplList::getNext(destination) : ImplList::getFirst()));
      throw;
   };
   if (cursor && !parameters.isRemote())
      cursor->setElement((parameters.getRelativePosition() != RPBefore)
         ? (nextDestination ? ImplList::getPrevious(nextDestination) : ImplList::getLast())
         : (destination ? ImplList::getNext(destination) : ImplList::getFirst()));
}

// move methods
void
GenericList::_moveTo(VirtualCollection& destination, const ExtendedReplaceParameters& parameters,
      VirtualCollectionCursor* vccCursor, VirtualCollectionCursor* vccDestinationCursor) {
   if (&destination != this) {
      VirtualCollection::pmoveTo(destination, parameters, vccCursor, vccDestinationCursor);
      return;
   };

   AssumeCondition(dynamic_cast<const GenericList*>(&destination))
   // Intern move without any modification on vccCursor
   VirtualCollection::_moveTo(destination, parameters, vccCursor, vccDestinationCursor);
   GenericListCursor *cursor = (GenericListCursor*) vccCursor,
                     *destinationCursor = (GenericListCursor*) vccDestinationCursor;
   ImplListElement* element = getElement(cursor ? cursor->element() : nullptr,
         parameters.getRemovePosition());
   ImplListElement *destinationElement=nullptr, *nextDestinationElement=nullptr;
   ((GenericList&) destination).retrieveBoundInsertion(parameters, destinationCursor, destinationElement, nextDestinationElement);
   if ((destinationElement == element) || (element == nextDestinationElement))
      return;

   ImplList::remove(element);
   if (destinationElement)
      ImplList::addAsNext(element, destinationElement, ImplList::DNext);
   else
      ImplList::addAsFirst(element);
   if (destinationCursor && !parameters.isRemote())
      destinationCursor->setElement(element);
}

void
GenericList::_moveAllTo(VirtualCollection& destination, const ExtendedReplaceParameters& parameters,
      const VirtualCollectionCursor* startCursor, const VirtualCollectionCursor* endCursor,
      VirtualCollectionCursor* destinationCursor) {
   if (this != &destination)
      VirtualCollection::pmoveAllTo(destination, parameters, startCursor, endCursor,
            destinationCursor);
   else
      _moveAllTo(*this, parameters, (const GenericListCursor*) startCursor,
            (const GenericListCursor*) endCursor, (GenericListCursor*) destinationCursor);
}

void
GenericList::_moveAllTo(GenericList& destination, const ExtendedReplaceParameters& parameters,
      const GenericListCursor* startCursor, const GenericListCursor* endCursor,
      GenericListCursor* destinationCursor) {
   VirtualCollection::_moveAllTo(destination, parameters, startCursor, endCursor,
         destinationCursor);

   // Interval of elements to move, their number and the insertion position
   ImplListElement* start = startCursor ? startCursor->element() : nullptr;
   ImplListElement* end = endCursor ? endCursor->element() : nullptr;
   if (!retrieveRemovedInterval(start, end, parameters.getRemovePosition())
       || (startCursor && endCursor && ImplList::compareCursors(start, end) == CRGreater))
      return;

   int movedElements = uCount;
   if (startCursor || endCursor)
      movedElements = ImplList::queryNumberOfElementsBetween(start, end);

   ImplListElement *destinationElement=nullptr, *nextDestinationElement=nullptr;
   destination.retrieveBoundInsertion(parameters, destinationCursor, destinationElement, nextDestinationElement);

   // cursor notification
   if (this != &destination) {
      if (movedElements == uCount) {
         InvalidateNotification invalidation;
         notifyCursorUpdate(invalidation);
      }
      else {
         RemoveBoundNotification invalidation((const ImplList&) *this, start, end);
         notifyCursorUpdate(invalidation);
      };
   };

   // perfor the move and update the number of elements
   ImplList::moveAllTo((ImplList&) destination, start, end, destinationElement);
   uCount -= movedElements;
   destination.uCount += movedElements;

   if (destinationCursor && !parameters.isRemote())
      destinationCursor->setElement((parameters.getInsertionPosition() != RPBefore)
         ? (nextDestinationElement ? ((const ImplList&) destination).getPrevious(nextDestinationElement) : ((const ImplList&) destination).getLast())
         : (destinationElement ? ((const ImplList&) destination).getNext(destinationElement) : ((const ImplList&) destination).getFirst()));
}

void
GenericList::_swap(VirtualCollection& source) {
   VirtualCollection::_swap(source);
   GenericList temp;
   _moveAllTo(temp, ExtendedReplaceParameters());
   try {
      source.moveAllTo(*this);
      temp._moveAllTo(source, ExtendedReplaceParameters());
   }
   catch (...) {
      temp._moveAllTo(*this, ExtendedReplaceParameters());
      throw;
   };
}

void
GenericList::_swap(GenericList& source) {
   VirtualCollection::_swap(source);
   InvalidateNotification invalidation;
   notifyCursorUpdate(invalidation);
   source.notifyCursorUpdate(invalidation);
   ImplList::swap((ImplList&) source);
   int tempCount = uCount;
   uCount = source.uCount;
   source.uCount = tempCount;
}

// suppression methods
void
GenericList::_remove(const ExtendedSuppressParameters& parameters, GenericListCursor* cursor) {
   VirtualCollection::_remove(parameters, cursor);
   ImplListElement* removedElement = (ImplListElement*) GenericList::_getElement(parameters, cursor);

   // optimized notification
   RemoveElementNotification invalidation(*removedElement);
   if (cursor) {
      if ((parameters.getRelativePosition() != RPUndefined))
         invalidation.setOrigin(*cursor);
      if ((parameters.getRelativePosition() == RPExact))
         cursor->invalidate();
   };
   notifyCursorUpdate(invalidation);
   ImplList::remove(removedElement);
   if (parameters.isFree() && removedElement)
      delete removedElement;
   uCount--;
}

void
GenericList::_removeAll(const ExtendedSuppressParameters& parameters,
      const GenericListCursor* startCursor, const GenericListCursor* endCursor) {
   VirtualCollection::_removeAll(parameters, startCursor, endCursor);

   // optimized version for the case where all elements are removed.
   if (!startCursor && ! endCursor) {
      InvalidateNotification invalidation;
      notifyCursorUpdate(invalidation);
      if (parameters.isFree())
         ImplList::freeAll();
      else
         ImplList::removeAll();
      uCount = 0;
      return;
   };

   // Interval of elements to remove, their number and the suppression position.
   ImplListElement* start = startCursor ? startCursor->element() : nullptr;
   ImplListElement* end = endCursor ? endCursor->element() : nullptr;
   if (!retrieveRemovedInterval(start, end, parameters.getRelativePosition())
       || (startCursor && endCursor && ImplList::compareCursors(start, end) == CRGreater))
      return;

   int removedElements = ImplList::queryNumberOfElementsBetween(start, end);

   // notification of the cursors
   RemoveBoundNotification invalidation((const ImplList&) *this, start, end);
   notifyCursorUpdate(invalidation);

   // perform the suppression and update the number of elements.
   ImplList removedList;
   ImplList::moveAllTo(removedList, start, end);
   if (parameters.isFree())
      removedList.freeAll();
   else
      removedList.removeAll();
   uCount -= removedElements;
}

// replacement method
void
GenericList::_replace(ImplListElement* newElement, const ExtendedReplaceParameters& parameters,
      GenericListCursor* cursor) {
   VirtualCollection::_replace(newElement, parameters, cursor);
   AddGuardian<ImplListElement> guard(*this, newElement, parameters);

   ImplListElement* replacedElement = (ImplListElement*) GenericList::_getElement(
         (const ExtendedSuppressParameters&) parameters, cursor);
   if ((replacedElement == newElement) && !parameters.isDuplicate()) {
      guard.release();
      return;
   };

   newElement = guard.handleDuplication();
   ImplList::addAsNext(newElement, replacedElement, DNext);
   guard.release();
   ImplList::remove(replacedElement);

   // optimized notification
   try {
      ChangeNotification change(*replacedElement, *newElement);
      if (cursor) {
         if ((parameters.getRemovePosition() != RPUndefined))
            change.setOrigin(*cursor);
         if ((parameters.getRemovePosition() == RPExact))
            cursor->setElement(newElement);
      };
      notifyCursorUpdate(change);
   }
   catch (...) {
      if (parameters.isFree()) delete replacedElement;
      throw;
   };
   if (parameters.isFree()) delete replacedElement;
}

// query methods
EnhancedObject*
GenericList::_getElement(const ExtendedLocateParameters& parameters, const GenericListCursor* cursor) const {
   VirtualCollection::_getElement(parameters, cursor);

   ImplListElement* result
      = getElement(cursor ? cursor->element() : nullptr, parameters.getRelativePosition());
   AssumeCondition(result)
   return result;
}

/*************************************************/
/* Implementation of the class GenericListCursor */
/*************************************************/

ComparisonResult
GenericListCursor::_compare(const EnhancedObject& asource) const
{  ComparisonResult result = VirtualCollectionCursor::_compare(asource);
   if (result == CREqual)
       return getSupport().getImplementation().compareCursors(pileElement,
          ((const GenericListCursor&) castFromCopyHandler(asource)).pileElement);
   return result;
}

bool
GenericListCursor::_isPositionned(const GenericList::ExtendedLocateParameters& pos,
      const VirtualCollectionCursor* vccCursor) const {
   VirtualCollectionCursor::_isPositionned(pos, vccCursor);
   const auto& cursor = (const GenericListCursor&) *vccCursor;
   switch (pos.getRelativePosition()) {
      case VirtualCollection::RPAfter:
         return vccCursor
            ? (getSupport().getImplementation().getPrevious(pileElement) == cursor.pileElement)
                  && (pileElement != getSupport().getImplementation().getFirst())
            : getSupport().getImplementation().getFirst() == pileElement;
      case VirtualCollection::RPBefore:
         return vccCursor
            ? (getSupport().getImplementation().getNext(pileElement) == cursor.pileElement)
                  && (cursor.pileElement != getSupport().getImplementation().getFirst())
            : getSupport().getImplementation().getLast() == pileElement;
      case VirtualCollection::RPExact:
         return pileElement == cursor.pileElement;
      case VirtualCollection::RPUndefined:
         return true;
   };
   return false;
}

bool
GenericListCursor::_position(const Position& pos) {
   VirtualCollectionCursor::_position(pos);
   switch (pos.getDirection()) {
      case Position::DInvalid: pileElement = nullptr; return false;
      case Position::DStart: return (pileElement = getSupport().getImplementation().getFirst());
      case Position::DEnd: return (pileElement = getSupport().getImplementation().getLast());
      case Position::DExact: return (pileElement != nullptr);
      case Position::DForward:
         if (pileElement) {
            pileElement = getSupport().getImplementation().getNext(pileElement);
            return (pileElement != getSupport().getImplementation().getFirst())
               ? pileElement : (pileElement = nullptr);
         }
         else
            return (pileElement = getSupport().getImplementation().getFirst());
      case Position::DBackward:
         if (pileElement) {
            if (pileElement != getSupport().getImplementation().getFirst())
               return (pileElement = getSupport().getImplementation().getPrevious(pileElement));
            return (pileElement = nullptr);
         }
         else
            return (pileElement = getSupport().getImplementation().getLast());
      case Position::DAny:
         return (pileElement = getSupport().getImplementation().getLast());
   };
   return (pileElement = nullptr);
}

} // end of namespace COL
