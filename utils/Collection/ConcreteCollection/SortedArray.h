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
// File      : SortedArray.h
// Description :
//   Definition of concrete collections based on sorted arrays.
//

#ifndef COL_SortedArrayH
#define COL_SortedArrayH

#include "Collection/VirtualCollection/VirtualSortedCollection.h"
#include "Pointer/ImplArray.h"

namespace COL {

#define DefTypeVirtualInheritedCollection VirtualSortedCollection
#define DefTypeVirtualInheritedCursor VirtualSortedCollectionCursor
#define DefTypeCollection GenericSortedArray
#define DefTypeCursor GenericSortedArrayCursor
#define DefTypeSorted
#include "Collection/ConcreteCollection/Array.inch"
#undef DefTypeSorted
#undef DefTypeCollection
#undef DefTypeCursor
#undef DefTypeVirtualInheritedCollection
#undef DefTypeVirtualInheritedCursor

/************************************************/
/* Definition of the template class SortedArray */
/************************************************/

template<class TypeElement, class Key, class Cast>
class SortedArrayCursor;

template<class Element, class TKey=VirtualSortedCollection::SimpleKey, class Cast=HandlerCopyCast<Element> >
class SortedArray : public GenericSortedArray {
  public:
   typedef TKey Key;
   typedef LightCopyKeyTraits<typename TKey::TypeOfKey> KeyTraits;
   typedef TInitialValues<Element, Cast> InitialValues;
   typedef TInitialNewValues<Element, Cast> InitialNewValues;

  private:
   typedef SortedArray<Element, TKey, Cast> thisType;
   typedef SortedArrayCursor<Element, TKey, Cast> thisCursorType;
   typedef GenericSortedArray inherited;
   typedef typename Key::TypeOfKey TypeOfKey;

  protected:
   virtual bool acceptElement(const EnhancedObject& source) const override { return true; }
   virtual EnhancedObject* _getElement(const ExtendedLocateParameters& parameters,
         const VirtualCollectionCursor* cursor) const override
      {  return Cast::castTo(_getElement(parameters, (thisCursorType*) cursor)); }

   virtual LocationResult _locateKey(const EnhancedObject& source, const ExtendedLocateParameters& parameters,
         VirtualSortedCollectionCursor* cursor, const VirtualSortedCollectionCursor* start,
         const VirtualSortedCollectionCursor* end) const override
      {  return GenericSortedArray::_locateKey(source, parameters, cursor, start, end); }
   LocationResult _locateKey(typename Key::KeyType source, const ExtendedLocateParameters& parameters,
         GenericSortedArrayCursor* cursor=nullptr, const GenericSortedArrayCursor* start=nullptr,
         const GenericSortedArrayCursor* end=nullptr) const
      {  return GenericSortedArray::_locateKey(TypeOfKey::castToCopyHandler(source), parameters,
            cursor, start, end);
      }
   typename Key::KeyType _queryKey(const ExtendedLocateParameters& parameters,
         const thisCursorType* cursor=nullptr) const
      {  return Key::key(getElement(parameters, cursor)); }
   Element* _getElement(const ExtendedLocateParameters& parameters, const thisCursorType* cursor=nullptr) const
      {  return tgetElement(TemplateQueryParameters<thisType, inherited, Element, KeyTraits, Cast>(),
            parameters, cursor);
      }
   Element* getInheritedElement(const ExtendedLocateParameters& parameters,
         const thisCursorType* cursor=nullptr) const
      {  return (Element*) Cast::castFrom(inherited::_getElement(parameters, cursor)); }
   friend class VirtualCollection::TemplateQueryParameters<thisType, inherited, Element, KeyTraits, Cast>;

  public:
   typedef Element Node;

   SortedArray() {}
   SortedArray(const InitialValues& initialValues)
      {  VirtualCollection::TInsertInitialValue<thisType, Element, Cast> insertInitialValue(*this);
         initialValues.foreachDo(insertInitialValue);
      }
   SortedArray(const InitialNewValues& initialValues)
      {  VirtualCollection::TInsertInitialNewValue<thisType, Element, Cast> insertInitialValue(*this);
         initialValues.foreachDo(insertInitialValue);
      }
   SortedArray(const SortedArray<Element, Key, Cast>& source, AddMode mode=AMNoDuplicate,
         const VirtualCast* retrieveRegistrationFromCopy=nullptr)
      :  GenericSortedArray(source, mode, retrieveRegistrationFromCopy) {}
   Template3DefineCopy(SortedArray, Element, Key, Cast)
   Template3DefineCollectionForAbstractCollect(SortedArray, SortedArrayCursor, Element, Key, Cast)

   virtual const EnhancedObject& key(const EnhancedObject& element) const override
      { return TypeOfKey::castToCopyHandler(Key::key((const Element&) Cast::castFrom(element))); }

#define DefTypeElement Element
#define DefTypeCollection SortedArray<Element, Key, Cast>
#define DefTypeCastToCollectHandler (EnhancedObject*) Cast::castTo
#define DefTypeCCastToCollectHandler (const EnhancedObject*) Cast::castTo
// #define DefTypeCastFromCollectHandler (Element*) Cast::castFrom
// #define DefTypeCCastFromCollectHandler (const Element*) Cast::castFrom
#define DefTypeCursor SortedArrayCursor<Element, Key, Cast>
#define DefTypeSorted
#define DefTypeFinal SortedArray<Element, Key, Cast>::
#define DefTypeKey typename Key::KeyType
#define DefMapInterface
   #include "Collection/ELMCollection.inch"
#undef DefMapInterface
#undef DefTypeFinal
#undef DefTypeKey
#undef DefTypeSorted
#undef DefTypeCursor
#undef DefTypeCastToCollectHandler
#undef DefTypeCCastToCollectHandler
// #undef DefTypeCastFromCollectHandler
// #undef DefTypeCCastFromCollectHandler
#undef DefTypeCollection
#undef DefTypeElement

   bool foreachDo(std::function<bool (const Element&)> function) const
      {  return GenericSortedArray::foreachDo(TemplateElementCastParameters<Element, Cast>(), function); }
   bool foreachSDo(std::function<bool (Element&)> function) const
      {  return GenericSortedArray::foreachDo(TemplateElementCastParameters<Element, Cast>(), function); }
   template <class Execute> bool foreachDo(Execute& function) const
      {  return GenericSortedArray::foreachDo(TemplateElementCastParameters<Element, Cast>(), function); }
   template <class Execute> bool foreachDo(Execute& function, const ExtendedLocateParameters& parameters,
      const Cursor* start, const Cursor* end) const
      {  return GenericSortedArray::foreachDo(TemplateElementCastParameters<Element, Cast>(), function,
            parameters, start, end);
      }
   bool foreachReverseDo(std::function<bool (const Element&)> function) const
      {  return GenericSortedArray::foreachReverseDo(TemplateElementCastParameters<Element, Cast>(), function); }
   bool foreachSReverseDo(std::function<bool (Element&)> function) const
      {  return GenericSortedArray::foreachReverseDo(TemplateElementCastParameters<Element, Cast>(), function); }
   template <class Execute> bool foreachReverseDo(Execute& function) const
      {  return GenericSortedArray::foreachReverseDo(TemplateElementCastParameters<Element, Cast>(), function); }
   template <class Execute> bool foreachReverseDo(Execute& function, const ExtendedLocateParameters& parameters,
      const Cursor* start, const Cursor* end) const
      {  return GenericSortedArray::foreachReverseDo(TemplateElementCastParameters<Element, Cast>(), function,
            parameters, start, end);
      }
   DeclareCollectionIteratorForConcreteCollect
};

/******************************************************/
/* Definition of the template class SortedArrayCursor */
/******************************************************/

template<class TypeElement, class Key=VirtualSortedCollection::SimpleKey, class Cast=HandlerCopyCast<TypeElement> >
class SortedArrayCursor : public GenericSortedArrayCursor {
  public:
   SortedArrayCursor(const SortedArray<TypeElement, Key, Cast>& support) : GenericSortedArrayCursor(support) {}
   SortedArrayCursor(const SortedArrayCursor<TypeElement, Key, Cast>& source) : GenericSortedArrayCursor(source) {}
   Template3DefineCopy(SortedArrayCursor, TypeElement, Key, Cast)
   Template3DefineCursorForAbstractCollect(SortedArray, SortedArrayCursor, TypeElement, Key, Cast)

   bool gotoElement(const TypeElement& element) { return getSupport().gotoAny(*this, element); }
   bool setIndex(int index) { return setSureIndex(index); }

#define DefCursor
#define DefTypeElement TypeElement
#define DefTypeInheritedCollection
#define DefTypeCollection SortedArray<TypeElement, Key, Cast>
#define DefTypeCastToCollectHandler (EnhancedObject*) Cast::castTo
#define DefTypeCCastToCollectHandler (const EnhancedObject*) Cast::castTo
#define DefTypeCastFromCollectHandler (TypeElement*) Cast::castFrom
#define DefTypeCCastFromCollectHandler (const TypeElement*) Cast::castFrom
#define DefTypeCursor SortedArrayCursor<TypeElement, Key, Cast>
#define DefTypeSorted
#define DefTypeFinal SortedArrayCursor<TypeElement, Key, Cast>::
#define DefTypeKey typename Key::KeyType
   #include "Collection/ELMCollection.inch"
#undef DefTypeFinal
#undef DefTypeKey
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

Template3InlineCollectionForAbstractCollect(SortedArray, SortedArrayCursor, TypeElement, Key, Cast)

template <class Element, class Key, class Cast>
class SortedArray<Element, Key, Cast>::iterator : public VirtualCollection::titerator<SortedArray<Element, Key, Cast>, Element> {
  private:
   typedef VirtualCollection::titerator<SortedArray<Element, Key, Cast>, Element> inherited;

   iterator(const SortedArray<Element, Key, Cast>& support) : inherited(support) {}
   friend class SortedArray<Element, Key, Cast>;

  protected:
   iterator(const typename inherited::Base& source) : inherited(source) {}
   friend class reverse_iterator;

  public:
   iterator(const iterator& source) = default;
   iterator(iterator&& source) : inherited(source) {}

   iterator& operator+=(int add)
      {  int indexResult = inherited::index() + add;
         int count = inherited::getSupport().count();
         indexResult %= (count+1);
         if (indexResult < 0)
            indexResult += (count+1);
         if (indexResult == count)
            indexResult = -1;
         inherited::setSureIndex(indexResult);
         return *this;
      }
   iterator operator+(int add) const
      {  int indexResult = inherited::index() + add;
         int count = inherited::getSupport().count();
         indexResult %= (count+1);
         if (indexResult < 0)
            indexResult += (count+1);
         if (indexResult == count)
            indexResult = -1;
         iterator result(*this);
         result.setSureIndex(indexResult);
         return result;
      }
   iterator& operator-=(int sub)
      {  int indexResult = inherited::index() - sub;
         int count = inherited::getSupport().count();
         indexResult %= (count+1);
         if (indexResult < 0)
            indexResult += (count+1);
         if (indexResult == count)
            indexResult = -1;
         inherited::setSureIndex(indexResult);
         return *this;
      }
   iterator operator-(int sub) const
      {  int indexResult = inherited::index() - sub;
         int count = inherited::getSupport().count();
         indexResult %= (count+1);
         if (indexResult < 0)
            indexResult += (count+1);
         if (indexResult == count)
            indexResult = -1;
         iterator result(*this);
         result.setSureIndex(indexResult);
         return result;
      }
};

template <class Element, class Key, class Cast>
inline typename SortedArray<Element, Key, Cast>::iterator
SortedArray<Element, Key, Cast>::begin() const {
   iterator result(*this);
   result.setToFirst();
   return result;
}

template <class Element, class Key, class Cast>
inline typename SortedArray<Element, Key, Cast>::iterator
SortedArray<Element, Key, Cast>::end() const
   {  return iterator(*this); }

template <class Element, class Key, class Cast>
class SortedArray<Element, Key, Cast>::reverse_iterator : public VirtualCollection::treverse_iterator<SortedArray<Element, Key, Cast>, Element> {
  private:
   typedef VirtualCollection::treverse_iterator<SortedArray<Element, Key, Cast>, Element> inherited;

   reverse_iterator(const SortedArray<Element, Key, Cast>& support) : inherited(support) {}
   friend class SortedArray<Element, Key, Cast>;

  protected:
   reverse_iterator(const typename inherited::Base& source) : inherited(source) {}

  public:
   reverse_iterator(const reverse_iterator& source) = default;
   reverse_iterator(reverse_iterator&& source) : inherited(source) {}
   iterator base() const
      {  iterator result((const iterator&) (const inherited&) *this);
         ++result;
         return result;
      }
   reverse_iterator& operator+=(int add)
      {  int indexResult = inherited::index() - add;
         int count = inherited::getSupport().count();
         indexResult %= (count+1);
         if (indexResult < 0)
            indexResult += (count+1);
         if (indexResult == count)
            indexResult = -1;
         inherited::setSureIndex(indexResult);
         return *this;
      }
   reverse_iterator operator+(int add) const
      {  int indexResult = inherited::index() - add;
         int count = inherited::getSupport().count();
         indexResult %= (count+1);
         if (indexResult < 0)
            indexResult += (count+1);
         if (indexResult == count)
            indexResult = -1;
         reverse_iterator result(*this);
         result.setSureIndex(indexResult);
         return result;
      }
   reverse_iterator& operator-=(int sub)
      {  int indexResult = inherited::index() + sub;
         int count = inherited::getSupport().count();
         indexResult %= (count+1);
         if (indexResult < 0)
            indexResult += (count+1);
         if (indexResult == count)
            indexResult = -1;
         inherited::setSureIndex(indexResult);
         return *this;
      }
   reverse_iterator operator-(int sub) const
      {  int indexResult = inherited::index() + sub;
         int count = inherited::getSupport().count();
         indexResult %= (count+1);
         if (indexResult < 0)
            indexResult += (count+1);
         if (indexResult == count)
            indexResult = -1;
         reverse_iterator result(*this);
         result.setSureIndex(indexResult);
         return result;
      }
};

template <class Element, class Key, class Cast>
inline typename SortedArray<Element, Key, Cast>::reverse_iterator
SortedArray<Element, Key, Cast>::rbegin() const
{  reverse_iterator result(*this);
   result.setToLast();
   return result;
}

template <class Element, class Key, class Cast>
inline typename SortedArray<Element, Key, Cast>::reverse_iterator
SortedArray<Element, Key, Cast>::rend() const
   {  return reverse_iterator(*this); }

/*************************************************/
/* Definition of the template class TSortedArray */
/*************************************************/

template<class TypeElement, class Key, class Cast>
class TSortedArrayCursor;

template<class TypeElement, class TypeKey=VirtualSortedCollection::SimpleKey, class TypeCast=HandlerCopyCast<TypeElement> >
class TSortedArray : public GenericSortedArray {
  private:
   typedef TSortedArray<TypeElement, TypeKey, TypeCast> thisType;
   typedef TSortedArrayCursor<TypeElement, TypeKey, TypeCast> thisCursorType;
   typedef GenericSortedArray inherited;

   virtual const EnhancedObject& key(const EnhancedObject& element) const override
      { AssumeUncalled const EnhancedObject* result = nullptr; return *result; }

   LocationResult _locateKey(const EnhancedObject& source, const ExtendedLocateParameters& parameters,
         VirtualSortedCollectionCursor* cursor, const VirtualSortedCollectionCursor* start,
         const VirtualSortedCollectionCursor* end) const override
      {  AssumeUncalled return LocationResult(); }

  public:
   typedef TypeKey Key;
   typedef SimpleKeyTraits<typename TypeKey::KeyType> KeyTraits;
   typename TypeKey::ControlKeyType key(const TypeElement& element) const { return TypeKey(*this).key(element); }
   typedef TypeElement Node;
   typedef TInitialValues<TypeElement, TypeCast> InitialValues;
   typedef TInitialNewValues<TypeElement, TypeCast> InitialNewValues;

  protected:
   virtual bool acceptElement(const EnhancedObject& source) const override { return true; }
   virtual ComparisonResult _compareElement(const EnhancedObject& fst, const EnhancedObject& snd) const override
      {  TypeKey typeKey(*this);
         return typeKey.compare(typeKey.key((const TypeElement&) TypeCast::castFrom(fst)), typeKey.key((const TypeElement&) TypeCast::castFrom(snd)));
      }
   LocationResult _locateKey(typename TypeKey::KeyType key, const ExtendedLocateParameters& parameters,
         Cursor* cursor=nullptr, const Cursor* start=nullptr, const Cursor* end=nullptr) const
      {  return tlocate<thisType, TypeElement, TypeKey, TypeCast>(*this, key, parameters, cursor, start, end);
      }
   virtual LocationResult _locate(const EnhancedObject& source, const ExtendedLocateParameters& parameters,
         VirtualSortedCollectionCursor* cursor=nullptr, const VirtualSortedCollectionCursor* start=nullptr,
         const VirtualSortedCollectionCursor* end=nullptr) const override
      {  return _locateKey(key((const TypeElement&) TypeCast::castFrom(source)), parameters,
            (Cursor*) cursor, (Cursor*) start, (Cursor*) end);
      }

   class GenericLocateParameters {
     private:
      TypeKey tkKey;
      
     public:
      GenericLocateParameters(const VirtualSortedCollection& vscSupport) : tkKey(vscSupport) {}
      typedef TypeElement Element;
      typedef TypeCast Cast;
      typedef TypeKey Key;

      typename TypeKey::ControlKeyType key(const TypeElement& teElement) const { return tkKey.key(teElement); }
      ComparisonResult keyCompare(typename TypeKey::KeyType fst, typename TypeKey::KeyType snd) const
         {  return tkKey.compare(fst, snd); }
   };

   virtual int _merge(const COL::ImplArray& source, int firstSourceIndex, int lastSourceIndex, bool doesDuplicate=false) override
      {  return GenericSortedArray::tmerge(GenericLocateParameters(*this), source, firstSourceIndex, lastSourceIndex, doesDuplicate); }
   typename TypeKey::KeyType _queryKey(const ExtendedLocateParameters& parameters,
         const thisCursorType* cursor=nullptr) const
      {  return TypeKey(*this).key(getElement(parameters, cursor)); }
   virtual EnhancedObject* _getElement(const ExtendedLocateParameters& parameters,
         const VirtualCollectionCursor* cursor) const override
      {  return TypeCast::castTo(_getElement(parameters, (thisCursorType*) cursor)); }
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
   TSortedArray() {}
   TSortedArray(const InitialValues& initialValues)
      {  VirtualCollection::TInsertInitialValue<thisType, TypeElement, TypeCast> insertInitialValue(*this);
         initialValues.foreachDo(insertInitialValue);
      }
   TSortedArray(const InitialNewValues& initialValues)
      {  VirtualCollection::TInsertInitialNewValue<thisType, TypeElement, TypeCast> insertInitialValue(*this);
         initialValues.foreachDo(insertInitialValue);
      }
   TSortedArray(const thisType& source, AddMode mode=AMNoDuplicate,
         const VirtualCast* retrieveRegistrationFromCopy=nullptr)
      :  GenericSortedArray(source, mode, retrieveRegistrationFromCopy) {}
   Template3DefineCopy(TSortedArray, TypeElement, TypeKey, TypeCast)
   Template3DefineCollectionForAbstractCollect(TSortedArray, TSortedArrayCursor, TypeElement, TypeKey, TypeCast)

#define DefTypeElement TypeElement
#define DefTypeCollection TSortedArray<TypeElement, TypeKey, TypeCast>
#define DefTypeCastToCollectHandler (EnhancedObject*) TypeCast::castTo
#define DefTypeCCastToCollectHandler (const EnhancedObject*) TypeCast::castTo
// #define DefTypeCastFromCollectHandler (TypeElement*) TypeCast::castFrom
// #define DefTypeCCastFromCollectHandler (const TypeElement*) TypeCast::castFrom
#define DefTypeFinal TSortedArray<TypeElement, TypeKey, TypeCast>::
#define DefTypeCursor TSortedArrayCursor<TypeElement, TypeKey, TypeCast>
#define DefTypeSorted
#define DefTypeKey typename TypeKey::KeyType
#define DefMapInterface
   #include "Collection/ELMCollection.inch"
#undef DefMapInterface
#undef DefTypeKey
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
      {  return GenericSortedArray::foreachDo(TemplateElementCastParameters<TypeElement, TypeCast>(), function); }
   bool foreachSDo(std::function<bool (TypeElement&)> function) const
      {  return GenericSortedArray::foreachDo(TemplateElementCastParameters<TypeElement, TypeCast>(), function); }
   template <class Execute> bool foreachDo(Execute& function) const
      {  return GenericSortedArray::foreachDo(TemplateElementCastParameters<TypeElement, TypeCast>(), function); }
   template <class Execute> bool foreachDo(Execute& function, const ExtendedLocateParameters& parameters,
      const Cursor* start, const Cursor* end) const
      {  return GenericSortedArray::foreachDo(TemplateElementCastParameters<TypeElement, TypeCast>(),
            function, parameters, start, end);
      }
   bool foreachReverseDo(std::function<bool (const TypeElement&)> function) const
      {  return GenericSortedArray::foreachReverseDo(TemplateElementCastParameters<TypeElement, TypeCast>(), function); }
   bool foreachSReverseDo(std::function<bool (TypeElement&)> function) const
      {  return GenericSortedArray::foreachReverseDo(TemplateElementCastParameters<TypeElement, TypeCast>(), function); }
   template <class Execute> bool foreachReverseDo(Execute& function) const
      {  return GenericSortedArray::foreachReverseDo(TemplateElementCastParameters<TypeElement, TypeCast>(), function); }
   template <class Execute> bool foreachReverseDo(Execute& function, const ExtendedLocateParameters& parameters,
      const Cursor* start, const Cursor* end) const
      {  return GenericSortedArray::foreachReverseDo(TemplateElementCastParameters<TypeElement, TypeCast>(),
            function, parameters, start, end);
      }
   DeclareCollectionIteratorForConcreteCollect
};

/*******************************************************/
/* Definition of the template class TSortedArrayCursor */
/*******************************************************/

template<class TypeElement, class Key=VirtualSortedCollection::SimpleKey, class Cast=HandlerCopyCast<TypeElement> >
class TSortedArrayCursor : public GenericSortedArrayCursor {
  public:
   TSortedArrayCursor(const TSortedArray<TypeElement, Key, Cast>& support) : GenericSortedArrayCursor(support) {}
   TSortedArrayCursor(const TSortedArrayCursor<TypeElement, Key, Cast>& source) : GenericSortedArrayCursor(source) {}
   Template3DefineCopy(TSortedArrayCursor, TypeElement, Key, Cast)
   Template3DefineCursorForAbstractCollect(TSortedArray, TSortedArrayCursor, TypeElement, Key, Cast)

   TypeElement* getSElement() const { return (TypeElement*) Cast::castFrom(GenericSortedArrayCursor::_getSElement()); }
   const TypeElement* getElement() const { return getSElement(); }
   const TypeElement& elementAt() const { return *getSElement(); }
   TypeElement& elementSAt() const { return *getSElement(); }

   bool gotoElement(const TypeElement& element) { return getSupport().gotoAny(*this, element); }
   bool setIndex(int index) { return setSureIndex(index); }
   typename Key::KeyType queryKey() const { return Key(getSupport()).key(elementAt()); }
};

Template3InlineCollectionForAbstractCollect(TSortedArray, TSortedArrayCursor, TypeElement, Key, Cast)

template <class Element, class Key, class Cast>
class TSortedArray<Element, Key, Cast>::iterator : public VirtualCollection::titerator<TSortedArray<Element, Key, Cast>, Element> {
  private:
   typedef VirtualCollection::titerator<TSortedArray<Element, Key, Cast>, Element> inherited;

   iterator(const TSortedArray<Element, Key, Cast>& support) : inherited(support) {}
   friend class TSortedArray<Element, Key, Cast>;

  protected:
   iterator(const typename inherited::Base& source) : inherited(source) {}
   friend class reverse_iterator;

  public:
   iterator(const iterator& source) = default;

   iterator& operator+=(int add)
      {  int indexResult = inherited::index() + add;
         int count = inherited::getSupport().count();
         indexResult %= (count+1);
         if (indexResult < 0)
            indexResult += (count+1);
         if (indexResult == count)
            indexResult = -1;
         inherited::setSureIndex(indexResult);
         return *this;
      }
   iterator operator+(int add) const
      {  int indexResult = inherited::index() + add;
         int count = inherited::getSupport().count();
         indexResult %= (count+1);
         if (indexResult < 0)
            indexResult += (count+1);
         if (indexResult == count)
            indexResult = -1;
         iterator result(*this);
         result.setSureIndex(indexResult);
         return result;
      }
   iterator& operator-=(int sub)
      {  int indexResult = inherited::index() - sub;
         int count = inherited::getSupport().count();
         indexResult %= (count+1);
         if (indexResult < 0)
            indexResult += (count+1);
         if (indexResult == count)
            indexResult = -1;
         inherited::setSureIndex(indexResult);
         return *this;
      }
   iterator operator-(int sub) const
      {  int indexResult = inherited::index() - sub;
         int count = inherited::getSupport().count();
         indexResult %= (count+1);
         if (indexResult < 0)
            indexResult += (count+1);
         if (indexResult == count)
            indexResult = -1;
         iterator result(*this);
         result.setSureIndex(indexResult);
         return result;
      }
};

template <class Element, class Key, class Cast>
inline typename TSortedArray<Element, Key, Cast>::iterator
TSortedArray<Element, Key, Cast>::begin() const {
   iterator result(*this);
   result.setToFirst();
   return result;
}

template <class Element, class Key, class Cast>
inline typename TSortedArray<Element, Key, Cast>::iterator
TSortedArray<Element, Key, Cast>::end() const
   {  return iterator(*this); }

template <class Element, class Key, class Cast>
class TSortedArray<Element, Key, Cast>::reverse_iterator : public VirtualCollection::treverse_iterator<TSortedArray<Element, Key, Cast>, Element> {
  private:
   typedef VirtualCollection::treverse_iterator<TSortedArray<Element, Key, Cast>, Element> inherited;

   reverse_iterator(const TSortedArray<Element, Key, Cast>& support) : inherited(support) {}
   friend class TSortedArray<Element, Key, Cast>;

  protected:
   reverse_iterator(const typename inherited::Base& source) : inherited(source) {}

  public:
   reverse_iterator(const reverse_iterator& source) = default;

   iterator base() const
      {  iterator result((const inherited&) *this);
         ++result;
         return result;
      }
   reverse_iterator& operator+=(int add)
      {  int indexResult = inherited::index() - add;
         int count = inherited::getSupport().count();
         indexResult %= (count+1);
         if (indexResult < 0)
            indexResult += (count+1);
         if (indexResult == count)
            indexResult = -1;
         inherited::setSureIndex(indexResult);
         return *this;
      }
   reverse_iterator operator+(int add) const
      {  int indexResult = inherited::index() - add;
         int count = inherited::getSupport().count();
         indexResult %= (count+1);
         if (indexResult < 0)
            indexResult += (count+1);
         if (indexResult == count)
            indexResult = -1;
         reverse_iterator result(*this);
         result.setSureIndex(indexResult);
         return result;
      }
   reverse_iterator& operator-=(int sub)
      {  int indexResult = inherited::index() + sub;
         int count = inherited::getSupport().count();
         indexResult %= (count+1);
         if (indexResult < 0)
            indexResult += (count+1);
         if (indexResult == count)
            indexResult = -1;
         inherited::setSureIndex(indexResult);
         return *this;
      }
   reverse_iterator operator-(int sub) const
      {  int indexResult = inherited::index() + sub;
         int count = inherited::getSupport().count();
         indexResult %= (count+1);
         if (indexResult < 0)
            indexResult += (count+1);
         if (indexResult == count)
            indexResult = -1;
         reverse_iterator result(*this);
         result.setSureIndex(indexResult);
         return result;
      }
};

template <class Element, class Key, class Cast>
inline typename TSortedArray<Element, Key, Cast>::reverse_iterator
TSortedArray<Element, Key, Cast>::rbegin() const
{  reverse_iterator result(*this);
   result.setToLast();
   return result;
}

template <class Element, class Key, class Cast>
inline typename TSortedArray<Element, Key, Cast>::reverse_iterator
TSortedArray<Element, Key, Cast>::rend() const
   {  return reverse_iterator(*this); }

} // end of namespace COL

#endif
