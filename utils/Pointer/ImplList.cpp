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
// Library   : Collection
// Unit      : Low level implementation
// File      : ImplList.cpp
// Description :
//   Implementation of a class of double linked list. It provides an implementation
//   for the COL::List collection.
//

#include "Pointer/ImplList.h"

namespace COL {

void
ImplListElement::insertAsNext(ImplListElement& next) {
   AssumeCondition(ImplListElement::isValid() && next.ImplListElement::isValid())
   pileNext->pilePrevious = next.pilePrevious;
   next.pilePrevious->pileNext = pileNext;
   pileNext = &next;
   next.pilePrevious = this;
   AssumeCondition(ImplListElement::isValid() && next.ImplListElement::isValid())
}

ImplListElement*
ImplListElement::disconnect() {
   AssumeCondition(ImplListElement::isValid())
   ImplListElement* temp = pileNext;
   pileNext->pilePrevious = pilePrevious;
   pilePrevious->pileNext = pileNext;
   pileNext = this;
   pilePrevious = this;
   AssumeCondition(ImplListElement::isValid() && temp->ImplListElement::isValid())
   return (temp != this) ? temp : nullptr;
}

ImplListElement*
ImplListElement::disconnect(ImplListElement& last) {
   AssumeCondition(ImplListElement::isValid() && last.ImplListElement::isValid())
   ImplListElement *nextLast = last.pileNext;
   last.pileNext->pilePrevious = pilePrevious;
   pilePrevious->pileNext = last.pileNext;
   last.pileNext = this;
   pilePrevious = &last;
   AssumeCondition(ImplListElement::isValid() && nextLast->ImplListElement::isValid())
   return (nextLast != this) ? nextLast : nullptr;
}

/************************************/
/* Implementation of class ImplList */
/************************************/

void
ImplList::assign(const ImplList& source, bool doesDuplicate,
      const VirtualCast* retrieveRegistrationFromCopy) {
   EnhancedObject::operator=(source);
   if (this != &source) {
      if (doesDuplicate) {
         freeAll();
         addCopyAll(source, nullptr, nullptr, nullptr, retrieveRegistrationFromCopy);
      }
      else
         removeAll();
   };
}

int
ImplList::queryNumberOfElementsBetween(ImplListElement* min, ImplListElement* max) const {
   if ((min == nullptr) || (max == nullptr))
      return 0;
   int result = 1;
   if (min != max) {
      ImplListElement* origin = min;
      do {
         min = min->getNext();
         result++;
      } while ((min != max) && (min != pileFirst) && (min != origin));
   };
   AssumeCondition(min == max)
   return result;
}

ComparisonResult
ImplList::compareCursors(ImplListElement* fst, ImplListElement* snd) const {
   if (fst == snd)
      return CREqual;
   if ((fst == nullptr) || (snd == nullptr))
      return CRNonComparable;
   if (fst == pileFirst)
      return CRLess;
   if (snd == pileFirst)
      return CRGreater;

   ImplListElement *fstCursor=fst->getPrevious(), *sndCursor=snd->getPrevious();
   while ((fstCursor != snd) && (fstCursor != pileFirst)
         && (sndCursor != fst) && (sndCursor != pileFirst)) {
        fstCursor = fstCursor->getPrevious();
        sndCursor = sndCursor->getPrevious();
   };

   if (fstCursor == snd)
      return CRGreater;
   if (fstCursor == pileFirst)
      return CRLess;
   if (sndCursor == fst)
      return CRLess;
   return CRGreater; // sndCursor == pileFirst
}

void
ImplList::addAsNext(ImplListElement* newElement, ImplListElement* cursor, Direction dir) {
   AssumeAllocation(newElement)
   AssumeCondition(newElement->isAtomic() && (((cursor != nullptr) && (pileFirst != nullptr))
         || ((cursor == nullptr) && (pileFirst == nullptr))))
   if (cursor) {
      if (dir == DNext)
         cursor->insertAsNext(*newElement);
      else {
         newElement->insertAsNext(*cursor);
         if (cursor == pileFirst)
            pileFirst = newElement;
      };
   }
   else {
      AssumeCondition(pileFirst == nullptr)
      pileFirst = newElement;
   };
}

void
ImplList::remove(ImplListElement* oldElement) {
   AssumeCondition(oldElement)
   ImplListElement* disconnectedList = oldElement->disconnect();
   if (oldElement == pileFirst)
      pileFirst = disconnectedList;
}

void
ImplList::freeAll() {
   if (pileFirst) {
      ImplListElement *last = pileFirst->getPrevious(),*pileNext = nullptr;
      try {
         while (pileFirst && (pileFirst != last)) {
            pileNext = pileFirst->getNext();
            delete pileFirst;
            pileFirst = pileNext;
         };
      } catch (...) {
         if (pileNext) {
            pileNext->setPrevious(last);
            last->setNext(pileNext);
         };
         pileFirst = pileNext;
         throw;
      };

      AssumeCondition(pileFirst)
      delete pileFirst;
      pileFirst = nullptr;
   };
}

void
ImplList::removeAllExcept(ImplListElement* lastElement) {
   while (pileFirst && (pileFirst != lastElement))
      pileFirst = pileFirst->disconnect();
   AssumeCondition(!lastElement || pileFirst == lastElement)
   if (lastElement) {
      ImplListElement* pileSecond = pileFirst->getNext();
      while (pileSecond && (pileSecond != lastElement))
         pileSecond = pileSecond->disconnect();
   };
}

void
ImplList::freeAllExcept(ImplListElement* lastElement) {
   while (pileFirst && (pileFirst != lastElement)) {
      ImplListElement* deleteElement = pileFirst;
      pileFirst = pileFirst->disconnect();
      if (deleteElement) delete deleteElement;
   };
   AssumeCondition(!lastElement || pileFirst == lastElement)
   if (lastElement) {
      ImplListElement* second = pileFirst->getNext();
      while (second && (second != lastElement)) {
         ImplListElement* deleteElement = second;
         second = second->disconnect();
         if (deleteElement) delete deleteElement;
      };
   };
}

bool
ImplList::isValid() const {
   if (pileFirst) {
      ImplListElement* cursor = pileFirst;
      while (cursor->getNext() && (cursor->getNext() != pileFirst)) {
         if (!cursor->isValid())
            return false;
         cursor = cursor->getNext();
      }
      if (cursor->getNext() != pileFirst)
         return false;
   };
   return true;
}

void
ImplList::addCopyAll(const ImplList& source, ImplListElement* start,
      ImplListElement* end, ImplListElement* destinationCursor,
      const VirtualCast* pvcRetrieveRegistrationFromCopy) {
   if (source.pileFirst) {
      const VirtualCastWithElement* retrieveRegistrationFromCopy = (const VirtualCastWithElement*) pvcRetrieveRegistrationFromCopy;
      
      if (start == nullptr)
         start = source.pileFirst;
      end = (end == nullptr) ? source.pileFirst : end->getNext();
      if (destinationCursor) {
         ImplListElement* nextDestination = destinationCursor->getNext();
         do {
            addAsNext(pvcRetrieveRegistrationFromCopy
               ? (ImplListElement*) retrieveRegistrationFromCopy
                     ->castFrom(start->createCopy(), start)
               : start->createSCopy(), destinationCursor, DNext);
            start = start->getNext();
         } while ((start != end) && (start != source.pileFirst) && (start != destinationCursor));
         if ((start != end) && (start != source.pileFirst)) {
            start = nextDestination;
            do {
               addAsNext(pvcRetrieveRegistrationFromCopy
                  ? (ImplListElement*) retrieveRegistrationFromCopy
                        ->castFrom(start->createCopy(), start)
                  : start->createSCopy(), destinationCursor, DNext);
               start = start->getNext();
            } while ((start != end) && (start != source.pileFirst));
         };
      }
      else {
         do {
            addAsLast(pvcRetrieveRegistrationFromCopy
               ? (ImplListElement*) retrieveRegistrationFromCopy
                     ->castFrom(start->createCopy(), start)
               : start->createSCopy());
            start = start->getNext();
         } while ((start != end) && (start != source.pileFirst));
      };
   };
}

void
ImplList::moveAllTo(ImplList& destination, ImplListElement* start,
      ImplListElement* end, ImplListElement* destinationCursor, bool doesAssertDistinction) {
   if (isEmpty())
      return;
   if (start == nullptr) { // integral transfer
      if (&destination == this)
         return;
      if (destination.pileFirst == nullptr) { // integral transfer into an empty list
         destination.pileFirst = pileFirst;
         pileFirst = nullptr;
         return;
      };
      if (destinationCursor == nullptr) { // integral transfer at the beginning of a list
         destination.getLast()->insertAsNext(*pileFirst);
         destination.pileFirst = pileFirst;
      }
      else // integral transfer elsewhere
         destinationCursor->insertAsNext(*pileFirst);
      pileFirst = nullptr;
   };

   // partial transfer
   if (end == nullptr) end = getLast();
   if (!doesAssertDistinction) { // verification of input data
      ImplListElement* last = getLast();
      ImplListElement* cursor = start;
      while ((cursor != end) && (cursor != last) && (cursor != destinationCursor))
         cursor = cursor->getNext();
      if ((cursor == destinationCursor)
            || ((&destination == this) && (start == pileFirst) && (destinationCursor==nullptr)))
         return;
      if (cursor != end)
         end = last;
   };

   // sure transfer from [start, end] to [destinationCursor]
   // suppression part
   ImplListElement* tempStart = start->disconnect(*end);
   if (start == pileFirst)
      pileFirst = tempStart;
   if (destination.pileFirst == nullptr) // transfer into an empty list
      destination.pileFirst = start;
   else if (destinationCursor == nullptr){ // transfer at the beginning of a list
      destination.getLast()->insertAsNext(*start);
      destination.pileFirst = start;
   }
   else // Transfert at the middle or at end of a list
      destinationCursor->insertAsNext(*start);
}

bool
ImplList::foreachDo(std::function<bool (ImplListElement&)> function,
      ImplListElement* prevStart, ImplListElement* nextEnd) const {
   if (!pileFirst)
      return true;
   bool result = true;
   ImplListElement* cursor = prevStart ? prevStart->getNext() : pileFirst;
   if (!prevStart || (cursor != pileFirst)) {
      nextEnd = (nextEnd ? nextEnd->getPrevious() : pileFirst->getPrevious());
      ImplListElement* next = nullptr;
      for (; result && cursor != nextEnd; cursor = next) {
         next = cursor->getNext();
         result = function(*cursor);
      };
      if (result)
         result = function(*nextEnd);
   };
   return result;
}

bool
ImplList::foreachReverseDo(std::function<bool (ImplListElement&)> function,
      ImplListElement* nextStart, ImplListElement* prevEnd) const {
   if (!pileFirst)
      return true;
   bool result = true;
   if (nextStart != pileFirst) {
      ImplListElement* cursor = nextStart ? nextStart->getPrevious() : pileFirst->getPrevious();
      prevEnd = (prevEnd ? prevEnd->getNext() : pileFirst);
      for (; result && cursor != prevEnd; cursor = cursor->getPrevious())
         result = function(*cursor);
      if (result)
         result = function(*cursor);
   };
   return result;
}

} // end of namespace COL
