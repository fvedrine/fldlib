/**************************************************************************/
/*                                                                        */
/*  This file is part of FLDLib                                           */
/*                                                                        */
/*  Copyright (C) 2014-2017                                               */
/*    CEA (Commissariat à l'énergie atomique et aux énergies              */
/*         alternatives)                                                  */
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
// Librairie   : Collection
// Module      : Abstract collections
// Fichier     : VirtualCollection.cpp
// Description :
//   Implementation of the class VirtualCollection.
//

#include "Collection/VirtualCollection/VirtualCollection.h"
// #include "Collection/Collection.hpp"

namespace COL {}

#include "Pointer/Pointer.template"
//#include "Collection/Collection.template"

namespace COL {

/*************************************************/
/* Implementation of the class VirtualCollection */
/*************************************************/

bool
VirtualCollection::isExcluded(RelativePosition insertPos, RelativePosition intervalMode,
      const VirtualCollectionCursor* startCursor, const VirtualCollectionCursor* endCursor,
      const VirtualCollectionCursor* cursor) const {
   AssumeCondition ((insertPos != RPUndefined) && (insertPos != RPExact))

   if (insertPos == RPUndefined)
      return false;
   bool isStrictlyBefore = (insertPos == RPAfter)
      ? (startCursor && cursor && (*startCursor > *cursor)
            && ((intervalMode == RPAfter) || (intervalMode == RPUndefined)
                || !startCursor->_isPositionned(RPAfter, cursor)))
         : (((intervalMode == RPAfter) ||(intervalMode == RPUndefined))
      ? (!startCursor && !cursor)
            || (startCursor && cursor && (*startCursor >= *cursor))
      : startCursor && (!cursor || (*startCursor > *cursor)));
   if (isStrictlyBefore)
      return true;

   bool isStrictlyAfter = (insertPos == RPBefore)
      ? (endCursor && cursor && (*endCursor < *cursor)
            && ((intervalMode == RPBefore) || (intervalMode == RPUndefined)
                || !endCursor->_isPositionned(RPBefore, cursor)))
         : (((intervalMode == RPBefore) || (intervalMode == RPUndefined))
      ? (!endCursor && !cursor)
            || (endCursor && cursor && (*endCursor <= *cursor))
      : endCursor && (!cursor || (*endCursor < *cursor)));
   return isStrictlyAfter;
}

void
VirtualCollection::pfullAssign(const VirtualCollection& source, const ExtendedReplaceParameters& parameters) {
   VirtualCollection::_fullAssign(source, parameters);
   if ((this != &source)) {
      _removeAll((const ExtendedSuppressParameters&) parameters);
      _addAll(source, (const ExtendedInsertionParameters&) parameters);
   };
}

int
VirtualCollection::pqueryCount(const ExtendedLocateParameters& parameters,
      const VirtualCollectionCursor* startCursor, const VirtualCollectionCursor* endCursor) const {
   VirtualCollection::_queryCount(parameters, startCursor, endCursor);
   VirtualCollection::PPCursor cursor = newCursor();
   bool doesContinue = true;
   if (startCursor) {
      *cursor = *startCursor;
      doesContinue = (parameters.isAfter() || parameters.isUndefined())
         ? cursor->setToNext() : (cursor->isValid() || cursor->setToFirst());
   };
   int result = 0;
   while (doesContinue && ((endCursor == nullptr) || cursor->isEqual(*endCursor))) {
      result++;
      doesContinue = cursor->setToNext();
   };
   if ((endCursor != nullptr) && endCursor->isValid())
      return doesContinue ? result+1 : 0;
   return (parameters.isBefore() || parameters.isUndefined()) ? result+1 : result;
}

#include "Collection/VirtualCollection/VirtualCollection.incc"

void
VirtualCollection::pmoveAllTo(VirtualCollection& destination, const ExtendedReplaceParameters& parameters,
      const VirtualCollectionCursor* startCursor, const VirtualCollectionCursor* endCursor,
      VirtualCollectionCursor* destinationCursor) {
   VirtualCollection::_moveAllTo(destination, parameters, startCursor, endCursor,
         destinationCursor);
   if (!normalize(startCursor, endCursor))
      return;

   if (&destination == this) { // Detection of intern moves likely to loop forever: nothing to do in that case
      if (parameters.getInsertionPosition() == RPUndefined)
         return;
      if (!isExcluded(parameters.getInsertionPosition(), parameters.getRemovePosition(),
               startCursor, endCursor, destinationCursor))
         return;
   };

   const ExtendedSuppressParameters& suppressParameters = (const ExtendedSuppressParameters&) parameters;
   PNT::AutoPointer<ExtendedReplaceParameters> paramsCopy(parameters);

   bool isForward = parameters.isRemote() ? (parameters.getInsertionPosition() != RPAfter)
      : (parameters.getInsertionPosition() != RPBefore);
   if (isForward
       ? (!startCursor || (!suppressParameters.isFirstExcluded() && startCursor->isFirst()))
       : (!endCursor || (!suppressParameters.isLastExcluded() && endCursor->isLast()))) {
      // The suppression always occurs at the beginning or at the end of the collection.
      paramsCopy->setRemovePosition(isForward ? RPAfter : RPBefore);

      // The normalization ensures at least a move
      _moveTo(destination, *paramsCopy, nullptr, destinationCursor);
      while (isForward
            ? ((!endCursor && !isEmpty()) || (endCursor && !endCursor->isFirst()))
            : ((!startCursor && !isEmpty()) || (startCursor && !startCursor->isLast())))
         _moveTo(destination, *paramsCopy, nullptr, destinationCursor);
      if (isForward
            ? (endCursor && !suppressParameters.isLastExcluded())
            : (startCursor && !suppressParameters.isFirstExcluded()))
         _moveTo(destination, *paramsCopy, nullptr, destinationCursor);
      return;
   };

   // The suppression occurs after *sourceCursor
   PPCursor sourceCursor(isForward ? *startCursor : *endCursor);
   RelativePosition pos = parameters.getRemovePosition();
   if ((pos != RPUndefined) && (pos != (isForward ? RPAfter : RPBefore))) {
      sourceCursor->position(Cursor::Position().setDirection(isForward ? RPBefore : RPAfter));
      paramsCopy->setRemovePosition(isForward ? RPAfter : RPBefore);
   };

   if (sourceCursor->isValid() && (isForward
         ? ((endCursor==nullptr) || (!sourceCursor->isEqual(*endCursor)))
         : ((startCursor==nullptr) || (!sourceCursor->isEqual(*startCursor))))) {
      for (; sourceCursor->isValid()
            && ((endCursor==nullptr) || (isForward
                  ? !sourceCursor->_isPositionned(RPBefore, endCursor)
                  : !sourceCursor->_isPositionned(RPAfter, endCursor)));
         sourceCursor->position(Cursor::Position().setDirection(isForward ? RPAfter : RPBefore)))
      _moveTo(destination, *paramsCopy, sourceCursor.key(), destinationCursor);
   };
   if (isForward 
       ? ((endCursor != nullptr) && sourceCursor->isValid() && !suppressParameters.isLastExcluded())
       : ((startCursor != nullptr) && sourceCursor->isValid() && !suppressParameters.isFirstExcluded()))
      _moveTo(destination, *paramsCopy, sourceCursor.key(), destinationCursor);
}

void
VirtualCollection::premoveAll(const ExtendedSuppressParameters& parameters,
      const VirtualCollectionCursor* start, const VirtualCollectionCursor* end) {
   VirtualCollection::_removeAll(parameters, start, end);
   if (!normalize(start, end))
      return;
   PNT::AutoPointer<ExtendedSuppressParameters> parametersCopy(parameters);
   if (!start || (!parameters.isFirstExcluded() && start->isFirst())) {
      // The suppression always occurs at the beginning of the list.
      parametersCopy->setRelativePosition(RPAfter);
      while ((!end && !isEmpty()) || (end && !end->isFirst()))
         _remove(*parametersCopy);
      if (end && !parameters.isLastExcluded())
         _remove(*parametersCopy);
      return;
   };

   PPCursor previousRemove(*start);
   if (!parameters.isFirstExcluded())
      previousRemove->setToPrevious();
   parametersCopy->setRelativePosition(RPAfter);

   // The suppression always occurs after *previousRemove
   while ((!end && !isEmpty()) || (end && !end->_isPositionned(RPAfter, previousRemove.key())))
      _remove(*parametersCopy, previousRemove.key());

   if (!parameters.isLastExcluded())
      _remove(*parametersCopy, previousRemove.key());
}

#include "Collection/ELMCollection.incc"

/*******************************************************/
/* Implementation of the class VirtualCollectionCursor */
/*******************************************************/

VirtualCollectionCursor&
VirtualCollectionCursor::operator+=(int add) {
   if (add > 0) {
      do {
         setToNext();
      } while (--add);
   }
   else if (add < 0) {
      do {
         setToPrevious();
      } while (++add);
   };
   return *this;
}

VirtualCollectionCursor&
VirtualCollectionCursor::operator-=(int add) {
   if (add > 0) {
      do {
         setToPrevious();
      } while (--add);
   }
   else if (add < 0) {
      do {
         setToNext();
      } while (++add);
   };
   return *this;
}

} // end of namespace COL

// template int COL::ImplArray::merge<TemplateElementKeyCastParameters<EnhancedObject, COL::VirtualSortedCollection::SimpleKey, SimpleCast> >(TemplateElementKeyCastParameters<EnhancedObject, COL::VirtualSortedCollection::SimpleKey, SimpleCast> const&, COL::ImplArray const&, int, int, bool, VirtualCast const*);

