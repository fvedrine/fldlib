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
// Fichier     : VirtualTree.h
// Description :
//   Definition of the class VirtualTree.
//

#ifndef COL_VirtualTreeH
#define COL_VirtualTreeH

#include "Collection/VirtualCollection/VirtualCollection.h"

namespace COL {

/***************************************/
/* Definition of the class VirtualTree */
/***************************************/

namespace DVirtualTree {

class Definitions {
  public:
   enum ValidityMode { VMUpRoot=0, VMTree=1, VMDownCursor=2, VMInvalidCursor=3 };
   typedef DVirtualTree::ExtendedTreeLocateParameters ExtendedTreeLocateParameters;
   typedef DVirtualTree::ExtendedTreeInsertionParameters ExtendedTreeInsertionParameters;
   typedef DVirtualTree::ExtendedTreeSuppressParameters ExtendedTreeSuppressParameters;
   typedef DVirtualTree::ExtendedTreeReplaceParameters ExtendedTreeReplaceParameters;
};

} // end of namespace DVirtualTree

class VirtualTreeCursor;
class VirtualTree : public VirtualCollection, public DVirtualTree::Definitions {
  public:
   class CollectionAccess;

  protected:
   friend class CollectionAccess;
   friend class VirtualTreeCursor;

  protected:
   class RouteFreeAction;
   class RouteCopyAction;
   class RouteLevelCopyAction;
   class CountAction;

   VirtualTree() {}
   VirtualTree(const VirtualTree& source, AddMode mode=AMNoDuplicate)
      :  VirtualCollection(source, mode) {}

   void pfullAssign(const VirtualTree& source, const ExtendedTreeReplaceParameters& parameters);
   void paddAll(const VirtualTree& source, const ExtendedTreeInsertionParameters& parameters,
         VirtualTreeCursor* cursor, const VirtualTreeCursor* startSource,
         const VirtualTreeCursor* endSource);

   virtual bool _hasSons(const VirtualTreeCursor& cursor) const { return false; }
   virtual bool _canBeFather(const VirtualTreeCursor& cursor) const { return false; }
   virtual int _querySonsCount(const VirtualTreeCursor& cursor) const { return 0; }
   int pqueryCount(const ExtendedTreeLocateParameters& parameters,
         const VirtualTreeCursor* start=nullptr, const VirtualTreeCursor* end=nullptr) const;

  public:
   class VirtualAction {
     public:
      typedef VirtualTreeCursor Cursor;
      virtual ~VirtualAction() {}
      virtual bool init(const VirtualTree& tree, const Cursor& cursorRoot) = 0;
      virtual bool ascent(const Cursor& cursor, EnhancedObject& son) = 0;
      virtual bool descent(const Cursor& cursor, EnhancedObject& father) = 0;
      virtual bool next(const Cursor& cursor, EnhancedObject& brother) = 0;
      virtual bool end(const VirtualTree& tree, const Cursor& cursorRoot) = 0;
   };

  protected:
   template <class Action, class TemplateParameters>
      bool foreachSonLevelSDo(TemplateParameters traits, Action& action,
         const ExtendedTreeLocateParameters& parameters,
         const typename TemplateParameters::Tree::Cursor* start,
         const typename TemplateParameters::Tree::Cursor* end) const;
   template <class Action, class TemplateParameters>
      bool foreachBrotherLevelSDo(TemplateParameters traits, Action& action,
         const ExtendedTreeLocateParameters& parameters,
         const typename TemplateParameters::Tree::Cursor* start,
         const typename TemplateParameters::Tree::Cursor* end) const;
   template <class Action, class TemplateParameters>
      bool foreachMultiSonLevelSDo(TemplateParameters traits, Action& action,
         const ExtendedTreeLocateParameters& parameters,
         const typename TemplateParameters::Tree::Cursor* start,
         const typename TemplateParameters::Tree::Cursor* end) const;
   template <class Action, class TemplateParameters>
      bool foreachMultiBrotherLevelSDo(TemplateParameters traits, Action& action,
         const ExtendedTreeLocateParameters& parameters,
         const typename TemplateParameters::Tree::Cursor* start,
         const typename TemplateParameters::Tree::Cursor* end) const;
   template <class Action, class TemplateParameters>
      bool foreachInnerSonsLevelSDo(TemplateParameters traits, Action& action,
         const ExtendedTreeLocateParameters& parameters,
         const typename TemplateParameters::Tree::Cursor* start,
         const typename TemplateParameters::Tree::Cursor* end) const;
   template <class Action, class TemplateParameters>
      bool foreachBrothersSDo(TemplateParameters traits, Action& action,
         const ExtendedTreeLocateParameters& parameters,
         const typename TemplateParameters::Tree::Cursor* start,
         const typename TemplateParameters::Tree::Cursor* end) const;
   template <class Action, class TemplateParameters>
      bool foreachSonsSDo(TemplateParameters traits, Action& action,
         const ExtendedTreeLocateParameters& parameters,
         const typename TemplateParameters::Tree::Cursor* start,
         const typename TemplateParameters::Tree::Cursor* end) const;

   template <class Action, class TemplateParameters>
      bool foreachSonLevelSReverseDo(TemplateParameters traits, Action& action,
         const ExtendedTreeLocateParameters& parameters,
         const typename TemplateParameters::Tree::Cursor* start,
         const typename TemplateParameters::Tree::Cursor* end) const;
   template <class Action, class TemplateParameters>
      bool foreachBrotherLevelSReverseDo(TemplateParameters traits, Action& action,
         const ExtendedTreeLocateParameters& parameters,
         const typename TemplateParameters::Tree::Cursor* start,
         const typename TemplateParameters::Tree::Cursor* end) const;
   template <class Action, class TemplateParameters>
      bool foreachMultiSonLevelSReverseDo(TemplateParameters traits, Action& action,
         const ExtendedTreeLocateParameters& parameters,
         const typename TemplateParameters::Tree::Cursor* start,
         const typename TemplateParameters::Tree::Cursor* end) const;
   template <class Action, class TemplateParameters>
      bool foreachMultiBrotherLevelSReverseDo(TemplateParameters traits, Action& action,
         const ExtendedTreeLocateParameters& parameters,
         const typename TemplateParameters::Tree::Cursor* start,
         const typename TemplateParameters::Tree::Cursor* end) const;
   template <class Action, class TemplateParameters>
      bool foreachInnerSonsLevelSReverseDo(TemplateParameters traits, Action& action,
         const ExtendedTreeLocateParameters& parameters,
         const typename TemplateParameters::Tree::Cursor* start,
         const typename TemplateParameters::Tree::Cursor* end) const;
   template <class Action, class TemplateParameters>
      bool foreachBrothersSReverseDo(TemplateParameters traits, Action& action,
         const ExtendedTreeLocateParameters& parameters,
         const typename TemplateParameters::Tree::Cursor* start,
         const typename TemplateParameters::Tree::Cursor* end) const;
   template <class Action, class TemplateParameters>
      bool foreachSonsSReverseDo(TemplateParameters traits, Action& action,
         const ExtendedTreeLocateParameters& parameters,
         const typename TemplateParameters::Tree::Cursor* start,
         const typename TemplateParameters::Tree::Cursor* end) const;

  public:
   DefineCopy(VirtualTree)
   DefineCollectionForAbstractCollect(VirtualTree, VirtualTreeCursor)

#define DefTypeCollection VirtualTree
#define DefTypeCursor VirtualTreeCursor
#define DefJustDeclare
   #include "Collection/ELMTreeCollection.inch"
#undef DefJustDeclare
#undef DefTypeCollection
#undef DefTypeCursor

   template <class Action, class TemplateParameters>
      bool tforeachSDo(TemplateParameters traits, Action& action,
         const ExtendedTreeLocateParameters& parameters,
         const typename TemplateParameters::Tree::Cursor* start=nullptr,
         const typename TemplateParameters::Tree::Cursor* end=nullptr) const;
   template <class Action, class TemplateParameters>
      bool tforeachSDo(TemplateParameters traits, Action& action) const;
   template <class Action, class TemplateParameters>
      bool tforeachSSubnodeDo(TemplateParameters traits, Action& action,
         const typename TemplateParameters::Tree::Cursor& cursor) const;
   template <class Action, class TemplateParameters>
      bool tforeachSReverseDo(TemplateParameters traits, Action& action,
         const ExtendedTreeLocateParameters& parameters,
         const typename TemplateParameters::Tree::Cursor* start=nullptr,
         const typename TemplateParameters::Tree::Cursor* end=nullptr) const;
   template <class Action, class TemplateParameters>
      bool tforeachSReverseDo(TemplateParameters traits, Action& action) const;
   template <class Action, class TemplateParameters>
      bool tforeachSSubnodeReverseDo(TemplateParameters traits, Action& action,
         const typename TemplateParameters::Tree::Cursor& cursor) const;

   class ForeachTemplateParameters {
     public:
      typedef VirtualTree Tree;
      typedef EnhancedObject Element;
      typedef SimpleCast Cast;
   };

   template <class Action>
      bool tforeachDo(Action& action, const ExtendedTreeLocateParameters& parameters,
         const VirtualTreeCursor* start=nullptr, const VirtualTreeCursor* end=nullptr) const
      {  return foreachSDo(ForeachTemplateParameters(), action, parameters, start, end); }
   template <class Action>
      bool tforeachSubnodeDo(Action& action, const VirtualTreeCursor& cursor) const
      {  return foreachSSubnodeDo(ForeachTemplateParameters(), action, cursor); }

   bool foreachDo(VirtualAction& action, const ExtendedTreeLocateParameters& parameters,
         const Cursor* start=nullptr, const Cursor* end=nullptr) const
      {  return tforeachSDo(ForeachTemplateParameters(), action, parameters, start, end); }
   bool foreachDo(VirtualAction& action) const
      {  return tforeachSDo(ForeachTemplateParameters(), action); }
   bool foreachSubnodeDo(VirtualAction& action, Cursor& cursor) const
      {  return tforeachSSubnodeDo(ForeachTemplateParameters(), action, cursor); }

   template <class Action>
      bool tforeachReverseDo(Action& action, const ExtendedTreeLocateParameters& parameters,
         const VirtualTreeCursor* start=nullptr, const VirtualTreeCursor* end=nullptr) const
      {  return foreachSReverseDo(ForeachTemplateParameters(), action, parameters, start, end); }
   template <class Action>
      bool tforeachSubnodeReverseDo(Action& action, const VirtualTreeCursor& cursor) const
      {  return foreachSSubnodeReverseDo(ForeachTemplateParameters(), action, cursor); }

   bool foreachReverseDo(VirtualAction& action, const ExtendedTreeLocateParameters& parameters,
         const Cursor* start=nullptr, const Cursor* end=nullptr) const
      {  return tforeachSReverseDo(ForeachTemplateParameters(), action, parameters, start, end); }
   bool foreachReverseDo(VirtualAction& action) const
      {  return tforeachSReverseDo(ForeachTemplateParameters(), action); }
   bool foreachSubnodeReverseDo(VirtualAction& action, Cursor& cursor) const
      { return tforeachSSubnodeReverseDo(ForeachTemplateParameters(), action, cursor); }

   template <class Execute, class TemplateParameters>
   class SimpleBottomUpAction;

   template <class Execute, class TemplateParameters>
   class SimpleTopDownAction;
};

class VirtualTreeCursor : public VirtualCollectionCursor, public DVirtualTree::Definitions {
  public:
   class CursorAccess;
   friend class CursorAccess;

   enum Route { RBottomUp, RTopDown };

   class ExtendedComparisonResult {
     public:
      enum ExtendedResult { ERUndefined, ERLess, ERSmaller, EREqual, ERBigger, ERGreater };

     private:
      ExtendedResult erResult;

     public:
      ExtendedComparisonResult() : erResult(ERUndefined) {}
      ExtendedComparisonResult(const ExtendedComparisonResult& source) = default;
      ExtendedComparisonResult& operator=(const ExtendedComparisonResult& ecrSource) = default;
      ExtendedComparisonResult& setResult(ComparisonResult result)
         {  erResult = (result == CRLess) ? ERLess
               : ((result == CRGreater) ? ERGreater
               : ((result == CREqual) ? EREqual : ERUndefined));
            return *this;
         }
      ExtendedComparisonResult& setSmaller() { erResult = ERSmaller; return *this; }
      ExtendedComparisonResult& setBigger() { erResult = ERBigger; return *this; }
      ExtendedComparisonResult& setEqual() { erResult = EREqual; return *this; }
      const ExtendedResult& getResult() const
         {  AssumeCondition(erResult != ERUndefined) return erResult; }
      bool isValid() const { return erResult != ERUndefined; }
      bool isEqual() const { return erResult == EREqual; }
      bool isSmaller() const { return erResult == ERSmaller; }
      bool isBigger() const { return erResult == ERBigger; }
      ComparisonResult getBasicResult() const
         {  AssumeCondition(erResult != ERUndefined)
            return ((erResult == ERLess) || (erResult == ERSmaller)) ? CRLess
                : (((erResult == ERGreater) || (erResult == ERBigger)) ? CRGreater
                : (((erResult == EREqual) ? CREqual
                : CRNonComparable)));
         }
   };

  private:
   class NoRouteAction {
     public:
      NoRouteAction() {}
      bool init(const VirtualTree& tree, const VirtualTreeCursor& rootCursor) { return true; }
      bool descent(const VirtualTreeCursor& cursor, EnhancedObject& father) { return true; }
      bool ascent(const VirtualTreeCursor& cursor, EnhancedObject& son) { return true; }
      bool next(const VirtualTreeCursor& cursor, EnhancedObject& brother) { return true; }
      bool end(const VirtualTree& tree, const VirtualTreeCursor& rootCursor) { return true; }
   };

  protected:
   class VirtualCursorPath : public EnhancedObject {
     private:
      typedef EnhancedObject inherited;

     public:
      VirtualCursorPath() {}
      VirtualCursorPath(const VirtualCursorPath& source) : inherited(source) {}
      virtual ~VirtualCursorPath() {}

      virtual bool hasAscent() const = 0;
      virtual bool ascent(VirtualTreeCursor& cursor) = 0;
      virtual bool up(VirtualTreeCursor& cursor) = 0;
      virtual bool down(VirtualTreeCursor& cursor) = 0;
      virtual bool next(VirtualTreeCursor& cursor) = 0;
      virtual bool hasNext(VirtualTreeCursor& cursor) const = 0;
      virtual bool descent(VirtualTreeCursor& cursor) = 0;
      virtual bool hasDescent() const = 0;
   };
   
   virtual int queryBaseDiff(const VirtualTreeCursor* cursor) const { AssumeUncalled return 0; }
   enum PathDirection { PDForward, PDBackward };
   virtual PNT::PassPointer<VirtualCursorPath> newPath(const VirtualTreeCursor* source,
         PathDirection direction=PDForward) const { AssumeUncalled return  PNT::PassPointer<VirtualCursorPath>(); }

   virtual ValidityMode _getMode() const { return VMUpRoot; }
   virtual ExtendedComparisonResult _fullCompare(const VirtualTreeCursor& source) const
      { return ExtendedComparisonResult().setEqual(); }

   VirtualTreeCursor(const VirtualTreeCursor& source) : VirtualCollectionCursor(source) {}
   VirtualTreeCursor(const VirtualTree& support) : VirtualCollectionCursor(support) {}

  public:
   class TreePosition : public Position {
     public:
      enum DescentMode { DMBrother, DMSon, DMFather, DMRoot };

     protected:
      DefineExtendedParameters(2, Position)

     public:
      TreePosition() {}
      TreePosition(Direction dir, DescentMode mode=DMBrother) : Position(dir)
         {  mergeOwnField(mode); }
      TreePosition(const TreePosition& source) : Position(source) {}
      DefineCopy(TreePosition)

      TreePosition& setDirection(Direction dir) { return (TreePosition&) Position::setDirection(dir); }
      TreePosition& setDescentMode(DescentMode mode) { setOwnField(mode); return *this; }

      TreePosition& setInvalid() { return (TreePosition&) Position::setInvalid(); }
      TreePosition& setDirection(VirtualCollection::RelativePosition pos)
         {  return (TreePosition&) Position::setDirection(pos); }
      TreePosition& setStart() { return (TreePosition&) Position::setStart(); }
      TreePosition& setEnd() { return (TreePosition&) Position::setEnd(); }
      TreePosition& setBackward() { return (TreePosition&) Position::setBackward(); }
      TreePosition& setForward() { return (TreePosition&) Position::setForward(); }
      TreePosition& setAny() { return (TreePosition&) Position::setAny(); }

      TreePosition& setBrother() { setOwnField(DMBrother); return *this; }
      TreePosition& setSon() { setOwnField(DMSon); return *this; }
      TreePosition& setFather() { setOwnField(DMFather); return *this; }
      TreePosition& setRoot() { setOwnField(DMRoot); return *this; }

      DescentMode getDescentMode() const { return (DescentMode) queryOwnField(); }
      bool isBrother() const { return getDescentMode() == DMBrother; }
      bool isSon() const { return getDescentMode() == DMSon; }
      bool isFather() const { return getDescentMode() == DMFather; }
      bool isRoot() const { return getDescentMode() == DMRoot; }
      bool isAscendant() const { return getDescentMode() & DMFather; }
   };

   DefineCopy(VirtualTreeCursor)
   DefineCursorForAbstractCollect(VirtualTree, VirtualTreeCursor)

#define DefCursor
#define DefTypeCollection VirtualTree
#define DefTypeCursor VirtualTreeCursor
   #include "Collection/ELMTreeCollection.inch"
#undef DefTypeCollection
#undef DefTypeCursor
#undef DefCursor

   bool setToInnerFirstSon();
   bool setToInnerLastSon();

   bool setToFirst(Route route)
      {  NoRouteAction action;
         return tsetToFirst<NoRouteAction>(route, action);
      }
   bool setToLast(Route route)
      {  NoRouteAction action;
         return tsetToLast<NoRouteAction>(route, action);
      }
   bool setToNext(Route route)
      {  NoRouteAction action;
         return tsetToNext<NoRouteAction>(route, action);
      }
   bool setToPrevious(Route route)
      {  NoRouteAction action;
         return tsetToPrevious<NoRouteAction>(route, action);
      }

   template <class Action> bool tsetToFirst(Route route, Action& action);
   template <class Action> bool tsetToLast(Route route, Action& action);
   template <class Action> bool tsetToNext(Route route, Action& action);
   template <class Action> bool tsetToPrevious(Route route, Action& action);
};

InlineCollectionForAbstractCollect(VirtualTree, VirtualTreeCursor)

template <class Execute, class TemplateParameters>
class VirtualTree::SimpleBottomUpAction {
  private:
   Execute& eFun;
   typedef typename TemplateParameters::Element Element;
   typedef typename TemplateParameters::Cast Cast;

  public:
   SimpleBottomUpAction(Execute& fun) : eFun(fun) {}

   bool init(const VirtualTree& tree, const Cursor& cursorRoot) { return true; }
   bool ascent(const Cursor& cursor, EnhancedObject& son)
      {  return eFun((Element&) Cast::castFrom(son)); }
   bool descent(const Cursor& cursor, EnhancedObject& father) { return true; }
   bool next(const Cursor& cursor, EnhancedObject& brother)
      {  return eFun((Element&) Cast::castFrom(brother)); }
   bool end(const VirtualTree& tree, const Cursor& cursorRoot)
      {  return eFun((Element&) Cast::castFrom(cursorRoot.elementSAt())); }
};

template <class Execute, class TemplateParameters>
class VirtualTree::SimpleTopDownAction {
  private:
   Execute& eFun;
   typedef typename TemplateParameters::Element Element;
   typedef typename TemplateParameters::Cast Cast;

  public:
   SimpleTopDownAction(Execute& fun) : eFun(fun) {}

   bool init(const VirtualTree& tree, const Cursor& cursorRoot)
      {  return eFun((Element&) Cast::castFrom(cursorRoot.elementSAt())); }
   bool ascent(const Cursor& cursor, EnhancedObject& son) {  return true; }
   bool descent(const Cursor& cursor, EnhancedObject& father)
      {  return eFun((Element&) Cast::castFrom(cursor.elementSAt())); }
   bool next(const Cursor& cursor, EnhancedObject& brother)
      {  return eFun((Element&) Cast::castFrom(cursor.elementSAt())); }
   bool end(const VirtualTree& tree, const Cursor& cursorRoot) {  return true; }
};
   
class VirtualTree::RouteFreeAction : public VirtualAction {
  private:
   VirtualTree& vtTree;

  public:
   RouteFreeAction(VirtualTree& tree) : vtTree(tree) {}

   virtual bool init(const VirtualTree&, const Cursor& cursorRoot) { return true; }
   virtual bool ascent(const Cursor& cursor, EnhancedObject& son)
      {  vtTree.freeLastSon(const_cast<Cursor&>(cursor)); return true; }
   virtual bool descent(const Cursor& cursor, EnhancedObject& father) { return true; }
   virtual bool next(const Cursor& cursor, EnhancedObject& brother)
      {  vtTree.freeBefore(const_cast<Cursor&>(cursor)); return true; }
   virtual bool end(const VirtualTree&, const Cursor& cursorRoot)
      {  vtTree.freeAt(const_cast<Cursor&>(cursorRoot)); return true; }
};

class VirtualTree::RouteCopyAction : public VirtualAction {
  private:
   VirtualTree&          vtThisTree;
   VirtualTree::PPCursor ppcThisCursor;
   bool fOnlySons;

   class BasicElement;
   class DescentHierarchy;

  protected:
   VirtualTree::Cursor& thisCursor() { return *ppcThisCursor; }
   VirtualTree& thisTree() { return vtThisTree; }

  public:
   RouteCopyAction(VirtualTree& thisTree, bool onlyOnSons)
      :  vtThisTree(thisTree), ppcThisCursor(thisTree.newCursor()), fOnlySons(onlyOnSons) {}

   virtual bool init(const VirtualTree& tree, const Cursor& sourceCursorRoot)
      {  if (!fOnlySons)
            vtThisTree.insertCopyRoot(sourceCursorRoot.elementAt());
         ppcThisCursor->setToRoot();
         return true;
      }
   virtual bool ascent(const Cursor& sourceCursor, EnhancedObject& son)
      {  ppcThisCursor->setToFather(); return true; }
   virtual bool descent(const Cursor& sourceCursor, EnhancedObject& father)
      {  vtThisTree.insertCopyAsFirstSon(*ppcThisCursor, sourceCursor.elementAt());
         return true;
      }
   virtual bool next(const Cursor& sourceCursor, EnhancedObject& brother)
      {  vtThisTree.insertCopyAsNext(*ppcThisCursor, sourceCursor.elementAt());
         return true;
      }
   virtual bool end(const VirtualTree& tree, const Cursor& sourceCursorRoot) { return true; }

   void descent() { ppcThisCursor->setToInvalidSon(); }
   void descent(const EnhancedObject& son)
      {  vtThisTree.insertCopyAsFirstSon(*ppcThisCursor, son); }
   void descentTo(const Cursor& cursor, int steps);
};

class VirtualTree::RouteLevelCopyAction : public VirtualTree::RouteCopyAction {
  private:
   int uLevel;
   VirtualTree::RelativePosition rpPos;

  protected:
   void increaseLevel() { uLevel++; }

  public:
   RouteLevelCopyAction(VirtualTree& thisTree,
         const ExtendedTreeInsertionParameters& parameters)
      :  RouteCopyAction(thisTree, false), uLevel(0), rpPos(parameters.getRelativePosition()) {}
   void setToCursor(const VirtualTreeCursor& cursor) { thisCursor() = cursor; }

   bool ascent(const Cursor& sourceCursor, EnhancedObject& son)
      {  VirtualTree::RouteCopyAction::ascent(sourceCursor, son);
         uLevel--;
         AssumeCondition(uLevel >= 0)
         return true;
      }
   bool next(const Cursor& sourceCursor, EnhancedObject& brother)
      {  if (uLevel > 0)
            RouteCopyAction::next(sourceCursor, brother);
         else {
            if (rpPos == RPBefore)
               thisTree().insertCopyAsPrevious(thisCursor(), sourceCursor.elementAt());
            else if (rpPos == RPAfter)
               thisTree().insertCopyAsNext(thisCursor(), sourceCursor.elementAt());
            else
               thisTree().addCopy(sourceCursor.elementAt(), thisCursor());
         };
         return true;
      }
   void descent() { VirtualTree::RouteCopyAction::descent(); uLevel++; }
   void descent(const EnhancedObject& son)
      {  VirtualTree::RouteCopyAction::descent(son); uLevel++; }
   bool descent(const Cursor& sourceCursor, EnhancedObject& father)
      {  VirtualTree::RouteCopyAction::descent(sourceCursor, father); uLevel++; return true; }
};

#define DefTypeCollection VirtualTree
#define DefTypeCursor VirtualTreeCursor
#define DefJustInline
   #include "Collection/ELMTreeCollection.inch"
#undef DefJustInline
#undef DefTypeCollection
#undef DefTypeCursor

class VirtualTree::CollectionAccess {
  public:
   typedef VirtualTree::ExtendedTreeLocateParameters ExtendedTreeLocateParameters;
   typedef VirtualTree::ExtendedTreeInsertionParameters ExtendedTreeInsertionParameters;
   typedef VirtualTree::ExtendedTreeSuppressParameters ExtendedTreeSuppressParameters;
   typedef VirtualTree::ExtendedTreeReplaceParameters ExtendedTreeReplaceParameters;
   static void fullAssign(VirtualTree& thisTree, const VirtualTree& sourceTree, const ExtendedTreeReplaceParameters& parameters)
      {  thisTree._fullAssign(sourceTree, parameters); }
   static void add(VirtualTree& thisTree, EnhancedObject* anObject,
         const ExtendedTreeInsertionParameters& parameters, VirtualTreeCursor* cursor=nullptr)
      {  thisTree._add(anObject, parameters, cursor); }
   static void addAll(VirtualTree& thisTree, const VirtualTree& sourceTree,
         const ExtendedTreeInsertionParameters& parameters, VirtualTreeCursor* cursor=nullptr,
         const VirtualTreeCursor* startSource=nullptr, const VirtualTreeCursor* endSource=nullptr)
      {  thisTree._addAll(sourceTree, parameters, cursor, startSource, endSource); }
   static void moveTo(VirtualTree& thisTree, VirtualTree& destination,
         const ExtendedTreeReplaceParameters& parameters, VirtualTreeCursor* cursor=nullptr,
         VirtualTreeCursor* destinationCursor=nullptr)
      {  thisTree._moveTo(destination, parameters, cursor, destinationCursor); }
   static void moveAllTo(VirtualTree& thisTree, VirtualTree& destination,
         const ExtendedTreeReplaceParameters& parameters, const VirtualTreeCursor* startCursor=nullptr,
         const VirtualTreeCursor* endCursor=nullptr, VirtualTreeCursor* destinationCursor=nullptr)
      {  thisTree._moveAllTo(destination, parameters, startCursor, endCursor, destinationCursor); }
   static void swap(VirtualTree& thisTree, VirtualTree& sourceTree) { thisTree._swap(sourceTree); }
   static void remove(VirtualTree& thisTree, const ExtendedTreeSuppressParameters& parameters,
         VirtualTreeCursor* cursor=nullptr) { thisTree._remove(parameters, cursor); }
   static void removeAll(VirtualTree& thisTree, const ExtendedTreeSuppressParameters& parameters,
         const VirtualTreeCursor* start=nullptr, const VirtualTreeCursor* end=nullptr)
      {  thisTree._removeAll(parameters, start, end); }
   static void replace(VirtualTree& thisTree, EnhancedObject* anObject,
         const ExtendedTreeReplaceParameters& parameters, VirtualTreeCursor* cursor=nullptr)
      {  thisTree._replace(anObject, parameters, cursor); }
   static EnhancedObject* queryElement(const VirtualTree& thisTree, const ExtendedTreeLocateParameters& parameters,
         const VirtualTreeCursor* cursor=nullptr)
      {  return thisTree._getElement(parameters, cursor); }
};

class VirtualTreeCursor::CursorAccess {
  public:
   typedef VirtualTreeCursor::ExtendedTreeLocateParameters ExtendedTreeLocateParameters;
   typedef VirtualTreeCursor::ExtendedTreeInsertionParameters ExtendedTreeInsertionParameters;
   typedef VirtualTreeCursor::ExtendedTreeSuppressParameters ExtendedTreeSuppressParameters;
   typedef VirtualTreeCursor::ExtendedTreeReplaceParameters ExtendedTreeReplaceParameters;
   typedef VirtualTreeCursor::TreePosition TreePosition;

   static bool isTreePositionned(const VirtualTreeCursor& cursor, const ExtendedTreeLocateParameters& pos,
         const VirtualTreeCursor* referenceCursor=nullptr)
      {  return cursor._isPositionned(pos, referenceCursor); }
   static bool position(VirtualTreeCursor& cursor, const TreePosition& pos)
      {  return cursor._position(pos); }
   static VirtualTree& getSupport(const VirtualTreeCursor& cursor)
      {  return (VirtualTree&) AbstractCursor::getSupport(cursor); }
};


} // end of namespace COL

#endif

