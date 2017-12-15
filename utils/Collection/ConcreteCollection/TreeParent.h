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
// File      : TreeParent.h
// Description :
//   Definition of the class GenericTreeParent. It is a tree class where each father
//     decides of the type of collection for its sons. Each sons contain a reference
//     to its father to improve the efficiency of some methods
//     like Cursor::gotoReference.
//

#ifndef COL_TreeParentH
#define COL_TreeParentH

#include "Collection/ConcreteCollection/TreeCommon.h"
#include "Collection/Implementation/ImplTreeParent.h"

namespace COL {

/***********************************************/
/* Definition of the template class TreeParent */
/***********************************************/

template<class TypeElement, class Cast=HandlerCast<TypeElement, TypeElement> >
class TreeParentCursor;

template<class Element, class Cast=HandlerCast<Element, Element> >
class TreeParent : public
   TGenericTree<typename Cast::Base,
                TGenericImplParentTreeCursor<typename Cast::Base::FlatCollection, typename Cast::Base::FlatNode>,
                TGenericImplParentTree<typename Cast::Base::FlatCollection, typename Cast::Base::FlatNode> > {
  private:
   typedef TreeParent<Element, Cast> thisType;
   typedef TGenericTree<typename Cast::Base,
                        TGenericImplParentTreeCursor<typename Cast::Base::FlatCollection, typename Cast::Base::FlatNode>,
                        TGenericImplParentTree<typename Cast::Base::FlatCollection, typename Cast::Base::FlatNode> >
      inherited;

  protected:
   typedef typename inherited::ExtendedTreeLocateParameters ExtendedTreeLocateParameters;
   typedef typename inherited::ExtendedTreeInsertionParameters ExtendedTreeInsertionParameters;
   typedef typename inherited::ExtendedTreeSuppressParameters ExtendedTreeSuppressParameters;
   typedef typename inherited::ExtendedTreeReplaceParameters ExtendedTreeReplaceParameters;
   virtual ComparisonResult _compare(const EnhancedObject& source) const override
      {  return inherited::_compare(source); }
   virtual EnhancedObject* _getElement(const typename inherited::ExtendedLocateParameters& parameters,
         const VirtualCollectionCursor* cursor=nullptr) const override
      {  return inherited::_getElement(parameters, cursor); }

  public:
   typedef typename inherited::AddMode AddMode;
   typedef typename inherited::RemoveMode RemoveMode;

   TreeParent() {}
   TreeParent(const thisType& source, AddMode mode=VirtualCollection::AMNoDuplicate,
         const VirtualCast* retrieveRegistrationFromCopy=nullptr)
      :  inherited(source, mode, retrieveRegistrationFromCopy) {}
   Template2DefineCopy(TreeParent, Element, Cast)
   Template2DefineCollectionForAbstractCollect(TreeParent, TreeParentCursor, Element, Cast)

#define DefTypeElement Element
#define DefTypeCollection thisType
#define DefTypeCastToCollectHandler Cast::castTo
#define DefTypeCCastToCollectHandler Cast::castTo
#define DefTypeCastFromCollectHandler(object) (Element*) Cast::castFrom((typename Cast::Base*) object)
#define DefTypeCCastFromCollectHandler(object) (const Element*) Cast::castFrom((const typename Cast::Base*) object)
#define DefTypeCursor TreeParentCursor<Element, Cast>
#define DefTypeFinal TreeParent<Element, Cast>::
   #include "Collection/ELMTreeCollection.inch"
#undef DefTypeFinal
#undef DefTypeCursor
#undef DefTypeCastToCollectHandler
#undef DefTypeCCastToCollectHandler
#undef DefTypeCastFromCollectHandler
#undef DefTypeCCastFromCollectHandler
#undef DefTypeCollection
#undef DefTypeElement

/*
   template <class Action>
      bool foreachDo(Action& action, const ExtendedTreeLocateParameters& parameters,
            const TreeParent<Element>::Cursor* start=nullptr,
            const TreeParent<Element>::Cursor* end=nullptr) const
      {  return GenericTreeParent::foreachSDo(ForeachTreeParentTemplateParameters<Element, SimpleCast>(),
            action, parameters, start, end);
      }
   template <class Action>
      bool foreachSubnodeDo(Action& action, const TreeParent<Element, Cast>::Cursor& cursor) const
      {  return GenericTreeParent::foreachSSubnodeDo(ForeachTreeParentTemplateParameters<Element, SimpleCast>(),
            action, cursor);
      }
*/
};

/*****************************************************/
/* Definition of the template class TreeParentCursor */
/*****************************************************/

template<class TypeElement, class Cast>
class TreeParentCursor : public TGenericTreeCursor<typename Cast::Base,
         TGenericImplParentTreeCursor<typename Cast::Base::FlatCollection, typename Cast::Base::FlatNode>,
         TGenericImplParentTree<typename Cast::Base::FlatCollection, typename Cast::Base::FlatNode> > {
   typedef TGenericTreeCursor<typename Cast::Base,
         TGenericImplParentTreeCursor<typename Cast::Base::FlatCollection, typename Cast::Base::FlatNode>,
         TGenericImplParentTree<typename Cast::Base::FlatCollection, typename Cast::Base::FlatNode> >
      inherited;
  protected:
   virtual void _gotoReference(const EnhancedObject& element) override
      {  inherited::getImplementation().gotoReference(
            (typename Cast::Base&) Cast::Base::castFromCopyHandler(element));
      }
   virtual EnhancedObject* _getSElement() const override { return inherited::_getSElement(); }
   virtual ComparisonResult _compare(const EnhancedObject& source) const override { return inherited::_compare(source); }

  public:
   TreeParentCursor(const TreeParent<TypeElement, Cast>& support) : inherited(support) {}
   TreeParentCursor(const TreeParentCursor<TypeElement, Cast>& source) : inherited(source) {}
   Template2DefineCopy(TreeParentCursor, TypeElement, Cast)
   Template2DefineCursorForAbstractCollect(TreeParent, TreeParentCursor, TypeElement, Cast)
   const typename Cast::Base::FlatCollection::Cursor& flatPart() const { return inherited::lastCursor(*this); }
   typename Cast::Base::FlatCollection::Cursor& flatPart() { return inherited::lastCursor(*this); }
   void updateMode() { inherited::updateMode(); }
   void updateModeNew()
      {  inherited::updateMode();
         inherited::getImplementation().updateElementFather();
      }

   #define DefCursor
   #define DefTypeCollection TreeParent<TypeElement, Cast>
   #define DefTypeCursor TreeParentCursor<TypeElement, Cast>
   #define DefTypeElement TypeElement
   #define DefTypeCastToCollectHandler Cast::castTo
   #define DefTypeCCastToCollectHandler Cast::castTo
   #define DefTypeCastFromCollectHandler(object) (TypeElement*) Cast::castFrom((typename Cast::Base*) object)
   #define DefTypeCCastFromCollectHandler(object) (const TypeElement*) Cast::castFrom((const typename Cast::Base*) object)
   #define DefTypeInheritedCollection
   #include "Collection/ELMTreeCollection.inch"
   #undef DefTypeInheritedCollection
   #undef DefTypeCastToCollectHandler
   #undef DefTypeCCastToCollectHandler
   #undef DefTypeCastFromCollectHandler
   #undef DefTypeCCastFromCollectHandler
   #undef DefTypeElement
   #undef DefTypeCollection
   #undef DefTypeCursor
   #undef DefCursor

   bool setToNext(typename inherited::Route route) {  return inherited::setToNext(route); }
   bool setToPrevious(typename inherited::Route route) {  return inherited::setToPrevious(route); }
};

Template2InlineCollectionForAbstractCollect(TreeParent, TreeParentCursor, Element, Cast)

} // end of namespace COL

#endif // COL_TreeParentH

