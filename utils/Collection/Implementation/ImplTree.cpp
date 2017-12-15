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
// Unit      : Low level implementation of tree collections
// File      : ImplTree.cpp
// Description :
//   Implementation of the classes GenericTreeElement, ImplTreeCursor that define
//   the elements and the cursors for the tree based collection implementations.
//

#include "Collection/Implementation/ImplTree.h"
// #include "Collection/Collection.hpp"

namespace COL {}

#include "Pointer/ImplList.template"
#include "Collection/Implementation/ImplCommonTree.template"

namespace COL {

/****************************************************************************/
/* Implementation of the class GenericImplTreeCursor::CollectionCursorLevel */
/****************************************************************************/

ComparisonResult
GenericImplTreeCursor::CollectionCursorLevel::_compare(const EnhancedObject& asource) const {
   ComparisonResult result = ImplListElement::_compare(asource);
   const auto& source = (const CollectionCursorLevel&) asource;
   if (result == CREqual)
      result = (&apvcCursor->getSupport() == &source.apvcCursor->getSupport())
         ? apvcCursor->compare(*source.apvcCursor)
         : CRNonComparable;
   return result;
}

/*****************************************************************************/
/* Implementation of the class GenericImplTreeCursor::CollectionCursorsStack */
/*****************************************************************************/

GenericImplTreeCursor::CollectionCursorsStack::ExtendedComparisonResult
GenericImplTreeCursor::CollectionCursorsStack::fullCompare(const CollectionCursorsStack& source) const {
   ComparisonResult result = inherited::_compare(castToCopyHandler(source));
   Cursor thisCursor(*this);
   Cursor sourceCursor(source);

   for (thisCursor.setToFirst(), sourceCursor.setToFirst();
        thisCursor.isValid() && sourceCursor.isValid() && (result == CREqual);
        thisCursor.setToNext(), sourceCursor.setToNext())
      result = thisCursor.elementAt().compare(sourceCursor.elementAt());
   if (result != CREqual)
      return ExtendedComparisonResult().setResult(result);
   if (thisCursor.isValid())
      return ExtendedComparisonResult().setBigger();
   if (sourceCursor.isValid())
      return ExtendedComparisonResult().setSmaller();
   return ExtendedComparisonResult().setEqual();
}

ComparisonResult
GenericImplTreeCursor::CollectionCursorsStack::levelCompare(const CollectionCursorsStack& source) const {
   if (isEmpty())
      return source.isEmpty() ? CREqual : CRLess;
   if (source.isEmpty())
      return CRGreater;
   if (count() < source.count()) {
      CollectionCursorsStack::Cursor sourceStackCursor(source);
      sourceStackCursor -= (source.count() - count());
      return getLast().getCursor().isEqual(sourceStackCursor.elementAt().getCursor())
         ? CRLess : CRNonComparable;
   };
   if (count() > source.count()) {
      CollectionCursorsStack::Cursor stackCursor(*this);
      stackCursor -= (count()- source.count());
      return stackCursor.elementAt().getCursor().isEqual(source.getLast().getCursor())
         ? CRGreater : CRNonComparable;
   };
   return getLast().getCursor().isEqual(source.getLast().getCursor())
         ? CREqual : CRNonComparable;
}

bool
GenericImplTreeCursor::CollectionCursorsStack::hasPrefix(const CollectionCursorsStack& source,
      CollectionCursorsStack::Cursor& stackCursor) {
   if (source.isEmpty())
      return true;
   if (isEmpty())
      return false;
   stackCursor.setToLast();

   stackCursor -= (count() - source.count());
   if (stackCursor.elementAt().getCursor().isEqual(source.getLast().getCursor()))
      return true;
   stackCursor.invalidate();
   return false;
}

void
GenericImplTreeCursor::CollectionCursorsStack::intersect(const CollectionCursorsStack& source,
      CollectionCursorsStack& thisDiff, CollectionCursorsStack& sourceDiff,
      CollectionCursorsStack& common) {
   Cursor thisCursor(*this), sourceCursor(source);
   for (thisCursor.setToFirst(), sourceCursor.setToFirst();
        thisCursor.isValid() && sourceCursor.isValid()
            && thisCursor.elementAt().getCursor().isEqual(sourceCursor.elementAt().getCursor());
        thisCursor.setToNext(), sourceCursor.setToNext())
      common.push(thisCursor.elementAt().getCursor());

   while (thisCursor.setToNext())
      thisDiff.push(thisCursor.elementAt().getCursor());
   while (sourceCursor.setToNext())
      sourceDiff.push(sourceCursor.elementAt().getCursor());
}

bool
GenericImplTreeCursor::CollectionCursorsStack::isValid() const {
   bool result = true;
   if (!isEmpty()) {
      const CollectionCursorLevel* previousLevelCursor = nullptr;
      result = foreachDo([&previousLevelCursor](const CollectionCursorLevel& levelCursor)
         {  if (!levelCursor.isValid())
               return false;
            if (previousLevelCursor) {
               if (!previousLevelCursor->getCursor().isValid())
                  return false;
               const GenericTreeElement& fatherElement
                  = GenericTreeElement::HandlerSonsAndSubTreeCast::castFrom(
                        previousLevelCursor->getCursor().elementAt());
               if (!fatherElement.hasSons()
                     || (&levelCursor.getCursor().getSupport() != &fatherElement.getSons()))
                  return false;
            };
            previousLevelCursor = &levelCursor;
            return true;
         });
   };
   return result;
}

bool
GenericImplTreeCursor::CollectionCursorsStack::normalize() {
   bool result = false;
   if (!isEmpty() && !lastCursor().hasSupport()) {
      do {
         pop();
      } while (!isEmpty() && !lastCursor().hasSupport());
      AssumeCondition(isEmpty() || lastCursor().isValid())
      result = true;
   };
   return result;
}

/*****************************************************************/
/* Implementation of the class GenericImplTreeCursor::CursorPath */
/*****************************************************************/

GenericImplTreeCursor::CursorPath::CursorPath(const GenericImplTree& tree,
      const GenericImplTreeCursor* fstCursor, const GenericImplTreeCursor* sndCursor)
      :  uAscentLevel(0), uDescentLevel(0), gitTree(tree) {
   AssumeCondition(fstCursor || sndCursor)
   if (!fstCursor) { // sndCursor != nullptr
      uDescentLevel = sndCursor->getLevel();
      ppccscDescentCursor = sndCursor->ccsStack.newCursor();
      ppccscDescentCursor->setToFirst();
   }
   else if (!sndCursor) { // fstCursor != nullptr
      uAscentLevel = fstCursor->getLevel();
      ppccscAscentCursor = fstCursor->ccsStack.newCursor();
      ppccscAscentCursor->setToFirst();
   }
   else { // fstCursor && sndCursor
      int diff = fstCursor->getLevel() - sndCursor->getLevel();
      if (diff >= 0)
         uAscentLevel = diff;
      else
         uDescentLevel = -diff;
      ppccscAscentCursor->setToLast();
      ppccscDescentCursor->setToLast();
      for (; diff > 0; diff--)
         ppccscAscentCursor->setToPrevious();
      for (; diff < 0; diff++)
         ppccscDescentCursor->setToPrevious();
      while (ppccscAscentCursor->isValid()
         && (!ppccscAscentCursor->elementAt().getCursor().isEqual(ppccscDescentCursor->elementAt().getCursor()))) {
         ppccscAscentCursor->setToPrevious();
         ppccscDescentCursor->setToPrevious();
         uAscentLevel++;
         uDescentLevel++;
      };
   };
}

/*****************************************************/
/* Implementation de la classe GenericImplTreeCursor */
/*****************************************************/

bool
GenericImplTreeCursor::invariant() const {
   if (!ccsStack.isValid())
      return false;
   if (isOnTree())
      return ccsStack.isEmpty() || ccsStack.lastCursor().isValid();
   if (isUpRoot())
      return ccsStack.isEmpty();
   if (isInvalidCursor())
      return !ccsStack.isEmpty() && !ccsStack.lastCursor().isValid();
   // isDownFinal()
   return ccsStack.isEmpty() || ccsStack.lastElement().isFinal();
   // return false;
}

ComparisonResult
GenericImplTreeCursor::levelCompare(const GenericImplTreeCursor& source) const {
   ComparisonResult result = ccsStack.levelCompare(source.ccsStack);
   if (result != CREqual)
      return result;
   if (isUpRoot())
      return source.isUpRoot() ? CREqual : CRLess;
   if (source.isUpRoot())
      return CRGreater;
   if (isDownFinal())
      return source.isDownFinal() ? CREqual : CRGreater;
   if (source.isDownFinal())
      return CRLess;
   return CREqual;
}

bool
GenericImplTreeCursor::isDescendant(const GenericImplTreeCursor& cursor, const FlatExtendedLocateParameters& pos) const {
   VirtualTreeCursor::ExtendedComparisonResult result = ccsStack.fullCompare(cursor.ccsStack);
   if (result.getResult() != VirtualTreeCursor::ExtendedComparisonResult::ERBigger)
      return false;
   if ((pos.isAfter()) || (pos.isBefore())) {
      CollectionCursorsStack::Cursor stackCusor(ccsStack);
      stackCusor.setToLast();
      for (int count = ccsStack.count()-cursor.ccsStack.count(); count > 0;
           --count, stackCusor.setToPrevious())
         if (!VirtualCollectionCursor::CursorAccess::isPositionned(
                  stackCusor.elementAt().getCursor(), pos))
            return false;
   };
   return true;
}

namespace DImplBasicTree {

template int queryCNumberOfElements(TemplateNode<GenericTreeElement>, const COL::VirtualCollection&);

} // end of namespace DImplBasicTree

template class TImplTree<GenericTreeElement, GenericImplTreeCursor>;

} // end of namespace COL

