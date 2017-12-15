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
// Fichier     : VirtualSortedCollection.h
// Description :
//   Definition of the class VirtualSortedCollection.
//   It is the base class of all sorted collections.
//

#ifndef COL_VirtualSortedCollectionH
#define COL_VirtualSortedCollectionH

#include "Collection/VirtualCollection/VirtualCollection.h"

namespace COL {

#define DefTypeCollection VirtualSortedCollection
#define DefTypeCursor VirtualSortedCollectionCursor
#define DefTypeSorted
#define DefTypeKey const EnhancedObject&

class VirtualSortedCollectionCursor;
class VirtualSortedCollection : public VirtualCollection {
  public:
   class LocationResult : protected ExtendedParameters {
     private:
      RelativePosition rpPos;
      PNT::PassPointer<ExtendedInsertionParameters> ppInsertion;

     protected:
      PNT::PassPointer<ExtendedInsertionParameters>& insertion() { return ppInsertion; }
      ExtendedInsertionParameters& getInsertion() const { return *ppInsertion; }

     public:
      LocationResult(RelativePosition pos=RPUndefined) : rpPos(pos) {}
      LocationResult(const LocationResult& source) = default;
      LocationResult& operator=(const LocationResult& source) = default;
      bool operator==(const RelativePosition& pos) const { return rpPos == pos; }
      bool operator==(const LocationResult& source) const { return rpPos == source.rpPos; }
      bool operator!=(const RelativePosition& pos) const { return rpPos != pos; }
      bool operator!=(const LocationResult& source) const { return rpPos != source.rpPos; }

      operator bool() const { return rpPos == RPExact; }
      const RelativePosition& getPosition() const { return rpPos; }
      RelativePosition getInsertionPosition() const
         {  AssumeCondition((rpPos == RPBefore) || (rpPos == RPAfter))
            return (rpPos == RPBefore) ? RPAfter : RPBefore;
         }
      bool isValid() const { return rpPos != RPUndefined; }
      bool isClosed() const { return !ppInsertion.isValid(); }
      bool isOpen() const { return ppInsertion.isValid(); }
      ExtendedInsertionParameters& queryInsertionParameters()
         {  if (!ppInsertion.isValid()) {
               ppInsertion.absorbElement(new ExtendedInsertionParameters());
               ppInsertion->setRelativePosition(getInsertionPosition());
            };
            return getInsertion();
         }
      const LocationResult& asSortedLocation() const { return *this; }
   };

  protected:
   virtual ComparisonResult _compareElement(const EnhancedObject& fst, const EnhancedObject& snd) const
      {  return fcompare(key(fst), key(snd)); }

   virtual LocationResult _locate(const EnhancedObject& source, const ExtendedLocateParameters& parameters,
         VirtualSortedCollectionCursor* cursor=nullptr, const VirtualSortedCollectionCursor* start=nullptr,
         const VirtualSortedCollectionCursor* end=nullptr) const;
   virtual LocationResult _locateKey(const EnhancedObject& source, const ExtendedLocateParameters& parameters,
         VirtualSortedCollectionCursor* cursor=nullptr, const VirtualSortedCollectionCursor* start=nullptr,
         const VirtualSortedCollectionCursor* end=nullptr) const;
   VirtualSortedCollection() {}
   VirtualSortedCollection(const VirtualSortedCollection& source, AddMode duplicate=AMNoDuplicate)
      :  VirtualCollection(source, duplicate) {}

  public:
   DefineCopy(VirtualSortedCollection)
   DefineCollectionForAbstractCollect(VirtualSortedCollection, VirtualSortedCollectionCursor)
   virtual const EnhancedObject& key(const EnhancedObject& element) const { return element; }

#define DefJustDeclare
   #include "Collection/ELMCollection.inch"
#undef DefJustDeclare

   class SimpleKey {
     public:
      SimpleKey() {}
      SimpleKey(const VirtualSortedCollection& support) {}
      typedef EnhancedObject TypeOfKey; // only useful for collections with keys deriving from EnhancedObject
      typedef const EnhancedObject& KeyType;
      typedef const EnhancedObject& ControlKeyType;
      static const EnhancedObject& key(const EnhancedObject& source) { return source; }
      static ComparisonResult compare(KeyType fstKey, KeyType sndKey) { return fcompare(fstKey, sndKey); }
   };
   
   class EnhancedKey {
     private:
      const VirtualSortedCollection& vscSupport;
      
     public:
      EnhancedKey(const VirtualSortedCollection& support) : vscSupport(support) {}

      typedef EnhancedObject TypeOfKey;
      typedef const EnhancedObject& KeyType;
      typedef const EnhancedObject& ControlKeyType;
      const EnhancedObject& key(const EnhancedObject& source) const { return vscSupport.key(source); }
      static ComparisonResult compare(KeyType fstKey, KeyType sndKey) { return fcompare(fstKey, sndKey); }
   };

   template <class TypeElement, class TypeKey, class TypeCast>
   class TemplateElementEnhancedKeyCastParameters {
     private:
      TypeKey tkKey;
      
     public:
      TemplateElementEnhancedKeyCastParameters(const VirtualSortedCollection& support)
         :  tkKey(support) {}
      typedef TypeElement Element;
      typedef TypeCast Cast;
      typedef TypeKey Key;

      typename TypeKey::ControlKeyType key(const TypeElement& element) const { return tkKey.key(element); }
      ComparisonResult keyCompare(typename TypeKey::KeyType fst, typename TypeKey::KeyType snd) const
         {  return tkKey.compare(fst, snd); }
   };
};

class VirtualSortedCollectionCursor : public VirtualCollectionCursor {
  protected:
   virtual bool _position(const Position& pos) override
      {  return VirtualCollectionCursor::_position(pos); }
   virtual void _gotoReference(const EnhancedObject& element) override;

   VirtualSortedCollectionCursor(const VirtualSortedCollection& support)
      :  VirtualCollectionCursor(support) {}
   VirtualSortedCollectionCursor(const VirtualSortedCollectionCursor& source)
      :  VirtualCollectionCursor(source) {}

  public:
   DefineCopy(VirtualSortedCollectionCursor)
   DefineCursorForAbstractCollect(VirtualSortedCollection, VirtualSortedCollectionCursor)

   bool isOnLocation() const { return isValid(); }

#define DefCursor
   #include "Collection/ELMCollection.inch"
#undef DefCursor
};

InlineCollectionForAbstractCollect(VirtualSortedCollection, VirtualSortedCollectionCursor)

inline VirtualSortedCollection::LocationResult
VirtualSortedCollection::_locateKey(const EnhancedObject& source, const ExtendedLocateParameters& parameters,
      VirtualSortedCollectionCursor* cursor, const VirtualSortedCollectionCursor* start,
      const VirtualSortedCollectionCursor* end) const {
   AssumeCondition((!cursor || (&cursor->getSSupport() == this))
      && (!start || (&start->getSSupport() == this))
      && (!end || (&end->getSSupport() == this)))
   return LocationResult(RPUndefined);
}

inline VirtualSortedCollection::LocationResult
VirtualSortedCollection::_locate(const EnhancedObject& source, const ExtendedLocateParameters& parameters,
      VirtualSortedCollectionCursor* cursor, const VirtualSortedCollectionCursor* start,
      const VirtualSortedCollectionCursor* end) const
   { return _locateKey(key(source), parameters, cursor, start, end); }

#define DefJustInline
   #include "Collection/ELMCollection.inch"
#undef DefJustInline

inline void
VirtualSortedCollectionCursor::_gotoReference(const EnhancedObject& element)
   {  VirtualSortedCollection::LocationResult
         result = getSupport().locate(element, *this, VirtualCollection::RPExact);
      AssumeCondition(result && (&element == &elementAt()))
   }

#undef DefTypeKey
#undef DefTypeSorted
#undef DefTypeCursor
#undef DefTypeCollection

template <typename TKey>
class SimpleKeyTraits {
  public:
   typedef TKey KeyType;
   typedef TKey ControlKeyType;
   static KeyType see(ControlKeyType key) { return key; }
   static ControlKeyType store(KeyType key) { return key; }
   static ControlKeyType copy(ControlKeyType key) { return key; }
};

template <class TKey>
class LightCopyKeyTraits {
  public:
   typedef const TKey& KeyType;
   typedef TKey ControlKeyType;
   static const TKey& see(const TKey& key) { return key; }
   static const TKey& store(const TKey& key) { return key; }
   static const TKey& copy(const TKey& key) { return key; }
};

template <class TKey>
class AllocKeyTraits {
  public:
   typedef const TKey& KeyType;
   typedef PNT::PassPointer<TKey> ControlKeyType;
   static const TKey& see(const PNT::PassPointer<TKey>& key) { return *key; }
   static PNT::PassPointer<TKey> store(const TKey& key) { return PNT::PassPointer<TKey>(key); }
   static PNT::PassPointer<TKey> copy(const PNT::PassPointer<TKey>& key)
      {  return PNT::PassPointer<TKey>(key, PNT::Pointer::Duplicate()); }
};

} // end of namespace COL

#endif
