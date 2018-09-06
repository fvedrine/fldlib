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
// File      : AutoPointer.h
// Description :
//   Definition of the template AutoPointer to hold control on the pointed element.
//

#ifndef PNT_AutoPointerH
#define PNT_AutoPointerH

#include "Pointer/PassPointer.h"

namespace PNT {

/***********************************/
/* template AutoPointer definition */
/***********************************/

template<class Element>
class AutoPointer : public Pointer {
  public:
   typedef Element* PElement;

   AutoPointer() {}
   AutoPointer(Element* element, Init) : Pointer(Element::castToCopyHandler(element), Init()) {}
   AutoPointer(const Element& element) : Pointer(element.createCopy(), Init()) {}
   AutoPointer(const AutoPointer<Element>& source) : Pointer()
      {  Pointer::fullAssign(source, Pointer::TPAuto); }
   AutoPointer(const PassPointer<Element>& source)
      {  PassPointer<Element> copy(source);
         Pointer::swap(copy);
      }
   AutoPointer(CPassPointer<Element>&& source)
      {  CPassPointer<Element> copy(source);
         Pointer::swap(copy);
      }
   virtual ~AutoPointer() { Pointer::release(true); }

   TemplateDefineCopy(AutoPointer, Element)
   DTemplateDefineAssign(AutoPointer, Element)
   AutoPointer<Element>& operator=(const AutoPointer<Element>& source)
      {  Pointer::fullAssign(source, TPAuto); return *this; }
   AutoPointer<Element>& operator=(const PassPointer<Element>& source) { assign(source); return *this; }
   AutoPointer<Element>& operator=(CPassPointer<Element>&& source) { assign(source); return *this; }
   void assign(const PassPointer<Element>& source)
      {  PassPointer<Element> copy(source);
         Pointer::swap(copy);
      }
   void assign(CPassPointer<Element>&& source)
      {  CPassPointer<Element> copy(source);
         Pointer::swap(copy);
      }
   void assign(Element* newElement, Init)
      {  Pointer::fullAssign(Element::castToCopyHandler(newElement), TPAuto, Init()); }
   void absorbElement(Element* element) { assign(element, Init()); }
   void setElement(const Element& element) { assign(element.createSCopy(), Init()); }
   void swap(AutoPointer<Element>& source) { Pointer::swap(source); }
   Element* extractElement()
      {  Element* result = (Element*) Element::castFromCopyHandler(&Pointer::getElement());
         Pointer::release(); return result;
      }
   void release() { Pointer::release(true); }

   Element& getElement() const { return (Element&) Element::castFromCopyHandler(Pointer::getElement()); }
   const Element& getCElement() const { return (const Element&) Element::castFromCopyHandler(Pointer::getCElement()); }
   operator Element*() const { return &getElement(); }
   const Element* key() const { return &getElement(); }
   Element* operator->() const { return &getElement(); }
   Element& operator*() const { return getElement(); }
};

/********************************************/
/* template TAutoPointer definition         */
/* (works even if Element is only declared) */
/********************************************/

template<class Element>
class TAutoPointer : public EnhancedObject {
  private:
   Element* peElement;

  protected:
   virtual ComparisonResult _compare(const EnhancedObject& source) const;

  public:
   class Init {};
   typedef Element* PElement;

   TAutoPointer() : peElement(nullptr) {}
   TAutoPointer(Element* element, Init) : peElement(element) {}
   TAutoPointer(const Element& element);
   TAutoPointer(const TAutoPointer<Element>& source);
   virtual ~TAutoPointer();

   TemplateDefineCopy(TAutoPointer, Element)
   DTemplateDefineAssign(TAutoPointer, Element)
   TAutoPointer<Element>& operator=(const TAutoPointer<Element>& source);
   void assign(Element* newElement, Init);
   void setElement(const Element& element);
   void swap(TAutoPointer<Element>& source);
   Element* extractElement() { Element* result = peElement; peElement = nullptr; return result; }
   void absorbElement(Element* element) { assign(element, Init()); }
   void release();

   operator Element*() const { return peElement; }
   const Element* key() const { return peElement; }
   operator const PElement&() const { return peElement; }
   virtual bool isValid() const { return peElement != nullptr; }

   Element* operator->() const { AssumeCondition(peElement) return peElement; }
   Element& operator*() const { AssumeCondition(peElement) return *peElement; }
};

template <class Element>
inline void
TAutoPointer<Element>::swap(TAutoPointer<Element>& source) {
   Element* temp = source.peElement;
   source.peElement = peElement;
   peElement = temp;
}

}

#endif // PNT_AutoPointerH
