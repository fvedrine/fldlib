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
// File      : List.h
// Description :
//   Definition of a list of double linked elements inheriting from VirtualCollection.
//

#ifndef COL_ListH
#define COL_ListH

#include "Collection/VirtualCollection/VirtualCollection.h"
#include "Pointer/ImplList.h"

namespace COL {

/***************************************/
/* Definition of the class GenericList */
/***************************************/

class GenericListCursor;
class GenericList : public VirtualCollection, private ImplList {
  private:
   int uCount;

   typedef ImplListElement* PImplListElement;
   ImplListElement* getElement(ImplListElement* element, RelativePosition position) const;
   bool retrieveRemovedInterval(PImplListElement& start, PImplListElement& end,
         RelativePosition pos) const;

  protected:
   // notification classes
   class ElementNotification;
   class BoundNotification;
   class RemoveElementNotification;
   class RemoveBoundNotification;
   class ChangeNotification;

   friend class ListCursorNotification;
   class ListCursorNotification;

   // methods to perform simple applications on an interval of elements
   template <class Execute, class TemplateParameters> bool foreachDo(TemplateParameters traits,
         Execute& function, ImplListElement* prevStart, ImplListElement* nextEnd) const
      {  return ImplList::foreachDo(traits, function, prevStart, nextEnd); }
   template <class Execute, class TemplateParameters> bool foreachReverseDo(TemplateParameters traits,
         Execute& function, ImplListElement* prevStart, ImplListElement* nextEnd) const
      {  return ImplList::foreachReverseDo(traits, function, prevStart, nextEnd); }
   void retrieveBoundInsertion(const ExtendedInsertionParameters& parameters,
         const GenericListCursor* cursor, PImplListElement& prevStart, PImplListElement& nextEnd) const;
   void retrieveBoundSuppression(const ExtendedSuppressParameters& parameters,
         const GenericListCursor* startCursor, const GenericListCursor* endCursor,
         PImplListElement& prevStart, PImplListElement& nextEnd) const;

  protected:
   virtual ComparisonResult _compare(const EnhancedObject& asource) const override
      {  return VirtualCollection::_compare(asource); }

   virtual void _fullAssign(const VirtualCollection& source, const ExtendedReplaceParameters& parameters) override
      {  VirtualCollection::pfullAssign(source, parameters); }
   void _fullAssign(const GenericList& source, const ExtendedReplaceParameters& parameters);

   // insertion methods
   virtual void _add(EnhancedObject* anObject, const ExtendedInsertionParameters& parameters,
         VirtualCollectionCursor* cursor) override
      {  _add((ImplListElement*) anObject, parameters, (GenericListCursor*) cursor); }
   void _add(EnhancedObject* anObject, const ExtendedInsertionParameters& parameters,
         GenericListCursor* cursor=nullptr)
      {  _add((ImplListElement*) anObject, parameters, cursor); }
   void _add(ImplListElement* newElement, const ExtendedInsertionParameters& parameters,
         GenericListCursor* cursor=nullptr);
   virtual void _addAll(const VirtualCollection& source, const ExtendedInsertionParameters& parameters,
         VirtualCollectionCursor* cursor=nullptr, const VirtualCollectionCursor* startSource=nullptr,
         const VirtualCollectionCursor* endSource=nullptr) override
      {  VirtualCollection::paddAll(source, parameters, cursor, startSource, endSource); }
   void _addAll(const GenericList& source, const ExtendedInsertionParameters& parameters,
         GenericListCursor* cursor=nullptr, const GenericListCursor* startSource=nullptr,
         const GenericListCursor* endSource=nullptr);

   // move methods
   virtual void _moveTo(VirtualCollection& destination, const ExtendedReplaceParameters& parameters,
         VirtualCollectionCursor* cursor=nullptr, VirtualCollectionCursor* destinationCursor=nullptr) override;
   virtual void _moveAllTo(VirtualCollection& destination, const ExtendedReplaceParameters& parameters,
         const VirtualCollectionCursor* startCursor=nullptr, const VirtualCollectionCursor* endCursor=nullptr,
         VirtualCollectionCursor* destinationCursor=nullptr) override;
   void _moveAllTo(GenericList& destination, const ExtendedReplaceParameters& parameters,
         const GenericListCursor* startCursor=nullptr, const GenericListCursor* endCursor=nullptr,
         GenericListCursor* destinationCursor=nullptr);
   virtual void _swap(VirtualCollection& source) override;
   void _swap(GenericList& source);

   // suppression methods
   virtual void _remove(const ExtendedSuppressParameters& parameters, VirtualCollectionCursor* cursor) override
      {  _remove(parameters, (GenericListCursor*) cursor); }
   void _remove(const ExtendedSuppressParameters& parameters, GenericListCursor* cursor=nullptr);
   virtual void _removeAll(const ExtendedSuppressParameters& parameters,
         const VirtualCollectionCursor* start, const VirtualCollectionCursor* end=nullptr) override
      {  _removeAll(parameters, (GenericListCursor*) start, (GenericListCursor*) end); }
   void _removeAll(const ExtendedSuppressParameters& parameters)
      {  _removeAll(parameters, (const GenericListCursor*) nullptr); }
   void _removeAll(const ExtendedSuppressParameters& parameters,
         const GenericListCursor* start, const GenericListCursor* end=nullptr);

   // replacement methods
   virtual void _replace(EnhancedObject* anObject, const ExtendedReplaceParameters& parameters,
         VirtualCollectionCursor* cursor=nullptr) override
      {  _replace((ImplListElement*) anObject, parameters, (GenericListCursor*) cursor); }
   void _replace(ImplListElement* newElement, const ExtendedReplaceParameters& parameters,
         GenericListCursor* cursor=nullptr);

   // query methods
   virtual int _queryCount(const ExtendedLocateParameters& parameters,
      const VirtualCollectionCursor* start=nullptr, const VirtualCollectionCursor* end=nullptr) const override
      {  return (!start && !end) ? uCount
            : VirtualCollection::pqueryCount(parameters, start, end);
      }
   virtual EnhancedObject* _getElement(const ExtendedLocateParameters& parameters,
         const VirtualCollectionCursor* cursor) const override
      {  return _getElement(parameters, (const GenericListCursor*) cursor); }
   EnhancedObject* _getElement(const ExtendedLocateParameters& parameters,
         const GenericListCursor* cursor=nullptr) const;

   friend class GenericListCursor;
   const ImplList& getImplementation() const { return (const ImplList&) *this; }

   GenericList() : uCount(0) {}
   GenericList(const GenericList& source, AddMode dupMode=AMNoDuplicate,
         const VirtualCast* retrieveRegistrationFromCopy=nullptr)
      :  VirtualCollection(source, dupMode), uCount(0)
      {  try {
            ImplList::assign((const ImplList&) source, true, retrieveRegistrationFromCopy);
            uCount = source.uCount;
         }
         catch (...) {
            ImplList::freeAll();
            uCount = 0;
            throw;
         };
      }

  public:
   typedef VirtualCollection::VirtualCastWithElement VirtualCastWithElement;
   DefineCopy(GenericList)
   DDefineAssign(GenericList)
   DCompare(GenericList)
   StaticInheritConversions(GenericList, VirtualCollection)
   DefineCollectionForAbstractCollect(GenericList, GenericListCursor)
   GenericList& operator=(const GenericList& source)
      {  return (GenericList&) VirtualCollection::operator=(source); }
   bool isEmpty() const { return ImplList::isEmpty(); }

   template <class Execute, class TemplateParameters> bool foreachDo(TemplateParameters traits, Execute& function) const
      {  return ImplList::foreachDo(traits, function); }
   template <class Execute, class TemplateParameters> bool foreachDo(TemplateParameters traits,
         Execute& function, const ExtendedLocateParameters& parameters,
         const Cursor* start, const Cursor* end) const;
   template <class Execute, class TemplateParameters> bool foreachReverseDo(TemplateParameters traits, Execute& function) const
      {  return ImplList::foreachReverseDo(traits, function); }
   template <class Execute, class TemplateParameters> bool foreachReverseDo(TemplateParameters traits,
         Execute& function, const ExtendedLocateParameters& parameters,
         const Cursor* start, const Cursor* end) const;
   virtual bool isValid() const override { return ImplList::isValid(); }
};

/*********************************************/
/* Definition of the class GenericListCursor */
/*********************************************/

class GenericListCursor : public VirtualCollectionCursor {
  public:
   typedef COL::ImplListElement ImplListElement;

  private:
   typedef ImplListElement* PImplListElement;
   ImplListElement* pileElement;

  protected:
   friend class GenericList::ListCursorNotification;
   const PImplListElement& element() const { return pileElement; }
   void setElement(ImplListElement* element) { pileElement = element; }

   virtual ComparisonResult _compare(const EnhancedObject& asource) const override;
   virtual bool _isPositionned(const ExtendedLocateParameters& pos,
         const VirtualCollectionCursor* cursor=nullptr) const override;
   virtual bool _position(const Position& pos) override;
   virtual EnhancedObject* _getSElement() const override
      {  AssumeCondition(pileElement) return pileElement; }
   virtual bool _isEqual(const AbstractCursor& cursor) const override;
   virtual void _gotoReference(const EnhancedObject& element) override
      {  setElement((ImplListElement*) &element); }

  public:
   GenericListCursor(const GenericList& support)
      :  VirtualCollectionCursor(support), pileElement(nullptr) {}
   GenericListCursor(const GenericListCursor& source) = default;
   DefineCopy(GenericListCursor)
   DDefineAssign(GenericListCursor)
   DefineCursorForAbstractCollect(GenericList, GenericListCursor)
   void swap(GenericListCursor& source)
      {  VirtualCollectionCursor::swap(source);
         auto temp = pileElement;
         pileElement = source.pileElement;
         source.pileElement = temp;
      }

   bool isEqual(const GenericListCursor& cursor) const
      {  return pileElement == cursor.pileElement; }
   virtual bool isValid() const override { return pileElement != nullptr; }

   GenericListCursor& operator+=(int add)
      {  return (GenericListCursor&) VirtualCollectionCursor::operator+=(add); }
   GenericListCursor& operator-=(int add)
      {  return (GenericListCursor&) VirtualCollectionCursor::operator-=(add); }
};

inline bool
GenericListCursor::_isEqual(const AbstractCursor& cursor) const
   {  return VirtualCollectionCursor::_isEqual(cursor)
         && (pileElement == ((const GenericListCursor&) cursor).pileElement);
   }

InlineCollectionForAbstractCollect(GenericList, GenericListCursor)

template <class Execute, class TemplateParameters>
inline bool
GenericList::foreachDo(TemplateParameters traits,
      Execute& function, const ExtendedLocateParameters& parameters,
      const Cursor* start, const Cursor* end) const {
   ImplListElement* startElement = start ? start->element() : nullptr;
   bool isSolved = false;
   if (parameters.isFirstExcluded() && startElement) {
      startElement = ImplList::getNext(startElement);
      if (startElement == ImplList::getFirst())
         isSolved = true;
   };
   ImplListElement* endElement = end ? end->element() : nullptr;
   if (parameters.isLastExcluded() && endElement) {
      endElement = ImplList::getPrevious(endElement);
      if (endElement == ImplList::getLast())
         isSolved = true;
   };
   return isSolved ? true : ImplList::foreachDo(traits, function, startElement, endElement);
}

template <class Execute, class TemplateParameters>
inline bool
GenericList::foreachReverseDo(TemplateParameters traits,
      Execute& function, const ExtendedLocateParameters& parameters,
      const Cursor* start, const Cursor* end) const {
   ImplListElement* startElement = start ? start->element() : nullptr;
   bool isSolved = false;
   if (parameters.isFirstExcluded() && startElement) {
      startElement = ImplList::getPrevious(startElement);
      if (startElement == ImplList::getLast())
         isSolved = true;
   };
   ImplListElement* endElement = end ? end->element() : nullptr;
   if (parameters.isLastExcluded() && endElement) {
      endElement = ImplList::getNext(endElement);
      if (endElement == ImplList::getFirst())
         isSolved = true;
   };
   return isSolved ? true : ImplList::foreachReverseDo(traits, function, startElement, endElement);
}

#include "Collection/ConcreteCollection/List.inline"

/********************************/
/* Definition of the class List */
/********************************/

class ListCursor;
class List : public GenericList {
  public:
   typedef ImplListElement Node;
   typedef TInitialValues<Node, SimpleCast> InitialValues;
   typedef TInitialNewValues<Node, SimpleCast> InitialNewValues;

  protected:
   virtual bool acceptElement(const EnhancedObject& asource) const override
      {  return dynamic_cast<const Node*>(&asource);  }

  public:
   List() {}
   List(const InitialValues& initialValues)
      {  TInsertInitialValue<List, Node, SimpleCast> insertInitialValue(*this);
         initialValues.foreachDo(insertInitialValue);
      }
   List(const InitialNewValues& initialValues)
      {  TInsertInitialNewValue<List, Node, SimpleCast> insertInitialValue(*this);
         initialValues.foreachDo(insertInitialValue);
      }
   List(const List& source, AddMode dupMode=AMNoDuplicate,
         const VirtualCast* retrieveRegistrationFromCopy=nullptr)
      : GenericList(source, dupMode, retrieveRegistrationFromCopy) {}
   DefineCopy(List)
   DDefineAssign(List)
   DefineCollectionForAbstractCollect(List, ListCursor)
   List& operator=(const List& source)
      {  VirtualCollection::operator=(source);
         if (this != &source)
            _removeAll(ExtendedSuppressParameters());
         return *this;
      }

#define DefJustDeclare
#define DefTypeCollection List
#define DefTypeCursor ListCursor
#define DefTypeFinal List::
#define DefTypeElement ImplListElement
   #include "Collection/ELMCollection.inch"
#undef DefTypeElement
#undef DefTypeCollection
#undef DefTypeCursor
#undef DefTypeFinal
#undef DefJustDeclare
};

/**************************************/
/* Definition of the class ListCursor */
/**************************************/

class ListCursor : public GenericListCursor {
  protected:
   virtual EnhancedObject* _getSElement() const override
      {  return GenericListCursor::_getSElement(); }

  public:
   ListCursor(const List& support) : GenericListCursor(support) {}
   ListCursor(const ListCursor& source) : GenericListCursor(source) {}
   DefineCopy(ListCursor)
   DefineCursorForAbstractCollect(List, ListCursor)

#define DefCursor
#define DefTypeCollection List
#define DefTypeCursor ListCursor
#define DefTypeFinal ListCursor::
#define DefTypeElement ImplListElement
   #include "Collection/ELMCollection.inch"
#undef DefTypeElement
#undef DefTypeCollection
#undef DefTypeCursor
#undef DefTypeFinal
#undef DefCursor
};

InlineCollectionForAbstractCollect(List, ListCursor)

#define DefJustInline
#define DefTypeCollection List
#define DefTypeCursor ListCursor
#define DefTypeFinal List::
#define DefTypeElement ImplListElement
   #include "Collection/ELMCollection.inch"
#undef DefTypeElement
#undef DefTypeCollection
#undef DefTypeCursor
#undef DefTypeFinal
#undef DefJustInline

/******************************************/
/* Definition of the template class TList */
/******************************************/

template<class TypeElement, class Cast>
class TListCursor;

class SimpleListCast {
  public:
   typedef ImplListElement Base;
   static ImplListElement* castFrom(ImplListElement* source) { return source; }
   static const ImplListElement* castFrom(const ImplListElement* source) { return source; }
   static ImplListElement& castFrom(ImplListElement& source) { return source; }
   static const ImplListElement& castFrom(const ImplListElement& source) { return source; }

   static ImplListElement* castTo(ImplListElement* source) { return source; }
   static const ImplListElement* castTo(const ImplListElement* source) { return source; }
   static ImplListElement& castTo(ImplListElement& source) { return source; }
   static const ImplListElement& castTo(const ImplListElement& source) { return source; }
};

template<class ListElement, class ListCast=HandlerCast<ListElement, ImplListElement> >
class TList : public List {
  public:
   typedef TInitialValues<ListElement, ListCast> InitialValues;
   typedef TInitialNewValues<ListElement, ListCast> InitialNewValues;
   typedef ListElement Node;
   Template2DefineCollectionForAbstractCollect(TList, TListCursor, ListElement, ListCast)

  protected:
   virtual bool acceptElement(const EnhancedObject& source) const override
      {  return dynamic_cast<const ListElement*>(ListCast::castFrom((const List::Node*) &source));  }
   typedef ListCast ParameterCast;

   template <class TemplateParameters>
   class ExtendedTemplateParameters {
     public:
      typedef typename TemplateParameters::Element Element;
      typedef HandlerLinkedCast<Element, typename TemplateParameters::Cast, ParameterCast> Cast;
   };

   template <class Execute, class TemplateParameters> bool foreachDo(TemplateParameters traits, Execute& function) const
      {  return List::foreachDo(ExtendedTemplateParameters<TemplateParameters>(), function); }
   template <class Execute, class TemplateParameters> bool foreachDo(TemplateParameters traits,
         Execute& function, const ExtendedLocateParameters& parameters,
         const Cursor* start, const Cursor* end) const
      {  return List::foreachDo(ExtendedTemplateParameters<TemplateParameters>(), function, parameters, start, end); }
   template <class Execute, class TemplateParameters> bool foreachReverseDo(TemplateParameters traits, Execute& function) const
      {  return List::foreachReverseDo(ExtendedTemplateParameters<TemplateParameters>(), function); }
   template <class Execute, class TemplateParameters> bool foreachReverseDo(TemplateParameters traits,
         Execute& function, const ExtendedLocateParameters& parameters,
         const Cursor* start, const Cursor* end) const
      {  return List::foreachReverseDo(ExtendedTemplateParameters<TemplateParameters>(), function, parameters, start, end); }

  public:
   TList() {}
   TList(const InitialValues& initialValues)
      {  VirtualCollection::TInsertInitialValue<TList<ListElement, ListCast>, ListElement, ListCast>
            insertInitialValue(*this);
         initialValues.foreachDo(insertInitialValue);
      }
   TList(const InitialNewValues& initialValues)
      {  VirtualCollection::TInsertInitialNewValue<TList<ListElement, ListCast>, ListElement, ListCast>
            insertInitialValue(*this);
         initialValues.foreachDo(insertInitialValue);
      }
   TList(const TList<ListElement, ListCast>& source, AddMode dupMode=AMNoDuplicate,
         const VirtualCast* retrieveRegistrationFromCopy=nullptr)
      : List(source, dupMode, retrieveRegistrationFromCopy) {}
   Template2DefineCopy(TList, ListElement, ListCast)

#define DefTypeElement ListElement
#define DefTypeCollection TList<ListElement, ListCast>
#define DefTypeCastToCollectHandler ListCast::castTo
#define DefTypeCCastToCollectHandler ListCast::castTo
#define DefTypeCastFromCollectHandler (ListElement*) ListCast::castFrom
#define DefTypeCCastFromCollectHandler (const ListElement*) ListCast::castFrom
#define DefTypeInheritedCollection List
#define DefTypeCursor TListCursor<ListElement, ListCast>
   #include "Collection/ELMCollection.inch"
#undef DefTypeElement
#undef DefTypeCollection
#undef DefTypeCastToCollectHandler
#undef DefTypeCCastToCollectHandler
#undef DefTypeCastFromCollectHandler
#undef DefTypeCCastFromCollectHandler
#undef DefTypeInheritedCollection
#undef DefTypeCursor

   bool foreachDo(std::function<bool (const ListElement&)> function) const
      {  return List::foreachDo(TemplateElementCastParameters<ListElement, ListCast>(), function); }
   bool foreachSDo(std::function<bool (ListElement&)> function) const
      {  return List::foreachDo(TemplateElementCastParameters<ListElement, ListCast>(), function); }
   template <class Execute> bool foreachDo(Execute& function) const
      {  return List::foreachDo(TemplateElementCastParameters<ListElement, ListCast>(), function); }
   template <class Execute> bool foreachDo(Execute& function, const ExtendedLocateParameters& parameters,
         const Cursor* start=nullptr, const Cursor* end=nullptr) const
      {  return List::foreachDo(TemplateElementCastParameters<ListElement, ListCast>(), function, parameters,
            start, end);
      }
   bool foreachReverseDo(std::function<bool (const ListElement&)> function) const
      {  return List::foreachReverseDo(TemplateElementCastParameters<ListElement, ListCast>(), function); }
   bool foreachSReverseDo(std::function<bool (ListElement&)> function) const
      {  return List::foreachReverseDo(TemplateElementCastParameters<ListElement, ListCast>(), function); }
   template <class Execute> bool foreachReverseDo(Execute& function) const
      {  return List::foreachReverseDo(TemplateElementCastParameters<ListElement, ListCast>(), function); }
   template <class Execute> bool foreachReverseDo(Execute& function, const ExtendedLocateParameters& parameters,
         const Cursor* start=nullptr, const Cursor* end=nullptr) const
      {  return List::foreachReverseDo(TemplateElementCastParameters<ListElement, ListCast>(), function, parameters,
            start, end);
      }
   DeclareCollectionIteratorForConcreteCollect
};

/************************************************/
/* Definition of the template class TListCursor */
/************************************************/

template<class TypeElement, class Cast=HandlerCast<TypeElement, ImplListElement> >
class TListCursor : public ListCursor {
  public:
   TListCursor(const TList<TypeElement, Cast>& support) : ListCursor(support) {}
   TListCursor(const TListCursor<TypeElement, Cast>& source) : ListCursor(source) {}
   Template2DefineCopy(TListCursor, TypeElement, Cast)
   Template2DefineCursorForAbstractCollect(TList, TListCursor, TypeElement, Cast)

   void gotoReference(const TypeElement& element)
      {  ListCursor::_gotoReference(Cast::castTo(element)); }
   TypeElement* getSElement() const { return (TypeElement*) Cast::castFrom((ImplListElement*) ListCursor::_getSElement()); }
   const TypeElement* getElement() const { return getSElement(); }
   const TypeElement& elementAt() const { return *getSElement(); }
   TypeElement& elementSAt() const { return *getSElement(); }
};

Template2InlineCollectionForAbstractCollect(TList, TListCursor, Element, Cast)
TemplateInlineCollectionIteratorForConcreteCollect(TList, Element, Cast)

} // end of namespace COL

#endif // COL_ListH
