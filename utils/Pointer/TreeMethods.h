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
// Unit      : Decision trees for method pointers
// File      : TreeMethods.h
// Description :
//   Definition of a small unit to desambiguate some virtual calls
//   through methods' trees. Such trees are defined with vectors and decision
//   nodes. Their construction is recursive to keep a static typing.
//

#ifndef PNT_TreeMethodsH
#define PNT_TreeMethodsH

#include "StandardClasses/StandardClasses.hpp"

namespace PNT {

template <typename PointerTypeMethod>
class DecisionNode : protected ExtendedParameters {
  protected:
   DefineExtendedParameters(2, ExtendedParameters)

   enum NodeType { NTMethodPointer=0, NTReferArray=1, NTControlArray=3 };
   enum ControlArray { CARefer=1, CAControl=3 };

  private:
   union { PointerTypeMethod ptmMethod; EnhancedObject* peoArray; };

  protected:
   bool isCleared() const { return (queryOwnField() == NTMethodPointer) && !ptmMethod; }

   DecisionNode() : ptmMethod(nullptr) {}
   DecisionNode(PointerTypeMethod method) : ptmMethod(method) {}
   DecisionNode(const DecisionNode<PointerTypeMethod>& source) : ptmMethod(nullptr)
      {  mergeOwnField(source.queryOwnField());
         if (queryOwnField() == NTMethodPointer)
            ptmMethod = source.ptmMethod;
         else
            peoArray = ((source.peoArray) && (queryOwnField() == NTControlArray))
               ? source.peoArray->createCopy() : source.peoArray;
      }

   DecisionNode<PointerTypeMethod>& setMethod(PointerTypeMethod method)
      {  AssumeCondition(isCleared()) ptmMethod = method; return *this; }
   DecisionNode<PointerTypeMethod>& resetMethod(PointerTypeMethod method)
      {  ptmMethod = method; return *this; }
   DecisionNode<PointerTypeMethod>& setArray(EnhancedObject* newArray, ControlArray control = CAControl)
      {  AssumeCondition(isCleared())
         peoArray = newArray;
         mergeOwnField((NodeType) control);
         return *this;
      }
   DecisionNode<PointerTypeMethod>& resetArray(EnhancedObject* newArray, ControlArray control = CAControl)
      {  if (!isCleared())
            free();
         peoArray = newArray;
         mergeOwnField((NodeType) control);
         return *this;
      }

   const PointerTypeMethod& getMethod() const { AssumeCondition(isMethod() && ptmMethod) return ptmMethod; }
   EnhancedObject& getArray() const { AssumeCondition(hasArray()) return *peoArray; }
   bool hasArray() const { return isArray() && peoArray; }
   bool isArray() const { return queryOwnField() & NTReferArray; }
   bool isControlledArray() const { return queryOwnField() == NTControlArray; }
   void free()
      {  if (isControlledArray() && peoArray)
            delete peoArray; 
         clearOwnField(); ptmMethod = nullptr;
      }

  public:
   DecisionNode<PointerTypeMethod>& operator=(const DecisionNode<PointerTypeMethod>& source);
   ~DecisionNode() { if (isControlledArray() && peoArray) delete peoArray; }
   bool isMethod() const { return (queryOwnField() == NTMethodPointer); }
   bool hasMethod() const { return isMethod() && ptmMethod; }
   void clear() { clearOwnField(); ptmMethod = nullptr; }
};

template <typename PointerTypeMethod>
inline DecisionNode<PointerTypeMethod>&
DecisionNode<PointerTypeMethod>::operator=(const DecisionNode<PointerTypeMethod>& dnSource) {
   if (this != &dnSource) {
      if (isControlledArray() && peoArray) delete peoArray;
      clear();
      mergeOwnField(dnSource.queryOwnField());
      if (isMethod())
         ptmMethod = dnSource.ptmMethod;
      else
         peoArray = (isControlledArray() && dnSource.peoArray) ? dnSource.peoArray->createSCopy()
                                                               : dnSource.peoArray;
   };
   return *this;
}

template <class TypeDecisionNode, int TSize>
class ConstMethodTable : public EnhancedObject {
  private:
   typedef ConstMethodTable<TypeDecisionNode, TSize> thisType;
   TypeDecisionNode atdnArray[TSize];

  protected:
   const TypeDecisionNode& elementAt(int index) const
      {  AssumeCondition((index >= 0) && (index < TSize))
         return atdnArray[index];
      }
   TypeDecisionNode& elementAt(int index)
      {  AssumeCondition((index >= 0) && (index < TSize))
         return atdnArray[index];
      }

   ConstMethodTable() { clear(); }
   ConstMethodTable(const ConstMethodTable<TypeDecisionNode, TSize>& source) = default;
   void clear()
      {  for (int index = 0; index < TSize; index++)
            atdnArray[index].clear();
      }

  public:
   DTemplate2DefineAssign(ConstMethodTable, TypeDecisionNode, TSize)
   const TypeDecisionNode& operator[](int index) const
      {  AssumeCondition((index >= 0) && (index < TSize))
         return atdnArray[index];
      }
   TypeDecisionNode& operator[](int index)
      {  AssumeCondition((index >= 0) && (index < TSize))
         return atdnArray[index];
      }
   static const int Size = TSize;
};

template <class TypeDecisionNode>
class TBaseStaticMethodTable : public EnhancedObject {
  protected:
   TypeDecisionNode* atdnArray;
   int uCount;
   int uAllocated;

  protected:
   const TypeDecisionNode& elementAt(int index) const
      {  AssumeCondition((index >= 0) && (index < uCount))
         return atdnArray[index];
      }
   TBaseStaticMethodTable() : atdnArray(nullptr), uCount(0), uAllocated(0) {}
   TBaseStaticMethodTable(const TBaseStaticMethodTable<TypeDecisionNode>& source)
      :  atdnArray((source.uCount > 0) ? new TypeDecisionNode[source.uCount] : nullptr),
         uCount(source.uCount), uAllocated(source.uCount)
      {  for (int index = 0; index < uCount; index++)
            atdnArray[index] = source.atdnArray[index];
      }

  public:
   virtual ~TBaseStaticMethodTable() { if (atdnArray) delete [] atdnArray; }
   TBaseStaticMethodTable<TypeDecisionNode>& operator=(const TBaseStaticMethodTable<TypeDecisionNode>& source)
      {  if (this != &source) {
            if (atdnArray) delete [] atdnArray;
            atdnArray = (source.uCount > 0) ? new TypeDecisionNode[source.uCount] : nullptr;
            uCount = source.uCount;
            uAllocated = source.uAllocated;
         }
         return *this;
      }
   DTemplateDefineAssign(TBaseStaticMethodTable, TypeDecisionNode)
   const TypeDecisionNode& operator[](int index) const
      {  AssumeCondition((index >= 0) && (index < uCount))
         return atdnArray[index];
      }
};


template <class TypeDecisionNode>
class StaticMethodTable : public TBaseStaticMethodTable<TypeDecisionNode> {
  private:
   typedef StaticMethodTable<TypeDecisionNode> thisType;
   typedef TBaseStaticMethodTable<TypeDecisionNode> inherited;
   typedef TypeDecisionNode* PDecisionNode;

  public:
   void setSize(int size)
      {  if (size > sallocated()) {
            realloc(size);
            scount() = size;
         };
      }

  protected:
   int& scount() { return inherited::uCount; }
   const int& scount() const { return inherited::uCount; }
   int& sallocated() { return inherited::uAllocated; }
   const int& sallocated() const { return inherited::uAllocated; }
   PDecisionNode& sarray() { return inherited::atdnArray; }
   const PDecisionNode& sarray() const { return inherited::atdnArray; }

   const TypeDecisionNode& elementAt(int index) const
      {  AssumeCondition((index >= 0) && (index < scount()))
         return sarray()[index];
      }
   TypeDecisionNode& elementAt(int index)
      {  AssumeCondition(index >= 0)
         assumeAlloc(index);
         if (index >= scount())
            scount() = index+1;
         return sarray()[index];
      }

#include "StandardClasses/UndefineNew.h"

   void realloc(int newAllocated)
      {  AssumeCondition(scount() <= newAllocated)
         if (newAllocated == 0) return;
         TypeDecisionNode* newArray = new TypeDecisionNode[newAllocated];
         AssumeAllocation(newArray)
         for (int index = 0; index < scount(); index++)
            newArray[index] = sarray()[index];
         if (sarray())
            delete [] sarray();
         sarray() = newArray;
         sallocated() = newAllocated;
      }

#include "StandardClasses/DefineNew.h"

   virtual void _realloc(int newAllocated) { realloc(newAllocated+5); }
   void assumeAlloc(int newAllocated)
      {  if (newAllocated > sallocated()) _realloc(newAllocated); }
   void bookPlace(int place)
      {  if (scount()+place > sallocated()) _realloc(scount()+place); }
   void clear() { sarray() = nullptr; scount() = sallocated() = 0; }
   void closeConstruction() { realloc(scount()); }

   StaticMethodTable() : inherited() {}
   StaticMethodTable(const StaticMethodTable<TypeDecisionNode>& source) :  inherited(source) {}
      
  public:
   StaticMethodTable<TypeDecisionNode>& operator=(const StaticMethodTable<TypeDecisionNode>& source)
      {  if (this != &source) {
            scount() = 0;
            assumeAlloc(source.scount());
            for (int index = 0; index < source.scount(); index++)
               sarray()[index] = source.sarray()[index];
            scount() = source.scount();
         };
         return *this;
      }
   DTemplateDefineAssign(StaticMethodTable, TypeDecisionNode)
   const TypeDecisionNode& operator[](int index) const
      {  AssumeCondition((index >= 0) && (index < scount()))
         return sarray()[index];
      }
   TypeDecisionNode& operator[](int index)
      {  AssumeCondition(index >= 0)
         assumeAlloc(index+1);
         if (index >= scount()) {
            scount() = index+1;
            sarray()[index].clear();
         };
         return sarray()[index];
      }
};

template <class TypeDecisionNode>
class TMovableStaticMethodTable : public TBaseStaticMethodTable<TypeDecisionNode> {
  private:
   typedef TBaseStaticMethodTable<TypeDecisionNode> inherited;
   typedef TMovableStaticMethodTable<TypeDecisionNode> thisType;
   typedef TypeDecisionNode* PDecisionNode;

  protected:
   int& scount() { return inherited::uCount; }
   const int& scount() const { return inherited::uCount; }
   int& sallocated() { return inherited::uAllocated; }
   const int& sallocated() const { return inherited::uAllocated; }
   PDecisionNode& sarray() { return inherited::atdnArray; }
   const PDecisionNode& sarray() const { return inherited::atdnArray; }

   void realloc(int newAllocated)
      {  AssumeCondition(scount() <= newAllocated)
         if (newAllocated == 0) return;
         TypeDecisionNode* newArray = new TypeDecisionNode[newAllocated];
         AssumeAllocation(newArray)
         if (scount() > 0)
            memcpy(newArray, sarray(), sizeof(TypeDecisionNode)*scount());
         if (sarray())
            delete [] sarray();
         sarray() = newArray;
         sallocated() = newAllocated;
      }
   void insert(int index, int numberOfElements)
      {  if ((index < scount()) && (numberOfElements > 0)) {
            assumeAlloc(scount() + numberOfElements);
            memmove(sarray() + index + numberOfElements, sarray() + index, sizeof(TypeDecisionNode)*(scount()-index));
            memset(sarray()+index, 0, numberOfElements*sizeof(TypeDecisionNode));
         };
      }
   void remove(int index, int numberOfElements)
      {  AssumeCondition(index + numberOfElements <= scount())
         if ((numberOfElements > 0) && (index + numberOfElements < scount()))
            memmove(sarray() + index, sarray() + index + numberOfElements, sizeof(TypeDecisionNode)*(scount()-numberOfElements-index));
         memset(sarray()+scount()-numberOfElements, 0, numberOfElements*sizeof(TypeDecisionNode));
      }
   virtual void _realloc(int newAllocated) { realloc(newAllocated+5); }
   void bookPlace(int place)
      {  if (scount()+place > sallocated()) _realloc(scount()+place); }
   void assumeAlloc(int newAllocated)
      {  if (newAllocated > sallocated()) _realloc(newAllocated); }

   const TypeDecisionNode& elementAt(int index) const
      {  AssumeCondition((index >= 0) && (index < scount()))
         return sarray()[index];
      }
   TypeDecisionNode& elementAt(int index)
      {  AssumeCondition(index >= 0)
         assumeAlloc(index);
         if (index >= scount())
            scount() = index+1;
         return sarray()[index];
      }
   void closeConstruction() { realloc(scount()); }
   void clear() { sarray()=nullptr; scount() = sallocated() = 0; }

   TMovableStaticMethodTable() : inherited() {}
   TMovableStaticMethodTable(const TMovableStaticMethodTable<TypeDecisionNode>& source)
      :  inherited(source) {}

  public:
   TMovableStaticMethodTable<TypeDecisionNode>& operator=(const TMovableStaticMethodTable<TypeDecisionNode>& source)
      {  if (this != &source) {
            scount() = 0;
            assumeAlloc(source.scount());
            for (int index = 0; index < source.scount(); index++)
               sarray()[index] = source.sarray()[index];
            scount() = source.scount();
         };
         return *this;
      }
   DTemplateDefineAssign(TMovableStaticMethodTable, TypeDecisionNode)

   const TypeDecisionNode& operator[](int index) const
      {  AssumeCondition((index >= 0) && (index < scount()))
         return sarray()[index];
      }
   TypeDecisionNode& operator[](int index)
      {  AssumeCondition(index >= 0)
         assumeAlloc(index);
         if (index >= scount())
            scount() = index+1;
         return sarray()[index];
      }
};

} // end of namespace PNT

#endif // PNT_TreeMethodsH

