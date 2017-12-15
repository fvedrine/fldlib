/**************************************************************************/
/*                                                                        */
/*  This file is part of FLDLib                                           */
/*                                                                        */
/*  Copyright (C) 2014-2017                                               */
/*    CEA (Commissariat à l'énergie atomique et aux énergies              */
/*         alternatives)                                                  */
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
// Librairie   : Collection
// Module      : Abstract collections
// Fichier     : VirtualCollection.h
// Description :
//   Definition of the class VirtualCollection.
//   It is the base class of all collections and it brings a generic interface
//   to the class AbstractCollection.
//

#ifndef COL_VirtualCollectionH
#define COL_VirtualCollectionH

#include "Collection/VirtualCollection/ExtendedParameters.h"

namespace COL {

/*****************************/
/* Definition of collections */
/*****************************/

class VirtualCollection;
namespace DVirtualCollection {

class Access {
  private:
   class InitialStoreValue : public COL::ImplListElement {
     private:
      typedef COL::ImplListElement inherited;
      EnhancedObject* peoObject;

     public:
      InitialStoreValue(EnhancedObject& object) : inherited(), peoObject(&object) {}
      InitialStoreValue(const InitialStoreValue& source) = default;
      DefineCopy(InitialStoreValue)
      DDefineAssign(InitialStoreValue)

      EnhancedObject& extractElement()
         {  EnhancedObject* result = peoObject;
            AssumeCondition(result != nullptr)
            peoObject = nullptr;
            return *result;
         }
      virtual bool isValid() const override
         {  return inherited::isValid() && (peoObject != nullptr); }
   };

  public:
   class InitialValues : public COL::TImplList<InitialStoreValue> {
     private:
      typedef COL::TImplList<InitialStoreValue> inherited;
      
     protected:
      friend class COL::VirtualCollection;

     public:
      InitialValues() {}
      InitialValues(const InitialValues& source) : inherited(source, true) {}
      virtual ~InitialValues() { inherited::freeAll(); }
      DefineCopy(InitialValues)
      DDefineAssign(InitialValues)
      InitialValues& operator=(const InitialValues& source)
         {  inherited::assign(source, true); return *this; }

      InitialValues& operator<<(EnhancedObject& source)
         {  inherited::add(new InitialStoreValue(source)); return *this; }
   };

  private:
   class InitialStoreNewValue : public COL::ImplListElement {
     private:
      typedef COL::ImplListElement inherited;
      PNT::PassPointer<EnhancedObject> ppeoObject;

     public:
      InitialStoreNewValue(const PNT::PassPointer<EnhancedObject>& object)
         :  ppeoObject(object) {}
      InitialStoreNewValue(const InitialStoreNewValue& source) = default;
      DefineCopy(InitialStoreNewValue)
      DDefineAssign(InitialStoreNewValue)

      InitialStoreNewValue& operator=(const InitialStoreNewValue& source)
         {  inherited::operator=(source);
            ppeoObject.fullAssign(source.ppeoObject);
            return *this;
         }

      EnhancedObject* extractElement()
         {  AssumeCondition(ppeoObject.isValid())
            return ppeoObject.extractElement();
         }
      virtual bool isValid() const override
         {  return inherited::isValid() && ppeoObject.isValid(); }
   };

  public:
   class InitialNewValues : public COL::TImplList<InitialStoreNewValue> {
     private:
      typedef COL::TImplList<InitialStoreNewValue> inherited;
      
     protected:
      friend class COL::VirtualCollection;

     public:
      InitialNewValues() {}
      InitialNewValues(const InitialNewValues& source) : inherited(source, true) {}
      virtual ~InitialNewValues() { inherited::freeAll(); }
      DefineCopy(InitialNewValues)
      DDefineAssign(InitialNewValues)

      InitialNewValues operator=(const InitialNewValues& source)
         {  inherited::assign(source, true); return *this; }
      InitialNewValues& operator<<(const PNT::PassPointer<EnhancedObject>& source)
         {  inherited::add(new InitialStoreNewValue(source)); return *this; }
      InitialNewValues& operator<<(EnhancedObject* source)
         {  inherited::add(new InitialStoreNewValue(PNT::PassPointer<EnhancedObject>(source, PNT::Pointer::Init())));
            return *this;
         }
   };

   template <class TypeElement, class TypeCast>
   class TInitialValues : public InitialValues {
     private:
      typedef InitialValues inherited;
      typedef TInitialValues<TypeElement, TypeCast> thisType;
      
     public:
      TInitialValues() : inherited() {}
      TInitialValues(const thisType& source) : inherited(source) {}
      DefineCopy(thisType)

      thisType& operator<<(TypeElement& source)
         {  return (thisType&) inherited::operator<<(TypeCast::castTo(source)); }
   };
   template <class TypeElement, class TypeCast>
   class TInitialNewValues : public InitialNewValues {
     private:
      typedef InitialNewValues inherited;
      typedef TInitialNewValues<TypeElement, TypeCast> thisType;
      
     public:
      TInitialNewValues() : inherited() {}
      TInitialNewValues(const thisType& source) = default;
      DefineCopy(thisType)

      thisType& operator<<(const PNT::PassPointer<TypeElement>& element)
         {  return (thisType&) inherited::operator<<(PNT::PassPointer<EnhancedObject>
               (TypeCast::castTo(element.extractElement()), PNT::Pointer::Init()));
         }
      thisType& operator<<(TypeElement* element)
         {  return (thisType&) inherited::operator<<(TypeCast::castTo(element)); }
   };

   template <class TypeCollection, class TypeElement, class TypeCast>
   class TInsertInitialValue {
     private:
      TypeCollection& tcList;

     public:
      TInsertInitialValue(TypeCollection& list) : tcList(list) {}
      bool operator()(InitialStoreValue& storeValue)
         {  tcList.add((TypeElement*) TypeCast::castFrom(
                  (typename TypeCast::Base*) &storeValue.extractElement()));
            return true;
         }
   };
   template <class TypeCollection, class TypeElement, class TypeCast>
   friend class TInsertInitialValue;

   template <class TypeCollection, class TypeElement, class TypeCast>
   class TInsertInitialNewValue {
     private:
      TypeCollection& tcList;

     public:
      TInsertInitialNewValue(TypeCollection& list) : tcList(list) {}
      bool operator()(InitialStoreNewValue& storeValue)
         {  tcList.addNew((TypeElement*) TypeCast::castFrom(
                  (typename TypeCast::Base*) storeValue.extractElement()));
            return true;
         }
   };
   template <class TypeCollection, class TypeElement, class TypeCast>
   friend class TInsertInitialNewValue;

   typedef DVirtualCollection::VirtualCastWithElement VirtualCastWithElement;
};

} // end of namespace DVirtualCollection

class VirtualCollectionCursor;
class VirtualCollection
   :  public AbstractCollection,
      public DVirtualCollection::Definitions,
      protected DVirtualCollection::Access {
  public:
   class CollectionAccess;
   typedef DVirtualCollection::Access::VirtualCastWithElement VirtualCastWithElement;

  protected:
   class InvalidateNotification;

  public:
   friend class VirtualCollectionCursor;
   typedef DVirtualCollection::ExtendedLocateParameters ExtendedLocateParameters;
   typedef DVirtualCollection::ExtendedInsertionParameters ExtendedInsertionParameters;
   typedef DVirtualCollection::ExtendedSuppressParameters ExtendedSuppressParameters;
   typedef DVirtualCollection::ExtendedReplaceParameters ExtendedReplaceParameters;
   virtual void handleError() { /* AssumeCondition(false) */ }

  private:
   typedef VirtualCollection thisType;

  protected:
   bool isExcluded(RelativePosition insertPos, RelativePosition intervalMode,
      const VirtualCollectionCursor* startCursor, const VirtualCollectionCursor* endCursor,
      const VirtualCollectionCursor* cursor) const;
   typedef const VirtualCollectionCursor* PCVirtualCollectionCursor;
   bool normalize(PCVirtualCollectionCursor& startCursor, PCVirtualCollectionCursor& endCursor) const;
   bool normalizeReverse(PCVirtualCollectionCursor& startCursor, PCVirtualCollectionCursor& endCursor) const;

  protected:
   VirtualCollection() {}
   VirtualCollection(const VirtualCollection& source, AddMode duplicate=AMNoDuplicate)
      :  AbstractCollection(source) {}

  public:
   template <class TypeElement>
   class AddGuardian : public ExtendedParameters {
     private:
      VirtualCollection& vcCollection;
      TypeElement* peNewElement;
      const DVirtualCollection::VirtualCastWithElement* pvcweCast;
      TemplateDefineExtendedParameters(2, ExtendedParameters)
      DefineSubExtendedParameters(FreeOnError, 1, INHERITED)
      DefineSubExtendedParameters(CanHandleDuplication, 1, FreeOnError)

      AddGuardian(const AddGuardian&) { AssumeUncalled }
      AddGuardian& operator=(const AddGuardian&) { AssumeUncalled return *this; }

     public:
      AddGuardian(VirtualCollection& collection, TypeElement* newElement, const ExtendedInsertionParameters& params)
         :  vcCollection(collection), peNewElement(newElement), pvcweCast(nullptr)
         {  AssumeCondition(newElement)
            if (params.isDuplicate()) {
               mergeCanHandleDuplicationField(1);
               pvcweCast = params.getSCast();
            }
            else if (params.isFreeOnError())
               mergeFreeOnErrorField(1);
         }
      ~AddGuardian()
         {  if (hasFreeOnErrorField() && peNewElement)
               {  delete peNewElement;
                  vcCollection.handleError();
               };
         }

      TypeElement* handleDuplication()
         {  if (hasCanHandleDuplicationField()) {
               peNewElement = pvcweCast
                  ? (TypeElement*) TypeElement::castFromCopyHandler(pvcweCast
                        ->castFrom(peNewElement->createCopy(), TypeElement::castToCopyHandler(peNewElement)))
                  : peNewElement->createSCopy();
               mergeFreeOnErrorField(1);
               clearCanHandleDuplicationField();
            };
            return peNewElement;
         }
      TypeElement* handleDuplication(const VirtualCast& cast)
         {  pvcweCast = (const DVirtualCollection::VirtualCastWithElement*) &cast;
            return handleDuplication();
         }
      TypeElement* handleDuplication(const VirtualCast* cast)
         {  pvcweCast = (const DVirtualCollection::VirtualCastWithElement*) cast;
            return handleDuplication();
         }
      void release() { clearOwnField(); peNewElement = nullptr; }
   };

  protected:
   // overload methods
   virtual bool acceptElement(const EnhancedObject& asource) const { return true; }
   friend class CollectionAccess;
   virtual void _fullAssign(const VirtualCollection& source, const ExtendedReplaceParameters& parameters);
   void pfullAssign(const VirtualCollection& source, const ExtendedReplaceParameters& parameters);

   // add methods
   virtual void _add(EnhancedObject* anObject, const ExtendedInsertionParameters& parameters,
         VirtualCollectionCursor* cursor=nullptr);
   void paddAll(const VirtualCollection& source, const ExtendedInsertionParameters& parameters,
         VirtualCollectionCursor* cursor=nullptr, const VirtualCollectionCursor* startSource=nullptr,
         const VirtualCollectionCursor* endSource=nullptr);
   virtual void _addAll(const VirtualCollection& source, const ExtendedInsertionParameters& parameters,
         VirtualCollectionCursor* cursor=nullptr, const VirtualCollectionCursor* startSource=nullptr,
         const VirtualCollectionCursor* endSource=nullptr);

   // move methods
   void pmoveTo(VirtualCollection& destination, const ExtendedReplaceParameters& parameters,
         VirtualCollectionCursor* cursor=nullptr, VirtualCollectionCursor* destinationCursor=nullptr);
   virtual void _moveTo(VirtualCollection& destination, const ExtendedReplaceParameters& parameters,
         VirtualCollectionCursor* cursor=nullptr, VirtualCollectionCursor* destinationCursor=nullptr);
   void pmoveAllTo(VirtualCollection& destination, const ExtendedReplaceParameters& parameters,
         const VirtualCollectionCursor* startCursor=nullptr, const VirtualCollectionCursor* endCursor=nullptr,
         VirtualCollectionCursor* destinationCursor=nullptr);
   virtual void _moveAllTo(VirtualCollection& destination, const ExtendedReplaceParameters& parameters,
         const VirtualCollectionCursor* startCursor=nullptr, const VirtualCollectionCursor* endCursor=nullptr,
         VirtualCollectionCursor* destinationCursor=nullptr);
   virtual void _swap(VirtualCollection& source) {}

   // remove methods
   virtual void _remove(const ExtendedSuppressParameters& parameters, VirtualCollectionCursor* cursor=nullptr);
   virtual void _removeAll(const ExtendedSuppressParameters& parameters,
         const VirtualCollectionCursor* start=nullptr, const VirtualCollectionCursor* end=nullptr);
   void premoveAll(const ExtendedSuppressParameters& parameters,
         const VirtualCollectionCursor* start=nullptr, const VirtualCollectionCursor* end=nullptr);

   // replace method
   virtual void _replace(EnhancedObject* anObject,
         const ExtendedReplaceParameters& parameters, VirtualCollectionCursor* cursor=nullptr);

   // query methods
   int pqueryCount(const ExtendedLocateParameters& parameters,
         const VirtualCollectionCursor* startCursor=nullptr, const VirtualCollectionCursor* endCursor=nullptr) const;
   virtual int _queryCount(const ExtendedLocateParameters& parameters,
         const VirtualCollectionCursor* startCursor=nullptr, const VirtualCollectionCursor* endCursor=nullptr) const
      { return 0; }
   // virtual int _queryCount() const { return 0; }
   virtual EnhancedObject* _getElement(const ExtendedLocateParameters& parameters,
         const VirtualCollectionCursor* cursor=nullptr) const;

   template <class TElement, class TCast>
   class TemplateForeachParameters {
     public:
      typedef TElement Element;
      typedef TCast Cast;
   };

   template <class TCollection, class TInheritedCollection, class TElement, class TKeyTraits, class TCast>
   class TemplateQueryParameters {
     public:
      typedef TCollection TypeCollection;
      typedef TInheritedCollection TypeInheritedCollection;
      typedef TElement Element;
      typedef TKeyTraits KeyTraits;
      typedef TCast Cast;

      static typename TypeCollection::LocationResult locateKey(const TypeCollection& thisCollection,
            typename KeyTraits::KeyType key, const ExtendedLocateParameters& parameters,
            typename TypeCollection::Cursor* cursor)
         { return thisCollection._locateKey(key, parameters, cursor); }
      static Element* getInheritedElement(const TypeInheritedCollection& thisCollection,
            const ExtendedLocateParameters& parameters, typename TypeCollection::Cursor* cursor)
         { return ((const TypeCollection&) thisCollection).getInheritedElement(parameters, cursor); }
   };

   template <class TypeQueryParameters> typename TypeQueryParameters::Element*
      tgetElement(TypeQueryParameters, const ExtendedLocateParameters& parameters,
         const VirtualCollectionCursor* cursor=nullptr) const
      {  typedef typename TypeQueryParameters::TypeCollection TypeCollection;
         typedef typename TypeQueryParameters::TypeInheritedCollection TypeInheritedCollection;
         typedef typename TypeQueryParameters::Element Element;
         typedef typename TypeQueryParameters::KeyTraits KeyTraits;
         Element* result = nullptr;
         if (parameters.isUndefined() && parameters.hasKey()) {
            AssumeCondition(dynamic_cast<const DVirtualCollection::TKeyHandler<KeyTraits>*>
               (&parameters.getKeyHandler()))
            typename TypeCollection::PPCursor cursor = ((const TypeCollection&) *this).newCursor();
            TypeQueryParameters::locateKey((const TypeCollection&) *this,
               ((const DVirtualCollection::TKeyHandler<KeyTraits>&)
                  parameters.getKeyHandler()).queryKey(),
               parameters, cursor.key());
            result = cursor->getSElement();
         }
         else
            result = TypeQueryParameters::getInheritedElement(
               (const TypeInheritedCollection&) *this, parameters,
               (typename TypeCollection::Cursor*) cursor);
         return result;
      }

  public:
   typedef EnhancedObject Node;
   DefineCopy(VirtualCollection)
   DDefineAssign(VirtualCollection)

   VirtualCollection& operator=(const VirtualCollection& source)
      {  PureAbstractCollection::operator=(source);
         _fullAssign(source, ExtendedReplaceParameters());
         return *this;
      }

  protected:
   virtual void assumeCompatibility(AbstractCursor& cursor) const override;
   virtual AbstractCursor* createNewCursor() const override;
  public:
   typedef PNT::TPassPointer<VirtualCollectionCursor, PPAbstractCursor> PPCursor;
   typedef VirtualCollectionCursor Cursor;
   virtual AbstractCollection* createDefault() const { return new VirtualCollection(); }
   VirtualCollection* createSDefault() const { return (VirtualCollection*) createDefault(); }
   PPCursor newCursor() const { return PPCursor(snewCursor()); }
   virtual bool isSpecial() const { return false; }

   #include "Collection/ELMCollection.inch"

   template <class Execute, class TemplateParameters>
      bool foreachDo(TemplateParameters traits, Execute& fun, const ExtendedLocateParameters& parameters,
         const VirtualCollectionCursor* start=nullptr, const VirtualCollectionCursor* end=nullptr) const;
   template <class Execute, class TemplateParameters>
      bool foreachSDo(TemplateParameters traits, Execute& fun, const ExtendedLocateParameters& parameters,
         const typename TemplateParameters::TypeCursor* start=nullptr,
         const typename TemplateParameters::TypeCursor* end=nullptr) const;
   template <class Execute, class TemplateParameters>
      bool foreachDo(TemplateParameters parameters, Execute& fun) const
      {  return foreachDo(parameters, fun, ExtendedLocateParameters()); }
   bool foreachDo(std::function<bool (EnhancedObject&)> func) const
      {  return foreachDo(TemplateElementCastParameters<EnhancedObject, SimpleCast>(),
            func, ExtendedLocateParameters());
      }

   template <class Execute, class TemplateParameters>
      bool foreachReverseDo(TemplateParameters traits, Execute& fun, const ExtendedLocateParameters& parameters,
         const VirtualCollectionCursor* start=nullptr, const VirtualCollectionCursor* end=nullptr) const;
   template <class Execute, class TemplateParameters>
      bool foreachSReverseDo(TemplateParameters traits, Execute& fun, const ExtendedLocateParameters& parameters,
         const typename TemplateParameters::TypeCursor* start=nullptr,
         const typename TemplateParameters::TypeCursor* end=nullptr) const;
   template <class Execute, class TemplateParameters>
      bool foreachReverseDo(TemplateParameters parameters, Execute& fun) const
      {  return foreachReverseDo(parameters, fun, ExtendedLocateParameters()); }
   bool foreachReverseDo(std::function<bool (EnhancedObject&)> func) const
      {  return foreachReverseDo(TemplateElementCastParameters<EnhancedObject, SimpleCast>(),
            func, ExtendedLocateParameters());
      }

   class iterator;
   typedef iterator const_iterator;
   iterator begin() const;
   iterator end() const;

   class reverse_iterator;
   typedef reverse_iterator const_reverse_iterator;
   reverse_iterator rbegin() const;
   reverse_iterator rend() const;

   template <class TypeCollection, class TypeElement> class titerator;
   template <class TypeCollection, class TypeElement> class treverse_iterator;
};

typedef PNT::AutoPointer<VirtualCollection> APVirtualCollection;

/*********************/
/* Cursor definition */
/*********************/

class VirtualCollectionCursor : public AbstractCursor {
  public:
   class Position : public EnhancedObject, public ExtendedParameters {
     public:
      enum Direction { DInvalid=0, DStart=1, DEnd=2, DExact=3, DForward=4, DBackward=5, DAny=6 };

     protected:
      DefineExtendedParameters(3, ExtendedParameters)

     public:
      Position() {}
      Position(Direction dir) { mergeOwnField(dir); }
      Position(const Position& source) : ExtendedParameters(source) {}
      DefineCopy(Position)
      DDefineAssign(Position)

      Direction getDirection() const { return (Direction) queryOwnField(); }
      Position& setDirection(Direction dir) { setOwnField(dir); return *this; }
      Position& setInvalid() { clearOwnField(); return *this; }
      Position& setDirection(VirtualCollection::RelativePosition pos)
         {  AssumeCondition((pos == VirtualCollection::RPAfter) || (pos == VirtualCollection::RPBefore))
            setOwnField(pos == VirtualCollection::RPBefore ? DBackward : DForward );
            return *this;
         }
      Position& setStartDirection(VirtualCollection::RelativePosition pos)
         {  AssumeCondition((pos == VirtualCollection::RPAfter) || (pos == VirtualCollection::RPBefore))
            setOwnField(pos == VirtualCollection::RPBefore ? DEnd : DStart );
            return *this;
         }
      Position& setStart() { setOwnField(DStart); return *this; }
      Position& setEnd() { setOwnField(DEnd); return *this; }
      Position& setExact() { setOwnField(DExact); return *this; }
      Position& setAny() { setOwnField(DAny); return *this; }
      Position& setBackward() { setOwnField(DBackward); return *this; }
      Position& setForward() { setOwnField(DForward); return *this; }

      bool isInvalid() const { return getDirection() == DInvalid; }
      bool isStart() const { return getDirection() == DStart; }
      bool isEnd() const { return getDirection() == DEnd; }
      bool isExact() const { return getDirection() == DExact; }
      bool isAny() const { return getDirection() == DAny; }
      bool isForward() const { return getDirection() == DForward; }
      bool isBackward() const { return getDirection() == DBackward; }
      bool isNear() const
         {  Direction direction = getDirection();
            return (direction == DForward) || (direction == DBackward);
         }
      bool isExtremity() const
         {  Direction direction = getDirection();
            return (direction == DStart) || (direction == DEnd);
         }
   };
   class CursorAccess;

  protected:
   typedef VirtualCollection::ExtendedLocateParameters ExtendedLocateParameters;
   typedef VirtualCollection::ExtendedSuppressParameters ExtendedSuppressParameters;
   typedef VirtualCollection::ExtendedInsertionParameters ExtendedInsertionParameters;
   typedef VirtualCollection::ExtendedReplaceParameters ExtendedReplaceParameters;
   VirtualCollectionCursor(const VirtualCollection& support) : AbstractCursor(support) {}
   VirtualCollectionCursor(const VirtualCollectionCursor& source) : AbstractCursor(source) {}

   friend class CursorAccess;
   virtual bool _isPositionned(const ExtendedLocateParameters& pos,
         const VirtualCollectionCursor* cursor=nullptr) const
      {  return false; }
   virtual bool _position(const Position& pos) { return false; }
   virtual EnhancedObject* _getSElement() const { AssumeUncalled return nullptr; }
   virtual bool _isEqual(const AbstractCursor& cursor) const override
      {  return AbstractCursor::_isEqual(cursor); }
   virtual void _gotoReference(const EnhancedObject& element) { AssumeUncalled }

  public:
   DefineCopy(VirtualCollectionCursor)
   DefineCursorForAbstractCollect(VirtualCollection, VirtualCollectionCursor)

#define DefCursor
   #include "Collection/ELMCollection.inch"
#undef DefCursor
   VirtualCollectionCursor& operator+=(int add);
   VirtualCollectionCursor& operator-=(int add);
};

InlineCollectionForAbstractCollect(VirtualCollection, VirtualCollectionCursor)

class VirtualCollection::InvalidateNotification : public CursorNotification {
  protected:
   void update(VirtualCollectionCursor& cursor)
      {  if (!hasOrigin() || !cursor.isEqual(getOrigin()))
            cursor.invalidate();
      }

  public:
   InvalidateNotification() {}
   InvalidateNotification(const InvalidateNotification& source) : CursorNotification(source) {}
   DefineCursorNotificationMethods(InvalidateNotification, VirtualCollectionCursor)
};

#include "Collection/VirtualCollection/VirtualCollection.inline"

/*********************************/
/* Definition of TCopyCollection */
/*********************************/

template <class TypeCollection>
class TCopyCollection : public TypeCollection {
  public:
   typedef typename TypeCollection::InitialValues InitialValues;
   typedef typename TypeCollection::InitialNewValues InitialNewValues;

  public:
   typedef typename TypeCollection::ExtendedLocateParameters ExtendedLocateParameters;
   typedef typename TypeCollection::ExtendedInsertionParameters ExtendedInsertionParameters;
   typedef typename TypeCollection::ExtendedSuppressParameters ExtendedSuppressParameters;
   typedef typename TypeCollection::ExtendedReplaceParameters ExtendedReplaceParameters;

  protected:
   virtual ComparisonResult _compare(const EnhancedObject& asource) const override
      {  return TypeCollection::_compare(asource); }
   TCopyCollection(const TCopyCollection<TypeCollection>& source,
         COL::VirtualCollection::AddMode mode, const VirtualCast* retrieveRegistrationFromCopy)
      :  TypeCollection(source, mode, retrieveRegistrationFromCopy) {}

  public:
   TCopyCollection() {}
   TCopyCollection(const typename TypeCollection::InitialValues& initialValues)
      :  TypeCollection(initialValues) {}
   TCopyCollection(const typename TypeCollection::InitialNewValues& initialValues)
      :  TypeCollection(initialValues) {}
   TCopyCollection(const TCopyCollection<TypeCollection>& source)
      :  TypeCollection(source, TypeCollection::AMDuplicate) {}
   virtual ~TCopyCollection() { TypeCollection::_removeAll(ExtendedSuppressParameters().setFree()); }
   TemplateDefineCopy(TCopyCollection, TypeCollection)
   DTemplateDefineAssign(TCopyCollection, TypeCollection)
   TCopyCollection<TypeCollection>& operator=(const TCopyCollection<TypeCollection>& source)
      {  TypeCollection::_fullAssign((const TypeCollection&) source,
            ExtendedReplaceParameters().setDuplicate().setFree());
         return *this;
      }
};

/****************************************/
/* Définition du patron TCastCollection */
/****************************************/

template <class TypeBase, class TypeElement, class Cast>
class TCastCollectionCursor;

#define DefTypeInheritedCollection TypeBase
#define DefTypeElement TypeElement
#define DefTypeCollection TCastCollection<TypeBase, TypeElement, Cast>
#define DefTypeCastToCollectHandler Cast::castTo
#define DefTypeCCastToCollectHandler Cast::castTo
#define DefTypeCastFromCollectHandler(object) (TypeElement*) Cast::castFrom((typename Cast::Base*) object)
#define DefTypeCCastFromCollectHandler(object) (const TypeElement*) Cast::castFrom((const typename Cast::Base*) object)
#define DefTypeCursor TCastCollectionCursor<TypeBase, TypeElement, Cast>

template <class TypeBase, class TypeElement, class Cast>
class TCastCollection : public TypeBase {
  public:
   typedef typename TypeBase::AddMode AddMode;
   typedef typename TypeBase::RemoveMode RemoveMode;
   typedef typename TypeBase::RelativePosition RelativePosition;
   typedef typename TypeBase::ExtendedLocateParameters ExtendedLocateParameters;
   typedef typename TypeBase::ExtendedInsertionParameters ExtendedInsertionParameters;
   typedef typename TypeBase::ExtendedSuppressParameters ExtendedSuppressParameters;
   typedef typename TypeBase::ExtendedReplaceParameters ExtendedReplaceParameters;
   typedef typename VirtualCollection::TInitialValues<TypeElement, Cast> InitialValues;
   typedef typename VirtualCollection::TInitialNewValues<TypeElement, Cast> InitialNewValues;
   TCastCollection() {}
   TCastCollection(const InitialValues& initialValues)
      {  VirtualCollection::TInsertInitialValue<TCastCollection<TypeBase, TypeElement, Cast>, TypeElement, Cast>
            insertInitialValue(*this);
         initialValues.foreachDo(insertInitialValue);
      }
   TCastCollection(const InitialNewValues& initialValues)
      {  VirtualCollection::TInsertInitialNewValue<TCastCollection<TypeBase, TypeElement, Cast>, TypeElement, Cast>
            insertInitialValue(*this);
         initialValues.foreachDo(insertInitialValue);
      }
   TCastCollection(const TCastCollection<TypeBase, TypeElement, Cast>& source,
         VirtualCollection::AddMode duplicate = VirtualCollection::AMNoDuplicate,
         const VirtualCast* retrieveRegistrationFromCopy=nullptr)
      :  TypeBase(source, duplicate, retrieveRegistrationFromCopy) {}
   Template3DefineCopy(TCastCollection, TypeBase, TypeElement, Cast)
   Template3DefineCollectionForAbstractCollect(TCastCollection, TCastCollectionCursor, TypeBase, TypeElement, Cast)

#define DefJustDeclare
   #include "Collection/ELMCollection.inch"
#undef DefJustDeclare
   
   bool foreachDo(std::function<bool (TypeElement&)> func) const
      { return TypeBase::foreachDo(TemplateElementCastParameters<TypeElement, Cast>(), func); }
   bool foreachDo(std::function<bool (TypeElement&)> func, const ExtendedLocateParameters& parameters,
         const Cursor* start=nullptr, const Cursor* end=nullptr) const
      {  return TypeBase::foreachDo(TemplateElementCastParameters<TypeElement, Cast>(), func,
            parameters, start, end);
      }
   template <class Execute> bool foreachDo(Execute& function) const
      { return TypeBase::foreachDo(TemplateElementCastParameters<TypeElement, Cast>(), function); }
   template <class Execute> bool foreachDo(Execute& function, const ExtendedLocateParameters& parameters,
         const Cursor* start=nullptr, const Cursor* end=nullptr) const
      {  return TypeBase::foreachDo(TemplateElementCastParameters<TypeElement, Cast>(), function, parameters,
            start, end);
      }
   bool foreachReverseDo(std::function<bool (TypeElement&)> func) const
      { return TypeBase::foreachReverseDo(TemplateElementCastParameters<TypeElement, Cast>(), func); }
   bool foreachReverseDo(std::function<bool (TypeElement&)> func, const ExtendedLocateParameters& parameters,
         const Cursor* start=nullptr, const Cursor* end=nullptr) const
      {  return TypeBase::foreachReverseDo(TemplateElementCastParameters<TypeElement, Cast>(), func,
            parameters, start, end);
      }
   template <class Execute> bool foreachReverseDo(Execute& function) const
      { return TypeBase::foreachReverseDo(TemplateElementCastParameters<TypeElement, Cast>(), function); }
   template <class Execute> bool foreachReverseDo(Execute& function, const ExtendedLocateParameters& parameters,
      const Cursor* start=nullptr, const Cursor* end=nullptr) const
      {  return TypeBase::foreachReverseDo(TemplateElementCastParameters<TypeElement, Cast>(), function, parameters,
            start, end);
      }
};

template <class TypeBase, class TypeElement, class Cast>
class TCastCollectionCursor : public TypeBase::Cursor {
  private:
   typedef TCastCollectionCursor<TypeBase, TypeElement, Cast> thisType;
   typedef TCastCollection<TypeBase, TypeElement, Cast> SupportType;
   typedef typename TypeBase::Cursor inherited;

  protected:
   virtual EnhancedObject* _getSElement() const override { return inherited::_getSElement(); }

  public:
   typedef typename TypeBase::ExtendedLocateParameters ExtendedLocateParameters;
   TCastCollectionCursor(const SupportType& support) : inherited(support) {}
   TCastCollectionCursor(const thisType& source) : inherited(source) {}
   Template3DefineCopy(TCastCollectionCursor, TypeBase, TypeElement, Cast)
   Template3DefineCursorForAbstractCollect(TCastCollection, TCastCollectionCursor, TypeBase, TypeElement, Cast)

#define DefCursor
   #include "Collection/ELMCollection.inch"
#undef DefCursor
};

Template3InlineCollectionForAbstractCollect(TCastCollection, TCastCollectionCursor, TypeBase, TypeElement, Cast)

#define DefTemplate template <class TypeBase, class TypeElement, class Cast>
#define DefJustInline
   #include "Collection/ELMCollection.inch"
#undef DefJustInline
#undef DefTemplate

#undef DefTypeCursor
#undef DefTypeCastToCollectHandler
#undef DefTypeCCastToCollectHandler
#undef DefTypeCastFromCollectHandler
#undef DefTypeCCastFromCollectHandler
#undef DefTypeCollection
#undef DefTypeElement
#undef DefTypeInheritedCollection

/************************************/
/* Definition of TVirtualCollection */
/************************************/

#define DefTypeCollection TVirtualCollection<TypeElement, Cast>
#define DefTypeCursor TVirtualCollectionCursor<TypeElement, Cast>
#define DefTypeElement TypeElement
#define DefTypeCastToCollectHandler Cast::castTo
#define DefTypeCCastToCollectHandler Cast::castTo
#define DefTypeCastFromCollectHandler(object) ((TypeElement*) Cast::castFrom(object))
#define DefTypeCCastFromCollectHandler(object) ((const TypeElement*) Cast::castFrom(object))

template <class TypeElement, class Cast=SimpleCast>
class TVirtualCollectionCursor;

template <class TypeElement, class Cast=SimpleCast>
class TVirtualCollection : public VirtualCollection {
  private:
   typedef VirtualCollection inherited;
   typedef TVirtualCollection<TypeElement, Cast> thisType;

  public:
   Template2DefineCollectionForAbstractCollect(TVirtualCollection, TVirtualCollectionCursor, TypeElement, Cast)
   friend class TVirtualCollectionCursor<TypeElement, Cast>;

  protected:
   TVirtualCollection() {}
   TVirtualCollection(const thisType& source, AddMode mode=AMNoDuplicate,
         const VirtualCast* retrieveRegistrationFromCopy=nullptr)
      :  inherited(source, mode) {}

  public:
   Template2DefineCopy(TVirtualCollection, TypeElement, Cast)

#define DefJustDeclare
   #include "Collection/ELMCollection.inch"
#undef DefJustDeclare

   class VirtualExecute {
     public:
      virtual ~VirtualExecute() {}
      virtual bool operator()(TypeElement& source) = 0;
   };

   bool vforeachDo(VirtualExecute& fun, const ExtendedLocateParameters& parameters,
         const Cursor* start=nullptr, const Cursor* end=nullptr) const
      {  return inherited::foreachDo(inherited::TemplateForeachParameters<TypeElement, Cast>(),
            fun, parameters, start, end);
      }
   bool foreachDo(std::function<bool (TypeElement&)> func, const ExtendedLocateParameters& parameters,
         const Cursor* start=nullptr, const Cursor* end=nullptr) const
      {  return inherited::foreachDo(inherited::TemplateForeachParameters<TypeElement, Cast>(),
            func, parameters, start, end);
      }
   bool vforeachDo(VirtualExecute& fun) const
      {  return inherited::foreachDo(inherited::TemplateForeachParameters<TypeElement, Cast>(),
            fun, ExtendedLocateParameters());
      }
   bool foreachDo(std::function<bool (TypeElement&)> func) const
      {  return inherited::foreachDo(inherited::TemplateForeachParameters<TypeElement, Cast>(),
            func, ExtendedLocateParameters());
      }

   template <class Execute>
      bool foreachDo(Execute& fun, const ExtendedLocateParameters& parameters,
         const Cursor* start=nullptr, const Cursor* end=nullptr) const
      {  return inherited::foreachDo(inherited::TemplateForeachParameters<TypeElement, Cast>(),
            fun, parameters, start, end);
      }
   template <class Execute>
      bool foreachDo(Execute& fun) const
      {  return inherited::foreachDo(inherited::TemplateForeachParameters<TypeElement, Cast>(),
            fun, ExtendedLocateParameters());
      }
   bool vforeachReverseDo(VirtualExecute& fun, const ExtendedLocateParameters& parameters,
         const Cursor* start=nullptr, const Cursor* end=nullptr) const
      {  return inherited::foreachReverseDo(inherited::TemplateForeachParameters<TypeElement, Cast>(),
            fun, parameters, start, end);
      }
   bool vforeachReverseDo(VirtualExecute& fun) const
      {  return inherited::foreachReverseDo(inherited::TemplateForeachParameters<TypeElement, Cast>(),
            fun, ExtendedLocateParameters());
      }
   bool foreachReverseDo(std::function<bool (TypeElement&)> func, const ExtendedLocateParameters& parameters,
         const Cursor* start=nullptr, const Cursor* end=nullptr) const
      {  return inherited::foreachReverseDo(inherited::TemplateForeachParameters<TypeElement, Cast>(),
            func, parameters, start, end);
      }
   bool foreachReverseDo(std::function<bool (TypeElement&)> func) const
      {  return inherited::foreachReverseDo(inherited::TemplateForeachParameters<TypeElement, Cast>(),
            func, ExtendedLocateParameters());
      }
   template <class Execute>
      bool foreachReverseDo(Execute& fun, const ExtendedLocateParameters& parameters,
         const Cursor* start=nullptr, const Cursor* end=nullptr) const
      {  return inherited::foreachReverseDo(inherited::TemplateForeachParameters<TypeElement, Cast>(),
            fun, parameters, start, end);
      }
   template <class Execute>
      bool foreachReverseDo(Execute& fun) const
      {  return inherited::foreachReverseDo(inherited::TemplateForeachParameters<TypeElement, Cast>(),
            fun, ExtendedLocateParameters());
      }
};

template <class TypeElement, class Cast>
class TVirtualCollectionCursor : public VirtualCollectionCursor {
  private:
   typedef VirtualCollectionCursor inherited;
   typedef TVirtualCollectionCursor<TypeElement, Cast> thisType;

  protected:
   TVirtualCollectionCursor(const thisType& source) : inherited(source) {}
   TVirtualCollectionCursor(const TVirtualCollection<TypeElement, Cast>& support)
      : inherited(support) {}

  public:
   Template2DefineCopy(TVirtualCollectionCursor, TypeElement, Cast)
   Template2DefineCursorForAbstractCollect(TVirtualCollection, TVirtualCollectionCursor, TypeElement, Cast)

#define DefCursor
   #include "Collection/ELMCollection.inch"
#undef DefCursor
};

Template2InlineCollectionForAbstractCollect(TVirtualCollection, TVirtualCollectionCursor, TypeElement, Cast)

#define DefTemplate template <class TypeElement, class Cast>
#define DefJustInline
   #include "Collection/ELMCollection.inch"
#undef DefJustInline
#undef DefTemplate

#undef DefTypeCollection
#undef DefTypeCursor
#undef DefTypeElement
#undef DefTypeCastToCollectHandler
#undef DefTypeCCastToCollectHandler
#undef DefTypeCastFromCollectHandler
#undef DefTypeCCastFromCollectHandler

class VirtualCollection::iterator : public VirtualCollection::PPCursor {
  private:
   typedef VirtualCollection::PPCursor inherited;

  protected:
   friend class VirtualCollection;
   
  public:
   iterator(const iterator& source) : inherited(source, PNT::Pointer::Duplicate()) {}
   iterator(iterator&& source) : inherited(source) {}
   iterator(PPCursor&& source) : inherited(source) {}
   
   bool operator==(const iterator& source) const
      {  return inherited::operator*().isEqual(source.inherited::operator*()); }
   bool operator!=(const iterator& source) const
      {  return !inherited::operator*().isEqual(source.inherited::operator*()); }
   iterator& operator=(const iterator& source)
      {  inherited::fullAssign(source); return *this; }
   iterator& operator=(iterator&& source)
      {  return (iterator&) inherited::operator=(source); }

   iterator& operator++() { inherited::operator*().setToNext(); return *this; }
   iterator& operator+=(int add)
      {  if (add > 0) {
            while (--add >= 0)
               inherited::operator*().setToNext();
         }
         else if (add < 0) {
            while (++add <= 0)
               inherited::operator*().setToPrevious();
         };
         return *this;
      }
   iterator operator+(int add) const
      {  iterator result(*this);
         if (add > 0) {
            while (--add >= 0)
               result.inherited::operator*().setToNext();
         }
         else if (add < 0) {
            while (++add <= 0)
               result.inherited::operator*().setToPrevious();
         };
         return result;
      }
   iterator& operator--() { inherited::operator*().setToPrevious(); return *this; }
   iterator& operator-=(int add) { return operator+=(-add); }
   iterator operator-(int sub) { return operator+(-sub); }
   EnhancedObject& operator*() const { return inherited::operator*().elementSAt(); }
   EnhancedObject* operator->() const { return &inherited::operator*().elementSAt(); }
};

inline VirtualCollection::iterator
VirtualCollection::begin() const {
   iterator result = newCursor();
   ((const PPCursor&) result)->setToFirst();
   return result;
}

inline VirtualCollection::iterator
VirtualCollection::end() const
   {  return iterator(newCursor()); }

class VirtualCollection::reverse_iterator : public VirtualCollection::PPCursor {
  private:
   typedef VirtualCollection::PPCursor inherited;

  protected:
   friend class VirtualCollection;
   reverse_iterator(Cursor* cursor, PPCursor::Init init) : PPCursor(cursor, init) {}
   
  public:
   reverse_iterator(const reverse_iterator& source) : inherited(source, PNT::Pointer::Duplicate()) {}
   reverse_iterator(reverse_iterator&& source) : inherited(source) {}
   reverse_iterator(PPCursor&& source) : inherited(source) {}
   
   bool operator==(const iterator& source) const
      {  return inherited::operator*().isEqual(source.inherited::operator*()); }
   bool operator!=(const iterator& source) const
      {  return !inherited::operator*().isEqual(source.inherited::operator*()); }

   reverse_iterator& operator=(const reverse_iterator& source)
      {  inherited::fullAssign(source); return *this; }
   reverse_iterator& operator=(reverse_iterator&& source)
      {  return (reverse_iterator&) inherited::operator=(source); }

   iterator base() const
      {  iterator result((const iterator&) (const inherited&) *this);
         ++result;
         return result;
      }
   reverse_iterator& operator++() { inherited::operator*().setToPrevious(); return *this; }
   reverse_iterator& operator+=(int add)
      {  if (add > 0) {
            while (--add >= 0)
               inherited::operator*().setToPrevious();
         }
         else if (add < 0) {
            while (++add <= 0)
               inherited::operator*().setToNext();
         };
         return *this;
      }
   reverse_iterator operator+(int add) const
      {  reverse_iterator result(*this);
         if (add > 0) {
            while (--add >= 0)
               result.inherited::operator*().setToPrevious();
         }
         else if (add < 0) {
            while (++add <= 0)
               result.inherited::operator*().setToNext();
         };
         return result;
      }
   reverse_iterator& operator--() { inherited::operator*().setToNext(); return *this; }
   reverse_iterator& operator-=(int add) { return operator+=(-add); }
   reverse_iterator operator-(int sub) { return operator+(-sub); }
   EnhancedObject& operator*() const { return inherited::operator*().elementSAt(); }
   EnhancedObject* operator->() const { return &inherited::operator*().elementSAt(); }
};

inline
VirtualCollection::reverse_iterator
VirtualCollection::rbegin() const
{  reverse_iterator result = newCursor();
   ((const PPCursor&) result)->setToLast();
   return result;
}

inline
VirtualCollection::reverse_iterator
VirtualCollection::rend() const
   {  return reverse_iterator(newCursor()); }

template <class TypeCollection, class TypeElement>
class VirtualCollection::titerator : public TypeCollection::Cursor {
  private:
   typedef titerator<TypeCollection, TypeElement> thisType;
   typedef typename TypeCollection::Cursor inherited;

  protected:
   typedef typename TypeCollection::Cursor Base;
   titerator(const Base& source) : inherited(source) {}
   titerator(const TypeCollection& support) : inherited(support) {}

  public:
   titerator(const thisType& source) = default;
   titerator(thisType&& source) : inherited(source) {}

   bool operator==(const thisType& source) const { return inherited::isEqual(source); }
   bool operator!=(const thisType& source) const { return !inherited::isEqual(source); }
   thisType& operator++() { inherited::setToNext(); return *this; }
   thisType& operator+=(int add)
      {  if (add > 0) {
            while (--add >= 0)
               inherited::setToNext();
         }
         else if (add < 0) {
            while (++add <= 0)
               inherited::setToPrevious();
         };
         return *this;
      }
   thisType operator+(int add) const
      {  thisType result(*this);
         if (add > 0) {
            while (--add >= 0)
               result.inherited::setToNext();
         }
         else if (add < 0) {
            while (++add <= 0)
               result.inherited::setToPrevious();
         };
         return result;
      }
   thisType& operator--() { inherited::setToPrevious(); return *this; }
   thisType& operator-=(int add) { return operator+=(-add); }
   thisType operator-(int sub) { return operator+(-sub); }
   TypeElement& operator*() const { return (TypeElement&) inherited::elementSAt(); }
   TypeElement* operator->() const { return (TypeElement*) &inherited::elementSAt(); }
};

template <class TypeCollection, class TypeElement>
class VirtualCollection::treverse_iterator : public TypeCollection::Cursor {
  private:
   typedef treverse_iterator<TypeCollection, TypeElement> thisType;
   typedef typename TypeCollection::Cursor inherited;

  protected:
   typedef typename TypeCollection::Cursor Base;
   treverse_iterator(const TypeCollection& support) : inherited(support) {}
   treverse_iterator(const Base& source) : inherited(source) {}

  public:
   treverse_iterator(const thisType& source) : inherited(source) {}
   treverse_iterator(thisType&& source) : inherited(source) {}

   bool operator==(const thisType& source) const { return isEqual(source); }
   bool operator!=(const thisType& source) const { return !isEqual(source); }
   thisType& operator++() { inherited::setToPrevious(); return *this; }
   thisType& operator+=(int add)
      {  if (add > 0) {
            while (--add >= 0)
               inherited::setToPrevious();
         }
         else if (add < 0) {
            while (++add <= 0)
               inherited::setToNext();
         };
         return *this;
      }
   thisType operator+(int add) const
      {  thisType result(*this);
         if (add > 0) {
            while (--add >= 0)
               result.inherited::setToPrevious();
         }
         else if (add < 0) {
            while (++add <= 0)
               result.inherited::setToNext();
         };
         return result;
      }
   thisType& operator--() { inherited::operator*().setToNext(); return *this; }
   thisType& operator-=(int add) { return operator+=(-add); }
   thisType operator-(int sub) { return operator+(-sub); }
   TypeElement& operator*() const { return inherited::operator*().elementSAt(); }
   TypeElement* operator->() const { return &inherited::operator*().elementSAt(); }
};

/**************************************/
/* Definition of TInterfaceCollection */
/**************************************/
// The element type, the cast type should be the same as the implementation TypeCollection.
//

#define DefTypeCollection TInterfaceCollection<TypeCollection, TypeElement, CastToImplementation, Cast>
#define DefTypeCursor TInterfaceCollectionCursor<TypeCollection, TypeElement, CastToImplementation, Cast>
#define DefTypeElement TypeElement
#define DefTypeCastToCollectHandler Cast::castTo
#define DefTypeCCastToCollectHandler Cast::castTo
#define DefTypeCastFromCollectHandler(object) ((TypeElement*) Cast::castFrom((typename Cast::Base*) object))
#define DefTypeCCastFromCollectHandler(object) ((const TypeElement*) Cast::castFrom((const typename Cast::Base*) object))
#define DefTypeFinal thisType::

namespace DInterfaceCollection {

#if defined(__GNUC__) && GCC_VERSION >= 40600
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Woverloaded-virtual"
#endif
template <class TypeElement, class CastToImplementation, class Cast>
class TImplementationRegistrationCast : public VirtualCollection::VirtualCastWithElement {
  private:
   typedef VirtualCollection::VirtualCastWithElement CastWithElement;
   typedef CastWithElement inherited;
   typedef TImplementationRegistrationCast<TypeElement, CastToImplementation, Cast> thisType;
   const CastWithElement* pvcweRetrieveElementFromCopy;

  protected:
   virtual EnhancedObject* _castFrom(EnhancedObject* copyObject, EnhancedObject* sourceCollectionObject) const
      {  return CastToImplementation::Base::castToCopyHandler((typename CastToImplementation::Base*)
            CastToImplementation::castTo((TypeElement*) Cast::castFrom((typename Cast::Base*) (pvcweRetrieveElementFromCopy
               ?  pvcweRetrieveElementFromCopy->castFrom(copyObject, CastToImplementation::Base::castToCopyHandler(
                     CastToImplementation::castTo((TypeElement*) Cast::castFrom((typename Cast::Base*)
                           sourceCollectionObject))))
               : copyObject))));
      }
   virtual EnhancedObject* _castTo(EnhancedObject* collectionObject, EnhancedObject* sourceCopyObject) const
      {  EnhancedObject* result = (typename Cast::Base*) Cast::castTo((TypeElement*)
            CastToImplementation::castFrom((typename CastToImplementation::Base*) CastToImplementation::Base::castFromCopyHandler(collectionObject)));
         return pvcweRetrieveElementFromCopy
            ? pvcweRetrieveElementFromCopy->castTo(result, sourceCopyObject) : result;
      }

  public:
   TImplementationRegistrationCast() : pvcweRetrieveElementFromCopy(nullptr) {}
   TImplementationRegistrationCast(const VirtualCast& retrieveElementFromCopy)
      :  pvcweRetrieveElementFromCopy((const CastWithElement*) &retrieveElementFromCopy)
      {  if (((const CastWithElement&) retrieveElementFromCopy).isValid())
            inherited::setCastWithElement();
      }
   TImplementationRegistrationCast(const thisType& source) = default;
   void setCast(const VirtualCast& retrieveElementFromCopy)
      {  pvcweRetrieveElementFromCopy = (const CastWithElement*) &retrieveElementFromCopy;
         inherited::setCastWithElement(((const CastWithElement&) retrieveElementFromCopy).isValid());
      }
};
#if defined(__GNUC__) && GCC_VERSION >= 40600
#pragma GCC diagnostic pop
#endif

template <class TypeBase, class TypeElement, class CastToImplementation, class Cast>
class TImplementationInsertionParameters : public DVirtualCollection::TCastParameters<TypeBase> {
  private:
   typedef DVirtualCollection::TCastParameters<TypeBase> inherited;
   typedef TImplementationInsertionParameters<TypeBase, TypeElement, CastToImplementation, Cast> thisType;
   TImplementationRegistrationCast<TypeElement, CastToImplementation, Cast> ircCast;
   const TypeBase& eipReference;

  public:
   TImplementationInsertionParameters(const TypeBase& parameters)
      :  inherited(parameters), eipReference(parameters)
      {  inherited::setExternCast(ircCast);
         if (parameters.hasCast())
            ircCast.setCast(parameters.getCast());
      }
   TImplementationInsertionParameters(const thisType& source) = default;
   Template4DefineCopy(TImplementationInsertionParameters, TypeBase, TypeElement, CastToImplementation, Cast)

   virtual const DVirtualCollection::VirtualKeyHandler& getKeyHandler() const override
      {  return eipReference.getKeyHandler(); }
   virtual const VirtualCast& getCast() const override { return ircCast; }
};

} // end of namespace DInterfaceCollection

template <class TypeCollection, class TypeElement, class CastToImplementation, class Cast=SimpleCast>
class TInterfaceCollectionCursor;

template <class TypeCollection, class TypeElement, class CastToImplementation, class Cast=SimpleCast>
class TInterfaceCollection : public TVirtualCollection<TypeElement, Cast> {
  private:
   typedef TVirtualCollection<TypeElement, Cast> inherited;
   typedef TInterfaceCollection<TypeCollection, TypeElement, CastToImplementation, Cast> thisType;

   TypeCollection tcImplementation;

  public:
   Template4DefineCollectionForAbstractCollect(TInterfaceCollection,
         TInterfaceCollectionCursor, TypeCollection, TypeElement, CastToImplementation, Cast)
   typedef typename inherited::ExtendedLocateParameters ExtendedLocateParameters;
   typedef typename inherited::ExtendedInsertionParameters ExtendedInsertionParameters;
   typedef typename inherited::ExtendedSuppressParameters ExtendedSuppressParameters;
   typedef typename inherited::ExtendedReplaceParameters ExtendedReplaceParameters;
   typedef typename inherited::AddMode AddMode;
   typedef typename inherited::RemoveMode RemoveMode;
   typedef typename inherited::RelativePosition RelativePosition;
   // typedef typename inherited::LocationResult LocationResult;
   typedef typename VirtualCollection::TInitialValues<TypeElement, Cast> InitialValues;
   typedef typename VirtualCollection::TInitialNewValues<TypeElement, Cast> InitialNewValues;

  private:
   static typename TypeCollection::Cursor* implementation(VirtualCollectionCursor* cursor)
      { return cursor ? &((Cursor&) *cursor).implementation() : nullptr; }
   static const typename TypeCollection::Cursor* implementation(const VirtualCollectionCursor* cursor)
      { return cursor ? &((const Cursor&) *cursor).implementation() : nullptr; }

   friend class TInterfaceCollectionCursor<TypeCollection, TypeElement, Cast>;
   
  public:
   TypeCollection& implementation() { return tcImplementation; }
   const TypeCollection& implementation() const { return tcImplementation; }

  protected:
   typedef DInterfaceCollection::TImplementationInsertionParameters<
         ExtendedReplaceParameters, TypeElement, CastToImplementation, Cast> ImplementationReplaceParameters;
   typedef DInterfaceCollection::TImplementationInsertionParameters<
         ExtendedInsertionParameters, TypeElement, CastToImplementation, Cast> ImplementationInsertionParameters;

   virtual void _fullAssign(const VirtualCollection& source, const ExtendedReplaceParameters& parameters) override
      {  inherited::_fullAssign(source, parameters);
         AssumeCondition(dynamic_cast<const thisType*>(&source));
         ImplementationReplaceParameters params(parameters);
         tcImplementation.fullAssign(((const thisType&) source).tcImplementation, params);
      }
   virtual void _add(EnhancedObject* anObject, const ExtendedInsertionParameters& parameters,
         VirtualCollectionCursor* cursor) override
      {  inherited::_add(anObject, parameters, cursor);
         ImplementationInsertionParameters params(parameters);
         tcImplementation.add(CastToImplementation::castTo((TypeElement*)
            Cast::castFrom((typename Cast::Base*) anObject)), params, implementation(cursor));
      }
   void _add(TypeElement* element, const ExtendedInsertionParameters& parameters, Cursor* cursor=nullptr)
      {  inherited::_add(element, parameters, cursor);
         ImplementationInsertionParameters params(parameters);
         tcImplementation.add(CastToImplementation::castTo(element), params, implementation(cursor));
      }
   virtual void _addAll(const VirtualCollection& source, const ExtendedInsertionParameters& parameters,
         VirtualCollectionCursor* cursor, const VirtualCollectionCursor* startSource,
         const VirtualCollectionCursor* endSource) override
      {  inherited::_addAll(source, parameters, cursor, startSource, endSource);
         ImplementationInsertionParameters params(parameters);
         if (dynamic_cast<const thisType*>(&source))
            tcImplementation.addAll(((const thisType&) source).tcImplementation, params,
               implementation(cursor), implementation(startSource), implementation(endSource));
         else
            ((VirtualCollection&) tcImplementation).addAll(source, params,
               implementation(cursor), startSource, endSource);
      }
   void _addAll(const thisType& source, const ExtendedInsertionParameters& parameters,
         Cursor* cursor=nullptr, const Cursor* startSource=nullptr, const Cursor* endSource=nullptr)
      {  inherited::_addAll(source, parameters, cursor, startSource, endSource);
         ImplementationInsertionParameters params(parameters);
         tcImplementation.addAll(source.tcImplementation, params, implementation(cursor),
               implementation(startSource), implementation(endSource));
      }
   virtual void _moveTo(VirtualCollection& destination, const ExtendedReplaceParameters& parameters,
         VirtualCollectionCursor* cursor, VirtualCollectionCursor* destinationCursor) override
      {  inherited::_moveTo(destination, parameters, cursor, destinationCursor);
         ImplementationReplaceParameters params(parameters);
         if (dynamic_cast<const thisType*>(&destination))
            tcImplementation.moveTo(((thisType&) destination).tcImplementation, params,
                  implementation(cursor), implementation(destinationCursor));
         else
            ((VirtualCollection&) tcImplementation).moveTo(destination, params,
               implementation(cursor), destinationCursor);
      }
   void _moveTo(thisType& destination, const ExtendedReplaceParameters& parameters,
         Cursor* cursor=nullptr, Cursor* destinationCursor=nullptr)
      {  inherited::_moveTo(destination, parameters, cursor, destinationCursor);
         ImplementationReplaceParameters params(parameters);
         tcImplementation.moveTo(destination.tcImplementation, params,
               implementation(cursor), implementation(destinationCursor));
      }
   virtual void _moveAllTo(VirtualCollection& destination, const ExtendedReplaceParameters& parameters,
         const VirtualCollectionCursor* startCursor, const VirtualCollectionCursor* endCursor,
         VirtualCollectionCursor* destinationCursor) override
      {  inherited::_moveAllTo(destination, parameters, startCursor, endCursor, destinationCursor);
         ImplementationReplaceParameters params(parameters);
         if (dynamic_cast<thisType*>(&destination))
            tcImplementation.moveAllTo((thisType&) destination, params, implementation(startCursor),
               implementation(endCursor), implementation(destinationCursor));
         else
            ((VirtualCollection&) tcImplementation).moveAllTo(destination, params,
               implementation(startCursor), implementation(endCursor), destinationCursor);
      }
   void _moveAllTo(thisType& destination, const ExtendedReplaceParameters& parameters,
         const Cursor* startCursor=nullptr, const Cursor* endCursor=nullptr,
         Cursor* destinationCursor=nullptr)
      {  inherited::_moveAllTo(destination, parameters, startCursor, endCursor, destinationCursor);
         ImplementationReplaceParameters params(parameters);
         tcImplementation.moveAllTo(destination, params, implementation(startCursor),
               implementation(endCursor), implementation(destinationCursor));
      }
   virtual void _swap(VirtualCollection& source) override
      {  inherited::_swap(source);
         tcImplementation.swap(((thisType&) source).tcImplementation);
      }
   virtual void _remove(const ExtendedSuppressParameters& parameters, VirtualCollectionCursor* cursor) override
      {  inherited::_remove(parameters, cursor);
         tcImplementation.remove(parameters, implementation(cursor));
      }
   void _remove(const ExtendedSuppressParameters& parameters, Cursor* cursor=nullptr)
      {  inherited::_remove(parameters, cursor);
         tcImplementation.remove(parameters, implementation(cursor));
      }
   virtual void _removeAll(const ExtendedSuppressParameters& parameters,
         const VirtualCollectionCursor* start, const VirtualCollectionCursor* end) override
      {  inherited::_removeAll(parameters, start, end);
         tcImplementation.removeAll(parameters, implementation(start), implementation(end));
      }
   void _removeAll(const ExtendedSuppressParameters& parameters,
         const Cursor* start=nullptr, const Cursor* end=nullptr)
      {  inherited::_removeAll(parameters, start, end);
         tcImplementation.removeAll(parameters, implementation(start), implementation(end));
      }
   virtual void _replace(EnhancedObject* anObject, const ExtendedReplaceParameters& parameters,
         VirtualCollectionCursor* cursor) override
      {  inherited::_replace(anObject, parameters, cursor);
         ImplementationReplaceParameters params(parameters);
         tcImplementation.replace(CastToImplementation::castTo((TypeElement*)
            Cast::castFrom((typename Cast::Base*) anObject)), params, implementation(cursor));
      }
   void _replace(TypeElement* anObject, const ExtendedReplaceParameters& parameters,
         Cursor* cursor=nullptr)
      {  inherited::_replace(anObject, parameters, cursor);
         ImplementationReplaceParameters params(parameters);
         tcImplementation.replace(CastToImplementation::castTo(anObject), params, implementation(cursor));
      }
   virtual int _queryCount(const ExtendedLocateParameters& parameters,
         const VirtualCollectionCursor* startCursor, const VirtualCollectionCursor* endCursor) const override
      {  return tcImplementation.queryCount(parameters, implementation(startCursor),
            implementation(endCursor));
      }
   int _queryCount(const ExtendedLocateParameters& parameters,
         const Cursor* startCursor=nullptr, const Cursor* endCursor=nullptr) const
      {  return tcImplementation.queryCount(parameters, implementation(startCursor),
            implementation(endCursor));
      }
   virtual EnhancedObject* _getElement(const ExtendedLocateParameters& parameters,
         const VirtualCollectionCursor* cursor) const override
      {  inherited::_getElement(parameters, cursor);
         return Cast::castTo(&CastToImplementation::castFrom(tcImplementation.getElement(parameters, implementation(cursor))));
      }
   TypeElement* _getElement(const ExtendedLocateParameters& parameters,
         const Cursor* cursor=nullptr) const
      {  inherited::_getElement(parameters, cursor);
         return &CastToImplementation::castFrom(tcImplementation.getElement(parameters, implementation(cursor)));
      }

  public:
   TInterfaceCollection() {}
   TInterfaceCollection(const InitialValues& initialValues)
      {  VirtualCollection::TInsertInitialValue<TInterfaceCollection<TypeCollection, TypeElement, CastToImplementation, Cast>, TypeElement, Cast>
            insertInitialValue(*this);
         initialValues.foreachDo(insertInitialValue);
      }
   TInterfaceCollection(const InitialNewValues& initialValues)
      {  VirtualCollection::TInsertInitialNewValue<TInterfaceCollection<TypeCollection, TypeElement, CastToImplementation, Cast>, TypeElement, Cast>
            insertInitialValue(*this);
         initialValues.foreachDo(insertInitialValue);
      }
  private:
   typedef DInterfaceCollection::TImplementationRegistrationCast<TypeElement, CastToImplementation, Cast>
      ImplementationRegistrationCast;
  public:
   TInterfaceCollection(const thisType& source, AddMode mode=VirtualCollection::AMDuplicate,
         const VirtualCast* retrieveRegistrationFromCopy=nullptr)
      : tcImplementation(source.tcImplementation, mode, retrieveRegistrationFromCopy
            ? &const_cast<const ImplementationRegistrationCast&>(
               (const ImplementationRegistrationCast&) ImplementationRegistrationCast(*retrieveRegistrationFromCopy))
            : &const_cast<const ImplementationRegistrationCast&>(
               (const ImplementationRegistrationCast&) ImplementationRegistrationCast())) {}
   Template4DefineCopy(TInterfaceCollection, TypeCollection, TypeElement, CastToImplementation, Cast)

   #include "Collection/ELMCollection.inch"

   template <class Execute>
      bool foreachDo(Execute& fun, const ExtendedLocateParameters& parameters,
         const Cursor* start=nullptr, const Cursor* end=nullptr) const
      {  VirtualCollection::TemplateForeachParameters<TypeElement, CastToImplementation> params;
         return tcImplementation.foreachDo(params, fun, parameters, implementation(start), implementation(end));
      }
   template <class Execute>
      bool foreachDo(Execute& fun) const
      {  VirtualCollection::TemplateForeachParameters<TypeElement, CastToImplementation> params;
         return tcImplementation.foreachDo(params, fun);
      }
   template <class Execute>
      bool foreachReverseDo(Execute& fun, const ExtendedLocateParameters& parameters,
         const Cursor* start=nullptr, const Cursor* end=nullptr) const
      {  VirtualCollection::TemplateForeachParameters<TypeElement, CastToImplementation> params;
         return tcImplementation.foreachReverseDo(params,
            fun, parameters, implementation(start), implementation(end));
      }
   template <class Execute>
      bool foreachReverseDo(Execute& fun) const
      {  VirtualCollection::TemplateForeachParameters<TypeElement, CastToImplementation> params;
         return tcImplementation.foreachReverseDo(params, fun);
      }
   bool foreachDo(std::function<bool (TypeElement&)> func, const ExtendedLocateParameters& parameters,
         const Cursor* start=nullptr, const Cursor* end=nullptr) const
      {  VirtualCollection::TemplateForeachParameters<TypeElement, CastToImplementation> params;
         return tcImplementation.foreachDo(params, func, parameters, start, end);
      }
   bool foreachDo(std::function<bool (TypeElement&)> func) const
      {  VirtualCollection::TemplateForeachParameters<TypeElement, CastToImplementation> params;
         return tcImplementation.foreachDo(params, func, ExtendedLocateParameters(), nullptr, nullptr);
      }
   bool foreachReverseDo(std::function<bool (TypeElement&)> func, const ExtendedLocateParameters& parameters,
         const Cursor* start=nullptr, const Cursor* end=nullptr) const
      {  VirtualCollection::TemplateForeachParameters<TypeElement, CastToImplementation> params;
         return tcImplementation.foreachReverseDo(params, func, parameters, start, end);
      }
   bool foreachReverseDo(std::function<bool (TypeElement&)> func) const
      {  VirtualCollection::TemplateForeachParameters<TypeElement, CastToImplementation> params;
         return tcImplementation.foreachReverseDo(params, func, ExtendedLocateParameters(), nullptr, nullptr);
      }
};

template <class TypeCollection, class TypeElement, class CastToImplementation, class Cast>
class TInterfaceCollectionCursor : public TVirtualCollectionCursor<TypeElement, Cast> {
  private:
   typedef TInterfaceCollectionCursor<TypeCollection, TypeElement, CastToImplementation, Cast> thisType;
   typedef TVirtualCollectionCursor<TypeElement, Cast> inherited;
   typedef typename TypeCollection::Cursor TypeCollectionCursor;

   TypeCollectionCursor tccImplementation;

  public:
   typedef typename inherited::ExtendedLocateParameters ExtendedLocateParameters;
   typedef typename inherited::Position Position;

  protected:
   TypeCollectionCursor& implementation() { return tccImplementation; }
   const TypeCollectionCursor& implementation() const { return tccImplementation; }

   static TypeCollectionCursor* implementation(VirtualCollectionCursor* cursor)
      {  return (cursor != nullptr) ? &((thisType*) cursor)->tccImplementation : nullptr ; }
   static const TypeCollectionCursor* implementation(const VirtualCollectionCursor* cursor)
      {  return (cursor != nullptr) ? &((const thisType*) cursor)->tccImplementation : nullptr ; }

   virtual bool _isPositionned(const ExtendedLocateParameters& pos,
         const VirtualCollectionCursor* cursor=nullptr) const override
      {  return tccImplementation.isPositionned(pos, implementation(cursor)); }
   virtual bool _position(const Position& pos) override
      {  return tccImplementation.position(pos); }
   virtual EnhancedObject* _getSElement() const override
      {  return Cast::castTo(CastToImplementation::castFrom(tccImplementation.getSElement())); }
   virtual bool _isEqual(const AbstractCursor& cursor) const override
      {  return tccImplementation.isEqual(((const thisType&) cursor).tccImplementation); }
   virtual void _gotoReference(const EnhancedObject& element) override
      {  return ((VirtualCollectionCursor&) tccImplementation).gotoReference(
            CastToImplementation::castTo(Cast::castFrom((typename Cast::Base&) element)));
      }

   virtual ComparisonResult _compare(const EnhancedObject& asource) const override
      {  ComparisonResult crResult = inherited::_compare(asource);
         const thisType& source = (const thisType&) inherited::castFromCopyHandler(asource);
         return (crResult == CREqual) ? tccImplementation.compare(source.tccImplementation) : crResult;
      }

  public:
   TInterfaceCollectionCursor(const TInterfaceCollection<TypeCollection, TypeElement, CastToImplementation, Cast>& support)
      : inherited(support), tccImplementation(support.implementation()) {}
   TInterfaceCollectionCursor(const thisType& source) = default;
   thisType& operator=(const thisType& source) = default;
   DefineCopy(thisType)
   DDefineAssign(thisType)
   Template4DefineCursorForAbstractCollect(TInterfaceCollection, TInterfaceCollectionCursor,
         TypeCollection, TypeElement, CastToImplementation, Cast)

#define DefCursor
   #include "Collection/ELMCollection.inch"
#undef DefCursor
};

#undef DefTypeCollection
#undef DefTypeCursor
#undef DefTypeElement
#undef DefTypeCastToCollectHandler
#undef DefTypeCCastToCollectHandler
#undef DefTypeCastFromCollectHandler
#undef DefTypeCCastFromCollectHandler
#undef DefTypeFinal

Template4InlineCollectionForAbstractCollect(TInterfaceCollection, TInterfaceCollectionCursor, TypeCollection, TypeElement, CastToImplementation, Cast)

} // end of namespace COL

#endif // COL_VirtualCollectionH

