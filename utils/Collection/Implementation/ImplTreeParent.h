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
// File      : ImplTreeParent.h
// Description :
//   Definition of the classes GenericImplParentTree that define tree based
//   collection implementations. The elements have a reference to their father
//   to localize them quickly in the tree.
//

#ifndef COL_ImplTreeParentH
#define COL_ImplTreeParentH

#include "Collection/Implementation/ImplCommonTree.h"
#include "Collection/ConcreteCollection/Array.h"
#include "Collection/ConcreteCollection/BasicList.h"

namespace COL {

/**************************************************************/
/* Definition of the template class TGenericParentTreeElement */
/**************************************************************/

template <class TypeFlatCollection, class TypeFlatNode>
class TGenericParentTreeElement : public TypeFlatNode, public ImplTreeElement {
  private:
   typedef TypeFlatNode inherited;
   typedef TGenericParentTreeElement<TypeFlatCollection, TypeFlatNode> thisType;
   thisType* pgpteParent;

  protected:
   virtual ComparisonResult _compare(const EnhancedObject& asource) const override
      {  return inherited::_compare(asource); }

  public:
   TGenericParentTreeElement() : pgpteParent(nullptr) {}
   TGenericParentTreeElement(TypeFlatCollection* sonsCollection)
      :  ImplTreeElement(sonsCollection), pgpteParent(nullptr) {}
   TGenericParentTreeElement(const thisType& source)
      :  inherited(source), ImplTreeElement(source), pgpteParent(nullptr) {}
   DefineCopy(thisType)
   DDefineAssign(thisType)
   thisType& operator=(const thisType& source)
      {  inherited::operator=(source);
         ImplTreeElement::operator=(source);
         return *this;
      }

   const TypeFlatCollection& getSons() const
      {  return (const TypeFlatCollection&) ImplTreeElement::getSons(); }
   TypeFlatCollection& sons() { return (TypeFlatCollection&) ImplTreeElement::sons(); }
   void swapSons(thisType& tpteSource) { ImplTreeElement::swapSons(tpteSource); }

   typedef HandlerCast<TGenericParentTreeElement<TypeFlatCollection, TypeFlatNode>, TypeFlatNode>
      HandlerSonsAndSubTreeCast;
   typedef TypeFlatCollection FlatCollection;
   typedef TypeFlatNode FlatNode;

   void setParent(thisType& parent) { AssumeCondition(!pgpteParent) pgpteParent = &parent; }
   void setSParent(thisType& parent) { pgpteParent = &parent; }
   void releaseParent() { pgpteParent=nullptr; }
   thisType& getParent() const { AssumeCondition(pgpteParent) return *pgpteParent; }
   thisType* parent() const { return pgpteParent; }
   bool hasParent() const { return pgpteParent != nullptr; }
   bool isRoot() const { return pgpteParent == nullptr; }
};

/*****************************************************************/
/* Definition of the template class TGenericImplParentTreeCursor */
/*****************************************************************/

template <class TypeFlatCollection, class TypeFlatNode>
class TGenericImplParentTree;

template <class TypeFlatCollection, class TypeFlatNode>
class TGenericImplParentTreeCursor : public ExtendedParameters {
  public:
   typedef VirtualCollectionCursor::CursorAccess::ExtendedLocateParameters FlatExtendedLocateParameters;
   typedef VirtualCollectionCursor::CursorAccess::Position FlatPosition;
   typedef VirtualTreeCursor::ValidityMode ValidityMode;
   static const ValidityMode VMTree = VirtualTreeCursor::VMTree;
   static const ValidityMode VMUpRoot = VirtualTreeCursor::VMUpRoot;
   static const ValidityMode VMDownCursor = VirtualTreeCursor::VMDownCursor;
   static const ValidityMode VMInvalidCursor = VirtualTreeCursor::VMInvalidCursor;
   typedef TGenericParentTreeElement<TypeFlatCollection, TypeFlatNode> Node;
   typedef typename TypeFlatCollection::Cursor SonsCursor;
   typedef typename TypeFlatCollection::PPCursor PPSonsCursor;
   typedef TGenericImplParentTree<TypeFlatCollection, TypeFlatNode> Tree;

   class StackPrefix {
     private:
      typedef TGenericImplParentTreeCursor<TypeFlatCollection, TypeFlatNode> TreeCursor;
      TreeCursor* ptcTreeCursor;
      Node* pnPrefix;
      Node* pnSonPrefix;

     public:
      StackPrefix(TreeCursor& treeCursor)
         :  ptcTreeCursor(&treeCursor), pnPrefix(nullptr), pnSonPrefix(nullptr) {}
      StackPrefix(const StackPrefix& source) = default;
      StackPrefix& operator=(const StackPrefix& spSource) = default;

      bool hasPrefix(const TreeCursor& tree)
         {  AssumeCondition(pnPrefix == nullptr)
            return ptcTreeCursor->isDescendant(tree, pnPrefix, pnSonPrefix);
         }
      bool isFull() const { AssumeCondition(pnPrefix != nullptr) return (pnSonPrefix == nullptr); }
      void advance() { pnPrefix = pnSonPrefix; pnSonPrefix = nullptr; }
      void replacePrefix(const TreeCursor& newPrefix) {}
      void cut()
         {  AssumeCondition(pnPrefix)
            ptcTreeCursor->setElement(*pnPrefix);
            ptcTreeCursor->setToInvalidSon();
         }
   };
   
   class CursorPath {
     private:
      typedef TGenericImplParentTreeCursor<TypeFlatCollection, TypeFlatNode> TreeCursor;
      typedef TGenericImplParentTree<TypeFlatCollection, TypeFlatNode> TreeSupport;
      typedef TArray<Node, HandlerCopyCast<Node> > NodeArray;
      NodeArray naDescentPath;
      int uAscentLevel;
      const TreeSupport& giptTree;
      
     public:
      CursorPath(const TreeSupport& tree, const TreeCursor* fstCursor, const TreeCursor* sndCursor);
      CursorPath(const CursorPath& source) = default;
      CursorPath& operator=(const CursorPath& source)
         {  AssumeCondition(&giptTree == &source.giptTree)
            naDescentPath = source.naDescentPath;
            uAscentLevel = source.uAscentLevel;
            return *this;
         }
      bool hasAscent() const { return (uAscentLevel > 1); }
      bool ascent(TreeCursor& cursor)
         {  AssumeCondition(uAscentLevel >= 1)
            bool result = (uAscentLevel > 1);
            if (result) {
               uAscentLevel--;
               cursor.setFather();
               AssumeCondition(&naDescentPath.getLast() == &cursor.lastElement())
            };
            return result;
         }
      bool up(TreeCursor& cursor)
         {  AssumeCondition(uAscentLevel == 1)
            uAscentLevel--;
            cursor.setFather();
            return cursor.isValid();
         }
      bool down(TreeCursor& cursor)
         {  AssumeCondition(uAscentLevel == 0)
            uAscentLevel++;
            cursor.setSonElement(naDescentPath.extractLast());
            return cursor.isValid();
         }
      bool next(TreeCursor& cursor)
         {  AssumeCondition(uAscentLevel >= 1)
            return (uAscentLevel > 1)
               ?  cursor.setBrother(FlatPosition().setForward(), giptTree)
               :  (!naDescentPath.isEmpty()
                     && (&naDescentPath.getLast() != &cursor.lastElement())
                     && cursor.setBrother(FlatPosition().setForward(), giptTree));
         }
      bool hasNext(TreeCursor& cursor) const
         {  return (uAscentLevel > 0) && !cursor.isLastBrother() && ((uAscentLevel > 1)
               || (!naDescentPath.isEmpty() && (&naDescentPath.getLast() != &cursor.lastElement())));
         }
      bool descent(TreeCursor& cursor)
         {  AssumeCondition(uAscentLevel == 1)
            bool result = !naDescentPath.isEmpty();
            if (result)
               cursor.setSonElement(naDescentPath.extractLast());
            return result;
         }
      bool hasDescent() const { return !naDescentPath.isEmpty(); }
   };

   typedef TGenericImplParentTreeCursor<TypeFlatCollection, TypeFlatNode> thisType;
   typedef typename Node::HandlerSonsAndSubTreeCast HandlerSonsAndSubTreeCast;

  private:
   PPSonsCursor ppscElement;
   Node* pnFather;

   bool invariantFather() const
      {  return pnFather == HandlerSonsAndSubTreeCast::castFrom(ppscElement->elementAt()).parent(); }

  protected:
   DefineExtendedParameters(2, ExtendedParameters)

  public:
   TGenericImplParentTreeCursor() : pnFather(nullptr) { setOwnField(VMUpRoot); }
   TGenericImplParentTreeCursor(const Tree& support);
   TGenericImplParentTreeCursor(const thisType& source)
      :  ExtendedParameters(source),
         ppscElement(source.ppscElement, PNT::Pointer::Duplicate()),
         pnFather(source.pnFather) {}
   thisType& operator=(const thisType& source);
   void swap(thisType& source)
      {  unsigned int tempParams = queryParams();
         params() = source.queryParams();
         source.params() = tempParams;
         ppscElement.swap(source.ppscElement);
         Node* temp = pnFather;
         pnFather = source.pnFather;
         source.pnFather = temp;
      }

   // query methods
   ValidityMode getMode() const { return (ValidityMode) queryOwnField(); }
   void setMode(ValidityMode mode) { setOwnField(mode); }
   bool isCollectionCursor() const { return ((getMode() == VMTree) && ppscElement.isValid()); }
   bool isOnTree() const { return getMode() == VMTree; }
   bool isUpRoot() const { return getMode() == VMUpRoot; }
   bool isRoot() const { return (getMode() == VMTree) && !ppscElement.isValid(); }
   bool isDownCursor() const { return queryOwnField() & VMDownCursor; }
   bool isDownFinal() const { return queryOwnField() == VMDownCursor; }
   bool isDownFinalRoot() const { return (queryOwnField() == VMDownCursor) && !ppscElement.isValid(); }
   bool isInvalidCursor() const { return getMode() == VMInvalidCursor; }
   bool hasFlatSupport() const { return (getMode() & VMTree) && !isRoot(); }

   Node& element() const
      {  AssumeCondition(isOnTree())
         return (!ppscElement.isValid() && (pnFather != nullptr))
            ? *pnFather : HandlerSonsAndSubTreeCast::castFrom(ppscElement->elementSAt());
      }
   Node& lastElement() const { return element(); }
   bool hasParentElement() const { return !isUpRoot() && !isRoot(); }
   Node* parentElement() const { return isRoot() ? nullptr : pnFather; }

   SonsCursor& lastCursor() const { return *ppscElement; }
   TypeFlatCollection& lastSupport() const
      {  AssumeCondition((pnFather != nullptr) && !isRoot())
         return pnFather->sons();
      }

   bool isValid() const { return (getMode() == VMTree); }
   bool repairMode();
   void updateElementFather()
      {  if (getMode() == VMTree) {
            Node* parent = parentElement();
            if (parent)
               lastElement().setSParent(*parent);
         };
      }
   bool invariant() const;

   int queryLevel() const;
   int getLevel() const { return queryLevel(); /* for concrete collections */ }
   bool isBrother(const thisType& cursor, const FlatExtendedLocateParameters& params) const;
   bool isSon(const thisType& cursor, const FlatExtendedLocateParameters& params) const;
   bool isDescendant(const thisType& cursor, const FlatExtendedLocateParameters& params) const;
   bool isDescendant(const thisType& source, Node*& prefix, Node*& sonPrefix) const;
   bool isFirstBrother() const
      {  return (getMode() == VMTree) && ((pnFather == nullptr)
            || VirtualCollectionCursor::CursorAccess::isPositionned(*ppscElement,
                  VirtualCollection::ExtendedLocateParameters().setAfter(), nullptr));
      }
   bool isLastBrother() const
      {  return (getMode() == VMTree) && ((pnFather == nullptr)
            || VirtualCollectionCursor::CursorAccess::isPositionned(*ppscElement,
                  VirtualCollection::ExtendedLocateParameters().setBefore(), nullptr));
      }
   ComparisonResult compare(const thisType& source) const;
   VirtualTreeCursor::ExtendedComparisonResult fullCompare(const thisType& source) const;
   ComparisonResult levelCompare(const thisType& source) const
      {  return fcompare(queryLevel(), source.queryLevel()); }
   bool isEqual(const thisType& source) const;

   void setElementLevel(Node& element) const { if (pnFather) element.setParent(*pnFather); }
   void setElementSubLevel(Node& element) const
      {  if (getMode() == VMTree) element.setParent(lastElement()); }
   void resetElementLevel(Node& element) const { element.releaseParent(); }
   void resetElementSubLevel(Node& element) const { element.releaseParent(); }
   
   // set methods
   void setElement(Node& reference);
   void setSonElement(Node& reference);
   bool setToInvalidSon();
   bool setFather();

   bool setBrother(const FlatPosition& pos, const Tree& tree);
   void pushSon(const Tree& tree);
   bool setSon(const FlatPosition& pos, const Tree& tree);
   PPSonsCursor setFatherWithLastCursorExtraction();
   bool setRoot(const Tree& tree);

   bool cut(const thisType& source);
   void normalize();

   void normalizeAfterEndSuppression(bool hasRoot);
   enum AssignToParameter { ATPWithRoot, ATPWithoutRoot };
   void assignTo(TBasicList<Node>& hierarchy, AssignToParameter param);
   void intersect(const thisType& source, TBasicList<Node>& thisDiff,
         TBasicList<Node>& sourceDiff, thisType& common);

   void invalidate()
      {  ppscElement.release(); pnFather = nullptr; setMode(VMUpRoot); }
   void gotoReference(const Node& element) { setElement(const_cast<Node&>(element)); }
};

/* Inline methods of the template class TGenericImplParentTreeCursor */

template <class TypeFlatCollection, class TypeFlatNode>
inline bool
TGenericImplParentTreeCursor<TypeFlatCollection, TypeFlatNode>::invariant() const {
   return (getMode() == VMUpRoot)
      ? ((pnFather == nullptr) && !ppscElement.isValid()) : ((getMode() == VMTree)
      ? ((!ppscElement.isValid() && (pnFather != nullptr))
            || (ppscElement.isValid() && ppscElement->isValid() && invariantFather()))
         : ((pnFather != nullptr) && ((getMode() == VMDownCursor)
      ?  !ppscElement.isValid() : (ppscElement.isValid() && !ppscElement->isValid()))));
}

template <class TypeFlatCollection, class TypeFlatNode>
inline TGenericImplParentTreeCursor<TypeFlatCollection, TypeFlatNode>&
TGenericImplParentTreeCursor<TypeFlatCollection, TypeFlatNode>::operator=(const thisType& source) {
   ExtendedParameters::operator=(source);
   ppscElement.fullAssign(source.ppscElement);
   pnFather = source.pnFather;
   return *this;
}

template <class TypeFlatCollection, class TypeFlatNode>
inline ComparisonResult
TGenericImplParentTreeCursor<TypeFlatCollection, TypeFlatNode>::compare(const thisType& source) const {
   VirtualTreeCursor::ExtendedComparisonResult result = fullCompare(source);
   return (result.isValid()) ? result.getBasicResult() : CRNonComparable;
}

template <class TypeFlatCollection, class TypeFlatNode>
inline bool
TGenericImplParentTreeCursor<TypeFlatCollection, TypeFlatNode>::isEqual(const thisType& source) const {
   return (getMode() == source.getMode()) && (pnFather == source.pnFather)
      && (ppscElement.isValid()
      ? (source.ppscElement.isValid() && ppscElement->isEqual(*source.ppscElement))
      : !source.ppscElement.isValid());
}

template <class TypeFlatCollection, class TypeFlatNode>
inline bool
TGenericImplParentTreeCursor<TypeFlatCollection, TypeFlatNode>::isSon(const thisType& cursor,
      const FlatExtendedLocateParameters& params) const {
   return cursor.isOnTree() && (pnFather == &cursor.element())
      && (isDownFinal() || VirtualCollectionCursor::CursorAccess::isPositionned(*ppscElement, params));
}

template <class TypeFlatCollection, class TypeFlatNode>
inline void
TGenericImplParentTreeCursor<TypeFlatCollection, TypeFlatNode>::setElement(Node& reference) {
   setMode(VMTree);
   pnFather = reference.parent();
   if (pnFather) {
      ppscElement = pnFather->getSons().newCursor();
      ppscElement->gotoReference(reference);
   }
   else
      pnFather = &reference;
}

template <class TypeFlatCollection, class TypeFlatNode>
inline void
TGenericImplParentTreeCursor<TypeFlatCollection, TypeFlatNode>::setSonElement(Node& reference) {
   pnFather = &reference;
   if (reference.hasSons()) {
      setMode(VMInvalidCursor);
      ppscElement = pnFather->getSons().newCursor();
   }
   else
      setMode(VMDownCursor);
}

template <class TypeFlatCollection, class TypeFlatNode>
inline bool
TGenericImplParentTreeCursor<TypeFlatCollection, TypeFlatNode>::setToInvalidSon() {
   AssumeCondition(isOnTree())
   bool result = false;
   if (element().hasSons()) {
      if (ppscElement.isValid())
         pnFather = &HandlerSonsAndSubTreeCast::castFrom(ppscElement->elementSAt());
      ppscElement = pnFather->getSons().newCursor();
      setMode(VMInvalidCursor);
      result = true;
   }
   else {
      if (ppscElement.isValid()) {
         pnFather = &HandlerSonsAndSubTreeCast::castFrom(ppscElement->elementSAt());
         ppscElement.release();
      };
      setMode(VMDownCursor);
   };
   return result;
}

template <class TypeFlatCollection, class TypeFlatNode>
inline bool
TGenericImplParentTreeCursor<TypeFlatCollection, TypeFlatNode>::setFather() {
   AssumeCondition(!isUpRoot())
   bool result = !isRoot();
   if (!result) {
      pnFather = nullptr;
      setMode(VMUpRoot);
   }
   else {
      AssumeCondition(pnFather != nullptr)
      if (pnFather->hasParent()) {
         ppscElement = pnFather->getParent().getSons().newCursor();
         ppscElement->gotoReference(*pnFather);
         pnFather = pnFather->parent();
      }
      else
         ppscElement.release();
      setMode(VMTree);
   };
   return result;
}

template <class TypeFlatCollection, class TypeFlatNode>
inline typename TypeFlatCollection::PPCursor
TGenericImplParentTreeCursor<TypeFlatCollection, TypeFlatNode>::setFatherWithLastCursorExtraction() {
   AssumeCondition(!isUpRoot())
   PPSonsCursor result;
   if (isRoot()) {
      pnFather = nullptr;
      setMode(VMUpRoot);
   }
   else {
      AssumeCondition(pnFather != nullptr)
      result = ppscElement;
      if (pnFather->hasParent()) {
         ppscElement = pnFather->getParent().getSons().newCursor();
         ppscElement->gotoReference(*pnFather);
         pnFather = pnFather->parent();
      };
      setMode(VMTree);
   };
   return result;
}

template <class TypeFlatCollection, class TypeFlatNode>
inline bool
TGenericImplParentTreeCursor<TypeFlatCollection, TypeFlatNode>::cut(const thisType& source) {
   bool result = false;
   AssumeCondition(source.isOnTree())
   if ((result = isEqual(source)) != false) {
      if (isRoot()) {
         pnFather = nullptr;
         setMode(VirtualTreeCursor::VMUpRoot);
      }
      else {
         ppscElement->invalidate();
         setMode(VMInvalidCursor);
      };
   }
   else if ((result = isDescendant(source, FlatExtendedLocateParameters())) != false) {
      pnFather = source.element().parent();
      if (pnFather != nullptr) {
         ppscElement = pnFather->getSons().newCursor();
         setMode(VMInvalidCursor);
      }
      else {
         ppscElement.release();
         setMode(VMUpRoot);
      };
   };
   return result;
}

/***********************************************************/
/* Definition of the template class TGenericImplParentTree */
/***********************************************************/

template <class TypeFlatCollection, class TypeFlatNode>
class TGenericImplParentTree
   :  public TImplTree<TGenericParentTreeElement<TypeFlatCollection, TypeFlatNode>,
                       TGenericImplParentTreeCursor<TypeFlatCollection, TypeFlatNode> > {
  private:
   typedef TImplTree<TGenericParentTreeElement<TypeFlatCollection, TypeFlatNode>,
                     TGenericImplParentTreeCursor<TypeFlatCollection, TypeFlatNode> > inherited;
  public:
   typedef TGenericParentTreeElement<TypeFlatCollection, TypeFlatNode> Node;
   typedef typename TypeFlatCollection::Cursor SonsCursor;
   typedef typename TypeFlatCollection::PPCursor PPSonsCursor;
   typedef TGenericImplParentTree<TypeFlatCollection, TypeFlatNode> thisType;
   typedef TGenericImplParentTreeCursor<TypeFlatCollection, TypeFlatNode> Cursor;
   typedef typename Node::HandlerSonsAndSubTreeCast HandlerSonsAndSubTreeCast;

  public:
   TGenericImplParentTree() {}
   TGenericImplParentTree(const thisType& source) : inherited(source) {}
   thisType& operator=(const thisType& source)
      {  return (thisType&) inherited::operator=(source); }

   void swap(thisType& source) { inherited::swap(source); }
};

/* Inline methods of the template class TGenericImplParentTreeCursor */

template <class TypeFlatCollection, class TypeFlatNode>
inline
TGenericImplParentTreeCursor<TypeFlatCollection, TypeFlatNode>::TGenericImplParentTreeCursor(
      const Tree& tree)
   :  pnFather(tree.hasRoot() ? &tree.root() : nullptr) {}

template <class TypeFlatCollection, class TypeFlatNode>
inline bool
TGenericImplParentTreeCursor<TypeFlatCollection, TypeFlatNode>::setRoot(const Tree& tree) {
   pnFather = tree.getSRoot();
   ppscElement.release();
   setMode((pnFather != nullptr) ? VirtualTreeCursor::VMTree : VirtualTreeCursor::VMUpRoot);
   return (pnFather != nullptr);
}

template <class TypeFlatCollection, class TypeFlatNode>
inline bool
TGenericImplParentTreeCursor<TypeFlatCollection, TypeFlatNode>::setBrother(
      const FlatPosition& pos, const Tree& tree) {
   AssumeCondition(invariant())
   bool result = false;
   if (isRoot() && pos.isNear())
      invalidate();
   else {
      if (ppscElement.isValid())
         setMode(((result = VirtualCollectionCursor::CursorAccess::position(*ppscElement, pos))
                  != false) ? VirtualTreeCursor::VMTree : VirtualTreeCursor::VMInvalidCursor);
      else
         result = (isUpRoot() ? setRoot(tree) : (isRoot() || isDownFinal()));
   };
   return result;
}

template <class TypeFlatCollection, class TypeFlatNode>
inline void
TGenericImplParentTreeCursor<TypeFlatCollection, TypeFlatNode>::pushSon(const Tree& tree)
   {  setToInvalidSon(); }

template <class TypeFlatCollection, class TypeFlatNode>
inline void
TGenericImplParentTreeCursor<TypeFlatCollection, TypeFlatNode>::normalize()
   {  if (!ppscElement->isValid())
         invalidate();
   }

template <class TypeFlatCollection, class TypeFlatNode>
inline void
TGenericImplParentTreeCursor<TypeFlatCollection, TypeFlatNode>::normalizeAfterEndSuppression(bool hasRoot) {
   if (isOnTree() || isDownCursor()) {
      if (ppscElement.isValid()) {
         if (!ppscElement->isValid())
            invalidate();
      }
      else if (!hasRoot)
         setMode(VMUpRoot);
   };
}

template <class TypeFlatCollection, class TypeFlatNode>
inline bool
TGenericImplParentTreeCursor<TypeFlatCollection, TypeFlatNode>::setSon(const FlatPosition& pos,
      const Tree& tree) {
   AssumeCondition(invariant())
   if (isUpRoot())
      return setRoot(tree);
   if (!setToInvalidSon())
      return false;
   bool result = VirtualCollectionCursor::CursorAccess::position(lastCursor(), pos);
   if (result)
      setMode(VMTree);
   return result;
}

} // end of namespace COL

#endif // COL_ImplTreeParentH
