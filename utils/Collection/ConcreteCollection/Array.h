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
// File      : Array.h
// Description :
//   Definition of concrete classes based on arrays.
//

#ifndef COL_ArrayH
#define COL_ArrayH

#include "Collection/VirtualCollection/VirtualCollection.h"
#include "Pointer/ImplArray.h"

namespace COL {

#define DefTypeVirtualInheritedCollection VirtualCollection
#define DefTypeVirtualInheritedCursor VirtualCollectionCursor
#define DefTypeCollection GenericArray
#define DefTypeCursor GenericArrayCursor
#include "Collection/ConcreteCollection/Array.inch"
#undef DefTypeCollection
#undef DefTypeCursor
#undef DefTypeVirtualInheritedCollection
#undef DefTypeVirtualInheritedCursor

/*******************************************/
/* Definition of the template class TArray */
/*******************************************/

template<class TypeElement, class Cast>
class TArrayCursor;

template<class Element, class Cast=HandlerCopyCast<Element> >
class TArray : public GenericArray {
  private:
   typedef TArray<Element, Cast> thisType;

  public:
   typedef TInitialValues<Element, Cast> InitialValues;
   typedef TInitialNewValues<Element, Cast> InitialNewValues;

  protected:
   virtual bool acceptElement(const EnhancedObject& asource) const override
      {  return dynamic_cast<const Element*>(Cast::castFrom(&asource));  }

  public:
   typedef Element Node;
   TArray() {}
   TArray(const InitialValues& initialValues)
      {  VirtualCollection::TInsertInitialValue<thisType, Element, Cast>
            insertInitialValue(*this);
         initialValues.foreachDo(insertInitialValue);
      }
   TArray(const InitialNewValues& initialValues)
      {  VirtualCollection::TInsertInitialNewValue<thisType, Element, Cast>
            insertInitialValue(*this);
         initialValues.foreachDo(insertInitialValue);
      }
   TArray(const thisType& source, AddMode dupMode=AMNoDuplicate,
         const VirtualCast* retrieveRegistrationFromCopy=nullptr)
      :  GenericArray(source, dupMode, retrieveRegistrationFromCopy) {}
   Template2DefineCopy(TArray, Element, Cast)
   Template2DefineCollectionForAbstractCollect(TArray, TArrayCursor, Element, Cast)

#define DefTypeElement Element
#define DefTypeCollection TArray<Element, Cast>
#define DefTypeCastToCollectHandler (EnhancedObject*) Cast::castTo
#define DefTypeCCastToCollectHandler (const EnhancedObject*) Cast::castTo
#define DefTypeCastFromCollectHandler (Element*) Cast::castFrom
#define DefTypeCCastFromCollectHandler (const Element*) Cast::castFrom
#define DefTypeInheritedCollection GenericArray
#define DefTypeCursor TArrayCursor<Element, Cast>
   #include "Collection/ELMCollection.inch"
#undef DefTypeElement
#undef DefTypeCollection
#undef DefTypeCastToCollectHandler
#undef DefTypeCCastToCollectHandler
#undef DefTypeCastFromCollectHandler
#undef DefTypeCCastFromCollectHandler
#undef DefTypeInheritedCollection
#undef DefTypeCursor

   Element& operator[](int index) { return (Element&) Cast::castFrom(GenericArray::operator[](index)); }
   const Element& operator[](int index) const { return (const Element&) Cast::castFrom(GenericArray::operator[](index)); }

   TVirtualCollection<Element, Cast>& getInterface()
      {  return (TVirtualCollection<Element, Cast>&) (VirtualCollection&) *this; }
   const TVirtualCollection<Element, Cast>& getInterface() const
      {  return (const TVirtualCollection<Element, Cast>&) (const VirtualCollection&) *this; }

   bool foreachDo(std::function<bool (const Element&)> function) const
      {  return GenericArray::foreachDo(TemplateElementCastParameters<Element, Cast>(), function); }
   bool foreachSDo(std::function<bool (Element&)> function) const
      {  return GenericArray::foreachDo(TemplateElementCastParameters<Element, Cast>(), function); }
   template <class Execute> bool foreachDo(Execute& function) const
      {  return GenericArray::foreachDo(TemplateElementCastParameters<Element, Cast>(), function); }
   template <class Execute> bool foreachDo(Execute& function, const ExtendedLocateParameters& params,
      const Cursor* start=nullptr, const Cursor* end=nullptr) const
      {  return GenericArray::foreachDo(TemplateElementCastParameters<Element, Cast>(), function,
            params, start, end);
      }
   bool foreachReverseDo(std::function<bool (const Element&)> function) const
      {  return GenericArray::foreachReverseDo(TemplateElementCastParameters<Element, Cast>(), function); }
   bool foreachSReverseDo(std::function<bool (Element&)> function) const
      {  return GenericArray::foreachReverseDo(TemplateElementCastParameters<Element, Cast>(), function); }
   template <class Execute> bool foreachReverseDo(Execute& function) const
      {  return GenericArray::foreachReverseDo(TemplateElementCastParameters<Element, Cast>(), function); }
   template <class Execute> bool foreachReverseDo(Execute& function, const ExtendedLocateParameters& params,
      const Cursor* start=nullptr, const Cursor* end=nullptr) const
      {  return GenericArray::foreachReverseDo(TemplateElementCastParameters<Element, Cast>(), function,
            params, start, end);
      }
   DeclareCollectionIteratorForConcreteCollect
};

/*************************************************/
/* Definition of the template class TArrayCursor */
/*************************************************/

template<class TypeElement, class Cast = HandlerCopyCast<TypeElement> >
class TArrayCursor : public GenericArrayCursor {
  public:
   TArrayCursor(const TArray<TypeElement, Cast>& support) : GenericArrayCursor(support) {}
   TArrayCursor(const TArrayCursor<TypeElement, Cast>& source) : GenericArrayCursor(source) {}
   Template2DefineCopy(TArrayCursor, TypeElement, Cast)
   Template2DefineCursorForAbstractCollect(TArray, TArrayCursor, TypeElement, Cast)

   TypeElement* getSElement() const { return (TypeElement*) Cast::castFrom(GenericArrayCursor::_getSElement()); }
   const TypeElement* getElement() const { return getSElement(); }
   const TypeElement& elementAt() const { return *getSElement(); }
   TypeElement& elementSAt() const { return *getSElement(); }

   bool setIndex(int index) { return GenericArrayCursor::setSureIndex(index); }
};

Template2InlineCollectionForAbstractCollect(TArray, TArrayCursor, Element, Cast)

template <class Element, class Cast>
class TArray<Element, Cast>::iterator : public VirtualCollection::titerator<TArray<Element, Cast>, Element> {
  private:
   typedef VirtualCollection::titerator<TArray<Element, Cast>, Element> inherited;

   iterator(const TArray<Element, Cast>& support) : inherited(support) {}

  protected:
   iterator(const typename inherited::Base& source) : inherited(source) {}
   friend class TArray<Element, Cast>;

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

template <class Element, class Cast>
inline typename TArray<Element, Cast>::iterator
TArray<Element, Cast>::begin() const {
   iterator result(*this);
   result.setToFirst();
   return result;
}

template <class Element, class Cast>
inline typename TArray<Element, Cast>::iterator
TArray<Element, Cast>::end() const
   {  return iterator(*this); }

template <class Element, class Cast>
class TArray<Element, Cast>::reverse_iterator : public VirtualCollection::treverse_iterator<TArray<Element, Cast>, Element> {
  private:
   typedef VirtualCollection::treverse_iterator<TArray<Element, Cast>, Element> inherited;

   reverse_iterator(const TArray<Element, Cast>& support) : inherited(support) {}
   friend class TArray<Element, Cast>;

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

template <class Element, class Cast>
inline typename TArray<Element, Cast>::reverse_iterator
TArray<Element, Cast>::rbegin() const
{  reverse_iterator result(*this);
   result.setToLast();
   return result;
}

template <class Element, class Cast>
inline typename TArray<Element, Cast>::reverse_iterator
TArray<Element, Cast>::rend() const
   {  return reverse_iterator(*this); }

} // end of namespace COL

#endif // COL_ArrayH
