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
// File      : Special.cpp
// Description :
//   Implementation of collections with a finite number of elements.
//

#include "Collection/ConcreteCollection/Special.h"
// #include "Collection/Collection.hpp"

namespace COL {

/************************************************/
/* Implementation of the class UniqueCollection */
/************************************************/

void
UniqueCollection::_addAll(const VirtualCollection& vcSource, const ExtendedInsertionParameters& parameters,
      VirtualCollectionCursor* cursor, const VirtualCollectionCursor* startSource,
      const VirtualCollectionCursor* endSource) {
   if (vcSource.isSpecial()) {
      AssumeCondition(dynamic_cast<const SpecialCollection*>(&vcSource))
      const auto& source = (const SpecialCollection&) vcSource;
      if (source.isUniqueType())
         _addAll((const UniqueCollection&) source, parameters, (Cursor*) cursor,
               (Cursor*) startSource, (Cursor*) endSource);
      else if (source.isDoubleType())
         _addAll((const DoubleCollection&) source, parameters, (Cursor*) cursor,
               (DoubleCollectionCursor*) startSource, (DoubleCollectionCursor*) endSource);
      else {
         AssumeCondition(dynamic_cast<const EmptyCollection*>(&source))
      }
   }
   else
      inherited::paddAll(vcSource, parameters, cursor, startSource, endSource);
}

void
UniqueCollection::_moveTo(VirtualCollection& vcDestination, const ExtendedReplaceParameters& parameters,
      VirtualCollectionCursor* cursor, VirtualCollectionCursor* destinationCursor) {
   if (vcDestination.isSpecial()) {
      AssumeCondition(dynamic_cast<const SpecialCollection*>(&vcDestination))
      const auto& destination = (const SpecialCollection&) vcDestination;
      if (destination.isUniqueType())
         _moveTo((UniqueCollection&) destination, parameters, (Cursor*) cursor,
               (Cursor*) destinationCursor);
      else if (destination.isDoubleType())
         _moveTo((DoubleCollection&) destination, parameters, (Cursor*) cursor,
               (DoubleCollectionCursor*) destinationCursor);
      else {
         AssumeCondition(dynamic_cast<const EmptyCollection*>(&destination))
      }
   }
   else
      inherited::pmoveTo(vcDestination, parameters, cursor, destinationCursor);
}

void
UniqueCollection::_moveAllTo(VirtualCollection& vcDestination, const ExtendedReplaceParameters& parameters,
      const VirtualCollectionCursor* startCursor, const VirtualCollectionCursor* endCursor,
      VirtualCollectionCursor* destinationCursor) {
   if (vcDestination.isSpecial()) {
      AssumeCondition(dynamic_cast<const SpecialCollection*>(&vcDestination))
      const auto& destination = (const SpecialCollection&) vcDestination;
      if (destination.isUniqueType())
         _moveAllTo((UniqueCollection&) destination, parameters, (Cursor*) startCursor,
               (Cursor*) endCursor, (Cursor*) destinationCursor);
      else if (destination.isDoubleType())
         _moveAllTo((DoubleCollection&) destination, parameters, (Cursor*) startCursor,
               (Cursor*) endCursor, (DoubleCollectionCursor*) destinationCursor);
      else {
         AssumeCondition(dynamic_cast<const EmptyCollection*>(&destination))
      }
   }
   else
      inherited::pmoveAllTo(vcDestination, parameters, startCursor, endCursor, destinationCursor);
}

void
UniqueCollection::_fullAssign(const VirtualCollection& source, const ExtendedReplaceParameters& parameters) {
   inherited::_fullAssign(source, parameters);
   AssumeCondition(source.count() <= 1)
   if (this != &source) {
      InvalidateNotification invalidation;
      notifyCursorUpdate(invalidation);
      EnhancedObject* oldElement = peoElement;
      peoElement = source.isEmpty() ? nullptr
         : (parameters.isDuplicate() ? (parameters.hasCast()
            ? parameters.getCast().castFrom(source.getFirst().createCopy())
            : source.getFirst().createCopy())
         : &source.getSFirst());
      if (parameters.isFree() && (oldElement != nullptr))
         delete oldElement;
   };
}

void
UniqueCollection::_fullAssign(const UniqueCollection& source, const ExtendedReplaceParameters& parameters) {
   inherited::_fullAssign(source, parameters);
   if (this != &source) {
      InvalidateNotification invalidation;
      notifyCursorUpdate(invalidation);
      EnhancedObject* oldElement = peoElement;
      peoElement = ((source.peoElement != nullptr) && parameters.isDuplicate())
         ? (parameters.hasCast() ? parameters.getCast().castFrom(source.peoElement->createCopy())
                                 : source.peoElement->createCopy())
         : source.peoElement;
      if (parameters.isFree() && (oldElement != nullptr))
         delete oldElement;
   };
}

void
UniqueCollection::_addAll(const UniqueCollection& source, const ExtendedInsertionParameters& parameters,
      Cursor* cursor, const Cursor* startSource, const Cursor* endSource) {
   inherited::_addAll(source, parameters, cursor, startSource, endSource);
   AssumeCondition((peoElement == nullptr) || (source.peoElement == nullptr))
   if (source.peoElement != nullptr) {
      peoElement = parameters.isDuplicate()
         ? (parameters.hasCast() ? parameters.getCast().castFrom(source.peoElement->createCopy())
                                 : source.peoElement->createCopy())
         : source.peoElement;
      if (cursor && !parameters.isRemote())
         cursor->setValid();
   };
}

void
UniqueCollection::_addAll(const DoubleCollection& source, const ExtendedInsertionParameters& parameters,
      Cursor* cursor, const DoubleCollectionCursor* startSource, const DoubleCollectionCursor* endSource) {
   inherited::_addAll(source, parameters, cursor, startSource, endSource);
   if (peoElement != nullptr) {
      AssumeCondition(source.isEmpty()
            || (startSource && startSource->isSecondMode() && endSource && endSource->isFirstMode()))
   }
   else {
      if (!source.hasFirst() || !source.hasSecond()) {
         if (source.hasFirst())
            peoElement = parameters.isDuplicate()
               ? (parameters.hasCast() ? parameters.getCast().castFrom(source.first().createCopy())
                                       : source.first().createCopy())
               : &source.first();
         else if (source.hasSecond())
            peoElement = parameters.isDuplicate()
               ? (parameters.hasCast() ? parameters.getCast().castFrom(source.second().createCopy())
                                       : source.second().createCopy())
               : &source.second();
      }
      else { // source.hasFirst() && source.hasSecond()
         if (startSource && startSource->isValid()) {
            AssumeCondition(startSource->isSecondMode()
                  || (endSource && endSource->isValid() && endSource->isFirstMode()))
            peoElement = parameters.isDuplicate()
               ? (parameters.hasCast() ? parameters.getCast().castFrom(startSource->elementAt().createCopy())
                                       : startSource->elementAt().createCopy())
               : &startSource->elementSAt();
         }
         else if (endSource && endSource->isValid()) {
            AssumeCondition(endSource->isFirstMode())
            peoElement = parameters.isDuplicate()
               ? (parameters.hasCast() ? parameters.getCast().castFrom(endSource->elementAt().createCopy())
                                       : endSource->elementAt().createCopy())
               : &endSource->elementSAt();
         }
         else
            AssumeUncalled
      };
      if (peoElement && cursor && !parameters.isRemote())
         cursor->setValid();
   };
}

void
UniqueCollection::_moveTo(UniqueCollection& destination, const ExtendedReplaceParameters& parameters,
      Cursor* cursor, Cursor* destinationCursor) {
   inherited::_moveTo(destination, parameters, cursor, destinationCursor);
   if (this != &destination) {
      AssumeCondition((destination.peoElement == nullptr) && (peoElement != nullptr))
      destination.peoElement = peoElement;
      peoElement = nullptr;
      InvalidateNotification invalidation;
      notifyCursorUpdate(invalidation);
      if (destinationCursor && !parameters.isRemote())
         destinationCursor->setValid();
   };
}

void
UniqueCollection::_moveTo(DoubleCollection& destination, const ExtendedReplaceParameters& parameters,
      Cursor* cursor, DoubleCollectionCursor* destinationCursor) {
   inherited::_moveTo(destination, parameters, cursor, destinationCursor);
   AssumeCondition((!destination.hasFirst() || !destination.hasSecond()) && (peoElement != nullptr))

   if (!destination.hasFirst()) {
      if (!destination.hasSecond() || parameters.getInsertion().isUndefined()
            || ((destinationCursor && destinationCursor->isValid())
               ? parameters.getInsertion().isBefore() : parameters.getInsertion().isAfter())) {
         destination.setFirst(parameters.hasCast() ?
               parameters.getCast().castFrom(*peoElement) : *peoElement);
         peoElement = nullptr;
         InvalidateNotification invalidation;
         notifyCursorUpdate(invalidation);
         if (destinationCursor && !parameters.isRemote())
            destinationCursor->setFirstMode();
         return;
      };
   }
   else { // destination.hasFirst() && !destination.hasSecond()
      if (parameters.getInsertion().isUndefined()
            || ((destinationCursor && destinationCursor->isValid())
               ? parameters.getInsertion().isAfter() : parameters.getInsertion().isBefore())) {
         destination.setSecond(parameters.hasCast() ?
               parameters.getCast().castFrom(*peoElement) : *peoElement);
         peoElement = nullptr;
         InvalidateNotification invalidation;
         notifyCursorUpdate(invalidation);
         if (destinationCursor && !parameters.isRemote())
            destinationCursor->setSecondMode();
         return;
      };
   };
   inherited::pmoveTo(destination, parameters, cursor, destinationCursor);
}

void
UniqueCollection::_moveAllTo(UniqueCollection& destination, const ExtendedReplaceParameters& parameters,
      const Cursor* startCursor, const Cursor* endCursor, Cursor* destinationCursor) {
   inherited::_moveAllTo(destination, parameters, startCursor, endCursor, destinationCursor);
   if (peoElement != nullptr) {
      if ((this == &destination)
            || (startCursor && startCursor->isValid() && parameters.getSuppress().isAfter())
            || (endCursor && endCursor->isValid() && parameters.getSuppress().isBefore()))
         return;
      AssumeCondition(destination.peoElement == nullptr)
      destination.peoElement = peoElement;
      peoElement = nullptr;
      InvalidateNotification invalidation;
      notifyCursorUpdate(invalidation);
      if (destinationCursor && !parameters.isRemote())
         destinationCursor->setValid();
   };
}

void
UniqueCollection::_moveAllTo(DoubleCollection& destination, const ExtendedReplaceParameters& parameters,
      const Cursor* startCursor, const Cursor* endCursor, DoubleCollectionCursor* destinationCursor) {
   inherited::_moveAllTo(destination, parameters, startCursor, endCursor, destinationCursor);
   if (peoElement != nullptr) {
      if ((startCursor && startCursor->isValid() && parameters.getSuppress().isAfter())
            || (endCursor && endCursor->isValid() && parameters.getSuppress().isBefore()))
         return;
      ExtendedInsertionParameters params(parameters);
      destination._add(peoElement, params.releaseFreeOnError(), destinationCursor);
      peoElement = nullptr;
      InvalidateNotification invalidation;
      notifyCursorUpdate(invalidation);
   };
}

int
UniqueCollection::queryInternCount(const ExtendedLocateParameters& parameters,
      const Cursor* startCursor, const Cursor* endCursor) const {
   if (peoElement == nullptr)
      return 0;
   if ((startCursor && startCursor->isValid() && parameters.isFirstExcluded())
            || (endCursor && endCursor->isValid() && parameters.isLastExcluded()))
      return 0;
   return 1;
}

/************************************************/
/* Implementation of the class DoubleCollection */
/************************************************/

int
DoubleCollection::queryInternCount(const ExtendedLocateParameters& parameters,
      const Cursor* startCursor, const Cursor* endCursor) const {
   int result = 2;
   if (startCursor && startCursor->isValid()) {
      if (endCursor && endCursor->isValid()) {
         if (startCursor->isSecondMode())
            --result;
         if (endCursor->isFirstMode())
            --result;
         if (parameters.isFirstExcluded())
            --result;
         if (parameters.isLastExcluded())
            --result;
         return (result > 0) ? result : 0;
      };
      if (startCursor->isSecondMode())
         --result;
      else if (peoSecond == nullptr)
         --result;
      if (parameters.isFirstExcluded())
         --result;
      return result;
   };
   if (endCursor && endCursor->isValid()) {
      if (endCursor->isFirstMode())
         --result;
      else if (peoFirst == nullptr)
         --result;
      if (parameters.isLastExcluded())
         --result;
      return result;
   };
   if (peoFirst == nullptr)
      --result;
   if (peoSecond == nullptr)
      --result;
   return result;
}

void
DoubleCollection::_fullAssign(const VirtualCollection& source, const ExtendedReplaceParameters& parameters) {
   inherited::_fullAssign(source, parameters);
   int sourceCount = source.count();
   AssumeCondition(sourceCount <= 2)
   if (this != &source) {
      InvalidateNotification invalidation;
      notifyCursorUpdate(invalidation);
      EnhancedObject *oldFirst = peoFirst, *oldSecond = peoSecond;
      peoFirst = (sourceCount == 0) ? nullptr
         : (parameters.isDuplicate()
            ? (parameters.hasCast() ? parameters.getCast().castFrom(source.getFirst().createCopy())
                                    : source.getFirst().createCopy())
            : &source.getSFirst());
      peoSecond = (sourceCount < 2) ? nullptr
         : (parameters.isDuplicate()
            ? (parameters.hasCast() ? parameters.getCast().castFrom(source.getLast().createCopy())
                                    : source.getLast().createCopy())
            : &source.getSLast());
      if (parameters.isFree()) {
         if (oldFirst != nullptr)
            delete oldFirst;
         if (oldSecond != nullptr)
            delete oldSecond;
      };
   };
}

void
DoubleCollection::_fullAssign(const DoubleCollection& source, const ExtendedReplaceParameters& parameters) {
   inherited::_fullAssign(source, parameters);
   if (this != &source) {
      InvalidateNotification invalidation;
      notifyCursorUpdate(invalidation);
      EnhancedObject *oldFirst = peoFirst, *oldSecond = peoSecond;
      peoFirst = ((source.peoFirst != nullptr) && parameters.isDuplicate())
         ? (parameters.hasCast() ? parameters.getCast().castFrom(source.peoFirst->createCopy())
                                 : source.peoFirst->createCopy())
         : source.peoFirst;
      peoSecond = ((source.peoSecond != nullptr) && parameters.isDuplicate())
         ? (parameters.hasCast() ? parameters.getCast().castFrom(source.peoSecond->createCopy())
                                    : source.peoSecond->createCopy())
         : source.peoSecond;
      if (parameters.isFree()) {
         if (oldFirst != nullptr)
            delete oldFirst;
         if (oldSecond != nullptr)
            delete oldSecond;
      };
   };
}

class DoubleCollection::InternMoveNotification : public CursorNotification {
  private:
   enum Move { MNoMove, MFirstToSecond, MSecondToFirst };
   typedef CursorNotification inherited;
   Move mMove;

  protected:
   void update(Cursor& cursor)
      {  if (cursor.isFirstMode() && (mMove == MFirstToSecond))
            cursor.setSecondMode();
         else if (cursor.isSecondMode() && (mMove == MSecondToFirst))
            cursor.setFirstMode();
      }

  public:
   InternMoveNotification() : mMove(MNoMove) {}
   InternMoveNotification(const InternMoveNotification& source) = default;
   DDefineAssign(InternMoveNotification)
   DefineCursorNotificationMethods(InternMoveNotification, Cursor)

   void setFirstToSecond() { mMove = MFirstToSecond; }
   void setSecondModeToFirst() { mMove = MSecondToFirst; }
};

void
DoubleCollection::_add(EnhancedObject* anObject, const ExtendedInsertionParameters& parameters,
      VirtualCollectionCursor* pvccCursor) {
   inherited::_add(anObject, parameters, pvccCursor);
   AddGuardian<EnhancedObject> guard(*this, anObject, parameters);
   AssumeCondition((peoFirst == nullptr) || (peoSecond == nullptr))
   Cursor* cursor = (Cursor*) pvccCursor;
   if (peoFirst == nullptr) {
      if ((cursor && cursor->isValid()) ? parameters.isAfter() : parameters.isBefore()) {
         peoFirst = peoSecond;
         peoSecond = guard.handleDuplication();
         guard.release();
         if (cursor && !parameters.isRemote())
            cursor->setSecondMode();
         InternMoveNotification internMove;
         internMove.setSecondModeToFirst();
         if (cursor && !parameters.isRemote())
            internMove.setOrigin(*cursor);
      }
      else {
         peoFirst = guard.handleDuplication();
         guard.release();
         if (cursor && !parameters.isRemote())
            cursor->setFirstMode();
      };
   }
   else { // peoFirst != nullptr && peoSecond == nullptr
      if ((pvccCursor && cursor->isValid()) ? parameters.isBefore() : parameters.isAfter()) {
         peoSecond = peoFirst;
         peoFirst = guard.handleDuplication();
         guard.release();
         if (cursor && !parameters.isRemote())
            cursor->setFirstMode();
         InternMoveNotification internMove;
         internMove.setFirstToSecond();
         if (cursor && !parameters.isRemote())
            internMove.setOrigin(*cursor);
      }
      else {
         peoSecond = guard.handleDuplication();
         guard.release();
         if (cursor && !parameters.isRemote())
            cursor->setFirstMode();
      };
   };
}

void
DoubleCollection::_addAll(const UniqueCollection& source, const ExtendedInsertionParameters& parameters,
      Cursor* cursor, const UniqueCollectionCursor* startSource, const UniqueCollectionCursor* endSource) {
   inherited::_addAll(source, parameters, cursor, startSource, endSource);
   AssumeCondition((peoFirst == nullptr) || (peoSecond == nullptr) || !source.hasElement())
   if (source.hasElement()) {
      AssumeCondition(!parameters.isFreeOnError() || parameters.isDuplicate())
      DoubleCollection::_add(&source.element(), parameters, cursor);
   };
}

void
DoubleCollection::_addAll(const DoubleCollection& source, const ExtendedInsertionParameters& parameters,
      Cursor* cursor, const Cursor* startSource, const Cursor* endSource) {
   inherited::_addAll(source, parameters, cursor, startSource, endSource);
   if ((peoFirst == nullptr) && (peoSecond == nullptr)) {
      if (startSource && startSource->isValid() && startSource->isSecondMode()) {
         if (!endSource || !endSource->isValid() || endSource->isSecondMode()) {
            AssumeCondition(source.peoSecond)
            peoSecond = parameters.isDuplicate()
               ? (parameters.hasCast() ? parameters.getCast().castFrom(source.peoSecond->createCopy())
                                       : source.peoSecond->createCopy())
               : source.peoSecond;
            if (cursor && !parameters.isRemote())
               cursor->setSecondMode();
            return;
         };
      };
      if (endSource && endSource->isValid() && endSource->isFirstMode()) {
         if (!startSource || !startSource->isValid() || startSource->isFirstMode()) {
            AssumeCondition(source.peoFirst)
            peoFirst = parameters.isDuplicate() ? (parameters.hasCast()
               ? parameters.getCast().castFrom(source.peoFirst->createCopy())
               : source.peoFirst->createCopy())
               : source.peoFirst;
            if (cursor && !parameters.isRemote())
               cursor->setFirstMode();
            return;
         };
      };
      peoFirst = parameters.isDuplicate() ? (parameters.hasCast()
         ? parameters.getCast().castFrom(source.peoFirst->createCopy())
         : source.peoFirst->createCopy())
         : source.peoFirst;
      peoSecond = parameters.isDuplicate() ? (parameters.hasCast()
         ? parameters.getCast().castFrom(source.peoSecond->createCopy())
         : source.peoSecond->createCopy())
         : source.peoSecond;
      if (cursor && !parameters.isRemote())
         cursor->setSecondMode();
      return;
   };
   if (startSource && startSource->isValid()) { // peoFirst != nullptr || peoSecond != nullptr
      if (endSource && endSource->isValid()) {
         if (startSource->isFirstMode()) {
            AssumeCondition(!endSource->isSecondMode())
            DoubleCollection::_add(&startSource->elementSAt(), parameters, cursor);
         }
         else if (endSource->isSecondMode()) // pcStartSource->isSecondMode()
            DoubleCollection::_add(&startSource->elementSAt(), parameters, cursor);
      }
      else {
         AssumeCondition(!startSource->isFirstMode() || !source.peoSecond)
         DoubleCollection::_add(&startSource->elementSAt(), parameters, cursor);
      };
   }
   else if (endSource && endSource->isValid()) { // (peoFirst != nullptr || peoSecond != nullptr) && !pcStartSource
      AssumeCondition(!endSource->isSecondMode() || !source.peoFirst)
      DoubleCollection::_add(&endSource->elementSAt(), parameters, cursor);
   }
   else {
      if (source.peoFirst)
         DoubleCollection::_add(source.peoFirst, parameters, cursor);
      if (source.peoSecond)
         DoubleCollection::_add(source.peoSecond, parameters, cursor);
   };
}

class DoubleCollection::RemoveNotification : public CursorNotification, protected ExtendedParameters {
  private:
   typedef CursorNotification inherited;

  protected:
   DefineExtendedParameters(4, ExtendedParameters)
   DefineSubExtendedParameters(SuppressFirst, 1, INHERITED)
   DefineSubExtendedParameters(SuppressSecond, 1, SuppressFirst)
   DefineSubExtendedParameters(OriginFirst, 1, SuppressSecond)
   DefineSubExtendedParameters(OriginSecond, 1, OriginFirst)
   
   void update(Cursor& cursor)
      {  if (cursor.isFirstMode() && hasSuppressFirstField())
            cursor.setInvalid();
         else if (cursor.isSecondMode() && hasSuppressSecondField())
            cursor.setInvalid();
      }

  public:
   RemoveNotification(const DoubleCollection& support)
      {  if (support.hasFirst())
            mergeOriginFirstField(1);
         if (support.hasSecond())
            mergeOriginSecondField(1);
      }
   RemoveNotification(const RemoveNotification& source)
      :  inherited(source), ExtendedParameters(source) {}
   DDefineAssign(RemoveNotification)
   DefineCursorNotificationMethods(RemoveNotification, Cursor)

   void compareWith(const DoubleCollection& support)
      {  if (!support.hasFirst() && hasOriginFirstField())
            mergeSuppressFirstField(1);
         if (!support.hasSecond() && hasOriginSecondField())
            mergeSuppressSecondField(1);
      }
   bool isValid() const override
      { return inherited::isValid() && (hasSuppressFirstField() || hasSuppressSecondField()); }
};

class DoubleCollection::SuppressionNotification : public CursorNotification {
  private:
   typedef CursorNotification inherited;
   Location lLocation;

  protected:
   void update(Cursor& cursor)
      {  if (cursor.isFirstMode() && (lLocation == LFirst))
            cursor.setInvalid();
         if (cursor.isSecondMode() && (lLocation == LSecond))
            cursor.setInvalid();
      }

  public:
   SuppressionNotification(Location location) : lLocation(location) {}
   SuppressionNotification(const SuppressionNotification& source) = default;
   DDefineAssign(SuppressionNotification)
   DefineCursorNotificationMethods(SuppressionNotification, Cursor)
};

void
DoubleCollection::_moveTo(UniqueCollection& destination, const ExtendedReplaceParameters& parameters,
      Cursor* cursor, UniqueCollectionCursor* destinationCursor) {
   inherited::_moveTo(destination, parameters, cursor, destinationCursor);
   Location location = getLocation(parameters.getRemovePosition(), cursor);
   EnhancedObject*& move = (location == LSecond) ? peoSecond : peoFirst;
   AssumeCondition(move)
   destination.setElement(parameters.hasCast() ? parameters.getCast().castFrom(*move) : *move);
   move = nullptr;
   if (cursor && parameters.getSuppress().isExact())
      cursor->setInvalid();
   SuppressionNotification suppression(location);
   if (cursor)
      suppression.setOrigin(*cursor);
   notifyCursorUpdate(suppression);
   if (destinationCursor && !parameters.isRemote())
      destinationCursor->setValid();
}

class DoubleCollection::SwapNotification : public CursorNotification {
  protected:
   void update(Cursor& cursor)
      {  if (cursor.isFirstMode())
            cursor.setSecondMode();
         else if (cursor.isSecondMode())
            cursor.setFirstMode();
      }

  public:
   SwapNotification() {}
   SwapNotification(const SwapNotification& source) : CursorNotification(source) {}
   DefineCursorNotificationMethods(SwapNotification, Cursor)
};

void
DoubleCollection::_moveTo(DoubleCollection& destination, const ExtendedReplaceParameters& parameters,
      Cursor* cursor, Cursor* destinationCursor) {
   inherited::_moveTo(destination, parameters, cursor, destinationCursor);
   Location location = getLocation(parameters.getRemovePosition(), cursor);
   EnhancedObject*& move = (location == LSecond) ? peoSecond : peoFirst;
   AssumeCondition(move)
   if (this != &destination) {
      destination._add(move, parameters.getInsertion(), destinationCursor);
      move = nullptr;
      if (cursor && parameters.getSuppress().isExact())
         cursor->setInvalid();
      SuppressionNotification suppression(location);
      if (cursor)
         suppression.setOrigin(*cursor);
      notifyCursorUpdate(suppression);
   }
   else {
      if (peoSecond && peoFirst) {
         bool doesSwap = false;
         if (move == peoFirst)
            doesSwap = (destinationCursor && destinationCursor->isValid())
               ? (destinationCursor->isSecondMode() && parameters.getInsertion().isAfter())
               : parameters.getInsertion().isBefore();
         else
            doesSwap = (destinationCursor && destinationCursor->isValid())
               ? (destinationCursor->isFirstMode() && parameters.getInsertion().isBefore())
               : parameters.getInsertion().isAfter();
         if (doesSwap) {
            EnhancedObject* temp = peoFirst;
            peoFirst = peoSecond;
            peoSecond = temp;
            SwapNotification swap;
            if (destinationCursor && !parameters.isRemote())
               swap.setOrigin(*destinationCursor);
            notifyCursorUpdate(swap);
         };
      };
   };
}

void
DoubleCollection::_moveAllTo(UniqueCollection& destination, const ExtendedReplaceParameters& parameters,
      const Cursor* startCursor, const Cursor* endCursor, UniqueCollectionCursor* destinationCursor) {
   inherited::_moveAllTo(destination, parameters, startCursor, endCursor, destinationCursor);
   Location location = LUndefined;
   if (startCursor && startCursor->isValid()) {
      if (endCursor && endCursor->isValid()) {
         if (endCursor->isFirstMode() && startCursor->isSecondMode())
            return;
         if (endCursor->isFirstMode()) { // startCursor->isFirstMode()
            if (!parameters.getSuppress().isLastExcluded())
               location = LFirst;
         }
         else if (startCursor->isSecondMode()) { // endCursor->isSecondMode()
            if (!parameters.getSuppress().isFirstExcluded())
               location = LSecond;
         }
         else { // startCursor->isFirstMode() && endCursor->isSecondMode()
            if (!parameters.getSuppress().isFirstExcluded())
               location = LFirst;
            if (!parameters.getSuppress().isLastExcluded()) {
               AssumeCondition(location == LUndefined)
               location = LSecond;
            };
         };
      }
      else { // startCursor->isValid() && endCursor == nullptr
         if (!parameters.getSuppress().isFirstExcluded())
            location = startCursor->isFirstMode() ? LFirst : LSecond;
         if (startCursor->isFirstMode() && peoSecond) {
            AssumeCondition(location == LUndefined)
            location = LSecond;
         };
      }
   }
   else if (endCursor && endCursor->isValid()) { // endCursor->isValid() && startCursor == nullptr
      if (!parameters.getSuppress().isLastExcluded())
         location = endCursor->isFirstMode() ? LFirst : LSecond;
      if (endCursor->isSecondMode() && peoFirst) {
         AssumeCondition(location == LUndefined)
         location = LFirst;
      };
   }
   else { // startCursor == nullptr && endCursor == nullptr
      AssumeCondition(!peoFirst || !peoSecond)
      if (peoFirst || peoSecond)
         location = (peoFirst != nullptr) ? LFirst : LSecond;
   };

   if (location != LUndefined) {
      EnhancedObject*& move = (location == LFirst) ? peoFirst : peoSecond;
      AssumeCondition(move != nullptr)
      destination.setElement(parameters.hasCast() ? parameters.getCast().castFrom(*move) : *move);
      move = nullptr;
      SuppressionNotification suppression(location);
      notifyCursorUpdate(suppression);
      if (destinationCursor && !parameters.isRemote())
         destinationCursor->setValid();
   };
}

void
DoubleCollection::_moveAllTo(DoubleCollection& destination, const ExtendedReplaceParameters& parameters,
      const Cursor* startCursor, const Cursor* endCursor, Cursor* destinationCursor) {
   inherited::_moveAllTo(destination, parameters, startCursor, endCursor, destinationCursor);
   if (this == &destination) {
      if (peoFirst && peoSecond) {
         bool doesSwap = false;
         const ExtendedSuppressParameters& suppressParameters = parameters;
         if (startCursor && (suppressParameters.isFirstExcluded() ? startCursor->isFirstMode() : startCursor->isSecondMode())
               && (!endCursor || (!suppressParameters.isLastExcluded() && endCursor->isSecondMode())))
            doesSwap = ((destinationCursor == nullptr) || !destinationCursor->isValid())
               ? parameters.getInsertion().isAfter()
               : (destinationCursor->isFirstMode() && parameters.getInsertion().isBefore());
         else if (endCursor && (suppressParameters.isLastExcluded() ? endCursor->isSecondMode() : endCursor->isFirstMode())
               && (!startCursor || (!suppressParameters.isFirstExcluded() && startCursor->isFirstMode())))
            doesSwap = ((destinationCursor == nullptr) || !destinationCursor->isValid())
               ? parameters.getInsertion().isBefore()
               : (destinationCursor->isSecondMode() && parameters.getInsertion().isAfter());
         if (doesSwap) {
            EnhancedObject* temp = peoFirst;
            peoFirst = peoSecond;
            peoSecond = temp;
            SwapNotification swap;
            if (destinationCursor && !parameters.isRemote())
               swap.setOrigin(*destinationCursor);
            notifyCursorUpdate(swap);
         };
      };
      return;
   };
   bool hasFirst = false;
   RemoveNotification remove(*this);
   if (startCursor && startCursor->isValid()) {
      if (endCursor && endCursor->isValid()) {
         if (endCursor->isFirstMode() && startCursor->isSecondMode())
            return;
         if (endCursor->isFirstMode()) { // startCursor->isFirstMode()
            if (!parameters.getSuppress().isLastExcluded()) {
               destination.add(peoFirst, parameters.getInsertion(), destinationCursor);
               peoFirst = nullptr;
            };
         }
         else if (startCursor->isSecondMode()) { // endCursor->isSecondMode()
            if (!parameters.getSuppress().isFirstExcluded()) {
               destination.add(peoSecond, parameters.getInsertion(), destinationCursor);
               peoSecond = nullptr;
            };
         }
         else { // startCursor->isFirstMode() && endCursor->isSecondMode()
            if (!parameters.getSuppress().isFirstExcluded()) {
               hasFirst = true;
               destination.add(peoFirst, parameters.getInsertion(), destinationCursor);
               peoFirst = nullptr;
            };
            if (!parameters.getSuppress().isLastExcluded()) {
               if (hasFirst) // handle the case parameters.getInsertion().isRemote()
                  destination.insertAtEnd(peoSecond);
               else
                  destination.add(peoSecond, parameters.getInsertion(), destinationCursor);
               peoSecond = nullptr;
            };
         };
      }
      else { // startCursor->isValid() && endCursor == nullptr
         if (!parameters.getSuppress().isFirstExcluded()) {
            hasFirst = true;
            EnhancedObject*& move = startCursor->isFirstMode() ? peoFirst : peoSecond;
            destination.add(move, parameters.getInsertion(), destinationCursor);
            move = nullptr;
         };
         if (startCursor->isFirstMode() && peoSecond) {
            if (hasFirst)
               destination.insertAtEnd(peoSecond);
            else
               destination.add(peoSecond, parameters.getInsertion(), destinationCursor);
            peoSecond = nullptr;
         };
      }
   }
   else if (endCursor && endCursor->isValid()) { // endCursor->isValid() && startCursor == nullptr
      if (endCursor->isSecondMode() && peoFirst) {
         hasFirst = true;
         destination.add(peoFirst, parameters.getInsertion(), destinationCursor);
         peoFirst = nullptr;
      };
      if (!parameters.getSuppress().isLastExcluded()) {
         EnhancedObject*& move = endCursor->isFirstMode() ? peoFirst : peoSecond;
         if (hasFirst)
            destination.insertAtEnd(move);
         else
            destination.add(move, parameters.getInsertion(), destinationCursor);
         move = nullptr;
      };
   }
   else { // startCursor == nullptr && endCursor == nullptr
      if (peoFirst) {
         hasFirst = true;
         destination.add(peoFirst, parameters.getInsertion(), destinationCursor);
         peoFirst = nullptr;
      };
      if (peoSecond) {
         if (hasFirst)
            destination.insertAtEnd(peoSecond);
         else
            destination.add(peoSecond, parameters.getInsertion(), destinationCursor);
         peoSecond = nullptr;
      };
   };

   remove.compareWith(*this);
   if (remove.isValid())
      notifyCursorUpdate(remove);
}

void
DoubleCollection::_remove(const ExtendedSuppressParameters& parameters, VirtualCollectionCursor* cursor) {
   inherited::_remove(parameters, cursor);
   Location location = getLocation(parameters.getRelativePosition(), (Cursor*) cursor);
   EnhancedObject*& move = (location == LFirst) ? peoFirst : peoSecond;
   AssumeCondition(move != nullptr)
   if (parameters.isFree())
      delete move;
   move = nullptr;
   if (cursor && parameters.isExact())
      ((Cursor*) cursor)->setInvalid();
   SuppressionNotification suppression(location);
   if (cursor)
      suppression.setOrigin(*cursor);
   notifyCursorUpdate(suppression);
}

void
DoubleCollection::_removeAll(const ExtendedSuppressParameters& parameters,
      const VirtualCollectionCursor* pvccStart, const VirtualCollectionCursor* pvccEnd) {
   inherited::_removeAll(parameters, pvccStart, pvccEnd);
   RemoveNotification remove(*this);
   Cursor *start = (Cursor*) pvccStart, *end = (Cursor*) pvccEnd;
   if (start && start->isValid()) {
      if (end && end->isValid()) {
         if (end->isFirstMode() && start->isSecondMode())
            return;
         if (end->isFirstMode()) { // start->isFirstMode()
            if (!parameters.isLastExcluded()) {
               if (peoFirst && parameters.isFree())
                  delete peoFirst;
               peoFirst = nullptr;
            };
         }
         else if (start->isSecondMode()) { // end->isSecondMode()
            if (!parameters.isFirstExcluded()) {
               if (peoSecond && parameters.isFree())
                  delete peoSecond;
               peoSecond = nullptr;
            };
         }
         else { // start->isFirstMode() && end->isSecondMode()
            if (!parameters.isFirstExcluded()) {
               if (peoFirst && parameters.isFree())
                  delete peoFirst;
               peoFirst = nullptr;
            };
            if (!parameters.isLastExcluded()) {
               if (peoSecond && parameters.isFree())
                  delete peoSecond;
               peoSecond = nullptr;
            };
         };
      }
      else { // start->isValid() && end == nullptr
         if (!parameters.isFirstExcluded()) {
            EnhancedObject*& move = start->isFirstMode() ? peoFirst : peoSecond;
            if (move && parameters.isFree())
               delete move;
            move = nullptr;
         };
         if (start->isFirstMode() && peoSecond) {
            if (parameters.isFree())
               delete peoSecond;
            peoSecond = nullptr;
         };
      }
   }
   else if (end && end->isValid()) { // end->isValid() && start == nullptr
      if (!parameters.isLastExcluded()) {
         EnhancedObject*& move = end->isFirstMode() ? peoFirst : peoSecond;
         if (move && parameters.isFree())
            delete move;
         move = nullptr;
      };
      if (end->isSecondMode() && peoFirst) {
         if (parameters.isFree())
            delete peoFirst;
         peoFirst = nullptr;
      };
   }
   else { // start == nullptr && end == nullptr
      if (parameters.isFree()) {
         if (peoFirst)
            delete peoFirst;
         peoFirst = nullptr;
         if (peoSecond)
            delete peoSecond;
         peoSecond = nullptr;
      }
      else
         peoFirst = peoSecond = nullptr;
   };

   remove.compareWith(*this);
   if (remove.isValid())
      notifyCursorUpdate(remove);
}

void
DoubleCollection::_replace(EnhancedObject* anObject,
      const ExtendedReplaceParameters& parameters, VirtualCollectionCursor* cursor) {
   inherited::_replace(anObject, parameters, cursor);
   AddGuardian<EnhancedObject> guard(*this, anObject, parameters);
   Location location = getLocation(parameters.getRemovePosition(), (Cursor*) cursor);
   EnhancedObject*& replace = (location == LFirst) ? peoFirst : peoSecond;
   AssumeCondition(replace)
   if (parameters.isFree())
      delete replace;
   replace = nullptr;
   replace = guard.handleDuplication();
   guard.release();
}

EnhancedObject*
DoubleCollection::_getElement(const ExtendedLocateParameters& parameters,
      const VirtualCollectionCursor* cursor) const {
   inherited::_getElement(parameters, cursor);
   Location location = getLocation(parameters.getRelativePosition(), (Cursor*) cursor);
   EnhancedObject* const & element = (location == LFirst) ? peoFirst : peoSecond;
   AssumeCondition(element != nullptr)
   return element;
}

/******************************************************/
/* Implementation of the class DoubleCollectionCursor */
/******************************************************/

bool
DoubleCollectionCursor::_isPositionned(const ExtendedLocateParameters& pos,
      const VirtualCollectionCursor* pvccCursor) const {
   bool result = inherited::_isPositionned(pos, pvccCursor) && (mMode != MInvalid);
   const DoubleCollectionCursor* cursor = (const DoubleCollectionCursor*) pvccCursor;
   if (result) {
      if (pos.isBefore())
         result = (cursor && cursor->isValid())
            ? ((cursor->mMode == MSecond) && (mMode == MFirst))
            : ((mMode == MSecond) || !getSupport().hasFirst());
      else if (pos.isAfter())
         result = (cursor && cursor->isValid())
            ? ((cursor->mMode == MFirst) && (mMode == MSecond))
            : ((mMode == MFirst) || !getSupport().hasSecond());
      else if (pos.isExact())
         result = cursor && (cursor->mMode == mMode);
   };
   return result;
}

bool
DoubleCollectionCursor::_position(const Position& pos) {
   bool result = false;
   if (pos.isInvalid())
      mMode = MInvalid;
   else if (pos.isNear()) {
      if (pos.isForward())
         while (incMode() && !getSupport().hasElement(mMode));
      else { // pPos.isBackward()
         while (decMode() && !getSupport().hasElement(mMode));
      };
      result = (mMode != MInvalid);
   }
   else if (pos.isExact())
      result = (mMode != MInvalid);
   else { // pos.isExtremity() || pos.isAny()
      mMode = MInvalid;
      if (pos.isStart() || pos.isAny())
         while (incMode() && !getSupport().hasElement(mMode));
      else // pPos.isEnd()
         while (decMode() && !getSupport().hasElement(mMode));
      result = (mMode != MInvalid);
   };
   return result;
}

} // end of namespace COL

