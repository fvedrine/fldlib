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
// Library   : Collection
// Unit      : Abstract collections
// File      : ELMCollection.h
// Description :
//   Definition of polymorph collection classes with a common ancestor.
//

#ifndef COL_ELMCollectionH
#define COL_ELMCollectionH

#include "Collection/VirtualCollection/VirtualCollection.h"

namespace COL {

template <class Element, class Collection, class Cast=SimpleCast>
class TCollection : public Collection {
  public:
   typedef typename Collection::AddMode AddMode;
   typedef typename Collection::RemoveMode RemoveMode;
   typedef typename Collection::RelativePosition RelativePosition;
   typedef typename Collection::ExtendedLocateParameters ExtendedLocateParameters;
   typedef typename Collection::ExtendedInsertionParameters ExtendedInsertionParameters;
   typedef typename Collection::ExtendedSuppressParameters ExtendedSuppressParameters;
   typedef typename Collection::ExtendedReplaceParameters ExtendedReplaceParameters;

   TCollection() {}
   TCollection(const TCollection<Element, Collection, Cast>& source, VirtualCollection::AddMode duplicate=VirtualCollection::AMNoDuplicate)
      :  Collection(source, duplicate) {}
   Template3DefineCopy(TCollection, Element, Collection, Cast)

   #define DefTypeElement Element
   #define DefTypeCollection TCollection<Element, Collection>
   #define DefTypeInheritedCollection Collection
   #define DefTypeCastToCollectHandler Cast::castTo
   #define DefTypeCCastToCollectHandler Cast::castTo
   #define DefTypeCastFromCollectHandler (Element*) Cast::castFrom
   #define DefTypeCCastFromCollectHandler (const Element*) Cast::castFrom
   #define DefTypeCursor typename Collection::Cursor
   #include "Collection/ELMCollection.inch"
   #undef DefTypeElement
   #undef DefTypeCollection
   #undef DefTypeInheritedCollection
   #undef DefTypeCastToCollectHandler
   #undef DefTypeCCastToCollectHandler
   #undef DefTypeCastFromCollectHandler
   #undef DefTypeCCastFromCollectHandler
   #undef DefTypeCursor
};

template <class Element, class Collection=VirtualCollection,
   class CollectionStorage=PNT::TPointer<VirtualCollection>, class Cast=SimpleCast>
class TWrapCollection : public EnhancedObject {
  public:
   typedef typename Collection::AddMode AddMode;
   typedef typename Collection::RemoveMode RemoveMode;
   typedef typename Collection::RelativePosition RelativePosition;
   typedef typename DVirtualCollection::ExtendedLocateParameters ExtendedLocateParameters;
   typedef typename DVirtualCollection::ExtendedInsertionParameters ExtendedInsertionParameters;
   typedef typename DVirtualCollection::ExtendedSuppressParameters ExtendedSuppressParameters;
   typedef typename DVirtualCollection::ExtendedReplaceParameters ExtendedReplaceParameters;

  private:
   typedef TWrapCollection<Element, Collection, CollectionStorage, Cast> thisType;
   CollectionStorage pcCollection; // préféré à l'héritage

   // template <class OtherElement>
   // friend class TWrapCollection<OtherElement, Collection, CollectionStorage, Cast>;

  protected:
   void _fullAssign(const thisType& source, const ExtendedReplaceParameters& parameters)
      {  pcCollection->_fullAssign(*source.pcCollection, parameters); }
   void _add(Element* element, const ExtendedInsertionParameters& parameters,
         typename Collection::Cursor* cursor=nullptr)
      {  pcCollection->_add(Cast::castTo(element), parameters, cursor); }
   void _addAll(const thisType& source, const ExtendedInsertionParameters& parameters,
         typename Collection::Cursor* cursor=nullptr, const typename Collection::Cursor* startSource=nullptr,
         const typename Collection::Cursor* endSource=nullptr)
      {  pcCollection->_addAll(*source.pcCollection, parameters, cursor, startSource, endSource); }
   void _moveTo(thisType& destination, const ExtendedReplaceParameters& parameters,
         typename Collection::Cursor* cursor=nullptr, typename Collection::Cursor* destinationCursor=nullptr)
      {  pcCollection->_moveTo(*destination.pcCollection, parameters, cursor, destinationCursor); }
   void _moveAllTo(thisType& destination, const ExtendedReplaceParameters& parameters,
         const typename Collection::Cursor* startCursor=nullptr, const typename Collection::Cursor* endCursor=nullptr,
         typename Collection::Cursor* destinationCursor=nullptr)
      {  pcCollection->_moveAllTo(*destination.pcCollection, parameters, startCursor,
            endCursor, destinationCursor);
      }
   void _remove(const ExtendedSuppressParameters& parameters, typename Collection::Cursor* cursor=nullptr)
      {  pcCollection->_remove(parameters, cursor); }
   void _removeAll(const ExtendedSuppressParameters& parameters,
            const typename Collection::Cursor* start=nullptr, const typename Collection::Cursor* end=nullptr)
      {  pcCollection->_removeAll(parameters, start, end); }
   void _replace(Element* element, const ExtendedReplaceParameters& parameters, typename Collection::Cursor* cursor=nullptr)
      {  pcCollection->_replace(Cast::castTo(element), parameters, cursor); }
   int _queryCount(const ExtendedLocateParameters& parameters,
         const typename Collection::Cursor* start=nullptr, const typename Collection::Cursor* end=nullptr) const
      {  return pcCollection->queryCount(parameters, start, end); }
   Element* _getElement(const ExtendedLocateParameters& parameters,
         const typename Collection::Cursor* cursor=nullptr) const
      {  return (Element*) pcCollection->_getElement(parameters, cursor); }

  public:
   TWrapCollection(Collection* collection)
      :  pcCollection(collection, CollectionStorage::Init()) {}
   TWrapCollection(const TWrapCollection<Element, Collection, CollectionStorage, Cast>& source)
      :  EnhancedObject(source), pcCollection(source.pcCollection) {}
   template <class OtherElement>
   TWrapCollection(const TWrapCollection<OtherElement, Collection, CollectionStorage, Cast>& source)
      :  EnhancedObject(source), pcCollection(source.pcCollection) {}
   Template4DefineCopy(TWrapCollection, Element, Collection, CollectionStorage, Cast)
   DTemplate4DefineAssign(TWrapCollection, Element, Collection, CollectionStorage, Cast)
   virtual bool isValid() const { return pcCollection.isValid(); }

   #define DefTypeElement Element
   #define DefTypeCollection TWrapCollection<Element, Collection, CollectionStorage, Cast>
   #define DefTypeCursor typename Collection::Cursor
   #include "Collection/ELMCollection.inch"
   #undef DefTypeElement
   #undef DefTypeCollection
   #undef DefTypeCursor

   Collection& getRealCollection() const { return (Collection&) *pcCollection; }
};

} // end of namespace COL

#endif // COL_ELMCollectionH
