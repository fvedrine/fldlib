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
// File      : BasicList.h
// Description :
//   Definition of a list of elements inheriting from VirtualCollection.
//   No inheritance relationship is assumed on the elements.
//

#ifndef COL_BasicListH
#define COL_BasicListH

#include "Collection/ConcreteCollection/List.h"

namespace COL {

/*************************************/
/* Definition of the class BasicList */
/*************************************/

class BasicListCursor;
class BasicList : public GenericList {
  public:
   typedef EnhancedObject Node;
   typedef VirtualCollection::InitialValues InitialValues;
   typedef VirtualCollection::InitialNewValues InitialNewValues;

  protected:
   friend class BasicListCursor;
   class BasicListElement : public ImplListElement {
     private:
      EnhancedObject* peoElement;

     protected:
      virtual ComparisonResult _compare(const EnhancedObject& asource) const override
         {  ComparisonResult result = ImplListElement::_compare(asource);
            AssumeCondition(peoElement && ((const BasicListElement&) asource).peoElement)
            if (result == CREqual)
               result = peoElement->compare(*((const BasicListElement&) asource).peoElement);
            return result;
         }

     public:
      BasicListElement(EnhancedObject& element) : peoElement(&element) {}
      BasicListElement(const BasicListElement& source, bool doesDuplicate=false)
         :  ImplListElement(source), peoElement(source.peoElement)
         {  if (doesDuplicate && peoElement)
               peoElement = peoElement->createCopy();
         }
      DefineCopy(BasicListElement)
      DDefineAssign(BasicListElement)
      const PEnhancedObject& element() const { return peoElement; }
      const EnhancedObject& getElement() const { AssumeCondition(peoElement) return *peoElement; }
      void freeElement()
         {  AssumeCondition(peoElement)
            delete peoElement; peoElement = nullptr;
         }
      void releaseElement() { peoElement = nullptr; }
      void copyElement(const VirtualCast* retrieveRegistrationFromCopy=nullptr)
         {  AssumeCondition(peoElement)
            peoElement = retrieveRegistrationFromCopy
               ? ((const VirtualCastWithElement*) retrieveRegistrationFromCopy)
                  ->castFrom(peoElement->createCopy(), peoElement)
               : peoElement->createCopy();
         }
      void setElement(EnhancedObject& element)
         {  AssumeCondition(peoElement == nullptr) peoElement = &element; }
   };

   const PEnhancedObject& castToElement(EnhancedObject& basicElement) const
      { return ((BasicListElement&) basicElement).element(); }

  protected:
   void _fullAssign(const BasicList& source, const ExtendedReplaceParameters& parameters);
   virtual void _fullAssign(const VirtualCollection& source, const ExtendedReplaceParameters& parameters) override
      {  GenericList::_fullAssign(source, parameters); }

   // insertion methods
   virtual void _add(EnhancedObject* anObject, const ExtendedInsertionParameters& parameters,
         VirtualCollectionCursor* cursor) override
      { _add(anObject, parameters, (BasicListCursor*) cursor); }
   void _add(EnhancedObject* anObject, const ExtendedInsertionParameters& parameters,
         BasicListCursor* cursor=nullptr);
   virtual void _addAll(const VirtualCollection& source, const ExtendedInsertionParameters& parameters,
         VirtualCollectionCursor* cursor=nullptr, const VirtualCollectionCursor* startSource=nullptr,
         const VirtualCollectionCursor* endSource=nullptr) override
      {  GenericList::_addAll(source, parameters, cursor, startSource, endSource); }
   void _addAll(const BasicList& source, const ExtendedInsertionParameters& parameters,
         BasicListCursor* cursor=nullptr, const BasicListCursor* startSource=nullptr,
         const BasicListCursor* endSource=nullptr);

   // move methods => the same as the GenericList ones

   // suppression methods
   virtual void _remove(const ExtendedSuppressParameters& parameters, VirtualCollectionCursor* cursor) override
      {  _remove(parameters, (BasicListCursor*) cursor); }
   void _remove(const ExtendedSuppressParameters& parameters, BasicListCursor* cursor=nullptr);
   virtual void _removeAll(const ExtendedSuppressParameters& parameters,
         const VirtualCollectionCursor* start, const VirtualCollectionCursor* end=nullptr) override
      {  _removeAll(parameters, (const BasicListCursor*) start, (const BasicListCursor*) end); }
   void _removeAll(const ExtendedSuppressParameters& parameters,
         const BasicListCursor* start=nullptr, const BasicListCursor* end=nullptr);

   // replace methods
   virtual void _replace(EnhancedObject* anObject, const ExtendedReplaceParameters& parameters,
         VirtualCollectionCursor* cursor) override
      {  _replace(anObject, parameters, (BasicListCursor*) cursor); }
   void _replace(EnhancedObject* newObject, const ExtendedReplaceParameters& parameters,
         BasicListCursor* cursor=nullptr);

   // query methods
   virtual EnhancedObject* _getElement(const ExtendedLocateParameters& parameters,
         const VirtualCollectionCursor* cursor) const override
      {  return castToElement(*GenericList::_getElement(parameters, cursor)); }
   EnhancedObject* _getElement(const ExtendedLocateParameters& parameters,
         const BasicListCursor* cursor=nullptr) const
      {  return castToElement(*GenericList::_getElement(parameters, (GenericListCursor*) cursor)); }
   virtual void _gotoReference(const EnhancedObject& element) { AssumeUncalled }

  public:
   BasicList() {}
   BasicList(const InitialValues& initialValues)
      {  TInsertInitialValue<BasicList, EnhancedObject, SimpleCast> insertInitialValue(*this);
         initialValues.foreachDo(insertInitialValue);
      }
   BasicList(const InitialNewValues& initialValues)
      {  TInsertInitialNewValue<BasicList, EnhancedObject, SimpleCast> insertInitialValue(*this);
         initialValues.foreachDo(insertInitialValue);
      }
   BasicList(const BasicList& source, AddMode dupMode=AMNoDuplicate,
      const VirtualCast* retrieveRegistrationFromCopy=nullptr);

   DefineCopy(BasicList)
   BasicList& operator=(const BasicList& source)
      {  GenericList::operator=(source);
         GenericList::_fullAssign(source, ExtendedReplaceParameters().setDuplicate());
         return *this;
      }

   DefineCollectionForAbstractCollect(BasicList, BasicListCursor)

#define DefJustDeclare
#define DefTypeCollection BasicList
#define DefTypeCursor BasicListCursor
#define DefTypeFinal BasicList::
   #include "Collection/ELMCollection.inch"
#undef DefTypeCollection
#undef DefTypeCursor
#undef DefTypeFinal
#undef DefJustDeclare

   template <class Execute, class TemplateParameters>
   class ShiftExecute {
     private:
      Execute& eFunction;

     public:
      typedef typename TemplateParameters::Element Element;
      typedef typename TemplateParameters::Cast Cast;
      ShiftExecute(Execute& function) : eFunction(function) {}
      bool operator()(const BasicListElement& node) const
         {  return eFunction((Element&) Cast::castFrom(node.getElement())); }
   };

   template <class Execute, class TemplateParameters> bool foreachDo(TemplateParameters traits, Execute& function) const
      {  ShiftExecute<Execute, TemplateParameters> execute(function);
         return GenericList::foreachDo(TemplateElementCastParameters<BasicListElement, SimpleCast>(), execute);
      }
   template <class Execute, class TemplateParameters> bool foreachDo(TemplateParameters traits,
         Execute& function, const ExtendedLocateParameters& parameters,
         const Cursor* start, const Cursor* end) const
      {  ShiftExecute<Execute, TemplateParameters> execute(function);
         return GenericList::foreachDo(TemplateElementCastParameters<BasicListElement, SimpleCast>(),
               execute, parameters, start, end);
      }
   template <class Execute, class TemplateParameters> bool foreachReverseDo(TemplateParameters traits, Execute& function) const
      {  ShiftExecute<Execute, TemplateParameters> execute(function);
         return GenericList::foreachReverseDo(TemplateElementCastParameters<BasicListElement, SimpleCast>(), execute);
      }
   template <class Execute, class TemplateParameters> bool foreachReverseDo(TemplateParameters traits,
         Execute& function, const ExtendedLocateParameters& parameters,
         const Cursor* start, const Cursor* end) const
      {  ShiftExecute<Execute, TemplateParameters> execute(function);
         return GenericList::foreachReverseDo(TemplateElementCastParameters<BasicListElement, SimpleCast>(),
               execute, parameters, start, end);
      }
};

/*******************************************/
/* Definition of the class BasicListCursor */
/*******************************************/

class BasicListCursor : public GenericListCursor {
  protected:
   BasicList::BasicListElement* element() const
      {  return (BasicList::BasicListElement*) GenericListCursor::element(); }
   void setElement(BasicList::BasicListElement* element)
      {  GenericListCursor::setElement(element); }

   virtual EnhancedObject* _getSElement() const override { return element()->element(); }

  public:
   BasicListCursor(const BasicList& support) : GenericListCursor(support) {}
   BasicListCursor(const BasicListCursor& source) : GenericListCursor(source) {}
   DefineCopy(BasicListCursor)
   DefineCursorForAbstractCollect(BasicList, BasicListCursor)
};

InlineCollectionForAbstractCollect(BasicList, BasicListCursor)

#define DefJustInline
#define DefTypeCollection BasicList
#define DefTypeCursor BasicListCursor
#define DefTypeFinal BasicList::
   #include "Collection/ELMCollection.inch"
#undef DefTypeCollection
#undef DefTypeCursor
#undef DefTypeFinal
#undef DefJustInline

/***********************************************/
/* Definition of the template class TBasicList */
/***********************************************/

template<class TypeElement, class Cast=HandlerCopyCast<TypeElement> >
class TBasicListCursor;

template<class Element, class Cast=HandlerCopyCast<Element> >
class TBasicList : public BasicList {
  public:
   typedef Element Node;
   typedef TInitialValues<Element, Cast> InitialValues;
   typedef TInitialNewValues<Element, Cast> InitialNewValues;

   TBasicList() {}
   TBasicList(const InitialValues& initialValues)
      {  VirtualCollection::TInsertInitialValue<TBasicList<Element, Cast>, Element, Cast>
            insertInitialValue(*this);
         initialValues.foreachDo(insertInitialValue);
      }
   TBasicList(const InitialNewValues& initialValues)
      {  VirtualCollection::TInsertInitialNewValue<TBasicList<Element, Cast>, Element, Cast>
            insertInitialValue(*this);
         initialValues.foreachDo(insertInitialValue);
      }
   TBasicList(const TBasicList<Element, Cast>& source, AddMode dupMode=AMNoDuplicate)
      :  BasicList(source, dupMode) {}
   Template2DefineCopy(TBasicList, Element, Cast)
   Template2DefineCollectionForAbstractCollect(TBasicList, TBasicListCursor, Element, Cast)

#define DefTypeElement Element
#define DefTypeCollection TBasicList<Element, Cast>
#define DefTypeCastToCollectHandler (EnhancedObject*) Cast::castTo
#define DefTypeCCastToCollectHandler (const EnhancedObject*) Cast::castTo
#define DefTypeCastFromCollectHandler (Element*) Cast::castFrom
#define DefTypeCCastFromCollectHandler (const Element*) Cast::castFrom
#define DefTypeInheritedCollection BasicList
#define DefTypeCursor TBasicListCursor<Element, Cast>
   #include "Collection/ELMCollection.inch"
#undef DefTypeElement
#undef DefTypeCollection
#undef DefTypeCastToCollectHandler
#undef DefTypeCCastToCollectHandler
#undef DefTypeCastFromCollectHandler
#undef DefTypeCCastFromCollectHandler
#undef DefTypeInheritedCollection
#undef DefTypeCursor

   TVirtualCollection<Element, Cast>& getInterface()
      {  return (TVirtualCollection<Element, Cast>&) (VirtualCollection&) *this; }
   const TVirtualCollection<Element, Cast>& getInterface() const
      {  return (const TVirtualCollection<Element, Cast>&) (const VirtualCollection&) *this; }

   bool foreachDo(std::function<bool (Element&)> function) const
      {  return BasicList::foreachDo(TemplateElementCastParameters<Element, Cast>(), function); }
   bool foreachSDo(std::function<bool (const Element&)> function) const
      {  return BasicList::foreachDo(TemplateElementCastParameters<Element, Cast>(), function); }
   template <class Execute> bool foreachDo(Execute& function) const
      {  return BasicList::foreachDo(TemplateElementCastParameters<Element, Cast>(), function); }
   template <class Execute> bool foreachDo(Execute& function, const ExtendedLocateParameters& parameters,
         const Cursor* start, const Cursor* end) const
      {  return BasicList::foreachDo(TemplateElementCastParameters<Element, Cast>(), function,
            parameters, start, end);
      }
   bool foreachReverseDo(std::function<bool (Element&)> function) const
      {  return BasicList::foreachReverseDo(TemplateElementCastParameters<Element, Cast>(), function); }
   bool foreachSReverseDo(std::function<bool (const Element&)> function) const
      {  return BasicList::foreachReverseDo(TemplateElementCastParameters<Element, Cast>(), function); }
   template <class Execute> bool foreachReverseDo(Execute& function) const
      { return BasicList::foreachReverseDo(TemplateElementCastParameters<Element, Cast>(), function); }
   template <class Execute> bool foreachReverseDo(Execute& function, const ExtendedLocateParameters& parameters,
         const Cursor* start, const Cursor* end) const
      {  return BasicList::foreachReverseDo(TemplateElementCastParameters<Element, Cast>(), function,
            parameters, start, end);
      }
   DeclareCollectionIteratorForConcreteCollect
};

/*****************************************************/
/* Definition of the template class TBasicListCursor */
/*****************************************************/

template<class TypeElement, class Cast>
class TBasicListCursor : public BasicListCursor {
  public:
   TBasicListCursor(const TBasicList<TypeElement, Cast>& support) : BasicListCursor(support) {}
   TBasicListCursor(const TBasicListCursor<TypeElement, Cast>& source) : BasicListCursor(source) {}
   Template2DefineCopy(TBasicListCursor, TypeElement, Cast)
   Template2DefineCursorForAbstractCollect(TBasicList, TBasicListCursor, TypeElement, Cast)

   TypeElement* getSElement() const { return (TypeElement*) Cast::castFrom(BasicListCursor::_getSElement()); }
   const TypeElement* getElement() const { return getSElement(); }
   const TypeElement& elementAt() const { return *getSElement(); }
   TypeElement& elementSAt() const { return *getSElement(); }
};

Template2InlineCollectionForAbstractCollect(TBasicList, TBasicListCursor, TypeElement, Cast)
TemplateInlineCollectionIteratorForConcreteCollect(TBasicList, TypeElement, Cast)

} // end of namespace

#endif // COL_BasicListH
