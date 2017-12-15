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
// Unit      : Basic pointers
// File      : MngPointer.h
// Description :
//   Definition of the template MngPointer to manage the sharing of objects
//   through simple reference counters.
//

#ifndef PNT_MngPointerH
#define PNT_MngPointerH

#include "Pointer/Pointer.h"

namespace PNT {

/*******************************************/
/* Definition of the base class MngElement */
/*******************************************/

class MngPointer;
class MngElement : public EnhancedObject {
  private:
   int uReferenced;

  protected:
   friend class MngPointer;
   bool unlock() { AssumeCondition(uReferenced > 0) return (--uReferenced == 0); }
   void lock() { ++uReferenced; }
   const int& getReferencedCounter() const { return uReferenced; }

  public:
   MngElement() : uReferenced(0) {}
   MngElement(const MngElement& source) : EnhancedObject(source), uReferenced(0) {}
   DefineCopy(MngElement)
   DDefineAssign(MngElement)
   MngElement& operator=(const MngElement& source)
      {  return (MngElement&) EnhancedObject::operator=(source); }
   
   MngPointer acquirePointer();
   bool isReferenced() const { return uReferenced > 0; }
   bool isSingleReferenced() const { return uReferenced == 1; }
};

class MngPointer : public Pointer {
  protected:
   class Acquire {};
   MngPointer(MngElement* element, Acquire) : Pointer(element, Init())
      {  element->lock(); }
   friend class MngElement;

  public:
   MngPointer() {}
   MngPointer(const MngElement& element) : Pointer(element.createCopy(), Init())
      {  getElement().lock(); }
   MngPointer(MngElement* element, Init) : Pointer(element, Init())
      {  element->lock(); AssumeCondition(element->getReferencedCounter() == 1); }
   MngPointer(const MngPointer& source, Duplicate)
      :  Pointer(source.Pointer::isValid() ? source.Pointer::getElement().createCopy() : nullptr, Init())
      {  getElement().lock(); }
   MngPointer(const MngPointer& source) : Pointer(source)
      {  if (Pointer::isValid()) getElement().lock(); }
   MngPointer& operator=(const MngPointer& source);
   virtual ~MngPointer() { release(); }

   DefineCopy(MngPointer)
   DDefineAssign(MngPointer)

   void assign(MngElement* newElement, Init);
   void fullAssign(const MngPointer& source);
   void absorbElement(MngElement* element) { assign(element, Init()); }
   void setElement(const MngElement& element) { assign(element.createSCopy(), Init()); }
   void swap(MngPointer& source) { Pointer::swap(source); }
   void release()
      {  if (Pointer::isValid())
            Pointer::release(getElement().unlock());
      }

   MngElement& getElement() const { return (MngElement&) Pointer::getElement(); }
   const MngElement& getCElement() const { return (const MngElement&) Pointer::getElement(); }
   MngElement* key() const { return (MngElement*) Pointer::key(); }
   MngElement* operator->() const { return &getElement(); }
   MngElement& operator*() const { return getElement(); }
};

inline MngPointer
MngElement::acquirePointer() { return MngPointer(this, MngPointer::Acquire()); }

/**************************************/
/* Definition of template TMngPointer */
/**************************************/

template<class Element, class Cast=HandlerCopyCast<Element> >
class TMngPointer : public MngPointer {
  protected:
   TMngPointer(const MngPointer& source) : MngPointer(source) {}

  public:
   typedef Element* PElement;
   typedef MngPointer::Duplicate Duplicate;

   TMngPointer() {}
   TMngPointer(const Element& element) : MngPointer((const MngElement&) Cast::castTo(element)) {}
   TMngPointer(Element* element, Init)
      :  MngPointer((MngElement*) Cast::castTo(element), Init()) {}
   TMngPointer(const TMngPointer<Element, Cast>& source, Duplicate)
      :  MngPointer(source, Duplicate()) {}
   TMngPointer(const TMngPointer<Element, Cast>& source) : MngPointer(source) {}

   Template2DefineCopy(TMngPointer, Element, Cast)
   DTemplate2Assign(TMngPointer, Element, Cast)
   void assign(Element* newElement, Init)
      {  MngPointer::assign((MngElement*) Cast::castTo(newElement), Init()); }
   void fullAssign(const TMngPointer<Element, Cast>& source)
      {  MngPointer::fullAssign(source); }
   void setElement(const Element& element)
      {  MngPointer::setElement((const MngElement&) Cast::castTo(element)); }
   void absorbElement(Element* element)
      {  MngPointer::absorbElement((MngElement*) Cast::castTo(element)); }
   void swap(TMngPointer<Element, Cast>& source) { MngPointer::swap(source); }

   Element& getElement() const { return (Element&) Cast::castFrom(MngPointer::getElement()); }
   const Element& getCElement() const { return (const Element&) Cast::castFrom(MngPointer::getCElement()); }
   Element* key() const { return (Element*) Cast::castFrom(MngPointer::key()); }
   Element* operator->() const { return &getElement(); }
   Element& operator*() const { return getElement(); }
};

}; // end of namespace

#endif // PNT_MngPointerH
