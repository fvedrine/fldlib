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
// File      : BinaryParent.cpp
// Description :
//   Implementation of balanced binary trees with parents.
//

#include "Collection/Implementation/BinaryParent.h"

namespace COL {};

#include "Pointer/Binary.template"

namespace COL {

/************************************************/
/* Implementation of class ImplParentBinaryNode */
/************************************************/

void
ImplParentBinaryNode::swapLeft(ImplParentBinaryNode* fst, ImplParentBinaryNode* snd) {
   inherited::swapLeft(fst, snd);
   ImplParentBinaryNode* temp = fst->pipbnParent;
   fst->pipbnParent = snd->pipbnParent;
   snd->pipbnParent = temp;
   if (fst->cleft()) {
      fst->cleft()->pipbnParent = fst;
      if ((fst->cleft() == snd) && fst->pipbnParent)
         fst->pipbnParent->ImplBinaryNode::changeSon(snd, fst);
   };

   if (snd->cleft()) {
      snd->cleft()->pipbnParent = snd;
      if ((snd->cleft() == fst) && snd->pipbnParent)
         snd->pipbnParent->ImplBinaryNode::changeSon(fst, snd);
   };
}

void
ImplParentBinaryNode::swapRight(ImplParentBinaryNode* fst, ImplParentBinaryNode* snd) {
   ImplBinaryNode::swapRight(fst, snd);
   ImplParentBinaryNode* temp = fst->pipbnParent;
   fst->pipbnParent = snd->pipbnParent;
   snd->pipbnParent = temp;
   if (fst->cright()) {
      fst->cright()->pipbnParent = fst;
      if ((fst->cright() == snd) && fst->pipbnParent)
         fst->pipbnParent->ImplBinaryNode::changeSon(snd, fst);
   };

   if (snd->cright()) {
      snd->cright()->pipbnParent = snd;
      if ((snd->cright() == fst) && snd->pipbnParent)
         snd->pipbnParent->ImplBinaryNode::changeSon(fst, snd);
   };
}

bool
ImplParentBinaryNode::changeSon(ImplParentBinaryNode* node, ImplParentBinaryNode* newNode) {
   bool result = true;
   if (ImplBinaryNode::left() == node)
      setLeft(newNode);
   else if (ImplBinaryNode::right() == node)
      setRight(newNode);
   else
      result = false;
   node->pipbnParent = nullptr;
   return result;
}

void
ImplParentBinaryNode::cswap(ImplParentBinaryNode* fst, ImplParentBinaryNode* snd) {
   ImplParentBinaryNode* temp = fst->cleft();
   fst->left() = snd->cleft();
   snd->left() = temp;
   if (fst->cleft())
      fst->cleft()->pipbnParent = fst;
   if (snd->cleft())
      snd->cleft()->pipbnParent = snd;

   temp = fst->cright();
   fst->right() = snd->cright();
   snd->right() = temp;
   if (fst->cright())
      fst->cright()->pipbnParent = fst;
   if (snd->cright())
      snd->cright()->pipbnParent = snd;

   temp = fst->pipbnParent;
   fst->pipbnParent = snd->pipbnParent;
   snd->pipbnParent = temp;
   if (fst->pipbnParent) {
      if (!fst->pipbnParent->ImplBinaryNode::changeSon(snd, fst))
         fst->pipbnParent = nullptr;
   };
   if (snd->pipbnParent) {
      if (!snd->pipbnParent->ImplBinaryNode::changeSon(fst, snd))
         snd->pipbnParent = nullptr;
   };
}

/******************************************************/
/* Implementation of class ImplParentBinaryTreeCursor */
/******************************************************/

ComparisonResult
ImplParentBinaryTreeCursor::_compare(const EnhancedObject& asource) const {
   ComparisonResult result = inherited::_compare(asource);
   if (result == CREqual) {
      const ImplParentBinaryTreeCursor& source = (const ImplParentBinaryTreeCursor&) asource;
      if ((pipbnElement == nullptr) || (source.pipbnElement == nullptr))
         return ((pipbnElement == nullptr) && (source.pipbnElement == nullptr)) ? CREqual : CRNonComparable;
      int depth = queryDepth(), sourceDepth = source.queryDepth();
      ImplParentBinaryNode
         *node = pipbnElement, *sourceNode = source.pipbnElement,
         *sonNode = nullptr, *sourceSonNode = nullptr;
      while (depth > sourceDepth) {
         sonNode = node;
         node = node->parent();
         --depth;
      };
      while (sourceDepth > depth) {
         sourceSonNode = sourceNode;
         sourceNode = sourceNode->parent();
         --sourceDepth;
      };
      while (node != sourceNode) {
         AssumeCondition(node && sourceNode)
         sonNode = node;
         node = node->parent();
         --depth;
         sourceSonNode = sourceNode;
         sourceNode = sourceNode->parent();
         --sourceDepth;
      };
      AssumeCondition(node)
      return node->compareSons(sonNode, sourceSonNode);
   };
   return result;
}

bool
ImplParentBinaryTreeCursor::setToFirst(ImplParentBinaryNode* root) {
   pipbnElement = root;
   if (!pipbnElement)
      return false;
   while (pipbnElement->cleft())
      pipbnElement = pipbnElement->cleft();
   return true;
}

bool
ImplParentBinaryTreeCursor::setToLast(ImplParentBinaryNode* root) {
   pipbnElement = root;
   if (!pipbnElement)
      return false;
   while (pipbnElement->cright())
      pipbnElement = pipbnElement->cright();
   return true;
}

bool
ImplParentBinaryTreeCursor::setToNext(ImplParentBinaryNode* root) {
   if (pipbnElement == nullptr) {
      if (!root)
         return false;
      pipbnElement = root;
      while (pipbnElement->cleft())
         pipbnElement = pipbnElement->cleft();
      return true;
   }
   else {
      if (pipbnElement->cright()) {
         pipbnElement = pipbnElement->cright();
         while (pipbnElement->cleft())
            pipbnElement = pipbnElement->cleft();
      }
      else {
         ImplParentBinaryNode* son = pipbnElement;
         pipbnElement = pipbnElement->parent();
         while ((pipbnElement != nullptr) && (son == pipbnElement->cright())) {
            son = pipbnElement;
            pipbnElement = pipbnElement->parent();
         };
         return (pipbnElement != nullptr);
      };
   };
   return true;
}

bool
ImplParentBinaryTreeCursor::setToPrevious(ImplParentBinaryNode* root) {
   if (pipbnElement == nullptr) {
      if (root == nullptr)
         return false;
      pipbnElement = root;
      while (pipbnElement->cleft())
         pipbnElement = pipbnElement->cleft();
      return true;
   }
   else {
      if (pipbnElement->cleft()) {
         pipbnElement = pipbnElement->cleft();
         while (pipbnElement->cright())
            pipbnElement = pipbnElement->cright();
      }
      else {
         ImplParentBinaryNode* son = pipbnElement;
         pipbnElement = pipbnElement->parent();
         while ((pipbnElement != nullptr) && (son == pipbnElement->cleft())) {
            son = pipbnElement;
            pipbnElement = pipbnElement->parent();
         };
         return (pipbnElement != nullptr);
      };
   };
   return true;
}

bool
ImplParentBinaryTreeCursor::isFirst() const {
   if ((pipbnElement == nullptr) || pipbnElement->cleft())
      return false;
   ImplParentBinaryNode* node = pipbnElement;
   while (node->parent() && (node->parent()->cleft() == node))
      node = node->parent();
   return (node->parent() == nullptr);
}

bool
ImplParentBinaryTreeCursor::isLast() const {
   if ((pipbnElement == nullptr) || pipbnElement->cright())
      return false;
   ImplParentBinaryNode* node = pipbnElement;
   while (node->parent() && (node->parent()->cright() == node))
      node = node->parent();
   return (node->parent() == nullptr);
}

bool
ImplParentBinaryTreeCursor::isNext(const ImplParentBinaryTreeCursor& source, bool isTreeEmpty) const {
   if (!ImplParentBinaryTreeCursor::isValid() || !source.ImplParentBinaryTreeCursor::isValid()) {
      if (!isValid() && !source.isValid())
         return isTreeEmpty;
      AssumeCondition(!isTreeEmpty)
      return !isValid() ? source.isLast() : isFirst();
   };
   AssumeCondition(!isTreeEmpty)
   if ((pipbnElement->cleft() && source.pipbnElement->cright())
      || (!pipbnElement->cleft() && !source.pipbnElement->cright()))
      return false;
   if (!pipbnElement->cleft()) { // source.pipbnElement->cright();
      ImplParentBinaryNode *node = pipbnElement;
      while (node->parent() && (node->parent()->cleft() == node))
         node = node->parent();
      return node->parent() == source.pipbnElement;
   }
   else { // !source.pipbnElement->cright() && pipbnElement->cleft()
      ImplParentBinaryNode *sourceNode = source.pipbnElement;
      while (sourceNode->parent() && (sourceNode->parent()->cright() == sourceNode))
         sourceNode = sourceNode->parent();
      return sourceNode->parent() == pipbnElement;
   };
}

int
ImplParentBinaryTreeCursor::queryDepth() const {
   int result = 0;
   ImplParentBinaryNode* node = pipbnElement;
   while (node != nullptr) {
      node = node->parent();
      ++result;
   };
   return result;
}

/************************************************/
/* Implementation of class ImplParentBinaryTree */
/************************************************/

#define DefineTypeNode BalancedNode<CustomImplParentBinaryNode>
#define DefineTypeBinaryTree ImplParentBinaryTree
#define DefineTypeBinaryParent
#include "Pointer/Binary.incc"
#undef DefineTypeBinaryParent
#undef DefineTypeNode
#undef DefineTypeBinaryTree

template ImplParentBinaryTree::Node::Balance
CustomImplBinaryTree::tlocateBefore<ImplParentBinaryTree::TemplateLocate>(ImplParentBinaryTree::TemplateLocate) const;

} // end of namespace COL

