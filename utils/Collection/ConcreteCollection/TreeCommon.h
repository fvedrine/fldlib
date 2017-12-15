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
// File      : TreeCommon.h
// Description :
//   Definition of a template class TGenericTree as a common definition for trees.
//

#ifndef COL_TreeCommonH
#define COL_TreeCommonH

#include "Collection/VirtualCollection/VirtualTree.h"
#include "Collection/ConcreteCollection/List.h"

namespace COL {

/*************************************************/
/* Definition of the template class TGenericTree */
/*************************************************/

template <class TypeNode, class TypeImplCursor, class TypeImplTree>
class TGenericTree;

template <class TypeNode, class TypeImplCursor, class TypeImplTree>
class TGenericTreeCursor;

namespace DTree {

class GenericTree {
  private:
   template <class TypeNode, class TypeImplCursor, class TypeImplTree>
   static typename TypeNode::FlatCollection::Cursor&
      lastCursor(TGenericTreeCursor<TypeNode, TypeImplCursor, TypeImplTree>& cursor);
   template <class TypeNode, class TypeImplCursor, class TypeImplTree>
   static const typename TypeNode::FlatCollection::Cursor&
      lastCursor(const TGenericTreeCursor<TypeNode, TypeImplCursor, TypeImplTree>& cursor);
   template <class TypeNode, class TypeImplCursor, class TypeImplTree>
   static typename TypeNode::FlatCollection&
      lastSupport(const TGenericTreeCursor<TypeNode, TypeImplCursor, TypeImplTree>& cursor);

  public:
   template <class TypeTree, class TemplateCollectionElementKeyCast>
   class KeySortedAccess;
   template <class TypeTree, class TemplateCollectionElementKeyCast>
   class KeyMapSortedAccess;
   template <class TypeTree, class TemplateCollectionElementKeyCast>
   class KeyMultipleSortedAccess;
   template <class TypeTree, class TemplateCollectionElementKeyCast>
   friend class KeySortedAccess;
   template <class TypeTree, class TemplateCollectionElementKeyCast>
   friend class KeyMapSortedAccess;
   template <class TypeTree, class TemplateCollectionElementKeyCast>
   friend class KeyMultipleSortedAccess;
};

} // end of namespace DTree

template <class TypeNode, class TypeImplCursor, class TypeImplTree>
class TGenericTree : public VirtualTree, protected TypeImplTree, public DTree::GenericTree {
  private:
   typedef TGenericTreeCursor<TypeNode, TypeImplCursor, TypeImplTree> InternCursor;

  public:
   typedef TypeNode Node;
   friend class DTree::GenericTree;

  protected:
   typedef typename TypeImplTree::FlatCollection FlatCollection;
   typedef typename FlatCollection::Cursor FlatCursor;
   typedef typename FlatCollection::PPCursor PPFlatCursor;

  private:
   typedef FlatCursor* PFlatCursor;
   typedef FlatCollection* PFlatCollection;
   typedef TGenericTree<TypeNode, TypeImplCursor, TypeImplTree> thisType;
   typedef VirtualTree inherited;

   class TreeMoveNotification;
   class TreeInvalidateNotification;
   class TreeBoundInvalidateNotification;
   class TreeNormalizationAfterSuppressionNotification;
   friend class TreeMoveNotification;
   friend class TreeInvalidateNotification;
   friend class TreeBoundInvalidateNotification;
   friend class TreeNormalizationAfterSuppressionNotification;
   class RouteFreeAction;

  public:
   Template3DefineCollectionForAbstractCollect(TGenericTree, TGenericTreeCursor, TypeNode, TypeImplCursor, TypeImplTree)
   // DefineCollectionForAbstractCollect(thisType, InternCursor)

  protected:
   typedef typename TypeNode::HandlerSonsAndSubTreeCast HandlerSonsAndSubTreeCast;
   typedef VirtualTree::ExtendedTreeLocateParameters ExtendedTreeLocateParameters;
   typedef VirtualTree::ExtendedTreeInsertionParameters ExtendedTreeInsertionParameters;
   typedef VirtualTree::ExtendedTreeSuppressParameters ExtendedTreeSuppressParameters;
   typedef VirtualTree::ExtendedTreeReplaceParameters ExtendedTreeReplaceParameters;

   virtual void _fullAssign(const VirtualCollection& source, const ExtendedReplaceParameters& parameters) override
      {  _fullAssign((const thisType&) source, ((const ExtendedTreeReplaceParameters&) parameters)); }
   void _fullAssign(const thisType& source, const ExtendedTreeReplaceParameters& parameters)
      {  VirtualTree::pfullAssign(source, parameters); }
   virtual void _add(EnhancedObject* anObject, const ExtendedInsertionParameters& parameters,
        VirtualCollectionCursor* cursor=nullptr) override
      {  _add((Node*) Node::castFromCopyHandler(anObject),
            (const ExtendedTreeInsertionParameters&) parameters, (Cursor*) cursor);
      }
   void _add(Node* anObject, const ExtendedTreeInsertionParameters& parameters,
         Cursor* cursor=nullptr);
   virtual void _addAll(const VirtualCollection& source, const ExtendedInsertionParameters& parameters,
         VirtualCollectionCursor* cursor, const VirtualCollectionCursor* startSource,
         const VirtualCollectionCursor* endSource) override;
   void _addAll(const thisType& source, const ExtendedTreeInsertionParameters& parameters,
         Cursor* cursor=nullptr, const Cursor* startSource=nullptr, const Cursor* endSource=nullptr);
   virtual void _moveTo(VirtualCollection& destination, const ExtendedReplaceParameters& parameters,
         VirtualCollectionCursor* cursor, VirtualCollectionCursor* destinationCursor) override
      {  AssumeCondition(dynamic_cast<thisType*>(&destination))
         _moveTo((thisType&) destination, (const ExtendedTreeReplaceParameters&) parameters,
               (Cursor*) cursor, (Cursor*) destinationCursor);
      }
   void _moveTo(thisType& destination, const ExtendedTreeReplaceParameters& parameters,
         Cursor* cursor=nullptr, Cursor* destinationCursor=nullptr);
   virtual void _moveAllTo(VirtualCollection& destination, const ExtendedReplaceParameters& parameters,
         const VirtualCollectionCursor* startCursor, const VirtualCollectionCursor* endCursor,
         VirtualCollectionCursor* destinationCursor) override
      {  AssumeCondition(dynamic_cast<thisType*>(&destination))
         _moveAllTo((thisType&) destination, (const ExtendedTreeReplaceParameters&) parameters,
               (const Cursor*) startCursor, (const Cursor*) endCursor, (Cursor*) destinationCursor);
      }
   void _moveAllTo(thisType& destination, const ExtendedTreeReplaceParameters& parameters,
         const Cursor* startCursor=nullptr, const Cursor* endCursor=nullptr,
         Cursor* destinationCursor=nullptr);
   virtual void _swap(VirtualCollection& source) override;
   virtual void _remove(const ExtendedSuppressParameters& parameters, VirtualCollectionCursor* cursor=nullptr) override;
   virtual void _removeAll(const ExtendedSuppressParameters& parameters,
         const VirtualCollectionCursor* start=nullptr, const VirtualCollectionCursor* end=nullptr) override;
   virtual void _replace(EnhancedObject* anObject,
         const ExtendedReplaceParameters& parameters, VirtualCollectionCursor* cursor=nullptr) override;
   int queryInternCount(const ExtendedTreeLocateParameters& parameters,
         const Cursor* start, const Cursor* end) const;
   virtual int _queryCount(const ExtendedLocateParameters& elpParameters,
         const VirtualCollectionCursor* start=nullptr, const VirtualCollectionCursor* end=nullptr) const override
      {  const ExtendedTreeLocateParameters& parameters = (const ExtendedTreeLocateParameters&) elpParameters;
         return (!start && !end && !parameters.isSingleLevel())
            ? TypeImplTree::getCount()
            : queryInternCount(parameters, (const Cursor*) start, (const Cursor*) end);
      }
   virtual EnhancedObject* _getElement(const ExtendedLocateParameters& parameters,
         const VirtualCollectionCursor* cursor=nullptr) const override;
   virtual bool _hasSons(const VirtualTreeCursor& cursor) const override;
   virtual bool _canBeFather(const VirtualTreeCursor& cursor) const override;
   virtual int _querySonsCount(const VirtualTreeCursor& cursor) const override;
   const typename TypeImplTree::FlatCollection& getSons(const Cursor& cursor) const;

   bool hasRoot() const { return TypeImplTree::hasRoot(); }
   Node& root() const { return (Node&) TypeImplTree::root(); }
   static void cut(TypeImplCursor& cursor, const TypeImplCursor& source)
      {  cursor.cut(source); }

   friend class TGenericTreeCursor<TypeNode, TypeImplCursor, TypeImplTree>;

  public:
   TGenericTree() {}
   TGenericTree(const thisType& source, AddMode mode=AMNoDuplicate,
         const VirtualCast* retrieveRegistrationFromCopy=nullptr)
      :  VirtualTree(source, mode), TypeImplTree(source)
      {  if (mode == AMDuplicate) {
            if (retrieveRegistrationFromCopy != nullptr)
               VirtualTree::pfullAssign(source,
                  DVirtualCollection::TCastParameters<ExtendedTreeReplaceParameters>()
                  .setExternCast(*retrieveRegistrationFromCopy).setDuplicate().setFreeOnError());
            else
               VirtualTree::pfullAssign(source,
                  ExtendedTreeReplaceParameters().setDuplicate().setFreeOnError());
         };
      }
   thisType& operator=(const thisType& source)
      {  return (thisType&) inherited::operator=(source); }
   Template3DefineCopy(TGenericTree, TypeNode, TypeImplCursor, TypeImplTree)
   DTemplate3DefineAssign(TGenericTree, TypeNode, TypeImplCursor, TypeImplTree)

   #define DefJustDeclare
   #define DefTypeCollection thisType
   #define DefTypeCursor Cursor
   #define DefTypeElement Node
   #include "Collection/ELMTreeCollection.inch"
   #undef DefTypeElement
   #undef DefTypeCursor
   #undef DefTypeCollection
   #undef DefJustDeclare

   virtual bool isValid() const override
      {  return VirtualTree::isValid() && TypeImplTree::isValid(); }
};

/*******************************************************/
/* Definition of the template class TGenericTreeCursor */
/*******************************************************/

template <class TypeNode, class TypeImplCursor, class TypeImplTree>
class TGenericTreeCursor : public VirtualTreeCursor, private TypeImplCursor {
  public:
   typedef VirtualTreeCursor::ValidityMode ValidityMode;

  private:
   typedef TGenericTree<TypeNode, TypeImplCursor, TypeImplTree> TypeTree;
   typedef TGenericTreeCursor<TypeNode, TypeImplCursor, TypeImplTree> thisType;
   typedef thisType ThisTreeCursor;

   class PositionVerification;
   friend class PositionVerification;
   friend class TypeTree::TreeInvalidateNotification;
   friend class TypeTree::TreeMoveNotification;
   friend class TypeTree::TreeBoundInvalidateNotification;
   friend class TypeTree::TreeNormalizationAfterSuppressionNotification;
   friend class TypeTree::RouteFreeAction;
   friend class DTree::GenericTree;

  protected:
   virtual ComparisonResult _compare(const EnhancedObject& asource) const override
      {  ComparisonResult result = VirtualTreeCursor::_compare(asource);
         return (result == CREqual)
            ? TypeImplCursor::compare((const thisType&) castFromCopyHandler(asource))
            : result;
      }

   TypeImplCursor& getImplementation() { return (TypeImplCursor&) *this; }
   const TypeImplCursor& getImplementation() const { return (const TypeImplCursor&) *this; }
   static const TypeImplTree& getImplementation(const TypeTree& tree) { return tree; }
   virtual bool _isPositionned(const ExtendedLocateParameters& pos, const VirtualCollectionCursor* cursor=nullptr) const override;
   virtual bool _position(const Position& pos) override;
   TypeNode& element() const
      {  return (TypeNode&) (TypeImplCursor::isRoot() ? getSupport().root()
            : TypeImplCursor::lastElement());
      }
   virtual EnhancedObject* _getSElement() const override
      {  return TypeNode::castToCopyHandler((TypeNode*) &element()); }
   virtual bool _isEqual(const AbstractCursor& cursor) const override
      {  return VirtualTreeCursor::_isEqual(cursor)
            && TypeImplCursor::isEqual((const thisType&) cursor);
      }
   bool invariant() const
      {  return TypeImplCursor::invariant() && (!TypeImplCursor::isRoot() || getSupport().hasRoot()); }
   virtual ValidityMode _getMode() const override
      {  return (ValidityMode) TypeImplCursor::getMode(); }
   virtual ExtendedComparisonResult _fullCompare(const VirtualTreeCursor& source) const override
      {  return TypeImplCursor::fullCompare((const thisType&) source); }

   static typename TypeImplTree::FlatCollection::Cursor& lastCursor(thisType& cursor)
      {  AssumeCondition((cursor.TypeImplCursor::getMode() == VMInvalidCursor)
            || (cursor.TypeImplCursor::getMode() == VMTree))
         return cursor.TypeImplCursor::lastCursor();
      }
   static const typename TypeImplTree::FlatCollection::Cursor& lastCursor(const thisType& cursor)
      {  AssumeCondition((cursor.TypeImplCursor::getMode() == VMInvalidCursor)
            || (cursor.TypeImplCursor::getMode() == VMTree))
         return cursor.TypeImplCursor::lastCursor();
      }
   static typename TypeImplTree::FlatCollection& lastSupport(const thisType& cursor)
      {  AssumeCondition((cursor.TypeImplCursor::getMode() == VMInvalidCursor)
            || (cursor.TypeImplCursor::getMode() == VMTree))
         return cursor.TypeImplCursor::lastSupport();
      }
   void updateMode()
      {  COL::VirtualTreeCursor::ValidityMode mode = TypeImplCursor::getMode();
         bool isValid = lastCursor(*this).isValid();
         if (isValid && mode == COL::VirtualTreeCursor::VMInvalidCursor)
            TypeImplCursor::setMode(COL::VirtualTreeCursor::VMTree);
         else if (!isValid && mode == COL::VirtualTreeCursor::VMTree)
            TypeImplCursor::setMode(COL::VirtualTreeCursor::VMInvalidCursor);
      }
      
   class CursorPath : public VirtualCursorPath {
     private:
      typedef VirtualCursorPath inherited;
      typename TypeImplCursor::CursorPath icpImplementation;
      
     public:
      CursorPath(const TypeTree& tree, const ThisTreeCursor* fstCursor,
            const ThisTreeCursor* sndCursor)
         :  icpImplementation(getImplementation(tree), (const TypeImplCursor*) fstCursor,
               (const TypeImplCursor*) sndCursor) {}
      CursorPath(const CursorPath& source) = default;
      DefineCopy(CursorPath)
      DDefineAssign(CursorPath)

      virtual bool hasAscent() const override { return icpImplementation.hasAscent(); }
      virtual bool ascent(VirtualTreeCursor& cursor) override
         {  return icpImplementation.ascent((ThisTreeCursor&) cursor); }
      virtual bool up(VirtualTreeCursor& cursor) override
         {  return icpImplementation.up((ThisTreeCursor&) cursor); }
      virtual bool down(VirtualTreeCursor& cursor) override
         {  return icpImplementation.down((ThisTreeCursor&) cursor); }
      virtual bool next(VirtualTreeCursor& cursor) override
         {  return icpImplementation.next((ThisTreeCursor&) cursor); }
      virtual bool hasNext(VirtualTreeCursor& cursor) const override
         {  return icpImplementation.hasNext((ThisTreeCursor&) cursor); }
      virtual bool descent(VirtualTreeCursor& cursor) override
         {  return icpImplementation.descent((ThisTreeCursor&) cursor); }
      virtual bool hasDescent() const override
         {  return icpImplementation.hasDescent(); }
   };
   friend class CursorPath;

   virtual int getBaseDiff(const VirtualTreeCursor* source) const
      {  return TypeImplCursor::getLevel() - (source
            ? ((const thisType&) *source).TypeImplCursor::getLevel() : 0);
      }
   virtual PNT::PassPointer<VirtualCursorPath> newPath(const VirtualTreeCursor* source,
         PathDirection direction) const override
      {  return PNT::PassPointer<VirtualCursorPath>((direction == PDForward)
               ? new CursorPath(getSupport(), this, (const ThisTreeCursor*) source)
               : new CursorPath(getSupport(), (const ThisTreeCursor*) source, this),
            PNT::PassPointer<VirtualCursorPath>::Init());
      }

  public:
   TGenericTreeCursor(const TypeTree& support) : VirtualTreeCursor(support) {}
   TGenericTreeCursor(const thisType& source) = default;
   Template3DefineCopy(TGenericTreeCursor, TypeNode, TypeImplCursor, TypeImplTree)
   DTemplate3DefineAssign(TGenericTreeCursor, TypeNode, TypeImplCursor, TypeImplTree)
   Template3DefineCursorForAbstractCollect(TGenericTree, TGenericTreeCursor, TypeNode, TypeImplCursor, TypeImplTree)
   void swap(thisType& source)
      {  VirtualTreeCursor::swap((VirtualTreeCursor&) source);
         TypeImplCursor::swap((TypeImplCursor&) source);
      }
   virtual bool isValid() const override
      {  return VirtualTreeCursor::isValid() && TypeImplCursor::isValid(); }
   const TypeImplCursor& implementation() const { return *this; }

   #define DefCursor
   #define DefTypeCollection TypeTree
   #define DefTypeCursor thisType
   #define DefTypeElement TypeNode
   #include "Collection/ELMTreeCollection.inch"
   #undef DefTypeElement
   #undef DefTypeCollection
   #undef DefTypeCursor
   #undef DefCursor
};

Template3InlineCollectionForAbstractCollect(TGenericTree, TGenericTreeCursor, TypeNode, TypeImplCursor, TypeImplTree)

#define DefJustInline
#define DefTypeCollection TGenericTree<TypeNode, TypeImplCursor, TypeImplTree>
#define DefTypeCursor TGenericTreeCursor<TypeNode, TypeImplCursor, TypeImplTree>
#define DefTypeElement TypeNode
#define DefTemplate template<class TypeNode, class TypeImplCursor, class TypeImplTree>
#include "Collection/ELMTreeCollection.inch"
#undef DefTemplate
#undef DefTypeElement
#undef DefTypeCursor
#undef DefTypeCollection
#undef DefJustInline

template<class TypeNode, class TypeImplCursor, class TypeImplTree>
inline void
TGenericTree<TypeNode, TypeImplCursor, TypeImplTree>::_addAll(const thisType& source,
      const ExtendedTreeInsertionParameters& parameters, Cursor* cursor,
      const Cursor* startSource, const Cursor* endSource)
   {  VirtualTree::paddAll(source, parameters, cursor, startSource, endSource); }

template<class TypeNode, class TypeImplCursor, class TypeImplTree>
inline void
TGenericTree<TypeNode, TypeImplCursor, TypeImplTree>::_addAll(const VirtualCollection& source,
      const ExtendedInsertionParameters& parameters, VirtualCollectionCursor* cursor,
      const VirtualCollectionCursor* startSource, const VirtualCollectionCursor* endSource) {
   AssumeCondition(dynamic_cast<const thisType*>(&source))
   _addAll((const thisType&) source, (const ExtendedTreeInsertionParameters&) parameters,
      (Cursor*) cursor, (Cursor*) startSource, (Cursor*) endSource);
}

template<class TypeNode, class TypeImplCursor, class TypeImplTree>
inline const typename TypeImplTree::FlatCollection&
TGenericTree<TypeNode, TypeImplCursor, TypeImplTree>::getSons(const Cursor& cursor) const
   { return cursor.elementAt().getSons(); }

namespace DTree {

template <class TypeNode, class TypeImplCursor, class TypeImplTree>
inline typename TypeNode::FlatCollection::Cursor&
GenericTree::lastCursor(TGenericTreeCursor<TypeNode, TypeImplCursor, TypeImplTree>& cursor)
   { return TGenericTreeCursor<TypeNode, TypeImplCursor, TypeImplTree>::lastCursor(cursor); }

template <class TypeNode, class TypeImplCursor, class TypeImplTree>
inline const typename TypeNode::FlatCollection::Cursor&
GenericTree::lastCursor(const TGenericTreeCursor<TypeNode, TypeImplCursor, TypeImplTree>& cursor)
   { return TGenericTreeCursor<TypeNode, TypeImplCursor, TypeImplTree>::lastCursor(cursor); }

template <class TypeNode, class TypeImplCursor, class TypeImplTree>
inline typename TypeNode::FlatCollection&
GenericTree::lastSupport(const TGenericTreeCursor<TypeNode, TypeImplCursor, TypeImplTree>& cursor)
   { return TGenericTreeCursor<TypeNode, TypeImplCursor, TypeImplTree>::lastSupport(cursor); }

} // end of namespace DTree

/*****************************************************************************/
/* Definition of the template class TGenericTree::TreeInvalidateNotification */
/*****************************************************************************/

template<class TypeNode, class TypeImplCursor, class TypeImplTree>
class TGenericTree<TypeNode, TypeImplCursor, TypeImplTree>::TreeInvalidateNotification
      : public CursorNotification {
  private:
   TypeImplCursor ticElement;

  protected:
   void update(Cursor& cursor)
      { TGenericTree<TypeNode, TypeImplCursor, TypeImplTree>::cut((TypeImplCursor&) cursor, ticElement); }

   friend class TGenericTree<TypeNode, TypeImplCursor, TypeImplTree>;
   TypeImplCursor& element() { return ticElement; }

  public:
   TreeInvalidateNotification(const TypeImplTree& tree) : ticElement(tree) {}
   TreeInvalidateNotification(const TreeInvalidateNotification& source) = default;
   DefineCursorNotificationMethods(TreeInvalidateNotification, Cursor)
   DDefineAssign(TreeInvalidateNotification)
};

/**********************************************************************/
/* Definition of the template class GenericTree::TreeMoveNotification */
/**********************************************************************/

template<class TypeNode, class TypeImplCursor, class TypeImplTree>
class TGenericTree<TypeNode, TypeImplCursor, TypeImplTree>::TreeMoveNotification
      : public CursorNotification {
  private:
   TypeImplCursor gitcSuppression;
   typedef TGenericTreeCursor<TypeNode, TypeImplCursor, TypeImplTree> Cursor;
   typedef typename TypeImplCursor::StackPrefix StackPrefix;

   class TreeCursorPrefixRegistration : public ImplListElement, public StackPrefix {
     public:
      TreeCursorPrefixRegistration(const StackPrefix& prefix) : StackPrefix(prefix) {}
      TreeCursorPrefixRegistration(const TreeCursorPrefixRegistration& source) = default;
      DefineCopy(TreeCursorPrefixRegistration)
      DDefineAssign(TreeCursorPrefixRegistration)
   };

   TList<TreeCursorPrefixRegistration> ltcprMovementCursors;

  protected:
   void update(Cursor& cursor)
      {  StackPrefix stackPrefix(cursor);
         if (stackPrefix.hasPrefix(gitcSuppression))
            ltcprMovementCursors.add(new TreeCursorPrefixRegistration(stackPrefix));
      }

  public:
   TreeMoveNotification() {}
   TreeMoveNotification(const TreeMoveNotification& source)
      :  CursorNotification(source), gitcSuppression(source.gitcSuppression) {}
   TreeMoveNotification& operator=(const TreeMoveNotification& source)
      {  CursorNotification::operator=(source);
         gitcSuppression = source.gitcSuppression;
         return *this;
      }
   virtual ~TreeMoveNotification() { ltcprMovementCursors.freeAll(); }
   DefineCursorNotificationMethods(TreeMoveNotification, Cursor)
   DDefineAssign(TreeMoveNotification)

   TypeImplCursor& suppression() { return gitcSuppression; }
   void notifyInvalidation()
      {  ltcprMovementCursors.foreachSDo([](TreeCursorPrefixRegistration& prefixRegistration)
            {  prefixRegistration.cut(); return true; });
         ltcprMovementCursors.freeAll();
      }
   void notifyPerformedMove(const TypeImplCursor& newPrefix)
      {  ltcprMovementCursors.foreachSDo([&newPrefix](TreeCursorPrefixRegistration& prefixRegistration)
            {  prefixRegistration.replacePrefix(newPrefix); return true; });
         ltcprMovementCursors.freeAll();
      }
};

template<class TypeNode, class TypeImplCursor, class TypeImplTree>
class TGenericTree<TypeNode, TypeImplCursor, TypeImplTree>::TreeBoundInvalidateNotification
      : public CursorNotification {
  private:
   TypeImplCursor ticParent;
   typedef typename TypeImplTree::FlatCollection FlatCollection;
   typedef typename FlatCollection::Cursor FlatCursor;
   typedef typename FlatCollection::PPCursor PPFlatCursor;
   typedef TGenericTreeCursor<TypeNode, TypeImplCursor, TypeImplTree> Cursor;

   PPFlatCursor ppfcStart, ppfcEnd;

  protected:
   void update(Cursor& gtcCursor)
      {  TypeImplCursor& cursor = (TypeImplCursor&) gtcCursor;
         typename TypeImplCursor::StackPrefix prefixCursor(cursor);
         if (contain(cursor, prefixCursor)) {
            prefixCursor.advance();
            prefixCursor.cut();
         };
      }

   friend class TGenericTree<TypeNode, TypeImplCursor, TypeImplTree>;
   TypeImplCursor& parent() { return ticParent; }
   PPFlatCursor& start() { return ppfcStart; }
   PPFlatCursor& end() { return ppfcEnd; }

  public:
   TreeBoundInvalidateNotification() {}
   TreeBoundInvalidateNotification(const TreeBoundInvalidateNotification& source)
      :  CursorNotification(source), ticParent(source.ticParent),
         ppfcStart(source.ppfcStart, PNT::Pointer::Duplicate()),
         ppfcEnd(source.ppfcEnd, PNT::Pointer::Duplicate()) {}
   DefineCursorNotificationMethods(TreeBoundInvalidateNotification, Cursor)
   DDefineAssign(TreeBoundInvalidateNotification)

   bool contain(const TypeImplCursor& cursor, typename TypeImplCursor::StackPrefix& prefixCursor)
      {  return prefixCursor.hasPrefix(ticParent) && !prefixCursor.isFull()
            && (!ppfcStart.isValid() || (cursor.lastCursor() >= *ppfcStart))
            && (!ppfcEnd.isValid() || (cursor.lastCursor() <= *ppfcEnd));
      }
   int countElement(const TypeImplTree& tree) const
      {  int result = 0;
         PPFlatCursor cursor = tree.getSElement(&ticParent).getSons().newCursor();
         if (ppfcStart.isValid())
            *cursor = *ppfcStart;
         while (cursor->isValid() && (!ppfcEnd.isValid() || !cursor->isEqual(*ppfcEnd)))
            result += TGenericTree<TypeNode, TypeImplCursor, TypeImplTree>::queryNumberOfElements(
               HandlerSonsAndSubTreeCast::castFrom(&cursor->elementAt()));
         return result;
      }
};

template<class TypeNode, class TypeImplCursor, class TypeImplTree>
class TGenericTree<TypeNode, TypeImplCursor, TypeImplTree>::TreeNormalizationAfterSuppressionNotification
      : public CursorNotification {
  private:
   bool fHasRoot;
   typedef TGenericTreeCursor<TypeNode, TypeImplCursor, TypeImplTree> Cursor;

  protected:
   void update(Cursor& cursor)
      { ((TypeImplCursor&) cursor).normalizeAfterEndSuppression(fHasRoot); }

  public:
   TreeNormalizationAfterSuppressionNotification(const TypeImplTree& tree) : fHasRoot(tree.hasRoot()) {}
   TreeNormalizationAfterSuppressionNotification(const TreeNormalizationAfterSuppressionNotification& source) = default;
   DefineCursorNotificationMethods(TreeNormalizationAfterSuppressionNotification, Cursor)
   DDefineAssign(TreeNormalizationAfterSuppressionNotification)
};

/*****************************************************************/
/* Definition of the template class GenericTree::RouteFreeAction */
/*****************************************************************/

template<class TypeNode, class TypeImplCursor, class TypeImplTree>
class TGenericTree<TypeNode, TypeImplCursor, TypeImplTree>::RouteFreeAction
      : public VirtualTree::RouteFreeAction {
  private:
   bool fOnlySons;

  public:
   RouteFreeAction(TGenericTree<TypeNode, TypeImplCursor, TypeImplTree>& tree, bool onlySons)
      : VirtualTree::RouteFreeAction(tree), fOnlySons(onlySons) {}

   bool end(const VirtualTree& tree, const VirtualTreeCursor& cursorRoot)
      {  return (!fOnlySons) ? VirtualTree::RouteFreeAction::end(tree, cursorRoot) : true; }
};

/********************************************************/
/* Definition of the class GenericTree::KeySortedAccess */
/********************************************************/
// Components to access to sorted sub-collections.
//

namespace DTree {

template <class TypeTree, class TemplateCollectionElementKeyCast>
class GenericTree::KeySortedAccess {
  private:
   typedef VirtualTree::AddMode AddMode;
   static const AddMode AMNoDuplicate = VirtualTree::AMNoDuplicate;
   typedef VirtualTree::RemoveMode RemoveMode;
   static const RemoveMode RMRemove = VirtualTree::RMRemove;
   typedef VirtualTree::RelativePosition RelativePosition;
   static const RelativePosition RPExact = VirtualTree::RPExact;
   typedef typename TypeTree::Cursor TypeTreeCursor;
   typedef typename TypeTree::Node TypeTreeNode;

  private:
   typedef typename TemplateCollectionElementKeyCast::TypeCollection TypeSortedCollection;
   typedef typename TemplateCollectionElementKeyCast::TypeElement    TypeSortedElement;
   typedef typename TemplateCollectionElementKeyCast::TypeKey        TypeKey;
   typedef typename TemplateCollectionElementKeyCast::TypeCast       TypeSortedTreeCast;
   typedef typename TypeSortedCollection::LocationResult             LocationResult;

  protected:
   const TypeTree& getTree(const TypeTreeCursor& cursor) const { return cursor.getSupport(); }
   TypeTree& getTree(const TypeTreeCursor& cursor)
      {  return const_cast<TypeTree&>(cursor.getSupport()); }
   static typename TypeSortedCollection::Cursor& lastCursor(TypeTreeCursor& cursor)
      {  return (typename TypeSortedCollection::Cursor&) GenericTree::lastCursor(cursor); }
   static const typename TypeSortedCollection::Cursor& lastCursor(const TypeTreeCursor& cursor)
      {  return (const typename TypeSortedCollection::Cursor&) GenericTree::lastCursor(cursor); }
   static TypeSortedCollection& lastSupport(const TypeTreeCursor& cursor)
      {  return (TypeSortedCollection&) GenericTree::lastSupport(cursor); }

  public:
   KeySortedAccess() {}

   #define DefTypeElement TypeSortedElement
   #include "Collection/ConcreteCollection/TreeCommonFlatAccess.inch"
   #undef DefTypeElement
};

template <class TypeTree, class TemplateCollectionElementKeyCast>
class GenericTree::KeyMapSortedAccess {
  private:
   typedef VirtualTree::AddMode AddMode;
   static const AddMode AMNoDuplicate = VirtualTree::AMNoDuplicate;
   typedef VirtualTree::RemoveMode RemoveMode;
   static const RemoveMode RMRemove = VirtualTree::RMRemove;
   typedef VirtualTree::RelativePosition RelativePosition;
   static const RelativePosition RPExact = VirtualTree::RPExact;
   typedef typename TypeTree::Cursor TypeTreeCursor;
   typedef typename TypeTree::Node TypeTreeNode;

  private:
   typedef typename TemplateCollectionElementKeyCast::TypeCollection TypeSortedCollection;
   typedef typename TemplateCollectionElementKeyCast::TypeElement    TypeSortedElement;
   typedef typename TemplateCollectionElementKeyCast::TypeKey        TypeKey;
   typedef typename TemplateCollectionElementKeyCast::TypeCast       TypeSortedTreeCast;
   typedef typename TypeSortedCollection::LocationResult             LocationResult;

  protected:
   const TypeTree& getTree(const TypeTreeCursor& cursor) const { return cursor.getSupport(); }
   TypeTree& getTree(const TypeTreeCursor& cursor)
      {  return const_cast<TypeTree&>(cursor.getSupport()); }
   static typename TypeSortedCollection::Cursor& lastCursor(TypeTreeCursor& cursor)
      {  return (typename TypeSortedCollection::Cursor&) GenericTree::lastCursor(cursor); }
   static const typename TypeSortedCollection::Cursor& lastCursor(const TypeTreeCursor& cursor)
      {  return (const typename TypeSortedCollection::Cursor&) GenericTree::lastCursor(cursor); }
   static TypeSortedCollection& lastSupport(const TypeTreeCursor& cursor)
      {  return (TypeSortedCollection&) GenericTree::lastSupport(cursor); }

  public:
   KeyMapSortedAccess() {}

   #define DefTypeElement TypeSortedElement
   #define DefMapAccess
   #include "Collection/ConcreteCollection/TreeCommonFlatAccess.inch"
   #undef DefMapAccess
   #undef DefTypeElement
};

template <class TypeTree, class TemplateCollectionElementKeyCast>
class GenericTree::KeyMultipleSortedAccess {
  private:
   typedef VirtualTree::AddMode AddMode;
   static const AddMode AMNoDuplicate = VirtualTree::AMNoDuplicate;
   typedef VirtualTree::RemoveMode RemoveMode;
   static const RemoveMode RMRemove = VirtualTree::RMRemove;
   typedef VirtualTree::RelativePosition RelativePosition;
   static const RelativePosition RPExact = VirtualTree::RPExact;
   typedef typename TypeTree::Cursor TypeTreeCursor;
   typedef typename TypeTree::Node TypeTreeNode;

  private:
   typedef typename TemplateCollectionElementKeyCast::TypeCollection TypeSortedCollection;
   typedef typename TemplateCollectionElementKeyCast::TypeElement    TypeSortedElement;
   typedef typename TemplateCollectionElementKeyCast::TypeKey        TypeKey;
   typedef typename TemplateCollectionElementKeyCast::TypeCast       TypeSortedTreeCast;
   typedef typename TypeSortedCollection::LocationResult             LocationResult;
   typedef typename TypeSortedCollection::Cursor                     TypeSortedCollectionCursor;
   typedef typename TypeSortedCollection::PPCursor                   PPTypeSortedCollectionCursor;
   typedef typename TypeSortedCollection::CommonElement              CommonElement;
   typedef typename TypeSortedCollection::UniqueElement              UniqueElement;
   typedef typename TypeSortedCollection::SingleElement              SingleElement;
   typedef typename TypeSortedCollection::MultipleElement            MultipleElement;

  protected:
   const TypeTree& getTree(const TypeTreeCursor& cursor) const { return cursor.getSupport(); }
   TypeTree& getTree(const TypeTreeCursor& cursor)
      {  return const_cast<TypeTree&>(cursor.getSupport()); }
   static typename TypeSortedCollection::Cursor& lastCursor(TypeTreeCursor& cursor)
      {  return (typename TypeSortedCollection::Cursor&) GenericTree::lastCursor(cursor); }
   static const typename TypeSortedCollection::Cursor& lastCursor(const TypeTreeCursor& cursor)
      {  return (const typename TypeSortedCollection::Cursor&) GenericTree::lastCursor(cursor); }
   static TypeSortedCollection& lastSupport(const TypeTreeCursor& cursor)
      {  return (TypeSortedCollection&) GenericTree::lastSupport(cursor); }

  public:
   KeyMultipleSortedAccess() {}

   #define DefTypeElement CommonElement
   #include "Collection/ConcreteCollection/TreeCommonFlatAccess.inch"
   #undef DefTypeElement

   #define DefBaseMultipleInterface
   #define DefTypeElement UniqueElement
   #include "Collection/ConcreteCollection/TreeCommonFlatAccess.inch"
   #undef DefTypeElement

   #define DefMapAccess
   #define DefTypeElement SingleElement
   #include "Collection/ConcreteCollection/TreeCommonFlatAccess.inch"
   #undef DefTypeElement
   #define DefTypeElement MultipleElement
   #include "Collection/ConcreteCollection/TreeCommonFlatAccess.inch"
   #undef DefTypeElement
   #undef DefMapAccess
   #undef DefBaseMultipleInterface
};

} // end of namespace DTree

} // end of namespace COL

#endif // COL_TreeCommonH
