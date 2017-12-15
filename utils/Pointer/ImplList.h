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
// File      : ImplList.h
// Description :
//   Definition of a class of double linked list. It provides an implementation
//   for the COL::List collection.
//

#ifndef COL_ImplListH
#define COL_ImplListH

#include "StandardClasses/StandardClasses.hpp"

namespace COL {

namespace DVirtualCollection {

#if defined(__GNUC__) && GCC_VERSION >= 40600
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Woverloaded-virtual"
#endif
class VirtualCastWithElement : public VirtualCast {
  private:
   typedef VirtualCast inherited;

  protected:
   DefineExtendedParameters(1, VirtualCast)
   void setCastWithElement(bool isValid=true) { mergeOwnField(isValid ? 1 : 0); }

  protected:
   virtual EnhancedObject* _castFrom(EnhancedObject* copyObject,
         EnhancedObject* sourceCollectionObject) const { AssumeUncalled return nullptr; }
   virtual EnhancedObject* _castTo(EnhancedObject* collectionObject,
         EnhancedObject* sourceCopyObject) const { AssumeUncalled return nullptr; }
   virtual EnhancedObject* _castFrom(EnhancedObject* copyObject) const
      {  return _castFrom(copyObject, nullptr); }
   virtual EnhancedObject* _castTo(EnhancedObject* collectionObject) const
      {  return _castTo(collectionObject, nullptr); }

  public:
   VirtualCastWithElement() {}
   VirtualCastWithElement(const VirtualCastWithElement& source) : inherited(source) {}

   bool isValid() const { return hasOwnField(); }
   bool isCastWithElement() const { return hasOwnField(); }

   EnhancedObject* castFrom(EnhancedObject* copyObject, EnhancedObject* sourceCollectionObject=nullptr) const
      {  return isValid() ? _castFrom(copyObject, sourceCollectionObject)
            : VirtualCast::castFrom(copyObject);
      }
   EnhancedObject& castFrom(EnhancedObject& copyObject) const
      {  return isValid() ? *_castFrom(&copyObject, nullptr)
            : VirtualCast::castFrom(copyObject);
      }
   EnhancedObject& castFrom(EnhancedObject& copyObject, EnhancedObject& sourceCollectionObject) const
      {  return isValid() ? *_castFrom(&copyObject, &sourceCollectionObject)
            : VirtualCast::castFrom(copyObject);
      }
   const EnhancedObject* castFrom(const EnhancedObject* copyObject, const EnhancedObject* sourceCollectionObject=nullptr) const
      {  return isValid() ? _castFrom(const_cast<EnhancedObject*>(copyObject),
                  const_cast<EnhancedObject*>(sourceCollectionObject))
            :  VirtualCast::castFrom(copyObject);
      }
   const EnhancedObject& castFrom(const EnhancedObject& copyObject) const
      {  return isValid() ? *_castFrom(const_cast<EnhancedObject*>(&copyObject), nullptr)
            :  VirtualCast::castFrom(copyObject);
      }
   const EnhancedObject& castFrom(const EnhancedObject& copyObject, const EnhancedObject& sourceCollectionObject) const
      {  return isValid() ? *_castFrom(const_cast<EnhancedObject*>(&copyObject),
                  const_cast<EnhancedObject*>(&sourceCollectionObject))
            :  VirtualCast::castFrom(copyObject);
      }

   EnhancedObject* castTo(EnhancedObject* collectionObject, EnhancedObject* sourceCopyObject=nullptr) const
      {  return isValid() ? _castTo(collectionObject, sourceCopyObject)
            :  VirtualCast::castTo(collectionObject);
      }
   EnhancedObject& castTo(EnhancedObject& collectionObject) const
      {  return isValid() ? *_castTo(&collectionObject, nullptr)
            :  VirtualCast::castTo(collectionObject);
      }
   EnhancedObject& castTo(EnhancedObject& collectionObject, EnhancedObject& sourceCopyObject) const
      {  return isValid() ? *_castTo(&collectionObject, &sourceCopyObject)
            :  VirtualCast::castTo(sourceCopyObject);
      }
   const EnhancedObject* castTo(const EnhancedObject* collectionObject, const EnhancedObject* sourceCopyObject=nullptr) const
      {  return isValid() ? _castTo(const_cast<EnhancedObject*>(collectionObject),
                  const_cast<EnhancedObject*>(sourceCopyObject))
            :  VirtualCast::castTo(sourceCopyObject);
      }
   const EnhancedObject& castTo(const EnhancedObject& collectionObject) const
      {  return isValid() ? *_castTo(const_cast<EnhancedObject*>(&collectionObject), nullptr)
            :  VirtualCast::castTo(collectionObject);
      }
   const EnhancedObject& castTo(const EnhancedObject& collectionObject, const EnhancedObject& sourceCopyObject) const
      {  return isValid() ? *_castTo(const_cast<EnhancedObject*>(&collectionObject),
                  const_cast<EnhancedObject*>(&sourceCopyObject))
            :  VirtualCast::castTo(collectionObject);
      }
};
#if defined(__GNUC__) && GCC_VERSION >= 40600
#pragma GCC diagnostic pop
#endif

} // end of namespace DVirtualCollection

class ImplListElement : public EnhancedObject {
  private:
   ImplListElement* pileNext;
   ImplListElement* pilePrevious;

  protected:
   ImplListElement() : pileNext(this), pilePrevious(this) {}
   ImplListElement(const ImplListElement& source)
      : EnhancedObject(source), pileNext(this), pilePrevious(this) {}

   void setNext(ImplListElement* next) { pileNext = next; }
   void setPrevious(ImplListElement* previous) { pilePrevious = previous; }
   ImplListElement* getNext() const { return pileNext; }
   ImplListElement* getPrevious() const { return pilePrevious; }

   friend class ImplList;
   void insertAsNext(ImplListElement& nextSource);
   ImplListElement* disconnect();
   ImplListElement* disconnect(ImplListElement& last);
   bool isAtomic() const { return (pileNext == this) && (pilePrevious == this); }

  public:
   DefineCopy(ImplListElement)
   DDefineAssign(ImplListElement)
   ImplListElement& operator=(const ImplListElement& source)
      {  return (ImplListElement&) EnhancedObject::operator=(source); }
   virtual bool isValid() const override
      {  return EnhancedObject::isValid() && (pileNext != nullptr) && (pilePrevious != nullptr)
            && (pileNext->pilePrevious == this) && (pilePrevious->pileNext == this);
      }
};

class ImplList : public EnhancedObject {
  private:
   typedef DVirtualCollection::VirtualCastWithElement VirtualCastWithElement;
   ImplListElement* pileFirst;

  protected:
   int queryNumberOfElementsBetween(ImplListElement* min, ImplListElement* max) const;
   int queryCount() { return (pileFirst != nullptr) ? queryNumberOfElementsBetween(pileFirst, pileFirst->getPrevious()) : 0; }

   ImplList(ImplListElement* first) : pileFirst(first) {}
   void lightDeconnect() { pileFirst = nullptr; }

  public:
   enum Direction { DNext, DPrevious };
   ImplList() : pileFirst(nullptr) {}
   ImplList(ImplList&& source) : pileFirst(source.pileFirst) { source.pileFirst = nullptr; }
   ImplList(const ImplList& source, bool doesDuplicate=false)
      :  EnhancedObject(source), pileFirst(nullptr)
      {  if (doesDuplicate) addCopyAll(source); }
   virtual ~ImplList() { if (pileFirst) removeAll(); }
   DefineCopy(ImplList)
   DDefineAssign(ImplList)
   ImplList& operator=(ImplList&& source)
      {  if (this != &source) {
            if (pileFirst) removeAll();
            pileFirst = source.pileFirst;
            source.pileFirst = nullptr;
         }
         return *this;
      }
   ImplList& operator=(const ImplList& source)
      {  return (ImplList&) EnhancedObject::operator=(source); }

   void assign(const ImplList& source, bool doesDuplicate,
         const VirtualCast* retrieveRegistrationFromCopy=nullptr);

     // query methods
   ComparisonResult compareCursors(ImplListElement* fst, ImplListElement* snd) const;
   bool isEmpty() const { return !pileFirst; }
   bool isSingleton() const { return pileFirst && (pileFirst->getNext() == pileFirst); }
   virtual bool isValid() const override;

   ImplListElement* getFirst() const { return pileFirst; }
   ImplListElement* getNext(ImplListElement* element) const
      {  AssumeCondition(element && element->ImplListElement::isValid())
         return element->getNext();
      }
   ImplListElement* getPrevious(ImplListElement* element) const
      {  AssumeCondition(element && element->ImplListElement::isValid())
         return element->getPrevious();
      }
   ImplListElement* getLast() const
      {  return pileFirst ? pileFirst->getPrevious() : nullptr; }
   template <class Execute, class TemplateParameters>
   bool foreachDo(TemplateParameters, Execute& function, ImplListElement* prevStart=nullptr,
      ImplListElement* nextEnd=nullptr) const;
   template <class Execute, class TemplateParameters>
   bool foreachReverseDo(TemplateParameters, Execute& function, ImplListElement* prevStart=nullptr,
      ImplListElement* nextEnd=nullptr) const;

     // Insertion
   void addAsFirst(ImplListElement* newElement)
      {  addAsNext(newElement, pileFirst, DPrevious); }
   void addAsLast(ImplListElement* newElement)
      {  addAsNext(newElement, pileFirst ? pileFirst->getPrevious() : nullptr, DNext); }
   void addAsNext(ImplListElement* newElement, ImplListElement* cursor, Direction direction);
   void add(ImplListElement* newElement) { addAsLast(newElement); }

     // Suppression
   void remove(ImplListElement* oldElement);

     // Methods to apply on all elements
   void freeAll();
   void removeAll() { while (pileFirst) pileFirst = pileFirst->disconnect(); }
   void freeAllExcept(ImplListElement* lastElement);
   void removeAllExcept(ImplListElement* lastElement);

   void moveAllTo(ImplList& destination, ImplListElement* start=nullptr,
         ImplListElement* end=nullptr, ImplListElement* destinationCursor=nullptr,
         bool doesAssertDistinction=false);
   void addCopyAll(const ImplList& source, ImplListElement* start=nullptr,
         ImplListElement* end=nullptr, ImplListElement* destinationCursor=nullptr,
         const VirtualCast* retrieveRegistrationFromCopy=nullptr);
   void swap(ImplList& source);

   class iterator {
     private:
      const ImplList* pilSupport;
      ImplListElement* pileCursor;

     public:
      iterator(const ImplList& support, ImplListElement* cursor)
         :  pilSupport(&support), pileCursor(cursor) {}
      iterator(const iterator& source) = default;
      ImplListElement& operator*() const { AssumeCondition(pileCursor) return *pileCursor; }
      ImplListElement* operator->() const { return pileCursor; }
      iterator& operator++()
         {  pileCursor = pileCursor ? pilSupport->getNext(pileCursor) : nullptr;
            if ((pileCursor = pilSupport->getFirst()) != nullptr)
               pileCursor = nullptr;
            return *this;
         }
      iterator& operator--()
         {  pileCursor = (pileCursor && pileCursor != pilSupport->getFirst())
                  ? pilSupport->getPrevious(pileCursor) : nullptr;
            return *this;
         }
   };
   iterator begin() const { return iterator(*this, pileFirst); }
   iterator end() const { return iterator(*this, nullptr); }
   typedef iterator const_iterator;

   class reverse_iterator {
     private:
      const ImplList* pilSupport;
      ImplListElement* pileCursor;

     public:
      reverse_iterator(const ImplList& support, ImplListElement* cursor)
         :  pilSupport(&support), pileCursor(cursor) {}
      reverse_iterator(const reverse_iterator& source) = default;
      ImplListElement& operator*() const { AssumeCondition(pileCursor) return *pileCursor; }
      ImplListElement* operator->() const { return pileCursor; }
      reverse_iterator& operator--()
         {  pileCursor = pileCursor ? pilSupport->getNext(pileCursor) : nullptr;
            if ((pileCursor = pilSupport->getFirst()) != nullptr)
               pileCursor = nullptr;
            return *this;
         }
      reverse_iterator& operator++()
         {  pileCursor = (pileCursor && pileCursor != pilSupport->getFirst())
                  ? pilSupport->getPrevious(pileCursor) : nullptr;
            return *this;
         }
   };

   typedef reverse_iterator const_reverse_iterator;
   reverse_iterator rbegin() const { return reverse_iterator(*this, pileFirst ? pileFirst->getPrevious() : nullptr); }
   reverse_iterator rend() const { return reverse_iterator(*this, nullptr); }

   bool foreachDo(std::function<bool (ImplListElement&)> func, ImplListElement* prevStart=nullptr,
      ImplListElement* nextEnd=nullptr) const;
   bool foreachReverseDo(std::function<bool (ImplListElement&)> func, ImplListElement* prevStart=nullptr,
      ImplListElement* nextEnd=nullptr) const;
};

inline void
ImplList::swap(ImplList& source) {
   ImplListElement* element = pileFirst;
   pileFirst = source.pileFirst;
   source.pileFirst = element;
}

/************************************/
/* Definition of template TImplList */
/************************************/

template<class Element, class Cast=SimpleCast>
class TImplList : public ImplList {
  private:
   typedef ImplList inherited;
   typedef TImplList<Element, Cast> thisType;

  protected:
   TImplList(Element* first) : ImplList((ImplListElement*) Cast::castTo(first)) {}

  public:
   TImplList() : ImplList() {}
   TImplList(thisType&& source) : ImplList(source) {}
   TImplList(const thisType& source) : ImplList(source) {}
   TImplList(const thisType& source, bool doesDuplicate) : ImplList(source, doesDuplicate) {}
   TImplList<Element, Cast>& operator=(const TImplList<Element, Cast>& source) = default;
   TImplList<Element, Cast>& operator=(TImplList<Element, Cast>&& source) = default;

   Template2DefineCopy(TImplList, Element, Cast)
   DTemplate2DefineAssign(TImplList, Element, Cast)
   void assign(const thisType& source, bool doesDuplicate)
      {  ImplList::assign(source, doesDuplicate); }

     // Query methods
   Element* getFirst() const
      {  return (Element*) Cast::castFrom(ImplList::getFirst()); }
   Element* getLast() const
      {  return (Element*) Cast::castFrom(ImplList::getLast()); }
   Element* getNext(Element* element) const
      {  return (Element*) Cast::castFrom(ImplList::getNext((ImplListElement*) Cast::castTo(element))); }
   Element* getPrevious(Element* element) const
      {  return (Element*) Cast::castFrom(ImplList::getPrevious((ImplListElement*) Cast::castTo(element))); }

     // Insertion
   void addAsFirst(Element* newElement)
      {  ImplList::addAsFirst((ImplListElement*) Cast::castTo(newElement)); }
   void addAsLast(Element* newElement)
      {  ImplList::addAsLast((ImplListElement*) Cast::castTo(newElement)); }
   void addAsNext(Element* newElement, Element* cursor, Direction dir)
      {  ImplList::addAsNext((ImplListElement*) Cast::castTo(newElement),
            (ImplListElement*) Cast::castTo(cursor), dir); }
   void add(Element* newElement)
      {  ImplList::add((ImplListElement*) Cast::castTo(newElement)); }

     // Suppression
   void remove(Element* oldElement)
      {  ImplList::remove((ImplListElement*) Cast::castTo(oldElement)); }
   void freeAllExcept(Element* lastElement)
      {  ImplList::freeAllExcept((ImplListElement*) Cast::castTo(lastElement)); }
   void removeAllExcept(Element* lastElement)
      {  ImplList::removeAllExcept((ImplListElement*) Cast::castTo(lastElement)); }

     // Methods to apply on all elements
   void moveAllTo(TImplList<Element, Cast>& destination, Element* first=nullptr,
         Element* last=nullptr, Element* destinationCursor=nullptr, bool doesAssertDistinction=false)
      { ImplList::moveAllTo(destination, (ImplListElement*) Cast::castTo(first),
            (ImplListElement*) Cast::castTo(last), (ImplListElement*) Cast::castTo(destinationCursor),
            doesAssertDistinction);
      }
   void addCopyAll(const TImplList<Element, Cast>& source, Element* first=nullptr,
         Element* last=nullptr, Element* destinationCursor=nullptr)
      { ImplList::addCopyAll(source, (ImplListElement*) Cast::castTo(first),
            (ImplListElement*) Cast::castTo(last), (ImplListElement*) Cast::castTo(destinationCursor));
      }
   void swap(TImplList<Element, Cast>& source) { ImplList::swap(source); }
   template <class Execute>
   bool foreachDo(Execute& function, Element* prevStart=nullptr, Element* nextEnd=nullptr) const
      {  return COL::ImplList::foreachDo(TemplateElementCastParameters<Element, Cast>(), function,
            (ImplListElement*) Cast::castTo(prevStart), (ImplListElement*) Cast::castTo(nextEnd));
      }
   template <class Execute>
   bool foreachReverseDo(Execute& function, Element* prevStart=nullptr, Element* nextEnd=nullptr) const
      {  return COL::ImplList::foreachReverseDo(TemplateElementCastParameters<Element, Cast>(), function,
            (ImplListElement*) Cast::castTo(prevStart), (ImplListElement*) Cast::castTo(nextEnd));
      }

   class iterator : public ImplList::iterator {
     private:
      typedef ImplList::iterator inherited;

     public:
      iterator(const TImplList<Element, Cast>& support, Element* cursor)
         :  inherited(support, (ImplListElement*) Cast::castTo(cursor)) {}
      iterator(const iterator& source) = default;

      Element& operator*() const { return (Element&) Cast::castFrom(inherited::operator*()); }
      Element* operator->() const { return (Element*) Cast::castFrom(inherited::operator->()); }
   };
   iterator begin() const { return iterator(*this, getFirst()); }
   iterator end() const { return iterator(*this, nullptr); }
   typedef iterator const_iterator;

   class reverse_iterator : public ImplList::reverse_iterator {
     private:
      typedef ImplList::reverse_iterator inherited;

     public:
      reverse_iterator(const TImplList<Element, Cast>& support, Element* cursor)
         :  inherited(support, (ImplListElement*) Cast::castTo(cursor)) {}
      reverse_iterator(const reverse_iterator& source) = default;

      Element& operator*() const { return (Element&) Cast::castFrom(inherited::operator*()); }
      Element* operator->() const { return (Element*) Cast::castFrom(inherited::operator->()); }
   };
   typedef reverse_iterator const_reverse_iterator;
   reverse_iterator rbegin() const { return reverse_iterator(*this, getLast()); }
   reverse_iterator rend() const { return reverse_iterator(*this, nullptr); }

   bool foreachDo(std::function<bool (Element&)> function, Element* prevStart=nullptr,
         Element* nextEnd=nullptr) const
      {  return COL::ImplList::foreachDo(TemplateElementCastParameters<Element, Cast>(), function,
            (ImplListElement*) Cast::castTo(prevStart), (ImplListElement*) Cast::castTo(nextEnd));
      }
   bool foreachCDo(std::function<bool (const Element&)> function, Element* prevStart=nullptr,
         Element* nextEnd=nullptr) const
      {  return COL::ImplList::foreachDo(TemplateElementCastParameters<Element, Cast>(), function,
            (ImplListElement*) Cast::castTo(prevStart), (ImplListElement*) Cast::castTo(nextEnd));
      }

   bool foreachReverseDo(std::function<bool (Element&)> function, Element* prevStart=nullptr,
         Element* nextEnd=nullptr) const
      {  return COL::ImplList::foreachReverseDo(TemplateElementCastParameters<Element, Cast>(), function,
            (ImplListElement*) Cast::castTo(prevStart), (ImplListElement*) Cast::castTo(nextEnd));
      }
   bool foreachCReverseDo(std::function<bool (const Element&)> function, Element* prevStart=nullptr,
         Element* nextEnd=nullptr) const
      {  return COL::ImplList::foreachReverseDo(TemplateElementCastParameters<Element, Cast>(), function,
            (ImplListElement*) Cast::castTo(prevStart), (ImplListElement*) Cast::castTo(nextEnd));
      }
};

} // end of namespace COL

#endif // COL_ImplListH


