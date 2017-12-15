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
// Fichier     : AbstractCollection.h
// Description :
//   Definition of the class AbstractCollection.
//   It implements the cursor notion and it is the base class of all collections.
//   A generic interface will be given by the class VirtualCollection.
//   The interface of AbstractCollection is rather minimal.
//

#ifndef COL_AbstractCollectionH
#define COL_AbstractCollectionH

#include "Pointer/ExtPointer.hpp"
#include "Collection/VirtualCollection/AbstractCollection.macro"

namespace COL {

class AbstractCursor;
class PPAbstractCursor;
class PureAbstractCollection : public EnhancedObject {
  public:
   PureAbstractCollection() {}
   PureAbstractCollection(const PureAbstractCollection& source) : EnhancedObject(source) {}
   DefineCopy(PureAbstractCollection)
};

class AbstractCollection : public PureAbstractCollection, private PNT::SharedElement {
  protected:
   class CursorNotification;

   virtual ComparisonResult _compare(const EnhancedObject& asource) const override
      {  return PureAbstractCollection::_compare(asource); }

   AbstractCollection() {}
   AbstractCollection(const AbstractCollection& source)
      :  PureAbstractCollection(source), PNT::SharedElement(source) {}

   virtual AbstractCursor* createNewCursor() const;
   virtual void assumeCompatibility(AbstractCursor& cursor) const;
   void notifyCursorUpdate(CursorNotification& notification) const;
   void notifyCursorUpdate(CursorNotification& notification, const AbstractCursor& cursor) const;

   PPAbstractCursor snewCursor() const;
   static void disableCursors(const AbstractCollection& collection)
     { const_cast<AbstractCollection&>(collection).PNT::SharedElement::removeAllCalls(); }

  public:
   class CastCursorHandler;
   class CastCollectionHandler;
   DefineCopy(AbstractCollection)
   StaticInheritConversions(AbstractCollection, PureAbstractCollection)

   PPAbstractCursor newCursor() const;
   virtual bool isValid() const override { return PNT::SharedElement::isValid(); }
};

/******************************/
/* Definition of cast classes */
/******************************/

class AbstractCollection::CastCursorHandler {
  public:
   static PNT::SharedElement& castTo(AbstractCollection& source)
      { return (PNT::SharedElement&) source; }
   static const PNT::SharedElement& castTo(const AbstractCollection& source)
      { return (const PNT::SharedElement&) source; }
   static AbstractCollection& castFrom(PNT::SharedElement& source)
      { return (AbstractCollection&) source; }
   static const AbstractCollection& castFrom(const PNT::SharedElement& source)
      { return (const AbstractCollection&) source; }

   static PNT::SharedElement* castTo(AbstractCollection* source)
      { return (PNT::SharedElement*) source; }
   static const PNT::SharedElement* castTo(const AbstractCollection* source)
      { return (const PNT::SharedElement*) source; }
   static AbstractCollection* castFrom(PNT::SharedElement* source)
      { return (AbstractCollection*) source; }
   static const AbstractCollection* castFrom(const PNT::SharedElement* source)
      { return (const AbstractCollection*) source; }
};

class AbstractCollection::CastCollectionHandler {
  public:
   static AbstractCollection& castTo(AbstractCollection& source) { return source; }
   static const AbstractCollection& castTo(const AbstractCollection& source) { return source; }
   static AbstractCollection& castFrom(AbstractCollection& source) { return source; }
   static const AbstractCollection& castFrom(const AbstractCollection& source) { return source; }

   static AbstractCollection* castTo(AbstractCollection* source) { return source; }
   static const AbstractCollection* castTo(const AbstractCollection* source) { return source; }
   static AbstractCollection* castFrom(AbstractCollection* source) { return source; }
   static const AbstractCollection* castFrom(const AbstractCollection* source) { return source; }
};

/*********************/
/* cursor definition */
/*********************/

class PureAbstractCursor : public EnhancedObject {
  public:
   PureAbstractCursor() {}
   PureAbstractCursor(const PureAbstractCursor& source) = default;
   DefineCopy(PureAbstractCursor)
};

typedef PNT::TSharedPointer<AbstractCollection, AbstractCollection::CastCursorHandler> SPAbstractCollection;
class AbstractCursor : public PureAbstractCursor, private SPAbstractCollection {
  protected:
   virtual ComparisonResult _compare(const EnhancedObject& asource) const override
      {  ComparisonResult result = PureAbstractCursor::_compare(asource);
         AssumeCondition(&getSSupport() == &castFromCopyHandler(asource).getSSupport())
         return result;
      }

   const AbstractCollection& getSSupport() const { return getCElement(); }
   static AbstractCollection& getSupport(const AbstractCursor& cursor) { return cursor.getElement(); }

   AbstractCursor(const AbstractCollection& support)
      : SPAbstractCollection(&const_cast<AbstractCollection&>(support), Init()) {}
   AbstractCursor(const AbstractCursor& source) = default;

   virtual bool _isEqual(const AbstractCursor& cursor) const
      {  return (&getSupport() == &cursor.getSupport()); }
   void swap(AbstractCursor& source) { SPAbstractCollection::swap(source); }

  public:
   class CastSupportHandler;
   AbstractCursor& operator=(const AbstractCursor& source)
      {  PureAbstractCursor::operator=(source);
         AssumeCondition(&getSSupport() == &source.getSSupport())
         SPAbstractCollection::operator=(source);
         return *this;
      }

   DefineCopy(AbstractCursor)
   DDefineAssign(AbstractCursor)
   DCompare(AbstractCursor)
   DefineCursorForAbstractCollect(AbstractCollection, AbstractCursor)
   StaticInheritConversions(AbstractCursor, PureAbstractCursor)

   virtual bool isValid() const override
      { AssumeCondition(SPAbstractCollection::isValid()) return true; }
   bool isEqual(const AbstractCursor& cursor) const { return _isEqual(cursor); }
   bool hasSupport() const { return SPAbstractCollection::isValid(); }
};

class PPAbstractCursor : public PNT::PassPointer<AbstractCursor> {
  public:
   PPAbstractCursor() {}
   PPAbstractCursor(AbstractCursor* cursor, Init)
      :  PNT::PassPointer<AbstractCursor>(cursor, Init()) {}
   PPAbstractCursor(const AbstractCursor& cursor) : PNT::PassPointer<AbstractCursor>(cursor) {}
   PPAbstractCursor(const PPAbstractCursor& source) = default;
   PPAbstractCursor(const PPAbstractCursor& source, Duplicate duplicate)
      :  PNT::PassPointer<AbstractCursor>(source, duplicate) {}
   DefineCopy(PPAbstractCursor)
   void assign(AbstractCursor* cursor, Init)
      {  PNT::PassPointer<AbstractCursor>::assign(cursor, Init()); }
};

/*****************************************/
/* Definition of cast classes on cursors */
/*****************************************/

class AbstractCursor::CastSupportHandler {
  public:
   static PNT::SharedPointer& castTo(AbstractCursor& cursor)
      { return (PNT::SharedPointer&) cursor; }
   static const PNT::SharedPointer& castTo(const AbstractCursor& cursor)
      { return (const PNT::SharedPointer&) cursor; }
   static AbstractCursor& castFrom(PNT::SharedPointer& source)
      { return (AbstractCursor&) source; }
   static const AbstractCursor& castFrom(const PNT::SharedPointer& source)
      { return (const AbstractCursor&) source; }

   static PNT::SharedPointer* castTo(AbstractCursor* cursor)
      { return (PNT::SharedPointer*) cursor; }
   static const PNT::SharedPointer* castTo(const AbstractCursor* cursor)
      { return (const PNT::SharedPointer*) cursor; }
   static AbstractCursor* castFrom(PNT::SharedPointer* source)
      { return (AbstractCursor*) source; }
   static const AbstractCursor* castFrom(const PNT::SharedPointer* source)
      { return (const AbstractCursor*) source; }
};

/***********************************************/
/* Definition of inline methods on collections */
/***********************************************/

class AbstractCollection::CursorNotification
   :  public PNT::SharedPointer::Notification, protected AbstractCursor::CastSupportHandler {
  private:
   typedef PNT::SharedPointer::Notification inherited;

  protected:
   void update(AbstractCursor& cursor) {}

  public:
   CursorNotification() {}
   CursorNotification(const CursorNotification& source) : inherited(source) {}
   DefineCursorNotificationMethods(CursorNotification, AbstractCursor)

   void setOrigin(const AbstractCursor& originCursor)
      {  inherited::setOrigin(AbstractCursor::CastSupportHandler::castTo(originCursor)); }
};

inline void
AbstractCollection::assumeCompatibility(AbstractCursor& cursor) const
   {  AssumeCondition(&cursor.getSupport() == this) }

inline void
AbstractCollection::notifyCursorUpdate(CursorNotification& notification) const
   {  PNT::SharedElement::notifyUpdate(notification); }

inline void
AbstractCollection::notifyCursorUpdate(CursorNotification& notification,
      const AbstractCursor& cursor) const
   {  notification.setOrigin(cursor);
      PNT::SharedElement::notifyUpdate(notification);
   }

inline AbstractCursor*
AbstractCollection::createNewCursor() const
   {  AbstractCursor* result = new AbstractCursor(*this);
      AssumeAllocation(result) return result;
   }

inline PPAbstractCursor
AbstractCollection::snewCursor() const
   { return PPAbstractCursor(createNewCursor(), PPAbstractCursor::Init()); }

inline PPAbstractCursor
AbstractCollection::newCursor() const { return snewCursor(); }

} // end of namespace COL

#endif

