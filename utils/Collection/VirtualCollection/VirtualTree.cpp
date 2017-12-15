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
// Fichier     : VirtualTree.cpp
// Description :
//   Implementation of the class VirtualTree.
//

#include "Collection/VirtualCollection/VirtualTree.h"
// #include "Collection/Collection.hpp"

namespace COL {}

#include "Collection/VirtualCollection/VirtualTree.template"

namespace COL {

/************************************************************/
/* Implementation of the class VirtualTree::RouteCopyAction */
/************************************************************/

class VirtualTree::RouteCopyAction::BasicElement : public ImplListElement {
  private:
   const EnhancedObject* peoSourceElement;

  public:
   BasicElement(const EnhancedObject& sourceElement) : peoSourceElement(&sourceElement) {}
   BasicElement(const BasicElement& source) = default;
   DefineCopy(BasicElement)
   DDefineAssign(BasicElement)

   const EnhancedObject& getSourceElement() const { return *peoSourceElement; }
};

class VirtualTree::RouteCopyAction::DescentHierarchy : public ImplList {
  public:
   DescentHierarchy() {}
   DescentHierarchy(const DescentHierarchy& source) : ImplList(source, true) {}
   virtual ~DescentHierarchy() { ImplList::freeAll(); }
   DefineCopy(DescentHierarchy)
   DDefineAssign(DescentHierarchy)
   DescentHierarchy& operator=(const DescentHierarchy& source)
      {  if (this != &source) {
            removeAll();
            addAll(source);
         };
         return *this;
      }

   void removeAll() { ImplList::freeAll(); }
   void addAll(const DescentHierarchy& source) { ImplList::addCopyAll(source); }
   void add(const EnhancedObject& sourceElement) { ImplList::addAsLast(new BasicElement(sourceElement)); }

   BasicElement* getFirst() const { return (BasicElement*) ImplList::getFirst(); }
   BasicElement* getNext(BasicElement* element) const
      {  return (BasicElement*) ImplList::getNext(element); }
   BasicElement* getPrevious(BasicElement* element) const
      {  return (BasicElement*) ImplList::getPrevious(element); }
   BasicElement* getLast() const { return (BasicElement*) ImplList::getLast(); }
};

void
VirtualTree::RouteCopyAction::descentTo(const VirtualTreeCursor& cursor, int steps) {
   DescentHierarchy diffStartStack;
   if (steps > 0) {
      PPCursor descentCursor(cursor);
      if (!descentCursor->isValid()) {
         steps--;
         descentCursor->setToFather();
      };
      while (steps-- > 0) {
         diffStartStack.add(descentCursor->elementAt());
         descentCursor->setToFather();
      };
   };

   BasicElement* descentCursor = diffStartStack.getFirst();
   if (descentCursor) {
      do {
         descent(descentCursor->getSourceElement());
         descentCursor = diffStartStack.getNext(descentCursor);
      } while (descentCursor != diffStartStack.getFirst());
   };
   if (!cursor.isValid())
      descent();
}

/*******************************************/
/* Implementation of the class VirtualTree */
/*******************************************/

class VirtualTree::CountAction : public VirtualAction {
  private:
   int uResult;

  public:
   CountAction() : uResult(0) {}
   bool init(const VirtualTree& tree, const Cursor& cursor)
      {  uResult++; return true; }
   bool ascent(const Cursor& cursor, EnhancedObject& son) {  return true; }
   bool descent(const Cursor& cursor, EnhancedObject& father)
      {  uResult++; return true; }
   bool next(const Cursor& cursor, EnhancedObject& brother)
      {  uResult++; return true; }
   bool end(const VirtualTree& tree, const Cursor& cursorRoot)
      {  return (uResult > 0); }
   const int& getResult() const { return uResult; }
};

int
VirtualTree::pqueryCount(const ExtendedTreeLocateParameters& parameters,
      const VirtualTreeCursor* start, const VirtualTreeCursor* end) const {
   if (parameters.isSingleLevel()) {
      if (parameters.isBrother() && (!start || !end)) {
         if (!start && !end)
            return isEmpty() ? 0 : 1;
         if ((start && !end->isValid()) || (end && !start->isValid())) {
            PPCursor cursor = newCursor();
            *cursor = (start) ? *start : *end;
            return cursor->setToFather() ? _querySonsCount(*cursor) : (isEmpty() ? 0 : 1);
         };
      }
      else if (parameters.isSons() && (!start || !end)) {
         if (start)
            return start->isValid() ? _querySonsCount(*start) : 0;
         if (end)
            return end->isValid() ? _querySonsCount(*end) : 0;
         return isEmpty() ? 0 : 1;
      };
   };

   CountAction counter;
   return foreachDo(counter, parameters, start, end) ? counter.getResult() : 0;
}

void
VirtualTree::pfullAssign(const VirtualTree& source, const ExtendedTreeReplaceParameters& parameters) {
   VirtualCollection::_fullAssign(source, parameters);
   if (this != &source) {
      _removeAll(parameters.getSuppress());
      if (parameters.isDuplicate()) {
         if (!parameters.isFreeOnError()) {
            RouteCopyAction copyAction(*this, parameters.isInsertionSons());
            source.foreachDo(copyAction);
         }
         else try {
            RouteCopyAction copyAction(*this, parameters.isInsertionSons());
            source.foreachDo(copyAction);
         }
         catch (...) {
            ExtendedTreeSuppressParameters params(parameters.getSuppress());
            _removeAll(params.setFree());
            throw;
         };
      };
   };
}


void
VirtualTree::paddAll(const VirtualTree& source, const ExtendedTreeInsertionParameters& parameters,
      VirtualTreeCursor* cursor, const VirtualTreeCursor* startSource,
      const VirtualTreeCursor* endSource) {
   VirtualCollection::_addAll(source, parameters, cursor, startSource, endSource);
   if (!parameters.isDuplicate())
      return;
   PPCursor garbageCursor;
   RouteLevelCopyAction copyAction(*this, parameters);

   // Define the insertion base
   if (parameters.isDescendant()) {
      if (!cursor) {
         garbageCursor = newCursor();
         cursor = garbageCursor.key();
      };
      if (parameters.isInnerSons()) {
         AssumeCondition(parameters.isInsertion())
         if (parameters.getRelativePosition() == RPBefore)
            while (cursor->setToFirstSon());
         else
            while (cursor->setToLastSon());
         if (cursor->getMode() == VirtualTreeCursor::VMDownCursor)
            cursor->setToFather();
      }
      else
         cursor->setToInvalidSon();
   };
   if (cursor)
      copyAction.setToCursor(*cursor);

   // Define the source interval
   if (startSource) {
      // Define the common part to the two source cursors
      int baseDiff = startSource->queryBaseDiff(endSource);
      copyAction.descentTo(*startSource, baseDiff);
   };

   // copy the end of the tree
   source.foreachDo(copyAction, parameters, startSource, endSource);
}

/*************************************************/
/* Implementation of the class VirtualTreeCursor */
/*************************************************/

bool
VirtualTreeCursor::setToInnerFirstSon() {
   if (!isValid() && !setToRoot())
      return false;
   while (setToFirstSon());
   return setToFather();
}

bool
VirtualTreeCursor::setToInnerLastSon() {
   if (!isValid() && !setToRoot())
      return false;
   while (setToLastSon());
   return setToFather();
}

} // end of namespace COL

