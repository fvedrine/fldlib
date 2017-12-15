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
// Fichier     : VirtualMapCollection.h
// Description :
//   Definition of the template class TVirtualMap.
//

#ifndef COL_VirtualMapCollectionH
#define COL_VirtualMapCollectionH

#include "Collection/VirtualCollection/VirtualSortedCollection.h"

namespace COL {

template <class TypeKeyTraits>
class TVirtualMapCollection;

namespace DVirtualMapCollection {

/******************************************************/
/* Definition of the template class TMapInitialValues */
/******************************************************/

class Access {
  private:
   template <class TypeKeyTraits>
   class TInitialStoreKeyValue : public COL::ImplListElement {
     private:
      typedef COL::ImplListElement inherited;
      typedef TInitialStoreKeyValue<TypeKeyTraits> thisType;
      typename TypeKeyTraits::ControlKeyType cktKey;
      EnhancedObject* peoObject;

     public:
      TInitialStoreKeyValue(typename TypeKeyTraits::KeyType key)
         :  inherited(), cktKey(TypeKeyTraits::store(key)), peoObject(nullptr) {}
      TInitialStoreKeyValue(typename TypeKeyTraits::KeyType key, EnhancedObject& source)
         :  inherited(), cktKey(TypeKeyTraits::store(key)), peoObject(&source) {}
      TInitialStoreKeyValue(const thisType& source)
         :  inherited(source), cktKey(TypeKeyTraits::copy(source.cktKey)), peoObject(source.peoObject) {}
      DefineCopy(thisType)
      DDefineAssign(thisType)

      EnhancedObject& extractElement()
         {  EnhancedObject* result = peoObject;
            AssumeCondition(result != nullptr)
            peoObject = nullptr;
            return *result;
         }
      void setElement(EnhancedObject& object)
         {  AssumeCondition(!peoObject)
            peoObject = &object;
         }
      typename TypeKeyTraits::KeyType queryKey() const { return TypeKeyTraits::see(cktKey); }
      virtual bool isValid() const override
         {  return inherited::isValid() && (peoObject != nullptr); }
   };

   template <class Element, class TypeKeyTraits, class Cast>
   class TInitialElementValues;

  public:
   template <class Element, class TypeKeyTraits, class Cast>
   class TMapInitialValues : public COL::TImplList<TInitialStoreKeyValue<TypeKeyTraits> > {
     private:
      typedef COL::TImplList<TInitialStoreKeyValue<TypeKeyTraits> > inherited;
      typedef TMapInitialValues<Element, TypeKeyTraits, Cast> thisType;
      
     protected:
      friend class COL::TVirtualMapCollection<TypeKeyTraits>;

     public:
      TMapInitialValues() {}
      TMapInitialValues(const thisType& source) : inherited(source, true) {}
      virtual ~TMapInitialValues() { inherited::freeAll(); }
      Template3DefineCopy(TMapInitialValues, Element, TypeKeyTraits, Cast)
      DTemplate3DefineAssign(TMapInitialValues, Element, TypeKeyTraits, Cast)

      thisType& operator=(const thisType& source)
         {  inherited::assign((const thisType&) source, true); return *this; }

      TInitialElementValues<Element, TypeKeyTraits, Cast> operator<<(typename TypeKeyTraits::KeyType key)
         {  TInitialStoreKeyValue<TypeKeyTraits>* storeKeyValue = nullptr;
            inherited::add(storeKeyValue = new TInitialStoreKeyValue<TypeKeyTraits>(key));
            return TInitialElementValues<Element, TypeKeyTraits, Cast>(*this, *storeKeyValue);
         }
      TMapInitialValues<Element, TypeKeyTraits, Cast>& setFrom(typename TypeKeyTraits::KeyType key, Element& source)
         {  TInitialStoreKeyValue<TypeKeyTraits>* storeKeyValue = nullptr;
            inherited::add(storeKeyValue = new TInitialStoreKeyValue<TypeKeyTraits>(key, Cast::castTo(source)));
            return *this;
         }
   };

  private:
   template <class Element, class TypeKeyTraits, class Cast>
   class TInitialElementValues {
     private:
      typedef TInitialElementValues<Element, TypeKeyTraits, Cast> thisType;
      TMapInitialValues<Element, TypeKeyTraits, Cast>& ivReferent;
      TInitialStoreKeyValue<TypeKeyTraits>& iskvStoreKeyValue;
      
     public:
      TInitialElementValues(TMapInitialValues<Element, TypeKeyTraits, Cast>& referent, 
            TInitialStoreKeyValue<TypeKeyTraits>& storeKeyValue)
         :  ivReferent(referent), iskvStoreKeyValue(storeKeyValue) {}
      TInitialElementValues(const thisType& source) = default;
      
      TMapInitialValues<Element, TypeKeyTraits, Cast>& operator<<(Element& source)
         {  iskvStoreKeyValue.setElement(Cast::castTo(source));
            return ivReferent;
         }
   };

   /*********************************************************/
   /* Definition of the template class TMapInitialNewValues */
   /*********************************************************/

   template <class TypeKeyTraits>
   class TInitialStoreKeyNewValue : public COL::ImplListElement {
     private:
      typedef COL::ImplListElement inherited;
      typedef TInitialStoreKeyNewValue<TypeKeyTraits> thisType;
      typename TypeKeyTraits::ControlKeyType cktKey;
      PNT::PassPointer<EnhancedObject> ppeoObject;

     public:
      TInitialStoreKeyNewValue(typename TypeKeyTraits::KeyType key)
         :  inherited(), cktKey(TypeKeyTraits::store(key)), ppeoObject() {}
      TInitialStoreKeyNewValue(typename TypeKeyTraits::KeyType key, const PNT::PassPointer<EnhancedObject>& object)
         :  inherited(), cktKey(TypeKeyTraits::store(key)), ppeoObject(object) {}
      TInitialStoreKeyNewValue(typename TypeKeyTraits::KeyType key, EnhancedObject* object)
         :  inherited(), cktKey(TypeKeyTraits::store(key)), ppeoObject(object, PNT::Pointer::Init()) {}
      TInitialStoreKeyNewValue(const thisType& source)
         :  inherited(source), cktKey(TypeKeyTraits::copy(source.cktKey)), ppeoObject(source.ppeoObject) {}
      TemplateDefineCopy(TInitialStoreKeyNewValue, TypeKeyTraits)
      DTemplateDefineAssign(TInitialStoreKeyNewValue, TypeKeyTraits)
      thisType& operator=(const thisType& source)
         {  inherited::operator=(source);
            cktKey = source.cktKey;
            ppeoObject.fullAssign(source.ppeoObject);
            return *this;
         }

      EnhancedObject* extractElement()
         {  AssumeCondition(ppeoObject.isValid())
            return ppeoObject.extractElement();
         }
      void absorbElement(EnhancedObject* object)
         {  AssumeCondition(!ppeoObject.isValid())
            ppeoObject.absorbElement(object);
         }
      PNT::PassPointer<EnhancedObject>& element() { return ppeoObject; }
      typename TypeKeyTraits::KeyType queryKey() const { return TypeKeyTraits::see(cktKey); }
      virtual bool isValid() const override
         {  return inherited::isValid() && (ppeoObject.isValid()); }
   };

   template <class Element, class TypeKeyTraits, class Cast>
   class TInitialElementNewValues;

  public:
   template <class Element, class TypeKeyTraits, class Cast>
   class TMapInitialNewValues : public COL::TImplList<TInitialStoreKeyNewValue<TypeKeyTraits> > {
     private:
      typedef COL::TImplList<TInitialStoreKeyNewValue<TypeKeyTraits> > inherited;
      typedef TMapInitialNewValues<Element, TypeKeyTraits, Cast> thisType;
      
     protected:
      friend class COL::TVirtualMapCollection<TypeKeyTraits>;

     public:
      TMapInitialNewValues() {}
      TMapInitialNewValues(const thisType& source) : inherited(source, true) {}
      virtual ~TMapInitialNewValues() { inherited::freeAll(); }
      DefineCopy(thisType)
      DDefineAssign(thisType)
      thisType& operator=(const thisType& source)
         {  inherited::assign((const thisType&) source, true); return *this; }

      TInitialElementNewValues<Element, TypeKeyTraits, Cast> operator<<(typename TypeKeyTraits::KeyType key)
         {  TInitialStoreKeyNewValue<TypeKeyTraits>* storeKeyValue = nullptr;
            inherited::add(storeKeyValue = new TInitialStoreKeyNewValue<TypeKeyTraits>(key));
            return TInitialElementNewValues<Element, TypeKeyTraits, Cast>(*this, *storeKeyValue);
         }
      TMapInitialNewValues<Element, TypeKeyTraits, Cast>& setFrom(typename TypeKeyTraits::KeyType key, const PNT::PassPointer<Element>& source)
         {  TInitialStoreKeyNewValue<TypeKeyTraits>* storeKeyValue = nullptr;
            inherited::add(storeKeyValue = new TInitialStoreKeyNewValue<TypeKeyTraits>(key, PNT::PassPointer<Element>(source).extractElement()));
            return *this;
         }
      TMapInitialNewValues<Element, TypeKeyTraits, Cast>& setFrom(typename TypeKeyTraits::KeyType key, Element* source)
         {  TInitialStoreKeyNewValue<TypeKeyTraits>* storeKeyValue = nullptr;
            inherited::add(storeKeyValue = new TInitialStoreKeyNewValue<TypeKeyTraits>(key, source));
            return *this;
         }
   };

  private:
   template <class Element, class TypeKeyTraits, class Cast>
   class TInitialElementNewValues {
     private:
      typedef TInitialElementNewValues<Element, TypeKeyTraits, Cast> thisType;
      TMapInitialNewValues<Element, TypeKeyTraits, Cast>& ivReferent;
      TInitialStoreKeyNewValue<TypeKeyTraits>& iskvStoreKeyValue;
      
     public:
      TInitialElementNewValues(TMapInitialNewValues<Element, TypeKeyTraits, Cast>& referent, 
            TInitialStoreKeyNewValue<TypeKeyTraits>& storeKeyValue)
         :  ivReferent(referent), iskvStoreKeyValue(storeKeyValue) {}
      TInitialElementNewValues(const thisType& source)
         :  ivReferent(source.ivReferent), iskvStoreKeyValue(source.iskvStoreKeyValue) {}
      
      TMapInitialNewValues<Element, TypeKeyTraits, Cast>&
         operator<<(const PNT::PassPointer<Element>& source)
         {  iskvStoreKeyValue.absorbElement(Cast::castTo(PNT::PassPointer<Element>(source).extractElement()));
            return ivReferent;
         }
      TMapInitialNewValues<Element, TypeKeyTraits, Cast>& operator<<(Element* source)
         {  iskvStoreKeyValue.absorbElement(Cast::castTo(source));
            return ivReferent;
         }
   };

  public:
   template <class TypeMapCollection, class TypeElement, class TypeKeyTraits, class TypeCast>
   class TMapInsertInitialValue {
     private:
      TypeMapCollection& tmcMap;

     public:
      TMapInsertInitialValue(TypeMapCollection& map) : tmcMap(map) {}
      bool operator()(TInitialStoreKeyValue<TypeKeyTraits>& storeValue)
         {  tmcMap.add(storeValue.queryKey(), (TypeElement*) TypeCast::castFrom(
                  (typename TypeCast::Base*) &storeValue.extractElement()));
            return true;
         }
   };

   template <class TypeMapCollection, class TypeElement, class TypeKeyTraits, class TypeCast>
   class TMapInsertInitialNewValue {
     private:
      TypeMapCollection& tmcMap;

     public:
      TMapInsertInitialNewValue(TypeMapCollection& map) : tmcMap(map) {}
      bool operator()(TInitialStoreKeyNewValue<TypeKeyTraits>& storeValue)
         {  tmcMap.addNew(storeValue.queryKey(), (TypeElement*) TypeCast::castFrom(
                  (typename TypeCast::Base*) storeValue.extractElement()));
            return true;
         }
   };
}; // end of class Access

class Definitions : public DVirtualCollection::Definitions {};

} // end of namespace DVirtualMapCollection

/*************************************************/
/* Definition of the class TVirtualMapCollection */
/*************************************************/

#define DefTypeCollection TVirtualMapCollection<TypeKeyTraits>
#define DefTypeCursor TVirtualMapCollectionCursor<TypeKeyTraits>
#define DefTypeSorted
#define DefTypeKey typename TypeKeyTraits::KeyType
#define DefTypeControlKey typename TypeKeyTraits::ControlKeyType
#define DefMapInterface

template <class TypeKeyTraits>
class TVirtualMapCollectionCursor;

template <class TypeKeyTraits>
class TVirtualMapCollection : public VirtualCollection, protected DVirtualMapCollection::Access {
  public:
   typedef VirtualSortedCollection::LocationResult LocationResult;
   typedef TypeKeyTraits KeyTraits;
   typedef DVirtualMapCollection::Access::TMapInitialValues<EnhancedObject, TypeKeyTraits, SimpleCast> InitialValues;
   typedef DVirtualMapCollection::Access::TMapInitialNewValues<EnhancedObject, TypeKeyTraits, SimpleCast> InitialNewValues;

  private:
   typedef VirtualCollection inherited;
   typedef TVirtualMapCollection<TypeKeyTraits> thisType;

  public:
   typedef DVirtualCollection::TMapParameters
      <VirtualCollection::ExtendedLocateParameters, TypeKeyTraits> ExtendedMapLocateParameters;
   typedef DVirtualCollection::TMapParameters
      <VirtualCollection::ExtendedInsertionParameters, TypeKeyTraits> ExtendedMapInsertionParameters;
   typedef DVirtualCollection::TMapParameters
      <VirtualCollection::ExtendedSuppressParameters, TypeKeyTraits> ExtendedMapSuppressParameters;
   typedef DVirtualCollection::TMapParameters
      <VirtualCollection::ExtendedReplaceParameters, TypeKeyTraits> ExtendedMapReplaceParameters;
   TemplateDefineCollectionForAbstractCollect(TVirtualMapCollection, TVirtualMapCollectionCursor, TypeKeyTraits)

  protected:
   typedef const Cursor* PCCursor;
   bool normalize(PCCursor& startCursor, PCCursor& endCursor) const
      {  PCVirtualCollectionCursor pvccStartCursor = startCursor, pvccEndCursor = endCursor;
         bool result = inherited::normalize(pvccStartCursor, pvccEndCursor);
         startCursor = (PCCursor) pvccStartCursor;
         endCursor = (PCCursor) pvccEndCursor;
         return result;
      }

   friend class TVirtualMapCollectionCursor<TypeKeyTraits>;

   TVirtualMapCollection() {}
   TVirtualMapCollection(const TVirtualMapCollection<TypeKeyTraits>& source, AddMode dupMode=AMNoDuplicate)
      :  VirtualCollection(source, dupMode) {}

   void paddAll(const thisType& source, const ExtendedInsertionParameters& parameters,
         Cursor* cursor, const Cursor* startSource, const Cursor* endSource);
   void pmoveTo(thisType& destination, const ExtendedReplaceParameters& parameters,
         Cursor* cursor, Cursor* destinationCursor);
   void pmoveAllTo(thisType& destination, const ExtendedReplaceParameters& parameters,
         const Cursor* startCursor, const Cursor* endCursor, Cursor* destinationCursor)
      {  return inherited::pmoveAllTo(destination, parameters, startCursor, endCursor, destinationCursor); }

   virtual LocationResult _locateKey(typename TypeKeyTraits::KeyType key, const ExtendedLocateParameters& parameters,
         Cursor* cursor=nullptr, const Cursor* start=nullptr, const Cursor* end=nullptr) const
      {  AssumeUncalled return LocationResult(); }
   virtual typename TypeKeyTraits::ControlKeyType _queryKey(const ExtendedLocateParameters& parameters,
         const VirtualCollectionCursor* cursor=nullptr) const
      {  AssumeUncalled typename TypeKeyTraits::ControlKeyType* result = nullptr; return *result; }

  public:
   TemplateDefineCopy(TVirtualMapCollection, TypeKeyTraits)

#define DefJustDeclare
#define DefExcludeElement
   #include "Collection/ELMCollection.inch"
   #include "Collection/ELMMapCollection.inch"
#undef DefExcludeElement
#undef DefJustDeclare

   class VirtualExecute {
     public:
      virtual ~VirtualExecute() {}
      virtual bool operator()(typename TypeKeyTraits::KeyType key, EnhancedObject& source) = 0;
   };

   template <class Execute, class TemplateParameters>
      bool foreachDo(TemplateParameters traits, Execute& fun, const ExtendedLocateParameters& parameters,
         const Cursor* start=nullptr, const Cursor* end=nullptr) const;
   template <class Execute, class TemplateParameters>
      bool foreachSDo(TemplateParameters traits, Execute& fun, const ExtendedLocateParameters& parameters,
         const typename TemplateParameters::TypeCursor* start=nullptr,
         const typename TemplateParameters::TypeCursor* end=nullptr) const;
   template <class Execute, class TemplateParameters>
      bool foreachDo(TemplateParameters traits, Execute& fun) const
      {  return foreachDo(traits, fun, ExtendedLocateParameters()); }
   template <class Execute, class TemplateParameters>
      bool foreachReverseDo(TemplateParameters traits, Execute& fun, const ExtendedLocateParameters& parameters,
         const Cursor* start=nullptr, const Cursor* end=nullptr) const;
   template <class Execute, class TemplateParameters>
      bool foreachSReverseDo(TemplateParameters traits, Execute& fun, const ExtendedLocateParameters& parameters,
         const typename TemplateParameters::TypeCursor* start=nullptr,
         const typename TemplateParameters::TypeCursor* end=nullptr) const;
   template <class Execute, class TemplateParameters>
      bool foreachReverseDo(TemplateParameters traits, Execute& fun) const
      { return foreachReverseDo(traits, fun, ExtendedLocateParameters()); }
};

/* Definition of the class TVirtualMapCollectionCursor */

template <class TypeKeyTraits>
class TVirtualMapCollectionCursor : public VirtualCollectionCursor {
  protected:
   TVirtualMapCollectionCursor(const TVirtualMapCollectionCursor<TypeKeyTraits>& source)
      :  VirtualCollectionCursor(source) {}
   TVirtualMapCollectionCursor(const TVirtualMapCollection<TypeKeyTraits>& support)
      :  VirtualCollectionCursor(support) {}

   virtual typename TypeKeyTraits::ControlKeyType _queryKey() const
      {  AssumeUncalled typename TypeKeyTraits::ControlKeyType* result = nullptr; return *result; }

  public:
   TemplateDefineCopy(TVirtualMapCollectionCursor, TypeKeyTraits)
   TemplateDefineCursorForAbstractCollect(TVirtualMapCollection, TVirtualMapCollectionCursor, TypeKeyTraits)

#define DefCursor
   #include "Collection/ELMCollection.inch"
#undef DefCursor

};

TemplateInlineCollectionForAbstractCollect(TVirtualMapCollection, TVirtualMapCollectionCursor, TypeKeyTraits)

#define DefTemplate template <typename TypeKeyTraits>
#define DefJustInline
#define DefExcludeElement
   #include "Collection/ELMCollection.inch"
   #include "Collection/ELMMapCollection.inch"
#undef DefExcludeElement
#undef DefJustInline
#undef DefTemplate

#undef DefTypeCollection
#undef DefTypeCursor
#undef DefTypeSorted
#undef DefTypeKey
#undef DefTypeControlKey
#undef DefMapInterface

/***************************************/
/* Definition of the class TVirtualMap */
/***************************************/

#define DefTypeCollection TVirtualMap<Element, TypeKeyTraits, Cast>
#define DefTypeCursor TVirtualMapCursor<Element, TypeKeyTraits, Cast>
#define DefTypeSorted
#define DefTypeKey typename TypeKeyTraits::KeyType
#define DefTypeControlKey typename TypeKeyTraits::ControlKeyType
#define DefMapInterface
#define DefTypeElement Element
#define DefTypeCastToCollectHandler Cast::castTo
#define DefTypeCCastToCollectHandler Cast::castTo
#define DefTypeCastFromCollectHandler (Element*) Cast::castFrom
#define DefTypeCCastFromCollectHandler (const Element*) Cast::castFrom

template <class Element, class TypeKeyTraits, class Cast>
class TVirtualMapCursor;

template <class Element, class TypeKeyTraits, class Cast>
class TVirtualMap : public TVirtualMapCollection<TypeKeyTraits>, public DVirtualMapCollection::Definitions {
  private:
   typedef TVirtualMapCollection<TypeKeyTraits> inherited;
   typedef TVirtualMap<Element, TypeKeyTraits, Cast> thisType;

  public:
   typedef TypeKeyTraits KeyTraits;
   typedef DVirtualMapCollection::Access::TMapInitialValues<Element, TypeKeyTraits, Cast> InitialValues;
   typedef DVirtualMapCollection::Access::TMapInitialNewValues<Element, TypeKeyTraits, Cast> InitialNewValues;
   typedef DVirtualMapCollection::Access::TMapInsertInitialValue<thisType, Element, TypeKeyTraits, Cast> InsertInitialValue;
   typedef DVirtualMapCollection::Access::TMapInsertInitialNewValue<thisType, Element, TypeKeyTraits, Cast> InsertInitialNewValue;
   typedef typename inherited::ExtendedLocateParameters ExtendedLocateParameters;
   typedef typename inherited::ExtendedInsertionParameters ExtendedInsertionParameters;
   typedef typename inherited::ExtendedSuppressParameters ExtendedSuppressParameters;
   typedef typename inherited::ExtendedReplaceParameters ExtendedReplaceParameters;
   typedef typename inherited::LocationResult LocationResult;

  public:
   Template3DefineCollectionForAbstractCollect(TVirtualMap, TVirtualMapCursor, Element, TypeKeyTraits, Cast)

  protected:
   typedef const Cursor* PCCursor;
   bool normalize(PCCursor& startCursor, PCCursor& endCursor) const
      {  VirtualCollection::PCVirtualCollectionCursor pvccStartCursor = startCursor, pvccEndCursor = endCursor;
         bool result = inherited::normalize(pvccStartCursor, pvccEndCursor);
         startCursor = (PCCursor) pvccStartCursor;
         endCursor = (PCCursor) pvccEndCursor;
         return result;
      }

   friend class TVirtualMapCursor<Element, TypeKeyTraits, Cast>;

   TVirtualMap() {}
   TVirtualMap(const thisType& source, VirtualCollection::AddMode dupMode=AMNoDuplicate) : inherited(source, dupMode) {}

  public:
   Template3DefineCopy(TVirtualMap, Element, TypeKeyTraits, Cast)

#define DefJustDeclare
#define DefExcludeElement
   #include "Collection/ELMCollection.inch"
   #include "Collection/ELMMapCollection.inch"
#undef DefExcludeElement
#undef DefJustDeclare

   class VirtualExecute {
     public:
      virtual ~VirtualExecute() {}
      virtual bool operator()(typename TypeKeyTraits::KeyType key, Element& source) = 0;
   };

   bool vforeachDo(VirtualExecute& fun, const ExtendedLocateParameters& parameters,
         const Cursor* start=nullptr, const Cursor* end=nullptr) const
      {  return inherited::foreachDo(VirtualCollection::TemplateForeachParameters<Element, Cast>(),
            fun, parameters, start, end);
      }
   bool foreachDo(std::function<bool (Element&)> func, const ExtendedLocateParameters& parameters,
         const Cursor* start=nullptr, const Cursor* end=nullptr) const
      {  return inherited::foreachDo(VirtualCollection::TemplateForeachParameters<Element, Cast>(),
            func, parameters, start, end);
      }
   bool vforeachDo(VirtualExecute& fun) const
      {  return inherited::foreachDo(VirtualCollection::TemplateForeachParameters<Element, Cast>(),
            fun, ExtendedLocateParameters());
      }
   bool foreachDo(std::function<bool (Element&)> func) const
      {  return inherited::foreachDo(VirtualCollection::TemplateForeachParameters<Element, Cast>(),
            func, ExtendedLocateParameters());
      }

   template <class Execute>
      bool foreachDo(Execute& fun, const ExtendedLocateParameters& parameters,
         const Cursor* start=nullptr, const Cursor* end=nullptr) const
      {  return inherited::foreachDo(VirtualCollection::TemplateForeachParameters<Element, Cast>(),
            fun, parameters, start, end);
      }
   template <class Execute>
      bool foreachDo(Execute& fun) const
      {  return inherited::foreachDo(VirtualCollection::TemplateForeachParameters<Element, Cast>(),
            fun, ExtendedLocateParameters());
      }

   bool vforeachReverseDo(VirtualExecute& fun, const ExtendedLocateParameters& parameters,
         const Cursor* start=nullptr, const Cursor* end=nullptr) const
      {  return inherited::foreachReverseDo(VirtualCollection::TemplateForeachParameters<Element, Cast>(),
            fun, parameters, start, end);
      }
   bool vforeachReverseDo(VirtualExecute& fun) const
      {  return inherited::foreachReverseDo(VirtualCollection::TemplateForeachParameters<Element, Cast>(),
            fun, ExtendedLocateParameters());
      }
   bool foreachReverseDo(std::function<bool (Element&)> func, const ExtendedLocateParameters& parameters,
         const Cursor* start=nullptr, const Cursor* end=nullptr) const
      {  return inherited::foreachReverseDo(VirtualCollection::TemplateForeachParameters<Element, Cast>(),
            func, parameters, start, end);
      }
   bool foreachReverseDo(std::function<bool (Element&)> func) const
      {  return inherited::foreachReverseDo(VirtualCollection::TemplateForeachParameters<Element, Cast>(),
            func, ExtendedLocateParameters());
      }
   template <class Execute>
      bool foreachReverseDo(Execute& fun, const ExtendedLocateParameters& parameters,
         const Cursor* start=nullptr, const Cursor* end=nullptr) const
      {  return inherited::foreachReverseDo(VirtualCollection::TemplateForeachParameters<Element, Cast>(),
            fun, parameters, start, end);
      }
   template <class Execute>
      bool foreachReverseDo(Execute& fun) const
      {  return inherited::foreachReverseDo(VirtualCollection::TemplateForeachParameters<Element, Cast>(),
            fun, ExtendedLocateParameters());
      }
};

/* Definition of the class TVirtualMapCursor */

template <class Element, class TypeKeyTraits, class Cast>
class TVirtualMapCursor : public TVirtualMapCollectionCursor<TypeKeyTraits> {
  private:
   typedef TVirtualMapCursor<Element, TypeKeyTraits, Cast> thisType;
   typedef TVirtualMapCollectionCursor<TypeKeyTraits> inherited;

  public:
   typedef typename inherited::Position Position;
   typedef typename inherited::ExtendedLocateParameters ExtendedLocateParameters;
   typedef typename inherited::ExtendedInsertionParameters ExtendedInsertionParameters;
   typedef typename inherited::ExtendedSuppressParameters ExtendedSuppressParameters;
   typedef typename inherited::ExtendedReplaceParameters ExtendedReplaceParameters;
   typedef typename inherited::LocationResult LocationResult;

  protected:
   TVirtualMapCursor(const thisType& source) : inherited(source) {}
   TVirtualMapCursor(const TVirtualMap<Element, TypeKeyTraits, Cast>& support)
      :  inherited(support) {}

  public:
   Template3DefineCopy(TVirtualMapCursor, Element, TypeKeyTraits, Cast)
   Template3DefineCursorForAbstractCollect(TVirtualMap, TVirtualMapCursor, Element, TypeKeyTraits, Cast)

#define DefTypeFinal inherited::
#define DefCursor
   #include "Collection/ELMCollection.inch"
#undef DefCursor
#undef DefTypeFinal

};

Template3InlineCollectionForAbstractCollect(TVirtualMap, TVirtualMapCursor, Element, TypeKeyTraits, Cast)

#define DefTemplate template <class Element, class TypeKeyTraits, class Cast>
#define DefJustInline
#define DefExcludeElement
   #include "Collection/ELMCollection.inch"
   #include "Collection/ELMMapCollection.inch"
#undef DefExcludeElement
#undef DefJustInline
#undef DefTemplate

#undef DefTypeCollection
#undef DefTypeCursor
#undef DefTypeSorted
#undef DefTypeKey
#undef DefTypeControlKey
#undef DefMapInterface
#undef DefTypeElement
#undef DefTypeCastToCollectHandler
#undef DefTypeCCastToCollectHandler
#undef DefTypeCastFromCollectHandler
#undef DefTypeCCastFromCollectHandler

/***************************************************/
/* Definition of the class TInterfaceMapCollection */
/***************************************************/
// The type of element, key, cast should be the same than the ones in the implementation TypeCollection.
//

#define DefTypeCollection TInterfaceMapCollection<TypeCollection, Element, TypeKeyTraits, CastToImplementation, Cast>
#define DefTypeCursor TInterfaceMapCollectionCursor<TypeCollection, Element, TypeKeyTraits, CastToImplementation, Cast>
#define DefTypeSorted
#define DefTypeKey typename TypeKeyTraits::KeyType
#define DefTypeControlKey typename TypeKeyTraits::ControlKeyType
#define DefMapInterface
#define DefTypeElement Element
#define DefTypeCastToCollectHandler Cast::castTo
#define DefTypeCCastToCollectHandler Cast::castTo
#define DefTypeCastFromCollectHandler (Element*) Cast::castFrom
#define DefTypeCCastFromCollectHandler (const Element*) Cast::castFrom
#define DefTypeFinal thisType::

template <class TypeCollection, class Element, class TypeKeyTraits, class CastToImplementation, class Cast>
class TInterfaceMapCollectionCursor;

template <class TypeCollection, class Element, class TypeKeyTraits, class CastToImplementation, class Cast>
class TInterfaceMapCollection : public TVirtualMap<Element, TypeKeyTraits, Cast> {
  private:
   typedef TInterfaceMapCollection<TypeCollection, Element, TypeKeyTraits, CastToImplementation, Cast> thisType;
   typedef TVirtualMap<Element, TypeKeyTraits, Cast> inherited;

   TypeCollection tcImplementation;

  public:
   Template5DefineCollectionForAbstractCollect(TInterfaceMapCollection,
         TInterfaceMapCollectionCursor, TypeCollection, Element, TypeKeyTraits, CastToImplementation, Cast)
   typedef typename inherited::ExtendedLocateParameters ExtendedLocateParameters;
   typedef typename inherited::ExtendedInsertionParameters ExtendedInsertionParameters;
   typedef typename inherited::ExtendedSuppressParameters ExtendedSuppressParameters;
   typedef typename inherited::ExtendedReplaceParameters ExtendedReplaceParameters;
   typedef typename inherited::AddMode AddMode;
   typedef typename inherited::RemoveMode RemoveMode;
   typedef typename inherited::RelativePosition RelativePosition;
   typedef typename inherited::LocationResult LocationResult;
   typedef typename inherited::InitialValues InitialValues;
   typedef typename inherited::InitialNewValues InitialNewValues;

  private:
   static typename TypeCollection::Cursor* implementation(VirtualCollectionCursor* cursor)
      {  return cursor ? &((Cursor&) *cursor).implementation() : nullptr; }
   static const typename TypeCollection::Cursor* implementation(const VirtualCollectionCursor* cursor)
      {  return cursor ? &((const Cursor&) *cursor).implementation() : nullptr; }

   friend class TInterfaceMapCollectionCursor<TypeCollection, Element, TypeKeyTraits, CastToImplementation, Cast>;
   TypeCollection& implementation() { return tcImplementation; }
   const TypeCollection& implementation() const { return tcImplementation; }

  protected:
   typedef DInterfaceCollection::TImplementationInsertionParameters<
      ExtendedReplaceParameters, Element, CastToImplementation, Cast> ImplementationReplaceParameters;
   typedef DInterfaceCollection::TImplementationInsertionParameters<
      ExtendedInsertionParameters, Element, CastToImplementation, Cast> ImplementationInsertionParameters;

   virtual void _fullAssign(const VirtualCollection& source, const ExtendedReplaceParameters& parameters)
      {  inherited::_fullAssign(source, parameters);
         AssumeCondition(dynamic_cast<const thisType*>(&source));
         ImplementationReplaceParameters params(parameters);
         tcImplementation.fullAssign(((const thisType&) source).tcImplementation, params);
      }
   virtual void _add(EnhancedObject* anObject, const ExtendedInsertionParameters& parameters,
         VirtualCollectionCursor* cursor)
      {  inherited::_add(anObject, parameters, cursor);
         ImplementationInsertionParameters params(parameters);
         tcImplementation.add(CastToImplementation::castFrom(anObject), params, implementation(cursor));
      }
   void _add(Element* anObject, const ExtendedInsertionParameters& parameters, Cursor* cursor=nullptr)
      {  inherited::_add(Cast::castTo(anObject), parameters, cursor);
         ImplementationInsertionParameters params(parameters);
         tcImplementation.add(anObject, params, implementation(cursor));
      }
   virtual void _addAll(const VirtualCollection& source, const ExtendedInsertionParameters& parameters,
         VirtualCollectionCursor* cursor, const VirtualCollectionCursor* startSource,
         const VirtualCollectionCursor* endSource)
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
         VirtualCollectionCursor* cursor, VirtualCollectionCursor* destinationCursor)
      {  inherited::_moveTo(destination, parameters, cursor, destinationCursor);
         if (dynamic_cast<const thisType*>(&destination))
            tcImplementation.moveTo(((thisType&) destination).tcImplementation, parameters,
                  implementation(cursor), implementation(destinationCursor));
         else
            ((VirtualCollection&) tcImplementation).moveTo(destination, parameters,
               implementation(cursor), destinationCursor);
      }
   void _moveTo(thisType& destination, const ExtendedReplaceParameters& parameters,
         Cursor* cursor=nullptr, Cursor* destinationCursor=nullptr)
      {  inherited::_moveTo(destination, parameters, cursor, destinationCursor);
         tcImplementation.moveTo(destination.tcImplementation, parameters,
               implementation(cursor), implementation(destinationCursor));
      }
   virtual void _moveAllTo(VirtualCollection& destination, const ExtendedReplaceParameters& parameters,
         const VirtualCollectionCursor* startCursor, const VirtualCollectionCursor* endCursor,
         VirtualCollectionCursor* destinationCursor)
      {  inherited::_moveAllTo(destination, parameters, startCursor, endCursor, destinationCursor);
         if (dynamic_cast<thisType*>(&destination))
            tcImplementation.moveAllTo((thisType&) destination), parameters, implementation(startCursor),
               implementation(endCursor), implementation(destinationCursor);
         else
            ((VirtualCollection&) tcImplementation).moveAllTo(destination, parameters,
               implementation(startCursor), implementation(endCursor), destinationCursor);
      }
   void _moveAllTo(thisType& destination, const ExtendedReplaceParameters& parameters,
         const Cursor* startCursor=nullptr, const Cursor* endCursor=nullptr,
         Cursor* destinationCursor=nullptr)
      {  inherited::_moveAllTo(destination, parameters, startCursor, endCursor, destinationCursor);
         tcImplementation.moveAllTo(destination, parameters, implementation(startCursor),
               implementation(endCursor), implementation(destinationCursor));
      }
   virtual void _swap(VirtualCollection& source)
      {  AssumeCondition(dynamic_cast<const thisType*>(&source))
         inherited::_swap(source);
         tcImplementation.swap(((const thisType&) source).tcImplementation);
      }
   virtual void _remove(const ExtendedSuppressParameters& parameters, VirtualCollectionCursor* cursor)
      {  inherited::_remove(parameters, cursor);
         tcImplementation.remove(parameters, implementation(cursor));
      }
   void _remove(const ExtendedSuppressParameters& parameters, Cursor* cursor=nullptr)
      {  inherited::_remove(parameters, cursor);
         tcImplementation.remove(parameters, implementation(cursor));
      }
   virtual void _removeAll(const ExtendedSuppressParameters& parameters,
         const VirtualCollectionCursor* start, const VirtualCollectionCursor* end)
      {  inherited::_removeAll(parameters, start, end);
         tcImplementation.removeAll(parameters, implementation(start), implementation(end));
      }
   void _removeAll(const ExtendedSuppressParameters& parameters,
         const Cursor* start=nullptr, const Cursor* end=nullptr)
      {  inherited::_removeAll(parameters, start, end);
         tcImplementation.removeAll(parameters, implementation(start), implementation(end));
      }
   virtual void _replace(EnhancedObject* anObject, const ExtendedReplaceParameters& parameters,
         VirtualCollectionCursor* cursor)
      {  inherited::_replace(anObject, parameters, cursor);
         ImplementationReplaceParameters params(parameters);
         tcImplementation.replace(CastToImplementation::castFrom(anObject),
               params, implementation(cursor));
      }
   void _replace(Element* anObject, const ExtendedReplaceParameters& parameters,
         Cursor* cursor=nullptr)
      {  inherited::_replace(Cast::castTo(anObject), parameters, cursor);
         ImplementationReplaceParameters params(parameters);
         tcImplementation.replace(anObject, params, implementation(cursor));
      }
   virtual int _queryCount(const ExtendedLocateParameters& parameters,
      const VirtualCollectionCursor* startCursor, const VirtualCollectionCursor* endCursor) const
      {  return implementation.queryCount(parameters, implementation(startCursor),
            implementation(endCursor));
      }
   int _queryCount(const ExtendedLocateParameters& parameters,
      const Cursor* startCursor=nullptr, const Cursor* endCursor=nullptr) const
      {  return tcImplementation.queryCount(parameters, implementation(startCursor),
            implementation(endCursor));
      }
   virtual EnhancedObject* _getElement(const ExtendedLocateParameters& parameters,
         const VirtualCollectionCursor* cursor) const
      {  inherited::_getElement(parameters, cursor);
         return CastToImplementation::castTo(tcImplementation.getElement(parameters, implementation(cursor)));
      }
   Element* _getElement(const ExtendedLocateParameters& parameters,
         const Cursor* cursor=nullptr) const
      {  inherited::_getElement(parameters, cursor);
         return tcImplementation.getElement(parameters, implementation(cursor));
      }
   virtual typename inherited::LocationResult _locateKey(typename TypeKeyTraits::KeyType key, const ExtendedLocateParameters& parameters,
         typename inherited::Cursor* cursor, const typename inherited::Cursor* start,
         const typename inherited::Cursor* end) const
      {  return tcImplementation.locateKey(key, parameters, implementation(cursor),
            implementation(start), implementation(end));
      }
   LocationResult _locateKey(typename TypeKeyTraits::KeyType key, const ExtendedLocateParameters& parameters,
         Cursor* cursor=nullptr, const Cursor* start=nullptr, const Cursor* end=nullptr) const
      {  return tcImplementation.locateKey(key, parameters, implementation(cursor),
            implementation(start), implementation(end));
      }
   virtual typename TypeKeyTraits::ControlKeyType _queryKey(const ExtendedLocateParameters& parameters,
         const VirtualCollectionCursor* cursor=nullptr) const
      {  return TypeKeyTraits::store(tcImplementation.queryKey(parameters, implementation(cursor))); }

  public:
   TInterfaceMapCollection() {}
   TInterfaceMapCollection(const InitialValues& initialValues)
      {  typename inherited::InsertInitialValue insertInitialValue(*this);
         initialValues.foreachDo(insertInitialValue);
      }
   TInterfaceMapCollection(const InitialNewValues& initialValues)
      {  typename inherited::InsertInitialNewValue insertInitialValue(*this);
         initialValues.foreachDo(insertInitialValue);
      }
   TInterfaceMapCollection(const thisType& source, AddMode dupMode=VirtualCollection::AMDuplicate,
         VirtualCast* retrieveRegistrationFromCopy=nullptr)
      :  tcImplementation(source.tcImplementation, dupMode, retrieveRegistrationFromCopy
            ? &DInterfaceCollection::TImplementationRegistrationCast<Element, CastToImplementation, Cast>(*retrieveRegistrationFromCopy)
            : &DInterfaceCollection::TImplementationRegistrationCast<Element, CastToImplementation, Cast>()) {}
   DefineCopy(thisType)

   #include "Collection/ELMCollection.inch"
#define DefExcludeElement
   #include "Collection/ELMMapCollection.inch"
#undef DefExcludeElement

   template <class Execute, class TemplateParameters>
      bool foreachDo(TemplateParameters traits, Execute& fun, const ExtendedLocateParameters& parameters,
         const VirtualCollectionCursor* start=nullptr, const VirtualCollectionCursor* end=nullptr) const
      {  return tcImplementation.foreachDo(traits, fun, parameters, implementation(start), implementation(end)); }
   template <class Execute, class TemplateParameters>
      bool foreachDo(TemplateParameters traits, Execute& fun) const
      {  return tcImplementation.foreachDo(traits, fun); }
   template <class Execute, class TemplateParameters>
      bool foreachReverseDo(TemplateParameters traits, Execute& fun, const ExtendedLocateParameters& parameters,
         const VirtualCollectionCursor* start=nullptr, const VirtualCollectionCursor* end=nullptr) const
      {  return tcImplementation.foreachReverseDo(traits, fun, parameters, implementation(start), implementation(end)); }
   template <class Execute, class TemplateParameters>
      bool foreachReverseDo(TemplateParameters traits, Execute& fun) const
      {  return tcImplementation.foreachReverseDo(traits, fun); }
};

/* Definition of the class TInterfaceMapCollectionCursor */

template <class TypeCollection, class Element, class TypeKeyTraits, class CastToImplementation, class Cast>
class TInterfaceMapCollectionCursor : public TVirtualMapCursor<Element, TypeKeyTraits, Cast> {
  private:
   typedef TInterfaceMapCollectionCursor<TypeCollection, Element, TypeKeyTraits, CastToImplementation, Cast> thisType;
   typedef TVirtualMapCursor<Element, TypeKeyTraits, Cast> inherited;
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
         const VirtualCollectionCursor* cursor=nullptr) const
      {  return tccImplementation.isPositionned(pos, implementation(cursor)); }
   virtual bool _position(const Position& pos)
      {  return tccImplementation.position(pos); }
   virtual EnhancedObject* _getSElement() const
      {  return CastToImplementation::castFrom(tccImplementation.getSElement()); }
   virtual bool _isEqual(const AbstractCursor& cursor) const
      {  return tccImplementation.isEqual((const thisType&) cursor); }
   virtual void _gotoReference(const EnhancedObject& element)
      {  return ((VirtualCollectionCursor&) tccImplementation).gotoReference(element); }
   virtual typename TypeKeyTraits::ControlKeyType _queryKey() const
      {  return TypeKeyTraits::store(tccImplementation.queryKey()); }


  public:
   TInterfaceMapCollectionCursor(const TInterfaceMapCollection<TypeCollection, Element, TypeKeyTraits, CastToImplementation, Cast>& support)
      : inherited(support), tccImplementation(support.implementation()) {}
   TInterfaceMapCollectionCursor(const thisType& source) : inherited(source), tccImplementation(source) {}
   DefineCopy(thisType)
   Template5DefineCursorForAbstractCollect(TInterfaceMapCollection, TInterfaceMapCollectionCursor,
         TypeCollection, Element, TypeKeyTraits, CastToImplementation, Cast)

#define DefCursor
   #include "Collection/ELMCollection.inch"
#undef DefCursor
};

#undef DefTypeCollection
#undef DefTypeCursor
#undef DefTypeSorted
#undef DefTypeKey
#undef DefTypeControlKey
#undef DefMapInterface
#undef DefTypeElement
#undef DefTypeCastToCollectHandler
#undef DefTypeCCastToCollectHandler
#undef DefTypeCastFromCollectHandler
#undef DefTypeCCastFromCollectHandler
#undef DefTypeFinal

Template5InlineCollectionForAbstractCollect(TInterfaceMapCollection, TInterfaceMapCollectionCursor, TypeCollection, Element, Key, CastToImplementation, Cast)

/*******************************************/
/* Définition du patron TMapCastCollection */
/*******************************************/

template <class TypeBase, class TypeElement, class TypeKeyTraits, class Cast>
class TMapCastCollectionCursor;

#define DefTypeCollection TMapCastCollection<TypeBase, TypeElement, TypeKeyTraits, Cast>
#define DefTypeCursor TMapCastCollectionCursor<TypeBase, TypeElement, TypeKeyTraits, Cast>
#define DefTypeInheritedCollection TypeBase
#define DefTypeSorted
#define DefTypeKey typename TypeKeyTraits::KeyType
#define DefTypeControlKey typename TypeKeyTraits::ControlKeyType
#define DefTypeElement TypeElement
#define DefMapInterface
#define DefTypeCastToCollectHandler Cast::castTo
#define DefTypeCCastToCollectHandler Cast::castTo
#define DefTypeCastFromCollectHandler (TypeElement*) Cast::castFrom
#define DefTypeCCastFromCollectHandler (const TypeElement*) Cast::castFrom

template <class TypeBase, class TypeElement, class TypeKeyTraits, class Cast>
class TMapCastCollection : public TypeBase {
  private:
   typedef TMapCastCollection<TypeBase, TypeElement, TypeKeyTraits, Cast> thisType;
   typedef TypeBase inherited;
   
  protected:
   virtual ComparisonResult _compare(const EnhancedObject& asource) const { return inherited::_compare(asource); }
   
  public:
   typedef typename inherited::template TMapInitialValues<TypeElement, TypeKeyTraits, Cast> InitialValues;
   typedef typename inherited::template TMapInitialNewValues<TypeElement, TypeKeyTraits, Cast> InitialNewValues;
   typedef typename inherited::template TMapInsertInitialValue<TMapCastCollection<TypeBase, TypeElement, TypeKeyTraits, Cast>, TypeElement, TypeKeyTraits, Cast> InsertInitialValue;
   typedef typename inherited::template TMapInsertInitialNewValue<TMapCastCollection<TypeBase, TypeElement, TypeKeyTraits, Cast>, TypeElement, TypeKeyTraits, Cast> InsertInitialNewValue;
   typedef typename TypeBase::AddMode AddMode;
   typedef typename TypeBase::RemoveMode RemoveMode;
   typedef typename TypeBase::RelativePosition RelativePosition;
   typedef typename TypeBase::ExtendedLocateParameters ExtendedLocateParameters;
   typedef typename TypeBase::ExtendedInsertionParameters ExtendedInsertionParameters;
   typedef typename TypeBase::ExtendedSuppressParameters ExtendedSuppressParameters;
   typedef typename TypeBase::ExtendedReplaceParameters ExtendedReplaceParameters;
   typedef typename TypeBase::LocationResult LocationResult;

   TMapCastCollection() {}
   TMapCastCollection(const InitialValues& initialValues)
      {  InsertInitialValue insertInitialValue(*this);
         initialValues.foreachDo(insertInitialValue);
      }
   TMapCastCollection(const InitialNewValues& initialValues)
      {  InsertInitialNewValue insertInitialValue(*this);
         initialValues.foreachDo(insertInitialValue);
      }
   TMapCastCollection(const TMapCastCollection<TypeBase, TypeElement, TypeKeyTraits, Cast>& source,
         VirtualCollection::AddMode dupMode = VirtualCollection::AMNoDuplicate)
      :  TypeBase(source, dupMode) {}
   DefineCopy(thisType)
   Template4DefineCollectionForAbstractCollect(TMapCastCollection, TMapCastCollectionCursor, TypeBase, TypeElement, TypeKeyTraits, Cast)

   #define DefTypeFinal TypeBase::
   #include "Collection/ELMCollection.inch"
#define DefExcludeElement
   #include "Collection/ELMMapCollection.inch"
#undef DefExcludeElement
   #undef DefTypeFinal
};

template <class TypeBase, class TypeElement, class TypeKeyTraits, class Cast>
class TMapCastCollectionCursor : public TypeBase::Cursor {
  private:
   typedef TMapCastCollectionCursor<TypeBase, TypeElement, TypeKeyTraits, Cast> thisType;
   typedef TMapCastCollection<TypeBase, TypeElement, TypeKeyTraits, Cast> SupportType;
   typedef typename TypeBase::Cursor inherited;

  protected:
   virtual ComparisonResult _compare(const EnhancedObject& asource) const { return inherited::_compare(asource); }
   
   virtual EnhancedObject* _getSElement() const { return inherited::_getSElement(); }
   virtual typename TypeKeyTraits::ControlKeyType _queryKey() const { return inherited::_queryKey(); }

  public:
   typedef typename TypeBase::ExtendedLocateParameters ExtendedLocateParameters;
   TMapCastCollectionCursor(const SupportType& support) : inherited(support) {}
   TMapCastCollectionCursor(const thisType& source) : inherited(source) {}
   DefineCopy(thisType)
   Template4DefineCursorForAbstractCollect(TMapCastCollection, TMapCastCollectionCursor, TypeBase, TypeElement, TypeKeyTraits, Cast)

#define DefCursor
   #include "Collection/ELMCollection.inch"
#undef DefCursor
};

#undef DefTypeCollection
#undef DefTypeCursor
#undef DefTypeInheritedCollection
#undef DefTypeSorted
#undef DefTypeKey
#undef DefTypeControlKey
#undef DefMapInterface
#undef DefTypeElement
#undef DefTypeCastToCollectHandler
#undef DefTypeCCastToCollectHandler
#undef DefTypeCastFromCollectHandler
#undef DefTypeCCastFromCollectHandler

Template4InlineCollectionForAbstractCollect(TMapCastCollection, TMapCastCollectionCursor, TypeBase, TypeElement, TypeKeyTraits, Cast)

} // end of namespace COL

#endif // COL_VirtualMapCollectionH
