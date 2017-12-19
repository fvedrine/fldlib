/**************************************************************************/
/*                                                                        */
/*  This file is part of FLDLib                                           */
/*                                                                        */
/*  Copyright (C) 2011-2017                                               */
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
// Library   : NumericalDomains
// Unit      : Affine relationships
// File      : FloatAffineBaseCommon.h
// Description :
//   Definition of a class of affine relations.
//

#ifndef NumericalDomains_FloatAffineBaseCommonH
#define NumericalDomains_FloatAffineBaseCommonH

#include "StandardClasses/Persistence.h"
#include "Pointer/SharedCollection.h"
#include "Collection/ConcreteCollection/List.h"
#include "Collection/ConcreteCollection/Array.h"
#include "Collection/ConcreteCollection/SortedArray.h"

namespace NumericalDomains { namespace DAffine {

class SymbolsManager;
class BaseConstraintsSet : public STG::IOObject {
  public:
   BaseConstraintsSet() = default;
   BaseConstraintsSet(const BaseConstraintsSet&) = default;
};

class SymbolRegistration : public COL::List::Node {
  public:
   SymbolRegistration() {}
   SymbolRegistration(const SymbolRegistration& source) = default;
};

class Symbol : public SymbolRegistration, public PNT::SharedCollection::Element, public STG::IOObject, protected ExtendedParameters {
  public:
   enum Type
      {  TUndefined,
         TCentral,      // input interval, universally quantified
         TNoise,        // symbol introduced by the approximation of non-linear operations, existentially quantified
         TMarkedNoise,  // artefact symbol for search or to solve inconsistencies between symbols, existentially quantified
         TClosed,       // symbol for summary application, universally quantified
         TDefined,      // symbol introduced by a constraint with an inline definition
         THighLevel,    // symbol introduced by an absorbable high level error
         TConstrained   // symbol introduced by a constraint
      };

  private:
   typedef SymbolRegistration inherited;
   int uOrder;

  protected:
   DefineExtendedParameters(3, ExtendedParameters)
   virtual ComparisonResult _compare(const EnhancedObject& asource) const override
      {  ComparisonResult result = inherited::_compare(asource);
         if (result == CREqual) {
            const Symbol& source = castFromCopyHandler(asource);
            result = fcompare(queryOwnField(), source.queryOwnField());
            if (result == CREqual)
               result = fcompare(uOrder, source.uOrder);
         };
         return result;
      }
   void setType(Type type) { AssumeCondition(!hasOwnField()) mergeOwnField(type); }

  public:
   Symbol() : uOrder(0) {}
   Symbol(const Symbol& source)
      :  inherited(source), PNT::SharedCollection::Element(source), STG::IOObject(source),
         ExtendedParameters(source), uOrder(0) {}
   Symbol& operator=(const Symbol& source)
      {  inherited::operator=(source);
         PNT::SharedCollection::Element::operator=((const PNT::SharedCollection::Element&) source);
         return *this;
      }
   DefineCopy(Symbol)
   DDefineAssign(Symbol)
   DCompare(Symbol)
   StaticInheritConversions(Symbol, inherited)

   bool hasOrder() const { return uOrder > 0; }
   void setOrder(int order) { uOrder = order; }
   int getOrder() const { return uOrder; }
   Type getType() const { return (Type) queryOwnField(); }
   typedef HandlerIntermediateCast<Symbol, SymbolRegistration, COL::List::Node> Registration;
   virtual const char* getOwnPrefix() const { return "x_"; }

   void unlock(PNT::DSharedCollection::GlobalElement& call)  { PNT::SharedCollection::Element::unlock(call); }
   void lock(PNT::DSharedCollection::GlobalElement& call)    { PNT::SharedCollection::Element::lock(call); }
};

class SymbolsManager;

class CentralSymbol : public Symbol {
  private:
   typedef Symbol inherited;

  public:
   CentralSymbol() { setType(TCentral); }
   CentralSymbol(const CentralSymbol& source) = default;
   virtual const char* getOwnPrefix() const { return "e_"; }
};

class ClosedSymbol : public Symbol {
  private:
   typedef Symbol inherited;
   int uLevel;

  protected:
   virtual ComparisonResult _compare(const EnhancedObject& asource) const override;

  public:
   ClosedSymbol(int level) : uLevel(level) { setType(TClosed); }
   ClosedSymbol(const ClosedSymbol& source) = default;
   ClosedSymbol& operator=(const ClosedSymbol& source)
      {  inherited::operator=(source);
         AssumeCondition(uLevel == source.uLevel)
         return *this;
      }
   DefineCopy(ClosedSymbol)
   DDefineAssign(ClosedSymbol)
   DCompare(ClosedSymbol)

   int getLevel() const { return uLevel; }
   virtual const char* getOwnPrefix() const override { return "l_"; }
};

class NoiseSymbol : public Symbol {
  private:
   typedef Symbol inherited;

  public:
   NoiseSymbol() { setType(TNoise); }
   NoiseSymbol(const NoiseSymbol& source) = default;
   virtual const char* getOwnPrefix() const { return "u_"; }
};

class HighLevelSymbol : public Symbol {
  private:
   typedef Symbol inherited;

  public:
   HighLevelSymbol() { setType(THighLevel); }
   HighLevelSymbol(const HighLevelSymbol& source) = default;
   virtual const char* getOwnPrefix() const { return "h_"; }
};

class MarkedNoiseSymbol : public Symbol {
  private:
   typedef Symbol inherited;

  public:
   MarkedNoiseSymbol(int order) { setType(TMarkedNoise); setOrder(order); }
   MarkedNoiseSymbol(const MarkedNoiseSymbol& source) = default;
   virtual const char* getOwnPrefix() const { return "um_"; }
};

class ConstrainedSymbol : public Symbol {
  private:
   typedef Symbol inherited;

  public:
   ConstrainedSymbol(int order) { setType(TConstrained); setOrder(order); }
   ConstrainedSymbol(const ConstrainedSymbol& source) = default;
   virtual const char* getOwnPrefix() const { return "c_"; }
};

class SymbolsManager : public EnhancedObject {
  private:
   typedef EnhancedObject inherited;
   COL::TCopyCollection<COL::TList<Symbol, Symbol::Registration> > lsCentralSymbols;
   COL::TCopyCollection<COL::TList<Symbol, Symbol::Registration> > lsNoiseSymbols;
   COL::TCopyCollection<COL::TList<Symbol, Symbol::Registration> > lsDefinedSymbols;
   COL::TCopyCollection<COL::TList<Symbol, Symbol::Registration> > lsHighLevelSymbols;
   int uHighLevelSymbolsCounter;

  public:
   SymbolsManager() : uHighLevelSymbolsCounter(0) {}
   SymbolsManager(const SymbolsManager& source) : uHighLevelSymbolsCounter(0) {}
   DefineCopy(SymbolsManager)

   void swap(SymbolsManager& source)
      {  lsCentralSymbols.swap(source.lsCentralSymbols);
         lsNoiseSymbols.swap(source.lsNoiseSymbols);
         lsDefinedSymbols.swap(source.lsDefinedSymbols);
         lsHighLevelSymbols.swap(source.lsHighLevelSymbols);
         int temp = uHighLevelSymbolsCounter;
         uHighLevelSymbolsCounter = source.uHighLevelSymbolsCounter;
         source.uHighLevelSymbolsCounter = temp;
      }
   void clear()
      {  lsCentralSymbols.freeAll();
         lsNoiseSymbols.freeAll();
         lsDefinedSymbols.freeAll();
         lsHighLevelSymbols.freeAll();
         uHighLevelSymbolsCounter = 0;
      }
   bool contains(const Symbol& symbol) const
      {  switch (symbol.getType()) {
            case Symbol::TCentral: return symbol.getOrder() <= lsCentralSymbols.count();
            case Symbol::TNoise: return symbol.getOrder() <= lsNoiseSymbols.count();
            case Symbol::TMarkedNoise: return symbol.getOrder() <= lsNoiseSymbols.count();
            case Symbol::TClosed: return false;
            case Symbol::TDefined: return symbol.getOrder() <= lsDefinedSymbols.count();
            case Symbol::THighLevel: return symbol.getOrder() <= uHighLevelSymbolsCounter;
            case Symbol::TConstrained: return false;
            default:
               return false;
         };
      }

   class MergeTable {
     private:
      int uCentralSymbolsLimit;
      int uNoiseSymbolsLimit;
      int uDefinedSymbolsLimit;
      int uHighLevelSymbolsLimit;

      int uCentralSymbolsAfterMerge;
      int uNoiseSymbolsAfterMerge;
      int uDefinedSymbolsAfterMerge;
      int uHighLevelSymbolsAfterMerge;
     public:
      MergeTable()
         :  uCentralSymbolsLimit(0), uNoiseSymbolsLimit(0),
            uDefinedSymbolsLimit(0), uHighLevelSymbolsLimit(0),
            uCentralSymbolsAfterMerge(0), uNoiseSymbolsAfterMerge(0),
            uDefinedSymbolsAfterMerge(0), uHighLevelSymbolsAfterMerge(0) {}
      MergeTable(const SymbolsManager& symbolsManager)
         :  uCentralSymbolsLimit(symbolsManager.lsCentralSymbols.count()),
            uNoiseSymbolsLimit(symbolsManager.lsNoiseSymbols.count()),
            uDefinedSymbolsLimit(symbolsManager.lsDefinedSymbols.count()),
            uHighLevelSymbolsLimit(symbolsManager.uHighLevelSymbolsCounter),
            uCentralSymbolsAfterMerge(0), uNoiseSymbolsAfterMerge(0),
            uDefinedSymbolsAfterMerge(0), uHighLevelSymbolsAfterMerge(0) {}

      void setInitial(const SymbolsManager& symbolsManager)
         {  uCentralSymbolsLimit = symbolsManager.lsCentralSymbols.count();
            uNoiseSymbolsLimit = symbolsManager.lsNoiseSymbols.count();
            uDefinedSymbolsLimit = symbolsManager.lsDefinedSymbols.count();
            uHighLevelSymbolsLimit = symbolsManager.uHighLevelSymbolsCounter;
         }
      void setAfterMerge(const SymbolsManager& symbolsManager)
         {  uCentralSymbolsAfterMerge = symbolsManager.lsCentralSymbols.count();
            uNoiseSymbolsAfterMerge = symbolsManager.lsNoiseSymbols.count();
            uDefinedSymbolsAfterMerge = symbolsManager.lsDefinedSymbols.count();
            uHighLevelSymbolsAfterMerge = symbolsManager.uHighLevelSymbolsCounter;
         }

      Symbol& findCentralSymbol(int index, SymbolsManager& symbolsManager) const
         {  if (index <= uCentralSymbolsLimit)
               return symbolsManager.findCentralSymbol(index);
            index += uCentralSymbolsAfterMerge-uCentralSymbolsLimit;
            while (symbolsManager.lsCentralSymbols.count() < index)
               symbolsManager.createCentralSymbol();
            return symbolsManager.findCentralSymbol(index);
         }
      Symbol& findNoiseSymbol(int index, SymbolsManager& symbolsManager) const
         {  if (index <= uNoiseSymbolsLimit)
               return symbolsManager.findNoiseSymbol(index);
            index += uNoiseSymbolsAfterMerge-uNoiseSymbolsLimit;
            while (symbolsManager.lsNoiseSymbols.count() < index)
               symbolsManager.createNoiseSymbol();
            return symbolsManager.findNoiseSymbol(index);
         }
      template <class TypeTraits>
      Symbol& findDefinedSymbol(TypeTraits traits, int index, SymbolsManager& symbolsManager,
            typename TypeTraits::Equation&& definition) const
         {  if (index <= uDefinedSymbolsLimit)
               return symbolsManager.findDefinedSymbol(index);
            index += uDefinedSymbolsAfterMerge-uDefinedSymbolsLimit;
            while (symbolsManager.lsDefinedSymbols.count() < index) {
               Symbol* symbol = symbolsManager.createUnrecordedDefinedSymbol(traits);
               symbolsManager.recordDefinedSymbol(traits, symbol);
            };
            Symbol& aresult = symbolsManager.findDefinedSymbol(index);
            AssumeCondition(dynamic_cast<typename TypeTraits::DefinedSymbol*>(&aresult))
            typename TypeTraits::DefinedSymbol& result = (typename TypeTraits::DefinedSymbol&) aresult;
            if (result.getDefinition().isAtomic() && result.getDefinition().getCentral().isZero())
               result.getSDefinition() = definition;
            // else {
            //    diff = definition;
            //    BuiltReal highLevelError;
            //    diff.plusAssign(result.getDefinition(), PCSourceRValue, highLevelError, true /* isMinus */, false /* hasExternalCoefficient */);
            //    if (!diff.isAtomic() || !diff.getCentral().isZero() || !highLevelError.isZero())
            //       throw STG::EReadError();
            // };
            return aresult;
         }
      Symbol& findHighLevelSymbol(int index, SymbolsManager& symbolsManager) const
         {  if (index <= uHighLevelSymbolsLimit)
               return symbolsManager.findHighLevelSymbol(index);
            index += uHighLevelSymbolsAfterMerge-uHighLevelSymbolsLimit;
            while (symbolsManager.uHighLevelSymbolsCounter < index)
               symbolsManager.createHighLevelSymbol();
            return symbolsManager.findHighLevelSymbol(index);
         }
   };
   virtual Symbol* createCentralSymbol()
      {  Symbol* result = new CentralSymbol();
         lsCentralSymbols.insertNewAtEnd(result);
         result->setOrder(lsCentralSymbols.count());
         return result;
      }
   virtual Symbol* createNoiseSymbol()
      {  Symbol* result = new NoiseSymbol();
         lsNoiseSymbols.insertNewAtEnd(result);
         result->setOrder(lsNoiseSymbols.count());
         return result;
      }
   virtual Symbol* createHighLevelSymbol()
      {  Symbol* result = new HighLevelSymbol();
         lsHighLevelSymbols.insertNewAtEnd(result);
         result->setOrder(++uHighLevelSymbolsCounter);
         return result;
      }
   void freeHighLevelSymbol(HighLevelSymbol* symbol) // [TODO] verify the manager for LinearInvariantSynthesis!!!
      {  
#if DefineDebugLevel >= 3
         AssumeCondition(!lsHighLevelSymbols.foreachDo([symbol](const Symbol& iterate)
            {  return &iterate != symbol; }))
#endif
         COL::TList<Symbol, Symbol::Registration>::Cursor cursor(lsHighLevelSymbols);
         cursor.gotoReference(*symbol);
         lsHighLevelSymbols.freeAt(cursor);
      }
   void freeSymbol(Symbol* symbol, bool hasShift=false)
      {  switch (symbol->getType()) {
            case Symbol::TCentral:
               {  AssumeCondition(hasShift || symbol->getOrder() <= lsCentralSymbols.count())
#if DefineDebugLevel >= 3
                  AssumeCondition(!lsCentralSymbols.foreachDo([symbol](const Symbol& iterate)
                     {  return &iterate != symbol; }))
#endif
                  COL::TList<Symbol, Symbol::Registration>::Cursor cursor(lsCentralSymbols);
                  cursor.gotoReference(*symbol);
                  lsCentralSymbols.freeAt(cursor);
               }
               break;
            case Symbol::TNoise:
            case Symbol::TMarkedNoise:
               {  AssumeCondition(hasShift || symbol->getOrder() <= lsNoiseSymbols.count())
#if DefineDebugLevel >= 3
                  AssumeCondition(!lsNoiseSymbols.foreachDo([symbol](const Symbol& iterate)
                     {  return &iterate != symbol; }))
#endif
                  COL::TList<Symbol, Symbol::Registration>::Cursor cursor(lsNoiseSymbols);
                  cursor.gotoReference(*symbol);
                  lsNoiseSymbols.freeAt(cursor);
               }
               break;
            case Symbol::TClosed:
               AssumeUncalled
            case Symbol::TDefined:
               {  AssumeCondition(hasShift || symbol->getOrder() <= lsDefinedSymbols.count())
#if DefineDebugLevel >= 3
                  AssumeCondition(!lsDefinedSymbols.foreachDo([symbol](const Symbol& iterate)
                     {  return &iterate != symbol; }))
#endif
                  COL::TList<Symbol, Symbol::Registration>::Cursor cursor(lsDefinedSymbols);
                  cursor.gotoReference(*symbol);
                  lsDefinedSymbols.freeAt(cursor);
               }
               break;
            case Symbol::THighLevel:
               {  AssumeCondition(hasShift || symbol->getOrder() <= uHighLevelSymbolsCounter)
#if DefineDebugLevel >= 3
                  AssumeCondition(!lsHighLevelSymbols.foreachDo([symbol](const Symbol& iterate)
                     {  return &iterate != symbol; }))
#endif
                  COL::TList<Symbol, Symbol::Registration>::Cursor cursor(lsHighLevelSymbols);
                  cursor.gotoReference(*symbol);
                  lsHighLevelSymbols.freeAt(cursor);
               }
               break;
            case Symbol::TConstrained:
               AssumeUncalled
            default:
               AssumeUncalled
         };
      }
   int getHighLevelSymbolOrder() const { return uHighLevelSymbolsCounter+1; }
   template <class TypeTraits>
   Symbol* createDefinedSymbol(TypeTraits traits, const typename TypeTraits::Equation& definition)
      {  Symbol* result = new typename TypeTraits::DefinedSymbol(definition);
         lsDefinedSymbols.insertNewAtEnd(result);
         result->setOrder(lsDefinedSymbols.count());
         return result;
      }
   template <class TypeTraits>
   Symbol* createUnrecordedDefinedSymbol(TypeTraits traits)
      {  Symbol* result = new typename TypeTraits::DefinedSymbol(typename TypeTraits::Equation());
         return result;
      }
   template <class TypeTraits>
   void recordDefinedSymbol(TypeTraits traits, Symbol* symbol)
      {  AssumeCondition(dynamic_cast<const typename TypeTraits::DefinedSymbol*>(symbol)
               && !symbol->hasOrder())
         lsDefinedSymbols.insertNewAtEnd(symbol);
         symbol->setOrder(lsDefinedSymbols.count());
      }
   Symbol* createMarkedNoiseSymbol(int order)
      {  Symbol* result = new MarkedNoiseSymbol(order);
         lsNoiseSymbols.insertNewAtEnd(result);
         return result;
      }
#if defined(__clang__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wempty-body"
#endif
   Symbol& findDefinedSymbol(int index) const
      {  COL::TList<Symbol, Symbol::Registration>::Cursor cursor(lsDefinedSymbols);
         Symbol* result = nullptr;
         while (cursor.setToPrevious() && (result = &cursor.elementSAt())->getOrder() > index);
         AssumeCondition(cursor.isValid() && result->getOrder() == index)
         return *result;
      }
   Symbol& findCentralSymbol(int index) const
      {  COL::TList<Symbol, Symbol::Registration>::Cursor cursor(lsCentralSymbols);
         Symbol* result = nullptr;
         while (cursor.setToPrevious() && (result = &cursor.elementSAt())->getOrder() > index);
         AssumeCondition(cursor.isValid() && result->getOrder() == index)
         return *result;
      }
   Symbol& findNoiseSymbol(int index) const
      {  COL::TList<Symbol, Symbol::Registration>::Cursor cursor(lsNoiseSymbols);
         Symbol* result = nullptr;
         while (cursor.setToPrevious() && (result = &cursor.elementSAt())->getOrder() > index);
         AssumeCondition(cursor.isValid() && result->getOrder() == index)
         return *result;
      }
   Symbol& findHighLevelSymbol(int index) const
      {  COL::TList<Symbol, Symbol::Registration>::Cursor cursor(lsHighLevelSymbols);
         Symbol* result = nullptr;
         while (cursor.setToPrevious() && (result = &cursor.elementSAt())->getOrder() > index);
         AssumeCondition(cursor.isValid() && result->getOrder() == index)
         return *result;
      }
   Symbol* locateHighLevelSymbol(int index) const
      {  COL::TList<Symbol, Symbol::Registration>::Cursor cursor(lsHighLevelSymbols);
         Symbol* result = nullptr;
         while (cursor.setToPrevious() && (result = &cursor.elementSAt())->getOrder() > index);
         return cursor.isValid() ? result : nullptr;
      }
#if defined(__clang__)
#pragma GCC diagnostic pop
#endif

   int getCountCentralSymbols() const { return lsCentralSymbols.count(); }
   int getCountNoiseSymbols() const { return lsNoiseSymbols.count(); }
   int getCountDefinedSymbols() const { return lsDefinedSymbols.count(); }
   int getMaxHighLevelSymbolOrder() const { return uHighLevelSymbolsCounter; }

   void moveCountedSymbolsTo(SymbolsManager& receiver)
      {  lsCentralSymbols.moveAllTo(receiver.lsCentralSymbols);
         lsNoiseSymbols.moveAllTo(receiver.lsNoiseSymbols);
         lsDefinedSymbols.moveAllTo(receiver.lsDefinedSymbols);
      }
   void moveHighLevelSymbolTo(Symbol& symbol, SymbolsManager& receiver, Symbol* beforeReceiver)
      {  COL::TList<Symbol, Symbol::Registration>::Cursor
            cursor(lsHighLevelSymbols), receiverCursor(receiver.lsHighLevelSymbols);
         cursor.gotoReference(symbol);
         if (beforeReceiver) {
            AssumeCondition(beforeReceiver->getOrder() < symbol.getOrder())
            receiverCursor.gotoReference(*beforeReceiver);
         }
         lsHighLevelSymbols.moveTo(receiver.lsHighLevelSymbols, COL::List::ExtendedReplaceParameters()
               .setRemoveExact().setInsertionPosition(COL::VirtualCollection::RPAfter),
               &cursor, &receiverCursor);
      }
   void freeCentralSymbolsAfter(int centralOrder)
      {  while (!lsCentralSymbols.isEmpty() && lsCentralSymbols.getLast().getOrder() > centralOrder)
            lsCentralSymbols.freeLast();
      }
   void freeNoiseSymbolsAfter(int noiseOrder)
      {  while (!lsNoiseSymbols.isEmpty() && lsNoiseSymbols.getLast().getOrder() > noiseOrder)
            lsNoiseSymbols.freeLast();
      }
   void freeDefinedSymbolsAfter(int definedOrder)
      {  while (!lsDefinedSymbols.isEmpty() && lsDefinedSymbols.getLast().getOrder() > definedOrder)
            lsDefinedSymbols.freeLast();
      }
   void freeHighLevelSymbolsAfter(int highLevelOrder)
      {  while (!lsHighLevelSymbols.isEmpty() && lsHighLevelSymbols.getLast().getOrder() > highLevelOrder)
            lsHighLevelSymbols.freeLast();
         uHighLevelSymbolsCounter = highLevelOrder;
      }
};

class ClosedSymbolsSet : public EnhancedObject {
  private:
   typedef EnhancedObject inherited;
   COL::TCopyCollection<COL::TList<Symbol, Symbol::Registration> > lsSymbols;
   int uClosedCounter;

  public:
   ClosedSymbolsSet() : uClosedCounter(0) {}
   ClosedSymbolsSet(const ClosedSymbolsSet& source) : uClosedCounter(0) {}
   DefineCopy(ClosedSymbolsSet)

   Symbol* createClosedSymbol(int level)
      {  Symbol* result = new ClosedSymbol(level);
         lsSymbols.insertNewAtEnd(result);
         result->setOrder(uClosedCounter++);
         return result;
      }
};

inline ComparisonResult
ClosedSymbol::_compare(const EnhancedObject& asource) const {
   const Symbol& source = (const Symbol&) Symbol::castFromCopyHandler(asource);
   ComparisonResult result = CREqual;
   if (source.getType() != TClosed)
      result = inherited::_compare(asource);
   else {
      AssumeCondition(dynamic_cast<const ClosedSymbol*>(&source))
      result = fcompare(uLevel, ((const ClosedSymbol&) source).uLevel);
      if (result == CREqual)
         result = inherited::_compare(asource);
   };
   return result;
}

}} // end of namespace NumericalDomains::DAffine

#endif // NumericalDomains_FloatAffineBaseCommonH

