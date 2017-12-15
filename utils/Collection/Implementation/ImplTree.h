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
// File      : ImplTree.h
// Description :
//   Definition of the classes GenericTreeElement, ImplTreeCursor that define
//   the elements and the cursors for the tree based collection implementations.
//

#ifndef COL_ImplTreeH
#define COL_ImplTreeH

#include "Collection/ConcreteCollection/List.h"
#include "Collection/Implementation/ImplCommonTree.h"

namespace COL {

/******************************************************************************/
/* Definition of the class GenericTreeElement = nodes and leaves of the trees */
/******************************************************************************/

class GenericTreeElement : public EnhancedObject, public ImplTreeElement {
  private:
   typedef EnhancedObject inherited;

  public:
   GenericTreeElement() {}
   GenericTreeElement(VirtualCollection* sonsCollection)
      :  ImplTreeElement(sonsCollection) {}
   GenericTreeElement(const GenericTreeElement& source)
      :  EnhancedObject(source), ImplTreeElement(source) {}
   DefineCopy(GenericTreeElement)
   DDefineAssign(GenericTreeElement)

   typedef HandlerCast<GenericTreeElement> HandlerSonsAndSubTreeCast;
   typedef VirtualCollection FlatCollection;
};

/*************************************************/
/* Definition of the class GenericImplTreeCursor */
/*************************************************/

class GenericImplTree;
class GenericImplTreeCursor : public ExtendedParameters, public DVirtualTree::Definitions {
  public:
   class StackPrefix;
   class CursorPath;

  private:
   // Definition of stacks of cursors
   class CollectionCursorsStack;
   class CollectionCursorLevel;
   #include "Collection/Implementation/TreeCursorField.inline"

   CollectionCursorsStack ccsStack;

   friend class StackPrefix;
   friend class CursorPath;
   void pushSon(const VirtualCollectionCursor& sonCursor);

  public:
   typedef VirtualCollectionCursor::CursorAccess::ExtendedLocateParameters FlatExtendedLocateParameters;
   typedef VirtualCollectionCursor::CursorAccess::Position FlatPosition;
   typedef GenericImplTree Tree;

   GenericImplTreeCursor() { mergeOwnField(VMUpRoot); }
   GenericImplTreeCursor(const GenericImplTree& tree);
   GenericImplTreeCursor(const GenericImplTreeCursor& source)
      :  ExtendedParameters(source), ccsStack(source.ccsStack) {}

   DefineExtendedParameters(2, ExtendedParameters)
   GenericImplTreeCursor& operator=(const GenericImplTreeCursor& source) = default;
   ComparisonResult compare(const GenericImplTreeCursor& source) const
      {  return (isOnTree() && source.isOnTree())
            ? ccsStack.compare(source.ccsStack) : CRNonComparable;
      }
   VirtualTreeCursor::ExtendedComparisonResult fullCompare(const GenericImplTreeCursor& source) const
      {  VirtualTreeCursor::ExtendedComparisonResult result = ccsStack.fullCompare(source.ccsStack);
         return (isUpRoot() || source.isUpRoot())
               ? ((isUpRoot() && source.isUpRoot())
            ? result : VirtualTreeCursor::ExtendedComparisonResult())
               : ((isDownFinal() || source.isDownFinal()) ? ((isDownFinal() && source.isDownFinal())
            ? result : VirtualTreeCursor::ExtendedComparisonResult()) : result);
      }
   ComparisonResult levelCompare(const GenericImplTreeCursor& source) const;
   bool isEqual(const GenericImplTreeCursor& source) const
      {  return (getMode() == source.getMode()) && ccsStack.isEqual(source.ccsStack); }
   bool isValid() const { return isOnTree(); }
   bool invariant() const;

   // query methods
   ValidityMode getMode() const { return (ValidityMode) queryOwnField(); }
   void setMode(ValidityMode mode) { setOwnField(mode); }
   bool isCollectionCursor() const { return ((getMode() == VMTree) && !ccsStack.isEmpty()); }
   bool isUpRoot() const { return (getMode() == VMUpRoot); }
   bool isRoot() const { return (getMode() == VMTree) && ccsStack.isEmpty(); }
   bool isOnTree() const { return getMode() == VMTree; }
   bool isDownCursor() const { return queryOwnField() & VMDownCursor; }
   bool isDownFinal() const { return queryOwnField() == VMDownCursor; }
   bool isDownFinalRoot() const { return (queryOwnField() == VMDownCursor) && ccsStack.isEmpty(); }
   bool isInvalidCursor() const { return getMode() == VMInvalidCursor; }
   bool hasFlatSupport() const { return (getMode() & VMTree) && !isRoot(); }
   bool hasStack() const { return !ccsStack.isEmpty(); }

   VirtualCollectionCursor& lastCursor() const { return ccsStack.lastCursor(); }
   GenericTreeElement& lastElement() const
      { return (GenericTreeElement&) GenericTreeElement::HandlerSonsAndSubTreeCast::castFrom(lastCursor().elementSAt()); }
   VirtualCollection& lastSupport() const
      { return VirtualCollectionCursor::CursorAccess::getSupport(ccsStack.lastCursor()); }

   int getLevel() const
      { return (isUpRoot() ? -1 : ((isDownFinal()) ? (ccsStack.count()+1) : ccsStack.count())); }
   bool isBrother(const GenericImplTreeCursor& cursor) const;
   bool isFirstBrother() const
      {  return (getMode() == VMTree) && (ccsStack.isEmpty()
            || VirtualCollectionCursor::CursorAccess::isPositionned(lastCursor(),
               VirtualCollection::ExtendedLocateParameters().setAfter(), nullptr));
      }
   bool isLastBrother() const
      {  return (getMode() == VMTree) && (ccsStack.isEmpty()
            || VirtualCollectionCursor::CursorAccess::isPositionned(lastCursor(),
               VirtualCollection::ExtendedLocateParameters().setBefore(), nullptr));
      }
   bool isBrother(const GenericImplTreeCursor& cursor, const FlatExtendedLocateParameters& params) const;
   bool isSon(const GenericImplTreeCursor& cursor, const FlatExtendedLocateParameters& params) const;
   bool isDescendant(const GenericImplTreeCursor& cursor, const FlatExtendedLocateParameters& params) const;

   void setElementLevel(GenericTreeElement& element) const {}
   void setElementSubLevel(GenericTreeElement& element) const {}
   void resetElementLevel(GenericTreeElement& element) const {}
   void resetElementSubLevel(GenericTreeElement& element) const {}

   // set methods
   bool setBrother(const FlatPosition& pos, const GenericImplTree& tree);
   void pushSon(const GenericImplTree& tree);
   bool setSon(const FlatPosition& pos, const GenericImplTree& tree);
   bool setFather();
   VirtualCollection::PPCursor setFatherWithLastCursorExtraction();
   bool setRoot(const GenericImplTree& tree);

   void normalize()
      {  if (ccsStack.normalize())
            setMode(VMDownCursor);
      }
   void normalizeAfterEndSuppression(bool hasRoot);
   void invalidate() { ccsStack.clear(); setMode(VMUpRoot); }
   void cut(const GenericImplTreeCursor& cursor);
   void swap(GenericImplTreeCursor& source)
      {  ccsStack.swap(source.ccsStack);
         ExtendedParameters temp(*this);
         ExtendedParameters::operator=(source);
         source.ExtendedParameters::operator=(temp);
      }

   /* Definition of the class StackPrefix */
   class StackPrefix {
     private:
      CollectionCursorsStack::Cursor ccscLast;
      GenericImplTreeCursor* pgitcTreeCursor;

     public:
      StackPrefix(GenericImplTreeCursor& treeCursor)
         : ccscLast(treeCursor.ccsStack), pgitcTreeCursor(&treeCursor) {}
      StackPrefix(const StackPrefix& source) = default;
      StackPrefix& operator=(const StackPrefix& source) = default;
      bool hasPrefix(const GenericImplTreeCursor& tree)
         {  return pgitcTreeCursor->ccsStack.hasPrefix(tree.ccsStack, ccscLast); }
      bool isFull() const { return ccscLast.isLast(); }
      void advance() { ccscLast.setToNext(); AssumeCondition(ccscLast.isValid()) }
      void replacePrefix(const GenericImplTreeCursor& newPrefix)
         {  pgitcTreeCursor->ccsStack.replacePrefix(ccscLast, newPrefix.ccsStack); }
      void cut()
         {  pgitcTreeCursor->ccsStack.cut(ccscLast);
            pgitcTreeCursor->setMode(pgitcTreeCursor->ccsStack.isEmpty() ? VMUpRoot : VMInvalidCursor);
         }
   };
   
   class CursorPath {
     private:
      CollectionCursorsStack::PPCursor ppccscAscentCursor;
      CollectionCursorsStack::PPCursor ppccscDescentCursor;
      int uAscentLevel;
      int uDescentLevel;
      const GenericImplTree& gitTree;
      
     public:
      CursorPath(const GenericImplTree& source, const GenericImplTreeCursor* fstCursor,
            const GenericImplTreeCursor* sndCursor);
      CursorPath(const CursorPath& source)
         :  ppccscAscentCursor(source.ppccscAscentCursor, PNT::Pointer::Duplicate()),
            ppccscDescentCursor(source.ppccscDescentCursor, PNT::Pointer::Duplicate()),
            uAscentLevel(source.uAscentLevel), uDescentLevel(source.uDescentLevel),
            gitTree(source.gitTree) {}
      CursorPath& operator=(const CursorPath& source)
         {  AssumeCondition(&gitTree == &source.gitTree)
            ppccscAscentCursor.fullAssign(source.ppccscAscentCursor);
            ppccscDescentCursor.fullAssign(source.ppccscDescentCursor);
            uAscentLevel = source.uAscentLevel;
            uDescentLevel = source.uDescentLevel;
            return *this;
         }
      bool hasAscent() const
         {  return (uAscentLevel > 1); }
      bool ascent(GenericImplTreeCursor& cursor)
         {  AssumeCondition(uAscentLevel >= 1)
            bool result = (uAscentLevel > 1);
            if (result) {
               uAscentLevel--;
               ppccscAscentCursor->setToPrevious();
               cursor.setFather();
               AssumeCondition(ppccscAscentCursor->elementAt().getCursor().isEqual(cursor.lastCursor()))
            };
            return result;
         }
      bool up(GenericImplTreeCursor& cursor)
         {  AssumeCondition(uAscentLevel == 1)
            uAscentLevel--;
            cursor.setFather();
            ppccscAscentCursor->invalidate();
            return cursor.isValid();
         }
      bool down(GenericImplTreeCursor& cursor)
         {  AssumeCondition(uAscentLevel == 0)
            uAscentLevel++;
            cursor.pushSon(ppccscDescentCursor->elementAt().getCursor());
            ppccscDescentCursor->setToNext();
            return ppccscDescentCursor.isValid();
         }
      bool next(GenericImplTreeCursor& cursor)
         {  AssumeCondition(uAscentLevel >= 1)
            return (uAscentLevel > 1)
               ?  cursor.setBrother(FlatPosition().setForward(), gitTree)
               :  (ppccscDescentCursor.isValid()
                     && !ppccscDescentCursor->elementAt().getCursor().isEqual(cursor.lastCursor())
                     && cursor.setBrother(FlatPosition().setForward(), gitTree));
         }
      bool hasNext(GenericImplTreeCursor& cursor) const
         {  return (uAscentLevel > 0) && !cursor.isLastBrother() && ((uAscentLevel > 1)
               || (ppccscDescentCursor.isValid()
                   && !ppccscDescentCursor->elementAt().getCursor().isEqual(cursor.lastCursor())));
         }
      bool descent(GenericImplTreeCursor& cursor)
         {  AssumeCondition((uAscentLevel == 1) && (uDescentLevel >= 1))
            bool result = (uDescentLevel > 1);
            if (result) {
               --uDescentLevel;
               ppccscDescentCursor->setToNext();
               cursor.pushSon(ppccscDescentCursor->elementAt().getCursor());
            };
            return result;
         }
      bool hasDescent() const { return (uDescentLevel > 1); }
   };
};

inline void
GenericImplTreeCursor::normalizeAfterEndSuppression(bool hasRoot) {
   if (isOnTree() || isDownCursor()) {
      if (hasStack()) {
         if (!lastCursor().isValid())
            setMode(VMInvalidCursor);
      }
      else if (!hasRoot)
         setMode(VMUpRoot);
   };
}

/* inline methods of the class GenericImplTreeCursor */

inline bool
GenericImplTreeCursor::isBrother(const GenericImplTreeCursor& cursor) const {
   return (getLevel() == cursor.getLevel())
      && ((isDownFinal() || cursor.isDownFinal())
         ? (isDownFinal() && cursor.isDownFinal() && ccsStack.isEqual(cursor.ccsStack))
         : ccsStack.isParentEqual(cursor.ccsStack));
}

inline bool
GenericImplTreeCursor::isBrother(const GenericImplTreeCursor& cursor, const FlatExtendedLocateParameters& pos) const {
   return isBrother(cursor) && ((getLevel() == 0)
      ? (pos.isUndefined() || (pos.isNear() && (cursor.isUpRoot() != isUpRoot()))
            || (pos.isExact() && (cursor.isUpRoot() == isUpRoot())))
      : VirtualCollectionCursor::CursorAccess::isPositionned(lastCursor(), pos, &cursor.lastCursor()));
}

inline bool
GenericImplTreeCursor::isSon(const GenericImplTreeCursor& cursor, const FlatExtendedLocateParameters& pos) const {
   if (!cursor.isOnTree())
      return false;
   if (isDownFinal())
      return ccsStack.isEqual(cursor.ccsStack);
   if (hasFlatSupport())
      return ccsStack.isDown(cursor.ccsStack)
         && VirtualCollectionCursor::CursorAccess::isPositionned(lastCursor(), pos);
   return false;
}

inline void
GenericImplTreeCursor::cut(const GenericImplTreeCursor& cursor) {
   AssumeCondition(cursor.isOnTree())
   StackPrefix prefix(*this);
   if (prefix.hasPrefix(cursor))
      prefix.cut();
}

/*******************************************/
/* Definition of the class GenericImplTree */
/*******************************************/

class GenericImplTree : public TImplTree<GenericTreeElement, GenericImplTreeCursor> {
  private:
   typedef TImplTree<GenericTreeElement, GenericImplTreeCursor> inherited;

  public:
   GenericImplTree() {}
   GenericImplTree(const GenericImplTree& source) {}
   GenericImplTree& operator=(const GenericImplTree& source) = default;
   void swap(GenericImplTree& source) { inherited::swap(source); }
};

/***********************************************************************/
/* Definition of the inline methods of the class GenericImplTreeCursor */
/***********************************************************************/

inline
GenericImplTreeCursor::GenericImplTreeCursor(const GenericImplTree& tree)
   { mergeOwnField(tree.hasRoot() ? VMTree : VMUpRoot); }

inline void
GenericImplTreeCursor::pushSon(const GenericImplTree& tree) {
   AssumeCondition(getMode() == VMTree)
   if (ccsStack.isEmpty()) {
      if (!tree.root().isFinal()) {
         ccsStack.push(tree.root().getSons().newCursor());
         setMode(VMInvalidCursor);
      }
      else
         setMode(VMDownCursor);
   }
   else
      setMode(ccsStack.push() ? VMInvalidCursor : VMDownCursor);
}

inline void
GenericImplTreeCursor::pushSon(const VirtualCollectionCursor& sonCursor) {
   AssumeCondition(getMode() == VMTree)
   ccsStack.push(sonCursor);
   if (!sonCursor.isValid())
      setMode(VMInvalidCursor);
}

inline bool
GenericImplTreeCursor::setFather() {
   if (getMode() == VMTree) {
      if (!ccsStack.isEmpty())
         ccsStack.pop();
      else
         setMode(VMUpRoot);
   }
   else if (getMode() == VMInvalidCursor) {
      ccsStack.pop();
      setMode(VMTree);
   }
   else if (getMode() == VMDownCursor)
      setMode(VMTree);
   return isValid();
}

inline VirtualCollection::PPCursor
GenericImplTreeCursor::setFatherWithLastCursorExtraction() {
   if (getMode() == VMTree) {
      if (!ccsStack.isEmpty())
         return ccsStack.popLastCursor();
      else
         setMode(VMUpRoot);
   }
   else if (getMode() == VMInvalidCursor) {
      setMode(VMTree);
      return ccsStack.popLastCursor();
   }
   else if (getMode() == VMDownCursor)
      setMode(VMTree);
   return VirtualCollection::PPCursor();
}

inline bool
GenericImplTreeCursor::setRoot(const GenericImplTree& tree)
   {  ccsStack.clear(); setMode(tree.hasRoot() ? VMTree : VMUpRoot); return isValid(); }

inline bool
GenericImplTreeCursor::setBrother(const FlatPosition& pos, const GenericImplTree& tree) {
   if (hasFlatSupport())
      return VirtualCollectionCursor::CursorAccess::position(lastCursor(), pos);
   if (isDownFinal())
      return false;
   if (isUpRoot())
      setMode(tree.hasRoot() && !pos.isInvalid() ? VMTree : VMUpRoot);
   else if (pos.isNear()) // isRoot()
      setMode(VMUpRoot);
   return isValid();
}

inline bool
GenericImplTreeCursor::setSon(const FlatPosition& pos, const GenericImplTree& tree) {
   pushSon(tree);
   bool result = false;
   if (hasFlatSupport()) {
      result = VirtualCollectionCursor::CursorAccess::position(lastCursor(), pos);
      if (result)
         setMode(VMTree);
   };
   return result;
}

} // end of namespace COL

#endif // COL_ImplTreeH

