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
// Library   : Pointer
// Unit      : Elaborated pointers
// File      : SharedCollection.h
// Description :
//   Definition of the classes SharedCollection, SharedCollection::GlobalElement
//   and SharedCollection::LocalElement. These classes manage shared elements that
//   can notify the collections that point onto them. It extends the class
//   SharedPointer with multiple pointed elements.
//

#ifndef PNT_SharedCollectionH
#define PNT_SharedCollectionH

#include "Pointer/SharedPointer.h"
#include "Pointer/ImplArray.h"
#include "Pointer/Binary.h"

namespace PNT {

class SharedCollection;

namespace DSharedCollection {

class Registration : public COL::ImplListElement {
  private:
   typedef COL::ImplListElement inherited;

  public:
   Registration() {}
   Registration(const Registration& source) : inherited(source) {}
};

/*************************************/
/* Definition of class GlobalElement */
/*************************************/

class LocalElement;
class GlobalElement : public Registration, public SharedPointer {
  private:
   typedef Registration inherited;
   SharedCollection* pscParent;

  protected:
   virtual ComparisonResult _compare(const EnhancedObject& asource) const override
      {  return inherited::_compare(asource); }
   void disconnect() { SharedPointer::disconnect(); }
   
  public:
   GlobalElement() : pscParent(nullptr) {}
   GlobalElement(LocalElement* element, Init);
   GlobalElement(const GlobalElement& source)
      :  inherited(source), SharedPointer(source), pscParent(nullptr) {}
   GlobalElement& operator=(const GlobalElement& source)
      {  inherited::operator=(source);
         SharedPointer::operator=(source);
         return *this;
      }
   DefineCopy(GlobalElement)
   DDefineAssign(GlobalElement)
   StaticInheritConversions(GlobalElement, inherited)

   virtual void assignGlobal(const GlobalElement& source) {}
   virtual bool isValid() const override { return SharedPointer::isValid(); }
   bool invariant() const
      {  return Registration::isValid() && SharedPointer::isValid() && pscParent; }
   bool invariant(const SharedCollection& parent) const
      {  return Registration::isValid() && SharedPointer::isValid() && (pscParent == &parent); }
   typedef HandlerMultiCast<GlobalElement, SharedPointer, SharedPointer::HandlerRegisterCast> CallCast;
   typedef HandlerIntermediateCast<GlobalElement, Registration, COL::ImplListElement> HandlerRegisterCast;
   friend class LocalElement;
   friend class PNT::SharedCollection;
};

/************************************/
/* Definition of class LocalElement */
/************************************/

class UpdateCopyTable;
class LocalElement : public SharedElement {
  public:
   typedef TPointerNotification<SharedCollection> Notification;

  protected:
   typedef COL::TImplList<GlobalElement, GlobalElement::CallCast> GlobalElementList;
     
  private:
   typedef SharedElement inherited;

  protected:
   GlobalElementList& calls() { return (GlobalElementList&) (COL::ImplList&) inherited::calls(); }
   const GlobalElementList& calls() const { return (const GlobalElementList&) (const COL::ImplList&) inherited::calls(); }

   void unlock(GlobalElement& call)  { inherited::unlock(call); }
   void lock(GlobalElement& call)    { inherited::lock(call); }
   void freeAllCalls();
   void freeCall(SharedCollection* collection);
   void removeAllCalls();
   void removeCall(SharedCollection& collection);
   void moveAllCallsTo(LocalElement& destination, bool doesAvoidDuplication);
   void duplicateCalls(const LocalElement& source, UpdateCopyTable& table,
         const VirtualCast* retrieveRegistrationFromCopy=nullptr);
   void notifyUpdate(Notification& notification) const;

  public:
   LocalElement() {}
   LocalElement(const LocalElement& source) : inherited(source) {}
   virtual ~LocalElement() { removeAllCalls(); }
   DefineCopy(LocalElement)
   DDefineAssign(LocalElement)

   bool isReferenced() const { return !calls().isEmpty(); }
   bool isSingleReferenced() const
      {  return !calls().isEmpty() && (calls().getFirst() == calls().getLast()); }
   friend class PNT::SharedCollection;
};

inline
GlobalElement::GlobalElement(LocalElement* element, Init init)
   : SharedPointer(element, init), pscParent(nullptr) {}

inline void
LocalElement::notifyUpdate(Notification& notification) const {
   calls().foreachDo([&notification](GlobalElement& call)
      {  AssumeCondition(call.pscParent)
         if (call.pscParent != notification.getOrigin())
            notification(*call.pscParent);
         return true;
      });
}

class UpdateCopyConnection : public COL::ImplBinaryTree::Node {
  private:
   typedef COL::ImplBinaryTree::Node inherited;
   const EnhancedObject* peoSource;
   EnhancedObject* peoTarget;

  protected:
   virtual ComparisonResult _compare(const EnhancedObject& asource) const override
      {  ComparisonResult result = inherited::_compare(asource);
         return (result == CREqual)
            ? tcompare(peoSource, ((const UpdateCopyConnection&) asource).peoSource)
            : result;
      }

  public:
   UpdateCopyConnection(const EnhancedObject& source, EnhancedObject& target)
      :  peoSource(&source), peoTarget(&target) {}
   UpdateCopyConnection(const UpdateCopyConnection& source) = default;
   DefineCopy(UpdateCopyConnection)
   DDefineAssign(UpdateCopyConnection)
      
   const EnhancedObject& source() const { return *peoSource; }
   EnhancedObject& target() const { return *peoTarget; }
   const LocalElement& sourceAsElement() const { return (const LocalElement&) *peoSource; }
   LocalElement& targetAsElement() const { return (LocalElement&) *peoTarget; }
};

class UpdateCopyTable : public COL::ImplBinaryTree {
  private:
   typedef COL::ImplBinaryTree inherited;
   enum Type { TUndefined, TElement, TCollection };
   Type tType;

  public:
   UpdateCopyTable() : tType(TUndefined) {}
   UpdateCopyTable(const UpdateCopyTable& source) : inherited(source, true), tType(source.tType) {}
   virtual ~UpdateCopyTable() { freeAll(); }
   UpdateCopyTable& operator=(const UpdateCopyTable& source)
      {  inherited::operator=(source);
         if (this != &source)
            duplicate(source);
         return *this;
      }
   DefineCopy(UpdateCopyTable)
   DDefineAssign(UpdateCopyTable)
   
   LocalElement& getTarget(const LocalElement& source) const
      {  AssumeCondition(tType == TElement)
         Cursor cursor(*this);
         EnhancedObject* fakeTarget = nullptr;
         Node::Balance balance = locateBefore(UpdateCopyConnection(source, *fakeTarget), cursor);
         AssumeCondition(balance == Node::BEqual)
         return (LocalElement&) ((UpdateCopyConnection*) cursor.elementAt())->target();
      }
   void addConnection(const LocalElement& source, LocalElement& target)
      {  AssumeCondition(tType <= TElement)
         if (tType == TUndefined) tType = TElement;
         add(new UpdateCopyConnection(source, target));
      }

   template <class TypeCollection>
   void setFrom(const TypeCollection& source, const TypeCollection& copy)
      {  AssumeCondition(tType <= TElement && source.count() == copy.count())
         if (tType == TUndefined) tType = TElement;
         typename TypeCollection::Cursor sourceCursor(source), copyCursor(copy);
         if (copyCursor.setToFirst()) {
            sourceCursor.setToFirst();
            do {
               add(new UpdateCopyConnection((const LocalElement&) sourceCursor.elementAt(),
                        (LocalElement&) copyCursor.elementSAt()));
               copyCursor.setToNext();
            } while (sourceCursor.setToNext());
            AssumeCondition(!sourceCursor.isValid() && !copyCursor.isValid())
         };
      }
   friend class LocalElement;
};

} // end of namespace DSharedCollection

/****************************************/
/* Definition of class SharedCollection */
/****************************************/

class SharedCollection : public COL::TImplList<DSharedCollection::GlobalElement,
      DSharedCollection::GlobalElement::HandlerRegisterCast> {
  public:
   typedef TPointerNotification<SharedCollection> Notification;

  protected:
   typedef DSharedCollection::GlobalElement GlobalElement;

  private:
   typedef DSharedCollection::LocalElement LocalElement;
   typedef COL::TImplList<GlobalElement, GlobalElement::HandlerRegisterCast> inherited;
   int uReferences;
   
   typedef COL::TImplList<GlobalElement, GlobalElement::CallCast> GlobalElementList;
   static SharedCollection* parent(const GlobalElement& global) { return global.pscParent; }

   void free(GlobalElement* element)
      {  AssumeCondition(uReferences >= 1)
         inherited::remove(element);
         --uReferences;
         if (element)
            delete element;
      }
   friend class DSharedCollection::LocalElement;

   class GlobalNotification {
     private:
      class VisitedCollections : public COL::TImplArray<SharedCollection> {
        private:
         typedef COL::TImplArray<SharedCollection> inherited;
         class SortedParameters {
           public:
            typedef SharedCollection Element;
            typedef HandlerCast<SharedCollection> Cast;
            static const SharedCollection* key(const SharedCollection& collection)
               {  return &collection; };
            class Key {
              public:
               typedef const SharedCollection* KeyType;
               static ComparisonResult compare(KeyType fstKey, KeyType sndKey)
                  {  return tcompare(fstKey, sndKey); }
            };
            static ComparisonResult keyCompare(Key::KeyType fst, Key::KeyType snd)
               {  return Key::compare(fst, snd); }
         };

        public:
         VisitedCollections() {}
         VisitedCollections(const VisitedCollections& source) : inherited(source) {}
         DefineCopy(VisitedCollections)

         bool locateOrAdd(const SharedCollection& parent)
            {  int localization = inherited::localize(&parent, SortedParameters());
               bool hasResult = (localization & 1);
               if (!hasResult)
                  inherited::insertAt(localization >> 1, (SharedCollection*) &parent);
               return hasResult;
            }
      };

      Notification& pnNotification;
      VisitedCollections vcVisited;

     public:
      GlobalNotification(Notification& notification) : pnNotification(notification) {}
      bool operator()(GlobalElement& call)
         {  AssumeCondition(call.pscParent)
            SharedCollection* parent = call.pscParent;
            if ((parent != pnNotification.getOrigin()) && !vcVisited.locateOrAdd(*parent))
               pnNotification(*parent);
            return true;
         }
   };
   friend class GlobalNotification;

   class LinearGlobalNotification {
     private:
      Notification& pnNotification;
      bool fFirst;

     public:
      LinearGlobalNotification(Notification& notification)
         :  pnNotification(notification), fFirst(true) {}
      bool operator()(GlobalElement& call)
         {  AssumeCondition(call.pscParent)
            SharedCollection* parent = call.pscParent;
            if ((parent != pnNotification.getOrigin())
                  && (fFirst || (parent->getGFirst() == &call)))
               pnNotification(*parent);
            return true;
         }
      void setAfterFirst() { fFirst = false; }
   };
   friend class LinearGlobalINotification;

  protected:
   void _lightAssign(const EnhancedObject& asource) { inherited::_assign(asource); }
   void fullAssign(const SharedCollection& source)
      {  if (this != &source) {
            inherited::foreachDo([this](GlobalElement& thisGlobal)
               {  LocalElement* element = (LocalElement*) &thisGlobal.getElement();
                  element->unlock(thisGlobal);
                  --uReferences;
                  if (!element->isReferenced())
                     delete element;
                  return true;
               });
            AssumeCondition(uReferences == 0)
            inherited::freeAll();
            source.inherited::foreachDo([this](GlobalElement& sourceGlobal)
               {  GlobalElement* thisGlobal = createGlobal();
                  thisGlobal->assignGlobal(sourceGlobal);
                  thisGlobal->pscParent = this;
                  addAsLast(thisGlobal);
                  ++uReferences;
                  ((LocalElement&) sourceGlobal.getElement()).lock(*thisGlobal);
                  return true;
               });
         };
      }

   void disconnect() { inherited::freeAll(); uReferences = 0; }
   void release()
      {  inherited::foreachDo([this](GlobalElement& thisGlobal)
            {  LocalElement* element = (LocalElement*) &thisGlobal.getElement();
               element->unlock(thisGlobal);
               --uReferences;
               if (!element->isReferenced())
                  delete element;
               return true;
            });
         AssumeCondition(uReferences == 0)
         inherited::freeAll();
      }
   void notifyUpdate(Notification& notification) const
      {  notification.setOrigin(*this);
         GlobalNotification globalNotification(notification);
         inherited::foreachDo([&notification, &globalNotification](GlobalElement& globalThis)
            {  LocalElement& element = (LocalElement&) globalThis.getElement();
               if (!element.calls().isEmpty() && ((!notification.getOrigin()
                     || !element.calls().isSingleton())))
                  element.calls().foreachDo(globalNotification);
               return true;
            });
      }
   void linearNotifyUpdate(Notification& notification) const
      {  notification.setOrigin(*this);
         LinearGlobalNotification globalNotification(notification);
         inherited::foreachDo([&notification, &globalNotification](GlobalElement& globalThis)
            {  LocalElement& element = (LocalElement&) globalThis.getElement();
               if (!element.calls().isEmpty() && ((!notification.getOrigin()
                     || !element.calls().isSingleton())))
                  element.calls().foreachDo(globalNotification);
               globalNotification.setAfterFirst();
               return true;
            });
      }

  public:
   class Element : public LocalElement {
     private:
      typedef LocalElement inherited;
      
     public:
      Element() {}
      Element(const Element& source) : inherited(source) {}
      DefineCopy(Element)

      typedef LinearGlobalNotification LinearUpdate;
      typedef GlobalNotification Update;

      void notifyUpdate(Notification& notification) const { inherited::notifyUpdate(notification); }
      void notifyUpdate(Update& update) const { calls().foreachDo(update); }
      void linearNotifyUpdate(LinearUpdate& update) const { calls().foreachDo(update); }
   };

  protected:
   virtual GlobalElement* createGlobal() { return new GlobalElement; }
   virtual GlobalElement* createGlobal(Element& element) { return new GlobalElement(&element, Pointer::Init()); }

  public:
   typedef LinearGlobalNotification LinearUpdate;
   typedef GlobalNotification Update;
   typedef DSharedCollection::UpdateCopyTable UpdateCopyTable;
      
   SharedCollection() : uReferences(0) {}
   SharedCollection(SharedCollection&& source)
      :  inherited(source), uReferences(source.uReferences)
      {  inherited::swap(source);
         inherited::foreachDo([this](GlobalElement& global)
            {  global.pscParent = this; return true; });
      }
   SharedCollection(const SharedCollection& source)
      :  inherited(source, true), uReferences(source.uReferences)
      {  inherited::foreachDo([this](GlobalElement& global)
            {  global.pscParent = this; return true; });
      }
   SharedCollection(const SharedCollection& source, const UpdateCopyTable& updateTable)
      :  inherited(source, true), uReferences(source.uReferences)
      {  inherited::foreachDo([this, &updateTable](GlobalElement& global)
            {  LocalElement& source = (LocalElement&) global.getElement();
               LocalElement& target = updateTable.getTarget(source);
               global.pscParent = this;
               source.unlock(global);
               target.lock(global);
               return true;
            });
      }
   virtual ~SharedCollection() { inherited::freeAll(); }
   SharedCollection& operator=(const SharedCollection& source)
      {  inherited::operator=(source);
         if (this != &source) {
            inherited::freeAll();
            uReferences = 0;
            source.inherited::foreachDo([this](GlobalElement& source)
               {  GlobalElement* thisGlobal = createGlobal();
                  thisGlobal->assignGlobal(source);
                  thisGlobal->pscParent = this;
                  addAsLast(thisGlobal);
                  ++uReferences;
                  ((LocalElement&) source.getElement()).lock(*thisGlobal);
                  return true;
               });
         };
         return *this;
      }
   SharedCollection& assignFromGlobal(const SharedCollection& source)
      {  inherited::operator=(source);
         if (this != &source) {
            inherited::freeAll();
            uReferences = 0;
            source.inherited::foreachDo([this](GlobalElement& source)
               {  GlobalElement* thisGlobal = source.createSCopy();
                  thisGlobal->pscParent = this;
                  addAsLast(thisGlobal);
                  ++uReferences;
                  return true;
               });
         };
         return *this;
      }
   SharedCollection& operator=(SharedCollection&& source)
      {  inherited::operator=(source);
         if (this != &source) {
            inherited::freeAll();
            inherited::swap((SharedCollection&) source);
            uReferences = source.uReferences;
            source.uReferences = 0;
            inherited::foreachDo([this](GlobalElement& global)
               {  global.pscParent = this; return true; });
         };
         return *this;
      }
   DefineCopy(SharedCollection)
   DDefineAssign(SharedCollection)
   void duplicate(const SharedCollection& source)
      {  AssumeCondition(uReferences == 0)
         inherited intern(source, true);
         inherited::swap(intern);
         uReferences = source.uReferences;
         inherited::foreachDo([this](GlobalElement& global)
            { global.pscParent = this; return true; });
      }
   void duplicate(const SharedCollection& source, const UpdateCopyTable& updateTable)
      {  AssumeCondition(uReferences == 0)
         inherited intern(source, true);
         inherited::swap(intern);
         uReferences = source.uReferences;
         inherited::foreachDo([this, &updateTable](GlobalElement& global)
            {  LocalElement& source = (LocalElement&) global.getElement();
               LocalElement& target = updateTable.getTarget(source);
               global.pscParent = this;
               source.unlock(global);
               target.lock(global);
               return true;
            });
      }
   void correctDuplicationWith(const UpdateCopyTable& updateTable)
      {  inherited::foreachDo([&updateTable](GlobalElement& global)
            {  LocalElement& source = (LocalElement&) global.getElement();
               LocalElement& target = updateTable.getTarget(source);
               source.unlock(global);
               target.lock(global);
               return true;
            });
      }
   GlobalElement* absorbGlobalElement(GlobalElement* global)
      {  AssumeAllocation(global)
         inherited::addAsLast(global);
         global->pscParent = this;
         ++uReferences;
         return global;
      }
   GlobalElement* addElement(Element& element)
      {  GlobalElement* global = createGlobal(element);
         inherited::addAsLast(global);
         global->pscParent = this;
         ++uReferences;
         return global;
      }
   GlobalElement* addElementAfter(Element& element, Element* previous)
      {  GlobalElement* global = nullptr;
         if (!previous)
            inherited::addAsFirst(global = createGlobal(element));
         else {
            GlobalElement* foundGlobal = nullptr;
            previous->calls().foreachDo([this, &foundGlobal](GlobalElement& globalElement)
               {  if (globalElement.pscParent == this)
                     foundGlobal = &globalElement;
                  return !foundGlobal;
               });
            if (foundGlobal)
               inherited::addAsNext(global = createGlobal(element), foundGlobal, inherited::DNext);
            else
               inherited::addAsFirst(global = createGlobal(element));
         };
         global->pscParent = this;
         ++uReferences;
         return global;
      }
   class Cursor;
   GlobalElement* absorbGlobalElementAfter(GlobalElement* global, Cursor& cursor)
      {  AssumeAllocation(global)
         if (!cursor.isValid())
            inherited::addAsFirst(global);
         else
            inherited::addAsNext(global, &cursor.globalElementSAt(), inherited::DNext);
         global->pscParent = this;
         ++uReferences;
         return global;
      }
   GlobalElement* addElementAfter(Element& element, Cursor& cursor)
      {  GlobalElement* global = nullptr;
         if (!cursor.isValid())
            inherited::addAsFirst(global = createGlobal(element));
         else
            inherited::addAsNext(global = createGlobal(element), &cursor.globalElementSAt(), inherited::DNext);
         global->pscParent = this;
         ++uReferences;
         return global;
      }
   void replaceAndFreeGlobalElementAtWith(Cursor& cursor, GlobalElement* newElement)
      {  if (!cursor.isValid()) {
            if (newElement) delete newElement;
            AssumeUncalled
         };
         if (&cursor.globalElementAt() == newElement)
            return;
         AssumeAllocation(newElement)
         GlobalElement* replacedElement = &cursor.globalElementSAt();
         // no other cursor should point on replacedElement !
         inherited::addAsNext(newElement, replacedElement, inherited::DNext);
         inherited::remove(replacedElement);
         delete replacedElement;
         cursor.setToGlobalElement(*newElement);
      }
   void replaceAndFreeGlobalElementAtWithCopy(Cursor& cursor, const GlobalElement& element)
      {  AssumeCondition(cursor.isValid())
         GlobalElement* replacedElement = &cursor.globalElementSAt();
         // no other cursor should point on replacedElement !
         GlobalElement* newElement = element.createSCopy();
         inherited::addAsNext(newElement, replacedElement, inherited::DNext);
         inherited::remove(replacedElement);
         delete replacedElement;
         cursor.setToGlobalElement(*newElement);
      }
   void replaceElementAtWith(Cursor& cursor, Element& element)
      {  cursor.globalElementSAt().setElement(element); }
   void replaceAndFreeElementAtWith(Cursor& cursor, Element& element)
      {  Element* oldElement = &cursor.elementSAt();
         cursor.globalElementSAt().setElement(element);
         if (oldElement)
            delete oldElement;
      }
   GlobalElement* addElementBefore(Element& element, Element* after)
      {  GlobalElement* global = nullptr;
         if (!after)
            inherited::addAsLast(global = createGlobal(element));
         else {
            GlobalElement* foundGlobal = nullptr;
            after->calls().foreachDo([this, &foundGlobal](GlobalElement& globalElement)
               {  if (globalElement.pscParent == this)
                     foundGlobal = &globalElement;
                  return !foundGlobal;
               });
            if (foundGlobal)
               inherited::addAsNext(global = createGlobal(element), foundGlobal, inherited::DPrevious);
            else
               inherited::addAsLast(global = createGlobal(element));
         };
         global->pscParent = this;
         ++uReferences;
         return global;
      }
   GlobalElement* absorbGlobalElementBefore(GlobalElement* global, Cursor& cursor)
      {  AssumeAllocation(global)
         if (!cursor.isValid())
            inherited::addAsLast(global);
         else
            inherited::addAsNext(global, &cursor.globalElementSAt(), inherited::DPrevious);
         global->pscParent = this;
         ++uReferences;
         return global;
      }
   GlobalElement* addElementBefore(Element& element, Cursor& cursor)
      {  GlobalElement* global = nullptr;
         if (!cursor.isValid())
            inherited::addAsLast(global = createGlobal(element));
         else
            inherited::addAsNext(global = createGlobal(element), &cursor.globalElementSAt(), inherited::DPrevious);
         global->pscParent = this;
         ++uReferences;
         return global;
      }
   void removeElement(Element& element)
      {  GlobalElement* foundGlobal = nullptr;
         // if (element.calls().count() <= uReferences)
         element.calls().foreachDo([this, &foundGlobal](GlobalElement& globalElement)
            {  if (globalElement.pscParent == this)
                  foundGlobal = &globalElement;
               return !foundGlobal;
            });
         AssumeCondition(foundGlobal)
         inherited::remove(foundGlobal);
         delete foundGlobal;
         --uReferences;
      }
   void removeAt(Cursor& cursor)
      {  inherited::remove(&cursor.globalElementSAt());
         delete &cursor.globalElementSAt();
         cursor.clear();
         --uReferences;
      }
   GlobalElement* extractAt(Cursor& cursor)
      {  inherited::remove(&cursor.globalElementSAt());
         GlobalElement* result = &cursor.globalElementSAt();
         cursor.clear();
         --uReferences;
         return result;
      }
   void freeElement(Element* element)
      {  if (element) {
            GlobalElement* foundGlobal = nullptr;
            // if (element->calls().count() <= uReferences)
            element->calls().foreachDo([this, &foundGlobal](GlobalElement& globalElement)
               {  if (globalElement.pscParent == this)
                     foundGlobal = &globalElement;
                  return !foundGlobal;
               });
            AssumeCondition(foundGlobal)
            inherited::remove(foundGlobal);
            delete foundGlobal;
            if (!element->isReferenced())
               delete element;
            --uReferences;
         };
      }
   void freeAt(Cursor& cursor)
      {  GlobalElement* global = &cursor.globalElementSAt();
         LocalElement* element = &cursor.elementSAt();
         inherited::remove(global);
         delete global;
         cursor.clear();
         if (!element->isReferenced())
            delete element;
         --uReferences;
      }
   void freeAll() { release(); }
   void freeAllExcept(Element* element)
      {  GlobalElement* localResult = nullptr;
         inherited::foreachDo([this, element, &localResult](GlobalElement& source)
            {  LocalElement* localElement = (LocalElement*) &source.getElement();
               if (localElement != element) {
                  localElement->unlock(source);
                  --uReferences;
                  if (!localElement->isReferenced())
                     delete localElement;
               }
               else
                  localResult = &source;
               return true;
            });
         AssumeCondition(element ? (uReferences == 1) : (uReferences == 0))
         inherited::freeAllExcept(localResult);
      }
   void removeAll() { disconnect(); }
   void removeAllExcept(Element* element)
      {  GlobalElement* locateResult = nullptr;
         bool hasNotLocated = inherited::foreachDo([element, &locateResult](GlobalElement& source)
            {  LocalElement* localElement = (LocalElement*) &source.getElement();
               if (localElement == element)
                  locateResult = &source;
               return locateResult == nullptr;
            });
         AssumeCondition(element ? !hasNotLocated : hasNotLocated)
         inherited::freeAllExcept(locateResult);
         uReferences = element ? 1 : 0;
      }
   void moveTo(Cursor& cursor, SharedCollection& receiver)
      {  GlobalElement* global = &cursor.globalElementSAt();
         remove(global);
         receiver.addAsLast(global);
         global->pscParent = &receiver;
         cursor.clear();
         --uReferences;
         ++receiver.uReferences;
      }
   void moveAllAfterTo(Cursor& cursor, SharedCollection& receiver)
      {  typedef GlobalElement::HandlerRegisterCast GlobalCast;
         inherited temp;
         int countMoves = queryNumberOfElementsBetween(
               cursor.isValid() ? GlobalCast::castTo(&cursor.globalElementSAt()) : nullptr,
               GlobalCast::castTo(inherited::getLast()));
         inherited::moveAllTo(temp, cursor.isValid() ? &cursor.globalElementSAt() : nullptr);
         cursor.clear();
         temp.foreachDo([this](GlobalElement& global)
               {  global.pscParent = this; return true; });
         uReferences -= countMoves;
         temp.moveAllTo(receiver);
         receiver.uReferences += countMoves;
      }

   class Cursor {
     private:
      SharedCollection& scCollection;
      GlobalElement* pgeElement;
      
     public:
      Cursor(const SharedCollection& collection)
         :  scCollection(const_cast<SharedCollection&>(collection)), pgeElement(nullptr) {}
      Cursor(const Cursor& source) : scCollection(source.scCollection), pgeElement(source.pgeElement) {}
      Cursor& operator=(const Cursor& source)
         {  AssumeCondition(&scCollection == &source.scCollection)
            pgeElement = source.pgeElement;
            return *this;
         }

      void clear() { pgeElement = nullptr; }
      void setToGlobalElement(GlobalElement& element) { pgeElement = &element; }
      bool setToFirst() { pgeElement = scCollection.getGFirst(); return pgeElement; }
      bool setToLast() { pgeElement = scCollection.getGLast(); return pgeElement; }
      bool setToNext()
         {  if (pgeElement) {
               if ((pgeElement = scCollection.getNext(pgeElement)) == scCollection.getGFirst())
                  pgeElement = nullptr;
            }
            else
               pgeElement = scCollection.getGFirst();
            return pgeElement;
         }
      bool setToPrevious()
         {  if (pgeElement) {
               if ((pgeElement = scCollection.getPrevious(pgeElement)) == scCollection.getGLast())
                  pgeElement = nullptr;
            }
            else
               pgeElement = scCollection.getGLast();
            return pgeElement;
         }
      bool isValid() const { return pgeElement; }
      Element& elementSAt() const
         {  AssumeCondition(pgeElement) return (Element&) pgeElement->getElement(); }
      const Element& elementAt() const
         {  AssumeCondition(pgeElement) return (const Element&) pgeElement->getCElement(); }
      GlobalElement& globalElementSAt() const
         {  AssumeCondition(pgeElement) return *pgeElement; }
      const GlobalElement& globalElementAt() const
         {  AssumeCondition(pgeElement) return *pgeElement; }
   };
   GlobalElement* getGFirst() const { return inherited::getFirst(); }
   GlobalElement* getGLast() const { return inherited::getLast(); }
   Element& getFirst() const { AssumeCondition(inherited::getFirst()) return (Element&) inherited::getFirst()->getElement(); }
   Element& getLast() const { AssumeCondition(inherited::getLast()) return (Element&) inherited::getLast()->getElement(); }
   Element* getPFirst() const
      {  GlobalElement* element = inherited::getFirst();
         return element ? (Element*) &element->getElement() : nullptr;
      }
   Element* getPLast() const
      {  GlobalElement* element = inherited::getLast();
         return element ? (Element*) &element->getElement() : nullptr;
      }
   bool isEmpty() const { return inherited::isEmpty(); }
   class ForeachElementCast {
     public:
      typedef GlobalElement Base;
      static Element& castFrom(GlobalElement& source) { return (Element&) source.getElement(); }
      static Element* castFrom(GlobalElement* source) { return &(Element&) source->getElement(); }
      static const Element& castFrom(const GlobalElement& source) { return (const Element&) source.getElement(); }
      static const Element* castFrom(const GlobalElement* source) { return &(const Element&) source->getElement(); }
   };
   typedef HandlerLinkedCast<Element, ForeachElementCast, DSharedCollection::GlobalElement::HandlerRegisterCast>
      ForeachListElementCast;
   bool foreachDo(std::function<bool (Element&)> function) const
      {  return COL::ImplList::foreachDo(TemplateElementCastParameters<Element, ForeachListElementCast>(), function); }
   bool foreachCDo(std::function<bool (const Element&)> function) const
      {  return COL::ImplList::foreachDo(TemplateElementCastParameters<Element, ForeachListElementCast>(), function); }
   bool foreachReverseDo(std::function<bool (Element&)> function) const
      {  return COL::ImplList::foreachReverseDo(TemplateElementCastParameters<Element, ForeachListElementCast>(), function); }
   bool foreachCReverseDo(std::function<bool (const Element&)> function) const
      {  return COL::ImplList::foreachReverseDo(TemplateElementCastParameters<Element, ForeachListElementCast>(), function); }
   
   friend class Cursor;
   int count() const { return uReferences; }
   void swap(SharedCollection& source)
      {  inherited::swap(source);
         inherited::foreachDo([this](GlobalElement& global)
            {  global.pscParent = this; return true; });
         source.inherited::foreachDo([&source](GlobalElement& global)
            {  global.pscParent = &source; return true; });
         int temp = uReferences;
         uReferences = source.uReferences;
         source.uReferences = temp;
      }

   virtual bool isValid() const override { return !inherited::isEmpty(); }
   bool invariant() const
      {  enum Result { RNotFound, RFoundOne, RFoundMultiple };
         Result result = RNotFound;
         auto globalInvariant = [this, &result](const GlobalElement& global)
            {  if (global.pscParent == this)
                  result = (result == RNotFound) ? RFoundOne : RFoundMultiple;
               return result != RFoundMultiple;
            };
         return inherited::foreachCDo([this, &globalInvariant, &result](const GlobalElement& global)
            {  ((const LocalElement&) *global).calls().foreachCDo(globalInvariant);
               bool hasResult = (result == RFoundOne);
               if (hasResult)
                  result = RNotFound;
               return hasResult && global.invariant(*this);
            });
      }
};

namespace DSharedCollection {

inline void
LocalElement::freeCall(SharedCollection* collection) {
   if (collection) {
      collection->removeElement((SharedCollection::Element&) *this);
      if (collection->count() == 0)
         delete collection;
   };
}

inline void
LocalElement::removeCall(SharedCollection& collection) {
   collection.removeElement((SharedCollection::Element&) *this);
}
      
} // end of namespace DSharedCollection

/********************************************/
/* Definition of template TSharedCollection */
/********************************************/

template <class TypeElement, class Cast>
class TSharedCollection : public SharedCollection {
  private:
   typedef SharedCollection inherited;
   typedef TSharedCollection<TypeElement, Cast> thisType;
   typedef inherited::Cursor InheritedCursor;
   
  public:
   typedef Pointer::Init Init;
   typedef inherited::GlobalElement GlobalElement;

   TSharedCollection() {}
   TSharedCollection(TypeElement* element, Init) : inherited(Cast::castTo(element), Init()) {}

   TSharedCollection(thisType&& source) = default;
   TSharedCollection(const thisType& source) = default;
   thisType& operator=(const thisType& source) = default;
   thisType& operator=(thisType&& source) = default;
   Template2DefineCopy(TSharedCollection, TypeElement, Cast)

   GlobalElement* addElement(TypeElement& element)
      {  return inherited::addElement(Cast::castTo(element)); }
   void removeElement(TypeElement& element)
      {  inherited::removeElement(Cast::castTo(element)); }
   void freeElement(TypeElement* element)
      {  inherited::freeElement(Cast::castTo(element)); }
   void freeAllExcept(TypeElement* element)
      {  inherited::freeAllExcept(Cast::castTo(element)); }
   void removeAllExcept(TypeElement* element)
      {  inherited::removeAllExcept(Cast::castTo(element)); }
   void swap(thisType& source) { inherited::swap(source); }
   TypeElement& getFirst() const { return (TypeElement&) Cast::castFrom(inherited::getFirst()); }
   TypeElement& getLast() const { return (TypeElement&) Cast::castFrom(inherited::getLast()); }
   TypeElement* getPFirst() const { return (TypeElement*) Cast::castFrom(inherited::getPFirst()); }
   TypeElement* getPLast() const { return (TypeElement*) Cast::castFrom(inherited::getPLast()); }

   class Cursor : public InheritedCursor {
     private:
      typedef InheritedCursor inherited;

     public:
      Cursor(const TSharedCollection<TypeElement, Cast>& collection) : inherited(collection) {}
      Cursor(const Cursor& source) : inherited(source) {}

      TypeElement& elementSAt() const
         {  return (TypeElement&) Cast::castFrom(inherited::elementSAt()); }
      const TypeElement& elementAt() const
         {  return (const TypeElement&) Cast::castFrom(inherited::elementAt()); }
   };

   class ForeachElementCast {
     public:
      typedef GlobalElement Base;
      static TypeElement& castFrom(GlobalElement& source)
         {  return (TypeElement&) Cast::castFrom((SharedCollection::Element&) source.getElement()); }
      static TypeElement* castFrom(GlobalElement* source)
         {  return &(TypeElement&) Cast::castFrom((const SharedCollection::Element&) source->getElement()); }
      static const Element& castFrom(const GlobalElement& source)
         {  return (const TypeElement&) Cast::castFrom((SharedCollection::Element&) source.getElement()); }
      static const Element* castFrom(const GlobalElement* source)
         {  return &(const TypeElement&) Cast::castFrom((const SharedCollection::Element&) source->getElement()); }
   };
   typedef HandlerLinkedCast<TypeElement, ForeachElementCast, DSharedCollection::GlobalElement::HandlerRegisterCast>
      ForeachListElementCast;
   bool foreachDo(std::function<bool (TypeElement&)> function) const
      {  return COL::ImplList::foreachDo(TemplateElementCastParameters<TypeElement, ForeachListElementCast>(), function); }
   bool foreachCDo(std::function<bool (const TypeElement&)> function) const
      {  return COL::ImplList::foreachDo(TemplateElementCastParameters<TypeElement, ForeachListElementCast>(), function); }
   bool foreachReverseDo(std::function<bool (TypeElement&)> function) const
      {  return COL::ImplList::foreachReverseDo(TemplateElementCastParameters<TypeElement, ForeachListElementCast>(), function); }
   bool foreachCReverseDo(std::function<bool (const TypeElement&)> function) const
      {  return COL::ImplList::foreachReverseDo(TemplateElementCastParameters<TypeElement, ForeachListElementCast>(), function); }
};

} // end of namespace PNT

#endif // PNT_SharedCollectionH
