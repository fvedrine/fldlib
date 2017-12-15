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
// File      : BinaryParent.h
// Description :
//   Definition of a class of balanced binary trees with parents. It provides an
//   implementation for the COL::SortedParentAVL collection.
//

#ifndef COL_BinaryParentH
#define COL_BinaryParentH

#include "Pointer/Binary.h"

namespace COL {

/********************************************/
/* Definition of class ImplParentBinaryNode */
/********************************************/

class ImplParentBinaryTree;
class ImplParentBinaryTreeCursor;
class ImplParentBinaryNode : public ImplBinaryNode {
  private:
   typedef ImplBinaryNode inherited;
   ImplParentBinaryNode* pipbnParent;

   static void swapLeft(ImplParentBinaryNode* fst, ImplParentBinaryNode* snd);
   static void swapRight(ImplParentBinaryNode* fst, ImplParentBinaryNode* snd);

  protected:
   friend class ImplParentBinaryTree;
   friend class ImplParentBinaryTreeCursor;
   typedef ImplParentBinaryNode* PImplParentBinaryNode;

   class SiblingNodeAccess {
     private:
      PImplBinaryNode& pibnSibling;

     public:
      SiblingNodeAccess(PImplBinaryNode& sibling) : pibnSibling(sibling) {}
      SiblingNodeAccess& operator=(ImplParentBinaryNode* sibling)
         {  pibnSibling = sibling; return *this; }
      operator ImplParentBinaryNode*() const { return (ImplParentBinaryNode*) pibnSibling; }
      ImplParentBinaryNode* operator->() const { return (ImplParentBinaryNode*) pibnSibling; }
      ImplParentBinaryNode& operator*() const { return (ImplParentBinaryNode&) *pibnSibling; }
   };

   // get methods for derived implementations
   ImplParentBinaryNode* left() const { return (ImplParentBinaryNode*) ImplBinaryNode::left(); }
   ImplParentBinaryNode* right() const { return (ImplParentBinaryNode*) ImplBinaryNode::right(); }
   SiblingNodeAccess left() { return SiblingNodeAccess(ImplBinaryNode::left()); }
   SiblingNodeAccess right() { return SiblingNodeAccess(ImplBinaryNode::right()); }

   ImplParentBinaryNode* cleft() const { return (ImplParentBinaryNode*) ImplBinaryNode::left(); }
   ImplParentBinaryNode* cright() const { return (ImplParentBinaryNode*) ImplBinaryNode::right(); }
   SiblingNodeAccess sleft() { return SiblingNodeAccess(ImplBinaryNode::left()); }
   SiblingNodeAccess sright() { return SiblingNodeAccess(ImplBinaryNode::right()); }

   const PImplParentBinaryNode& parent() const { return pipbnParent; }
   PImplParentBinaryNode& parent() { return pipbnParent; }

   // methods called for the implementation of ImplBinaryTree
   bool changeSon(ImplParentBinaryNode* node, ImplParentBinaryNode* newNode);
   void setLeft(ImplParentBinaryNode* left);
   void setRight(ImplParentBinaryNode* right);

   ImplParentBinaryNode* removeLeft();
   ImplParentBinaryNode* removeRight();
   void disconnect();
   void disconnectSon(ImplParentBinaryNode* node);

   bool isSingleton() const { return ImplBinaryNode::isSingleton(); }

   static void swap(ImplParentBinaryNode* fst, ImplParentBinaryNode* snd,
         PImplBinaryNode& root);
   static void cswap(ImplParentBinaryNode* fst, ImplParentBinaryNode* snd);
   static void swapNew(ImplParentBinaryNode* fst, ImplParentBinaryNode* snd,
         PImplBinaryNode& root);

   static ImplParentBinaryNode* rotateRight(ImplParentBinaryNode* node);
   static ImplParentBinaryNode* rotateLeft(ImplParentBinaryNode* node);

  public:
   ImplParentBinaryNode() : pipbnParent(nullptr) {}
   ImplParentBinaryNode(const ImplParentBinaryNode& source)
      :  inherited(source), pipbnParent(nullptr) {}
   ImplParentBinaryNode& operator=(const ImplParentBinaryNode& source)
      {  return (ImplParentBinaryNode&) inherited::operator=(source); }

   bool isValid() const
      {  return ImplBinaryNode::isValid() && (!cleft() || (cleft()->pipbnParent == this))
            && (!cright() || (cright()->pipbnParent == this));
            // && (!pipbnParent || (pipbnParent->cleft() == this) || (pipbnParent->cright() == this));
            // suppressed for ternary trees 
      }
};

/* Definition of the inlined methods of class ImplParentBinaryNode */

inline void
ImplParentBinaryNode::setLeft(ImplParentBinaryNode* left) {
   ImplBinaryNode::setLeft(left);
   left->pipbnParent = this;
}

inline void
ImplParentBinaryNode::setRight(ImplParentBinaryNode* right) {
   ImplBinaryNode::setRight(right);
   right->pipbnParent = this;
}

inline ImplParentBinaryNode*
ImplParentBinaryNode::removeLeft() {
   ImplParentBinaryNode* result = (ImplParentBinaryNode*) ImplBinaryNode::removeLeft();
   if (cleft() != nullptr)
      cleft()->pipbnParent = this;
   result->pipbnParent = nullptr;
   return result;
}

inline ImplParentBinaryNode*
ImplParentBinaryNode::removeRight() {
   ImplParentBinaryNode* result = (ImplParentBinaryNode*) ImplBinaryNode::removeRight();
   if (cright() != nullptr)
      cright()->pipbnParent = this;
   result->pipbnParent = nullptr;
   return result;
}

inline void
ImplParentBinaryNode::disconnect() {
   if (ImplBinaryNode::left())
      cleft()->pipbnParent = nullptr;
   if (ImplBinaryNode::right())
      cright()->pipbnParent = nullptr;
   ImplBinaryNode::disconnect();
   pipbnParent = nullptr;
}

inline void
ImplParentBinaryNode::disconnectSon(ImplParentBinaryNode* node) {
   ImplBinaryNode::disconnectSon(node);
   node->pipbnParent = nullptr;
}

inline void
ImplParentBinaryNode::swapNew(ImplParentBinaryNode* fst, ImplParentBinaryNode* snd,
      PImplBinaryNode& root) {
   cswap(fst, snd);
   if (root == fst)
      root = snd;
}

inline void
ImplParentBinaryNode::swap(ImplParentBinaryNode* fst, ImplParentBinaryNode* snd,
      PImplBinaryNode& root) {
   cswap(fst, snd);
   if (root == fst)
      root = snd;
   else if (root == snd)
      root = fst;
}

inline ImplParentBinaryNode*
ImplParentBinaryNode::rotateRight(ImplParentBinaryNode* node) {
   ImplParentBinaryNode* result = (ImplParentBinaryNode*) inherited::rotateRight(node);
   result->pipbnParent = node->pipbnParent;
   if (result->pipbnParent) {
      if (!result->pipbnParent->ImplBinaryNode::changeSon(node, result))
         result->pipbnParent = nullptr;
   };
   node->pipbnParent = result;
   if (node->cleft())
      node->cleft()->pipbnParent = node;
   return result;
}

inline ImplParentBinaryNode*
ImplParentBinaryNode::rotateLeft(ImplParentBinaryNode* node) {
   ImplParentBinaryNode* result = (ImplParentBinaryNode*) inherited::rotateLeft(node);
   result->pipbnParent = node->pipbnParent;
   if (result->pipbnParent) {
      if (!result->pipbnParent->ImplBinaryNode::changeSon(node, result))
         result->pipbnParent = nullptr;
   };
   node->pipbnParent = result;
   if (node->cright())
      node->cright()->pipbnParent = node;
   return result;
}

/**************************************************/
/* Definition of class ImplParentBinaryTreeCursor */
/**************************************************/

class BinaryParentSuppressionNotification;
class BinaryParentReplaceNotification;

class ImplParentBinaryTreeCursor : public EnhancedObject {
  private:
   typedef EnhancedObject inherited;
   ImplParentBinaryNode* pipbnElement;

  protected:
   virtual ComparisonResult _compare(const EnhancedObject& asource) const override;

   void invalidate() { pipbnElement = nullptr;  }
   bool setToFirst(ImplParentBinaryNode* root);
   bool setToLast(ImplParentBinaryNode* root);
   bool setToNext(ImplParentBinaryNode* root);
   bool setToPrevious(ImplParentBinaryNode* root);

   bool setToLeft()
      {  AssumeCondition(pipbnElement)
         ImplParentBinaryNode* result = pipbnElement->cleft();
         return result ? (pipbnElement = result) != nullptr : false;
      }
   bool setToRight()
      {  AssumeCondition(pipbnElement)
         ImplParentBinaryNode* result = pipbnElement->cright();
         return result ? (pipbnElement = result) != nullptr : false;
      }
   bool setToSon(ImplParentBinaryNode* son)
      {  AssumeCondition(pipbnElement && son
            && ((pipbnElement->cleft() == son) || (pipbnElement->cright() == son)))
         pipbnElement = son;
         return true;
      }
   bool setToParent()
      {  AssumeCondition(pipbnElement)
         return ((pipbnElement = pipbnElement->parent()) != nullptr);
      }
   bool setToRoot(ImplParentBinaryNode* root)
      {  AssumeCondition(!root || !root->parent())
         return (pipbnElement = root) != nullptr;
      }

   bool isFirst() const;
   bool isLast() const;
   bool isNext(const ImplParentBinaryTreeCursor& source, bool isTreeEmpty) const;
   bool isPrevious(const ImplParentBinaryTreeCursor& source, bool isTreeEmpty) const
      { return source.isNext(*this, isTreeEmpty); }

   ImplParentBinaryNode* getParent() const { return pipbnElement->parent(); }
   ImplParentBinaryNode* getReference() const { return pipbnElement; }
   void setToReference(ImplParentBinaryNode* element)
      {  AssumeCondition(element) pipbnElement = element; }

   friend class BinaryParentSuppressionNotification;
   friend class BinaryParentReplaceNotification;

  public:
   ImplParentBinaryTreeCursor(ImplParentBinaryNode* element=nullptr)
      :  pipbnElement(element) {}
   ImplParentBinaryTreeCursor(const ImplParentBinaryTreeCursor& source) = default;
   DefineCopy(ImplParentBinaryTreeCursor)
   DDefineAssign(ImplParentBinaryTreeCursor)

   void swap(ImplParentBinaryTreeCursor& source)
      {  ImplParentBinaryNode* temp = pipbnElement;
         pipbnElement = source.pipbnElement;
         source.pipbnElement = temp;
      }

   virtual bool isValid() const override { return pipbnElement != nullptr; }
   int queryDepth() const;
};

template <class TypeNode>
class TImplParentBinaryTreeCursor : public ImplParentBinaryTreeCursor {
  protected:
   bool setToFirst(TypeNode* root) { return ImplParentBinaryTreeCursor::setToFirst(root); }
   bool setToLast(TypeNode* root) { return ImplParentBinaryTreeCursor::setToLast(root); }
   bool setToNext(TypeNode* root) { return ImplParentBinaryTreeCursor::setToNext(root); }
   bool setToPrevious(TypeNode* root) { return ImplParentBinaryTreeCursor::setToPrevious(root); }

   bool setToLeft() { return ImplParentBinaryTreeCursor::setToLeft(); }
   bool setToRight() { return ImplParentBinaryTreeCursor::setToRight(); }
   bool setToSon(TypeNode* node) { return ImplParentBinaryTreeCursor::setToSon(node); }
   bool setToParent() { return ImplParentBinaryTreeCursor::setToParent(); }
   bool setToRoot(TypeNode* node) { return ImplParentBinaryTreeCursor::setToRoot(node); }

   TypeNode* getParent() const { return (TypeNode*) ImplParentBinaryTreeCursor::getParent(); }
   TypeNode* getReference() const { return (TypeNode*) ImplParentBinaryTreeCursor::getReference(); }

  public:
   TImplParentBinaryTreeCursor(TypeNode* node=nullptr)
      : ImplParentBinaryTreeCursor(node) {}
   TImplParentBinaryTreeCursor(const TImplParentBinaryTreeCursor<TypeNode>& source) = default;
   TemplateDefineCopy(TImplParentBinaryTreeCursor, TypeNode)
};

class BinaryParentSuppressionNotification {
  private:
   ImplParentBinaryNode* pipbnSuppressNode;

  public:
   BinaryParentSuppressionNotification() : pipbnSuppressNode(nullptr) {}
   BinaryParentSuppressionNotification(const BinaryParentSuppressionNotification& source) = default;
   BinaryParentSuppressionNotification& operator=(const BinaryParentSuppressionNotification& bpsnSource) = default;
   void setStart(ImplParentBinaryNode* suppressNode)
      {  pipbnSuppressNode = suppressNode; }
   void apply(ImplParentBinaryTreeCursor& cursor) const
      {  if (pipbnSuppressNode == cursor.getReference())
            cursor.invalidate();
      }
   bool isValid() const { return pipbnSuppressNode != nullptr; }
};

class BinaryParentReplaceNotification {
  private:
   ImplParentBinaryNode* pipbnOldNode;
   ImplParentBinaryNode* pipbnNewNode;

  public:
   BinaryParentReplaceNotification() = default;
   BinaryParentReplaceNotification(const BinaryParentReplaceNotification& bprnSource) = default;
   BinaryParentReplaceNotification& operator=(const BinaryParentReplaceNotification& bprnSource) = default;
   void setStart(ImplParentBinaryNode* oldNode, ImplParentBinaryNode* newNode)
      {  pipbnOldNode = oldNode;
         pipbnNewNode = newNode;
      }
   void apply(ImplParentBinaryTreeCursor& cursor) const
      {  if (pipbnOldNode == cursor.getReference())
            cursor.setToReference(pipbnNewNode);
      }
   bool isValid() const { return (pipbnOldNode != nullptr) && (pipbnNewNode != nullptr); }
};

/**********************************************/
/* Definition of class ImplParentBalancedNode */
/**********************************************/

class ImplParentExtendedBinaryNode : public ImplParentBinaryNode, protected ExtendedParameters {
  public:
   ImplParentExtendedBinaryNode() {}
   ImplParentExtendedBinaryNode(const ImplParentExtendedBinaryNode& source)
      :  ImplParentBinaryNode(source) {}
   ImplParentExtendedBinaryNode& operator=(const ImplParentExtendedBinaryNode& source)
      {  return (ImplParentExtendedBinaryNode&) ImplParentBinaryNode::operator=(source); }
};

class ImplParentBalancedNode : public TBalancedNode<ImplParentExtendedBinaryNode> {
  private:
   typedef TBalancedNode<ImplParentExtendedBinaryNode> inherited;

  protected:
   friend class ImplParentBinaryTree;
   ImplParentBalancedNode* left() const { return (ImplParentBalancedNode*) inherited::left(); }
   ImplParentBalancedNode* right() const { return (ImplParentBalancedNode*) inherited::right(); }

   static void swap(ImplParentBalancedNode* fst, ImplParentBalancedNode* snd,
         PImplBinaryNode& root)
      {  inherited::swap(fst, snd, root);
         swapBalance(fst, snd);
      }
   static void swapNew(ImplParentBalancedNode* fst, ImplParentBalancedNode* snd,
         PImplBinaryNode& root)
      {  inherited::swapNew(fst, snd, root);
         swapBalance(fst, snd);
      }

  public:
   ImplParentBalancedNode() = default;
   ImplParentBalancedNode(const ImplParentBalancedNode& source) = default;
};

/********************************************/
/* Definition of class ImplParentBinaryTree */
/********************************************/

class CustomImplParentBinaryNode : public EnhancedObject, public ImplParentBalancedNode {
  protected:
   friend class ImplParentBinaryTree;

   static CustomImplParentBinaryNode* rotateRight(CustomImplParentBinaryNode* node, bool& doesPropagate)
      {  return (CustomImplParentBinaryNode*) ImplParentBalancedNode::rotateRight(node, doesPropagate); }
   static CustomImplParentBinaryNode* rotateLeft(CustomImplParentBinaryNode* node, bool& doesPropagate)
      {  return (CustomImplParentBinaryNode*) ImplParentBalancedNode::rotateLeft(node, doesPropagate); }
   static CustomImplParentBinaryNode* rotateLeftRight(CustomImplParentBinaryNode* node)
      {  return (CustomImplParentBinaryNode*) ImplParentBalancedNode::rotateLeftRight(node); }
   static CustomImplParentBinaryNode* rotateRightLeft(CustomImplParentBinaryNode* node)
      {  return (CustomImplParentBinaryNode*) ImplParentBalancedNode::rotateRightLeft(node); }
   CustomImplParentBinaryNode* parent() const
      { return (CustomImplParentBinaryNode*) ImplParentBalancedNode::parent(); }

  public:
   CustomImplParentBinaryNode() {}
   CustomImplParentBinaryNode(const CustomImplParentBinaryNode& source) = default;
   DefineCopy(CustomImplParentBinaryNode)
   DDefineAssign(CustomImplParentBinaryNode)

   virtual bool isValid() const override
      {  return EnhancedObject::isValid() && ImplParentBalancedNode::isValid(); }
};

#define DefineTypeNode CustomImplParentBinaryNode
#define DefineTypeBinaryTree ImplParentBinaryTree
#define DefineTypeBinaryParent
#define DefineTypeCast SimpleCast
#include "Pointer/Binary.inch"
#undef DefineTypeCast
#undef DefineTypeBinaryParent
#undef DefineTypeNode
#undef DefineTypeBinaryTree

}

#endif
