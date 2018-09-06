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
// Library   : Collection
// Unit      : Low level implementation
// File      : ImplArray.h
// Description :
//   Definition of a class of arrays. It provides an implementation
//   for the COL::TArray collection.
//

#ifndef COL_ImplArrayH
#define COL_ImplArrayH

#include "Pointer/Vector.h"

namespace COL {

/*********************************/
/* Definition of class ImplArray */
/*********************************/

class ImplArray : public EnhancedObject {
  private:
   EnhancedObject** apeoArray;
   int uCount;
   int uAllocated;
   typedef DVirtualCollection::VirtualCastWithElement VirtualCastWithElement;

  protected:
   void realloc(int newAllocated);
   void bookPlace(int places=1) { if (uCount+places > uAllocated) _realloc(uCount+places); }

   // Methods to override
   virtual void _realloc(int newSize) { realloc((newSize <= 2) ? 4 : (newSize*3/2)); }

   // query methods
#if defined __GNUC__ && GCC_VERSION >= 40600
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wstrict-overflow"
#endif
   PEnhancedObject& referenceAt(int index) const
      {  AssumeCondition((index >= 0) && (index < uCount)) return apeoArray[index]; }
#if defined __GNUC__ && GCC_VERSION >= 40600
#pragma GCC diagnostic pop
#endif
   EnhancedObject** array() const { return apeoArray; }
   int queryPlaces() const { return uAllocated-uCount; }

   // block operations
   void addAtEnd(const ImplArray& source, int firstSource, int lastSource,
         bool duplicate, const VirtualCast* retrieveRegistrationFromCopy=nullptr);
   void moveToAtEnd(ImplArray& destination, int first, int last);

  public:
   const int& queryAllocated() const { return uAllocated; }
   void compress();
   template <class TemplateParameters> void bubbleSort(const TemplateParameters& parameters, int first, int last);
   template <class TemplateParameters> void quickSort(const TemplateParameters& parameters, int first, int last);
   template <class TemplateParameters> void selectNth(const TemplateParameters& parameters, int nb, int first, int last);
   template <class TemplateParameters> int localize(typename TemplateParameters::Key::KeyType key,
         const TemplateParameters& parameters, int first=0, int last=-1) const;
   template <class TemplateParameters> int merge(const TemplateParameters& parameters,
         const ImplArray& source, int firstSource, int lastSource, bool doesDuplicate,
         const VirtualCast* retrieveRegistrationFromCopy=nullptr);

  public:
   ImplArray() : apeoArray(nullptr), uCount(0), uAllocated(0) {}
   ImplArray(ImplArray&& source)
      :  apeoArray(source.apeoArray), uCount(source.uCount), uAllocated(source.uAllocated)
      {  source.apeoArray = nullptr; source.uCount = 0; source.uAllocated = 0; }
   ImplArray(const ImplArray& source, bool doesDuplicate=false,
         const VirtualCast* retrieveRegistrationFromCopy=nullptr);
   virtual ~ImplArray() { if (apeoArray) delete [] apeoArray; }
   DefineCopy(ImplArray)
   DDefineAssign(ImplArray)
   ImplArray& operator=(ImplArray&& source)
      {  if (this != &source) {
            if (apeoArray) delete [] apeoArray;
            apeoArray = source.apeoArray;
            uCount = source.uCount;
            uAllocated = source.uAllocated;
            source.apeoArray = nullptr;
            source.uCount = 0;
            source.uAllocated = 0;
         };
         return *this;
      }
   ImplArray& operator=(const ImplArray& source)
      {  EnhancedObject::operator=(source);
         assign(source, false);
         return *this;
      }
   void assign(const ImplArray& source, bool doesDuplicate,
      const VirtualCast* retrieveRegistrationFromCopy=nullptr);
   void moveAllTo(ImplArray& destination, int firstIndex=0, int lastIndex=-1, int destinationIndex=-1);
   void addAll(const ImplArray& source, int firstSourceIndex=0, int lastSourceIndex=-1,
      int destinationIndex=-1, bool doesDuplicate=false,
      const VirtualCast* retrieveRegistrationFromCopy=nullptr);

   void insertAt(int index, EnhancedObject* anObject);
   void insertAtEnd(EnhancedObject* anObject)
      {  AssumeAllocation(anObject) bookPlace();
         apeoArray[uCount++] = anObject;
      }
   void removeAt(int index);
   void removeAtEnd() { AssumeCondition(uCount > 0) apeoArray[--uCount] = nullptr; }
   void removeAll(bool doesFree=false);
   void removeAllBetween(int firstIndex, int lastIndex, bool doesFree=false);
   void swap(ImplArray& source);

   const PEnhancedObject& elementAt(int index) const { return referenceAt(index); }
   const PEnhancedObject& operator[](int index) const { return referenceAt(index); }

   const int& count() const { return uCount; }
   template <class Execute, class TemplateParameters> bool foreachDo(TemplateParameters parameters,
      Execute& function, int startIndex=0, int endIndex=-1) const;
   template <class Execute, class TemplateParameters> bool foreachReverseDo(TemplateParameters parameters,
      Execute& function, int startIndex=-1, int endIndex=0) const;
   bool foreachDo(std::function<bool (EnhancedObject&)> func, int startIndex=0, int endIndex=-1) const;
   bool foreachReverseDo(std::function<bool (EnhancedObject&)> func, int startIndex=-1, int endIndex=0) const;

   virtual bool isValid() const override
      {  return EnhancedObject::isValid() && (uCount >= 0) && (uCount <= uAllocated); }
};

template <class Execute, class TemplateParameters>
inline bool
ImplArray::foreachDo(TemplateParameters /* parameters */, Execute& function, int index, int endIndex) const {
   typedef typename TemplateParameters::Element Element;
   typedef typename TemplateParameters::Cast Cast;

   if (endIndex < 0) endIndex = uCount-1;
   for (; index <= endIndex; index++)
      if (!function((Element&) *Cast::castFrom(apeoArray[index])))
         return false;
   return true;
}

template <class Execute, class TemplateParameters>
inline bool
ImplArray::foreachReverseDo(TemplateParameters /* parameters */, Execute& function, int index, int endIndex) const {
   typedef typename TemplateParameters::Element Element;
   typedef typename TemplateParameters::Cast Cast;

   if (index < 0) index = uCount-1;
   if (endIndex < 0) endIndex = 0;
   for (; index >= endIndex; index--)
      if (!function((Element&) *Cast::castFrom(apeoArray[index])))
         return false;
   return true;
}

inline bool
ImplArray::foreachDo(std::function<bool (EnhancedObject&)> function, int index, int endIndex) const {
   if (endIndex < 0) endIndex = uCount-1;
   for (; index <= endIndex; index++)
      if (!function(*apeoArray[index]))
         return false;
   return true;
}

inline bool
ImplArray::foreachReverseDo(std::function<bool (EnhancedObject&)> function, int index, int endIndex) const {
   if (index < 0) index = uCount-1;
   if (endIndex < 0) endIndex = 0;
   for (; index >= endIndex; index--)
      if (!function(*apeoArray[index]))
         return false;
   return true;
}

template <class Element, class Cast=SimpleCast>
class TImplArray : public ImplArray {
  public:
   TImplArray() {}
   TImplArray(TImplArray<Element, Cast>&& source) : ImplArray(source) {}
   TImplArray(const TImplArray<Element, Cast>& source, bool doesDuplicate=false)
      :  ImplArray(source, doesDuplicate) {}
   TImplArray<Element, Cast>& operator=(const TImplArray<Element, Cast>& source) = default;
   TImplArray<Element, Cast>& operator=(TImplArray<Element, Cast>&& source) = default;
   Template2DefineCopy(TImplArray, Element, Cast)
   void assign(const TImplArray<Element, Cast>& source, bool doesDuplicate)
      {  ImplArray::assign(source, doesDuplicate); }
   void moveAllTo(TImplArray<Element, Cast>& destination, int firstIndex=0,
         int lastIndex=-1, int destinationIndex=-1)
      {  ImplArray::moveAllTo(destination, firstIndex, lastIndex, destinationIndex); }
   void addAll(const TImplArray<Element, Cast>& source, int firstSourceIndex=0, int lastSourceIndex=-1,
         int destinationIndex=-1, bool doesDuplicate=false)
      {  ImplArray::addAll(source, firstSourceIndex, lastSourceIndex, destinationIndex, doesDuplicate); }

   void insertAt(int index, Element* newElement)
      {  ImplArray::insertAt(index, Cast::castTo(newElement)); }
   void insertAtEnd(Element* newElement)
      {  ImplArray::insertAtEnd(Cast::castTo(newElement)); }
   void swap(TImplArray<Element, Cast>& source) {  ImplArray::swap(source); }

   Element* elementAt(int index) const
      { return (Element*) Cast::castFrom(ImplArray::elementAt(index)); }
   Element* operator[](int index) const
      { return (Element*) Cast::castFrom(ImplArray::operator[](index)); }

   template <class Execute>
   bool foreachDo(Execute& function, int startIndex=0, int endIndex=-1) const
      {  return ImplArray::foreachDo(TemplateElementCastParameters<Element, Cast>(),
            function, startIndex, endIndex);
      }
   template <class Execute>
   bool foreachReverseDo(Execute& function, int startIndex=0, int endIndex=-1) const
      {  return ImplArray::foreachReverseDo(TemplateElementCastParameters<Element, Cast>(),
            function, startIndex, endIndex);
      }
   bool foreachDo(std::function<bool (Element&)> function, int startIndex=0, int endIndex=-1) const
      {  return ImplArray::foreachDo(TemplateElementCastParameters<Element, Cast>(),
            function, startIndex, endIndex);
      }
   bool foreachReverseDo(std::function<bool (Element&)> function, int startIndex=-1, int endIndex=0) const
      {  return ImplArray::foreachReverseDo(TemplateElementCastParameters<Element, Cast>(),
            function, startIndex, endIndex);
      }
};

} // end of namespace COL

#endif


