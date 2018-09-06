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
// File      : Binary.h
// Description :
//   Definition of a class of balanced binary trees. It provides an implementation
//   for the COL::SortedAVL collection.
//

#ifndef COL_BinaryH
#define COL_BinaryH

#include "Pointer/ImplArray.h"
#include "Pointer/ImplList.h"

namespace COL {

/**************************************/
/* Definition of class ImplBinaryNode */
/**************************************/

class ImplBinaryTreeCursorNotification;
class BinaryInsertionNotification;
class BinarySuppressionNotification;
class BinaryReplaceNotification;
class CustomImplBinaryTree;
class ImplBinaryTreeCursor;

class ImplBinaryNode {
  public:
   enum Balance { BEqual=0, BLeft=1, BRight=2, BAll=3 };

  private:
   ImplBinaryNode* pibnLeft;
   ImplBinaryNode* pibnRight;

  protected:
   static void swapLeft(ImplBinaryNode* fst, ImplBinaryNode* snd);
   static void swapRight(ImplBinaryNode* fst, ImplBinaryNode* snd);

  protected:
   friend class ImplBinaryTreeCursorNotification;
   friend class BinaryInsertionNotification;
   friend class BinarySuppressionNotification;
   friend class BinaryReplaceNotification;
   friend class ImplBinaryTreeCursor;
   friend class CustomImplBinaryTree;
   typedef ImplBinaryNode* PImplBinaryNode;

   // Methodes get for inherited classes
   PImplBinaryNode& left() { return pibnLeft; }
   PImplBinaryNode& right() { return pibnRight; }
   const PImplBinaryNode& left() const { return pibnLeft; }
   const PImplBinaryNode& right() const { return pibnRight; }

   // Methods called to implement ImplBinaryTree
   bool changeSon(ImplBinaryNode* node, ImplBinaryNode* newNode);
   void setLeft(const PImplBinaryNode& left) { pibnLeft = left; }
   void setRight(const PImplBinaryNode& right) { pibnRight = right; }
   ImplBinaryNode* removeLeft();
   ImplBinaryNode* removeRight();
   void disconnect() { pibnLeft = pibnRight = nullptr; }
   void disconnectSon(ImplBinaryNode* node);
   bool isSingleton() const { return !pibnLeft && !pibnRight; }

   static void swap(ImplBinaryNode* fst, ImplBinaryNode* snd,
      PImplBinaryNode& parentFst, PImplBinaryNode& parentSnd, PImplBinaryNode& root);
   static void swapNew(ImplBinaryNode* fst, ImplBinaryNode* snd,
      ImplBinaryNode* parentFst, PImplBinaryNode& root);

   static ImplBinaryNode* rotateRight(ImplBinaryNode* node);
   static ImplBinaryNode* rotateLeft(ImplBinaryNode* node);

   ComparisonResult compareSons(ImplBinaryNode* fstSon, ImplBinaryNode* sndSon) const;

   class NodeToObjectCast {
     public:
      static EnhancedObject* castTo(ImplBinaryNode* source) { return (EnhancedObject*) (void*) source; }
      static const EnhancedObject* castTo(const ImplBinaryNode* source) { return (const EnhancedObject*) (void*) source; }
      static EnhancedObject& castTo(ImplBinaryNode& source) { return *((EnhancedObject*) (void*) &source); }
      static const EnhancedObject& castTo(const ImplBinaryNode& source) { return *((const EnhancedObject*) (void*) &source); }

      static ImplBinaryNode* castFrom(EnhancedObject* source) { return (ImplBinaryNode*) (void*) source; }
      static const ImplBinaryNode* castFrom(const EnhancedObject* source) { return (const ImplBinaryNode*) (void*) source; }
      static ImplBinaryNode& castFrom(EnhancedObject& source) { return *((ImplBinaryNode*) (void*) &source); }
      static const ImplBinaryNode& castFrom(const EnhancedObject& source) { return *((const ImplBinaryNode*) (void*) &source); }
   };

   class DescentTrace : public TImplArray<ImplBinaryNode, NodeToObjectCast> {
     private:
      typedef TImplArray<ImplBinaryNode, NodeToObjectCast> inherited;

     public:
      DescentTrace(ImplBinaryNode* node=nullptr, int defaultHeight=5)
         {  realloc(defaultHeight); if (node) insertAtEnd(node); }
      DescentTrace(const DescentTrace& source) : inherited(source) {}
      DefineCopy(DescentTrace)

      void setAllocationHeight(int defaultHeight) { realloc(defaultHeight); }
      bool push(ImplBinaryNode* node) { if (node) insertAtEnd(node); return node; }
      void pop() { removeAtEnd(); }
      void pop(ImplBinaryNode* node)
         {  removeAtEnd();
            referenceAt(count()-1) = NodeToObjectCast::castTo(node);
         }
      ImplBinaryNode* getLast() const { return elementAt(count()-1); }

      bool isEmpty() const { return count() == 0; }
      void setNode(int index, ImplBinaryNode* node)
         {  referenceAt(index) = NodeToObjectCast::castTo(node); }

      // Interface for cursors
      template <class TypeNode>
      class TraceProperty {
        private:
         int uIndex;
         DescentTrace* pdtCursor;

         TypeNode* getLastNode() const { return (TypeNode*) ((const DescentTrace&) *pdtCursor)[uIndex]; }

        public:
         TraceProperty(DescentTrace& cursor, int index)
            : uIndex(index), pdtCursor(&cursor) {}
         operator TypeNode*() const { return getLastNode(); }
         TypeNode* operator->() const { return getLastNode(); }
         TypeNode& operator*() const { return *getLastNode(); }
         TraceProperty<TypeNode>& operator=(TypeNode* node)
            {  pdtCursor->setNode(uIndex, node); return *this; }
      };
      template <class TypeNode> friend class TraceProperty;

      ImplBinaryNode* getReference() const { return getLast(); }
      void invalidate() { removeAll(); }
      const int& depth() const { return count(); }
      void removeAllAfter(int index) { removeAllBetween(index, -1); }

      TraceProperty<ImplBinaryNode> operator[](int index)
         { return TraceProperty<ImplBinaryNode>(*this, index); }
      ImplBinaryNode* operator[](int index) const { return inherited::operator[](index); }

      static ImplBinaryNode* left(ImplBinaryNode* node) { return node->left(); }
      static ImplBinaryNode* right(ImplBinaryNode* node) { return node->right(); }
      static ComparisonResult compareSons(const ImplBinaryNode* parent,
            ImplBinaryNode* fstSon, ImplBinaryNode* sndSon)
         {  return parent->compareSons(fstSon, sndSon); }
   };
   friend class DescentTrace;

  public:
   ImplBinaryNode() : pibnLeft(nullptr), pibnRight(nullptr) {}
   ImplBinaryNode(const ImplBinaryNode& /* source */) : pibnLeft(nullptr), pibnRight(nullptr) {}
   ImplBinaryNode& operator=(const ImplBinaryNode& /* source */) { return *this; }

   template <typename KeyType, class ElementType, class TabType, class Cast>
   bool tisValid(const TabType& tab) const
      {  return (!pibnLeft
               || (tab.keyFromCell((const ElementType&) Cast::castFrom(*pibnLeft))
                  < tab.keyFromCell((const ElementType&) Cast::castFrom(*this))))
            && (!pibnRight
               || (tab.keyFromCell((const ElementType&) Cast::castFrom(*pibnRight))
                  > tab.keyFromCell((const ElementType&) Cast::castFrom(*this))));
      }
   bool isValid() const { return true; }
};

/* Definition of inline methods of class ImplBinaryNode */

inline ImplBinaryNode*
ImplBinaryNode::removeLeft() {
   AssumeCondition(pibnLeft && (!pibnLeft->pibnRight || !pibnLeft->pibnLeft))
   ImplBinaryNode* temp = pibnLeft;
   pibnLeft = pibnLeft->pibnLeft ? pibnLeft->pibnLeft : pibnLeft->pibnRight;
   temp->disconnect();
   return temp;
}

inline ImplBinaryNode*
ImplBinaryNode::removeRight() {
   AssumeCondition(pibnRight && (!pibnRight->pibnRight || !pibnRight->pibnLeft))
   ImplBinaryNode* temp = pibnRight;
   pibnRight = pibnRight->pibnLeft ? pibnRight->pibnLeft : pibnRight->pibnRight;
   temp->disconnect();
   return temp;
}

inline void
ImplBinaryNode::disconnectSon(ImplBinaryNode* node) {
   AssumeCondition(node && ((node == pibnLeft) || (node == pibnRight))
      && !node->pibnLeft && !node->pibnRight)
   ((node == pibnLeft) ? pibnLeft : pibnRight) = nullptr;
}

inline bool
ImplBinaryNode::changeSon(ImplBinaryNode* node, ImplBinaryNode* newNode) {
   bool result = true;
   if (pibnLeft == node)
      pibnLeft = newNode;
   else if (pibnRight == node)
      pibnRight = newNode;
   else
      result = false;
   return result;
}

inline void
ImplBinaryNode::swapLeft(ImplBinaryNode* fst, ImplBinaryNode* snd) {
   ImplBinaryNode* temp = fst->pibnLeft;
   fst->pibnLeft = snd->pibnLeft;
   snd->pibnLeft = temp;
   if (fst->pibnLeft == fst)
      fst->pibnLeft = snd;
   if (snd->pibnLeft == snd)
      snd->pibnLeft = fst;
}

inline void
ImplBinaryNode::swapRight(ImplBinaryNode* fst, ImplBinaryNode* snd) {
   ImplBinaryNode* temp = fst->pibnRight;
   fst->pibnRight = snd->pibnRight;
   snd->pibnRight = temp;
   if (fst->pibnRight == fst)
      fst->pibnRight = snd;
   if (snd->pibnRight == snd)
      snd->pibnRight = fst;
}

inline ImplBinaryNode*
ImplBinaryNode::rotateRight(ImplBinaryNode* node) {
   AssumeCondition(node && node->pibnLeft)
   ImplBinaryNode* result = node->pibnLeft;
   node->pibnLeft = result->pibnRight;
   result->pibnRight = node;
   return result;
}

inline ImplBinaryNode*
ImplBinaryNode::rotateLeft(ImplBinaryNode* node) {
   AssumeCondition(node && node->pibnRight)
   ImplBinaryNode* result = node->pibnRight;
   node->pibnRight = result->pibnLeft;
   result->pibnLeft = node;
   return result;
}

inline ComparisonResult
ImplBinaryNode::compareSons(ImplBinaryNode* fstSon, ImplBinaryNode* sndSon) const {
   AssumeCondition((!fstSon || (fstSon == pibnLeft) || (fstSon == pibnRight))
      && (!sndSon || (sndSon == pibnLeft) || (sndSon == pibnRight)))
   return (fstSon == sndSon) ? CREqual :
         ((fstSon == pibnLeft) ? CRLess :
         ((fstSon == pibnRight) ? CRGreater :
         ((sndSon == pibnLeft) ? CRGreater :
         ((sndSon == pibnRight) ? CRLess : CRNonComparable))));
}

/********************************************************/
/* Definition of class ImplBinaryTreeCursorNotification */
/********************************************************/

class ImplBinaryTreeCursorNotification : public EnhancedObject {
  private:
   typedef ImplBinaryNode::PImplBinaryNode PImplBinaryNode;
   static const PImplBinaryNode& left(const PImplBinaryNode& implBinaryNode) { return implBinaryNode->left(); }
   static const PImplBinaryNode& right(const PImplBinaryNode& implBinaryNode) { return implBinaryNode->right(); }

   typedef ImplBinaryNode::DescentTrace DescentTrace;

  public:
   class AtomicNotification : public EnhancedObject {
     public:
      enum Rotation { RNoRotation, RRotateLeft, RRotateRight };

     private:
      ImplBinaryNode* pibnAffectedNode;
      ImplBinaryNode* pibnNewRootNode;
      Rotation rRotation;
      int uDepth;

     protected:
      const PImplBinaryNode& getAffectedNode() const { return pibnAffectedNode; }
      const PImplBinaryNode& getNewRootNode() const { return pibnNewRootNode; }
      const int& getDepth() const { return uDepth; }

      friend class ImplBinaryTreeCursorNotification;
      void setDepth(int depth) { AssumeCondition(uDepth == -1) uDepth = depth; }

     public:
      AtomicNotification(ImplBinaryNode* affectedNode)
         :  pibnAffectedNode(affectedNode), pibnNewRootNode(nullptr), rRotation(RNoRotation), uDepth(-1) {}
      AtomicNotification(ImplBinaryNode* affectedNode, ImplBinaryNode* newRootNode, Rotation rotation)
         :  pibnAffectedNode(affectedNode), pibnNewRootNode(newRootNode),
            rRotation(rotation), uDepth(-1) { AssumeCondition(rRotation != RNoRotation) }
      AtomicNotification(const AtomicNotification& source) = default;
      DefineCopy(AtomicNotification)
      DDefineAssign(AtomicNotification)

      void applyRotation(DescentTrace& cursor) const;
      void apply(DescentTrace& cursor) const
         {  if (rRotation != RNoRotation)
               applyRotation(cursor);
         }
      virtual bool isValid() const override
         {  return EnhancedObject::isValid() && pibnAffectedNode && (uDepth >= 0)
               && ((rRotation == RNoRotation) ? !pibnNewRootNode : ((bool) pibnNewRootNode));
         }
      bool isRotation() const { return rRotation != RNoRotation; }
   };
   friend class AtomicNotification;

  private:
   int uMinDepth;
   TImplArray<AtomicNotification> iaanNotifications;

  public:
   ImplBinaryTreeCursorNotification() : uMinDepth(-1) {}
   ImplBinaryTreeCursorNotification(const ImplBinaryTreeCursorNotification& source)
      :  EnhancedObject(source), uMinDepth(source.uMinDepth),
         iaanNotifications(source.iaanNotifications, true) {}
   virtual ~ImplBinaryTreeCursorNotification() { iaanNotifications.removeAll(true); }
   ImplBinaryTreeCursorNotification& operator=(const ImplBinaryTreeCursorNotification& source)
      {  EnhancedObject::operator=(source);
         uMinDepth = source.uMinDepth;
         iaanNotifications.assign(source.iaanNotifications, true);
         return *this;
      }

   DefineCopy(ImplBinaryTreeCursorNotification)
   DDefineAssign(ImplBinaryTreeCursorNotification)

   class SameLevel {};
   void add(const AtomicNotification& notification, SameLevel)
      {  AssumeCondition(uMinDepth >= 0)
         AtomicNotification* copyNotification = notification.createSCopy();
         iaanNotifications.insertAtEnd(copyNotification);
         copyNotification->setDepth(uMinDepth);
      }
   void add(const AtomicNotification& notification)
      {  AssumeCondition(uMinDepth > 0)
         AtomicNotification* copyNotification = notification.createSCopy();
         iaanNotifications.insertAtEnd(copyNotification);
         copyNotification->setDepth(--uMinDepth);
      }
   void apply(DescentTrace& cursor) const;

   void setStartLevel(int startLevel) { AssumeCondition(uMinDepth == -1) uMinDepth = startLevel; }
   virtual bool isValid() const override { return EnhancedObject::isValid() && (uMinDepth >= 0); }
   void normalize();
};

/***************************************************/
/* Definition of class BinaryInsertionNotification */
/***************************************************/

class BinaryInsertionNotification {
  private:
   ImplBinaryTreeCursorNotification ibtcnNotification;

  public:
   BinaryInsertionNotification() {}
   BinaryInsertionNotification(const BinaryInsertionNotification& source) = default;
   BinaryInsertionNotification& operator=(const BinaryInsertionNotification& source)
      {  ibtcnNotification.assign(source.ibtcnNotification); return *this; }

   // Methods to set up notifications
   ImplBinaryTreeCursorNotification& notifications() { return ibtcnNotification; }
   void setStart(int startLevel) { ibtcnNotification.setStartLevel(startLevel); }

   // Update methods
   void apply(ImplBinaryNode::DescentTrace& cursor) const
      {  ibtcnNotification.apply(cursor); }
   bool isValid() const { return ibtcnNotification.isValid(); }
};

/*****************************************************/
/* Definition of class BinarySuppressionNotification */
/*****************************************************/

class BinarySuppressionNotification {
  private:
   ImplBinaryTreeCursorNotification ibtcnBalanceNotifications;
   ImplBinaryNode* pnRemovedNode;
   int uRemovedDepth;
   ImplBinaryNode* pnExchangedNode;
   int uExchangedDepth;

  protected:
   bool isAfterRemoved(const ImplBinaryNode::DescentTrace& cursor, ImplBinaryNode* nextExpected) const
      {  return ((uExchangedDepth >= 0) && pnExchangedNode)
            ? ((uExchangedDepth+1 < cursor.depth())
               && (cursor[uExchangedDepth] == pnRemovedNode)
               && (cursor[uExchangedDepth+1] == nextExpected))
            : ((uRemovedDepth+1 < cursor.depth())
               && (cursor[uRemovedDepth] == pnRemovedNode)
               && (cursor[uRemovedDepth+1] == nextExpected));
      }

  public:
   BinarySuppressionNotification()
      :  pnRemovedNode(nullptr), uRemovedDepth(-1), pnExchangedNode(nullptr), uExchangedDepth(-1) {}
   BinarySuppressionNotification(const BinarySuppressionNotification& source) = default;
   BinarySuppressionNotification& operator=(const BinarySuppressionNotification& source) = default;

   // Methods to set fields
   ImplBinaryTreeCursorNotification& notifications() { return ibtcnBalanceNotifications; }
   void setStart(ImplBinaryNode* removedNode, int removedDepth)
      {  AssumeCondition(removedNode && (removedDepth >= 0))
         pnRemovedNode = removedNode;
         uRemovedDepth = removedDepth;
         ibtcnBalanceNotifications.setStartLevel(removedDepth);
      }
   void setStart(ImplBinaryNode* removedNode, int removedDepth,
         ImplBinaryNode* exchangedNode, int exchangedDepth)
      {  AssumeCondition(removedNode && (removedDepth >= 0)
            && exchangedNode && (exchangedDepth >= 0)
            && (exchangedDepth < removedDepth))
         pnRemovedNode = removedNode;
         uRemovedDepth = removedDepth;
         pnExchangedNode = exchangedNode;
         uExchangedDepth = exchangedDepth;
         ibtcnBalanceNotifications.setStartLevel(removedDepth);
      }

   // Notification methods
   void apply(ImplBinaryNode::DescentTrace& dtCursor) const
      {  const ImplBinaryNode::DescentTrace& cursor = (const ImplBinaryNode::DescentTrace&) dtCursor;
         if (!cursor.isEmpty()) {
            AssumeCondition(isValid())
            if (cursor.getReference() == pnRemovedNode)
               dtCursor.invalidate();
            else if ((uExchangedDepth >= 0) && pnExchangedNode && (uExchangedDepth < dtCursor.depth())
                  && (cursor[uExchangedDepth] == pnRemovedNode)) {
               dtCursor[uExchangedDepth] = pnExchangedNode;
               if (cursor.getReference() == pnExchangedNode)
                  dtCursor.removeAllAfter(uExchangedDepth+1);
               else if ((uRemovedDepth < dtCursor.depth()) && (cursor[uRemovedDepth] == pnExchangedNode))
                  dtCursor.removeAt(uRemovedDepth);
            }
            else if ((uRemovedDepth < dtCursor.depth()) && (cursor[uRemovedDepth] == pnRemovedNode))
               dtCursor.removeAt(uRemovedDepth);
            ibtcnBalanceNotifications.apply(dtCursor);
         };
      }

   bool isValid() const
      {  return ibtcnBalanceNotifications.isValid()
            && pnRemovedNode && (uRemovedDepth >= 0) && (uExchangedDepth < uRemovedDepth);
      }
};

/*************************************************/
/* Definition of class BinaryReplaceNotification */
/*************************************************/

class BinaryReplaceNotification {
  private:
   int uDepth;
   ImplBinaryNode* pnOldNode;
   ImplBinaryNode* pnNewNode;

  public:
   BinaryReplaceNotification() : uDepth(-1), pnOldNode(nullptr), pnNewNode(nullptr) {}
   BinaryReplaceNotification(const BinaryReplaceNotification& source) = default;
   BinaryReplaceNotification& operator=(const BinaryReplaceNotification& source) = default;

   // set methods
   void setStart(int depth, ImplBinaryNode* oldNode, ImplBinaryNode* newNode)
      {  uDepth = depth;
         pnOldNode = oldNode;
         pnNewNode = newNode;
      }

   // update methods for the cursors
   void apply(ImplBinaryNode::DescentTrace& dtCursor) const
      {  const ImplBinaryNode::DescentTrace& cursor = (const ImplBinaryNode::DescentTrace&) dtCursor;
         if (uDepth < cursor.depth() && cursor[uDepth] == pnOldNode)
            dtCursor[uDepth] = pnNewNode;
      }
   bool isValid() const
      {  return (uDepth >= 0) && pnOldNode && pnNewNode; }
};

/********************************************/
/* Definition of class ImplBinaryTreeCursor */
/********************************************/

class ImplBinaryTreeCursor : public EnhancedObject {
  protected:
   typedef ImplBinaryNode::DescentTrace ImplBinaryNodeDescentTrace;
   class DescentTrace : public ImplBinaryNodeDescentTrace {
     private:
      typedef ImplBinaryNodeDescentTrace inherited;

     protected:
      virtual ComparisonResult _compare(const EnhancedObject& asource) const override;

     public:
      DescentTrace(ImplBinaryNode* node=nullptr, int defaultHeight=5) : inherited(node, defaultHeight) {}
      DescentTrace(const DescentTrace& source) : inherited(source) {}
      DefineCopy(DescentTrace)

      virtual bool isValid() const override;
   };
   friend class DescentTrace;

  private:
   static ImplBinaryNode* left(ImplBinaryNode* node) { return node->left(); }
   static ImplBinaryNode* right(ImplBinaryNode* node) { return node->right(); }
   static ComparisonResult compareSons(const ImplBinaryNode* parent,
         ImplBinaryNode* fstSon, ImplBinaryNode* sndSon)
      {  return parent->compareSons(fstSon, sndSon); }

   DescentTrace dtTrace;

  protected:
   virtual ComparisonResult _compare(const EnhancedObject& asource) const override
      {  ComparisonResult result = EnhancedObject::_compare(asource);
         return (result == CREqual)
            ? dtTrace.compare(((const ImplBinaryTreeCursor&) asource).dtTrace) : result;
      }

   void invalidate() { dtTrace.invalidate();  }
   bool setToFirst(ImplBinaryNode* root);
   bool setToLast(ImplBinaryNode* root);
   bool setToNext(ImplBinaryNode* root);
   bool setToPrevious(ImplBinaryNode* root);

   bool setToLeft();
   bool setToRight();
   bool setToSon(ImplBinaryNode* son);
   bool setToParent() { dtTrace.pop(); return dtTrace.count() > 0; }
   bool setToRoot(ImplBinaryNode* root);

   bool isFirst() const;
   bool isLast() const;
   bool isNext(const ImplBinaryTreeCursor& source, bool isTreeEmpty) const;
   bool isPrevious(const ImplBinaryTreeCursor& source, bool isTreeEmpty) const
      { return source.isNext(*this, isTreeEmpty); }

   ImplBinaryNode* getParent() const { return dtTrace[dtTrace.count()-2]; }
   ImplBinaryNode* getReference() const { return dtTrace[dtTrace.count()-1]; }

   DescentTrace& trace() { return dtTrace; }
   const DescentTrace& trace() const { return dtTrace; }
   ImplBinaryNode* operator[](int uIndex) const { return dtTrace[uIndex]; }
   DescentTrace::TraceProperty<ImplBinaryNode> operator[](int index) { return dtTrace[index]; }

   void removeAt(int index) { dtTrace.removeAt(index); }
   void insertAt(int index, ImplBinaryNode* node) { dtTrace.insertAt(index, node); }

   template <class TypeNode> friend class TraceProperty;
   friend class ImplBinaryTreeCursorNotification;
   friend class ImplBinaryTreeCursorNotification::AtomicNotification;
   friend class BinaryInsertionNotification;
   friend class BinarySuppressionNotification;
   friend class BinaryReplaceNotification;

  public:
   ImplBinaryTreeCursor(ImplBinaryNode* node=nullptr, int defaultHeight=5)
      :  dtTrace(node, defaultHeight) {}
   ImplBinaryTreeCursor(const ImplBinaryTreeCursor& source) = default;
   DefineCopy(ImplBinaryTreeCursor)
   DDefineAssign(ImplBinaryTreeCursor)
   void swap(ImplBinaryTreeCursor& source) { dtTrace.swap(source.dtTrace); }

   void setAllocationHeight(int defaultHeight) { dtTrace.setAllocationHeight(defaultHeight); }
   virtual bool isValid() const override { return (dtTrace.count() > 0); }
   const int& depth() const { return dtTrace.depth(); }
   bool invariant() const { return EnhancedObject::isValid() && dtTrace.isValid(); }
};

template <class TypeNode>
class TImplBinaryTreeCursor : public ImplBinaryTreeCursor {
  protected:
   bool setToFirst(TypeNode* root) { return ImplBinaryTreeCursor::setToFirst(root); }
   bool setToLast(TypeNode* root) { return ImplBinaryTreeCursor::setToLast(root); }
   bool setToNext(TypeNode* root) { return ImplBinaryTreeCursor::setToNext(root); }
   bool setToPrevious(TypeNode* root) { return ImplBinaryTreeCursor::setToPrevious(root); }

   bool setToLeft() { return ImplBinaryTreeCursor::setToLeft(); }
   bool setToRight() { return ImplBinaryTreeCursor::setToRight(); }
   bool setToSon(TypeNode* ptnNode) { return ImplBinaryTreeCursor::setToSon(ptnNode); }
   bool setToParent() { return ImplBinaryTreeCursor::setToParent(); }
   bool setToRoot(TypeNode* node) { return ImplBinaryTreeCursor::setToRoot(node); }

   TypeNode* getParent() const { return (TypeNode*) ImplBinaryTreeCursor::getParent(); }
   TypeNode* getReference() const { return (TypeNode*) ImplBinaryTreeCursor::getReference(); }
   TypeNode* operator[](int index) const { return (TypeNode*) trace()[index]; }
   DescentTrace::TraceProperty<TypeNode> operator[](int index)
      { return DescentTrace::TraceProperty<TypeNode>(trace(), index); }

  public:
   TImplBinaryTreeCursor(TypeNode* node, int defaultHeight)
      : ImplBinaryTreeCursor(node, defaultHeight) {}
   TImplBinaryTreeCursor(const TImplBinaryTreeCursor<TypeNode>& source)
      : ImplBinaryTreeCursor(source) {}
   TemplateDefineCopy(TImplBinaryTreeCursor, TypeNode)
};

/* Definition of inline methods for class ImplBinaryTreeCursor */

inline bool
ImplBinaryTreeCursor::setToLeft() {
   ImplBinaryNode* son = getReference()->left();
   dtTrace.push(son);
   return son;
}

inline bool
ImplBinaryTreeCursor::setToRight() {
   ImplBinaryNode* son = getReference()->right();
   dtTrace.push(son);
   return son;
}

inline bool
ImplBinaryTreeCursor::setToSon(ImplBinaryNode* son) {
   AssumeCondition(son
         && ((getReference()->left() == son) || (getReference()->right() == son)))
   dtTrace.push(son);
   return true;
}

inline bool
ImplBinaryTreeCursor::setToRoot(ImplBinaryNode* root) {
   invalidate();
   dtTrace.push(root);
   return root;
}

/****************************************/
/* Definition of template TBalancedNode */
/****************************************/

template <class TypeNode>
class TBalancedNode : public TypeNode {
  public:
   typedef ImplBinaryNode::Balance Balance;
   static const Balance BLeft = ImplBinaryNode::BLeft;
   static const Balance BRight = ImplBinaryNode::BRight;
   static const Balance BAll = ImplBinaryNode::BAll;
   static const Balance BEqual = ImplBinaryNode::BEqual;

  protected:
   TemplateDefineExtendedParameters(2, TypeNode)

   TBalancedNode<TypeNode>* left() const { return (TBalancedNode<TypeNode>*) TypeNode::left(); }
   TBalancedNode<TypeNode>* right() const { return (TBalancedNode<TypeNode>*) TypeNode::right(); }

   Balance getBalance() const { return (Balance) queryOwnField(); }
   bool isLeftBalanced() const { return queryOwnField() & ImplBinaryNode::BLeft; }
   bool isRightBalanced() const { return queryOwnField() & ImplBinaryNode::BRight; }
   bool balance(Balance balance) const { return queryOwnField() == balance; }
   bool balance(TypeNode* son) const;
   bool unbalance(Balance balance) const { return queryOwnField() == (unsigned) (ImplBinaryNode::BAll-balance); }
   bool unbalance(TypeNode* son) const;
   void addBalance(TypeNode* son);
   void setEqualBalance() { clearOwnField(); }
   void removeBalance(Balance balance);

   bool setLeft(TBalancedNode<TypeNode>* left);
   bool setRight(TBalancedNode<TypeNode>* right);
   void setBalance(const TBalancedNode<TypeNode>& source) { setOwnField(source.queryOwnField()); }
   TBalancedNode<TypeNode>* removeLeft()
      {  TBalancedNode<TypeNode>* result = (TBalancedNode<TypeNode>*) TypeNode::removeLeft();
         result->setOwnField(ImplBinaryNode::BEqual);
         return result;
      }
   TBalancedNode<TypeNode>* removeRight()
      {  TBalancedNode<TypeNode>* result = (TBalancedNode<TypeNode>*) TypeNode::removeRight();
         result->setOwnField(ImplBinaryNode::BEqual);
         return result;
      }
   TBalancedNode<TypeNode>* removeSon(Balance balance)
      {  AssumeCondition((balance == ImplBinaryNode::BLeft) || (balance == ImplBinaryNode::BRight))
         return (balance == ImplBinaryNode::BLeft) ? removeLeft() : removeRight();
      }
   void disconnect() { TypeNode::disconnect(); setOwnField(ImplBinaryNode::BEqual); }

   static TBalancedNode<TypeNode>* rotateRight(TBalancedNode<TypeNode>* node, bool& doesPropagate);
   static TBalancedNode<TypeNode>* rotateLeft(TBalancedNode<TypeNode>* node, bool& doesPropagate);
   static TBalancedNode<TypeNode>* rotateLeftRight(TBalancedNode<TypeNode>* node);
   static TBalancedNode<TypeNode>* rotateRightLeft(TBalancedNode<TypeNode>* node);
   static void swapBalance(TBalancedNode<TypeNode>* fst, TBalancedNode<TypeNode>* snd)
      {  int balance = fst->queryOwnField();
         fst->setOwnField(snd->queryOwnField());
         snd->setOwnField(balance);
      }

  public:
   TBalancedNode<TypeNode>() {}
   TBalancedNode<TypeNode>(const TBalancedNode<TypeNode>& source)
      : TypeNode(source) { setOwnField(ImplBinaryNode::BEqual); }
   TBalancedNode<TypeNode>& operator=(const TBalancedNode<TypeNode>& source)
      {  return (TBalancedNode<TypeNode>&) TypeNode::operator=(source); }

   bool isValid() const { return TypeNode::isValid() && (queryOwnField() != ImplBinaryNode::BAll); }
};

/* Definition of inline methods of template TBalancedNode */

template <class TypeNode>
inline bool
TBalancedNode<TypeNode>::balance(TypeNode* son) const {
   AssumeCondition((left() == son) || (right() == son))
   return (left() == son) ? (queryOwnField() & ImplBinaryNode::BLeft) : (queryOwnField() & ImplBinaryNode::BRight);
}

template <class TypeNode>
inline bool
TBalancedNode<TypeNode>::unbalance(TypeNode* son) const {
   AssumeCondition((left() == son) || (right() == son))
   return (left() == son) ? (queryOwnField() & ImplBinaryNode::BRight) : (queryOwnField() & ImplBinaryNode::BLeft);
}

template <class TypeNode>
inline void
TBalancedNode<TypeNode>::addBalance(TypeNode* son) {
   AssumeCondition(!hasOwnField() && ((left() == son) || (right() == son)))
   setOwnField((left() == son) ? ImplBinaryNode::BLeft : ImplBinaryNode::BRight);
}

template <class TypeNode>
inline void
TBalancedNode<TypeNode>::removeBalance(Balance balance) {
   AssumeCondition((balance == ImplBinaryNode::BLeft) || (balance == ImplBinaryNode::BRight))
   setOwnField(ImplBinaryNode::BAll-balance);
}

template <class TypeNode>
inline bool
TBalancedNode<TypeNode>::setLeft(TBalancedNode<TypeNode>* left) {
   AssumeCondition(!TypeNode::left() && !left->queryOwnField() && left->isSingleton())
   TypeNode::setLeft(left);
   bool result = false;
   if (queryOwnField() == ImplBinaryNode::BRight)
      setOwnField(ImplBinaryNode::BEqual);
   else if (queryOwnField() == ImplBinaryNode::BEqual)
      result = true;
   else
      AssumeUncalled
   return result;
}

template <class TypeNode>
inline bool
TBalancedNode<TypeNode>::setRight(TBalancedNode<TypeNode>* right) {
   AssumeCondition(!TypeNode::right() && !right->queryOwnField())
   TypeNode::setRight(right);
   bool result = false;
   if (queryOwnField() == ImplBinaryNode::BLeft)
      setOwnField(ImplBinaryNode::BEqual);
   else if (queryOwnField() == ImplBinaryNode::BEqual)
      result = true;
   else
      AssumeUncalled
   return result;
}

template <class TypeNode>
inline TBalancedNode<TypeNode>*
TBalancedNode<TypeNode>::rotateRight(TBalancedNode<TypeNode>* node, bool& doesPropagate) {
   AssumeCondition((node->queryOwnField() & ImplBinaryNode::BLeft) && !(node->left()->queryOwnField() & ImplBinaryNode::BRight))
   TBalancedNode<TypeNode>* result = (TBalancedNode<TypeNode>*) TypeNode::rotateRight(node);
   if (result->queryOwnField() & ImplBinaryNode::BLeft) {
      node->setOwnField(ImplBinaryNode::BEqual);
      result->setOwnField(ImplBinaryNode::BEqual);
      doesPropagate = false;
   }
   else {
      node->setOwnField(ImplBinaryNode::BLeft);
      result->setOwnField(ImplBinaryNode::BRight);
      doesPropagate = true;
   };
   return result;
}

template <class TypeNode>
inline TBalancedNode<TypeNode>*
TBalancedNode<TypeNode>::rotateLeft(TBalancedNode<TypeNode>* node, bool& doesPropagate) {
   AssumeCondition((node->queryOwnField() & ImplBinaryNode::BRight) && !(node->right()->queryOwnField() & ImplBinaryNode::BLeft))
   TBalancedNode<TypeNode>* result = (TBalancedNode<TypeNode>*) TypeNode::rotateLeft(node);
   if (result->queryOwnField() & ImplBinaryNode::BRight) {
      node->setOwnField(ImplBinaryNode::BEqual);
      result->setOwnField(ImplBinaryNode::BEqual);
      doesPropagate = false;
   }
   else {
      node->setOwnField(ImplBinaryNode::BRight);
      result->setOwnField(ImplBinaryNode::BLeft);
      doesPropagate = true;
   };
   return result;
}

template <class TypeNode>
inline TBalancedNode<TypeNode>*
TBalancedNode<TypeNode>::rotateLeftRight(TBalancedNode<TypeNode>* node) {
   AssumeCondition((node->queryOwnField() & ImplBinaryNode::BLeft) && (node->left()->queryOwnField() & ImplBinaryNode::BRight))
   TBalancedNode<TypeNode>* temp = node->left();
   TBalancedNode<TypeNode>* result = (TBalancedNode<TypeNode>*) TypeNode::rotateLeft(temp);
   node->TypeNode::setLeft(result);
   result = (TBalancedNode<TypeNode>*) TypeNode::rotateRight(node);
   temp->setOwnField(result->queryOwnField() & ImplBinaryNode::BRight ? ImplBinaryNode::BLeft : ImplBinaryNode::BEqual);
   node->setOwnField(result->queryOwnField() & ImplBinaryNode::BLeft ? ImplBinaryNode::BRight : ImplBinaryNode::BEqual);
   result->setOwnField(ImplBinaryNode::BEqual);
   return result;
}

template <class TypeNode>
inline TBalancedNode<TypeNode>*
TBalancedNode<TypeNode>::rotateRightLeft(TBalancedNode<TypeNode>* node) {
   AssumeCondition((node->queryOwnField() & ImplBinaryNode::BRight) && (node->right()->queryOwnField() & ImplBinaryNode::BLeft))
   TBalancedNode<TypeNode>* temp = node->right();
   TBalancedNode<TypeNode>* result = (TBalancedNode<TypeNode>*) TypeNode::rotateRight(temp);
   node->TypeNode::setRight(result);
   result = (TBalancedNode<TypeNode>*) TypeNode::rotateLeft(node);
   temp->setOwnField(result->queryOwnField() & ImplBinaryNode::BLeft ? ImplBinaryNode::BRight : ImplBinaryNode::BEqual);
   node->setOwnField(result->queryOwnField() & ImplBinaryNode::BRight ? ImplBinaryNode::BLeft : ImplBinaryNode::BEqual);
   result->setOwnField(ImplBinaryNode::BEqual);
   return result;
}

/****************************************/
/* Definition of class ImplBalancedNode */
/****************************************/

class ImplExtendedBinaryNode : public ImplBinaryNode, protected ExtendedParameters {
  public:
   ImplExtendedBinaryNode() {}
   ImplExtendedBinaryNode(const ImplExtendedBinaryNode& source)
      :  ImplBinaryNode(source), ExtendedParameters() {}
   ImplExtendedBinaryNode& operator=(const ImplExtendedBinaryNode& source)
      {  return (ImplExtendedBinaryNode&) ImplBinaryNode::operator=(source); }
};

class ImplBalancedNode : public TBalancedNode<ImplExtendedBinaryNode> {
  public:
   typedef ImplBinaryNode::PImplBinaryNode PImplBinaryNode;

  private:
   typedef TBalancedNode<ImplExtendedBinaryNode> inherited;

  protected:
   friend class ImplBinaryTree;
   ImplBalancedNode* left() const { return (ImplBalancedNode*) inherited::left(); }
   ImplBalancedNode* right() const { return (ImplBalancedNode*) inherited::right(); }

   static void swap(ImplBalancedNode* fst, ImplBalancedNode* snd,
      PImplBinaryNode& parentFst, PImplBinaryNode& parentSnd, PImplBinaryNode& root)
      {  inherited::swap(fst, snd, parentFst, parentSnd, root);
         swapBalance(fst, snd);
      }
   static void swapNew(ImplBalancedNode* fst, ImplBalancedNode* snd,
      ImplBalancedNode* parentFst, PImplBinaryNode& root)
      {  inherited::swapNew(fst, snd, parentFst, root);
         swapBalance(fst, snd);
      }

  public:
   ImplBalancedNode() {}
   ImplBalancedNode(const ImplBalancedNode& source) : inherited(source) {}
};

/********************************************/
/* Definition of class CustomImplBinaryTree */
/********************************************/

class CustomImplBinaryTree : public EnhancedObject {
  private:
   typedef ImplBinaryNode::PImplBinaryNode PImplBinaryNode;
   ImplBinaryNode* pibnRoot;
   int uCount;
   int uHeight;

  protected:
   template <class Action, class Node, class Tree, class Cursor>
      bool tsetToFirst(Cursor& cursor, Action& action) const;
   template <class Action, class Node, class Tree, class Cursor>
      bool tsetToLast(Cursor& cursor, Action& action) const;
   template <class Action, class Node, class Tree, class Cursor>
      bool tsetToNext(Cursor& cursor, Action& action) const;
   template <class Action, class Node, class Tree, class Cursor>
      bool tsetToPrevious(Cursor& cursor, Action& action) const;
   template <class Action, class Node, class Tree, class Cursor>
      bool foreachNodeDo(Action& action) const;
   template <class Action, class Node, class Tree, class Cursor>
      bool foreachReverseNodeDo(Action& action) const;

   // struct TemplateLocate {
   //   public:
   //    typename KeyType;
   //    class ElementType;
   //    class TabType;
   //    class TabImplementation;
   //    class Cast;
   //    class TypeNode;
   //    class TypeCursor;
   //
   //    KeyType ktKey;
   //    TypeCursor cCursor;
   //    static ComparisonResult compare(KeyType fst, KeyType snd);
   // };

   template <class TemplateLocate>
         ImplBalancedNode::Balance tlocateBefore(TemplateLocate locate) const;

   const PImplBinaryNode& root() const { return pibnRoot; }
   PImplBinaryNode& root() { return pibnRoot; }
   void setRoot(ImplBinaryNode* newRoot) { pibnRoot = newRoot; }
   int& count() { return uCount; }
   const int& height() const { return uHeight; }
   int& height() { return uHeight; }
   void clearRoot() { pibnRoot = nullptr; uCount = uHeight = 0; }
   static const CustomImplBinaryTree& castFromImplementation(const CustomImplBinaryTree& tree)
      {  return tree; }

  public:
   CustomImplBinaryTree() : pibnRoot(nullptr), uCount(0), uHeight(0) {}
   CustomImplBinaryTree(const CustomImplBinaryTree& source)
      :  EnhancedObject(source), pibnRoot(nullptr), uCount(0), uHeight(0) {}
   CustomImplBinaryTree& operator=(const CustomImplBinaryTree& source)
      {  return (CustomImplBinaryTree&) EnhancedObject::operator=(source); }
   DefineCopy(CustomImplBinaryTree)
   DDefineAssign(CustomImplBinaryTree)

     // get methods
   bool isEmpty() const { return !pibnRoot; }
   bool isSingleton() const { return (pibnRoot && pibnRoot->isSingleton()); }
   const int& count() const { return uCount; }

   ImplBinaryNode* queryFirst() const;
   ImplBinaryNode* queryLast() const;

   void swap(CustomImplBinaryTree& source);
};

inline void
CustomImplBinaryTree::swap(CustomImplBinaryTree& source) {
   ImplBinaryNode* tempNode = pibnRoot;
   pibnRoot = source.pibnRoot;
   source.pibnRoot = tempNode;
   int temp = uCount;
   uCount = source.uCount;
   source.uCount = temp;
   temp = uHeight;
   uHeight = source.uHeight;
   source.uHeight = temp;
}

/**************************************/
/* Definition of class ImplBinaryTree */
/**************************************/

class CustomImplBinaryNode : public EnhancedObject, public ImplBalancedNode {
  protected:
   friend class ImplBinaryTree;

   static CustomImplBinaryNode* rotateRight(CustomImplBinaryNode* node, bool& doesPropagate)
      {  return (CustomImplBinaryNode*) ImplBalancedNode::rotateRight(node, doesPropagate); }
   static CustomImplBinaryNode* rotateLeft(CustomImplBinaryNode* node, bool& doesPropagate)
      {  return (CustomImplBinaryNode*) ImplBalancedNode::rotateLeft(node, doesPropagate); }
   static CustomImplBinaryNode* rotateLeftRight(CustomImplBinaryNode* node)
      {  return (CustomImplBinaryNode*) ImplBalancedNode::rotateLeftRight(node); }
   static CustomImplBinaryNode* rotateRightLeft(CustomImplBinaryNode* node)
      {  return (CustomImplBinaryNode*) ImplBalancedNode::rotateRightLeft(node); }

   static void swap(CustomImplBinaryNode* fst, CustomImplBinaryNode* snd,
      CustomImplBinaryNode*& sparentFst, CustomImplBinaryNode*& sparentSnd, PImplBinaryNode& root)
      {  ImplBinaryNode *parentFst = sparentFst, *parentSnd = sparentSnd;
         ImplBalancedNode::swap(fst, snd, parentFst, parentSnd, root);
         sparentFst = (CustomImplBinaryNode*) parentFst;
         sparentSnd = (CustomImplBinaryNode*) parentSnd;
      }

  public:
   CustomImplBinaryNode() {}
   CustomImplBinaryNode(const CustomImplBinaryNode& source) = default;
   DefineCopy(CustomImplBinaryNode)
   DDefineAssign(CustomImplBinaryNode)

   virtual bool isValid() const override
      {  return EnhancedObject::isValid() && ImplBalancedNode::isValid(); }
};

#define DefineTypeNode CustomImplBinaryNode
#define DefineTypeBinaryTree ImplBinaryTree
#define DefineTypeCast SimpleCast 
#include "Pointer/Binary.inch"
#undef DefineTypeCast
#undef DefineTypeNode
#undef DefineTypeBinaryTree

}

#endif
