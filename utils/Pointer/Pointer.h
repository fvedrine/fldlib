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
// File      : Pointer.h
// Description :
//   Definition of the class Pointer that inherits from EnhancedObject.
//

#ifndef PNT_PointerH
#define PNT_PointerH

#include "StandardClasses/StandardClasses.hpp"

namespace PNT {

class Pointer : public EnhancedObject {
  public:
   class Init {};
   class Duplicate {};

  private:
   EnhancedObject* peoElement;

  protected:
   enum TypePointer { TPStandard, TPAuto, TPPass };

   void _lightAssign(const EnhancedObject& source) { EnhancedObject::_assign(source); }
   virtual ComparisonResult _compare(const EnhancedObject& source) const override
      {  ComparisonResult result = EnhancedObject::_compare(source);
         return (result == CREqual) ?
            peoElement->compare(*((const Pointer&) source).peoElement) : result;
      }

   Pointer() : peoElement(nullptr) {}
   Pointer(EnhancedObject* element, Init) : peoElement(element) { AssumeAllocation(element) }
   Pointer(const Pointer& source) = default;

   EnhancedObject* key() const { return peoElement; }
   EnhancedObject* operator->() const { AssumeCondition(peoElement) return peoElement; }
   EnhancedObject& operator*() const { AssumeCondition(peoElement) return *peoElement; }
   EnhancedObject& getElement() const { AssumeCondition(peoElement) return *peoElement; }
   const EnhancedObject& getCElement() const { AssumeCondition(peoElement) return *peoElement; }
   void setElement(EnhancedObject* element) { peoElement = element; }
   void release(bool doesFree=false)
      {  if (doesFree && peoElement) delete peoElement; peoElement = nullptr; }
   void swap(Pointer& source)
      {  EnhancedObject* temp = peoElement;
         peoElement = source.peoElement;
         source.peoElement = temp;
      }
   void fullAssign(const Pointer& source, TypePointer type);
   void fullAssign(EnhancedObject* element, TypePointer type, Init);

  public:
   DefineCopy(Pointer)
   DDefineAssign(Pointer)

   virtual bool isValid() const override { return peoElement != nullptr; }
};

template<class Element, class Cast=SimpleCast>
class TPointer : public Pointer {
  public:
   typedef Element* PElement;

   TPointer() {}
   TPointer(Element* element, Init) : Pointer(Cast::castTo(element), Pointer::Init()) {}
   TPointer(const Element& element) : Pointer(Cast::castTo(&element), Pointer::Init()) {}
   TPointer(const TPointer<Element, Cast>& source) = default;

   Template2DefineCopy(TPointer, Element, Cast)
   DTemplate2Assign(TPointer, Element, Cast)
   void assign(Element* newElement, Init)
      {  Pointer::setElement(Cast::castTo(newElement)); }
   void setElement(const Element& element) { assign(&element, Pointer::Init()); }
   void swap(TPointer<Element, Cast>& source) { Pointer::swap(source); }
   void release() { Pointer::release(); }

   Element* key() const
      { return Pointer::isValid() ? (Element*) Cast::castFrom(&Pointer::getElement()) : nullptr; }
   Element* operator->() const { return (Element*) Cast::castFrom(Pointer::operator->()); }
   Element& operator*() const { return *(operator->()); }
};

} // end of namespace PNT

#endif
