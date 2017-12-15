/**************************************************************************/
/*                                                                        */
/*  This file is part of FLDLib                                           */
/*                                                                        */
/*  Copyright (C) 2013-2017                                               */
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
// Library   : Pointer
// Unit      : Elaborated pointers
// File      : SharedPointer.h
// Description :
//   Definition of the classes SharedPointer, SharedElement and of the template TSharedPointer.
//   These classes manage shared elements that can notify the pointers that point onto them.
//

#ifndef PNT_SharedPointerH
#define PNT_SharedPointerH

#include "Pointer/Pointer.h"
#include "Pointer/ImplList.h"

namespace PNT {

/*****************************************************/
/* Definition of template class TPointerNotification */
/*****************************************************/

template <class TypePointer>
class TPointerNotification : public EnhancedObject {
  private:
   typedef EnhancedObject inherited;
   typedef TPointerNotification<TypePointer> thisType;
   const TypePointer* pspOrigin;

  protected:
   virtual void _update(TypePointer& call) {}

  public:
   TPointerNotification() : pspOrigin(nullptr) {}
   TPointerNotification(const TypePointer& origin) : pspOrigin(&origin) {}
   TPointerNotification(const TPointerNotification<TypePointer>& source) = default;
   TemplateDefineCopy(TPointerNotification, TypePointer)
   DTemplateDefineAssign(TPointerNotification, TypePointer)

   const TypePointer* getOrigin() const { return pspOrigin; }
   void setOrigin(const TypePointer& origin)
      {  AssumeCondition(pspOrigin == nullptr) pspOrigin = &origin; }
   void clearOrigin() { pspOrigin = nullptr; }
   bool operator()(TypePointer& call) { if (&call != pspOrigin) _update(call); return true; }
};

template <class TypePointer, class TypeDerivedPointer>
class TFunctionPointerNotification : public TPointerNotification<TypePointer> {
  private:
   typedef TPointerNotification<TypePointer> inherited;
   typedef TFunctionPointerNotification<TypePointer, TypeDerivedPointer> thisType;
   std::function<void(TypeDerivedPointer&)> fFunction;

  protected:
   virtual void _update(TypePointer& call) override { fFunction((TypeDerivedPointer&) call); }

  public:
   TFunctionPointerNotification(std::function<void(TypeDerivedPointer&)> function)
      :  fFunction(function) {}
   TFunctionPointerNotification(const TypePointer& origin, std::function<void(TypeDerivedPointer&)> function)
      :  inherited(origin), fFunction(function) {}
   TFunctionPointerNotification(const TFunctionPointerNotification<TypePointer, TypeDerivedPointer>& source) = default;
   thisType& operator=(const thisType& source) = default;
   Template2DefineCopy(TFunctionPointerNotification, TypePointer, TypeDerivedPointer)
   DTemplate2DefineAssign(TFunctionPointerNotification, TypePointer, TypeDerivedPointer)
};

/*************************************/
/* Definition of class SharedPointer */
/*************************************/

class SharedElement;
class SharedPointer : public Pointer, private COL::ImplListElement {
  public:
   typedef TPointerNotification<SharedPointer> Notification;

  protected:
   // Definition of inherited methods
   void fullAssign(const SharedPointer& source);
   virtual ComparisonResult _compare(const EnhancedObject& asource) const override
      {  return Pointer::_compare(asource); }

   // memory management (may react to or trigger the destruction of the pointed element)
   friend class SharedElement;
   void setPointerElement(EnhancedObject* element) { Pointer::setElement(element); }
   void disconnect() { Pointer::release(); }
   void release();
   void notifyUpdate(Notification& notification) const;
   void notifyUpdate(std::function<void(SharedPointer&)> function) const
      {  TFunctionPointerNotification<SharedPointer, SharedPointer> notification(function);
         notifyUpdate(notification);
      }
   typedef HandlerCast<SharedPointer, COL::ImplListElement> HandlerRegisterCast;

   friend class COL::TImplList<SharedPointer, HandlerRegisterCast>;
   friend class HandlerCast<SharedPointer, COL::ImplListElement>;

  public:
   SharedPointer() {}
   SharedPointer(SharedElement* element, Init);
   SharedPointer(const SharedPointer& source);
   virtual ~SharedPointer();
   SharedPointer& operator=(const SharedPointer& source);
   StaticInheritConversions(SharedPointer, Pointer)

   DefineCopy(SharedPointer)
   DDefineAssign(SharedPointer)
   DCompare(SharedPointer)
   void setElement(SharedElement& element);
   SharedPointer& assign(SharedElement* element, Init);

   void swap(SharedPointer& source);

   SharedElement& getElement() const { return (SharedElement&) Pointer::getElement(); }
   const SharedElement& getCElement() const
      {  return (const SharedElement&) Pointer::getElement(); }
   const SharedElement* key() const { return &getElement(); }
   SharedElement* operator->() const { return &getElement(); }
   SharedElement& operator*() const { return getElement(); }
   virtual bool isValid() const override { return Pointer::isValid() && COL::ImplListElement::isValid(); }

   typedef SharedElement Element;
};

/*************************************/
/* Definition of class SharedElement */
/*************************************/

class SharedElement : public EnhancedObject {
  public:
   typedef TPointerNotification<SharedPointer> Notification;

  protected:
   typedef COL::TImplList<SharedPointer, SharedPointer::HandlerRegisterCast> SharedPointerList;

  private:
   SharedPointerList splCalls;

  protected:
   friend class SharedPointer;
   SharedPointerList& calls() { return splCalls; }
   const SharedPointerList& calls() const { return splCalls; }
   bool unlock(SharedPointer& call)
      {  AssumeCondition(call.key() == this)
         splCalls.remove(&call);
         call.disconnect();
         return splCalls.isEmpty();
      }
   void lock(SharedPointer& call)
      {  AssumeCondition(!call.Pointer::isValid())
         splCalls.add(&call);
         call.setPointerElement(this);
      }
   void notifyUpdate(Notification& notification) const
      {  if (!splCalls.isEmpty() && ((!notification.getOrigin() || !splCalls.isSingleton())))
            splCalls.foreachDo(notification);
      }
   void notifyUpdate(std::function<bool (SharedPointer&)>&& notification) const
      {  if (!splCalls.isEmpty())
            splCalls.foreachDo(notification);
      }
   void freeAllCalls()
      {  splCalls.foreachDo([](SharedPointer& call){ call.disconnect(); return true; });
         splCalls.freeAll();
      }
   void removeAllCalls()
      {  splCalls.foreachDo([](SharedPointer& call){ call.disconnect(); return true; });
         splCalls.removeAll();
      }
   void moveAllCallsTo(SharedElement& destination)
      {  splCalls.foreachDo([&destination](SharedPointer& call)
            {  call.setPointerElement(&destination); return true; });
         splCalls.moveAllTo(destination.splCalls);
      }

  public:
   SharedElement() {}
   SharedElement(const SharedElement& source) = default;
   virtual ~SharedElement() { removeAllCalls(); }
   SharedElement& operator=(const SharedElement& source)
      {  return (SharedElement&) EnhancedObject::operator=(source); }
   DefineCopy(SharedElement)
   DDefineAssign(SharedElement)

   bool isReferenced() const { return !splCalls.isEmpty(); }
   bool isSingleReferenced() const
      {  SharedPointer* firstCall = splCalls.getFirst();
         return firstCall && (firstCall == splCalls.getLast());
      }
   bool isDoubleReferenced() const
      {  return !splCalls.isEmpty()
            && (splCalls.getNext(splCalls.getFirst()) == splCalls.getLast());
      }
};

/* Inline methods of class SharedPointer */

inline
SharedPointer::SharedPointer(SharedElement* element, Init)
   {  AssumeAllocation(element) element->lock(*this); }

inline
SharedPointer::SharedPointer(const SharedPointer& source)
   {  if (source.SharedPointer::isValid()) source.getElement().lock(*this); }

inline
SharedPointer::~SharedPointer() { if (Pointer::isValid()) getElement().unlock(*this); }

inline void
SharedPointer::notifyUpdate(Notification& notification) const {
   AssumeCondition(SharedPointer::isValid())
   notification.setOrigin(*this);
   getCElement().notifyUpdate(notification);
}

/*****************************************/
/* Definition of template TSharedPointer */
/*****************************************/

template <class TypeElement, class Cast=HandlerCast<TypeElement, SharedElement> >
class TSharedPointer : public SharedPointer {
  private:
   typedef SharedPointer inherited;
   typedef TSharedPointer<TypeElement, Cast> thisType;
   
  protected:
   void notifyUpdate(SharedPointer::Notification& notification) const
      {  inherited::notifyUpdate(notification); }
   void notifyUpdate(std::function<void(TSharedPointer<TypeElement, Cast>&)> function) const
      {  TFunctionPointerNotification<SharedPointer, thisType> notification(function);
         inherited::notifyUpdate(notification);
      }
   
  public:
   TSharedPointer() {}
   TSharedPointer(TypeElement* element, Init)
      :  SharedPointer((SharedElement*) Cast::castTo(element), Pointer::Init()) {}
   TSharedPointer(const thisType& source) : inherited(source) {}
   Template2DefineCopy(TSharedPointer, TypeElement, Cast)

   void setElement(TypeElement& element)
      {  inherited::setElement((SharedElement&) Cast::castTo(element)); }
   thisType& assign(TypeElement* element, Init init)
      {  return (thisType&) inherited::assign((SharedElement*) Cast::castTo(element), init); }
   TypeElement& getElement() const { return (TypeElement&) Cast::castFrom(inherited::getElement()); }
   const TypeElement& getCElement() const
      {  return (const TypeElement&) Cast::castFrom(inherited::getCElement()); }
   const TypeElement* key() const { return &getElement(); }
   TypeElement* operator->() const { return &getElement(); }
   TypeElement& operator*() const { return getElement(); }
   void swap(TSharedPointer<TypeElement, Cast>& source) { inherited::swap(source); }
};

} // end of namespace PNT

#endif // PNT_SharedPointerH
