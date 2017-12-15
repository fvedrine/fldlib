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
// File      : Vector.h
// Description :
//   Definition of the template TVector. The interface exposed is similar
//   to the one of TImplArray. The stored element can have an inheritance
//   hierarchy different from the EnhancedObject one. The implementation is
//   duplicated for each TVector instantiation.
//

#ifndef COL_VectorH
#define COL_VectorH

#include "Pointer/ImplList.h"

namespace COL {}

#include <string.h>

namespace COL {

namespace DVector {

class ReallocTraits {
  public:
   static int queryReallocation(int baseReallocation)
      {  return (baseReallocation <= 2) ? 4 : (baseReallocation*3/2); }
};

template <typename TypeElement>
class TElementTraits {
  public:
   static const bool FInitialCleared = true;
   static void clear(TypeElement& element) {}
   static void clearAll(TypeElement* array, int count)
      {  while (--count >= 0)
            clear(array[count]);
      }
   static void copy(TypeElement& element, const TypeElement& source) { element = source; }
   static void copyAll(TypeElement* array, const TypeElement* source, int count)
      {  for (int index = 0; index < count; ++index)
            copy(array[index], source[index]);
      }
};

template <typename TypeElement>
class TSystemElementTraits {
  public:
   static const bool FInitialCleared = true;
   static void clear(TypeElement& element) {}
   static void clearAll(TypeElement* array, int count)
      {  if (count > 0)
            memset(array, 0, count*sizeof(TypeElement));
      }
   static void copy(TypeElement& element, const TypeElement& source)
      {  element = source; }
   static void copyAll(TypeElement* array, const TypeElement* source, int count)
      {  if (count > 0)
            memcpy(array, source, count*sizeof(TypeElement));
      }
};

} // end of namespace DVector

template <typename TypeElement, class TypeElementTraits = DVector::TElementTraits<TypeElement>,
   class TypeReallocTraits=DVector::ReallocTraits>
class TVector {
  private:
   typedef TVector<TypeElement, TypeElementTraits, TypeReallocTraits> thisType;

   TypeElement* ateArray;
   int uCount;
   int uAllocated;

  protected:
   void realloc(int newAllocated);
   void _realloc(int newSize) { realloc(TypeReallocTraits::queryReallocation(newSize)); }

   // Query methods
   const int& queryAllocated() const { return uAllocated; }
   TypeElement* array() const { return ateArray; }

   // Block operations
   void addAtEnd(const thisType& source, int firstSource, int lastSource);
   void moveToAtEnd(thisType& destination, int first, int last);

  public:
   TVector() : ateArray(nullptr), uCount(0), uAllocated(0) {}
   TVector(thisType&& source) : ateArray(source.ateArray), uCount(source.uCount), uAllocated(source.uAllocated)
      {  source.ateArray = nullptr; source.uCount = 0; source.uAllocated = 0; }
   TVector(const thisType& source);
   ~TVector() { if (ateArray) delete [] ateArray; }

   thisType& assign(const thisType& source);
   thisType& operator=(thisType&& source)
      {  if (this != &source) {
            if (ateArray) delete [] ateArray;
            ateArray = source.ateArray;
            uCount = source.uCount;
            uAllocated = source.uAllocated;
            source.ateArray = nullptr;
            source.uCount = 0;
            source.uAllocated = 0;
         }
         return *this;
      }
         
   thisType& operator=(const thisType& source) {  return assign(source); }
   void moveAllTo(thisType& destination, int firstIndex=0, int lastIndex=-1, int destinationIndex=-1);
   void addAll(const thisType& source, int firstSourceIndex=0, int lastSourceIndex=-1,
      int destinationIndex=-1);

   void insertAt(int index, const TypeElement& element);
   void insertAtEnd(const TypeElement& element)
      {  bookPlace();
         TypeElementTraits::copy(ateArray[uCount++], element);
      }
   void removeAt(int index);
   void removeAtEnd()
      {  AssumeCondition(uCount > 0)
         TypeElementTraits::clear(ateArray[--uCount]);
      }
   void removeAll();
   void removeAllBetween(int firstIndex, int lastIndex);
   void swap(thisType& source);

   void bookPlace(int places=1) { if (uCount+places > uAllocated) _realloc(uCount+places); }
   void compress();
   int queryPlaces() const { return uAllocated-uCount; }

   TypeElement& referenceAt(int index) const
      {  AssumeCondition((index >= 0) && (index < uCount)) return ateArray[index];}
   const TypeElement& elementAt(int index) const { return referenceAt(index); }
   const TypeElement& operator[](int index) const { return referenceAt(index); }
   const TypeElement& first() const { return referenceAt(0); }
   const TypeElement& last() const { return referenceAt(uCount-1); }
   TypeElement& sfirst() { return referenceAt(0); }
   TypeElement& slast() { return referenceAt(uCount-1); }

   const int& count() const { return uCount; }
   template <class Execute> bool foreachDo(Execute& function, int startIndex=0, int endIndex=-1) const
      {  if (endIndex < 0)
            endIndex = uCount-1;
         bool result = true;
         for (int index=startIndex; result && (index <= endIndex); index++)
            result = function(ateArray[index]);
         return result;
      }
   template <class Execute> bool foreachReverseDo(Execute& function, int startIndex=-1, int endIndex=0) const
      {  if (startIndex < 0)
            startIndex = uCount-1;
         if (endIndex < 0)
            endIndex = 0;
         bool result = true;
         for (int index=startIndex; result && (index >= endIndex); index--)
            result = function(ateArray[index]);
         return result;
      }
   bool isValid() const { return (uCount >= 0) && (uCount <= uAllocated); }
};

template <typename TypeElement, class TypeElementTraits, class TypeReallocTraits>
inline void
TVector<TypeElement, TypeElementTraits, TypeReallocTraits>::swap(thisType& source) {
   int tempCount = uCount;
   int tempAllocated = uAllocated;
   TypeElement* tempArray = ateArray;
   uCount = source.uCount;
   uAllocated = source.uAllocated;
   ateArray = source.ateArray;
   source.uCount = tempCount;
   source.uAllocated = tempAllocated;
   source.ateArray = tempArray;
}

} // end of namespace COL

#endif // COL_VectorH

