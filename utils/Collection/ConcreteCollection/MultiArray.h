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
// File      : MultiArray.h
// Description :
//   Definition of multi-sets based on sorted arrays.
//

#ifndef COL_MultiArrayH
#define COL_MultiArrayH

#include "Collection/VirtualCollection/VirtualMultiSortedCollection.h"
#include "Pointer/ImplArray.h"

namespace COL {

#define DefTypeVirtualInheritedCollection VirtualMultiSortedCollection
#define DefTypeVirtualInheritedCursor VirtualMultiSortedCollectionCursor
#define DefTypeCollection GenericMultiArray
#define DefTypeCursor GenericMultiArrayCursor
#define DefTypeSorted
#define DefTypeMultiSorted
#include "Collection/ConcreteCollection/Array.inch"
#undef DefTypeMultiSorted
#undef DefTypeSorted
#undef DefTypeCollection
#undef DefTypeCursor
#undef DefTypeVirtualInheritedCollection
#undef DefTypeVirtualInheritedCursor

/************************************************/
/* Definition of the template class SortedArray */
/************************************************/

template<class TypeElement, class Key, class Cast>
class MultiArrayCursor;

template<class Element, class TKey=VirtualMultiSortedCollection::SimpleKey, class Cast=HandlerCast<Element> >
class MultiArray : public GenericMultiArray {
  public:
   typedef TKey Key;
   typedef LightCopyKeyTraits<typename TKey::TypeOfKey> KeyTraits;
   typedef TInitialValues<Element, Cast> InitialValues;
   typedef TInitialNewValues<Element, Cast> InitialNewValues;

  private:
   typedef MultiArray<Element, TKey, Cast> thisType;
   typedef MultiArrayCursor<Element, TKey, Cast> thisCursorType;
   typedef GenericMultiArray inherited;
   typedef typename Key::TypeOfKey TypeOfKey;

  protected:
   virtual bool acceptElement(const EnhancedObject& asource) const { return true; }
   virtual EnhancedObject* _getElement(const ExtendedLocateParameters& parameters,
         const VirtualCollectionCursor* cursor) const
      {  return Cast::castTo(_getElement(parameters, (thisCursorType*) cursor)); }

   LocationResult _locateKey(const EnhancedObject& source, const ExtendedLocateParameters& parameters,
         VirtualMultiSortedCollectionCursor* cursor, const VirtualMultiSortedCollectionCursor* start,
         const VirtualMultiSortedCollectionCursor* end) const
      {  return GenericMultiArray::_locateKey(source, parameters, cursor, start, end); }

   LocationResult _locateKey(typename Key::KeyType key, const ExtendedLocateParameters& parameters,
         GenericMultiArrayCursor* cursor=nullptr, const GenericMultiArrayCursor* start=nullptr,
         const GenericMultiArrayCursor* end=nullptr) const
      {  return GenericMultiArray::_locateKey(TypeOfKey::castToCopyHandler(key), parameters,
            cursor, start, end);
      }
   typename Key::KeyType _queryKey(const ExtendedLocateParameters& parameters,
         const thisCursorType* cursor=nullptr) const
      {  return Key::key(getElement(parameters, cursor)); }
   Element* _getElement(const ExtendedLocateParameters& parameters,
         const thisCursorType* cursor=nullptr) const
      {  return tgetElement(TemplateQueryParameters<thisType, inherited, Element, KeyTraits, Cast>(),
            parameters, cursor);
      }
   Element* getInheritedElement(const ExtendedLocateParameters& parameters,
         const thisCursorType* cursor=nullptr) const
      {  return (Element*) Cast::castFrom(inherited::_getElement(parameters, cursor)); }
   friend class VirtualCollection::TemplateQueryParameters<thisType, inherited, Element, KeyTraits, Cast>;

  public:
   typedef Element Node;

   MultiArray() {}
   MultiArray(const InitialValues& initialValues)
      {  VirtualCollection::TInsertInitialValue<thisType, Element, Cast> insertInitialValue(*this);
         initialValues.foreachDo(insertInitialValue);
      }
   MultiArray(const InitialNewValues& initialValues)
      {  VirtualCollection::TInsertInitialNewValue<thisType, Element, Cast> insertInitialValue(*this);
         initialValues.foreachDo(insertInitialValue);
      }
   MultiArray(const MultiArray<Element, Key, Cast>& source, AddMode mode=AMNoDuplicate,
         const VirtualCast* retrieveRegistrationFromCopy=nullptr)
      :  GenericMultiArray(source, mode, retrieveRegistrationFromCopy) {}
   Template3DefineCopy(MultiArray, Element, Key, Cast)
   Template3DefineCollectionForAbstractCollect(MultiArray, MultiArrayCursor, Element, Key, Cast)

   virtual const EnhancedObject& key(const EnhancedObject& element) const
      { return TypeOfKey::castToCopyHandler(Key::key((const Element&) Cast::castFrom(element))); }

#define DefTypeElement Element
#define DefTypeCollection MultiArray<Element, Key, Cast>
#define DefTypeCastToCollectHandler (EnhancedObject*) Cast::castTo
#define DefTypeCCastToCollectHandler (const EnhancedObject*) Cast::castTo
// #define DefTypeCastFromCollectHandler (Element*) Cast::castFrom
// #define DefTypeCCastFromCollectHandler (const Element*) Cast::castFrom
#define DefTypeCursor MultiArrayCursor<Element, Key, Cast>
#define DefTypeSorted
#define DefTypeMultiSorted
#define DefTypeFinal MultiArray<Element, Key, Cast>::
#define DefTypeKey typename Key::KeyType
#define DefMapInterface
   #include "Collection/ELMCollection.inch"
#undef DefMapInterface
#undef DefTypeFinal
#undef DefTypeKey
#undef DefTypeMultiSorted
#undef DefTypeSorted
#undef DefTypeCursor
#undef DefTypeCastToCollectHandler
#undef DefTypeCCastToCollectHandler
// #undef DefTypeCastFromCollectHandler
// #undef DefTypeCCastFromCollectHandler
#undef DefTypeCollection
#undef DefTypeElement

   bool foreachDo(std::function<bool (const Element&)> function) const
      {  return GenericMultiArray::foreachDo(TemplateElementCastParameters<Element, Cast>(), function); }
   bool foreachSDo(std::function<bool (Element&)> function) const
      {  return GenericMultiArray::foreachDo(TemplateElementCastParameters<Element, Cast>(), function); }
   template <class Execute> bool foreachDo(Execute& function) const
      {  return GenericMultiArray::foreachDo(TemplateElementCastParameters<Element, Cast>(), function); }
   template <class Execute> bool foreachDo(Execute& function, const ExtendedLocateParameters& parameters,
         const Cursor* start, const Cursor* end) const
      {  return GenericMultiArray::foreachDo(TemplateElementCastParameters<Element, Cast>(), function,
            parameters, start, end);
      }
   bool foreachReverseDo(std::function<bool (const Element&)> function) const
      {  return GenericMultiArray::foreachReverseDo(TemplateElementCastParameters<Element, Cast>(), function); }
   bool foreachSReverseDo(std::function<bool (Element&)> function) const
      {  return GenericMultiArray::foreachReverseDo(TemplateElementCastParameters<Element, Cast>(), function); }
   template <class Execute> bool foreachReverseDo(Execute& function) const
      {  return GenericMultiArray::foreachReverseDo(TemplateElementCastParameters<Element, Cast>(), function); }
   template <class Execute> bool foreachReverseDo(Execute& function, const ExtendedLocateParameters& parameters,
         const Cursor* start, const Cursor* end) const
      {  return GenericMultiArray::foreachReverseDo(TemplateElementCastParameters<Element, Cast>(), function,
            parameters, start, end);
      }
   DeclareCollectionIteratorForConcreteCollect
};

/*****************************************************/
/* Definition of the template class MultiArrayCursor */
/*****************************************************/

template<class TypeElement, class Key=VirtualMultiSortedCollection::SimpleKey, class Cast=HandlerCast<TypeElement> >
class MultiArrayCursor : public GenericMultiArrayCursor {
  public:
   MultiArrayCursor(const MultiArray<TypeElement, Key, Cast>& support) : GenericMultiArrayCursor(support) {}
   MultiArrayCursor(const MultiArrayCursor<TypeElement, Key, Cast>& source) : GenericMultiArrayCursor(source) {}
   Template3DefineCopy(MultiArrayCursor, TypeElement, Key, Cast)
   Template3DefineCursorForAbstractCollect(MultiArray, MultiArrayCursor, TypeElement, Key, Cast)

   bool gotoElement(const TypeElement& element) { return getSupport().gotoAny(*this, element); }
   bool setIndex(int index) { return setSureIndex(index); }

#define DefCursor
#define DefTypeElement TypeElement
#define DefTypeInheritedCollection
#define DefTypeCollection MultiArray<TypeElement, Key, Cast>
#define DefTypeCastToCollectHandler (EnhancedObject*) Cast::castTo
#define DefTypeCCastToCollectHandler (const EnhancedObject*) Cast::castTo
#define DefTypeCastFromCollectHandler (TypeElement*) Cast::castFrom
#define DefTypeCCastFromCollectHandler (const TypeElement*) Cast::castFrom
#define DefTypeCursor MultiArrayCursor<TypeElement, Key, Cast>
#define DefTypeSorted
#define DefTypeMultiSorted
#define DefTypeFinal MultiArrayCursor<TypeElement, Key, Cast>::
#define DefTypeKey typename Key::KeyType
   #include "Collection/ELMCollection.inch"
#undef DefTypeFinal
#undef DefTypeKey
#undef DefTypeMultiSorted
#undef DefTypeSorted
#undef DefTypeCursor
#undef DefTypeCastToCollectHandler
#undef DefTypeCCastToCollectHandler
#undef DefTypeCastFromCollectHandler
#undef DefTypeCCastFromCollectHandler
#undef DefTypeCollection
#undef DefTypeInheritedCollection
#undef DefTypeElement
#undef DefCursor

   typename Key::KeyType queryKey() const { return Key::key(elementAt()); }
};

Template3InlineCollectionForAbstractCollect(MultiArray, MultiArrayCursor, TypeElement, Key, Cast)
Template2InlineCollectionIteratorForConcreteCollect(MultiArray, TypeElement, Key, Cast)

/************************************************/
/* Definition of the template class TMultiArray */
/************************************************/

template<class TypeElement, class Key, class Cast>
class TMultiArrayCursor;

template<class TypeElement, class TypeKey=VirtualMultiSortedCollection::SimpleKey, class TypeCast=HandlerCast<TypeElement> >
class TMultiArray : public GenericMultiArray {
  private:
   typedef TMultiArray<TypeElement, TypeKey, TypeCast> thisType;
   typedef TMultiArrayCursor<TypeElement, TypeKey, TypeCast> thisCursorType;
   typedef GenericMultiArray inherited;

   virtual const EnhancedObject& key(const EnhancedObject& element) const
      {  AssumeUncalled return *(const EnhancedObject*) nullptr; }

   LocationResult _locateKey(const EnhancedObject& source, const ExtendedLocateParameters& parameters,
         VirtualMultiSortedCollectionCursor* cursor, const VirtualMultiSortedCollectionCursor* start,
         const VirtualMultiSortedCollectionCursor* end) const { AssumeUncalled return LocationResult(); }

  public:
   typedef TypeKey Key;
   typedef SimpleKeyTraits<typename TypeKey::KeyType> KeyTraits;
   typename TypeKey::ControlKeyType key(const TypeElement& element) const { return TypeKey::key(element); }
   typedef TypeElement Node;
   typedef TInitialValues<TypeElement, TypeCast> InitialValues;
   typedef TInitialNewValues<TypeElement, TypeCast> InitialNewValues;

  protected:
   virtual bool acceptElement(const EnhancedObject& source) const { return true; }

   virtual ComparisonResult _compareElement(const EnhancedObject& fst, const EnhancedObject& snd) const
      { return TypeKey::compare(key((const TypeElement&) TypeCast::castFrom(fst)), key((const TypeElement&) TypeCast::castFrom(snd))); }
   LocationResult _locateKey(typename TypeKey::KeyType key, const ExtendedLocateParameters& parameters,
         Cursor* cursor=nullptr, const Cursor* start=nullptr, const Cursor* end=nullptr) const
      {  return tlocate<thisType, TypeElement, TypeKey, TypeCast>(*this, key, parameters, cursor, start, end); }
   virtual LocationResult _locate(const EnhancedObject& source, const ExtendedLocateParameters& parameters,
         VirtualMultiSortedCollectionCursor* cursor=nullptr, const VirtualMultiSortedCollectionCursor* start=nullptr,
         const VirtualMultiSortedCollectionCursor* end=nullptr) const
      {  return _locateKey(key((const TypeElement&) TypeCast::castFrom(source)), parameters,
            (Cursor*) cursor, (Cursor*) start, (Cursor*) end);
      }

   virtual int _merge(const COL::ImplArray& source, int firstSourceIndex, int lastSourceIndex, bool doesDuplicate=false)
      {  return GenericMultiArray::tmerge(TemplateElementKeyCastParameters<TypeElement, TypeKey, TypeCast>(), source, firstSourceIndex, lastSourceIndex, doesDuplicate);
      }
   typename TypeKey::KeyType _queryKey(const ExtendedLocateParameters& parameters,
         const thisCursorType* cursor=nullptr) const
      { return TypeKey::key(getElement(parameters, cursor)); }
   virtual EnhancedObject* _getElement(const ExtendedLocateParameters& parameters,
         const VirtualCollectionCursor* cursor) const
      { return TypeCast::castTo(_getElement(parameters, (thisCursorType*) cursor)); }
   TypeElement* _getElement(const ExtendedLocateParameters& parameters,
         const thisCursorType* cursor=nullptr) const
      {  return tgetElement(TemplateQueryParameters<thisType, inherited, TypeElement, KeyTraits, TypeCast>(),
            parameters, cursor);
      }
   TypeElement* getInheritedElement(const ExtendedLocateParameters& parameters,
         const thisCursorType* cursor=nullptr) const
      {  return (TypeElement*) TypeCast::castFrom(inherited::_getElement(parameters, cursor)); }
   friend class VirtualCollection::TemplateQueryParameters<thisType, inherited, TypeElement, KeyTraits, TypeCast>;

  public:
   TMultiArray() {}
   TMultiArray(const InitialValues& initialValues)
      {  VirtualCollection::TInsertInitialValue<thisType, TypeElement, TypeCast> insertInitialValue(*this);
         initialValues.foreachDo(insertInitialValue);
      }
   TMultiArray(const InitialNewValues& initialValues)
      {  VirtualCollection::TInsertInitialNewValue<thisType, TypeElement, TypeCast> insertInitialValue(*this);
         initialValues.foreachDo(insertInitialValue);
      }
   TMultiArray(const TMultiArray<TypeElement, TypeKey, TypeCast>& source, AddMode mode=AMNoDuplicate,
         const VirtualCast* retrieveRegistrationFromCopy=nullptr)
      :  GenericMultiArray(source, mode, retrieveRegistrationFromCopy) {}
   Template3DefineCopy(TMultiArray, TypeElement, TypeKey, TypeCast)
   Template3DefineCollectionForAbstractCollect(TMultiArray, TMultiArrayCursor, TypeElement, TypeKey, TypeCast)

#define DefTypeElement TypeElement
#define DefTypeCollection TMultiArray<TypeElement, TypeKey, TypeCast>
#define DefTypeCastToCollectHandler (EnhancedObject*) TypeCast::castTo
#define DefTypeCCastToCollectHandler (const EnhancedObject*) TypeCast::castTo
// #define DefTypeCastFromCollectHandler (TypeElement*) TypeCast::castFrom
// #define DefTypeCCastFromCollectHandler (const TypeElement*) TypeCast::castFrom
#define DefTypeFinal TMultiArray<TypeElement, TypeKey, TypeCast>::
#define DefTypeCursor TMultiArrayCursor<TypeElement, TypeKey, TypeCast>
#define DefTypeSorted
#define DefTypeMultiSorted
#define DefTypeKey typename TypeKey::KeyType
#define DefMapInterface
   #include "Collection/ELMCollection.inch"
#undef DefMapInterface
#undef DefTypeKey
#undef DefTypeMultiSorted
#undef DefTypeSorted
#undef DefTypeElement
#undef DefTypeCollection
#undef DefTypeCastToCollectHandler
#undef DefTypeCCastToCollectHandler
// #undef DefTypeCastFromCollectHandler
// #undef DefTypeCCastFromCollectHandler
#undef DefTypeFinal
#undef DefTypeCursor

   bool foreachDo(std::function<bool (const TypeElement&)> function) const
      {  return GenericMultiArray::foreachDo(TemplateElementCastParameters<TypeElement, TypeCast>(), function); }
   bool foreachSDo(std::function<bool (TypeElement&)> function) const
      {  return GenericMultiArray::foreachDo(TemplateElementCastParameters<TypeElement, TypeCast>(), function); }
   template <class Execute> bool foreachDo(Execute& function) const
      {  return GenericMultiArray::foreachDo(TemplateElementCastParameters<TypeElement, TypeCast>(), function); }
   template <class Execute> bool foreachDo(Execute& function, const ExtendedLocateParameters& parameters,
         const Cursor* start, const Cursor* end) const
      {  return GenericMultiArray::foreachDo(TemplateElementCastParameters<TypeElement, TypeCast>(),
            function, parameters, start, end);
      }
   bool foreachReverseDo(std::function<bool (const TypeElement&)> function) const
      {  return GenericMultiArray::foreachReverseDo(TemplateElementCastParameters<TypeElement, TypeCast>(), function); }
   bool foreachSReverseDo(std::function<bool (TypeElement&)> function) const
      {  return GenericMultiArray::foreachReverseDo(TemplateElementCastParameters<TypeElement, TypeCast>(), function); }
   template <class Execute> bool foreachReverseDo(Execute& function) const
      {  return GenericMultiArray::foreachReverseDo(TemplateElementCastParameters<TypeElement, TypeCast>(), function); }
   template <class Execute> bool foreachReverseDo(Execute& function, const ExtendedLocateParameters& parameters,
         const Cursor* start, const Cursor* end) const
      {  return GenericMultiArray::foreachReverseDo(TemplateElementCastParameters<TypeElement, TypeCast>(),
            function, parameters, start, end);
      }
   DeclareCollectionIteratorForConcreteCollect
};

/******************************************************/
/* Definition of the template class TMultiArrayCursor */
/******************************************************/

template<class TypeElement, class Key=VirtualMultiSortedCollection::SimpleKey, class Cast=HandlerCast<TypeElement> >
class TMultiArrayCursor : public GenericMultiArrayCursor {
  public:
   TMultiArrayCursor(const TMultiArray<TypeElement, Key, Cast>& support) : GenericMultiArrayCursor(support) {}
   TMultiArrayCursor(const TMultiArrayCursor<TypeElement, Key, Cast>& source) : GenericMultiArrayCursor(source) {}
   Template3DefineCopy(TMultiArrayCursor, TypeElement, Key, Cast)
   Template3DefineCursorForAbstractCollect(TMultiArray, TMultiArrayCursor, TypeElement, Key, Cast)

   TypeElement* getSElement() const { return (TypeElement*) Cast::castFrom(GenericMultiArrayCursor::_getSElement()); }
   const TypeElement* getElement() const { return getSElement(); }
   const TypeElement& elementAt() const { return *getSElement(); }
   TypeElement& elementSAt() const { return *getSElement(); }

   bool gotoElement(const TypeElement& element) { return getSupport().gotoAny(*this, element); }
   bool setIndex(int index) { return setSureIndex(index); }
   typename Key::KeyType queryKey() const { return Key::key(elementAt()); }
};

Template3InlineCollectionForAbstractCollect(TMultiArray, TMultiArrayCursor, TypeElement, Key, Cast)
Template2InlineCollectionIteratorForConcreteCollect(TMultiArray, TypeElement, Key, Cast)

} // end of namespace COL

#endif // COL_MultiArrayH
