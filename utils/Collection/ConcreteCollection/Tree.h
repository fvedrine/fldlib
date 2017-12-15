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
// File      : Tree.h
// Description :
//   Definition of the class GenericTree. It is a tree class where each father
//     decides of the type of collection for its sons.
//

#ifndef COL_TreeH
#define COL_TreeH

#include "Collection/ConcreteCollection/TreeCommon.h"
#include "Collection/Implementation/ImplTree.h"

namespace COL {

/***************************************/
/* Definition of the class GenericTree */
/***************************************/

class GenericTreeCursor;
class GenericTree : public TGenericTree<GenericTreeElement, GenericImplTreeCursor, GenericImplTree> {
  private:
   typedef TGenericTree<GenericTreeElement, GenericImplTreeCursor, GenericImplTree> inherited;
  public:
   template <class TemplateCollectionElementKeyCast>
   class KeySortedAccess
      : public DTree::GenericTree::KeySortedAccess<GenericTree, TemplateCollectionElementKeyCast> {};

  public:
   GenericTree() {}
   GenericTree(const GenericTree& source, AddMode mode=AMNoDuplicate,
         const VirtualCast* retrieveRegistrationFromCopy=nullptr)
      :  inherited (source, mode, retrieveRegistrationFromCopy) {}

   DefineCopy(GenericTree)
   DefineCollectionForAbstractCollect(GenericTree, GenericTreeCursor)

   #define DefJustDeclare
   #define DefTypeCollection GenericTree
   #define DefTypeCursor GenericTreeCursor
   #define DefTypeElement GenericTreeElement
   #include "Collection/ELMTreeCollection.inch"
   #undef DefTypeElement
   #undef DefTypeCursor
   #undef DefTypeCollection
   #undef DefJustDeclare
};

/*********************************************/
/* Definition of the class GenericTreeCursor */
/*********************************************/

class GenericTreeCursor : public TGenericTreeCursor<GenericTreeElement, GenericImplTreeCursor, GenericImplTree> {
  private:
   typedef TGenericTreeCursor<GenericTreeElement, COL::GenericImplTreeCursor, GenericImplTree> inherited;

  public:
   GenericTreeCursor(const GenericTree& support) : inherited(support) {}
   GenericTreeCursor(const GenericTreeCursor& source) : inherited(source) {}
   DefineCopy(GenericTreeCursor)
   DefineCursorForAbstractCollect(GenericTree, GenericTreeCursor)
   const VirtualCollection::Cursor& flatPart() const { return lastCursor(*this); }
   VirtualCollection::Cursor& flatPart() { return lastCursor(*this); }

   #define DefCursor
   #define DefTypeCollection GenericTree
   #define DefTypeCursor GenericTreeCursor
   #define DefTypeElement GenericTreeElement
   #define DefTypeInheritedCollection
   #include "Collection/ELMTreeCollection.inch"
   #undef DefTypeInheritedCollection
   #undef DefTypeElement
   #undef DefTypeCollection
   #undef DefTypeCursor
   #undef DefCursor
};

InlineCollectionForAbstractCollect(GenericTree, GenericTreeCursor)

#define DefJustInline
#define DefTypeCollection GenericTree
#define DefTypeCursor GenericTreeCursor
#define DefTypeElement GenericTreeElement
#define DefTypeInheritedCollection inherited
#define DefTypeCastToCollectHandler
#define DefTypeCCastToCollectHandler
#define DefTypeCastFromCollectHandler
#define DefTypeCCastFromCollectHandler
#include "Collection/ELMTreeCollection.inch"
#undef DefTypeCastToCollectHandler
#undef DefTypeCCastToCollectHandler
#undef DefTypeCastFromCollectHandler
#undef DefTypeCCastFromCollectHandler
#undef DefTypeInheritedCollection
#undef DefTypeElement
#undef DefTypeCursor
#undef DefTypeCollection
#undef DefJustInline

/*****************************************/
/* Definition of the template class Tree */
/*****************************************/

class SimpleTreeCast {
  public:
   typedef GenericTreeElement Base;
   static GenericTreeElement* castFrom(EnhancedObject* source)
      {  return (GenericTreeElement*) source; }
   static const GenericTreeElement* castFrom(const EnhancedObject* source)
      {  return (const GenericTreeElement*) source; }
   static GenericTreeElement& castFrom(EnhancedObject& source)
      {  return (GenericTreeElement&) source; }
   static const GenericTreeElement& castFrom(const EnhancedObject& source)
      {  return (const GenericTreeElement&) source; }

   static GenericTreeElement* castFrom(GenericTreeElement* source) { return source; }
   static const GenericTreeElement* castFrom(const GenericTreeElement* source) { return source; }
   static GenericTreeElement& castFrom(GenericTreeElement& source) { return source; }
   static const GenericTreeElement& castFrom(const GenericTreeElement& source) { return source; }

   static GenericTreeElement* castTo(GenericTreeElement* source) { return source; }
   static const GenericTreeElement* castTo(const GenericTreeElement* source) { return source; }
   static GenericTreeElement& castTo(GenericTreeElement& source) { return source; }
   static const GenericTreeElement& castTo(const GenericTreeElement& source) { return source; }
};

template<class Element, class Cast=SimpleTreeCast>
class Tree;
template<class TypeElement, class Cast=SimpleTreeCast>
class TreeCursor;

template<class TypeElement, class TypeCast>
class ForeachTreeTemplateParameters {
  public:
   typedef TypeElement Element;
   typedef COL::Tree<TypeElement, TypeCast> Tree;
};

template<class Element, class Cast>
class Tree : public GenericTree {
  private:
   typedef Tree<Element, Cast> ThisType;

  public:
   Tree() {}
   Tree(const Tree<Element, Cast>& source, AddMode mode=AMNoDuplicate,
         const VirtualCast* retrieveRegistrationFromCopy=nullptr)
      :  GenericTree(source, mode, retrieveRegistrationFromCopy) {}
   Template2DefineCopy(Tree, Element, Cast)
   Template2DefineCollectionForAbstractCollect(Tree, TreeCursor, Element, Cast)

#define DefTypeElement Element
#define DefTypeCollection Tree<Element, Cast>
#define DefTypeCastToCollectHandler Cast::castTo
#define DefTypeCCastToCollectHandler Cast::castTo
#define DefTypeCastFromCollectHandler(object) (Element*) Cast::castFrom((typename Cast::Base*) object)
#define DefTypeCCastFromCollectHandler(object) (const Element*) Cast::castFrom((const typename Cast::Base*) object)
#define DefTypeCursor TreeCursor<Element, Cast>
#define DefTypeFinal Tree<Element, Cast>::
   #include "Collection/ELMTreeCollection.inch"
#undef DefTypeFinal
#undef DefTypeCursor
#undef DefTypeCastToCollectHandler
#undef DefTypeCCastToCollectHandler
#undef DefTypeCastFromCollectHandler
#undef DefTypeCCastFromCollectHandler
#undef DefTypeCollection
#undef DefTypeElement

   const typename Cast::Base::FlatCollection::Cursor& flatPart() const { return GenericTree::lastCursor(*this); }
   typename Cast::Base::FlatCollection::Cursor& flatPart() { return GenericTree::lastCursor(*this); }
   
   template <class Action>
      bool foreachDo(Action& action, const ExtendedTreeLocateParameters& parameters,
            const typename Tree<Element, Cast>::Cursor* start=nullptr, const typename Tree<Element, Cast>::Cursor* end=nullptr) const
      {  return GenericTree::tforeachSDo(ForeachTreeTemplateParameters<Element, Cast>(),
            action, parameters, start, end);
      }
   template <class Action>
      bool foreachDo(Action& action) const
      {  return GenericTree::tforeachSDo(ForeachTreeTemplateParameters<Element, Cast>(), action); }
   template <class Action>
      bool foreachSubnodeDo(Action& action, const typename Tree<Element, Cast>::Cursor& cursor) const
      {  return GenericTree::tforeachSSubnodeDo(ForeachTreeTemplateParameters<Element, Cast>(),
            action, cursor);
      }
   template <class Action>
      bool foreachReverseDo(Action& action, const ExtendedTreeLocateParameters& parameters,
            const typename Tree<Element, Cast>::Cursor* start=nullptr, const typename Tree<Element, Cast>::Cursor* end=nullptr) const
      {  return GenericTree::tforeachSReverseDo(ForeachTreeTemplateParameters<Element, Cast>(),
            action, parameters, start, end);
      }
   template <class Action>
      bool foreachReverseDo(Action& action) const
      {  return GenericTree::tforeachSReverseDo(ForeachTreeTemplateParameters<Element, Cast>(), action); }
   template <class Action>
      bool foreachSubnodeReverseDo(Action& action, const typename Tree<Element, Cast>::Cursor& cursor) const
      {  return GenericTree::tforeachSSubnodeReverseDo(ForeachTreeTemplateParameters<Element, Cast>(),
            action, cursor);
      }
};

} // end of namespace COL

namespace COL {

/***********************************************/
/* Definition of the template class TreeCursor */
/***********************************************/

template<class TypeElement, class Cast>
class TreeCursor : public GenericTreeCursor {
  public:
   TreeCursor(const COL::Tree<TypeElement, Cast>& support) : GenericTreeCursor(support) {}
   TreeCursor(const TreeCursor<TypeElement, Cast>& source) : GenericTreeCursor(source) {}
   Template2DefineCopy(TreeCursor, TypeElement, Cast)
   Template2DefineCursorForAbstractCollect(COL::Tree, TreeCursor, TypeElement, Cast)

   TypeElement* getSElement() const
      { return (TypeElement*) Cast::castFrom((GenericTreeElement*) GenericTreeCursor::_getSElement()); }
   const TypeElement* getElement() const { return getSElement(); }
   const TypeElement& elementAt() const { return *getSElement(); }
   TypeElement& elementSAt() const { return *getSElement(); }
   
   void updateMode() { inherited::updateMode(); }
};

Template2InlineCollectionForAbstractCollect(COL::Tree, TreeCursor, Element, Cast)

} // end of namespace COL

#endif
