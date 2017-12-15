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
// File      : ImplCommonTree.h
// Description :
//   Definition of a common implementation for all tree collections.
//

#ifndef COL_ImplCommonTreeH
#define COL_ImplCommonTreeH

#include "Collection/VirtualCollection/VirtualTree.h"

namespace COL {

/*******************************************/
/* Definition of the class ImplTreeElement */
/*******************************************/

class ImplTreeElement {
  private:
   APVirtualCollection apvcSons;

  protected:
   void createSons(VirtualCollection* sonsCollection)
      {  apvcSons.absorbElement(sonsCollection); }

  public:
   ImplTreeElement() {}
   ImplTreeElement(VirtualCollection* sonsCollection)
      :  apvcSons(sonsCollection, APVirtualCollection::Init()) {}
   ImplTreeElement(const ImplTreeElement& source)
      {  if (source.apvcSons.isValid())
            apvcSons.assign(source.apvcSons->createSDefault(), APVirtualCollection::Init());
      }
   ImplTreeElement& operator=(const ImplTreeElement& source) { return *this; }

   const VirtualCollection& getSons() const { return *apvcSons; }
   VirtualCollection& sons() { return *apvcSons; }
   bool hasSons() const { return apvcSons.isValid(); }
   bool containSons() const { return hasSons() && !apvcSons->isEmpty(); }
   bool isFinal() const { return !apvcSons.isValid(); }
   void swapSons(ImplTreeElement& source) { apvcSons.swap(source.apvcSons); }

   // typedef ... FlatCollection
   // typedef ... HandlerSonsAndSubTreeCast
};

/***************************************************/
/* Definition of the template class TImplBasicTree */
/***************************************************/

namespace DImplBasicTree {

template <class TNode>
class TemplateNode { public: typedef TNode TypeNode; };

template <class TypeTemplateNode>
int queryCNumberOfElements(TypeTemplateNode node,
      const typename TypeTemplateNode::TypeNode::FlatCollection& sons); // recursive counting

template <class TypeNode>
int queryNumberOfElements(const TypeNode* element) {
   return (element == nullptr) ? 0
      : ((element->isFinal() || (element->getSons().count() == 0)) ? 1
      : (queryCNumberOfElements(TemplateNode<TypeNode>(), element->getSons())+1));
}

} // end of namespace DImplBasicTree

template <class TypeNode>
class TImplBasicTree {
  private:
   typedef TImplBasicTree<TypeNode> thisType;

   TypeNode* ptnRoot;
   int uCount;

  protected:
   typedef typename TypeNode::HandlerSonsAndSubTreeCast HandlerSonsAndSubTreeCast;
   typedef typename TypeNode::FlatCollection FlatColletion;
   int& count() { return uCount; }

  public:
   TImplBasicTree() : ptnRoot(nullptr), uCount(0) {}
   TImplBasicTree(const thisType& ibtSource) : ptnRoot(nullptr), uCount(0) {}
   thisType& operator=(const thisType& source)
      {  ptnRoot = nullptr; uCount = 0; return *this; }
   bool isValid() const
      {  return ptnRoot ? (uCount > 0) : (uCount == 0); }

   bool hasRoot() const { return ptnRoot; }
   TypeNode& root() const { AssumeCondition(ptnRoot) return *ptnRoot; }
   const TypeNode& getRoot() const { return root(); }
   TypeNode* getSRoot() const { return ptnRoot; }
   void setRoot(TypeNode* root)
      {  AssumeCondition(ptnRoot == nullptr)
         ptnRoot = root;
         uCount = queryNumberOfElements(ptnRoot);
      }
   void removeRoot() { ptnRoot = nullptr; uCount = 0; }
   void freeRoot() { if (ptnRoot) delete ptnRoot; ptnRoot = nullptr; uCount = 0; }
   const int& getCount() const { return uCount; }

   static int queryNumberOfElements(const TypeNode* element)
      {  return DImplBasicTree::queryNumberOfElements(element); }
   void swap(thisType& source);
};

template <class TypeNode>
inline void
TImplBasicTree<TypeNode>::swap(thisType& source) {
   TypeNode* temp = ptnRoot;
   ptnRoot = source.ptnRoot;
   source.ptnRoot = temp;

   int tempCount = uCount;
   uCount = source.uCount;
   source.uCount = tempCount;
}

/**********************************************/
/* Definition of the template class TImplTree */
/**********************************************/

template <class TypeNode, class TypeCursor>
class TImplTree : public TImplBasicTree<TypeNode>, public DVirtualTree::Definitions {
  private:
   typedef TImplTree<TypeNode, TypeCursor> thisType;
   typedef TImplBasicTree<TypeNode> inherited;

  public:
   typedef VirtualCollection::CollectionAccess::ExtendedLocateParameters FlatExtendedLocateParameters;
   typedef VirtualCollection::CollectionAccess::ExtendedInsertionParameters FlatExtendedInsertionParameters;
   typedef VirtualCollection::CollectionAccess::ExtendedSuppressParameters FlatExtendedSuppressParameters;
   typedef VirtualTree::CollectionAccess::ExtendedTreeLocateParameters ExtendedTreeLocateParameters;

   typedef typename TypeNode::FlatCollection FlatCollection;
   typedef typename FlatCollection::Cursor FlatCursor;
   typedef typename FlatCollection::PPCursor PPFlatCursor;
   typedef typename FlatCursor::CursorAccess::Position FlatPosition;

   TImplTree() {}
   TImplTree(const thisType& source) : TImplBasicTree<TypeNode>(source) {}

   TypeNode& getSElement(const TypeCursor* cursor) const;
   void addBrother(TypeNode* newElement, TypeCursor& cursor,
         const FlatExtendedInsertionParameters& pos);
   void addRemoteBrother(TypeNode* newElement, const TypeCursor& cursor,
         const FlatExtendedInsertionParameters& pos, FlatCursor* brotherCursor=nullptr);
   void addRemoteSon(TypeNode* newElement, const TypeCursor& cursor,
         const FlatExtendedInsertionParameters& pos, FlatCursor* sonCursor=nullptr);
   void removeAtomic(TypeCursor& cursor, FlatExtendedSuppressParameters& parameters);
   void remove(TypeCursor& cursor, FlatExtendedSuppressParameters& parameters);
   void removeAll(TypeCursor& parent, const FlatCursor* start=nullptr,
         const FlatCursor* end=nullptr);

   void freeLastAtomicSon(TypeCursor& cursor);
   void freeBeforeAtomic(TypeCursor& cursor);

  protected:
   // Methods for positionning the cursor
   void setElementCursor(TypeCursor& cursor, const ExtendedTreeLocateParameters& parameters) const;
   void setParentCursor(TypeCursor& cursor, PPFlatCursor& son,
         const ExtendedTreeLocateParameters& parameters) const;
   bool queryCollectionCursor(const TypeCursor* cursor,
         const ExtendedTreeLocateParameters& parameters, FlatCollection*& collectionResult,
         PPFlatCursor& cursorResult, TypeCursor* ascendance=nullptr) const;
};

template <class TypeNode, class TypeCursor>
inline TypeNode&
TImplTree<TypeNode, TypeCursor>::getSElement(const TypeCursor* cursor) const {
   AssumeCondition(!cursor || cursor->isValid())
   return cursor && !cursor->isRoot() ? cursor->lastElement() : inherited::root();
}

template <class TypeNode, class TypeCursor>
inline void
TImplTree<TypeNode, TypeCursor>::addBrother(TypeNode* newElement, TypeCursor& cursor,
      const FlatExtendedInsertionParameters& pos) {
   AssumeCondition(cursor.hasFlatSupport())
   VirtualCollection::CollectionAccess::add(cursor.lastSupport(),
         inherited::HandlerSonsAndSubTreeCast::castTo(newElement), pos, &cursor.lastCursor());
   cursor.setMode(VMTree);
   inherited::count() += inherited::queryNumberOfElements(newElement);
   cursor.setElementLevel(*newElement);
}

template <class TypeNode, class TypeCursor>
inline void
TImplTree<TypeNode, TypeCursor>::addRemoteBrother(TypeNode* newElement,
      const TypeCursor& cursor, const FlatExtendedInsertionParameters& pos,
      FlatCursor* brotherCursor) {
   AssumeCondition(cursor.hasFlatSupport())
   VirtualCollection::CollectionAccess::add(cursor.lastSupport(),
         inherited::HandlerSonsAndSubTreeCast::castTo(newElement), pos, brotherCursor);
   inherited::count() += inherited::queryNumberOfElements(newElement);
   cursor.setElementLevel(*newElement);
}

template <class TypeNode, class TypeCursor>
inline void
TImplTree<TypeNode, TypeCursor>::addRemoteSon(TypeNode* newElement,
      const TypeCursor& cursor, const FlatExtendedInsertionParameters& pos,
      FlatCursor* sonCursor) {
   VirtualCollection::CollectionAccess::add(getSElement(&cursor).sons(),
         inherited::HandlerSonsAndSubTreeCast::castTo(newElement), pos, sonCursor);
   inherited::count() += inherited::queryNumberOfElements(newElement);
   cursor.setElementSubLevel(*newElement);
}

template <class TypeNode, class TypeCursor>
inline void
TImplTree<TypeNode, TypeCursor>::removeAtomic(TypeCursor& cursor, FlatExtendedSuppressParameters& parameters) {
   AssumeCondition(cursor.isOnTree())
   if (cursor.hasFlatSupport()) {
      if (!parameters.isFree())
         cursor.resetElementLevel((TypeNode&) *inherited::HandlerSonsAndSubTreeCast::castFrom(
            (const typename inherited::HandlerSonsAndSubTreeCast::Base*) inherited::HandlerSonsAndSubTreeCast::Base::castFromCopyHandler(
            VirtualCollection::CollectionAccess::getElement(
               cursor.lastSupport(), parameters, &cursor.lastCursor()))));
      VirtualCollection::CollectionAccess::remove(cursor.lastSupport(),
         parameters, &cursor.lastCursor());
      if (!cursor.lastCursor().isValid())
         cursor.setMode(TypeCursor::VMInvalidCursor);
      inherited::count()--;
   }
   else {
      if (parameters.isFree())
         inherited::freeRoot();
      else
         inherited::removeRoot();
      cursor.setMode(TypeCursor::VMUpRoot);
   }
}

template <class TypeNode, class TypeCursor>
inline void
TImplTree<TypeNode, TypeCursor>::remove(TypeCursor& cursor, FlatExtendedSuppressParameters& parameters) {
   AssumeCondition(cursor.isOnTree())
   if (cursor.hasFlatSupport()) {
      VirtualCollectionCursor* lastCursor = &cursor.lastCursor();
      int removedElements = inherited::queryNumberOfElements(inherited::HandlerSonsAndSubTreeCast::castFrom(
         (const typename inherited::HandlerSonsAndSubTreeCast::Base*) inherited::HandlerSonsAndSubTreeCast::Base::castFromCopyHandler(
         VirtualCollection::CollectionAccess::getElement(cursor.lastSupport(), parameters,
            lastCursor))));
      if (!parameters.isFree())
         cursor.resetElementLevel((TypeNode&) *inherited::HandlerSonsAndSubTreeCast::castFrom(
            (const typename inherited::HandlerSonsAndSubTreeCast::Base*) inherited::HandlerSonsAndSubTreeCast::Base::castFromCopyHandler(
               VirtualCollection::CollectionAccess::getElement(
               cursor.lastSupport(), parameters, lastCursor))));
      VirtualCollection::CollectionAccess::remove(cursor.lastSupport(),
         parameters, lastCursor);
      if (!lastCursor->isValid())
         cursor.setMode(TypeCursor::VMInvalidCursor);
      inherited::count() -= removedElements;
   }
   else {
      if (parameters.isFree())
         inherited::freeRoot();
      else
         inherited::removeRoot();
   }
}

template <class TypeNode, class TypeCursor>
inline void
TImplTree<TypeNode, TypeCursor>::removeAll(TypeCursor& parent, const FlatCursor* start,
      const FlatCursor* end) {
   VirtualCollection* collection = &getSElement(&parent).sons();
   int removedElements = 0;
   {  VirtualCollection::PPCursor cursor = collection->newCursor();
      if (start)
         *cursor = *start;
      else
         cursor->setToFirst();
      while (cursor->isValid() && (!end || !cursor->isEqual(*end))) {
         removedElements += inherited::queryNumberOfElements(inherited::HandlerSonsAndSubTreeCast::castFrom(
            (const typename inherited::HandlerSonsAndSubTreeCast::Base*) inherited::HandlerSonsAndSubTreeCast::Base::castFromCopyHandler(
            &cursor->elementAt())));
         parent.resetElementSubLevel((TypeNode&) inherited::HandlerSonsAndSubTreeCast::castFrom(
            (const typename inherited::HandlerSonsAndSubTreeCast::Base&) inherited::HandlerSonsAndSubTreeCast::Base::castFromCopyHandler(
            cursor->elementSAt())));
      };
   };
   VirtualCollection::CollectionAccess::removeAll(*collection,
      VirtualCollection::CollectionAccess::ExtendedSuppressParameters(), start, end);
   inherited::count() -= removedElements;
}

template <class TypeNode, class TypeCursor>
inline void
TImplTree<TypeNode, TypeCursor>::freeLastAtomicSon(TypeCursor& cursor) {
   getSElement(&cursor).sons().freeLast();
   inherited::count()--;
}

template <class TypeNode, class TypeCursor>
inline void
TImplTree<TypeNode, TypeCursor>::freeBeforeAtomic(TypeCursor& cursor) {
   AssumeCondition(cursor.isOnTree())
   if (cursor.hasFlatSupport()) {
      VirtualCollection::CollectionAccess::remove(cursor.lastSupport(),
         VirtualCollection::CollectionAccess::ExtendedSuppressParameters().setFree().setBefore(),
         &cursor.lastCursor());
      inherited::count()--;
   }
   else {
      AssumeCondition(inherited::count() == 1)
      inherited::removeRoot();
   }
}

template <class TypeNode, class TypeCursor>
inline bool
TImplTree<TypeNode, TypeCursor>::queryCollectionCursor(const TypeCursor* cursor,
      const ExtendedTreeLocateParameters& parameters, FlatCollection*& collectionResult,
      PPFlatCursor& cursorResult, TypeCursor* ascendance) const {
   bool result = true;
   if (ascendance && cursor)
      *ascendance = *cursor;
   if (cursor && cursor->hasFlatSupport())
      cursorResult.setElement(cursor->lastCursor());
   if (parameters.isSons()) {
      collectionResult = cursorResult.isValid()
         ? &inherited::HandlerSonsAndSubTreeCast::castFrom(cursorResult->elementSAt()).sons()
         : &inherited::root().sons();
      cursorResult.release();
   }
   else if (parameters.isInnerSons())
      result = false;
   else if (cursorResult.isValid()) {
      collectionResult = (FlatCollection*) &VirtualCollectionCursor::CursorAccess::getSupport(*cursorResult);
      if (ascendance)
         ascendance->setFather();
   };
   return result;
}

} // end of namespace COL

#endif  // COL_ImplCommonTreeH
