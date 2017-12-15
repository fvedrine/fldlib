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
// File      : Special.h
// Description :
//   Definition of collections with a finite number of elements.
//

#ifndef COL_SpecialH
#define COL_SpecialH

#include "Collection/VirtualCollection/VirtualCollection.h"

namespace COL {

/*******************************************/
/* Definition of the class EmptyCollection */
/*******************************************/

#define DefTypeCollection EmptyCollection
#define DefTypeCursor EmptyCollectionCursor

class SpecialCollection : public VirtualCollection {
  public:
   SpecialCollection() {}
   SpecialCollection(const SpecialCollection& source, AddMode mode=AMDuplicate)
      :  VirtualCollection(source, mode) {}

   virtual bool isSpecial() const { return true; }
   virtual bool isEmptyType() const { return false; }
   virtual bool isUniqueType() const { return false; }
   virtual bool isDoubleType() const { return false; }
};

class EmptyCollectionCursor;
class EmptyCollection : public VirtualCollection {
  private:
   typedef VirtualCollection inherited;
   typedef EmptyCollection thisType;

  protected:
   virtual void _fullAssign(const VirtualCollection& source, const ExtendedReplaceParameters& parameters) override
      {  inherited::_fullAssign(source, parameters);
         AssumeCondition(source.isEmpty())
      }
   virtual void _add(EnhancedObject* anObject, const ExtendedInsertionParameters& parameters,
         VirtualCollectionCursor* cursor=nullptr) override
      {  inherited::_add(anObject, parameters, cursor);
         AddGuardian<EnhancedObject> guard(*this, anObject, parameters);
         AssumeUncalled
      }
   virtual void _addAll(const VirtualCollection& source, const ExtendedInsertionParameters& parameters,
         VirtualCollectionCursor* cursor=nullptr, const VirtualCollectionCursor* startSource=nullptr,
         const VirtualCollectionCursor* endSource=nullptr) override
      {  inherited::_addAll(source, parameters, cursor, startSource, endSource);
         AssumeCondition(source.queryCount(ExtendedLocateParameters(), startSource, endSource) == 0)
      }
   virtual void _moveTo(VirtualCollection& destination, const ExtendedReplaceParameters& parameters,
         VirtualCollectionCursor* cursor=nullptr, VirtualCollectionCursor* destinationCursor=nullptr) override
      {  inherited::_moveTo(destination, parameters, cursor, destinationCursor);
         AssumeUncalled
      }
   virtual void _moveAllTo(VirtualCollection& destination, const ExtendedReplaceParameters& parameters,
         const VirtualCollectionCursor* startCursor=nullptr, const VirtualCollectionCursor* endCursor=nullptr,
         VirtualCollectionCursor* destinationCursor=nullptr) override
      {  inherited::_moveAllTo(destination, parameters, startCursor, endCursor, destinationCursor); }
   virtual void _swap(VirtualCollection& source) override
      {  AssumeCondition(dynamic_cast<EmptyCollection*>(&source)) }
   virtual void _remove(const ExtendedSuppressParameters& parameters, VirtualCollectionCursor* cursor=nullptr) override
      {  inherited::_remove(parameters, cursor);
         AssumeUncalled
      }
   virtual void _removeAll(const ExtendedSuppressParameters& parameters,
         const VirtualCollectionCursor* start=nullptr, const VirtualCollectionCursor* end=nullptr) override
      {  inherited::_removeAll(parameters, start, end); }
   virtual void _replace(EnhancedObject* anObject,
         const ExtendedReplaceParameters& parameters, VirtualCollectionCursor* cursor=nullptr) override
      {  inherited::_replace(anObject, parameters, cursor);
         AddGuardian<EnhancedObject> guard(*this, anObject, parameters);
         AssumeUncalled
      }
   virtual int _queryCount(const ExtendedLocateParameters& parameters,
         const VirtualCollectionCursor* startCursor=nullptr, const VirtualCollectionCursor* endCursor=nullptr) const override
      {  inherited::_queryCount(parameters, startCursor, endCursor);
         return 0;
      }
   virtual EnhancedObject* _getElement(const ExtendedLocateParameters& parameters,
         const VirtualCollectionCursor* cursor=nullptr) const override
      {  inherited::_getElement(parameters, cursor);
         AssumeUncalled
         return nullptr;
      }

  public:
   EmptyCollection() {}
   EmptyCollection(const EmptyCollection& source, AddMode mode=AMDuplicate,
         const VirtualCast* retrieveRegistrationFromCopy=nullptr)
      :  inherited(source, mode) {}
   typedef EnhancedObject Node;
   DefineCopy(EmptyCollection)
   DefineCollectionForAbstractCollect(EmptyCollection, EmptyCollectionCursor)

   virtual bool isEmptyType() const { return true; }

   template <class TemplateParameters>
   TVirtualCollection<typename TemplateParameters::Element, typename TemplateParameters::Cast>&
      getInterface(TemplateParameters)
      {  return (TVirtualCollection<typename TemplateParameters::Element, typename TemplateParameters::Cast>&)
            (VirtualCollection&) *this;
      }
   template <class TemplateParameters>
   const TVirtualCollection<typename TemplateParameters::Element, typename TemplateParameters::Cast>&
      getInterface(TemplateParameters) const
      {  return (const TVirtualCollection<typename TemplateParameters::Element, typename TemplateParameters::Cast>&)
            (const VirtualCollection&) *this;
      }

#define DefJustDeclare
   #include "Collection/ELMCollection.inch"
#undef DefJustDeclare

   template <class Execute, class TemplateParameters> bool
      foreachDo(TemplateParameters traits, Execute& fun, const ExtendedLocateParameters& parameters,
         const VirtualCollectionCursor* start=nullptr, const VirtualCollectionCursor* end=nullptr) const
      {  return true; }
   template <class Execute, class TemplateParameters> bool
      foreachSDo(TemplateParameters traits, Execute& fun, const ExtendedLocateParameters& parameters,
         const typename TemplateParameters::TypeCursor* start=nullptr,
         const typename TemplateParameters::TypeCursor* end=nullptr) const
      {  return true; }
   template <class Execute, class TemplateParameters> bool
      foreachDo(TemplateParameters traits, Execute& fun) const
      { return true; }
   template <class Execute, class TemplateParameters> bool
      foreachReverseDo(TemplateParameters traits, Execute& fun, const ExtendedLocateParameters& parameters,
         const VirtualCollectionCursor* start=nullptr, const VirtualCollectionCursor* end=nullptr) const
      {  return true; }
   template <class Execute, class TemplateParameters> bool
      foreachSReverseDo(TemplateParameters traits, Execute& fun, const ExtendedLocateParameters& parameters,
         const typename TemplateParameters::TypeCursor* start=nullptr,
         const typename TemplateParameters::TypeCursor* end=nullptr) const
      {  return true; }
   template <class Execute, class TemplateParameters> bool
      foreachReverseDo(TemplateParameters traits, Execute& fun) const
      {  return true; }
};

class EmptyCollectionCursor : public VirtualCollectionCursor {
  private:
   typedef VirtualCollectionCursor inherited;
   typedef EmptyCollectionCursor thisType;

  protected:
   virtual bool _isPositionned(const ExtendedLocateParameters& pos,
         const VirtualCollectionCursor* cursor=nullptr) const override { return false; }
   virtual bool _position(const Position& pos) override { return false; }
   virtual EnhancedObject* _getSElement() const override {  AssumeUncalled return nullptr; }
   virtual bool _isEqual(const AbstractCursor& cursor) const override { return true; }
   virtual void _gotoReference(const EnhancedObject& element) override {  AssumeUncalled; }

  public:
   EmptyCollectionCursor(const EmptyCollection& support) : inherited(support) {}
   EmptyCollectionCursor(const thisType& source) : inherited(source) {}
   DefineCopy(EmptyCollectionCursor)
   DefineCursorForAbstractCollect(EmptyCollection, EmptyCollectionCursor)
   void swap(thisType& source) {}
   virtual bool isValid() const override { return false; }

#define DefTypeFinal EmptyCollectionCursor::
#define DefCursor
   #include "Collection/ELMCollection.inch"
#undef DefCursor
#undef DefTypeFinal
};

InlineCollectionForAbstractCollect(EmptyCollection, EmptyCollectionCursor)

#define DefTypeFinal EmptyCollection::
#define DefJustInline
   #include "Collection/ELMCollection.inch"
#undef DefJustInline
#undef DefTypeFinal

#undef DefTypeCollection
#undef DefTypeCursor

/********************************************/
/* Definition of the class UniqueCollection */
/********************************************/

#define DefTypeCollection UniqueCollection
#define DefTypeCursor UniqueCollectionCursor

class DoubleCollection;
class DoubleCollectionCursor;
class UniqueCollectionCursor;

class UniqueCollection : public VirtualCollection {
  public:
   DefineCollectionForAbstractCollect(UniqueCollection, UniqueCollectionCursor)

  private:
   typedef VirtualCollection inherited;
   typedef UniqueCollection thisType;
   EnhancedObject* peoElement;

   int queryInternCount(const ExtendedLocateParameters& parameters,
      const Cursor* startCursor, const Cursor* endCursor) const;

  protected:
   virtual void _fullAssign(const VirtualCollection& source, const ExtendedReplaceParameters& parameters) override;
   void _fullAssign(const UniqueCollection& source, const ExtendedReplaceParameters& parameters);
   virtual void _add(EnhancedObject* anObject, const ExtendedInsertionParameters& parameters,
         VirtualCollectionCursor* cursor=nullptr) override;
   virtual void _addAll(const VirtualCollection& source, const ExtendedInsertionParameters& parameters,
         VirtualCollectionCursor* cursor, const VirtualCollectionCursor* startSource,
         const VirtualCollectionCursor* endSource) override;
   void _addAll(const UniqueCollection& source, const ExtendedInsertionParameters& parameters,
         Cursor* cursor=nullptr, const Cursor* startSource=nullptr, const Cursor* endSource=nullptr);
   void _addAll(const DoubleCollection& source, const ExtendedInsertionParameters& parameters,
         Cursor* cursor, const DoubleCollectionCursor* startSource, const DoubleCollectionCursor* endSource);
   virtual void _moveTo(VirtualCollection& destination, const ExtendedReplaceParameters& parameters,
         VirtualCollectionCursor* cursor, VirtualCollectionCursor* destinationCursor) override;
   void _moveTo(UniqueCollection& destination, const ExtendedReplaceParameters& parameters,
         Cursor* cursor=nullptr, Cursor* destinationCursor=nullptr);
   void _moveTo(DoubleCollection& destination, const ExtendedReplaceParameters& parameters,
         Cursor* cursor, DoubleCollectionCursor* destinationCursor);
   virtual void _moveAllTo(VirtualCollection& destination, const ExtendedReplaceParameters& parameters,
         const VirtualCollectionCursor* startCursor, const VirtualCollectionCursor* endCursor,
         VirtualCollectionCursor* destinationCursor) override;
   void _moveAllTo(UniqueCollection& destination, const ExtendedReplaceParameters& parameters,
         const Cursor* startCursor=nullptr, const Cursor* endCursor=nullptr, Cursor* destinationCursor=nullptr);
   void _moveAllTo(DoubleCollection& destination, const ExtendedReplaceParameters& parameters,
         const Cursor* startCursor, const Cursor* endCursor, DoubleCollectionCursor* destinationCursor);
   virtual void _swap(VirtualCollection& source) override;
   virtual void _remove(const ExtendedSuppressParameters& parameters, VirtualCollectionCursor* cursor=nullptr) override;
   virtual void _removeAll(const ExtendedSuppressParameters& parameters,
         const VirtualCollectionCursor* start=nullptr, const VirtualCollectionCursor* end=nullptr) override;
   virtual void _replace(EnhancedObject* anObject, const ExtendedReplaceParameters& parameters,
         VirtualCollectionCursor* cursor=nullptr) override;
   virtual int _queryCount(const ExtendedLocateParameters& parameters,
         const VirtualCollectionCursor* startCursor=nullptr, const VirtualCollectionCursor* endCursor=nullptr) const override;
   virtual EnhancedObject* _getElement(const ExtendedLocateParameters& parameters,
         const VirtualCollectionCursor* cursor=nullptr) const override;

   bool hasElement() const { return peoElement != nullptr; }
   EnhancedObject& element() const { AssumeCondition(peoElement) return *peoElement; }
   void setElement(EnhancedObject& element)
      {  AssumeCondition(!peoElement) peoElement = &element; }
   friend class UniqueCollectionCursor;
   friend class DoubleCollection;

  public:
   UniqueCollection() : peoElement(nullptr) {}
   UniqueCollection(const InitialValues& initialValues)
      {  TInsertInitialValue<UniqueCollection, EnhancedObject, SimpleCast> insertInitialValue(*this);
         initialValues.foreachDo(insertInitialValue);
      }
   UniqueCollection(const InitialNewValues& initialValues)
      {  TInsertInitialNewValue<UniqueCollection, EnhancedObject, SimpleCast> insertInitialValue(*this);
         initialValues.foreachDo(insertInitialValue);
      }
   UniqueCollection(const UniqueCollection& source, AddMode mode=AMDuplicate,
         const VirtualCast* retrieveRegistrationFromCopy=nullptr)
      :  inherited(source), peoElement((source.peoElement && (mode==AMDuplicate))
            ? (retrieveRegistrationFromCopy
               ? ((const VirtualCastWithElement*) retrieveRegistrationFromCopy)
                  ->castFrom(source.peoElement->createCopy(), source.peoElement)
               : source.peoElement->createCopy()) : source.peoElement) {}
   typedef EnhancedObject Node;
   DefineCopy(UniqueCollection)
   DDefineAssign(UniqueCollection)

   virtual bool isUniqueType() const { return true; }

#define DefJustDeclare
   #include "Collection/ELMCollection.inch"
#undef DefJustDeclare

   template <class Execute, class TemplateParameters> bool
      foreachDo(TemplateParameters traits, Execute& fun, const ExtendedLocateParameters& parameters,
         const VirtualCollectionCursor* start=nullptr, const VirtualCollectionCursor* end=nullptr) const
      {  return (peoElement == nullptr) ? true :
            (((start && start->isValid())
               || (end && end->isValid())) && parameters.isNear() ? true
            : fun((typename TemplateParameters::Element&) TemplateParameters::Cast::castFrom(*peoElement)));
      }
   template <class Execute, class TemplateParameters> bool
      foreachSDo(TemplateParameters traits, Execute& fun, const ExtendedLocateParameters& parameters,
         const typename TemplateParameters::TypeCursor* start=nullptr,
         const typename TemplateParameters::TypeCursor* end=nullptr) const
      {  return (peoElement == nullptr) ? true :
            (((start && start->isValid())
               || (end && end->isValid())) && parameters.isNear() ? true
            : fun((typename TemplateParameters::Element&) TemplateParameters::Cast::castFrom(*peoElement)));
      }
   template <class Execute, class TemplateParameters> bool
      foreachDo(TemplateParameters traits, Execute& fun) const
      { return (peoElement == nullptr) ? true
            : fun((typename TemplateParameters::Element&) TemplateParameters::Cast::castFrom(*peoElement));
      }
   template <class Execute, class TemplateParameters> bool
      foreachReverseDo(TemplateParameters traits, Execute& fun, const ExtendedLocateParameters& parameters,
         const VirtualCollectionCursor* start=nullptr, const VirtualCollectionCursor* end=nullptr) const
      {  return (peoElement == nullptr) ? true :
            (((start && start->isValid())
               || (end && end->isValid())) && parameters.isNear() ? true
            : fun((typename TemplateParameters::Element&) TemplateParameters::Cast::castFrom(*peoElement)));
      }
   template <class Execute, class TemplateParameters> bool
      foreachSReverseDo(TemplateParameters traits, Execute& fun, const ExtendedLocateParameters& parameters,
         const typename TemplateParameters::TypeCursor* start=nullptr,
         const typename TemplateParameters::TypeCursor* end=nullptr) const
      {  return (peoElement == nullptr) ? true :
            (((start && start->isValid())
               || (end && end->isValid())) && parameters.isNear() ? true
            : fun((typename TemplateParameters::Element&) TemplateParameters::Cast::castFrom(*peoElement)));
      }
   template <class Execute, class TemplateParameters> bool
      foreachReverseDo(TemplateParameters traits, Execute& fun) const
      {  return (peoElement == nullptr) ? true
            : fun((typename TemplateParameters::Element&) TemplateParameters::Cast::castFrom(*peoElement));
      }
};

class UniqueCollectionCursor : public VirtualCollectionCursor {
  private:
   typedef VirtualCollectionCursor inherited;
   typedef UniqueCollectionCursor thisType;
   bool fValid;

  protected:
   virtual ComparisonResult _compare(const EnhancedObject& asource) const override
      {  ComparisonResult result = inherited::_compare(asource);
         const UniqueCollectionCursor& source = (const UniqueCollectionCursor&) castFromCopyHandler(asource);
         return (result != CREqual) ? result : ((source.fValid == fValid) ? CREqual : CRNonComparable);
      }
   virtual bool _isPositionned(const ExtendedLocateParameters& pos,
         const VirtualCollectionCursor* cursor=nullptr) const override
      {  return fValid && ((cursor && cursor->isValid()) ? !pos.isNear() : !pos.isExact()); }
   virtual bool _position(const Position& pos) override
      {  return (fValid = pos.isInvalid() ? false
            : ((getSupport().hasElement()) ? (pos.isNear() ? !fValid : true) : false));
      }
   virtual EnhancedObject* _getSElement() const override
      {  AssumeCondition(fValid)
         return &getSupport().element();
      }
   virtual bool _isEqual(const AbstractCursor& cursor) const override
      {  return inherited::_isEqual(cursor)
            && (fValid == ((const UniqueCollectionCursor&) cursor).fValid);
      }
   virtual void _gotoReference(const EnhancedObject& element) override
      {  AssumeCondition(&getSupport().element() == &element)
         fValid = true;
      }

   void setValid() { fValid = true; }
   void setInvalid() { fValid = false; }
   friend class DoubleCollection;

  public:
   UniqueCollectionCursor(const UniqueCollection& support) : inherited(support), fValid(false) {}
   UniqueCollectionCursor(const thisType& source) = default;
   DefineCopy(UniqueCollectionCursor)
   DDefineAssign(UniqueCollectionCursor)

   DefineCursorForAbstractCollect(UniqueCollection, UniqueCollectionCursor)
   virtual bool isValid() const override { return fValid; }
   void swap(thisType& source)
      {  bool temp = fValid;
         fValid = source.fValid;
         source.fValid = temp;
      }

#define DefTypeFinal UniqueCollectionCursor::
#define DefCursor
   #include "Collection/ELMCollection.inch"
#undef DefCursor
#undef DefTypeFinal
};

InlineCollectionForAbstractCollect(UniqueCollection, UniqueCollectionCursor)

inline void
UniqueCollection::_add(EnhancedObject* anObject, const ExtendedInsertionParameters& parameters,
      VirtualCollectionCursor* cursor) {
   AddGuardian<EnhancedObject> guard(*this, anObject, parameters);
   inherited::_add(anObject, parameters, cursor);
   AssumeCondition(peoElement == nullptr);
   peoElement = guard.handleDuplication();
   guard.release();
   if (cursor && !parameters.isRemote())
      ((Cursor*) cursor)->setValid();
}

inline void
UniqueCollection::_swap(VirtualCollection& vcSource) {
   inherited::_swap(vcSource);
   AssumeCondition(dynamic_cast<UniqueCollection*>(&vcSource))
   UniqueCollection& source = (UniqueCollection&) vcSource;
   InvalidateNotification invalidation;
   notifyCursorUpdate(invalidation);
   source.notifyCursorUpdate(invalidation);

   EnhancedObject* temp = peoElement;
   peoElement = source.peoElement;
   source.peoElement = temp;
}

inline void
UniqueCollection::_remove(const ExtendedSuppressParameters& parameters, VirtualCollectionCursor* cursor) {
   inherited::_remove(parameters, cursor);
   AssumeCondition(peoElement != nullptr)
   if (parameters.isFree())
      delete peoElement;
   peoElement = nullptr;
   InvalidateNotification invalidation;
   if (cursor) {
      invalidation.setOrigin(*cursor);
      ((Cursor*) cursor)->setInvalid();
   };
   notifyCursorUpdate(invalidation);
}

inline void
UniqueCollection::_removeAll(const ExtendedSuppressParameters& parameters,
      const VirtualCollectionCursor* start, const VirtualCollectionCursor* end) {
   inherited::_removeAll(parameters, start, end);
   if (peoElement != nullptr) {
      if (!(start && start->isValid() && parameters.isFirstExcluded())
            && !(end && end->isValid() && parameters.isLastExcluded())) {
         if (parameters.isFree())
            delete peoElement;
         peoElement = nullptr;
         InvalidateNotification invalidation;
         notifyCursorUpdate(invalidation);
      };
   };
}

inline void
UniqueCollection::_replace(EnhancedObject* anObject,
      const ExtendedReplaceParameters& parameters, VirtualCollectionCursor* cursor) {
   inherited::_replace(anObject, parameters, cursor);
   AddGuardian<EnhancedObject> guard(*this, anObject, parameters);
   if (parameters.isFree())
      delete peoElement;
   peoElement = nullptr;
   peoElement = guard.handleDuplication();
   guard.release();
}

inline int
UniqueCollection::_queryCount(const ExtendedLocateParameters& parameters,
      const VirtualCollectionCursor* startCursor, const VirtualCollectionCursor* endCursor) const {
   inherited::_queryCount(parameters, startCursor, endCursor);
   return (!startCursor && !endCursor) ? ((peoElement != nullptr) ? 1 : 0)
      :  queryInternCount(parameters, (Cursor*) startCursor, (Cursor*) endCursor);
}

inline EnhancedObject*
UniqueCollection::_getElement(const ExtendedLocateParameters& parameters,
      const VirtualCollectionCursor* cursor) const {
   inherited::_getElement(parameters, cursor);
   AssumeCondition((peoElement != nullptr) && ((cursor && cursor->isValid())
      ? !parameters.isNear() : !parameters.isExact()))
   return peoElement;
}

#define DefTypeFinal UniqueCollection::
#define DefJustInline
   #include "Collection/ELMCollection.inch"
#undef DefJustInline
#undef DefTypeFinal

#undef DefTypeCollection
#undef DefTypeCursor

/********************************************/
/* Definition of the class DoubleCollection */
/********************************************/

#define DefTypeCollection DoubleCollection
#define DefTypeCursor DoubleCollectionCursor

class DoubleCollectionCursor;
class DoubleCollection : public VirtualCollection {
  public:
   DefineCollectionForAbstractCollect(DoubleCollection, DoubleCollectionCursor)

  private:
   typedef VirtualCollection inherited;
   typedef DoubleCollection thisType;
   EnhancedObject *peoFirst, *peoSecond;

   int queryInternCount(const ExtendedLocateParameters& parameters,
         const Cursor* startCursor, const Cursor* endCursor) const;

   enum Location { LUndefined, LFirst, LSecond };
   Location getLocation(RelativePosition pos, const Cursor* cursor) const;

   class InternMoveNotification;
   class SwapNotification;
   class SuppressionNotification;
   class RemoveNotification;
   friend class RemoveNotification;
   friend class SuppressionNotification;

  protected:
   virtual void _fullAssign(const VirtualCollection& source, const ExtendedReplaceParameters& parameters) override;
   void _fullAssign(const DoubleCollection& source, const ExtendedReplaceParameters& parameters);
   virtual void _add(EnhancedObject* anObject, const ExtendedInsertionParameters& parameters,
         VirtualCollectionCursor* cursor=nullptr) override;
   virtual void _addAll(const VirtualCollection& source, const ExtendedInsertionParameters& parameters,
         VirtualCollectionCursor* cursor, const VirtualCollectionCursor* startSource,
         const VirtualCollectionCursor* endSource) override;
   void _addAll(const UniqueCollection& source, const ExtendedInsertionParameters& parameters,
         Cursor* cursor, const UniqueCollectionCursor* startSource, const UniqueCollectionCursor* endSource);
   void _addAll(const DoubleCollection& source, const ExtendedInsertionParameters& parameters,
         Cursor* cursor=nullptr, const Cursor* startSource=nullptr, const Cursor* endSource=nullptr);
   virtual void _moveTo(VirtualCollection& destination, const ExtendedReplaceParameters& parameters,
         VirtualCollectionCursor* cursor, VirtualCollectionCursor* destinationCursor) override;
   void _moveTo(UniqueCollection& destination, const ExtendedReplaceParameters& parameters,
         Cursor* cursor, UniqueCollectionCursor* destinationCursor);
   void _moveTo(DoubleCollection& destination, const ExtendedReplaceParameters& parameters,
         Cursor* cursor=nullptr, Cursor* destinationCursor=nullptr);
   virtual void _moveAllTo(VirtualCollection& destination, const ExtendedReplaceParameters& parameters,
         const VirtualCollectionCursor* startCursor, const VirtualCollectionCursor* endCursor,
         VirtualCollectionCursor* destinationCursor) override;
   void _moveAllTo(UniqueCollection& destination, const ExtendedReplaceParameters& parameters,
         const Cursor* startCursor, const Cursor* endCursor, UniqueCollectionCursor* destinationCursor);
   void _moveAllTo(DoubleCollection& destination, const ExtendedReplaceParameters& parameters,
         const Cursor* startCursor=nullptr, const Cursor* endCursor=nullptr, Cursor* destinationCursor=nullptr);
   virtual void _swap(VirtualCollection& source) override;
   virtual void _remove(const ExtendedSuppressParameters& parameters, VirtualCollectionCursor* cursor=nullptr) override;
   virtual void _removeAll(const ExtendedSuppressParameters& parameters,
         const VirtualCollectionCursor* start=nullptr, const VirtualCollectionCursor* end=nullptr) override;
   virtual void _replace(EnhancedObject* anObject,
         const ExtendedReplaceParameters& parameters, VirtualCollectionCursor* cursor=nullptr) override;
   virtual int _queryCount(const ExtendedLocateParameters& parameters,
         const VirtualCollectionCursor* startCursor=nullptr, const VirtualCollectionCursor* endCursor=nullptr) const override;
   virtual EnhancedObject* _getElement(const ExtendedLocateParameters& parameters,
         const VirtualCollectionCursor* cursor=nullptr) const override;

   friend class DoubleCollectionCursor;
   friend class UniqueCollection;
   bool hasFirst() const { return peoFirst != nullptr; }
   EnhancedObject& first() const { AssumeCondition(peoFirst) return *peoFirst; }
   bool hasSecond() const { return peoSecond != nullptr; }
   EnhancedObject& second() const { AssumeCondition(peoSecond) return *peoSecond; }
   void setFirst(EnhancedObject& newElement)
      {  AssumeCondition(!peoFirst) peoFirst = &newElement; }
   void setSecond(EnhancedObject& newElement)
      {  AssumeCondition(!peoSecond) peoSecond = &newElement; }
   bool hasElement(int position) const
      {  AssumeCondition(position == 1 || position == 2)
         return (position == 1) ? (peoFirst != nullptr) : (peoSecond != nullptr);
      }
   EnhancedObject& element(int position) const
      {  AssumeCondition(((position == 1) && (peoFirst != nullptr))
            || ((position == 2) && (peoSecond != nullptr)))
         return (position == 1) ? *peoFirst : *peoSecond;
      }

  public:
   DoubleCollection() : peoFirst(nullptr), peoSecond(nullptr) {}
   DoubleCollection(const InitialValues& initialValues)
      {  TInsertInitialValue<DoubleCollection, EnhancedObject, SimpleCast> insertInitialValue(*this);
         initialValues.foreachDo(insertInitialValue);
      }
   DoubleCollection(const InitialNewValues& initialValues)
      {  TInsertInitialNewValue<DoubleCollection, EnhancedObject, SimpleCast> insertInitialValue(*this);
         initialValues.foreachDo(insertInitialValue);
      }
   DoubleCollection(const DoubleCollection& source, AddMode mode=AMDuplicate,
         const VirtualCast* retrieveRegistrationFromCopy=nullptr)
      :  inherited(source),
         peoFirst((source.peoFirst && (mode==AMDuplicate)) ? (retrieveRegistrationFromCopy
            ? ((const VirtualCastWithElement*) retrieveRegistrationFromCopy)
                  ->castFrom(source.peoFirst->createCopy(), source.peoFirst)
            : source.peoFirst->createCopy())
            : source.peoFirst),
         peoSecond((source.peoSecond && (mode==AMDuplicate)) ? (retrieveRegistrationFromCopy
            ? ((const VirtualCastWithElement*) retrieveRegistrationFromCopy)
                  ->castFrom(source.peoSecond->createCopy(), source.peoSecond)
            : source.peoSecond->createCopy())
            : source.peoSecond) {}
   typedef EnhancedObject Node;
   DefineCopy(DoubleCollection)
   DDefineAssign(DoubleCollection)

   virtual bool isDoubleType() const { return true; }

#define DefJustDeclare
   #include "Collection/ELMCollection.inch"
#undef DefJustDeclare

   template <class Execute, class TemplateParameters> bool
      foreachDo(TemplateParameters traits, Execute& fun, const ExtendedLocateParameters& parameters,
         const VirtualCollectionCursor* start=nullptr, const VirtualCollectionCursor* end=nullptr) const;
   template <class Execute, class TemplateParameters> bool
      foreachSDo(TemplateParameters traits, Execute& fun, const ExtendedLocateParameters& parameters,
         const typename TemplateParameters::TypeCursor* start=nullptr,
         const typename TemplateParameters::TypeCursor* end=nullptr) const;
   template <class Execute, class TemplateParameters> bool
      foreachDo(TemplateParameters traits, Execute& fun) const
      {  bool result = true;
         if (peoFirst)
            result = fun((typename TemplateParameters::Element&) TemplateParameters::Cast::castFrom(*peoFirst));
         if (peoSecond && result)
            result = fun((typename TemplateParameters::Element&) TemplateParameters::Cast::castFrom(*peoSecond));
         return result;
      }
   template <class Execute, class TemplateParameters> bool
      foreachReverseDo(TemplateParameters traits, Execute& fun, const ExtendedLocateParameters& parameters,
         const VirtualCollectionCursor* start=nullptr, const VirtualCollectionCursor* end=nullptr) const;
   template <class Execute, class TemplateParameters> bool
      foreachSReverseDo(TemplateParameters traits, Execute& fun, const ExtendedLocateParameters& parameters,
         const typename TemplateParameters::TypeCursor* start=nullptr,
         const typename TemplateParameters::TypeCursor* end=nullptr) const;
   template <class Execute, class TemplateParameters> bool
      foreachReverseDo(TemplateParameters traits, Execute& fun) const
      {  bool result = true;
         if (peoSecond)
            result = fun((typename TemplateParameters::Element&) TemplateParameters::Cast::castFrom(*peoSecond));
         if (peoFirst && result)
            result = fun((typename TemplateParameters::Element&) TemplateParameters::Cast::castFrom(*peoFirst));
         return result;
      }
};

class DoubleCollectionCursor : public VirtualCollectionCursor {
  private:
   typedef VirtualCollectionCursor inherited;
   typedef DoubleCollectionCursor thisType;
   enum Mode { MInvalid=0, MFirst=1, MSecond=2 };
   Mode mMode;

   bool incMode()
      {  mMode = (Mode) (mMode+1);
         bool result = (mMode < 3);
         if (!result)
            mMode = MInvalid;
         return result;
      }
   bool decMode()
      {  if (mMode == MInvalid)
            mMode = (Mode) 3;
         mMode = (Mode) (mMode-1);
         return mMode > MInvalid;
      }

  protected:
   virtual ComparisonResult _compare(const EnhancedObject& asource) const override
      {  ComparisonResult result = inherited::_compare(asource);
         const DoubleCollectionCursor& source = (const DoubleCollectionCursor&) castFromCopyHandler(asource);
         return (result != CREqual)
            ? result : ((source.mMode == mMode)
            ? CREqual : ((mMode == MInvalid || source.mMode == MInvalid)
            ? CRNonComparable
            : fcompare(mMode, source.mMode)));
      }
   virtual bool _isPositionned(const ExtendedLocateParameters& pos,
         const VirtualCollectionCursor* cursor=nullptr) const override;
   virtual bool _position(const Position& pos) override;
   virtual EnhancedObject* _getSElement() const override
      {  AssumeCondition((mMode != MInvalid))
         return &getSupport().element(mMode);
      }
   virtual bool _isEqual(const AbstractCursor& cursor) const override
      {  return inherited::_isEqual(cursor)
            && (mMode == ((const DoubleCollectionCursor&) cursor).mMode);
      }
   virtual void _gotoReference(const EnhancedObject& element) override
      {  const DoubleCollection& support = getSupport();
         mMode = (support.hasFirst() && (&support.first() == &element))
            ? MFirst : ((support.hasSecond() && (&support.second() == &element))
            ? MSecond : MInvalid);
         AssumeCondition(mMode != MInvalid)
      }

   void setSecondMode() { mMode = MSecond; }
   void setFirstMode() { mMode = MFirst; }
   void setInvalid() { mMode = MInvalid; }
   bool isFirstMode() const { return mMode == MFirst; }
   bool isSecondMode() const { return mMode == MSecond; }
   friend class UniqueCollection;
   friend class DoubleCollection::InternMoveNotification;
   friend class DoubleCollection::SuppressionNotification;
   friend class DoubleCollection::RemoveNotification;
   friend class DoubleCollection::SwapNotification;

  public:
   DoubleCollectionCursor(const DoubleCollection& support) : inherited(support), mMode(MInvalid) {}
   DoubleCollectionCursor(const thisType& source) : inherited(source), mMode(source.mMode) {}
   DefineCopy(DoubleCollectionCursor)
   DDefineAssign(DoubleCollectionCursor)
   DefineCursorForAbstractCollect(DoubleCollection, DoubleCollectionCursor)
   void swap(thisType& source)
      {  Mode temp = mMode;
         mMode = source.mMode;
         source.mMode = temp;
      }
   virtual bool isValid() const override { return (mMode != MInvalid); }

#define DefTypeFinal DoubleCollectionCursor::
#define DefCursor
   #include "Collection/ELMCollection.inch"
#undef DefCursor
#undef DefTypeFinal

   bool invariant() const
      {  return (mMode == MFirst)
            ? getSupport().hasFirst() : ((mMode == MSecond)
            ? getSupport().hasSecond() : true);
      }
};

inline DoubleCollection::Location
DoubleCollection::getLocation(RelativePosition pos, const Cursor* cursor) const {
   Location result = LUndefined;
   if (pos == RPBefore) {
      if (cursor && cursor->isValid()) {
         AssumeCondition(!cursor->isFirstMode())
         result = LFirst;
      }
      else
         result = (peoSecond != nullptr) ? LSecond : LFirst;
   }
   else if (pos == RPAfter) {
      if (cursor && cursor->isValid()) {
         AssumeCondition(!cursor->isSecondMode())
         result = LSecond;
      }
      else
         result = (peoFirst == nullptr) ? LSecond : LFirst;
   }
   else if (pos == RPExact) {
      AssumeCondition(cursor && cursor->isValid())
      result = cursor->isSecondMode() ? LSecond : LFirst;
   }
   else // pos == RPUndefined
      result = (peoSecond != nullptr) ? LSecond : LFirst;
   return result;
}

InlineCollectionForAbstractCollect(DoubleCollection, DoubleCollectionCursor)

inline void
DoubleCollection::_addAll(const VirtualCollection& vcSource, const ExtendedInsertionParameters& parameters,
      VirtualCollectionCursor* cursor, const VirtualCollectionCursor* startSource,
      const VirtualCollectionCursor* endSource) {
   if (vcSource.isSpecial()) {
      const auto& source = (const SpecialCollection&) vcSource;
      if (source.isUniqueType())
         _addAll((const UniqueCollection&) source, parameters, (Cursor*) cursor,
               (UniqueCollectionCursor*) startSource, (UniqueCollectionCursor*) endSource);
      else if (source.isDoubleType())
         _addAll((const DoubleCollection&) source, parameters, (Cursor*) cursor,
               (Cursor*) startSource, (Cursor*) endSource);
      else {
         AssumeCondition(source.isEmptyType())
      }
   }
   else
      inherited::paddAll(vcSource, parameters, cursor, startSource, endSource);
}

inline void
DoubleCollection::_moveTo(VirtualCollection& vcDestination, const ExtendedReplaceParameters& parameters,
      VirtualCollectionCursor* cursor, VirtualCollectionCursor* destinationCursor) {
   if (vcDestination.isSpecial()) {
      const auto& destination = (const SpecialCollection&) vcDestination;
      if (destination.isDoubleType())
         _moveTo((DoubleCollection&) destination, parameters, (Cursor*) cursor,
               (Cursor*) destinationCursor);
      else if (destination.isUniqueType())
         _moveTo((UniqueCollection&) destination, parameters, (Cursor*) cursor,
               (UniqueCollectionCursor*) destinationCursor);
      else {
         AssumeCondition(destination.isEmptyType())
      }
   }
   else
      inherited::pmoveTo(vcDestination, parameters, cursor, destinationCursor);
}

inline void
DoubleCollection::_moveAllTo(VirtualCollection& vcDestination, const ExtendedReplaceParameters& parameters,
      const VirtualCollectionCursor* startCursor, const VirtualCollectionCursor* endCursor,
      VirtualCollectionCursor* destinationCursor) {
   if (vcDestination.isSpecial()) {
      const auto& destination = (const SpecialCollection&) vcDestination;
      if (destination.isUniqueType())
         _moveAllTo((UniqueCollection&) destination, parameters, (Cursor*) startCursor,
               (Cursor*) endCursor, (UniqueCollectionCursor*) destinationCursor);
      else if (destination.isDoubleType())
         _moveAllTo((DoubleCollection&) destination, parameters, (Cursor*) startCursor,
               (Cursor*) endCursor, (Cursor*) destinationCursor);
      else {
         AssumeCondition(destination.isEmptyType())
      }
   }
   else
      inherited::pmoveAllTo(vcDestination, parameters, startCursor, endCursor, destinationCursor);
}

inline void
DoubleCollection::_swap(VirtualCollection& vcSource) {
   AssumeCondition(dynamic_cast<DoubleCollection*>(&vcSource))
   DoubleCollection& source = (DoubleCollection&) vcSource;
   if (this != &source) {
      InvalidateNotification invalidation;
      notifyCursorUpdate(invalidation);
      source.notifyCursorUpdate(invalidation);

      EnhancedObject* temp = peoFirst;
      peoFirst = source.peoFirst;
      source.peoFirst = temp;
      temp = peoSecond;
      peoSecond = source.peoSecond;
      source.peoSecond = temp;
   };
}

inline int
DoubleCollection::_queryCount(const ExtendedLocateParameters& parameters,
      const VirtualCollectionCursor* startCursor, const VirtualCollectionCursor* endCursor) const {
   inherited::_queryCount(parameters, startCursor, endCursor);
   int result = 0;
   if (!startCursor && !endCursor) {
      if (peoFirst) ++result;
      if (peoSecond) ++result;
   }
   else
      result = queryInternCount(parameters, (Cursor*) startCursor, (Cursor*) endCursor);
   return result;
}

#define DefTypeFinal DoubleCollection::
#define DefJustInline
   #include "Collection/ELMCollection.inch"
#undef DefJustInline
#undef DefTypeFinal

#undef DefTypeCollection
#undef DefTypeCursor

/******************************************************/
/* Definition of the template class TUniqueCollection */
/******************************************************/

template <class TypeElement, class Cast=SimpleCast>
class TUniqueCollectionCursor;

#define DefTypeElement TypeElement
#define DefTypeCollection TUniqueCollection<TypeElement, Cast>
#define DefTypeCastToCollectHandler Cast::castTo
#define DefTypeCCastToCollectHandler Cast::castTo
#define DefTypeCastFromCollectHandler (TypeElement*) Cast::castFrom
#define DefTypeCCastFromCollectHandler (const TypeElement*) Cast::castFrom
#define DefTypeInheritedCollection UniqueCollection
#define DefTypeCursor TUniqueCollectionCursor<TypeElement, Cast>

template <class TypeElement, class Cast=SimpleCast>
class TUniqueCollection : public UniqueCollection {
  private:
   typedef TUniqueCollection<TypeElement, Cast> thisType;

  public:
   typedef TypeElement Node;
   typedef TInitialValues<TypeElement, Cast> InitialValues;
   typedef TInitialNewValues<TypeElement, Cast> InitialNewValues;

   TUniqueCollection() {}
   TUniqueCollection(const InitialValues& initialValues)
      {  UniqueCollection::TInsertInitialValue<thisType, TypeElement, Cast> insertInitialValue(*this);
         initialValues.foreachDo(insertInitialValue);
      }
   TUniqueCollection(const InitialNewValues& initialValues)
      {  UniqueCollection::TInsertInitialNewValue<thisType, TypeElement, Cast> insertInitialValue(*this);
         initialValues.foreachDo(insertInitialValue);
      }
   TUniqueCollection(const thisType& source, AddMode mode=AMNoDuplicate,
         const VirtualCast* retrieveRegistrationFromCopy=nullptr)
      :  UniqueCollection(source, mode, retrieveRegistrationFromCopy) {}
   Template2DefineCopy(TUniqueCollection, TypeElement, Cast)
   Template2DefineCollectionForAbstractCollect(TUniqueCollection, TUniqueCollectionCursor, TypeElement, Cast)

   #include "Collection/ELMCollection.inch"

   TVirtualCollection<TypeElement, Cast>& getInterface()
      {  return (TVirtualCollection<TypeElement, Cast>&) (VirtualCollection&) *this; }
   const TVirtualCollection<TypeElement, Cast>& getInterface() const
      {  return (const TVirtualCollection<TypeElement, Cast>&) (const VirtualCollection&) *this; }

   template <class Execute> bool foreachDo(Execute& function) const
      {  return UniqueCollection::foreachDo(TemplateElementCastParameters<TypeElement, Cast>(), function); }
   template <class Execute> bool foreachDo(Execute& function, const ExtendedLocateParameters& parameters,
         const Cursor* start, const Cursor* end) const
      {  return UniqueCollection::foreachDo(TemplateElementCastParameters<TypeElement, Cast>(), function, parameters,
            start, end);
      }
   template <class Execute> bool foreachReverseDo(Execute& function) const
      {  return UniqueCollection::foreachReverseDo(TemplateElementCastParameters<TypeElement, Cast>(), function); }
   template <class Execute> bool foreachReverseDo(Execute& function, const ExtendedLocateParameters& parameters,
         const Cursor* start, const Cursor* end) const
      {  return UniqueCollection::foreachReverseDo(TemplateElementCastParameters<TypeElement, Cast>(), function, parameters,
            start, end);
      }
};
#undef DefTypeElement

#define DefTypeElement TypeElement
template<class TypeElement, class Cast>
class TUniqueCollectionCursor : public UniqueCollectionCursor {
  public:
   TUniqueCollectionCursor(const TUniqueCollection<TypeElement, Cast>& support) : UniqueCollectionCursor(support) {}
   TUniqueCollectionCursor(const TUniqueCollectionCursor<TypeElement, Cast>& source) : UniqueCollectionCursor(source) {}
   Template2DefineCopy(TUniqueCollectionCursor, TypeElement, Cast)
   Template2DefineCursorForAbstractCollect(TUniqueCollection, TUniqueCollectionCursor, TypeElement, Cast)

#define DefCursor
   #include "Collection/ELMCollection.inch"
#undef DefCursor
};

#undef DefTypeElement
#undef DefTypeCollection
#undef DefTypeCastToCollectHandler
#undef DefTypeCCastToCollectHandler
#undef DefTypeCastFromCollectHandler
#undef DefTypeCCastFromCollectHandler
#undef DefTypeInheritedCollection
#undef DefTypeCursor

Template2InlineCollectionForAbstractCollect(TUniqueCollection, TUniqueCollectionCursor, TypeElement, Cast)

/******************************************************/
/* Definition of the template class TDoubleCollection */
/******************************************************/

template <class TypeElement, class Cast=SimpleCast>
class TDoubleCollectionCursor;

#define DefTypeElement TypeElement
#define DefTypeCollection TDoubleCollection<TypeElement, Cast>
#define DefTypeCastToCollectHandler Cast::castTo
#define DefTypeCCastToCollectHandler Cast::castTo
#define DefTypeCastFromCollectHandler (TypeElement*) Cast::castFrom
#define DefTypeCCastFromCollectHandler (const TypeElement*) Cast::castFrom
#define DefTypeInheritedCollection DoubleCollection
#define DefTypeCursor TDoubleCollectionCursor<TypeElement, Cast>

template <class TypeElement, class Cast=SimpleCast>
class TDoubleCollection : public DoubleCollection {
  private:
   typedef TDoubleCollection<TypeElement, Cast> thisType;

  public:
   typedef TypeElement Node;
   typedef TInitialValues<TypeElement, Cast> InitialValues;
   typedef TInitialNewValues<TypeElement, Cast> InitialNewValues;

   TDoubleCollection() {}
   TDoubleCollection(const InitialValues& initialValues)
      {  DoubleCollection::TInsertInitialValue<thisType, TypeElement, Cast> insertInitialValue(*this);
         initialValues.foreachDo(insertInitialValue);
      }
   TDoubleCollection(const InitialNewValues& initialValues)
      {  DoubleCollection::TInsertInitialNewValue<thisType, TypeElement, Cast> insertInitialValue(*this);
         initialValues.foreachDo(insertInitialValue);
      }
   TDoubleCollection(const thisType& source, AddMode mode=AMNoDuplicate,
         const VirtualCast* retrieveRegistrationFromCopy=nullptr)
      :  DoubleCollection(source, mode, retrieveRegistrationFromCopy) {}
   Template2DefineCopy(TDoubleCollection, TypeElement, Cast)
   Template2DefineCollectionForAbstractCollect(TDoubleCollection, TDoubleCollectionCursor, TypeElement, Cast)

   #include "Collection/ELMCollection.inch"

   TVirtualCollection<TypeElement, Cast>& getInterface()
      {  return (TVirtualCollection<TypeElement, Cast>&) (VirtualCollection&) *this; }
   const TVirtualCollection<TypeElement, Cast>& getInterface() const
      {  return (const TVirtualCollection<TypeElement, Cast>&) (const VirtualCollection&) *this; }

   template <class Execute> bool foreachDo(Execute& function) const
      {  return DoubleCollection::foreachDo(TemplateElementCastParameters<TypeElement, Cast>(), function); }
   template <class Execute> bool foreachDo(Execute& function, const ExtendedLocateParameters& parameters,
         const Cursor* start, const Cursor* end) const
      {  return DoubleCollection::foreachDo(TemplateElementCastParameters<TypeElement, Cast>(), function, parameters,
            start, end);
      }
   template <class Execute> bool foreachReverseDo(Execute& function) const
      {  return DoubleCollection::foreachReverseDo(TemplateElementCastParameters<TypeElement, Cast>(), function); }
   template <class Execute> bool foreachReverseDo(Execute& function, const ExtendedLocateParameters& parameters,
         const Cursor* start, const Cursor* end) const
      {  return DoubleCollection::foreachReverseDo(TemplateElementCastParameters<TypeElement, Cast>(), function, parameters,
            start, end);
      }
};
#undef DefTypeElement

#define DefTypeElement TypeElement
template<class TypeElement, class Cast>
class TDoubleCollectionCursor : public DoubleCollectionCursor {
  public:
   TDoubleCollectionCursor(const TDoubleCollection<TypeElement, Cast>& support) : DoubleCollectionCursor(support) {}
   TDoubleCollectionCursor(const TDoubleCollectionCursor<TypeElement, Cast>& source) : DoubleCollectionCursor(source) {}
   Template2DefineCopy(TDoubleCollectionCursor, TypeElement, Cast)
   Template2DefineCursorForAbstractCollect(TDoubleCollection, TDoubleCollectionCursor, TypeElement, Cast)

#define DefCursor
   #include "Collection/ELMCollection.inch"
#undef DefCursor
};

#undef DefTypeElement
#undef DefTypeCollection
#undef DefTypeCastToCollectHandler
#undef DefTypeCCastToCollectHandler
#undef DefTypeCastFromCollectHandler
#undef DefTypeCCastFromCollectHandler
#undef DefTypeInheritedCollection
#undef DefTypeCursor

Template2InlineCollectionForAbstractCollect(TDoubleCollection, TDoubleCollectionCursor, TypeElement, Cast)

} // end of namespace COL

#endif // COL_SpecialH
