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
// File      : Binary.cpp
// Description :
//   Implementation of a class of balanced binary trees. It provides an implementation
//   for the COL::SortedAVL collection.
//

#include "Pointer/Binary.h"
#include "Pointer/Binary.template"

namespace COL {

/******************************************/
/* Implementation of class ImplBinaryNode */
/******************************************/

void
ImplBinaryNode::swap(ImplBinaryNode* fst, ImplBinaryNode* snd,
      PImplBinaryNode& parentFst, PImplBinaryNode& parentSnd, PImplBinaryNode& root) {
   AssumeCondition(root && fst && snd)
   if (fst != snd) {
      swapLeft(fst, snd);
      swapRight(fst, snd);

      if (parentFst != parentSnd) {
         if (parentFst != snd) {
            if (parentFst)
               parentFst->changeSon(fst, snd);
            else
               root = snd;
         }
         else
            parentFst = fst;
         if (parentSnd != fst) {
            if (parentSnd)
               parentSnd->changeSon(snd, fst);
            else
               root = fst;
         }
         else
            parentSnd = snd;
         ImplBinaryNode* temp = parentFst;
         parentFst = parentSnd;
         parentSnd = temp;
      }
      else {
         AssumeCondition(parentFst)
         if (parentFst->pibnLeft == fst)
            parentFst->pibnLeft = snd;
         else if (parentFst->pibnLeft == snd)
            parentFst->pibnLeft = fst;
         if (parentFst->pibnRight == fst)
            parentFst->pibnRight = snd;
         else if (parentFst->pibnRight == snd)
            parentFst->pibnRight = fst;
      };
   };
}

void
ImplBinaryNode::swapNew(ImplBinaryNode* fst, ImplBinaryNode* snd,
      ImplBinaryNode* parentFst, PImplBinaryNode& root) {
   AssumeCondition(root && fst && snd && !snd->pibnLeft && !snd->pibnRight)
   swapLeft(fst, snd);
   swapRight(fst, snd);

   if (parentFst)
      parentFst->changeSon(fst, snd);
   else
      root = snd;
}

/************************************************************/
/* Implementation of class ImplBinaryTreeCursorNotification */
/************************************************************/

void
ImplBinaryTreeCursorNotification::normalize() {
   AssumeCondition((iaanNotifications.count() == 0)
         || (uMinDepth == iaanNotifications[iaanNotifications.count()-1]->getDepth()))
   while (!iaanNotifications[iaanNotifications.count()-1]->isRotation()) {
      iaanNotifications.removeAtEnd();
      if (iaanNotifications.count() > 0)
         uMinDepth = iaanNotifications[iaanNotifications.count()-1]->getDepth();
   };
}

void
ImplBinaryTreeCursorNotification::AtomicNotification::applyRotation(DescentTrace& dtCursor) const {
   const DescentTrace& cursor = (const DescentTrace&) dtCursor;
   if (cursor[uDepth] != pibnAffectedNode)
      return;
   if (cursor.depth() > uDepth+1) {
      if (cursor[uDepth+1] == ((rRotation == RRotateLeft)
                                ? left(pibnAffectedNode) : right(pibnAffectedNode)))
         dtCursor.insertAt(uDepth, pibnNewRootNode);
      else if (cursor[uDepth+1] == pibnNewRootNode) {
         if (dtCursor.depth() > uDepth+2) {
            if (cursor[uDepth+2] == ((rRotation == RRotateLeft)
                                      ? right(pibnNewRootNode) : left(pibnNewRootNode)))
               dtCursor.removeAt(uDepth);
            else if (cursor[uDepth+2] == ((rRotation == RRotateLeft)
                                      ? right(pibnAffectedNode) : left(pibnAffectedNode))) {
               dtCursor[uDepth] = pibnNewRootNode;
               dtCursor[uDepth+1] = pibnAffectedNode;
            };
         }
         else
            dtCursor.removeAt(uDepth);
      }
      else // traitement des arbres ternaires
         dtCursor.insertAt(uDepth, pibnNewRootNode);
   }
   else
      dtCursor.insertAt(uDepth, pibnNewRootNode);
}

void
ImplBinaryTreeCursorNotification::apply(DescentTrace& dtCursor) const {
   const DescentTrace& cursor = (const DescentTrace&) dtCursor;
   if (!isValid()) {
      dtCursor.invalidate();
      return;
   };
   // AssumeCondition(isValid())
   int advanceNotificationIndex = iaanNotifications.count();
   int notificationIndex = 0;
   do {
      notificationIndex = advanceNotificationIndex-1;
      advanceNotificationIndex = notificationIndex-1;
      while ((advanceNotificationIndex >= 0)
         && (iaanNotifications[advanceNotificationIndex]->getDepth()
               == iaanNotifications[advanceNotificationIndex+1]->getDepth()))
         advanceNotificationIndex--;
   } while ((++advanceNotificationIndex >= 0)
         && (iaanNotifications[advanceNotificationIndex]->getDepth() < cursor.depth())
         && (cursor[iaanNotifications[advanceNotificationIndex]->getDepth()]
            == iaanNotifications[advanceNotificationIndex]->getAffectedNode()));
   if (advanceNotificationIndex < 0)
      notificationIndex = -1;

   while (++notificationIndex < iaanNotifications.count())
      iaanNotifications[notificationIndex]->apply(dtCursor);
}

/************************************************/
/* Implementation of class ImplBinaryTreeCursor */
/************************************************/

/* Implementation of class ImplBinaryTreeCursor::DescentTrace */

bool
ImplBinaryTreeCursor::DescentTrace::isValid() const {
   bool result = inherited::isValid();
   for (int index = 0; result && (index < count()-1); index++) {
      result = result && ((left(elementAt(index)) == elementAt(index+1))
            || (right(elementAt(index)) == elementAt(index+1)));
   };
   return result;
}

/* Implementation of class ImplBinaryTreeCursor */

ComparisonResult
ImplBinaryTreeCursor::DescentTrace::_compare(const EnhancedObject& asource) const {
   ComparisonResult result = inherited::_compare(asource);
   if (result != CREqual)
      return result;
   const DescentTrace& source = (const DescentTrace&) asource;
   int thisLength = count(), sourceLength = source.count();
   if ((thisLength == 0) || (sourceLength == 0))
      return (thisLength == sourceLength) ? CREqual : CRNonComparable;

   ImplBinaryNode *thisSon = nullptr, *sourceSon = nullptr;
   while (sourceLength > thisLength)
      sourceSon = source[--sourceLength];
   while (thisLength > sourceLength)
      thisSon = (*this)[--thisLength];
   while (source[--sourceLength] != (*this)[--thisLength]) {
      sourceSon = source[sourceLength];
      thisSon = (*this)[thisLength];
   };
   return compareSons((*this)[thisLength], thisSon, sourceSon);
}

bool
ImplBinaryTreeCursor::setToFirst(ImplBinaryNode* root) {
   dtTrace.removeAll();
   if (!root)
      return false;
   for (; root; root = root->left())
      dtTrace.push(root);
   return true;
}

bool
ImplBinaryTreeCursor::setToLast(ImplBinaryNode* root) {
   dtTrace.removeAll();
   if (!root)
      return false;
   for (; root; root = root->right())
      dtTrace.push(root);
   return true;
}

bool
ImplBinaryTreeCursor::setToNext(ImplBinaryNode* root) {
   if (dtTrace.isEmpty()) {
      if (!root)
         return false;
      for (; root; root = root->left())
         dtTrace.push(root);
      return true;
   }
   else {
      ImplBinaryNode* last=dtTrace.getLast();
      if (last->right()) {
         dtTrace.push(last = last->right());
         for (last = last->left(); last; last = last->left())
            dtTrace.push(last);
      }
      else {
         dtTrace.pop();
         while (!dtTrace.isEmpty() && (last == dtTrace.getLast()->right())) {
            last = dtTrace.getLast();
            dtTrace.pop();
         };
         AssumeCondition(dtTrace.isEmpty() || last == dtTrace.getLast()->left())
         return !dtTrace.isEmpty();
      };
   };
   return true;
}

bool
ImplBinaryTreeCursor::setToPrevious(ImplBinaryNode* root) {
   if (dtTrace.isEmpty()) {
      if (!root)
         return false;
      for (; root; root = root->right())
         dtTrace.push(root);
      return true;
   }
   else {
      ImplBinaryNode* last=dtTrace.getLast();
      if (last->left()) {
         dtTrace.push(last = last->left());
         for (last = last->right(); last; last = last->right())
            dtTrace.push(last);
      }
      else {
         dtTrace.pop();
         while (!dtTrace.isEmpty() && (last == dtTrace.getLast()->left())) {
            last = dtTrace.getLast();
            dtTrace.pop();
         };
         return !dtTrace.isEmpty();
      };
   };
   return true;
}

bool
ImplBinaryTreeCursor::isFirst() const {
   if (dtTrace.isEmpty() || getReference()->left())
      return false;
   ImplBinaryNode *parent = dtTrace[0], *son = nullptr;
   for (int index = 1; index < dtTrace.count(); index++) {
      son = dtTrace[index];
      if (parent->left() != son)
         return false;
      parent = son;
   };
   return true;
}

bool
ImplBinaryTreeCursor::isLast() const {
   if (dtTrace.isEmpty() || getReference()->right())
      return false;
   ImplBinaryNode *parent = dtTrace[0], *son = nullptr;
   for (int index = 1; index < dtTrace.count(); index++) {
      son = dtTrace[index];
      if (parent->right() != son)
         return false;
      parent = son;
   };
   return true;
}

bool
ImplBinaryTreeCursor::isNext(const ImplBinaryTreeCursor& source, bool isTreeEmpty) const {
   if (!isValid() || !source.isValid()) {
      if (!isValid() && !source.isValid())
         return isTreeEmpty;
      AssumeCondition(!isTreeEmpty)
      return !isValid() ? source.isLast() : isFirst();
   };
   AssumeCondition(!isTreeEmpty)
   if ((getReference()->left() && source.getReference()->right())
      || (!getReference()->left() && !source.getReference()->right()))
      return false;
   if (!getReference()->left()) { // source.getReference()->right();
      if (dtTrace.count() <= source.dtTrace.count())
         return false;
      ImplBinaryNode *parent = source.dtTrace[source.dtTrace.count()-1];
      if ((dtTrace[source.dtTrace.count()-1] != parent)
          || (dtTrace[source.dtTrace.count()] != parent->right()))
         return false;
      ImplBinaryNode *son = nullptr;
      for (int index = source.dtTrace.count()+1; index < dtTrace.count(); index++) {
         son = dtTrace[index];
         if (parent->left() != son)
            return false;
         parent = son;
      };
   }
   else { // !source.getReference()->right() && getReference()->left()
      if (dtTrace.count() >= source.dtTrace.count())
         return false;
      ImplBinaryNode *parent = dtTrace[dtTrace.count()-1];
      if ((source.dtTrace[dtTrace.count()-1] != parent)
          || (source.dtTrace[dtTrace.count()] != parent->left()))
         return false;
      ImplBinaryNode *son = nullptr;
      for (int index = dtTrace.count()+1; index < source.dtTrace.count(); index++) {
         son = source.dtTrace[index];
         if (parent->right() != son)
            return false;
         parent = son;
      };
   };
   return true;
}

/************************************************/
/* Implementation of class CustomImplBinaryTree */
/************************************************/

ImplBinaryNode*
CustomImplBinaryTree::queryFirst() const {
   ImplBinaryNode* result = pibnRoot;
   AssumeCondition(result)
   for (ImplBinaryNode* sonResult = result->left();
         sonResult && ((result = sonResult) != nullptr); )
      sonResult = result->left();
   return result;
}

ImplBinaryNode*
CustomImplBinaryTree::queryLast() const {
   ImplBinaryNode* result = pibnRoot;
   AssumeCondition(result)
   for (ImplBinaryNode* sonResult = result->right();
         sonResult && ((result = sonResult) != nullptr); )
      sonResult = result->right();
   return result;
}

/********************************************/
/* Implantation de la classe ImplBinaryTree */
/********************************************/

#if defined(__GNUC__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wstrict-overflow"
#endif
#define DefineTypeNode BalancedNode<CustomImplBinaryNode>
#define DefineTypeBinaryTree ImplBinaryTree
#include "Pointer/Binary.incc"
#undef DefineTypeNode
#undef DefineTypeBinaryTree

template ImplBinaryTree::Node::Balance
CustomImplBinaryTree::tlocateBefore<ImplBinaryTree::TemplateLocate>(ImplBinaryTree::TemplateLocate) const;

#if defined(__GNUC__)
#pragma GCC diagnostic pop
#endif
} // end of namespace COL

