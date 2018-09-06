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
// Fichier     : VirtualMultiSortedCollection.h
// Description :
//   Definition of the class VirtualMultiSortedCollection.
//   It is the base class of all sorted multiset collections.
//

#ifndef COL_VirtualMultiSortedCollectionH
#define COL_VirtualMultiSortedCollectionH

#include "Collection/VirtualCollection/VirtualSortedCollection.h"

namespace COL {

#define DefTypeCollection VirtualMultiSortedCollection
#define DefTypeCursor VirtualMultiSortedCollectionCursor
#define DefTypeSorted
#define DefTypeMultiSorted
#define DefTypeKey const EnhancedObject&

class VirtualMultiSortedCollectionCursor;
class VirtualMultiSortedCollection : public VirtualCollection {
  private:
   typedef VirtualCollection inherited;

  public:
   DefineCollectionForAbstractCollect(VirtualMultiSortedCollection, VirtualMultiSortedCollectionCursor)
   class KeyLocationResult : public VirtualSortedCollection::LocationResult {
     private:
      typedef VirtualSortedCollection::LocationResult inherited;
      int uCount;
      int uCursorPosition;

     public:
      KeyLocationResult(RelativePosition pos=RPUndefined)
         :  inherited(pos), uCount(0), uCursorPosition(0) {}
      KeyLocationResult(int count, int cursorPosition)
         :  inherited(RPExact), uCount(count), uCursorPosition(cursorPosition)
         {  AssumeCondition((uCount > 0) && (uCursorPosition < uCount) && (uCursorPosition >= 0)) }
      KeyLocationResult(const KeyLocationResult& source)
         :  inherited(source), uCount(source.uCount), uCursorPosition(source.uCursorPosition) {}

      void setCount(int count)
         {  int oldCount = uCount;
            uCount = count;
            if (!isValid()) {
               uCount = oldCount;
               AssumeUncalled
            };
         }
      void setCursorPosition(int cursorPosition)
         {  int oldCursorPosition = uCursorPosition;
            uCursorPosition = cursorPosition;
            if (!isValid()) {
               uCursorPosition = oldCursorPosition;
               AssumeUncalled
            };
         }

      const int& count() { return uCount; }
      void setToFirst(VirtualMultiSortedCollectionCursor& cursor);
      void setToLast(VirtualMultiSortedCollectionCursor& cursor);
      void setToIndex(VirtualMultiSortedCollectionCursor& cursor, int index);

      bool isValid() const
         {  return inherited::isValid() && (uCount >= 0) && (uCursorPosition >= 0)
               && (getPosition() == RPExact)
                  ? (uCount > 0) && (uCursorPosition < uCount)
                  : (uCount == 0) && (uCursorPosition == 0);
         }
   };
   typedef KeyLocationResult LocationResult;

  protected:
   virtual ComparisonResult _compareElement(const EnhancedObject& fst, const EnhancedObject& snd) const
      {  return fcompare(key(fst), key(snd)); }

   void paddAll(const VirtualCollection& source, const ExtendedInsertionParameters& parameters,
         VirtualCollectionCursor* cursor, const VirtualCollectionCursor* startSource,
         const VirtualCollectionCursor* endSource)
      {  inherited::paddAll(source, parameters, cursor, startSource, endSource); }

   virtual LocationResult _locate(const EnhancedObject& source, const ExtendedLocateParameters& parameters,
         VirtualMultiSortedCollectionCursor* cursor=nullptr, const VirtualMultiSortedCollectionCursor* start=nullptr,
         const VirtualMultiSortedCollectionCursor* end=nullptr) const;
   virtual LocationResult _locateKey(const EnhancedObject& source, const ExtendedLocateParameters& parameters,
         VirtualMultiSortedCollectionCursor* cursor=nullptr, const VirtualMultiSortedCollectionCursor* start=nullptr,
         const VirtualMultiSortedCollectionCursor* end=nullptr) const;
   VirtualMultiSortedCollection() {}
   VirtualMultiSortedCollection(const VirtualMultiSortedCollection& source, AddMode dupMode=AMNoDuplicate)
      :  VirtualCollection(source, dupMode) {}

  public:
   DefineCopy(VirtualMultiSortedCollection)
   virtual const EnhancedObject& key(const EnhancedObject& element) const { return element; }

#define DefJustDeclare
   #include "Collection/ELMCollection.inch"
#undef DefJustDeclare

   class EnhancedKey {
     private:
      const VirtualMultiSortedCollection& vscSupport;
      
     public:
      EnhancedKey(const VirtualMultiSortedCollection& support) : vscSupport(support) {}

      typedef EnhancedObject TypeOfKey;
      typedef const EnhancedObject& KeyType;
      typedef const EnhancedObject& ControlKeyType;
      const EnhancedObject& key(const EnhancedObject& source) const
         {  return vscSupport.key(source); }
      static ComparisonResult compare(KeyType fstKey, KeyType sndKey)
         {  return fcompare(fstKey, sndKey); }
   };
   template <class TypeElement, class TypeKey, class TypeCast>
   class TemplateElementEnhancedKeyCastParameters {
     private:
      TypeKey tkKey;
      
     public:
      TemplateElementEnhancedKeyCastParameters(const VirtualMultiSortedCollection& support)
         :  tkKey(support) {}
      typedef TypeElement Element;
      typedef TypeCast Cast;
      typedef TypeKey Key;

      typename TypeKey::ControlKeyType key(const TypeElement& element) const { return tkKey.key(element); }
      ComparisonResult keyCompare(typename TypeKey::KeyType fst, typename TypeKey::KeyType snd) const
         {  return tkKey.compare(fst, snd); }
   };

   typedef VirtualSortedCollection::SimpleKey SimpleKey;
};

class VirtualMultiSortedCollectionCursor : public VirtualCollectionCursor {
  protected:
   virtual bool _position(const Position& pos) override
      {  return VirtualCollectionCursor::_position(pos); }
   virtual void _gotoReference(const EnhancedObject& element) override;

   VirtualMultiSortedCollectionCursor(const VirtualMultiSortedCollection& support)
      :  VirtualCollectionCursor(support) {}
   VirtualMultiSortedCollectionCursor(const VirtualMultiSortedCollectionCursor& source)
      :  VirtualCollectionCursor(source) {}

  public:
   DefineCopy(VirtualMultiSortedCollectionCursor)
   DefineCursorForAbstractCollect(VirtualMultiSortedCollection, VirtualMultiSortedCollectionCursor)

#define DefCursor
   #include "Collection/ELMCollection.inch"
#undef DefCursor
};

InlineCollectionForAbstractCollect(VirtualMultiSortedCollection, VirtualMultiSortedCollectionCursor)

inline void
VirtualMultiSortedCollection::KeyLocationResult::setToFirst(VirtualMultiSortedCollectionCursor& cursor)
   {  AssumeCondition(inherited::isValid() && (uCursorPosition >= 0)
         && (getPosition() == RPExact) && (uCount > 0) && (uCursorPosition < uCount))
      cursor -= uCursorPosition;
   }

inline void
VirtualMultiSortedCollection::KeyLocationResult::setToLast(VirtualMultiSortedCollectionCursor& cursor)
   {  AssumeCondition(inherited::isValid() && (uCursorPosition >= 0)
         && (getPosition() == RPExact) && (uCount > 0) && (uCursorPosition < uCount))
      cursor += (uCount-uCursorPosition-1);
   }

inline void
VirtualMultiSortedCollection::KeyLocationResult::setToIndex(VirtualMultiSortedCollectionCursor& cursor, int uIndex)
   {  AssumeCondition(inherited::isValid() && (uCursorPosition >= 0) && (getPosition() == RPExact)
         && (uCount > 0) && (uCursorPosition < uCount) && (uIndex >= 0) && (uIndex < uCount))
      cursor += (uIndex-uCursorPosition);
   }

inline VirtualMultiSortedCollection::LocationResult
VirtualMultiSortedCollection::_locateKey(const EnhancedObject& /* source */, const ExtendedLocateParameters& /* parameters */,
      VirtualMultiSortedCollectionCursor* cursor, const VirtualMultiSortedCollectionCursor* start,
      const VirtualMultiSortedCollectionCursor* end) const {
   AssumeCondition((!cursor || (&cursor->getSSupport() == this))
      && (!start || (&start->getSSupport() == this))
      && (!end || (&end->getSSupport() == this)))
   return LocationResult(RPUndefined);
}

inline VirtualMultiSortedCollection::LocationResult
VirtualMultiSortedCollection::_locate(const EnhancedObject& source, const ExtendedLocateParameters& parameters,
      VirtualMultiSortedCollectionCursor* cursor, const VirtualMultiSortedCollectionCursor* start,
      const VirtualMultiSortedCollectionCursor* end) const
   {  return _locateKey(key(source), parameters, cursor, start, end); }

#define DefJustInline
   #include "Collection/ELMCollection.inch"
#undef DefJustInline

inline void
VirtualMultiSortedCollectionCursor::_gotoReference(const EnhancedObject& element) {
   VirtualMultiSortedCollection::LocationResult
      result = getSupport().locate(element, *this, VirtualCollection::RPExact);
   if (result.count() == 1) {
      AssumeCondition(&elementAt() == &element)
   }
   else if (result.count() > 1) {
      result.setToFirst(*this);
      int elementsLeft = result.count();
      while ((elementsLeft != 0) && (&elementAt() != &element))
         elementsLeft--;
      AssumeCondition(elementsLeft > 0)
   }
   else
      AssumeUncalled
}

#undef DefTypeKey
#undef DefTypeSorted
#undef DefTypeMultiSorted
#undef DefTypeCursor
#undef DefTypeCollection

} // end of namespace COL

#endif // COL_VirtualMultiSortedCollectionH
