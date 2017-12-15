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
// File      : MultiSorted.h
// Description :
//   Definition of multiple collections based on sorted collections
//   and double linked lists for the elements with the same key.
//

#ifndef COL_MultiSortedH
#define COL_MultiSortedH

#include "Collection/VirtualCollection/VirtualMultiSortedCollection.h"
#include "Collection/ConcreteCollection/List.h"
#include "Collection/ConcreteCollection/Array.h"

namespace COL {

namespace DMultipleSortedCollection {

template <class TypeSortedNode>
class TBaseRegistration {
  private:
   const TypeSortedNode* ptsnNode;

  public:
   TBaseRegistration() : ptsnNode(nullptr) {}
   TBaseRegistration(const TBaseRegistration<TypeSortedNode>& source) : ptsnNode(nullptr) {}
   TBaseRegistration& operator=(const TBaseRegistration<TypeSortedNode>& source) { return *this; }
   void setBase(const TypeSortedNode& node) { ptsnNode = &node; }
   static const bool HasBase = true;
   const TypeSortedNode& getBase() const { AssumeCondition(ptsnNode) return *ptsnNode; }
   bool isValid() const { return ptsnNode != nullptr; }
};

template <class TypeSortedNode>
class TBaseNoRegistration {
  public:
   TBaseNoRegistration& operator=(const TBaseNoRegistration<TypeSortedNode>& source) { return *this; }
   void setBase(const TypeSortedNode& node) {}
   static const bool HasBase = false;
   const TypeSortedNode& getBase() const { AssumeUncalled }
   bool isValid() const { return true; }
};

template <class TypeTraits, class TypeExecute>
class ElementExecute {
  private:
   TypeExecute& eFunction;

  public:
   ElementExecute(TypeExecute& function) : eFunction(function) {}
   bool operator()(typename TypeTraits::SortedCollection::Node& node) const
      {  bool result = false;
         typename TypeTraits::IdentificationElement identification = TypeTraits::identifyElementFromSorted(node);
         if (identification == TypeTraits::IEReceiverMultiple)
            result = ((const typename TypeTraits::ReceiverMultipleElement&) node)
               .getElements().foreachDo(eFunction);
         else if (identification == TypeTraits::IEReceiverSingle)
            result = eFunction(((const typename TypeTraits::ReceiverSingleElement&) node).getElement());
         else {
            AssumeCondition(identification == TypeTraits::IEUnique)
            result = eFunction(((const typename TypeTraits::UniqueElement&) node));
         };
         return result;
      }
};

template <class TypeTraits, class TypeExecute>
class ReverseElementExecute {
  private:
   TypeExecute& eFunction;

  public:
   ReverseElementExecute(TypeExecute& function) : eFunction(function) {}
   bool operator()(typename TypeTraits::SortedCollection::Node& node) const
      {  bool result = false;
         typename TypeTraits::IdentificationElement identification
            = TypeTraits::identifyElementFromSorted(node);
         if (identification == TypeTraits::IEReceiverMultiple)
            result = ((const typename TypeTraits::ReceiverMultipleElement&) node)
               .getElements().foreachReverseDo(eFunction);
         else if (identification == TypeTraits::IEReceiverSingle)
            result = eFunction(((const typename TypeTraits::ReceiverSingleElement&) node).getElement());
         else {
            AssumeCondition(identification == TypeTraits::IEUnique)
            result = eFunction(((const typename TypeTraits::UniqueElement&) node));
         };
         return result;
      }
};

template <class TypeCommonElement, class TypeSortedNode>
class TUnusefulSingleElement : public TypeCommonElement, public TBaseRegistration<TypeSortedNode> {
  private:
   typedef TypeCommonElement inherited;
   typedef TUnusefulSingleElement<TypeCommonElement, TypeSortedNode> thisType;

  public:
   TUnusefulSingleElement() { AssumeUncalled }
   TUnusefulSingleElement(const thisType& source) = default;
   Template2DefineCopy(TUnusefulSingleElement, TypeCommonElement, TypeSortedNode)
};

template <class TypeCommonElement, class TypeSortedNode>
class TUnusefulUniqueElement : public TypeSortedNode, public TypeCommonElement {
  private:
   typedef TypeSortedNode inherited;
   typedef TUnusefulUniqueElement<TypeCommonElement, TypeSortedNode> thisType;
  public:
   TUnusefulUniqueElement() { AssumeUncalled }
   TUnusefulUniqueElement(const thisType& source) = default;
   Template2DefineCopy(TUnusefulUniqueElement, TypeCommonElement, TypeSortedNode)
   StaticInheritConversions(thisType, inherited)
};

} // end of namespace DMultipleSortedCollection

template <class TypeSortedCollection, class TypeCommonElement>
class TMultipleBaseSortedTraits {
  public:
   typedef TypeSortedCollection SortedCollection;
   typedef TypeCommonElement CommonElement;
   typedef typename TypeSortedCollection::KeyTraits KeyTraits;

  public:
   typedef HandlerCast<TypeCommonElement> BaseCast;

   typedef DVirtualCollection::TMapParameters<VirtualCollection::ExtendedLocateParameters, KeyTraits>
      ExtendedMapLocateParameters;
   typedef DVirtualCollection::TMapParameters<VirtualCollection::ExtendedInsertionParameters, KeyTraits>
      ExtendedMapInsertionParameters;
   typedef DVirtualCollection::TMapParameters<VirtualCollection::ExtendedSuppressParameters, KeyTraits>
      ExtendedMapSuppressParameters;
   typedef DVirtualCollection::TMapParameters<VirtualCollection::ExtendedReplaceParameters, KeyTraits>
      ExtendedMapReplaceParameters;
   static typename KeyTraits::KeyType queryKey(const DVirtualCollection::VirtualKeyHandler& keyHandler)
      {  return ((const DVirtualCollection::TKeyHandler<KeyTraits>&) keyHandler).queryKey(); }
   static typename KeyTraits::KeyType queryKey(const VirtualCollection::ExtendedLocateParameters& parameters)
      {  return queryKey(parameters.getKeyHandler()); }
};

template <class TypeBase, class TypeUniqueElement>
class TUniqueElementSortedTraits : public TypeBase {
  public:
   typedef typename TypeBase::CommonElement CommonElement;
   typedef TypeUniqueElement UniqueElement;
   typedef HandlerCast<UniqueElement, CommonElement> UniqueBaseCast;
   typedef HandlerDynamicCast<UniqueElement, CommonElement> UniqueDynamicBaseCast;
   static const bool HasCopyHandlerOnSorted = true;
   typedef TVirtualCast<EnhancedObject, SimpleCast> RetrieveUniqueRegistrationFromCopyCast;
};

template <class TypeBase, class TypeSingleElement>
class TSingleElementSortedTraits : public TypeBase {
  public:
   typedef typename TypeBase::CommonElement CommonElement;
   typedef typename TypeBase::SortedCollection SortedCollection;
   typedef typename SortedCollection::Key Key;
   typedef typename TypeBase::BaseCast BaseCast;
   typedef TypeSingleElement                          SingleElement;
   typedef HandlerCast<SingleElement, CommonElement>  SingleBaseCast;
   typedef HandlerDynamicCast<SingleElement, CommonElement> SingleDynamicBaseCast;
   static const bool HasCopyHandlerOnSingle = true;
   typedef TVirtualCast<EnhancedObject, SimpleCast> RetrieveSingleRegistrationFromCopyCast;

  private:
   typedef TSingleElementSortedTraits<TypeBase, TypeSingleElement> thisType;
   typedef thisType TraitsType;

  public:
   static void setSingleBase(SingleElement& element, const typename SortedCollection::Node& node )
      {  element.setBase(node); }
   static const bool HasSingleBase = SingleElement::HasBase;
   static const typename SortedCollection::Node& getSingleBase(const SingleElement& element)
      {  return element.getBase(); }

   class ReceiverSingleElement : public SortedCollection::Node {
     private:
      typedef typename SortedCollection::Node inherited;
      PNT::AutoPointer<SingleElement> aptseElement;

     public:
      ReceiverSingleElement(typename Key::KeyType key, SingleElement* element)
         :  inherited(key)
         {  aptseElement.absorbElement(element);
            setSingleBase(*element, *this);
         }
      ReceiverSingleElement(const ReceiverSingleElement& source) : inherited(source) {}
      ReceiverSingleElement& operator=(const ReceiverSingleElement& source)
         {  inherited::operator=(source);
            aptseElement = source.aptseElement;
            setSingleBase(*aptseElement, *this);
            return *this;
         }

      DefineCopy(ReceiverSingleElement)
      DDefineAssign(ReceiverSingleElement)

      ReceiverSingleElement(typename Key::KeyType key) : inherited(key) {}
      ReceiverSingleElement* createCopyAndShareElement() const
         {  ReceiverSingleElement* result = new ReceiverSingleElement(*this);
            AssumeAllocation(result)
            result->aptseElement.absorbElement(const_cast<SingleElement*>(aptseElement.key()));
            return result;
         }
      void adjustCopy(const ReceiverSingleElement& source,
            const VirtualCast* retrieveSingleFromCopy=nullptr)
         {  if (source.aptseElement.isValid()) {
               if (!retrieveSingleFromCopy)
                  aptseElement = source.aptseElement;
               else
                  aptseElement.absorbElement((SingleElement*) SingleElement::castFromCopyHandler(
                        retrieveSingleFromCopy->castFrom(source.aptseElement->createCopy())));
               setSingleBase(*aptseElement, *this);
            };
         }
      bool isValid() const
         {  return inherited::isValid() && aptseElement.isValid()
               && (!HasSingleBase || &getSingleBase(*aptseElement) == this);
         }
      SingleElement& getElement() const { return *aptseElement; }
      SingleElement* extractElement() { return aptseElement.extractElement(); }
      void absorbElement(SingleElement* element) { aptseElement.absorbElement(element); }
   };
};

template <class TypeBase, class TypeMultipleElement, class TypeMultipleCollection>
class TMultipleElementSortedTraits : public TypeBase {
  public:
   typedef typename TypeBase::CommonElement CommonElement;
   typedef typename TypeBase::SortedCollection SortedCollection;
   typedef typename SortedCollection::Key Key;
   typedef typename TypeBase::BaseCast BaseCast;
   typedef TypeMultipleElement                          MultipleElement;
   typedef HandlerCast<MultipleElement, CommonElement>  MultipleBaseCast;
   typedef HandlerDynamicCast<MultipleElement, CommonElement> MultipleDynamicBaseCast;

   typedef TypeMultipleCollection MultipleCollection;
   static const bool HasCopyHandlerOnMultiple = true;
   typedef TVirtualCast<EnhancedObject, SimpleCast> RetrieveMultipleRegistrationFromCopyCast;

  private:
   typedef TMultipleElementSortedTraits<TypeBase, TypeMultipleElement, TypeMultipleCollection> thisType;
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
         : inherited(key)
         {  mcElements.addNew(element);
            setMultipleBase(*element, *this);
         }
      ReceiverMultipleElement(const ReceiverMultipleElement& source) : inherited(source) {}
      ReceiverMultipleElement& operator=(const ReceiverMultipleElement& source)
         {  inherited::operator=(source);
            mcElements.fullAssign(source.mcElements);
            mcElements.foreachDo([this](MultipleElement& multipleElement)
               {  TraitsType::setMultipleBase(multipleElement, *this); return true; });
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
               mcElements.fullAssign(source.mcElements, DVirtualCollection::TCastParameters
                  <VirtualCollection::ExtendedReplaceParameters>().setExternCast(*retrieveMultipleFromCopy).setDuplicate());
            mcElements.foreachDo([this](MultipleElement& multipleElement)
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
         {  mcElements.replace(newElement, parameters, cursor);
            setMultipleBase(*newElement, *this);
         }
      MultipleElement& getElement(const VirtualCollection::ExtendedLocateParameters& parameters,
            typename MultipleCollection::Cursor* cursor)
         {  return mcElements.getElement(parameters, cursor); }

      typename MultipleCollection::PPCursor newCursor() const { return mcElements.newCursor(); }
      MultipleElement* extractElement() { return &mcElements.extractFirst(); }
      int count() const { return mcElements.count(); }
      bool contain(const typename MultipleCollection::Cursor& cursor) const
         {  return &mcElements == &cursor.getSupport(); }
      const MultipleCollection& getElements() const { return mcElements; }

      bool isValid() const
         {  return inherited::isValid() && !mcElements.isEmpty()
               && mcElements.foreachDo([this](MultipleElement& multipleElement)
                     {  return (!HasMultipleBase
                           || (&TraitsType::getMultipleBase(multipleElement) == this));
                     });
         }
   };
};

namespace DMultipleSortedCollection {

class Identification {
  public:
   enum IdentificationElement
      {  IEUndefined, IEUnique, IESingle, IEMultiple, IEReceiverSingle, IEReceiverMultiple };
};

} // end of namespace DMultipleSortedCollection

template <class TypeSortedCollection, class TypeCommonElement,
   class TypeUniqueElement, class TypeSingleElement, class TypeMultipleElement>
class TMultipleSortedTraits
   : public TMultipleElementSortedTraits<
               TSingleElementSortedTraits<
                  TUniqueElementSortedTraits<
                     TMultipleBaseSortedTraits<TypeSortedCollection, TypeCommonElement>,
                     TypeUniqueElement>,
                  TypeSingleElement>,
               TypeMultipleElement,
               TList<TypeMultipleElement> >, public DMultipleSortedCollection::Identification {
  private:
   typedef TMultipleElementSortedTraits<TSingleElementSortedTraits<TUniqueElementSortedTraits<
      TMultipleBaseSortedTraits<TypeSortedCollection, TypeCommonElement>, TypeUniqueElement>,
         TypeSingleElement>, TypeMultipleElement, TList<TypeMultipleElement> > inherited;

  public:
   static IdentificationElement identifyElementFromCopyHandler(const EnhancedObject& element)
      {  IdentificationElement result = IEUndefined;
         if (dynamic_cast<const typename inherited::UniqueElement*>((const typename inherited::SortedCollection::Node*)
                  inherited::SortedCollection::Node::castFromCopyHandler(&element)))
            result = IEUnique;
         else if (inherited::SingleDynamicBaseCast::castFrom(inherited::SingleBaseCast::castTo((const typename inherited::SingleElement*)
                  inherited::SingleElement::castFromCopyHandler(&element))))
            result = IESingle;
         else {
            AssumeCondition(dynamic_cast<const typename inherited::MultipleElement*>(
               (const typename inherited::MultipleCollection::Node*)
               inherited::MultipleCollection::Node::castFromCopyHandler(&element)))
            result = IEMultiple;
         };
         return result;
      }
   static IdentificationElement identifyElementFromCommon(const TypeCommonElement& element)
      {  IdentificationElement result = IEUndefined;
         if (dynamic_cast<const typename inherited::UniqueElement*>(&element))
            result = IEUnique;
         else if (dynamic_cast<const typename inherited::SingleElement*>(&element))
            result = IESingle;
         else {
            AssumeCondition(dynamic_cast<const typename inherited::MultipleElement*>(&element))
            result = IEMultiple;
         };
         return result;
      }
   static IdentificationElement identifyElementFromSorted(const typename TypeSortedCollection::Node& element)
      {  IdentificationElement result = IEUndefined;
         if (dynamic_cast<const typename inherited::ReceiverSingleElement*>(&element))
            result = IEReceiverSingle;
         else if (dynamic_cast<const typename inherited::ReceiverMultipleElement*>(&element))
            result = IEReceiverMultiple;
         else {
            AssumeCondition(dynamic_cast<const typename inherited::UniqueElement*>(&element))
            result = IEUnique;
         };
         return result;
      }
};

template <class TypeTraits>
class TMultipleTraitsForCommonCopy : public TypeTraits {
  public:
   static const bool HasCopyHandlerOnSorted = false;
   typedef TVirtualCast<typename TypeTraits::SortedCollection::Node,
         HandlerIntermediateCast<typename TypeTraits::SortedCollection::Node, typename TypeTraits::UniqueElement, typename TypeTraits::CommonElement> >
      RetrieveUniqueRegistrationFromCopyCast;

   static const bool HasCopyHandlerOnSingle = true;
   typedef TVirtualCast<EnhancedObject, SimpleCast> RetrieveSingleRegistrationFromCopyCast;

   static const bool HasCopyHandlerOnMultiple = false;
   typedef TVirtualCast<typename TypeTraits::MultipleCollection::Node,
         HandlerIntermediateCast<typename TypeTraits::MultipleCollection::Node, typename TypeTraits::MultipleElement, typename TypeTraits::CommonElement> >
      RetrieveMultipleRegistrationFromCopyCast;
};

/**************************************************************/
/* Definition of the template class TMultipleSortedCollection */
/**************************************************************/

#define DefTypeVirtualInheritedCollection VirtualMultiSortedCollection
#define DefTypeVirtualInheritedCursor VirtualMultiSortedCollectionCursor
#define DefTypeCollection TMultipleSortedCollection<TypeTraits>
#define DefTypeCursor TMultipleSortedCollectionCursor<TypeTraits>
#define DefTypeSorted
#define DefTypeMultiSorted

template <class TypeTraits>
class TMultipleSortedCollectionCursor;

template <class TypeTraits>
class TMultipleSortedCollection : public VirtualMultiSortedCollection, public DMultipleSortedCollection::Identification {
  protected:
   typedef typename TypeTraits::ExtendedMapLocateParameters ExtendedMapLocateParameters;
   typedef typename TypeTraits::ExtendedMapInsertionParameters ExtendedMapInsertionParameters;
   typedef typename TypeTraits::ExtendedMapSuppressParameters ExtendedMapSuppressParameters;
   typedef typename TypeTraits::ExtendedMapReplaceParameters ExtendedMapReplaceParameters;

  private:
   static const bool HasCopyHandlerOnSorted = TypeTraits::HasCopyHandlerOnSorted;
   static const bool HasCopyHandlerOnSingle = TypeTraits::HasCopyHandlerOnSingle;
   static const bool HasCopyHandlerOnMultiple = TypeTraits::HasCopyHandlerOnMultiple;

  public:
   typedef typename TypeTraits::UniqueBaseCast UniqueBaseCast;
   typedef typename TypeTraits::UniqueDynamicBaseCast UniqueDynamicBaseCast;
   typedef typename TypeTraits::SingleBaseCast SingleBaseCast;
   typedef typename TypeTraits::SingleDynamicBaseCast SingleDynamicBaseCast;
   typedef typename TypeTraits::MultipleBaseCast MultipleBaseCast;
   typedef typename TypeTraits::MultipleDynamicBaseCast MultipleDynamicBaseCast;

   typedef typename TypeTraits::SortedCollection SortedCollection;
   typedef typename TypeTraits::MultipleCollection MultipleCollection;
   typedef typename TypeTraits::CommonElement CommonElement;
   typedef typename TypeTraits::UniqueElement UniqueElement;
   typedef typename TypeTraits::SingleElement SingleElement;
   typedef typename TypeTraits::MultipleElement MultipleElement;
   typedef typename TypeTraits::ReceiverSingleElement ReceiverSingleElement;
   typedef typename TypeTraits::ReceiverMultipleElement ReceiverMultipleElement;
   typedef typename TypeTraits::SortedCollection::Key Key;
   typedef typename TypeTraits::BaseCast BaseCast;
   typedef CommonElement Node;

   class EConflictKeyError {};
   typedef VirtualCollection::RelativePosition RelativePosition;
   static const RelativePosition RPUndefined = VirtualCollection::RPUndefined;
   static const RelativePosition RPBefore    = VirtualCollection::RPBefore;
   static const RelativePosition RPExact     = VirtualCollection::RPExact;
   static const RelativePosition RPAfter     = VirtualCollection::RPAfter;

   typedef VirtualCollection::AddMode AddMode;
   static const AddMode AMNoDuplicate        = VirtualCollection::AMNoDuplicate;
   static const AddMode AMDuplicate          = VirtualCollection::AMDuplicate;
   typedef VirtualCollection::RemoveMode RemoveMode;
   static const RemoveMode RMRemove          = VirtualCollection::RMRemove;
   static const RemoveMode RMFree            = VirtualCollection::RMFree;

   static const bool HasNativeCopy = (HasCopyHandlerOnSorted && HasCopyHandlerOnSingle && HasCopyHandlerOnMultiple);

   typedef typename TypeTraits::RetrieveUniqueRegistrationFromCopyCast RetrieveUniqueRegistrationFromCopyCast;
   typedef typename TypeTraits::RetrieveMultipleRegistrationFromCopyCast RetrieveMultipleRegistrationFromCopyCast;
   typedef typename TypeTraits::RetrieveSingleRegistrationFromCopyCast RetrieveSingleRegistrationFromCopyCast;

#if defined(__GNUC__) && GCC_VERSION >= 40600
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Woverloaded-virtual"
#endif
   class RetrieveRegistrationFromCopyCast : public VirtualCastWithElement {
     private:
      static const VirtualCastWithElement& getInterface(const VirtualCast& source)
         {  return (const VirtualCastWithElement&) source; }

     protected:
      virtual EnhancedObject* _castFrom(EnhancedObject* copyObject, EnhancedObject* sourceCollectionObject) const
         {  EnhancedObject* result = nullptr;
            IdentificationElement identification = TypeTraits::identifyElementFromCopyHandler(*sourceCollectionObject);
            if (identification == IEUnique) {
               typename SortedCollection::Node* sresult = (typename SortedCollection::Node*)
                  SortedCollection::Node::castFromCopyHandler(
                     getInterface(RetrieveUniqueRegistrationFromCopyCast()).castFrom(copyObject, sourceCollectionObject));
               AssumeCondition(dynamic_cast<UniqueElement*>(sresult))
               result = SortedCollection::Node::castToCopyHandler(sresult);
            }
            else if (identification == IESingle) {
               SingleElement* sresult = (SingleElement*) SingleElement::castFromCopyHandler(
                     getInterface(RetrieveSingleRegistrationFromCopyCast()).castFrom(copyObject, sourceCollectionObject));
               result = SingleElement::castToCopyHandler(sresult);
            }
            else {
               AssumeCondition(identification == IEMultiple)
               typename MultipleCollection::Node* sresult = (typename MultipleCollection::Node*)
                  MultipleCollection::Node::castFromCopyHandler(
                     getInterface(RetrieveMultipleRegistrationFromCopyCast()).castFrom(copyObject, sourceCollectionObject));
               AssumeCondition(dynamic_cast<MultipleElement*>(sresult))
               result = MultipleCollection::Node::castToCopyHandler(sresult);
            };
            return result;
         }
      virtual EnhancedObject* _castTo(EnhancedObject* collectionObject, EnhancedObject* sourceCopyObject) const
         {  EnhancedObject* result = nullptr;
            IdentificationElement identification = TypeTraits::identifyElementFromCopyHandler(*collectionObject);
            if (identification == IEUnique)
               result = getInterface(RetrieveUniqueRegistrationFromCopyCast()).castTo(collectionObject, sourceCopyObject);
            else if (identification == IEMultiple)
               result = getInterface(RetrieveMultipleRegistrationFromCopyCast()).castTo(collectionObject, sourceCopyObject);
            else {
               AssumeCondition(identification == IESingle)
               result = getInterface(RetrieveSingleRegistrationFromCopyCast()).castTo(collectionObject);
            };
            return result;
         }
      
     public:
      RetrieveRegistrationFromCopyCast() { setCastWithElement(); }
      RetrieveRegistrationFromCopyCast(const RetrieveRegistrationFromCopyCast& source)
         :  VirtualCastWithElement(source) {}
   };

   class RetrieveSortedRegistrationFromCopyCast : public VirtualCastWithElement {
     private:
      static const VirtualCastWithElement& getInterface(const VirtualCast& source)
         {  return (const VirtualCastWithElement&) source; }
      virtual EnhancedObject* _castFrom(EnhancedObject* copyObject) const
         {  return VirtualCastWithElement::_castFrom(copyObject); }
      virtual EnhancedObject* _castTo(EnhancedObject* collectionObject) const
         {  return VirtualCastWithElement::_castTo(collectionObject); }

     protected:
      virtual EnhancedObject* _castFrom(EnhancedObject* copyObject, EnhancedObject* sourceCollectionObject) const
         {  EnhancedObject* result = copyObject;
            IdentificationElement identification = TypeTraits::identifyElementFromCopyHandler(*sourceCollectionObject);
            if (identification == IEUnique) {
               typename SortedCollection::Node* sresult = (typename SortedCollection::Node*)
                  SortedCollection::Node::castFromCopyHandler(
                     getInterface(RetrieveUniqueRegistrationFromCopyCast()).castFrom(copyObject, sourceCollectionObject));
               AssumeCondition(dynamic_cast<UniqueElement*>(sresult))
               result = SortedCollection::Node::castToCopyHandler(
                     (typename SortedCollection::Node*) (UniqueElement*) sresult);
            };
            return result;
         }
      virtual EnhancedObject* _castTo(EnhancedObject* collectionObject, EnhancedObject* sourceCopyObject) const
         {  EnhancedObject* result = collectionObject;
            IdentificationElement identification = TypeTraits::identifyElementFromCopyHandler(*collectionObject);
            if (identification == IEUnique)
               result = getInterface(RetrieveUniqueRegistrationFromCopyCast()).castTo(collectionObject, sourceCopyObject);
            return result;
         }
      
     public:
      RetrieveSortedRegistrationFromCopyCast() { setCastWithElement(); }
      RetrieveSortedRegistrationFromCopyCast(const RetrieveSortedRegistrationFromCopyCast& source)
         :  VirtualCastWithElement(source) {}
   };

   class RetrieveAssistedRegistrationFromCopyCast : public VirtualCastWithElement {
     private:
      const VirtualCastWithElement& vcweRetrieveCommonFromCopy;

     protected:
      virtual EnhancedObject* _castFrom(EnhancedObject* copyObject, EnhancedObject* sourceCollectionObject) const
         {  EnhancedObject* result = nullptr;
            if (!vcweRetrieveCommonFromCopy.isCastWithElement()) {
               CommonElement* common = (CommonElement*) CommonElement::castFromCopyHandler(
                     vcweRetrieveCommonFromCopy.castFrom(copyObject));
               IdentificationElement identification = TypeTraits::identifyElementFromCommon(*common);
               if (identification == IEUnique)
                  result = SortedCollection::Node::castToCopyHandler(
                     (typename SortedCollection::Node*) (UniqueElement*) UniqueBaseCast::castFrom(common));
               else if (identification == IEMultiple)
                  result = MultipleCollection::Node::castToCopyHandler(
                     (typename MultipleCollection::Node*) (MultipleElement*) MultipleBaseCast::castFrom(common));
               else {
                  AssumeCondition(identification == IESingle)
                  result = SingleElement::castToCopyHandler((SingleElement*) SingleBaseCast::castFrom(common));
               };
            }
            else {
               IdentificationElement identification = TypeTraits::identifyElementFromCopyHandler(*sourceCollectionObject);
               if (identification == IEUnique) {
                  UniqueElement* reference = (UniqueElement*) (typename SortedCollection::Node*)
                        SortedCollection::Node::castFromCopyHandler(sourceCollectionObject);
                  UniqueElement* sresult = (UniqueElement*) UniqueBaseCast::castFrom(
                     (CommonElement*) CommonElement::castFromCopyHandler(
                        vcweRetrieveCommonFromCopy.castFrom(copyObject,
                           CommonElement::castToCopyHandler((CommonElement*) UniqueBaseCast::castTo(reference)))));
                  result = SortedCollection::Node::castToCopyHandler((typename SortedCollection::Node*) sresult);
               }
               else if (identification == IESingle) {
                  SingleElement* reference = (SingleElement*)
                        SingleElement::castFromCopyHandler(sourceCollectionObject);
                  SingleElement* sresult = (SingleElement*) SingleBaseCast::castFrom(
                     (CommonElement*) CommonElement::castFromCopyHandler(
                        vcweRetrieveCommonFromCopy.castFrom(copyObject,
                           CommonElement::castToCopyHandler((CommonElement*) SingleBaseCast::castTo(reference)))));
                  result = SingleElement::castToCopyHandler(sresult);
               }
               else {
                  AssumeCondition(identification == IEMultiple)
                  MultipleElement* reference = (MultipleElement*) ((typename MultipleCollection::Node*)
                     MultipleCollection::Node::castFromCopyHandler(sourceCollectionObject));
                  MultipleElement* sresult = (MultipleElement*) MultipleBaseCast::castFrom(
                     (CommonElement*) CommonElement::castFromCopyHandler(
                        vcweRetrieveCommonFromCopy.castFrom(copyObject,
                           CommonElement::castToCopyHandler((CommonElement*) MultipleBaseCast::castTo(reference)))));
                  result = MultipleCollection::Node::castToCopyHandler((typename MultipleCollection::Node*) sresult);
               };
            };
            return result;
         }
      virtual EnhancedObject* _castTo(EnhancedObject* collectionObject, EnhancedObject* sourceCopyObject) const
         {  EnhancedObject* result = nullptr;
            IdentificationElement identification = TypeTraits::identifyElementFromCopyHandler(*collectionObject);
            if (identification == IEUnique) {
               UniqueElement* sresult = (UniqueElement*) (typename SortedCollection::Node*)
                     SortedCollection::Node::castFromCopyHandler(collectionObject);
               result = vcweRetrieveCommonFromCopy.castTo(CommonElement::castToCopyHandler(
                     (CommonElement*) UniqueBaseCast::castTo(sresult)), sourceCopyObject);
            }
            else if (identification == IEMultiple) {
               MultipleElement* sresult = (MultipleElement*) (typename MultipleCollection::Node*)
                     MultipleCollection::Node::castFromCopyHandler(collectionObject);
               result = vcweRetrieveCommonFromCopy.castTo(CommonElement::castToCopyHandler(
                      (CommonElement*) MultipleBaseCast::castTo(sresult)), sourceCopyObject);
            }
            else {
               AssumeCondition(identification == IESingle)
               SingleElement* sresult = (SingleElement*)
                     SingleElement::castFromCopyHandler(collectionObject);
               result = vcweRetrieveCommonFromCopy.castTo(CommonElement::castToCopyHandler(
                        (CommonElement*) SingleBaseCast::castTo(sresult)), sourceCopyObject);
            };
            return result;
         }
      
     public:
      RetrieveAssistedRegistrationFromCopyCast(const VirtualCast& retrieveCommonFromCopy)
         :  vcweRetrieveCommonFromCopy((const VirtualCastWithElement&) retrieveCommonFromCopy)
         {  setCastWithElement(); }
      RetrieveAssistedRegistrationFromCopyCast(const RetrieveAssistedRegistrationFromCopyCast& source) = default;
   };

   class RetrieveSortedAssistedRegistrationFromCopyCast : public VirtualCastWithElement {
     private:
      const VirtualCastWithElement& vcweRetrieveCommonFromCopy;

      virtual EnhancedObject* _castFrom(EnhancedObject* copyObject) const
         {  return VirtualCastWithElement::_castFrom(copyObject); }
      virtual EnhancedObject* _castTo(EnhancedObject* collectionObject) const
         {  return VirtualCastWithElement::_castTo(collectionObject); }

     protected:
      virtual EnhancedObject* _castFrom(EnhancedObject* copyObject, EnhancedObject* sourceCollectionObject) const
         {  EnhancedObject* result = copyObject;
            typename SortedCollection::Node* reference = (typename SortedCollection::Node*)
               SortedCollection::Node::castFromCopyHandler(sourceCollectionObject);
            IdentificationElement identification = TypeTraits::identifyElementFromSorted(*reference);
            if (identification == IEUnique) {
               UniqueElement* unique = (UniqueElement*) UniqueBaseCast::castFrom((CommonElement*)
                  CommonElement::castFromCopyHandler(vcweRetrieveCommonFromCopy.castFrom(copyObject,
                     CommonElement::castToCopyHandler(UniqueBaseCast::castTo((UniqueElement*) reference)))));
               result = SortedCollection::Node::castToCopyHandler(
                     (typename SortedCollection::Node*) unique);
            };
            return result;
         }
      virtual EnhancedObject* _castTo(EnhancedObject* collectionObject, EnhancedObject* sourceCopyObject) const
         {  EnhancedObject* result = collectionObject;
            typename SortedCollection::Node* sresult = (typename SortedCollection::Node*)
               SortedCollection::Node::castFromCopyHandler(collectionObject);
            IdentificationElement identification = TypeTraits::identifyElementFromSorted(*sresult);
            if (identification == IEUnique)
               result = vcweRetrieveCommonFromCopy.castTo(CommonElement::castToCopyHandler(
                        (CommonElement*) UniqueBaseCast::castTo((UniqueElement*) sresult)),
                     sourceCopyObject);
            return result;
         }
      
     public:
      RetrieveSortedAssistedRegistrationFromCopyCast(const VirtualCast& retrieveCommonFromCopy)
         : vcweRetrieveCommonFromCopy((const VirtualCastWithElement&) retrieveCommonFromCopy)
         {  setCastWithElement(); }
      RetrieveSortedAssistedRegistrationFromCopyCast(const RetrieveSortedAssistedRegistrationFromCopyCast& source) = default;
   };

   class RetrieveUniqueAssistedRegistrationFromCopyCast : public VirtualCastWithElement {
     private:
      const VirtualCastWithElement& vcweRetrieveCommonFromCopy;

     protected:
      virtual EnhancedObject* _castFrom(EnhancedObject* copyObject, EnhancedObject* sourceCollectionObject) const
         {  UniqueElement* unique = (UniqueElement*) UniqueBaseCast::castFrom((CommonElement*)
               CommonElement::castFromCopyHandler(vcweRetrieveCommonFromCopy.castFrom(copyObject,
                  CommonElement::castToCopyHandler((CommonElement*)
                     UniqueBaseCast::castTo((UniqueElement*) (typename SortedCollection::Node*)
                        SortedCollection::Node::castFromCopyHandler(sourceCollectionObject))))));
            AssumeCondition(UniqueDynamicBaseCast::castFrom(UniqueBaseCast::castTo(unique)))
            return SortedCollection::Node::castToCopyHandler(
                  (typename SortedCollection::Node*) unique);
         }
      virtual EnhancedObject* _castTo(EnhancedObject* collectionObject, EnhancedObject* sourceCopyObject) const
         {  typename SortedCollection::Node* result = (typename SortedCollection::Node*)
               SortedCollection::Node::castFromCopyHandler(collectionObject);
            AssumeCondition(dynamic_cast<UniqueElement*>(result))
            return vcweRetrieveCommonFromCopy.castTo(CommonElement::castToCopyHandler((CommonElement*)
                  UniqueBaseCast::castTo((UniqueElement*) result)), sourceCopyObject);
         }
      virtual EnhancedObject* _castFrom(EnhancedObject* copyObject) const
         {  UniqueElement* unique = (UniqueElement*) UniqueBaseCast::castFrom((CommonElement*)
               CommonElement::castFromCopyHandler(vcweRetrieveCommonFromCopy.castFrom(copyObject)));
            AssumeCondition(dynamic_cast<UniqueElement*>(unique))
            return SortedCollection::Node::castToCopyHandler(
               (typename SortedCollection::Node*) unique);
         }
      virtual EnhancedObject* _castTo(EnhancedObject* collectionObject) const
         {  typename SortedCollection::Node* result = (typename SortedCollection::Node*)
               SortedCollection::Node::castFromCopyHandler(collectionObject);
            AssumeCondition(dynamic_cast<UniqueElement*>(result))
            return vcweRetrieveCommonFromCopy.castTo(CommonElement::castToCopyHandler((CommonElement*)
                  UniqueBaseCast::castTo((UniqueElement*) result)));
         }
      
     public:
      RetrieveUniqueAssistedRegistrationFromCopyCast(const VirtualCast& retrieveCommonFromCopy)
         :  vcweRetrieveCommonFromCopy((const VirtualCastWithElement&) retrieveCommonFromCopy)
         {  if (vcweRetrieveCommonFromCopy.isCastWithElement())
               setCastWithElement();
         }
      RetrieveUniqueAssistedRegistrationFromCopyCast(const RetrieveUniqueAssistedRegistrationFromCopyCast& source) = default;
   };

   class RetrieveMultipleAssistedRegistrationFromCopyCast : public VirtualCastWithElement {
     private:
      const VirtualCastWithElement& vcweRetrieveCommonFromCopy;

     protected:
      virtual EnhancedObject* _castFrom(EnhancedObject* copyObject, EnhancedObject* collectionObject) const
         {  MultipleElement* multiple = (MultipleElement*) MultipleBaseCast::castFrom((CommonElement*)
               CommonElement::castFromCopyHandler(vcweRetrieveCommonFromCopy.castFrom(copyObject,
                  CommonElement::castToCopyHandler((CommonElement*)
                     MultipleBaseCast::castTo((MultipleElement*) (typename MultipleCollection::Node*)
                        MultipleCollection::Node::castFromCopyHandler(collectionObject))))));
            AssumeCondition(MultipleDynamicBaseCast::castFrom(MultipleBaseCast::castTo(multiple)))
            return MultipleCollection::Node::castToCopyHandler(
                  (typename MultipleCollection::Node*) multiple);
         }
      virtual EnhancedObject* _castTo(EnhancedObject* collectionObject, EnhancedObject* sourceCopyObject) const
         {  typename MultipleCollection::Node* result = (typename MultipleCollection::Node*)
               MultipleCollection::Node::castFromCopyHandler(collectionObject);
            AssumeCondition(dynamic_cast<MultipleElement*>(result))
            return vcweRetrieveCommonFromCopy.castTo(CommonElement::castToCopyHandler((CommonElement*)
                     MultipleBaseCast::castTo((MultipleElement*) result)), sourceCopyObject);
         }
      virtual EnhancedObject* _castFrom(EnhancedObject* copyObject) const
         {  MultipleElement* multiple = (MultipleElement*) MultipleBaseCast::castFrom((CommonElement*)
               CommonElement::castFromCopyHandler(vcweRetrieveCommonFromCopy.castFrom(copyObject)));
            AssumeCondition(MultipleDynamicBaseCast::castFrom(MultipleBaseCast::castTo(multiple)))
            return MultipleCollection::Node::castToCopyHandler(
               (typename MultipleCollection::Node*) multiple);
         }
      virtual EnhancedObject* _castTo(EnhancedObject* collectionObject) const
         {  typename MultipleCollection::Node* result = (typename MultipleCollection::Node*)
               MultipleCollection::Node::castFromCopyHandler(collectionObject);
            AssumeCondition(dynamic_cast<MultipleElement*>(result))
            return vcweRetrieveCommonFromCopy.castTo(CommonElement::castToCopyHandler((CommonElement*)
                     MultipleBaseCast::castTo((MultipleElement*) result)));
         }
      
     public:
      RetrieveMultipleAssistedRegistrationFromCopyCast(const VirtualCast& retrieveCommonFromCopy)
         :  vcweRetrieveCommonFromCopy((const VirtualCastWithElement&) retrieveCommonFromCopy)
         {  if (vcweRetrieveCommonFromCopy.isCastWithElement())
               setCastWithElement();
         }
      RetrieveMultipleAssistedRegistrationFromCopyCast(const RetrieveMultipleAssistedRegistrationFromCopyCast& source) = default;
   };

   class RetrieveSingleAssistedRegistrationFromCopyCast : public VirtualCastWithElement {
     private:
      const VirtualCastWithElement& vcweRetrieveCommonFromCopy;

     protected:
      virtual EnhancedObject* _castFrom(EnhancedObject* copyObject, EnhancedObject* sourceCollectionObject) const
         {  SingleElement* single = (SingleElement*) SingleBaseCast::castFrom((CommonElement*)
               CommonElement::castFromCopyHandler(vcweRetrieveCommonFromCopy.castFrom(copyObject,
                  CommonElement::castToCopyHandler((CommonElement*)
                     SingleBaseCast::castTo((SingleElement*)
                        SingleElement::castFromCopyHandler(sourceCollectionObject))))));
            AssumeCondition(SingleDynamicBaseCast::castFrom(SingleBaseCast::castTo(single)))
            return SingleElement::castToCopyHandler(single);
         }
      virtual EnhancedObject* _castTo(EnhancedObject* collectionObject, EnhancedObject* sourceCopyObject) const
         {  SingleElement* result = (SingleElement*) SingleElement::castFromCopyHandler(collectionObject);
            AssumeCondition(SingleDynamicBaseCast::castFrom(SingleBaseCast::castTo(result)))
            return vcweRetrieveCommonFromCopy.castTo(CommonElement::castToCopyHandler(
                     (CommonElement*) SingleBaseCast::castTo(result)), sourceCopyObject);
         }
      virtual EnhancedObject* _castFrom(EnhancedObject* copyObject) const
         {  SingleElement* single = (SingleElement*) SingleBaseCast::castFrom((CommonElement*)
               CommonElement::castFromCopyHandler(vcweRetrieveCommonFromCopy.castFrom(copyObject)));
            AssumeCondition(SingleDynamicBaseCast::castFrom(SingleBaseCast::castTo(single)))
            return SingleElement::castToCopyHandler(single);
         }
      virtual EnhancedObject* _castTo(EnhancedObject* collectionObject) const
         {  SingleElement* result = (SingleElement*) SingleElement::castFromCopyHandler(collectionObject);
            AssumeCondition(SingleDynamicBaseCast::castFrom(SingleBaseCast::castTo(result)))
            return vcweRetrieveCommonFromCopy.castTo(CommonElement::castToCopyHandler(
                     (CommonElement*) SingleBaseCast::castTo(result)));
         }
      
     public:
      RetrieveSingleAssistedRegistrationFromCopyCast(const VirtualCast& retrieveCommonFromCopy)
         :  vcweRetrieveCommonFromCopy((const VirtualCastWithElement&) retrieveCommonFromCopy)
         {  if (vcweRetrieveCommonFromCopy.isCastWithElement())
               setCastWithElement();
         }
      RetrieveSingleAssistedRegistrationFromCopyCast(const RetrieveSingleAssistedRegistrationFromCopyCast& source) = default;
   };
#if defined(__GNUC__) && GCC_VERSION >= 40600
#pragma GCC diagnostic pop
#endif

  private:
   typedef TMultipleSortedCollection<TypeTraits> thisType;
   typedef VirtualMultiSortedCollection inherited;

   typedef HandlerLinkedCast<UniqueElement, UniqueBaseCast, BaseCast> CompleteUniqueBaseCast;
   typedef HandlerLinkedCast<SingleElement, SingleBaseCast, BaseCast> CompleteSingleBaseCast;
   typedef HandlerLinkedCast<MultipleElement, MultipleBaseCast, BaseCast> CompleteMultipleBaseCast;

   static ComparisonResult getComparison(RelativePosition pos)
      {  return (pos == RPBefore) ? CRGreater : CRLess; }
   static ComparisonResult getComparison(const ExtendedLocateParameters& params)
      {  return getComparison(params.getRelativePosition()); }
   static ComparisonResult inverse(ComparisonResult compare)
      {  return (compare == CRGreater) ? CRLess : CRGreater; }
   static RelativePosition inverse(RelativePosition pos)
      {  return (pos == COL::VirtualCollection::RPBefore)
            ? COL::VirtualCollection::RPAfter
            : COL::VirtualCollection::RPBefore;
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
                  if (!HasCopyHandlerOnSorted)
                     scSortedCollection.fullAssign(source.scSortedCollection, DVirtualCollection::TCastParameters
                        <ExtendedReplaceParameters>().setExternCast(RetrieveSortedRegistrationFromCopyCast()).setDuplicate());
                  else
                     scSortedCollection.fullAssign(source.scSortedCollection,
                           ExtendedReplaceParameters().setDuplicate());
                  RetrieveMultipleRegistrationFromCopyCast retrieveMultipleRegistrationFromCopyCast;
                  RetrieveSingleRegistrationFromCopyCast retrieveSingleRegistrationFromCopyCast;
                  correctDuplicationCopyReceiver(source, &(const VirtualCast&) retrieveMultipleRegistrationFromCopyCast,
                        &(const VirtualCast&) retrieveSingleRegistrationFromCopyCast);
               };
            }
            else {
               scSortedCollection.fullAssign(source.scSortedCollection,
                     DVirtualCollection::TCastParameters<ExtendedReplaceParameters>()
                     .setExternCast(RetrieveSortedAssistedRegistrationFromCopyCast(*retrieveCommonFromCopy))
                     .setDuplicate());
               RetrieveMultipleAssistedRegistrationFromCopyCast retrieveMultipleAssistedRegistrationFromCopyCast(*retrieveCommonFromCopy);
               RetrieveSingleAssistedRegistrationFromCopyCast retrieveSingleAssistedRegistrationFromCopyCast(*retrieveCommonFromCopy);
               correctDuplicationCopyReceiver(source,
                     &(const VirtualCast&) retrieveMultipleAssistedRegistrationFromCopyCast,
                     &(const VirtualCast&) retrieveSingleAssistedRegistrationFromCopyCast);
            };
         };
         uCount = source.uCount;
      }
   void addBody(const thisType& source, const ExtendedInsertionParameters& parameters,
         const VirtualCast* retrieveCommonFromCopy, typename SortedCollection::Cursor* cursor,
         const typename SortedCollection::Cursor* start, const typename SortedCollection::Cursor* end,
         VirtualCollection::RelativePosition direction=RPAfter, int addReceiver = -1)
      {  if (!parameters.isDuplicate()) {
            scSortedCollection.addAll(source.scSortedCollection, parameters, cursor,
                  start, end);
            correctDuplicationSplitReceiver(cursor, direction, addReceiver, parameters.isRemote());
         }
         else if (!retrieveCommonFromCopy) {
            if (HasNativeCopy) {
               scSortedCollection.addAll(source.scSortedCollection, parameters, cursor,
                     start, end);
               correctDuplicationCopyReceiver(source, nullptr, nullptr, cursor, start, end,
                     direction, parameters.isRemote());
            }
            else {
               if (!HasCopyHandlerOnSorted)
                  scSortedCollection.addAll(source.scSortedCollection, DVirtualCollection::TCastParameters
                     <ExtendedInsertionParameters>(parameters).setExternCast(RetrieveSortedRegistrationFromCopyCast()),
                     cursor, start, end);
               else
                  scSortedCollection.addAll(source.scSortedCollection, parameters, cursor,
                        start, end);
               RetrieveMultipleRegistrationFromCopyCast retrieveMultipleRegistrationFromCopyCast;
               RetrieveSingleRegistrationFromCopyCast retrieveSingleRegistrationFromCopyCast;
               correctDuplicationCopyReceiver(source, &retrieveMultipleRegistrationFromCopyCast,
                     &retrieveSingleRegistrationFromCopyCast, cursor, start, end,
                     direction, parameters.isRemote());
            };
         }
         else {
            scSortedCollection.addAll(source.scSortedCollection,
                  DVirtualCollection::TCastParameters<ExtendedInsertionParameters>(parameters)
                  .setExternCast(RetrieveSortedAssistedRegistrationFromCopyCast(*retrieveCommonFromCopy)),
                  cursor, start, end);
            RetrieveMultipleAssistedRegistrationFromCopyCast retrieveMultipleAssistedRegistrationFromCopyCast(*retrieveCommonFromCopy);
            RetrieveSingleAssistedRegistrationFromCopyCast retrieveSingleAssistedRegistrationFromCopyCast(*retrieveCommonFromCopy);
            correctDuplicationCopyReceiver(source, &retrieveMultipleAssistedRegistrationFromCopyCast,
                  &retrieveSingleAssistedRegistrationFromCopyCast, cursor,
                  start, end, direction, parameters.isRemote());
         };
      }

  public:
   TMultipleSortedCollection() : scSortedCollection(), uCount(0) {}
   TMultipleSortedCollection(const COL::VirtualCollection::InitialValues& initialValues)
      :  uCount(0) {}
   TMultipleSortedCollection(const COL::VirtualCollection::InitialNewValues& initialValues)
      :  uCount(0) {}
   TMultipleSortedCollection(const thisType& source, AddMode mode=AMNoDuplicate)
      :  inherited(source, mode), uCount(source.uCount)
      {  if (HasCopyHandlerOnSorted || (mode == AMNoDuplicate))
            scSortedCollection.fullAssign(source.scSortedCollection,
                  COL::VirtualCollection::ExtendedReplaceParameters().setDuplicate(mode));
         if (source.count() > 0) {
            if (mode == AMNoDuplicate)
               correctDuplicationSplitReceiver();
            else if (HasNativeCopy)
               correctDuplicationCopyReceiver(source);
            else {
               if (!HasCopyHandlerOnSorted)
                  scSortedCollection.fullAssign(source.scSortedCollection, DVirtualCollection::TCastParameters
                     <ExtendedReplaceParameters>().setExternCast(RetrieveSortedRegistrationFromCopyCast()).setDuplicate());
               RetrieveMultipleRegistrationFromCopyCast retrieveMultipleRegistrationFromCopyCast;
               RetrieveSingleRegistrationFromCopyCast retrieveSingleRegistrationFromCopyCast;
               correctDuplicationCopyReceiver(source, &((const VirtualCast&) retrieveMultipleRegistrationFromCopyCast),
                     &((const VirtualCast&) retrieveSingleRegistrationFromCopyCast));
            };
         };
      }
   TMultipleSortedCollection(thisType&& source)
      :  inherited(source), uCount(source.uCount)
      {  scSortedCollection.swap(source.scSortedCollection);
         source.uCount = 0;
      }
   TMultipleSortedCollection(const thisType& source, AddMode mode, const VirtualCast* retrieveCommonFromCopy)
      :  uCount(0)
      {  duplicateBody(source, mode, retrieveCommonFromCopy); }
   virtual ~TMultipleSortedCollection()
      {  if ((uCount != 0) && (scSortedCollection.count() != 0))
            _removeAll(ExtendedSuppressParameters());
      }
   TemplateDefineCopy(TMultipleSortedCollection, TypeTraits)
   DTemplateDefineAssign(TMultipleSortedCollection, TypeTraits)
   thisType& operator=(const thisType& source)
      {  if (this != &source) {
            if ((source.uCount != 0) && (source.scSortedCollection.count() != 0))
               source._removeAll(ExtendedSuppressParameters());
         };
         scSortedCollection.fullAssign(source.scSortedCollection,
               COL::VirtualCollection::ExtendedReplaceParameters());
         if (source.count() > 0)
            correctDuplicationSplitReceiver();
      }
   thisType& operator=(thisType&& source)
      {  scSortedCollection.swap(source.scSortedCollection);
         if (this != &source)
            source.uCount = 0;
      }
   bool invariant() const
      {  int leftCount = uCount;
         return scSortedCollection.foreachDo([&leftCount](const typename SortedCollection::Node& node)
            {  bool result = true;
               IdentificationElement identification = TypeTraits::identifyElementFromSorted(node);
               if (identification == IEReceiverMultiple) {
                  leftCount -= ((const ReceiverMultipleElement&) node).count();
                  result = ((const ReceiverMultipleElement&) node).isValid();
               }
               else if (identification == IEReceiverSingle) {
                  leftCount--;
                  result = ((const ReceiverSingleElement&) node).isValid();
               }
               else {
                  leftCount--;
                  result = dynamic_cast<const UniqueElement*>(&node);
               };
               return result;
            }) && (leftCount == 0);
      }

   TemplateDefineCollectionForAbstractCollect(TMultipleSortedCollection, TMultipleSortedCollectionCursor, TypeTraits)

  private:
   class CountReceiver {
     private:
      int uCount;
      int uElements;

     public:
      CountReceiver() : uCount(0), uElements(0) {}

      bool operator()(const typename SortedCollection::Node& node)
         {  IdentificationElement identification = TypeTraits::identifyElementFromSorted(node);
            if ((identification == IEReceiverSingle) || (identification == IEReceiverMultiple)) {
               ++uCount;
               uElements += (identification == IEReceiverMultiple)
                  ? ((const ReceiverMultipleElement&) node).count() : 1;
            }
            else
               ++uElements;
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
         void add(ReceiverSingleElement& element)
            {  element.extractElement();
               inherited::add(&element);
            }
      };

      FreeArray  faFreeNodes;

     public:
      FreeReceiver(int allocation) : faFreeNodes(allocation) {}
      bool operator()(typename SortedCollection::Node& node)
         {  IdentificationElement identification = TypeTraits::identifyElementFromSorted(node);
            if (identification == IEReceiverMultiple)
               faFreeNodes.add((ReceiverMultipleElement&) node);
            else if (identification == IEReceiverSingle)
               faFreeNodes.add((ReceiverSingleElement&) node);
            return true;
         }
      void freeAll() { faFreeNodes.freeAll(); }
   };

   void correctDuplicationSplitReceiver(const typename SortedCollection::Cursor* cursor=nullptr,
         VirtualCollection::RelativePosition direction=RPAfter, int addReceiver = -1,
         bool isFirstExcluded=false);
   void correctDuplicationCopyReceiver(const thisType& source,
         const VirtualCast* retrieveMultipleRegistrationFromCopy=nullptr,
         const VirtualCast* retrieveSingleRegistrationFromCopy=nullptr,
         const typename SortedCollection::Cursor* cursor=nullptr,
         const typename SortedCollection::Cursor* sourceStart=nullptr,
         const typename SortedCollection::Cursor* sourceEnd=nullptr,
         VirtualCollection::RelativePosition direction=RPAfter, bool isFirstExcluded=false);

  protected:
   const SortedCollection& getSortedCollection() const { return scSortedCollection; }
   friend class TMultipleSortedCollectionCursor<TypeTraits>;

  protected:
   virtual void _fullAssign(const VirtualCollection& source, const ExtendedReplaceParameters& parameters)
      {  inherited::pfullAssign(source, parameters); }
   void _fullAssign(const thisType& source, const ExtendedReplaceParameters& parameters);

   virtual void _add(EnhancedObject* anObject, const ExtendedInsertionParameters& parameters,
         VirtualCollectionCursor* cursor=nullptr); // dispatch method
   void _add(UniqueElement* element, const ExtendedInsertionParameters& parameters,
         Cursor* cursor=nullptr);
   void _add(MultipleElement* element, const ExtendedInsertionParameters& parameters,
         Cursor* cursor=nullptr);
   void _add(typename Key::KeyType key, UniqueElement* element, const ExtendedInsertionParameters& parameters,
         Cursor* cursor=nullptr);
   void _add(typename Key::KeyType key, SingleElement* element, const ExtendedInsertionParameters& parameters,
         Cursor* cursor=nullptr);
   void _add(typename Key::KeyType key, MultipleElement* element, const ExtendedInsertionParameters& parameters,
         Cursor* cursor=nullptr);

   void paddAll(const thisType& source, const ExtendedInsertionParameters& parameters,
         Cursor* cursor, const Cursor* startSource, const Cursor* endSource);
   virtual void _addAll(const VirtualCollection& source, const ExtendedInsertionParameters& parameters,
         VirtualCollectionCursor* cursor, const VirtualCollectionCursor* startSource,
         const VirtualCollectionCursor* endSource)
      {  inherited::paddAll(source, parameters, cursor, startSource, endSource); }
   void _addAll(const thisType& source, const ExtendedInsertionParameters& parameters,
         Cursor* cursor=nullptr, const Cursor* startSource=nullptr, const Cursor* endSource=nullptr);
   virtual void _moveTo(VirtualCollection& destination, const ExtendedReplaceParameters& parameters,
         VirtualCollectionCursor* cursor, VirtualCollectionCursor* destinationCursor)
      {  inherited::pmoveTo(destination, parameters, cursor, destinationCursor); }
   void _moveTo(thisType& destination, const ExtendedReplaceParameters& parameters,
         Cursor* cursor=nullptr, Cursor* destinationCursor=nullptr);
   virtual void _moveAllTo(VirtualCollection& destination, const ExtendedReplaceParameters& parameters,
         const VirtualCollectionCursor* startCursor, const VirtualCollectionCursor* endCursor,
         VirtualCollectionCursor* destinationCursor)
      {  inherited::pmoveAllTo(destination, parameters, startCursor, endCursor, destinationCursor); }
   void _moveAllTo(thisType& destination, const ExtendedReplaceParameters& parameters,
         const Cursor* startCursor=nullptr, const Cursor* endCursor=nullptr,
         Cursor* destinationCursor=nullptr);
   virtual void _remove(const ExtendedSuppressParameters& parameters, VirtualCollectionCursor* cursor)
      {  _remove(parameters, (Cursor*) cursor); }
   void _remove(const ExtendedSuppressParameters& parameters, Cursor* cursor=nullptr);
   virtual void _removeAll(const ExtendedSuppressParameters& parameters,
         const VirtualCollectionCursor* start, const VirtualCollectionCursor* end)
      {  _removeAll(parameters, (const Cursor*) start, (const Cursor*) end); }
   void _removeAll(const ExtendedSuppressParameters& parameters,
         const Cursor* start=nullptr, const Cursor* end=nullptr);
   virtual void _replace(EnhancedObject* anObject,
         const ExtendedReplaceParameters& parameters, VirtualCollectionCursor* cursor)
      {  _replace((CommonElement*) BaseCast::castFrom(anObject), parameters, (Cursor*) cursor); }
   void _replace(CommonElement* element, const ExtendedReplaceParameters& parameters,
         Cursor* cursor=nullptr);
   virtual int _queryCount(const ExtendedLocateParameters& parameters,
      const VirtualCollectionCursor* start=nullptr, const VirtualCollectionCursor* end=nullptr) const
      {  return (!start && !end) ? uCount
            : VirtualCollection::pqueryCount(parameters, start, end);
      }
   virtual EnhancedObject* _getElement(const ExtendedLocateParameters& parameters,
         const VirtualCollectionCursor* cursor) const
      {  return BaseCast::castTo(_getElement(parameters, (Cursor*) cursor)); }
   CommonElement* _getElement(const ExtendedLocateParameters& parameters, const Cursor* cursor=nullptr) const;

   virtual LocationResult _locate(const EnhancedObject& source, const ExtendedLocateParameters& parameters,
         VirtualMultiSortedCollectionCursor* cursor, const VirtualMultiSortedCollectionCursor* start,
         const VirtualMultiSortedCollectionCursor* end) const;
   LocationResult _locate(const UniqueElement& source, const ExtendedLocateParameters& parameters,
         Cursor* cursor=nullptr, const Cursor* start=nullptr, const Cursor* end=nullptr) const;
   LocationResult _locateKey(typename Key::KeyType key, const ExtendedLocateParameters& parameters,
         Cursor* cursor=nullptr, const Cursor* start=nullptr, const Cursor* end=nullptr) const;

   typename TypeTraits::KeyTraits::ControlKeyType _queryKey(const ExtendedLocateParameters& parameters,
         const Cursor* cursor=nullptr) const;
   
  private:
   virtual const EnhancedObject& key(const EnhancedObject& element) const { return inherited::key(element); }
   virtual VirtualMultiSortedCollection::LocationResult _locateKey(const EnhancedObject& source, const ExtendedLocateParameters& parameters,
         VirtualMultiSortedCollectionCursor* cursor, const VirtualMultiSortedCollectionCursor* start,
         const VirtualMultiSortedCollectionCursor* end) const
      {  return inherited::_locateKey(source, parameters, cursor, start, end); }

  public:
   typename TypeTraits::KeyTraits::ControlKeyType key(const UniqueElement& element) const
      { return TypeTraits::KeyTraits::store(Key::key(element)); }
   virtual typename TypeTraits::KeyTraits::ControlKeyType key(const SingleElement& element) const { AssumeUncalled }
   virtual typename TypeTraits::KeyTraits::ControlKeyType key(const MultipleElement& element) const { AssumeUncalled }

   #define DefJustDeclare
   #define DefTypeKey typename Key::KeyType

   #define DefTypeElement CommonElement
   #define DefExcludeElement
   #define DefMapInterface
   #define DefTypeControlKey typename TypeTraits::KeyTraits::ControlKeyType
   #include "Collection/ELMCollection.inch"
   #undef DefTypeControlKey
   #undef DefMapInterface
   #undef DefExcludeElement
   #undef DefTypeElement

   #define DefBaseMultipleInterface
   #define DefImplementsNativeMethods

   #define DefTypeElement UniqueElement
   #include "Collection/ELMCollection.inch"
   #undef DefTypeElement
   #define DefTypeElement SingleElement
   #include "Collection/ELMMapCollection.inch"
   #undef DefTypeElement
   #define DefTypeElement MultipleElement
   #include "Collection/ELMMapCollection.inch"
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
            if (start->multipleCursor().isValid()) {
               AssumeCondition(end->multipleCursor().isValid())
               return ((const ReceiverMultipleElement&) start->sortedCursor().elementAt())
                  .getElements().foreachDo(function, parameters,
                        start->multipleCursor().key(), end->multipleCursor().key());
            };
            if (!parameters.isExact())
               return true;
            AssumeCondition(!end->multipleCursor().isValid())
            IdentificationElement identification = TypeTraits::identifyElementFromSorted(
                  start->sortedCursor().elementAt());
            if (identification == IEReceiverSingle)
               return function(((const ReceiverSingleElement&)
                     start->sortedCursor().elementAt()).getElement());
            return function(((const UniqueElement&) start->sortedCursor().elementAt()));
         };
         ExtendedLocateParameters internParameters;
         if (start && start->multipleCursor().isValid()) {
            if (!((const ReceiverMultipleElement&) start->sortedCursor().elementAt())
               .getElements().foreachDo(function, parameters, start->multipleCursor().key(), nullptr))
               return false;
            internParameters.setFirstExcluded();
         };
         if (end && end->multipleCursor().isValid())
            internParameters.setLastExcluded();
         DMultipleSortedCollection::ElementExecute<TypeTraits, Execute> execute(function);
         if (!scSortedCollection.foreachDo(execute, internParameters,
               start ? &start->sortedCursor() : nullptr, end ? &end->sortedCursor() : nullptr))
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
            if (start->multipleCursor().isValid()) {
               AssumeCondition(end->multipleCursor().isValid())
               return ((const ReceiverMultipleElement&) start->sortedCursor().elementAt())
                  .getElements().foreachReverseDo(function, parameters,
                        start->multipleCursor().key(), end->multipleCursor().key());
            };
            if (!parameters.isExact())
               return true;
            AssumeCondition(!end->multipleCursor().isValid())
            IdentificationElement identification = TypeTraits::identifyElementFromSorted(
                  start->sortedCursor().elementAt());
            if (identification == IEReceiverSingle)
               return function(((const ReceiverSingleElement&)
                     start->sortedCursor().elementAt()).getElement());
            return function(((const UniqueElement&) start->sortedCursor().elementAt()));
         };
         ExtendedLocateParameters internParameters;
         if (start && start->multipleCursor().isValid()) {
            if (!((const ReceiverMultipleElement&) start->sortedCursor().elementAt())
               .getElements().foreachReverseDo(function, parameters, start->multipleCursor().key(), nullptr))
               return false;
            internParameters.setFirstExcluded();
         };
         if (end && end->multipleCursor().isValid())
            internParameters.setLastExcluded();
         DMultipleSortedCollection::ReverseElementExecute<TypeTraits, Execute> execute(function);
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
      {  DMultipleSortedCollection::ReverseElementExecute<TypeTraits, Execute> execute(function);
         return scSortedCollection.foreachReverseDo(execute);
      }
};

/********************************************************************/
/* Definition of the template class TMultipleSortedCollectionCursor */
/********************************************************************/

template <class TypeTraits>
class TMultipleSortedCollectionCursor : public VirtualMultiSortedCollectionCursor,
                                        public DMultipleSortedCollection::Identification {
  public:
   typedef typename TypeTraits::SortedCollection SortedCollection;
   typedef typename TypeTraits::SortedCollection::Cursor SortedCollectionCursor;
   typedef typename TypeTraits::MultipleCollection MultipleCollection;
   typedef typename TypeTraits::MultipleCollection::PPCursor PPMultipleCollectionCursor;
   typedef typename TypeTraits::CommonElement CommonElement;
   typedef typename TypeTraits::UniqueElement UniqueElement;
   typedef typename TypeTraits::SingleElement SingleElement;
   typedef typename TypeTraits::MultipleElement MultipleElement;
   typedef typename TypeTraits::ReceiverSingleElement ReceiverSingleElement;
   typedef typename TypeTraits::ReceiverMultipleElement ReceiverMultipleElement;
   typedef typename TypeTraits::SortedCollection::Key Key;
   typedef typename TypeTraits::BaseCast BaseCast;
   typedef typename TypeTraits::UniqueBaseCast UniqueBaseCast;
   typedef typename TypeTraits::UniqueDynamicBaseCast UniqueDynamicBaseCast;
   typedef typename TypeTraits::SingleBaseCast SingleBaseCast;
   typedef typename TypeTraits::SingleDynamicBaseCast SingleDynamicBaseCast;
   typedef typename TypeTraits::MultipleBaseCast MultipleBaseCast;
   typedef typename TypeTraits::MultipleDynamicBaseCast MultipleDynamicBaseCast;

  private:
   typedef VirtualMultiSortedCollectionCursor inherited;
   typedef TMultipleSortedCollection<TypeTraits> CollectionSupport;
   typedef TMultipleSortedCollectionCursor<TypeTraits> thisType;

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
            ? COL::VirtualCollection::RPAfter
            : COL::VirtualCollection::RPBefore;
      }

  protected:
   virtual ComparisonResult _compare(const EnhancedObject& asource) const
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

  public:
   SortedCollectionCursor& sortedCursor() { return sccCursor; }
   const SortedCollectionCursor& sortedCursor() const { return sccCursor; }
   PPMultipleCollectionCursor& multipleCursor() { return ppmccMultipleCursor; }
   const PPMultipleCollectionCursor& multipleCursor() const { return ppmccMultipleCursor; }

  protected:
   virtual bool _position(const inherited::Position& pos);
   virtual void _gotoReference(const EnhancedObject& element);
   virtual bool _isPositionned(const ExtendedLocateParameters& pos, const VirtualCollectionCursor* cursor=nullptr) const;
   virtual EnhancedObject* _getSElement() const
      {  CommonElement* result = nullptr;
         if (ppmccMultipleCursor.isValid())
            result = MultipleBaseCast::castTo((MultipleElement*) &ppmccMultipleCursor->elementSAt());
         else {
            typename SortedCollection::Node* node = &sccCursor.elementSAt();
            IdentificationElement identification = TypeTraits::identifyElementFromSorted(*node);
            AssumeCondition(identification != IEReceiverMultiple)
            if (identification == IEReceiverSingle)
               result = SingleBaseCast::castTo(&((const ReceiverSingleElement&) *node).getElement());
            else {
               AssumeCondition(identification == IEUnique)
               result = UniqueBaseCast::castTo((UniqueElement*) node);
            };
         };
         return BaseCast::castTo(result);
      }

   virtual bool _isEqual(const AbstractCursor& cursor) const
      {  const thisType& source = (const thisType&) cursor;
         return inherited::_isEqual(cursor) && sccCursor.isEqual(source.sccCursor)
            && (ppmccMultipleCursor.isValid()
               ? (source.ppmccMultipleCursor.isValid() && ppmccMultipleCursor->isEqual(*source.ppmccMultipleCursor))
               : !source.ppmccMultipleCursor.isValid());
      }
   typename TypeTraits::KeyTraits::ControlKeyType _queryKey() const { return sccCursor.queryKey(); }

  public:
   TMultipleSortedCollectionCursor(const CollectionSupport& support)
      :  inherited(support), sccCursor(support.getSortedCollection()) {}
   TMultipleSortedCollectionCursor(const thisType& source)
      :  inherited(source), sccCursor(source.sccCursor),
         ppmccMultipleCursor(source.ppmccMultipleCursor, PNT::Pointer::Duplicate()) {}
   TMultipleSortedCollectionCursor(thisType&& source)
      :  inherited(source.getSupport()), sccCursor(source.getSupport().getSortedCollection())
      {  inherited::swap(source);
         sccCursor.swap(source.sccCursor);
         ppmccMultipleCursor.swap(source.ppmccMultipleCursor);
      }
   TMultipleSortedCollectionCursor& operator=(const thisType& source)
      {  inherited::operator=(source);
         sccCursor = source.sccCursor;
         ppmccMultipleCursor.fullAssign(source.ppmccMultipleCursor);
         return *this;
      }

   TemplateDefineCopy(TMultipleSortedCollectionCursor, TypeTraits)
   DTemplateDefineAssign(TMultipleSortedCollectionCursor, TypeTraits)
   TemplateDefineCursorForAbstractCollect(TMultipleSortedCollection, TMultipleSortedCollectionCursor, TypeTraits)
   void swap(thisType& source)
      {  inherited::swap(source);
         sccCursor.swap(source.sccCursor);
         ppmccMultipleCursor.swap(source.ppmccMultipleCursor);
      }

   virtual bool isValid() const
      {  bool result = inherited::isValid() && sccCursor.isValid();
         if (result && ppmccMultipleCursor.isValid())
            result = (TypeTraits::identifyElementFromSorted(sccCursor.elementAt()) == TypeTraits::IEReceiverMultiple);
         else if (result)
            result = (TypeTraits::identifyElementFromSorted(sccCursor.elementAt()) != TypeTraits::IEReceiverMultiple);
         return result;
      }
   bool invariant() const
      {  return !(ppmccMultipleCursor.isValid()
            && (!sccCursor.isValid() || !ppmccMultipleCursor->isValid()));
      }

   bool isUnique() const
      {  return sccCursor.isValid()
            && (TypeTraits::identifyElementFromSorted(sccCursor.elementAt()) == IEUnique);
      }
   bool isSingle() const
      {  return sccCursor.isValid()
            && (TypeTraits::identifyElementFromSorted(sccCursor.elementAt()) == IEReceiverSingle);
      }
   bool isMultiple() const
      {  return ppmccMultipleCursor.isValid(); }

#define DefCursor
#define DefTypeFinal thisType::
// #define DefTypeCastToCollectHandler BaseCast::castTo
// #define DefTypeCCastToCollectHandler BaseCast::castTo
#define DefTypeCastToCollectHandler 
#define DefTypeCCastToCollectHandler 
#define DefTypeCastFromCollectHandler BaseCast::castFrom
#define DefTypeCCastFromCollectHandler BaseCast::castFrom
#define DefTypeElement CommonElement
   #include "Collection/ELMCollection.inch"
#undef DefTypeElement
#undef DefTypeCastToCollectHandler
#undef DefTypeCCastToCollectHandler
#undef DefTypeCastFromCollectHandler
#undef DefTypeCCastFromCollectHandler
#undef DefTypeFinal
#undef DefCursor

   const UniqueElement& uniqueElementAt() const
      {  UniqueElement* result = UniqueBaseCast::castFrom(BaseCast::castFrom(_getSElement()));
         AssumeCondition(UniqueDynamicBaseCast::castFrom(UniqueBaseCast::castTo(result)))
         return *result;
      }
   const SingleElement& singleElementAt() const
      {  SingleElement* result = SingleBaseCast::castFrom(BaseCast::castFrom(_getSElement()));
         AssumeCondition(SingleDynamicBaseCast::castFrom(SingleBaseCast::castTo(result)))
         return *result;
      }
   const MultipleElement& multipleElementAt() const
      {  MultipleElement* result = MultipleBaseCast::castFrom(BaseCast::castFrom(_getSElement()));
         AssumeCondition(MultipleDynamicBaseCast::castFrom(MultipleBaseCast::castTo(result)))
         return *result;
      }
   typename TypeTraits::KeyTraits::ControlKeyType queryKey() const { return _queryKey(); }
};

TemplateInlineCollectionForAbstractCollect(TMultipleSortedCollection, TMultipleSortedCollectionCursor, TypeTraits)

#define DefTemplate template <class TypeTraits>
#define DefJustInline
#define DefTypeKey typename Key::KeyType

#define DefTypeElement typename TypeTraits::CommonElement
#define DefExcludeElement
// #define DefTypeCastToCollectHandler BaseCast::castTo
// #define DefTypeCCastToCollectHandler BaseCast::castTo
#define DefTypeCastToCollectHandler (typename TypeTraits::CommonElement*)
#define DefTypeCCastToCollectHandler (const typename TypeTraits::CommonElement*)
#define DefTypeCastFromCollectHandler BaseCast::castFrom
#define DefTypeCCastFromCollectHandler BaseCast::castFrom
#define DefMapInterface
#define DefTypeControlKey typename TypeTraits::KeyTraits::ControlKeyType
#include "Collection/ELMCollection.inch"
#undef DefTypeControlKey
#undef DefMapInterface
#undef DefTypeCastToCollectHandler
#undef DefTypeCCastToCollectHandler
#undef DefTypeCastFromCollectHandler
#undef DefTypeCCastFromCollectHandler
#undef DefTypeElement
#undef DefExcludeElement

#define DefBaseMultipleInterface

#define DefImplementsNativeMethods
#define DefTypeElement typename TypeTraits::UniqueElement
// #define DefTypeCastToCollectHandler CompleteUniqueBaseCast::castTo
// #define DefTypeCCastToCollectHandler CompleteUniqueBaseCast::castTo
#define DefTypeCastToCollectHandler UniqueBaseCast::castTo
#define DefTypeCCastToCollectHandler UniqueBaseCast::castTo
#define DefTypeCastFromCollectHandler CompleteUniqueBaseCast::castFrom
#define DefTypeCCastFromCollectHandler CompleteUniqueBaseCast::castFrom
#include "Collection/ELMCollection.inch"
#undef DefTypeCastToCollectHandler
#undef DefTypeCCastToCollectHandler
#undef DefTypeCastFromCollectHandler
#undef DefTypeCCastFromCollectHandler
#undef DefTypeElement

#define DefTypeElement typename TypeTraits::SingleElement
// #define DefTypeCastToCollectHandler CompleteSingleBaseCast::castTo
// #define DefTypeCCastToCollectHandler CompleteSingleBaseCast::castTo
#define DefTypeCastToCollectHandler SingleBaseCast::castTo
#define DefTypeCCastToCollectHandler SingleBaseCast::castTo
#define DefTypeCastFromCollectHandler CompleteSingleBaseCast::castFrom
#define DefTypeCCastFromCollectHandler CompleteSingleBaseCast::castFrom
#include "Collection/ELMMapCollection.inch"
#undef DefTypeCastToCollectHandler
#undef DefTypeCCastToCollectHandler
#undef DefTypeCastFromCollectHandler
#undef DefTypeCCastFromCollectHandler
#undef DefTypeElement

#define DefTypeElement typename TypeTraits::MultipleElement
// #define DefTypeCastToCollectHandler CompleteMultipleBaseCast::castTo
// #define DefTypeCCastToCollectHandler CompleteMultipleBaseCast::castTo
#define DefTypeCastToCollectHandler MultipleBaseCast::castTo
#define DefTypeCCastToCollectHandler MultipleBaseCast::castTo
#define DefTypeCastFromCollectHandler CompleteMultipleBaseCast::castFrom
#define DefTypeCCastFromCollectHandler CompleteMultipleBaseCast::castFrom
#include "Collection/ELMMapCollection.inch"
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

template <class TypeBase>
class TMultiMapAdaptor : public TypeBase {
  public:
   typename TypeBase::LocationResult locate(const typename TypeBase::Node& sortedNode,
         typename TypeBase::Cursor& cursor, COL::VirtualCollection::RelativePosition pos=COL::VirtualCollection::RPUndefined)
      {  AssumeUncalled }
   typename TypeBase::LocationResult locate(const typename TypeBase::Node& element,
         const typename TypeBase::ExtendedLocateParameters& parameters,
         typename TypeBase::Cursor* cursor=nullptr, const typename TypeBase::Cursor* start=nullptr,
         const typename TypeBase::Cursor* end=nullptr) const { AssumeUncalled }
};
   
} // end of namespace COL

#endif // COL_MultiSortedH

