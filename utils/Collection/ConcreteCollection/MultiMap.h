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
// File      : MultiMap.h
// Description :
//   Definition of multiple maps based on sorted collections
//     and double linked lists for the elements with the same key.
//   It is a specialization of multiple collections with only multiple elements.
//

#ifndef COL_MultiMapH
#define COL_MultiMapH

#include "Collection/ConcreteCollection/MultiSorted.h"

namespace COL {

template <class TypeSortedCollection, class TypeMultipleElement,
      class TypeMultipleCollection = TList<TypeMultipleElement> >
class TMultiMapSortedTraits {
  public:
   typedef TypeSortedCollection SortedCollection;
   typedef typename TypeSortedCollection::KeyTraits KeyTraits;

   typedef HandlerCast<TypeMultipleElement> BaseCast;

   typedef COL::DVirtualCollection::TMapParameters<VirtualCollection::ExtendedLocateParameters, KeyTraits>
      ExtendedMapLocateParameters;
   typedef COL::DVirtualCollection::TMapParameters<VirtualCollection::ExtendedInsertionParameters, KeyTraits>
      ExtendedMapInsertionParameters;
   typedef COL::DVirtualCollection::TMapParameters<VirtualCollection::ExtendedSuppressParameters, KeyTraits>
      ExtendedMapSuppressParameters;
   typedef COL::DVirtualCollection::TMapParameters<VirtualCollection::ExtendedReplaceParameters, KeyTraits>
      ExtendedMapReplaceParameters;
   static typename KeyTraits::KeyType queryKey(const COL::DVirtualCollection::VirtualKeyHandler& keyHandler)
      {  return ((const COL::DVirtualCollection::TKeyHandler<KeyTraits>&) keyHandler).queryKey(); }
   static typename KeyTraits::KeyType queryKey(const VirtualCollection::ExtendedLocateParameters& parameters)
      {  return queryKey(parameters.getKeyHandler()); }

   typedef typename SortedCollection::Key Key;
   typedef TypeMultipleElement MultipleElement;

   typedef TypeMultipleCollection MultipleCollection;
   static const bool HasCopyHandlerOnMultiple = true;
   typedef TVirtualCast<EnhancedObject, SimpleCast> RetrieveMultipleRegistrationFromCopyCast;

  private:
   typedef TMultiMapSortedTraits<TypeSortedCollection, TypeMultipleElement, TypeMultipleCollection> thisType;
   typedef thisType TraitsType;

  public:
   static void setMultipleBase(MultipleElement& element, const typename SortedCollection::Node& node )
      {  element.setBase(node); }
   static const bool HasMultipleBase = MultipleElement::HasBase;
   static const typename SortedCollection::Node& getMultipleBase(const MultipleElement& element)
      {  return element.getBase(); }

   class ReceiverMultipleElement : public SortedCollection::Node {
     private:
      typedef typename SortedCollection::Node inherited;
      MultipleCollection mcElements;

     public:
      ReceiverMultipleElement(typename Key::KeyType key) : inherited(key) {}
      ReceiverMultipleElement(typename Key::KeyType key, MultipleElement* element)
         :  inherited(key)
         {  mcElements.addNew(element);
            setMultipleBase(*element, *this);
         }
      ReceiverMultipleElement(const ReceiverMultipleElement& source) : inherited(source) {}
      ReceiverMultipleElement& operator=(const ReceiverMultipleElement& source)
         {  inherited::operator=(source);
            mcElements.fullAssign(source.mcElements);
            mcElements.foreachSDo([this](MultipleElement& multipleElement)
               {  TraitsType::setMultipleBase(multipleElement, *this); return true; });
            return *this;
         }
      DefineCopy(ReceiverMultipleElement)
      DDefineAssign(ReceiverMultipleElement)

      virtual ~ReceiverMultipleElement() { mcElements.freeAll(); }

      ReceiverMultipleElement* createCopyAndShareElement() const
         {  ReceiverMultipleElement* result = new ReceiverMultipleElement(*this);
            AssumeAllocation(result)
            result->mcElements.fullAssign(mcElements, VirtualCollection::ExtendedReplaceParameters());
            return result;
         }
      void adjustCopy(const ReceiverMultipleElement& source,
            const VirtualCast* retrieveMultipleFromCopy=nullptr)
         {  if (!retrieveMultipleFromCopy)
               mcElements.fullAssign(source.mcElements,
                     VirtualCollection::ExtendedReplaceParameters().setDuplicate());
            else
               mcElements.fullAssign(source.mcElements, COL::DVirtualCollection::TCastParameters
                  <VirtualCollection::ExtendedReplaceParameters>().setExternCast(*retrieveMultipleFromCopy).setDuplicate());

            mcElements.foreachSDo([this](MultipleElement& multipleElement)
               {  TraitsType::setMultipleBase(multipleElement, *this); return true; });
         }
      bool isEmpty() const { return mcElements.isEmpty(); }
      bool isSingleton() const { return mcElements.count() == 1; }
      void add(MultipleElement* element, const VirtualCollection::ExtendedInsertionParameters& parameters,
            typename MultipleCollection::Cursor* cursor=nullptr)
         {  mcElements.add(element, parameters, cursor);
            setMultipleBase(*element, *this);
         }
      bool moveTo(ReceiverMultipleElement& destination, const VirtualCollection::ExtendedReplaceParameters& parameters,
            typename MultipleCollection::Cursor& cursor, typename MultipleCollection::Cursor& destinationCursor)
         {  mcElements.moveTo(destination.mcElements, parameters, &cursor, &destinationCursor);
            if (&destination != this)
               setMultipleBase((MultipleElement&) destinationCursor.elementAt(), destination);
            return ((&destination != this) && mcElements.isEmpty());
         }
      bool remove(const VirtualCollection::ExtendedSuppressParameters& parameters,
            typename MultipleCollection::Cursor* cursor)
         {  mcElements.remove(parameters, cursor);
            return isEmpty();
         }
      bool removeAll(const VirtualCollection::ExtendedSuppressParameters& parameters,
            typename MultipleCollection::Cursor* start=nullptr, typename MultipleCollection::Cursor* end=nullptr)
         {  mcElements.removeAll(parameters, start, end);
            return isEmpty();
         }
      void replace(MultipleElement* newElement, const VirtualCollection::ExtendedReplaceParameters parameters,
            typename MultipleCollection::Cursor* cursor)
         {  mcElements.replace(newElement, parameters, cursor); }
      MultipleElement& getElement(const VirtualCollection::ExtendedLocateParameters& parameters,
            typename MultipleCollection::Cursor* cursor)
         {  return mcElements.getElement(parameters, cursor); }
      typename MultipleCollection::PPCursor newCursor() const { return mcElements.newCursor(); }
      MultipleElement* extractElement() { return &mcElements.extractFirst(); }
      int count() const { return mcElements.count(); }
      bool contain(const typename MultipleCollection::Cursor& cursor) const
         {  return &mcElements == &cursor.getSupport(); }
      const MultipleCollection& getElements() const { return mcElements; }

      bool isValid() const override
         {  return inherited::isValid() && !mcElements.isEmpty() && mcElements.foreachDo(
               [this](const MultipleElement& multipleElement)
                  {  return (!HasMultipleBase
                        || (&TraitsType::getMultipleBase(multipleElement) == this));
                  });
         }
   };
};

/**********************************************/
/* Definition of the template class TMultiMap */
/**********************************************/

#define DefTypeVirtualInheritedCollection VirtualMultiSortedCollection
#define DefTypeVirtualInheritedCursor VirtualMultiSortedCollectionCursor
#define DefTypeCollection TMultiMap<TypeTraits>
#define DefTypeCursor TMultiMapCursor<TypeTraits>
#define DefTypeSorted
#define DefTypeMultiSorted

template <class TypeTraits>
class TMultiMapCursor;

template <class TypeTraits>
class TMultiMap : public VirtualMultiSortedCollection {
  protected:
   typedef typename TypeTraits::ExtendedMapLocateParameters ExtendedMapLocateParameters;
   typedef typename TypeTraits::ExtendedMapInsertionParameters ExtendedMapInsertionParameters;
   typedef typename TypeTraits::ExtendedMapSuppressParameters ExtendedMapSuppressParameters;
   typedef typename TypeTraits::ExtendedMapReplaceParameters ExtendedMapReplaceParameters;

  private:
   static const bool HasCopyHandlerOnMultiple = TypeTraits::HasCopyHandlerOnMultiple;

  public:
   typedef typename TypeTraits::SortedCollection SortedCollection;
   typedef typename TypeTraits::MultipleCollection MultipleCollection;
   typedef typename TypeTraits::MultipleElement MultipleElement;
   typedef typename TypeTraits::ReceiverMultipleElement ReceiverMultipleElement;
   typedef typename TypeTraits::SortedCollection::Key Key;
   typedef typename TypeTraits::BaseCast BaseCast;
   typedef MultipleElement Node;

   class EConflictKeyError {};
   static const bool HasNativeCopy = HasCopyHandlerOnMultiple;

   typedef typename TypeTraits::RetrieveMultipleRegistrationFromCopyCast RetrieveMultipleRegistrationFromCopyCast;
   class RetrieveRegistrationFromCopyCast : public VirtualCastWithElement {
     private:
      static const VirtualCastWithElement& getInterface(const VirtualCast& source)
         {  return (const VirtualCastWithElement&) source; }
      virtual EnhancedObject* _castFrom(EnhancedObject* copyObject) const
         {  return VirtualCastWithElement::_castFrom(copyObject); }
      virtual EnhancedObject* _castTo(EnhancedObject* collectionObject) const
         {  return VirtualCastWithElement::_castTo(collectionObject); }

     protected:
      virtual EnhancedObject* _castFrom(EnhancedObject* copyObject, EnhancedObject* sourceCollectionObject) const
         {  typename MultipleCollection::Node* result = (typename MultipleCollection::Node*)
               MultipleCollection::Node::castFromCopyHandler(
                  getInterface(RetrieveMultipleRegistrationFromCopyCast()).castFrom(copyObject, sourceCollectionObject));
            AssumeCondition(dynamic_cast<MultipleElement*>(result))
            return MultipleCollection::Node::castToCopyHandler(result);
         }
      virtual EnhancedObject* _castTo(EnhancedObject* collectionObject, EnhancedObject* sourceCopyObject) const
         {  return getInterface(RetrieveMultipleRegistrationFromCopyCast()).castTo(collectionObject, sourceCopyObject); }
      
     public:
      RetrieveRegistrationFromCopyCast() { setCastWithElement(); }
      RetrieveRegistrationFromCopyCast(const RetrieveRegistrationFromCopyCast& source) = default;
   };

   class RetrieveAssistedRegistrationFromCopyCast : public VirtualCastWithElement {
     private:
      const VirtualCastWithElement& vcweRetrieveCommonFromCopy;

      virtual EnhancedObject* _castFrom(EnhancedObject* copyObject) const
         {  return VirtualCastWithElement::_castFrom(copyObject); }
      virtual EnhancedObject* _castTo(EnhancedObject* collectionObject) const
         {  return VirtualCastWithElement::_castTo(collectionObject); }

     protected:
      virtual EnhancedObject* _castFrom(EnhancedObject* copyObject, EnhancedObject* sourceCollectionObject) const
         {  EnhancedObject* result = nullptr;
            if (!vcweRetrieveCommonFromCopy.isCastWithElement())
               result = vcweRetrieveCommonFromCopy.castFrom(copyObject);
            else {
               MultipleElement* reference = (MultipleElement*) ((typename MultipleCollection::Node*)
                  MultipleCollection::Node::castFromCopyHandler(sourceCollectionObject));
               result = vcweRetrieveCommonFromCopy.castFrom(copyObject, MultipleElement::castToCopyHandler(reference));
            };
            return result;
         }
      virtual EnhancedObject* _castTo(EnhancedObject* collectionObject, EnhancedObject* sourceCopyObject) const
         {  MultipleElement* result = (MultipleElement*) (typename MultipleCollection::Node*)
                  MultipleCollection::Node::castFromCopyHandler(collectionObject);
            return vcweRetrieveCommonFromCopy.castTo(MultipleElement::castToCopyHandler(result),
                  sourceCopyObject);
         }
      
     public:
      RetrieveAssistedRegistrationFromCopyCast(const VirtualCast& retrieveCommonFromCopy)
         :  vcweRetrieveCommonFromCopy((const VirtualCastWithElement&) retrieveCommonFromCopy)
         {  setCastWithElement(); }
      RetrieveAssistedRegistrationFromCopyCast(const RetrieveAssistedRegistrationFromCopyCast& source) = default;
   };

  private:
   typedef TMultiMap<TypeTraits> thisType;
   typedef VirtualMultiSortedCollection inherited;

   static ComparisonResult getComparison(RelativePosition pos)
      {  return (pos == COL::VirtualCollection::RPBefore) ? CRGreater : CRLess; }
   static ComparisonResult getComparison(const ExtendedLocateParameters& params)
      {  return getComparison(params.getRelativePosition()); }
   static ComparisonResult inverse(ComparisonResult compare)
      {  return (compare == CRGreater) ? CRLess : CRGreater; }
   static RelativePosition inverse(RelativePosition pos)
      {  return (pos == COL::VirtualCollection::RPBefore)
            ? COL::VirtualCollection::RPAfter : COL::VirtualCollection::RPBefore;
      }

   SortedCollection scSortedCollection;
   int uCount;
   void duplicateBody(const thisType& source, AddMode mode, const VirtualCast* retrieveCommonFromCopy)
      {  if (source.count() > 0) {
            if (mode == AMNoDuplicate) {
               try {
                  scSortedCollection.fullAssign(source.scSortedCollection, ExtendedReplaceParameters());
                  correctDuplicationSplitReceiver();
               }
               catch (...) {
                  scSortedCollection.deleteAll();
                  throw;
               };
            }
            else if (!retrieveCommonFromCopy) {
               if (HasNativeCopy) {
                  scSortedCollection.fullAssign(source.scSortedCollection,
                        ExtendedReplaceParameters().setDuplicate());
                  correctDuplicationCopyReceiver(source);
               }
               else {
                  scSortedCollection.fullAssign(source.scSortedCollection,
                        ExtendedReplaceParameters().setDuplicate());
                  RetrieveMultipleRegistrationFromCopyCast retrieveMultipleRegistrationFromCopyCast;
                  correctDuplicationCopyReceiver(source, &(const VirtualCast&) retrieveMultipleRegistrationFromCopyCast);
               };
            }
            else {
               scSortedCollection.fullAssign(source.scSortedCollection,
                     COL::DVirtualCollection::TCastParameters<ExtendedReplaceParameters>()
                     .setExternCast(*retrieveCommonFromCopy).setDuplicate());
               correctDuplicationCopyReceiver(source, retrieveCommonFromCopy);
            };
         };
         uCount = source.uCount;
      }
   void addBody(const thisType& source, const ExtendedInsertionParameters& parameters,
         const VirtualCast* retrieveCommonFromCopy,
         typename SortedCollection::Cursor* cursor, const typename SortedCollection::Cursor* start, const typename SortedCollection::Cursor* end,
         VirtualCollection::RelativePosition direction=COL::VirtualCollection::RPAfter, int addReceiver = -1)
      {  if (!parameters.isDuplicate()) {
            scSortedCollection.addAll(source.scSortedCollection, parameters, cursor,
                  start, end);
            correctDuplicationSplitReceiver(cursor, direction, addReceiver, parameters.isRemote());
         }
         else if (!retrieveCommonFromCopy) {
            if (HasNativeCopy) {
               scSortedCollection.addAll(source.scSortedCollection, parameters, cursor,
                     start, end);
               correctDuplicationCopyReceiver(source, nullptr, cursor, start, end,
                     direction, parameters.isRemote());
            }
            else {
               scSortedCollection.addAll(source.scSortedCollection, parameters, cursor,
                     start, end);
               RetrieveMultipleRegistrationFromCopyCast retrieveMultipleRegistrationFromCopyCast;
               correctDuplicationCopyReceiver(source, &retrieveMultipleRegistrationFromCopyCast,
                     cursor, start, end, direction, parameters.isRemote());
            };
         }
         else {
            scSortedCollection.addAll(source.scSortedCollection,
                  COL::DVirtualCollection::TCastParameters<ExtendedInsertionParameters>(parameters)
                  .setExternCast(*retrieveCommonFromCopy), cursor, start, end);
            RetrieveAssistedRegistrationFromCopyCast retrieveAssistedRegistrationFromCopyCast(*retrieveCommonFromCopy);
            correctDuplicationCopyReceiver(source, &retrieveAssistedRegistrationFromCopyCast,
                  cursor, start, end, direction, parameters.isRemote());
         };
      }

  public:
   TMultiMap() : scSortedCollection(), uCount(0) {}
   TMultiMap(const COL::VirtualCollection::InitialValues& initialValues)
      :  inherited(), scSortedCollection(), uCount(0) {}
   TMultiMap(const COL::VirtualCollection::InitialNewValues& initialValues)
      :  inherited(), scSortedCollection(), uCount(0) {}
   TMultiMap(const thisType& source, AddMode mode=AMNoDuplicate)
      :  inherited(source, mode), uCount(source.uCount)
      {  if (mode == AMNoDuplicate)
            scSortedCollection.fullAssign(source.scSortedCollection,
                  COL::VirtualCollection::ExtendedReplaceParameters().setDuplicate(mode));
         if (source.count() > 0) {
            if (mode == AMNoDuplicate)
               correctDuplicationSplitReceiver();
            else if (HasNativeCopy)
               correctDuplicationCopyReceiver(source);
            else {
               RetrieveMultipleRegistrationFromCopyCast retrieveMultipleRegistrationFromCopyCast;
               correctDuplicationCopyReceiver(source, &(const VirtualCast&) retrieveMultipleRegistrationFromCopyCast);
            };
         };
      }
   TMultiMap(thisType&& source)
      :  inherited(source), uCount(source.uCount)
      {  scSortedCollection.swap(source.scSortedCollection); }
   TMultiMap(const thisType& source, AddMode mode, const VirtualCast* retrieveCommonFromCopy)
      :  uCount(0)
      {  duplicateBody(source, mode, retrieveCommonFromCopy); }
   thisType& operator=(const thisType& source)
      {  if (this != &source) {
            if ((uCount != 0) && (scSortedCollection.count() != 0))
               _removeAll(ExtendedSuppressParameters());
            uCount = source.uCount;
            scSortedCollection.fullAssign(source.scSortedCollection,
               COL::VirtualCollection::ExtendedReplaceParameters());
            if (source.count() > 0)
               correctDuplicationSplitReceiver();
         };
         return *this;
      }
   thisType& operator=(thisType&& source)
      {  if (this != &source) {
            if ((uCount != 0) && (scSortedCollection.count() != 0))
               _removeAll(ExtendedSuppressParameters());
            uCount = source.uCount;
            source.uCount = 0;
            scSortedCollection.swap(source.scSortedCollection);
         };
         return *this;
      }
   virtual ~TMultiMap()
      {  if ((uCount != 0) && (scSortedCollection.count() != 0))
            _removeAll(ExtendedSuppressParameters());
      }
   TemplateDefineCopy(TMultiMap, TypeTraits)
   DTemplateDefineAssign(TMultiMap, TypeTraits)
   bool invariant() const
      {  int leftCount = uCount;
         return scSortedCollection.foreachDo([&leftCount]
            (const typename SortedCollection::Node& node)
            {  leftCount -= ((const ReceiverMultipleElement&) node).count();
               return ((const ReceiverMultipleElement&) node).isValid();
            })
            ? (leftCount == 0) : false;
      }

   TemplateDefineCollectionForAbstractCollect(TMultiMap, TMultiMapCursor, TypeTraits)

  private:
   class CountReceiver {
     private:
      int uCount;
      int uElements;

     public:
      CountReceiver() : uCount(0), uElements(0) {}

      bool operator()(const typename SortedCollection::Node& node)
         {  ++uCount;
            uElements += ((const ReceiverMultipleElement&) node).count();
            return true;
         }
      const int& getCountReceiver() const { return uCount; }
      const int& getCountElement() const { return uElements; }
   };

   class FreeReceiver {
     private:
      class FreeArray : public TArray<typename SortedCollection::Node> {
        private:
         typedef TArray<typename SortedCollection::Node> inherited;

        public:
         FreeArray(int allocation) { inherited::bookPlace(allocation); }
         FreeArray(const FreeArray& source) {}
         DefineCopy(FreeArray)

         void add(ReceiverMultipleElement& element)
            {  element.removeAll(ExtendedSuppressParameters(), nullptr, nullptr);
               inherited::add(&element);
            }
      };

      FreeArray  faFreeNodes;

     public:
      FreeReceiver(int allocation)
         :  faFreeNodes(allocation) {}
      bool operator()(typename SortedCollection::Node& node)
         {  faFreeNodes.add((ReceiverMultipleElement&) node);
            return true;
         }
      void freeAll() { faFreeNodes.freeAll(); }
   };

   void correctDuplicationSplitReceiver(const typename SortedCollection::Cursor* cursor=nullptr,
         VirtualCollection::RelativePosition direction=COL::VirtualCollection::RPAfter, int addReceiver = -1,
         bool isFirstExcluded=false);
   void correctDuplicationCopyReceiver(const thisType& source,
         const VirtualCast* retrieveMultipleRegistrationFromCopy=nullptr,
         const typename SortedCollection::Cursor* cursor=nullptr,
         const typename SortedCollection::Cursor* sourceStart=nullptr,
         const typename SortedCollection::Cursor* sourceEnd=nullptr,
         VirtualCollection::RelativePosition direction=COL::VirtualCollection::RPAfter, bool isFirstExcluded=false);

  protected:
   const SortedCollection& getSortedCollection() const { return scSortedCollection; }
   friend class TMultiMapCursor<TypeTraits>;

  protected:
   virtual void _fullAssign(const VirtualCollection& source, const ExtendedReplaceParameters& parameters) override
      {  inherited::pfullAssign(source, parameters); }
   void _fullAssign(const thisType& source, const ExtendedReplaceParameters& parameters);

   virtual void _add(EnhancedObject* anObject, const ExtendedInsertionParameters& parameters,
         VirtualCollectionCursor* cursor=nullptr) override; // dispatch method
   void _add(MultipleElement* element, const ExtendedInsertionParameters& parameters,
         Cursor* cursor=nullptr);
   void _add(typename Key::KeyType key, MultipleElement* element, const ExtendedInsertionParameters& parameters,
         Cursor* cursor=nullptr);

   void paddAll(const thisType& source, const ExtendedInsertionParameters& parameters,
         Cursor* cursor, const Cursor* startSource, const Cursor* endSource);
   virtual void _addAll(const VirtualCollection& source, const ExtendedInsertionParameters& parameters,
         VirtualCollectionCursor* cursor, const VirtualCollectionCursor* startSource,
         const VirtualCollectionCursor* endSource) override
      {  inherited::paddAll(source, parameters, cursor, startSource, endSource); }
   void _addAll(const thisType& source, const ExtendedInsertionParameters& parameters,
         Cursor* cursor=nullptr, const Cursor* startSource=nullptr, const Cursor* endSource=nullptr);
   virtual void _moveTo(VirtualCollection& destination, const ExtendedReplaceParameters& parameters,
         VirtualCollectionCursor* cursor, VirtualCollectionCursor* destinationCursor) override
      {  inherited::pmoveTo(destination, parameters, cursor, destinationCursor); }
   void _moveTo(thisType& destination, const ExtendedReplaceParameters& parameters,
         Cursor* cursor=nullptr, Cursor* destinationCursor=nullptr);
   virtual void _moveAllTo(VirtualCollection& destination, const ExtendedReplaceParameters& parameters,
         const VirtualCollectionCursor* startCursor, const VirtualCollectionCursor* endCursor,
         VirtualCollectionCursor* destinationCursor) override
      {  inherited::pmoveAllTo(destination, parameters, startCursor, endCursor, destinationCursor); }
   void _moveAllTo(thisType& destination, const ExtendedReplaceParameters& parameters,
         const Cursor* startCursor=nullptr, const Cursor* endCursor=nullptr,
         Cursor* destinationCursor=nullptr);
   virtual void _remove(const ExtendedSuppressParameters& parameters, VirtualCollectionCursor* cursor) override
      {  _remove(parameters, (Cursor*) cursor); }
   void _remove(const ExtendedSuppressParameters& parameters, Cursor* cursor=nullptr);
   virtual void _removeAll(const ExtendedSuppressParameters& parameters,
         const VirtualCollectionCursor* start, const VirtualCollectionCursor* end) override
      {  _removeAll(parameters, (const Cursor*) start, (const Cursor*) end); }
   void _removeAll(const ExtendedSuppressParameters& parameters,
         const Cursor* start=nullptr, const Cursor* end=nullptr);
   virtual void _replace(EnhancedObject* anObject,
         const ExtendedReplaceParameters& parameters, VirtualCollectionCursor* cursor) override
      {  _replace((MultipleElement*) BaseCast::castFrom(anObject), parameters, (Cursor*) cursor); }
   void _replace(MultipleElement* element, const ExtendedReplaceParameters& parameters,
         Cursor* cursor=nullptr);
   virtual int _queryCount(const ExtendedLocateParameters& parameters,
         const VirtualCollectionCursor* start=nullptr, const VirtualCollectionCursor* end=nullptr) const override
      {  return (!start && !end) ? uCount
            : VirtualCollection::pqueryCount(parameters, start, end);
      }
   virtual EnhancedObject* _getElement(const ExtendedLocateParameters& parameters,
         const VirtualCollectionCursor* cursor) const override
      {  return BaseCast::castTo(_getElement(parameters, (Cursor*) cursor)); }
   MultipleElement* _getElement(const ExtendedLocateParameters& parameters, const Cursor* cursor=nullptr) const;

   virtual LocationResult _locate(const EnhancedObject& source, const ExtendedLocateParameters& parameters,
         VirtualMultiSortedCollectionCursor* cursor, const VirtualMultiSortedCollectionCursor* start,
         const VirtualMultiSortedCollectionCursor* end) const override;
   LocationResult _locateKey(typename Key::KeyType key, const ExtendedLocateParameters& parameters,
         Cursor* cursor=nullptr, const Cursor* start=nullptr, const Cursor* end=nullptr) const;

   typename TypeTraits::KeyTraits::ControlKeyType _queryKey(const ExtendedLocateParameters& parameters,
         const Cursor* cursor=nullptr) const;
   
  private:
   virtual const EnhancedObject& key(const EnhancedObject& element) const override { return inherited::key(element); }
   virtual VirtualMultiSortedCollection::LocationResult _locateKey(const EnhancedObject& source,
         const ExtendedLocateParameters& parameters, VirtualMultiSortedCollectionCursor* cursor,
         const VirtualMultiSortedCollectionCursor* start, const VirtualMultiSortedCollectionCursor* end) const override
      {  return inherited::_locateKey(source, parameters, cursor, start, end); }

  public:
   virtual typename TypeTraits::KeyTraits::ControlKeyType key(const MultipleElement& element) const { AssumeUncalled }

   #define DefJustDeclare
   #define DefTypeKey typename Key::KeyType

   #define DefTypeElement MultipleElement
   #define DefExcludeElement
   #define DefExcludeReplaceElement
   #define DefMapInterface
   #define DefTypeControlKey typename TypeTraits::KeyTraits::ControlKeyType
   #define DefBaseMultipleInterface
   #define DefNeedLocate
   #include "Collection/ELMCollection.inch"
   #undef DefNeedLocate
   #undef DefBaseMultipleInterface
   #undef DefTypeControlKey
   #undef DefMapInterface
   #undef DefExcludeElement
   #undef DefExcludeReplaceElement
   #undef DefTypeElement

   #define DefBaseMultipleInterface
   #define DefImplementsNativeMethods
   #define DefTypeElement MultipleElement
   // #define DefExcludeElement
   #include "Collection/ELMMapCollection.inch"
   // #undef DefExcludeElement
   #undef DefTypeElement
   #undef DefImplementsNativeMethods
   #undef DefBaseMultipleInterface

   #undef DefTypeKey
   #undef DefJustDeclare

   template <class Execute> bool foreachDo(Execute& function, const ExtendedLocateParameters& parameters,
         const Cursor* start, const Cursor* end) const
      {  start = ((start == nullptr) || !start->isValid()) ? nullptr : start;
         end = ((end == nullptr) || !end->isValid()) ? nullptr : end;
         if (start && end && (start->sortedCursor() > end->sortedCursor()))
            return true;
         if (start && end && start->sortedCursor().isEqual(end->sortedCursor())) {
            return ((const ReceiverMultipleElement&) start->sortedCursor().elementAt())
               .getElements().foreachDo(function, parameters,
                     start->multipleCursor().key(), end->multipleCursor().key());
         };
         ExtendedLocateParameters internParameters;
         if (start) {
            if (!((const ReceiverMultipleElement&) start->sortedCursor().elementAt())
               .getElements().foreachDo(function, parameters, start->multipleCursor().key(), nullptr))
               return false;
            internParameters.setFirstExcluded();
         };
         if (end)
            internParameters.setLastExcluded();
         DMultipleSortedCollection::ElementExecute<TypeTraits, Execute> execute(function);
         if (!scSortedCollection.foreachDo(execute, internParameters,
               start ? &start->sortedCursor() : nullptr,
               end ? &end->sortedCursor() : nullptr))
            return false;

         if (internParameters.isLastExcluded())
            return ((const ReceiverMultipleElement&) end->sortedCursor().elementAt())
               .getElements().foreachDo(function, parameters, nullptr, end->multipleCursor().key());
         return true;
      }
   template <class Execute> bool foreachDo(Execute& function) const
      {  DMultipleSortedCollection::ElementExecute<TypeTraits, Execute> execute(function);
         return scSortedCollection.foreachDo(execute);
      }
   template <class Execute> bool foreachReverseDo(Execute& function, const ExtendedLocateParameters& parameters,
         const Cursor* start, const Cursor* end) const
      {  start = ((start == nullptr) || !start->isValid()) ? nullptr : start;
         end = ((end == nullptr) || !end->isValid()) ? nullptr : end;
         if (start && end && (start->sortedCursor() < end->sortedCursor()))
            return true;
         if (start && end && start->sortedCursor().isEqual(end->sortedCursor())) {
            return ((const ReceiverMultipleElement&) start->sortedCursor().elementAt())
               .getElements().foreachReverseDo(function, parameters,
                     start->multipleCursor().key(), end->multipleCursor().key());
         };
         ExtendedLocateParameters internParameters;
         if (start) {
            if (!((const ReceiverMultipleElement&) start->sortedCursor().elementAt())
               .getElements().foreachReverseDo(function, parameters, start->multipleCursor().key(), nullptr))
               return false;
            internParameters.setFirstExcluded();
         };
         if (end)
            internParameters.setLastExcluded();
         DMultipleSortedCollection::ElementExecute<TypeTraits, Execute> execute(function);
         if (!scSortedCollection.foreachReverseDo(execute, internParameters,
               start ? &start->sortedCursor() : nullptr,
               end ? &end->sortedCursor() : nullptr))
            return false;

         if (internParameters.isLastExcluded())
            return ((const ReceiverMultipleElement&) end->sortedCursor().elementAt())
               .getElements().foreachReverseDo(function, parameters, nullptr, end->multipleCursor().key());
         return true;
      }
   template <class Execute> bool foreachReverseDo(Execute& function) const
      {  DMultipleSortedCollection::ElementExecute<TypeTraits, Execute> execute(function);
         return scSortedCollection.foreachReverseDo(execute);
      }
};

/****************************************************/
/* Definition of the template class TMultiMapCursor */
/****************************************************/

template <class TypeTraits>
class TMultiMapCursor : public VirtualMultiSortedCollectionCursor {
  public:
   typedef typename TypeTraits::SortedCollection SortedCollection;
   typedef typename TypeTraits::SortedCollection::Cursor SortedCollectionCursor;
   typedef typename TypeTraits::MultipleCollection MultipleCollection;
   typedef typename TypeTraits::MultipleCollection::PPCursor PPMultipleCollectionCursor;
   typedef typename TypeTraits::MultipleElement MultipleElement;
   typedef typename TypeTraits::ReceiverMultipleElement ReceiverMultipleElement;
   typedef typename TypeTraits::SortedCollection::Key Key;
   typedef typename TypeTraits::BaseCast BaseCast;

  private:
   typedef VirtualMultiSortedCollectionCursor inherited;
   typedef TMultiMap<TypeTraits> CollectionSupport;
   typedef TMultiMapCursor<TypeTraits> thisType;

   SortedCollectionCursor sccCursor;
   PPMultipleCollectionCursor ppmccMultipleCursor;

   void normalize()
      {  if (!thisType::isValid()) {
            if (sccCursor.isValid()) sccCursor.invalidate();
            if (ppmccMultipleCursor.isValid()) ppmccMultipleCursor.release();
         };
      }
   static VirtualCollection::RelativePosition inverse(VirtualCollection::RelativePosition pos)
      {  return (pos == COL::VirtualCollection::RPBefore)
            ? COL::VirtualCollection::RPAfter : COL::VirtualCollection::RPBefore;
      }

  protected:
   virtual ComparisonResult _compare(const EnhancedObject& asource) const override
      {  ComparisonResult result = inherited::_compare(asource);
         if (result == CREqual) {
            const thisType& source = (const thisType&) castFromCopyHandler(asource);
            if ((result = sccCursor.compare(source.sccCursor)) == CREqual)
               result = ppmccMultipleCursor.isValid() ? (source.ppmccMultipleCursor.isValid()
                  ? ppmccMultipleCursor->compare(*source.ppmccMultipleCursor)
                  : CRNonComparable) : (source.ppmccMultipleCursor.isValid()
                  ? CRNonComparable : CREqual);
         };
         return result;
      }

   SortedCollectionCursor& sortedCursor() { return sccCursor; }
   const SortedCollectionCursor& sortedCursor() const { return sccCursor; }
   PPMultipleCollectionCursor& multipleCursor() { return ppmccMultipleCursor; }
   const PPMultipleCollectionCursor& multipleCursor() const { return ppmccMultipleCursor; }

   virtual bool _position(const inherited::Position& pos) override;
   virtual void _gotoReference(const EnhancedObject& element) override;
   virtual bool _isPositionned(const ExtendedLocateParameters& pos, const VirtualCollectionCursor* cursor=nullptr) const override;
   virtual EnhancedObject* _getSElement() const override
      {  return BaseCast::castTo((MultipleElement*) &ppmccMultipleCursor->elementSAt()); }

   virtual bool _isEqual(const AbstractCursor& cursor) const override
      {  const thisType& source = (const thisType&) cursor;
         return inherited::_isEqual(cursor) && sccCursor.isEqual(source.sccCursor)
            && (ppmccMultipleCursor.isValid()
               ? (source.ppmccMultipleCursor.isValid() && ppmccMultipleCursor->isEqual(*source.ppmccMultipleCursor))
               : !source.ppmccMultipleCursor.isValid());
      }
   typename TypeTraits::KeyTraits::KeyType _queryKey() const { return sccCursor.queryKey(); }

  public:
   TMultiMapCursor(const CollectionSupport& support)
      :  inherited(support), sccCursor(support.getSortedCollection()) {}
   TMultiMapCursor(const thisType& source)
      :  inherited(source), sccCursor(source.sccCursor),
         ppmccMultipleCursor(source.ppmccMultipleCursor, PNT::Pointer::Duplicate()) {}
   TMultiMapCursor(thisType&& source)
      :  inherited(source), sccCursor(source.sccCursor),
         ppmccMultipleCursor(source.ppmccMultipleCursor) {}
   thisType& operator=(const thisType& source)
      {  inherited::operator=(source);
         sccCursor = source.sccCursor;
         ppmccMultipleCursor.fullAssign(source.ppmccMultipleCursor);
         return *this;
      }
   thisType& operator=(thisType&& source)
      {  inherited::operator=(source);
         sccCursor.swap(source.sccCursor);
         ppmccMultipleCursor.swap(source.ppmccMultipleCursor);
         return *this;
      }
   TemplateDefineCopy(TMultiMapCursor, TypeTraits)
   TemplateDefineCursorForAbstractCollect(TMultiMap, TMultiMapCursor, TypeTraits)
   void swap(thisType& source)
      {  inherited::swap(source);
         sccCursor.swap(source.sccCursor);
         ppmccMultipleCursor.swap(source.ppmccMultipleCursor);
      }

   virtual bool isValid() const override
      {  return inherited::isValid() && sccCursor.isValid() && ppmccMultipleCursor.isValid(); }
   bool invariant() const
      {  return ppmccMultipleCursor.isValid() == sccCursor.isValid(); }

#define DefCursor
#define DefTypeFinal thisType::
#define DefTypeCastToCollectHandler BaseCast::castTo
#define DefTypeCCastToCollectHandler BaseCast::castTo
#define DefTypeCastFromCollectHandler BaseCast::castFrom
#define DefTypeCCastFromCollectHandler BaseCast::castFrom
#define DefTypeElement MultipleElement
   #include "Collection/ELMCollection.inch"
#undef DefTypeElement
#undef DefTypeCastToCollectHandler
#undef DefTypeCCastToCollectHandler
#undef DefTypeCastFromCollectHandler
#undef DefTypeCCastFromCollectHandler
#undef DefTypeFinal
#undef DefCursor

   typename TypeTraits::KeyTraits::KeyType queryKey() const { return _queryKey(); }
};

TemplateInlineCollectionForAbstractCollect(TMultiMap, TMultiMapCursor, TypeTraits)

#define DefTemplate template <class TypeTraits>
#define DefJustInline
#define DefTypeKey typename Key::KeyType

#define DefImplementsNativeMethods
#define DefTypeElement typename TypeTraits::MultipleElement
#define DefExcludeElement
#define DefExcludeReplaceElement
#define DefMapInterface
#define DefTypeControlKey typename TypeTraits::KeyTraits::ControlKeyType
#define DefBaseMultipleInterface
#define DefNeedLocate
#include "Collection/ELMCollection.inch"
#undef DefNeedLocate
#undef DefBaseMultipleInterface
#undef DefTypeControlKey
#undef DefMapInterface
#undef DefExcludeReplaceElement
#undef DefExcludeElement
#undef DefTypeElement
#undef DefImplementsNativeMethods
   
#define DefBaseMultipleInterface
#define DefImplementsNativeMethods
#define DefTypeElement typename TypeTraits::MultipleElement
#define DefTypeCastToCollectHandler BaseCast::castTo
#define DefTypeCCastToCollectHandler BaseCast::castTo
#define DefTypeCastFromCollectHandler BaseCast::castFrom
#define DefTypeCCastFromCollectHandler BaseCast::castFrom
#define DefMapInterface
#define DefTypeControlKey typename TypeTraits::KeyTraits::ControlKeyType
#include "Collection/ELMMapCollection.inch"
#undef DefTypeControlKey
#undef DefMapInterface
#undef DefTypeCastToCollectHandler
#undef DefTypeCCastToCollectHandler
#undef DefTypeCastFromCollectHandler
#undef DefTypeCCastFromCollectHandler
#undef DefTypeElement
#undef DefImplementsNativeMethods
#undef DefBaseMultipleInterface

#undef DefTypeKey
#undef DefJustInline
#undef DefTemplate

#undef DefTypeVirtualInheritedCollection
#undef DefTypeVirtualInheritedCursor
#undef DefTypeCollection
#undef DefTypeCursor
#undef DefTypeMultiSorted
#undef DefTypeSorted

} // end of namespace COL

#endif // COL_MultiMapH

