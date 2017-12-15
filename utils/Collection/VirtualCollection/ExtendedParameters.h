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
// Fichier     : ExtendedParameters.h
// Description :
//   Definition of extended parameters inherited by all collection classes.
//

#ifndef COL_ExtendedParametersH
#define COL_ExtendedParametersH

#include "Collection/VirtualCollection/AbstractCollection.h"

namespace COL { namespace DVirtualCollection {

/*************************************************************/
/* Definition of VirtualCollection::ExtendedLocateParameters */
/*************************************************************/
// Define a localization position (undefined, before, exact, after)
// see RelativePosition.
//

class Definitions {
  public:
   enum RelativePosition { RPUndefined=0, RPBefore=1, RPExact=2, RPAfter=3 };
   enum AddMode { AMNoDuplicate=0, AMDuplicate=1 };
   enum RemoveMode { RMRemove=0, RMFree=1 };
};

class VirtualKeyHandler : protected ExtendedParameters {
  protected:
   DefineExtendedParameters(1, ExtendedParameters)

   void setOpen() { mergeOwnField(1); }

  public:
   virtual ~VirtualKeyHandler() {}

   bool isClosed() const { return !hasOwnField(); }
   bool isOpen() const { return hasOwnField(); }
};

template <class TypeKeyTraits>
class TKeyHandler : public VirtualKeyHandler {
  private:
   typedef TKeyHandler<TypeKeyTraits> thisType;
   typename TypeKeyTraits::ControlKeyType cktKey;

  public:
   TKeyHandler(typename TypeKeyTraits::KeyType key) : cktKey(TypeKeyTraits::store(key)) {}
   TKeyHandler(const TKeyHandler<TypeKeyTraits>& source)
      : VirtualKeyHandler(source), cktKey(TypeKeyTraits::copy(source.cktKey)) {}
   typename TypeKeyTraits::KeyType queryKey() const { return TypeKeyTraits::see(cktKey); }
};

template <class TypeBase, class TypeKeyTraits>
class TMapParameters : public TypeBase, public TKeyHandler<TypeKeyTraits> {
  private:
   typedef TMapParameters<TypeBase, TypeKeyTraits> thisType;
   typedef TypeBase inherited;
   typedef TKeyHandler<TypeKeyTraits> inheritedKey;

  public:
   TMapParameters(typename TypeKeyTraits::KeyType key) : inheritedKey(key) { TypeBase::setMap(); }
   TMapParameters(const TypeBase& source, typename TypeKeyTraits::KeyType key)
      : inherited(source), inheritedKey(key) { TypeBase::setMap(); }
   TMapParameters(const thisType& source) : inherited(source), inheritedKey(source) {}
   DefineCopy(thisType)
   DDefineAssign(thisType)
   thisType& operator=(const thisType& source)
      {  return (thisType&) inherited::operator=(source); }

   virtual const VirtualKeyHandler& getKeyHandler() const override { return (const inheritedKey&) *this; }
};

template <class TypeBase, class TypeRefer>
class TReferenceParameters : public TypeBase {
  private:
   typedef TReferenceParameters<TypeBase, TypeRefer> thisType;

   const TypeRefer& trReference;

  public:
   TReferenceParameters(const TypeBase& source, const TypeRefer& reference)
      :  TypeBase(source), trReference(reference) {}
   TReferenceParameters(const thisType& source) = default;
   DefineCopy(thisType)

   virtual const VirtualKeyHandler& getKeyHandler() const override
      {  return trReference.getKeyHandler(); }
};

template <class TypeBase, class TypeRefer>
class TReferenceInsertionParameters : public TypeBase {
  private:
   typedef TReferenceInsertionParameters<TypeBase, TypeRefer> thisType;

   const TypeRefer& trReference;

  public:
   TReferenceInsertionParameters(const TypeBase& source, const TypeRefer& reference)
      :  TypeBase(source), trReference(reference) {}
   TReferenceInsertionParameters(const thisType& ttSource) = default;
   DefineCopy(thisType)

   virtual const VirtualKeyHandler& getKeyHandler() const override { return trReference.getKeyHandler(); }
   virtual const VirtualCast& getCast() const override {  return trReference.getCast(); }
};

class ExtendedLocateParameters
   :  public EnhancedObject, public ExtendedParameters, public Definitions {
  protected:
   enum SortedLocation { SLNone, SLElement, SLMap };
   DefineExtendedParameters(4, ExtendedParameters)
   DefineSubExtendedParameters(RelativePosition, 2, INHERITED)
   DefineSubExtendedParameters(SortedLocation, 2, RelativePosition)
   ExtendedLocateParameters& setMap() { setSortedLocationField(SLMap); return *this; }
   ExtendedLocateParameters& setElement() { setSortedLocationField(SLElement); return *this; }

  public:
   ExtendedLocateParameters(RelativePosition position = RPUndefined)
      {  mergeRelativePositionField(position); }
   ExtendedLocateParameters(const ExtendedLocateParameters& source)
      :  ExtendedParameters(source) {}
   DefineCopy(ExtendedLocateParameters)
   DDefineAssign(ExtendedLocateParameters)

   ExtendedLocateParameters& setRelativePosition(RelativePosition position)
      {  setRelativePositionField(position); return *this; }
   RelativePosition getRelativePosition() const { return (RelativePosition) queryRelativePositionField(); }
   bool isAfter() const { return getRelativePosition() == RPAfter; }
   bool isBefore() const { return getRelativePosition() == RPBefore; }
   bool isExact() const { return getRelativePosition() == RPExact; }
   bool isNear() const { return getRelativePosition() & RPBefore; }
   bool isUndefined() const { return getRelativePosition() == RPUndefined; }
   bool isFirstExcluded() const
      {  RelativePosition position = getRelativePosition();
         return (position == RPAfter) || (position == RPUndefined);
      }
   bool isLastExcluded() const
      {  RelativePosition position = getRelativePosition();
         return (position == RPBefore) || (position == RPUndefined);
      }
   ExtendedLocateParameters& setFirstExcluded()
      {  RelativePosition position = getRelativePosition();
         setRelativePositionField((position == RPExact) ? RPAfter
               : ((position == RPBefore) ? RPUndefined : position));
         return *this;
      }
   ExtendedLocateParameters& setLastExcluded()
      {  RelativePosition position = getRelativePosition();
         setRelativePositionField((position == RPExact) ? RPBefore
               : ((position == RPAfter) ? RPUndefined : position));
         return *this;
      }
   ExtendedLocateParameters& unsetFirstExcluded()
      {  RelativePosition position = getRelativePosition();
         setRelativePositionField((position == RPAfter) ? RPExact
               : ((position == RPUndefined) ? RPBefore : position));
         return *this;
      }
   ExtendedLocateParameters& unsetLastExcluded()
      {  RelativePosition position = getRelativePosition();
         setRelativePositionField((position == RPBefore) ? RPExact
               : ((position == RPUndefined) ? RPAfter : position));
         return *this;
      }
   bool hasKey() const { return querySortedLocationField() == SLMap; }
   bool containsKey() const { return querySortedLocationField() == SLElement; }
   bool hasNoKey() const { return !hasSortedLocationField(); }
   ExtendedLocateParameters& setLocal() { clearSortedLocationField(); return *this; }

   ExtendedLocateParameters& setExact() { return setRelativePosition(RPExact); }
   ExtendedLocateParameters& setBefore() { return setRelativePosition(RPBefore); }
   ExtendedLocateParameters& setAfter() { return setRelativePosition(RPAfter); }
   ExtendedLocateParameters& setUndefined() { return setRelativePosition(RPUndefined); }
   virtual const VirtualKeyHandler& getKeyHandler() const
      {  AssumeUncalled const VirtualKeyHandler* result = nullptr; return *result; }
};

/**************************************************************************/
/* Definition of the class VirtualCollection::ExtendedInsertionParameters */
/**************************************************************************/

template <class TypeBase>
class TCastParameters : public TypeBase {
  private:
   typedef TypeBase inherited;
   typedef TCastParameters<TypeBase> thisType;
   const VirtualCast* pvcCast; 
   
  public:
   TCastParameters() : pvcCast(nullptr) {}
   TCastParameters(const thisType& source) = default;
   TCastParameters(const TypeBase& source) : inherited(source), pvcCast(nullptr) {}
   DefineCopy(thisType)
   
   thisType& setExternCast(const VirtualCast& cast)
      {  pvcCast = &cast; return (thisType&) inherited::setExternCast(); }
   virtual const VirtualCast& getCast() const override {  AssumeCondition(pvcCast) return *pvcCast; }
};

class ExtendedInsertionParameters : public ExtendedLocateParameters {
  protected:
   DefineExtendedParameters(4, ExtendedLocateParameters)
   DefineSubExtendedParameters(Duplicate, 1, INHERITED)
   DefineSubExtendedParameters(ExternCast, 1, Duplicate)
   DefineSubExtendedParameters(FreeOnError, 1, ExternCast)
   DefineSubExtendedParameters(Remote, 1, FreeOnError)
   ExtendedInsertionParameters& setExternCast() { mergeExternCastField(1); return *this; }

  public:
   ExtendedInsertionParameters(AddMode duplicate=AMNoDuplicate, RemoveMode freeOnError=RMRemove,
         RelativePosition position=RPUndefined) : ExtendedLocateParameters(position)
      {  mergeDuplicateField(duplicate); mergeFreeOnErrorField(freeOnError); }
   ExtendedInsertionParameters(const ExtendedInsertionParameters& source) = default;
   DefineCopy(ExtendedInsertionParameters)

   ExtendedInsertionParameters& setRelativePosition(RelativePosition position)
      {  return (ExtendedInsertionParameters&)
           ExtendedLocateParameters::setRelativePosition(position);
      }
   ExtendedInsertionParameters& setExact() { return (ExtendedInsertionParameters&) ExtendedLocateParameters::setExact(); }
   ExtendedInsertionParameters& setBefore() { return (ExtendedInsertionParameters&) ExtendedLocateParameters::setBefore(); }
   ExtendedInsertionParameters& setAfter() { return (ExtendedInsertionParameters&) ExtendedLocateParameters::setAfter(); }
   ExtendedInsertionParameters& setUndefined() { return (ExtendedInsertionParameters&) ExtendedLocateParameters::setUndefined(); }
   ExtendedInsertionParameters& setFreeOnError()
      {  mergeFreeOnErrorField(RMFree); return *this; }
   ExtendedInsertionParameters& setFreeOnError(RemoveMode removeModeOnError)
      {  setFreeOnErrorField(removeModeOnError); return *this; }
   ExtendedInsertionParameters& releaseFreeOnError()
      {  clearFreeOnErrorField(); return *this; }
   ExtendedInsertionParameters& setDuplicate()
      {  mergeDuplicateField(AMDuplicate); return *this; }
   ExtendedInsertionParameters& setDuplicate(AddMode duplicate)
      {  setDuplicateField(duplicate); return *this; }
   ExtendedInsertionParameters& releaseDuplicate() { clearDuplicateField(); return *this; }
   ExtendedInsertionParameters& setRemote() { mergeRemoteField(1); return *this; }
   ExtendedInsertionParameters& releaseRemote() { clearRemoteField(); return *this; }

   bool isInsertion() const
      {  RelativePosition pos = getRelativePosition();
         return (pos == RPBefore) || (pos == RPAfter);
      }
   RelativePosition getRelativePosition() const
      {  return ExtendedLocateParameters::getRelativePosition(); }
   bool isFreeOnError() const { return hasFreeOnErrorField(); }
   bool isDuplicate() const { return hasDuplicateField(); }
   AddMode getDuplicate() const { return (AddMode) queryDuplicateField(); }
   bool hasFinalControl() const { return isFreeOnError() || isDuplicate(); }
   bool isRemote() const { return hasRemoteField(); } // remote insertion
   bool hasCast() const { return hasExternCastField(); }
   virtual const VirtualCast& getCast() const
      {  AssumeUncalled const VirtualCast* result = nullptr; return *result; }
   const VirtualCastWithElement* getSCast() const
      {  return hasExternCastField() ? (const VirtualCastWithElement*) &getCast() : nullptr; }

   ExtendedInsertionParameters& setLocal()
      {  clearExternCastField();
         return (ExtendedInsertionParameters&) ExtendedLocateParameters::setLocal();
      }
   ExtendedInsertionParameters& clearCast()
      {  clearExternCastField(); return *this; }
};

/*************************************************************************/
/* Definition of the class VirtualCollection::ExtendedSuppressParameters */
/*************************************************************************/

class ExtendedSuppressParameters : public ExtendedLocateParameters {
  protected:
   DefineExtendedParameters(1, ExtendedLocateParameters)

  public:
   ExtendedSuppressParameters(RemoveMode removeMode=RMRemove, RelativePosition position=RPUndefined)
      :  ExtendedLocateParameters(position) { mergeOwnField(removeMode); }
   ExtendedSuppressParameters(const ExtendedSuppressParameters& source) = default;
   DefineCopy(ExtendedSuppressParameters)

   ExtendedSuppressParameters& setFree(RemoveMode mode) { setOwnField(mode); return *this; }
   ExtendedSuppressParameters& releaseFree() { clearOwnField(); return *this; }
   ExtendedSuppressParameters& setFree() { mergeOwnField(RMFree); return *this; }
   ExtendedSuppressParameters& setRelativePosition(RelativePosition position)
      {  return (ExtendedSuppressParameters&) ExtendedLocateParameters::setRelativePosition(position); }
   ExtendedSuppressParameters& setExact() { return (ExtendedSuppressParameters&) ExtendedLocateParameters::setExact(); }
   ExtendedSuppressParameters& setBefore() { return (ExtendedSuppressParameters&) ExtendedLocateParameters::setBefore(); }
   ExtendedSuppressParameters& setAfter() { return (ExtendedSuppressParameters&) ExtendedLocateParameters::setAfter(); }
   ExtendedSuppressParameters& setUndefined() { return (ExtendedSuppressParameters&) ExtendedLocateParameters::setUndefined(); }

   RelativePosition getRelativePosition() const
      {  return ExtendedLocateParameters::getRelativePosition(); }
   bool isFree() const { return hasOwnField(); }
};

/************************************************************************/
/* Definition of the class VirtualCollection::ExtendedReplaceParameters */
/************************************************************************/

class ExtendedReplaceParameters
   :  public ExtendedInsertionParameters, public ExtendedSuppressParameters {
  protected:
   virtual ComparisonResult _compare(const EnhancedObject& asource) const override
      {  return ExtendedInsertionParameters::_compare(asource); }

  public:
   ExtendedReplaceParameters(AddMode duplicate=AMNoDuplicate, RemoveMode free=RMRemove,
         RemoveMode freeOnError=RMRemove)
      :  ExtendedInsertionParameters(duplicate, freeOnError, RPExact),
         ExtendedSuppressParameters(free) {}
   ExtendedReplaceParameters(const ExtendedReplaceParameters& source) = default;
   DefineCopy(ExtendedReplaceParameters)
   DDefineAssign(ExtendedReplaceParameters)
   StaticInheritConversions(ExtendedReplaceParameters, ExtendedInsertionParameters)

   bool getMoving() const
      {  return ExtendedInsertionParameters::getRelativePosition() == RPUndefined; }
   bool isFreeOnError() const
      {  return ExtendedInsertionParameters::isFreeOnError(); }
   bool isDuplicate() const
      {  return ExtendedInsertionParameters::isDuplicate(); }
   bool isFree() const
      {  return ExtendedSuppressParameters::isFree(); }
   RelativePosition getInsertionPosition() const
      {  return ExtendedInsertionParameters::getRelativePosition(); }
   RelativePosition getRemovePosition() const
      {  return ExtendedSuppressParameters::getRelativePosition(); }
   const ExtendedSuppressParameters& getSuppress() const
      {  return (const ExtendedSuppressParameters&) *this; }
   const ExtendedInsertionParameters& getInsertion() const
      {  return (const ExtendedInsertionParameters&) *this; }

   ExtendedReplaceParameters& setMoving()
      {  return (ExtendedReplaceParameters&)
            ExtendedInsertionParameters::setRelativePosition(RPUndefined);
      }
   ExtendedReplaceParameters& setFreeOnError(RemoveMode freeOnError=RMFree)
      {  return (ExtendedReplaceParameters&) ExtendedInsertionParameters::setFreeOnError(freeOnError); }
   ExtendedReplaceParameters& releaseFreeOnError()
      {  return (ExtendedReplaceParameters&) ExtendedInsertionParameters::releaseFreeOnError(); }
   ExtendedReplaceParameters& setFree(RemoveMode mode)
      {  return (ExtendedReplaceParameters&) ExtendedSuppressParameters::setFree(mode); }
   ExtendedReplaceParameters& setFree()
      {  return (ExtendedReplaceParameters&) ExtendedSuppressParameters::setFree(); }
   ExtendedReplaceParameters& setDuplicate()
      {  return (ExtendedReplaceParameters&) ExtendedInsertionParameters::setDuplicate(); }
   ExtendedReplaceParameters& setDuplicate(AddMode duplicate)
      {  return (ExtendedReplaceParameters&) ExtendedInsertionParameters::setDuplicate(duplicate); }
   ExtendedReplaceParameters& releaseDuplicate()
      {  return (ExtendedReplaceParameters&) ExtendedInsertionParameters::releaseDuplicate(); }
   ExtendedReplaceParameters& setRemovePosition(RelativePosition position)
      {  return (ExtendedReplaceParameters&) ExtendedSuppressParameters::setRelativePosition(position); }
   ExtendedReplaceParameters& setRemoveExact()
      {  return (ExtendedReplaceParameters&) ExtendedSuppressParameters::setExact(); }
   ExtendedReplaceParameters& setRemoveBefore()
      {  return (ExtendedReplaceParameters&) ExtendedSuppressParameters::setBefore(); }
   ExtendedReplaceParameters& setRemoveAfter()
      {  return (ExtendedReplaceParameters&) ExtendedSuppressParameters::setAfter(); }
   ExtendedReplaceParameters& setInsertionPosition(RelativePosition position)
      {  return (ExtendedReplaceParameters&) ExtendedInsertionParameters::setRelativePosition(position); }
      
   ExtendedReplaceParameters& setLocal()
      {  ExtendedSuppressParameters::setLocal();
         return (ExtendedReplaceParameters&) ExtendedInsertionParameters::setLocal();
      }
   ExtendedReplaceParameters& clearCast()
      {  return (ExtendedReplaceParameters&) ExtendedInsertionParameters::clearCast(); }
   virtual const VirtualKeyHandler& getKeyHandler() const override
      {  return ExtendedSuppressParameters::getKeyHandler(); }
};

} // end of namespace DVirtualCollection

namespace DVirtualTree {

using namespace DVirtualCollection;

class ExtendedTreeLocateParameters : public ExtendedLocateParameters {
  private:
   typedef ExtendedLocateParameters inherited;

  public:
   ExtendedTreeLocateParameters() {}
   ExtendedTreeLocateParameters(const ExtendedLocateParameters& source)
      :  ExtendedLocateParameters(source)
      {  params() &= ExtendedLocateParameters::MASK; }
   ExtendedTreeLocateParameters(RelativePosition pos) { setRelativePosition(pos); }
   ExtendedTreeLocateParameters(const ExtendedTreeLocateParameters& source) = default;
   DefineCopy(ExtendedTreeLocateParameters)

   DefineExtendedParameters(3, inherited)
   DefineSubExtendedParameters(Level, 2, INHERITED)
   DefineSubExtendedParameters(SingleLevel, 1, Level)

   enum LevelMode { LMBrothers=0, LMSons=1, LMInnerSons=2 };

   ExtendedTreeLocateParameters& setSons() { setLevelField(LMSons); return *this; }
   ExtendedTreeLocateParameters& setInnerSons()  { setLevelField(LMInnerSons); return *this; }
   ExtendedTreeLocateParameters& setBrothers() { clearLevelField(); return *this; }
   bool isBrother() const { return queryLevelField() == LMBrothers; }
   bool isSons() const { return queryLevelField() == LMSons; }
   bool isInnerSons() const { return queryLevelField() == LMInnerSons; }
   bool isDescendant() const { return hasLevelField(); }

   ExtendedTreeLocateParameters& setSingleLevel() { setSingleLevelField(1); return *this; }
   ExtendedTreeLocateParameters& setRecursiveLevel() { clearSingleLevelField(); return *this; }
   bool isSingleLevel() const { return hasSingleLevelField(); }

   ExtendedTreeLocateParameters& setExact() { return setRelativePosition(RPExact); }
   ExtendedTreeLocateParameters& setBefore() { return setRelativePosition(RPBefore); }
   ExtendedTreeLocateParameters& setAfter() { return setRelativePosition(RPAfter); }
   ExtendedTreeLocateParameters& setUndefined() { return setRelativePosition(RPUndefined); }

   ExtendedTreeLocateParameters& setRelativePosition(RelativePosition pos)
      {  return (ExtendedTreeLocateParameters&) inherited::setRelativePosition(pos);  }
};

class ExtendedTreeInsertionParameters : public ExtendedInsertionParameters {
  private:
   typedef ExtendedInsertionParameters inherited;

  public:
   DefineExtendedParameters(2, inherited)
   DefineSubExtendedParameters(Level, 2, INHERITED)
   enum LevelMode { LMBrothers=0, LMSons=1, LMInnerSons=2 };

   ExtendedTreeInsertionParameters() {}
   ExtendedTreeInsertionParameters(const ExtendedInsertionParameters& source)
      :  inherited(source)
      {  params() &= ExtendedInsertionParameters::MASK; }
   ExtendedTreeInsertionParameters(RelativePosition pos,
         AddMode duplicate=AMNoDuplicate, RemoveMode freeOnError=RMRemove)
      :  inherited(duplicate, freeOnError, pos) {}
   ExtendedTreeInsertionParameters(const ExtendedTreeInsertionParameters& source) = default;
   DefineCopy(ExtendedTreeInsertionParameters)

   ExtendedTreeInsertionParameters& setSons() { setLevelField(LMSons); return *this; }
   ExtendedTreeInsertionParameters& setInnerSons()  { setLevelField(LMInnerSons); return *this; }
   ExtendedTreeInsertionParameters& setBrothers() { clearLevelField(); return *this; }
   bool isBrother() const { return queryLevelField() == LMBrothers; }
   bool isSons() const { return queryLevelField() == LMSons; }
   bool isInnerSons() const { return queryLevelField() == LMInnerSons; }
   bool isDescendant() const { return hasLevelField(); }
   ExtendedTreeInsertionParameters& setRemote() { return (ExtendedTreeInsertionParameters&) inherited::setRemote(); }
   ExtendedTreeInsertionParameters& releaseRemote() { return (ExtendedTreeInsertionParameters&) inherited::releaseRemote(); }
   ExtendedTreeInsertionParameters& setClose() { return releaseRemote(); }

   ExtendedTreeInsertionParameters& setRelativePosition(RelativePosition pos)
      {  return (ExtendedTreeInsertionParameters&) inherited::setRelativePosition(pos);  }
   ExtendedTreeInsertionParameters& setFreeOnError(RemoveMode removeModeOnError=RMFree)
      {  return (ExtendedTreeInsertionParameters&) inherited::setFreeOnError(removeModeOnError); }
   ExtendedTreeInsertionParameters& setDuplicate()
      {  return (ExtendedTreeInsertionParameters&) inherited::setDuplicate(); }
   ExtendedTreeInsertionParameters& setDuplicate(AddMode duplicate)
      {  return (ExtendedTreeInsertionParameters&) inherited::setDuplicate(duplicate); }
   ExtendedTreeInsertionParameters& releaseDuplicate()
      {  return (ExtendedTreeInsertionParameters&) inherited::releaseDuplicate(); }

   ExtendedTreeInsertionParameters& setExact() { return (ExtendedTreeInsertionParameters&) inherited::setExact(); }
   ExtendedTreeInsertionParameters& setBefore() { return (ExtendedTreeInsertionParameters&) inherited::setBefore(); }
   ExtendedTreeInsertionParameters& setAfter() { return (ExtendedTreeInsertionParameters&) inherited::setAfter(); }
   ExtendedTreeInsertionParameters& setUndefined() { return (ExtendedTreeInsertionParameters&) inherited::setUndefined(); }
   ExtendedTreeInsertionParameters& releaseFreeOnError() { return (ExtendedTreeInsertionParameters&) inherited::releaseFreeOnError(); }
   ExtendedTreeInsertionParameters& setLocal() { return (ExtendedTreeInsertionParameters&) inherited::setLocal(); }
   ExtendedTreeInsertionParameters& clearCast() { return (ExtendedTreeInsertionParameters&) inherited::clearCast(); }
      
   typedef TReferenceParameters<ExtendedTreeLocateParameters, ExtendedTreeInsertionParameters> Locate;
   Locate queryLocate() const
      {  ExtendedTreeLocateParameters result(*this);
         result.mergeOwnField(queryLevelField());
         return Locate(result, *this);
      }
};

class ExtendedTreeSuppressParameters : public ExtendedSuppressParameters {
  private:
   typedef ExtendedSuppressParameters inherited;

  public:
   DefineExtendedParameters(2, inherited)
   DefineSubExtendedParameters(Level, 2, INHERITED)
   enum LevelMode { LMBrothers=0, LMSons=1, LMInnerSons=2 };

   ExtendedTreeSuppressParameters() {}
   ExtendedTreeSuppressParameters(RelativePosition pos, RemoveMode remove=RMRemove)
      :  inherited(remove, pos) {}
   ExtendedTreeSuppressParameters(const ExtendedSuppressParameters& source)
      :  inherited(source) { params() &= inherited::MASK; }
   ExtendedTreeSuppressParameters(const ExtendedTreeSuppressParameters& source) = default;
   DefineCopy(ExtendedTreeSuppressParameters)

   ExtendedTreeSuppressParameters& setSons() { setLevelField(LMSons); return *this; }
   ExtendedTreeSuppressParameters& setInnerSons()  { setLevelField(LMInnerSons); return *this; }
   ExtendedTreeSuppressParameters& setBrothers() { clearLevelField(); return *this; }
   bool isBrother() const { return queryLevelField() == LMBrothers; }
   bool isSons() const { return queryLevelField() == LMSons; }
   bool isInnerSons() const { return queryLevelField() == LMInnerSons; }
   bool isDescendant() const { return hasLevelField(); }

   ExtendedTreeSuppressParameters& setRelativePosition(RelativePosition pos)
      {  return (ExtendedTreeSuppressParameters&) inherited::setRelativePosition(pos);  }
   ExtendedTreeSuppressParameters& setFree(RemoveMode mode)
      {  return (ExtendedTreeSuppressParameters&) inherited::setFree(mode); }
   ExtendedTreeSuppressParameters& setFree()
      {  return (ExtendedTreeSuppressParameters&) inherited::setFree(); }
   ExtendedTreeSuppressParameters& releaseFree()
      {  return (ExtendedTreeSuppressParameters&) inherited::releaseFree(); }
   ExtendedTreeSuppressParameters& setExact() { return (ExtendedTreeSuppressParameters&) inherited::setExact(); }
   ExtendedTreeSuppressParameters& setBefore() { return (ExtendedTreeSuppressParameters&) inherited::setBefore(); }
   ExtendedTreeSuppressParameters& setAfter() { return (ExtendedTreeSuppressParameters&) inherited::setAfter(); }
   ExtendedTreeSuppressParameters& setUndefined() { return (ExtendedTreeSuppressParameters&) inherited::setUndefined(); }

   typedef TReferenceParameters<ExtendedTreeLocateParameters, ExtendedTreeSuppressParameters> Locate;
   Locate queryLocate() const
      {  ExtendedTreeLocateParameters result(*this);
         result.mergeLevelField(queryLevelField());
         return Locate(result, *this);
      }
};

class ExtendedTreeReplaceParameters : public ExtendedReplaceParameters {
  public:
   ExtendedTreeReplaceParameters() {}
   ExtendedTreeReplaceParameters(const ExtendedTreeReplaceParameters& source) = default;
   DefineCopy(ExtendedTreeReplaceParameters)

   const ExtendedTreeInsertionParameters& getInsertion() const
      {  return ((const ExtendedTreeInsertionParameters&)
            ((const ExtendedInsertionParameters&) (*this)));
      }
   ExtendedTreeInsertionParameters& getSInsertion()
      {  return ((ExtendedTreeInsertionParameters&) ((ExtendedInsertionParameters&) (*this))); }
   const ExtendedTreeSuppressParameters& getSuppress() const
      {  return ((const ExtendedTreeSuppressParameters&)
            ((const ExtendedSuppressParameters&) (*this)));
      }
   ExtendedTreeSuppressParameters& getSSuppress()
      {  return ((ExtendedTreeSuppressParameters&) ((ExtendedSuppressParameters&) (*this))); }

   ExtendedTreeReplaceParameters& setInsertionSons()
      {  getSInsertion().setSons(); return *this; }
   ExtendedTreeReplaceParameters& setInsertionInnerSons()
      {  getSInsertion().setInnerSons(); return *this; }
   ExtendedTreeReplaceParameters& setInsertionBrothers()
      {  getSInsertion().setBrothers(); return *this; }
   bool isInsertionSons() const { return getInsertion().isSons(); }
   bool isInsertionInnerSons() const { return getInsertion().isInnerSons(); }
   bool isInsertionDescendant() const { return getInsertion().hasOwnField(); }

   ExtendedTreeReplaceParameters& setRemoveSons() { getSSuppress().setSons(); return *this; }
   ExtendedTreeReplaceParameters& setRemoveInnerSons() { getSSuppress().setInnerSons(); return *this; }
   ExtendedTreeReplaceParameters& setRemoveBrothers() { getSSuppress().setBrothers(); return *this; }
   bool isRemoveSons() const { return getSuppress().isSons(); }
   bool isRemoveInnerSons() const { return getSuppress().isInnerSons(); }
   bool isRemoveDescendant() const { return getSuppress().hasOwnField(); }

   ExtendedTreeReplaceParameters& setMoving()
      {  return (ExtendedTreeReplaceParameters&) ExtendedReplaceParameters::setMoving();  }
   ExtendedTreeReplaceParameters& setFreeOnError(RemoveMode freeOnError=RMFree)
      {  return (ExtendedTreeReplaceParameters&) ExtendedReplaceParameters::setFreeOnError(freeOnError); }
   ExtendedTreeReplaceParameters& setFree(RemoveMode mode)
      {  return (ExtendedTreeReplaceParameters&) ExtendedReplaceParameters::setFree(mode); }
   ExtendedTreeReplaceParameters& setFree()
      {  return (ExtendedTreeReplaceParameters&) ExtendedReplaceParameters::setFree(); }
   ExtendedTreeReplaceParameters& setDuplicate()
      {  return (ExtendedTreeReplaceParameters&) ExtendedReplaceParameters::setDuplicate(); }
   ExtendedTreeReplaceParameters& setDuplicate(AddMode duplicate)
      {  return (ExtendedTreeReplaceParameters&) ExtendedReplaceParameters::setDuplicate(duplicate); }
   ExtendedTreeReplaceParameters& setRemovePosition(RelativePosition position)
      {  return (ExtendedTreeReplaceParameters&) ExtendedReplaceParameters::setRemovePosition(position);  }
   ExtendedTreeReplaceParameters& setInsertionPosition(RelativePosition position)
      {  return (ExtendedTreeReplaceParameters&) ExtendedReplaceParameters::setInsertionPosition(position);  }
};

}} // end of namespace COL::DVirtualTree

#endif //  COL_ExtendedParametersH

