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
// File      : ImplArray.cpp
// Description :
//   Implementation of a class of arrays. It provides an implementation
//   for the COL::TArray collection.
//

#include "Pointer/ImplArray.h"
#include <memory.h>

namespace COL {

void
ImplArray::realloc(int newAllocated) {
   AssumeCondition(uCount <= newAllocated)
   EnhancedObject** newArray = new PEnhancedObject[newAllocated];
   AssumeAllocation(newArray)
   memset(newArray, 0, newAllocated*sizeof(PEnhancedObject));
   if (uCount)
      memcpy(newArray, apeoArray, uCount*sizeof(PEnhancedObject));
   uAllocated = newAllocated;
   if (apeoArray) delete [] apeoArray;
   apeoArray = newArray;
}

ImplArray::ImplArray(const ImplArray& source, bool doesDuplicate,
      const VirtualCast* pvcRetrieveRegistrationFromCopy)
   :  EnhancedObject(source), apeoArray(nullptr), uCount(0), uAllocated(0) {
   if (source.uCount) {
      realloc(source.uCount);
      if (doesDuplicate) {
         const VirtualCastWithElement* retrieveRegistrationFromCopy = (const VirtualCastWithElement*) pvcRetrieveRegistrationFromCopy;
         for (uCount = 0; uCount < source.uCount; uCount++)
            apeoArray[uCount] = pvcRetrieveRegistrationFromCopy
               ? retrieveRegistrationFromCopy->castFrom(source.apeoArray[uCount]->createCopy(), source.apeoArray[uCount])
               : source.apeoArray[uCount]->createCopy();
      }
      else {
         memcpy(apeoArray, source.apeoArray, source.uCount*sizeof(PEnhancedObject));
         uCount = source.uCount;
      };
   };
}

void
ImplArray::assign(const ImplArray& source, bool doesDuplicate,
      const VirtualCast* pvcRetrieveRegistrationFromCopy) {
   if (&source != this) {
      removeAll(doesDuplicate);
      if (source.uCount > uAllocated)
         realloc(source.uCount);
      if (source.uCount) {
         if (doesDuplicate) {
            const VirtualCastWithElement* retrieveRegistrationFromCopy = (const VirtualCastWithElement*) pvcRetrieveRegistrationFromCopy;
            for (uCount = 0; uCount < source.uCount; uCount++)
               apeoArray[uCount] = pvcRetrieveRegistrationFromCopy
                  ? retrieveRegistrationFromCopy->castFrom(source.apeoArray[uCount]->createCopy(), source.apeoArray[uCount])
                  : source.apeoArray[uCount]->createCopy();
         }
         else {
            memcpy(apeoArray, source.apeoArray, source.uCount*sizeof(PEnhancedObject));
            uCount = source.uCount;
         };
      };
   };
}

void
ImplArray::compress() {
   if (uCount < uAllocated) {
      if (uCount == 0) {
         if (apeoArray)
            delete [] apeoArray;
         uAllocated = 0;
      }
      else
         realloc(uCount);
   };
}

void
ImplArray::addAll(const ImplArray& source, int firstSourceIndex, int lastSourceIndex,
      int destinationIndex, bool doesDuplicate, const VirtualCast* pvcRetrieveRegistrationFromCopy) {
   if (firstSourceIndex < 0)
      firstSourceIndex = 0;
   else if (firstSourceIndex >= source.uCount)
      return;
   if ((lastSourceIndex < 0) || (lastSourceIndex >= source.uCount))
      lastSourceIndex = source.uCount-1;
   if (lastSourceIndex < firstSourceIndex)
      return;

   if ((destinationIndex < 0) || (destinationIndex > uCount))
      destinationIndex = uCount;
   bookPlace(1 + (lastSourceIndex-firstSourceIndex));

   int sourceIndex=firstSourceIndex;
   try {
      if (&source != this) {
         if (destinationIndex < uCount)
            memmove(apeoArray+destinationIndex+1+(lastSourceIndex-firstSourceIndex),
                    apeoArray+destinationIndex, (uCount-destinationIndex)*sizeof(PEnhancedObject));
         if (!doesDuplicate) {
            memcpy(apeoArray+destinationIndex, source.apeoArray+firstSourceIndex,
                   (1+(lastSourceIndex-firstSourceIndex))*sizeof(PEnhancedObject));
         }
         else {
            const VirtualCastWithElement* retrieveRegistrationFromCopy = (const VirtualCastWithElement*) pvcRetrieveRegistrationFromCopy;
            for (; sourceIndex <= lastSourceIndex; sourceIndex++)
               apeoArray[destinationIndex+(sourceIndex-firstSourceIndex)] = pvcRetrieveRegistrationFromCopy
                  ? retrieveRegistrationFromCopy->castFrom(source.apeoArray[sourceIndex]->createCopy(), source.apeoArray[sourceIndex])
                  : source.apeoArray[sourceIndex]->createCopy();
         };
      }
      else {
         if (destinationIndex < uCount) // book some place after destinationIndex
            memmove(apeoArray+destinationIndex+1+lastSourceIndex-firstSourceIndex,
               apeoArray+destinationIndex, (uCount-destinationIndex)*sizeof(PEnhancedObject));
         if (destinationIndex <= firstSourceIndex) {
            // sources are in [1+lastSourceIndex, 1+2*lastSourceIndex-firstSourceIndex]
            if (!doesDuplicate) {
               if (destinationIndex == firstSourceIndex)
                  return;
               memmove(apeoArray+destinationIndex, apeoArray+lastSourceIndex+1,
                  (1+lastSourceIndex-firstSourceIndex)*sizeof(PEnhancedObject));
            }
            else {
               const VirtualCastWithElement* retrieveRegistrationFromCopy = (const VirtualCastWithElement*) pvcRetrieveRegistrationFromCopy;
               for (; sourceIndex <= lastSourceIndex; sourceIndex++) {
                  EnhancedObject* sourceElement = apeoArray[lastSourceIndex+1+(sourceIndex-firstSourceIndex)]; 
                  apeoArray[destinationIndex+(sourceIndex-firstSourceIndex)] = pvcRetrieveRegistrationFromCopy
                     ? retrieveRegistrationFromCopy->castFrom(sourceElement->createCopy(), sourceElement)
                     : sourceElement->createCopy();
               };
            };
         }
         else if (destinationIndex > lastSourceIndex) { // no shift
            if (!doesDuplicate) {
               memmove(apeoArray+destinationIndex, apeoArray+firstSourceIndex,
                  (1+lastSourceIndex-firstSourceIndex)*sizeof(PEnhancedObject));
            }
            else {
               const VirtualCastWithElement* retrieveRegistrationFromCopy = (const VirtualCastWithElement*) pvcRetrieveRegistrationFromCopy;
               for (; sourceIndex <= lastSourceIndex; sourceIndex++) {
                  EnhancedObject* sourceElement = apeoArray[firstSourceIndex+(sourceIndex-firstSourceIndex)]; 
                  apeoArray[destinationIndex+(sourceIndex-firstSourceIndex)] = pvcRetrieveRegistrationFromCopy
                     ? retrieveRegistrationFromCopy->castFrom(sourceElement->createCopy(), sourceElement)
                     : sourceElement->createCopy();
               };
            };
         }
         else { // firstSourceIndex < destinationIndex <= lastSourceIndex
            if (!doesDuplicate) { // sources are in [firstSourceIndex, destinationIndex-1]
                                  // and [destinationIndex + (lastSourceIndex-firstSourceIndex+1), 2*lastSourceIndex-firstSourceIndex+1]
               memmove(apeoArray+destinationIndex, apeoArray+firstSourceIndex,
                  (destinationIndex-firstSourceIndex)*sizeof(PEnhancedObject));
               memmove(apeoArray+2*destinationIndex-firstSourceIndex,
                       apeoArray+destinationIndex+(lastSourceIndex-firstSourceIndex+1),
                       (lastSourceIndex+1-destinationIndex)*sizeof(PEnhancedObject));
            }
            else {
               const VirtualCastWithElement* retrieveRegistrationFromCopy = (const VirtualCastWithElement*) pvcRetrieveRegistrationFromCopy;
               for (; sourceIndex <= destinationIndex-1; sourceIndex++) {
                  EnhancedObject* sourceElement = apeoArray[firstSourceIndex+(sourceIndex-firstSourceIndex)];
                  apeoArray[destinationIndex+(sourceIndex-firstSourceIndex)] = pvcRetrieveRegistrationFromCopy
                     ? retrieveRegistrationFromCopy->castFrom(sourceElement->createCopy(), sourceElement)
                     : sourceElement->createCopy();
               };
               for(; sourceIndex <= lastSourceIndex; sourceIndex++) {
                  EnhancedObject* sourceElement = apeoArray[sourceIndex + lastSourceIndex-firstSourceIndex+1];
                  apeoArray[destinationIndex+(sourceIndex-firstSourceIndex)] = pvcRetrieveRegistrationFromCopy
                     ? retrieveRegistrationFromCopy->castFrom(sourceElement->createCopy(), sourceElement)
                     : sourceElement->createCopy();
               };
            };
         };
      };
   }
   catch (...) { // shift of destinations (errors that occur in createCopy)
      memmove(apeoArray+destinationIndex+(sourceIndex-firstSourceIndex),
              apeoArray+destinationIndex+(lastSourceIndex-firstSourceIndex+1),
              uCount-destinationIndex*sizeof(PEnhancedObject));
      memset(apeoArray+uCount+1+lastSourceIndex-sourceIndex, 0,
         (lastSourceIndex-sourceIndex+1)*sizeof(PEnhancedObject));
      uCount += (sourceIndex-firstSourceIndex);
      throw;
   }
   uCount += 1 + (lastSourceIndex-firstSourceIndex);
}

void
ImplArray::moveAllTo(ImplArray& destination, int firstIndex, int lastIndex, int destinationIndex) {
   if (firstIndex < 0)
      firstIndex = 0;
   else if (firstIndex >= uCount)
      return;
   if ((lastIndex < 0) || (lastIndex >= uCount))
      lastIndex = uCount-1;
   if (lastIndex < firstIndex)
      return;

   if ((destinationIndex < 0) || (destinationIndex > destination.uCount))
      destinationIndex = destination.uCount;

   if (&destination != this) {
      destination.bookPlace(1 + (lastIndex-firstIndex));
      if (destinationIndex < destination.uCount)
         memmove(destination.apeoArray+destinationIndex+1+(lastIndex-firstIndex),
                 destination.apeoArray+destinationIndex,
                 (destination.uCount-destinationIndex)*sizeof(PEnhancedObject));
      memcpy(destination.apeoArray+destinationIndex, apeoArray+firstIndex,
             (1+(lastIndex-firstIndex))*sizeof(PEnhancedObject));
      if (lastIndex < uCount-1) {
         memmove(apeoArray+firstIndex, apeoArray+lastIndex+1,
            (uCount-lastIndex-1)*sizeof(PEnhancedObject));
         memset(apeoArray+uCount-1-lastIndex+firstIndex, 0, (lastIndex+1-firstIndex)*sizeof(PEnhancedObject));
      };
      uCount -= 1+(lastIndex-firstIndex);
      destination.uCount += 1+(lastIndex-firstIndex);
   }
   else {
      if ((firstIndex <= destinationIndex) && (destinationIndex-1 <= lastIndex))
         return;
      if (destinationIndex < firstIndex) {
         PEnhancedObject* tempArray = new PEnhancedObject[firstIndex-destinationIndex];
         AssumeAllocation(tempArray)
         memcpy(tempArray, apeoArray+destinationIndex,
            (firstIndex-destinationIndex)*sizeof(PEnhancedObject));
         memmove(apeoArray+destinationIndex, apeoArray+firstIndex,
            (lastIndex-firstIndex+1)*sizeof(PEnhancedObject));
         memcpy(apeoArray+destinationIndex+(lastIndex-firstIndex+1), tempArray,
            (firstIndex-destinationIndex)*sizeof(PEnhancedObject));
         if (tempArray) delete [] tempArray;
      }
      else { // destinationIndex > lastIndex+1
         PEnhancedObject* tempArray = new PEnhancedObject[destinationIndex-1-lastIndex];
         AssumeAllocation(tempArray)
         memcpy(tempArray, apeoArray+lastIndex+1,
            (destinationIndex-lastIndex-1)*sizeof(PEnhancedObject));
         memmove(apeoArray+destinationIndex+firstIndex-lastIndex-1, apeoArray+firstIndex,
            (lastIndex-firstIndex+1)*sizeof(PEnhancedObject));
         memcpy(apeoArray+firstIndex, tempArray,
            (destinationIndex-lastIndex-1)*sizeof(PEnhancedObject));
         if (tempArray) delete [] tempArray;
      };
   };
}

void
ImplArray::insertAt(int index, EnhancedObject* anObject) {
   AssumeAllocation(anObject);
   AssumeCondition((index >= 0) && (index <= uCount));
   bookPlace();
   if (uCount > index)
      memmove(apeoArray+index+1, apeoArray+index, (uCount-index)*sizeof(PEnhancedObject));
   apeoArray[index] = anObject;
   ++uCount;
}

void
ImplArray::removeAt(int index) {
   AssumeCondition((index >= 0) && (index < uCount))
   if (uCount-1 > index)
      memmove(apeoArray+index, apeoArray+index+1, (uCount-index-1)*sizeof(PEnhancedObject));
   apeoArray[--uCount] = nullptr;
}

void
ImplArray::removeAllBetween(int firstIndex, int lastIndex, bool doesFree) {
   AssumeCondition((firstIndex >= 0) && (firstIndex <= uCount))
   if ((lastIndex < 0) || (lastIndex >= uCount-1)) {
      if (firstIndex < uCount) {
         if (doesFree) {
            for (int index = firstIndex; index < uCount; ++index) {
               if (apeoArray[index] != nullptr) {
                  delete apeoArray[index];
                  apeoArray[index] = nullptr;
               };
            };
         }
         else
            memset(apeoArray+firstIndex, 0, (uCount-firstIndex)*sizeof(PEnhancedObject));
         uCount = firstIndex;
      };
   }
   else {
      if (firstIndex <= lastIndex) {
         if (doesFree) {
            for (int index = firstIndex; index <= lastIndex; ++index) {
               if (apeoArray[index] != nullptr) {
                  delete apeoArray[index];
                  apeoArray[index] = nullptr;
               };
            };
         };
         memmove(apeoArray+firstIndex, apeoArray+lastIndex+1, (uCount-lastIndex-1)*sizeof(PEnhancedObject));
         int oldCount = uCount;
         uCount -= (lastIndex-firstIndex+1);
         memset(apeoArray+uCount, 0, (oldCount-uCount)*sizeof(PEnhancedObject));
      };
   };
}

void
ImplArray::removeAll(bool doesFree) {
   if (doesFree && (uCount > 0)) {
      while (uCount--) {
         if (apeoArray[uCount]) delete apeoArray[uCount];
         apeoArray[uCount] = nullptr;
      };
   }
   else
      memset(apeoArray, 0, uCount*sizeof(PEnhancedObject));
   uCount = 0;
}

void
ImplArray::swap(ImplArray& source) {
   int tempCount = uCount;
   int tempAllocated = uAllocated;
   EnhancedObject** tempArray = apeoArray;
   uCount = source.uCount;
   uAllocated = source.uAllocated;
   apeoArray = source.apeoArray;
   source.uCount = tempCount;
   source.uAllocated = tempAllocated;
   source.apeoArray = tempArray;
}

// block operations
void
ImplArray::addAtEnd(const ImplArray& source, int firstSource, int lastSource, bool doesDuplicate,
      const VirtualCast* pvcRetrieveRegistrationFromCopy) {
   if (lastSource < firstSource)
      return;
   AssumeCondition((firstSource >= 0) && (lastSource < source.uCount))
   bookPlace(lastSource-firstSource+1);
   if (doesDuplicate) {
      const VirtualCastWithElement* retrieveRegistrationFromCopy = (const VirtualCastWithElement*) pvcRetrieveRegistrationFromCopy;
      while (firstSource <= lastSource) {
         EnhancedObject* sourceElement = source.apeoArray[firstSource++];
         insertAtEnd(pvcRetrieveRegistrationFromCopy
            ? retrieveRegistrationFromCopy->castFrom(sourceElement->createCopy(), sourceElement)
            : sourceElement->createCopy());
      };
   }
   else {
      memmove(apeoArray+uCount, source.apeoArray+firstSource,
         (lastSource-firstSource+1)*sizeof(PEnhancedObject));
      uCount += (lastSource-firstSource+1);
   };
}

void
ImplArray::moveToAtEnd(ImplArray& destination, int first, int last) {
   if (last < first)
      return;
   AssumeCondition((this != &destination) && (first >= 0) && (last < uCount))
   destination.bookPlace(last-first+1);
   memcpy(destination.apeoArray+destination.uCount, apeoArray+first, (last-first+1)*sizeof(PEnhancedObject));
   int firstRemove = first + (uCount - last -1);
   if (uCount > last+1)
      memmove(apeoArray+first, apeoArray+last+1, (uCount-last-1)*sizeof(PEnhancedObject));
   memset(apeoArray+firstRemove, 0, (last-first+1)*sizeof(PEnhancedObject));
   uCount -= (last-first+1);
   destination.uCount += (last-first+1);
}

} // end of namespace COL

