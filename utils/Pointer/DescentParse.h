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
// Unit      : Parser
// File      : DescentParse.h
// Description :
//   Definition of classes to implement recursive descent parsers.
//

#ifndef DescentParserH
#define DescentParserH

#include "StandardClasses/Persistence.h"
#include "Pointer/AutoPointer.h"
#include "Pointer/ImplArray.h"

namespace STG {

namespace Parser {

/******************************************/
/* Definition of the template TStateStack */
/******************************************/

template <class TypeArguments>
class TStateStack : public EnhancedObject, public Lexer::Base {
  private:
   typedef TStateStack<TypeArguments> thisType;
   typedef EnhancedObject inherited;

  public:
   typedef TypeArguments ParseArgument;
   class VirtualParseState : public EnhancedObject {
     private:
      int uPoint;
      PNT::AutoPointer<EnhancedObject> apeoResult;
      typedef EnhancedObject inherited;

     protected:
      virtual ComparisonResult _compare(const EnhancedObject& asource) const override
         {  ComparisonResult result = inherited::_compare(asource);
            const VirtualParseState& source = (const VirtualParseState&) asource;
            return (result == CREqual) ? fcompare(uPoint, source.uPoint) : result;
         }

     public:
      VirtualParseState() : uPoint(0) {}
      VirtualParseState(const VirtualParseState& source) = default;
      DefineCopy(VirtualParseState)
      DDefineAssign(VirtualParseState)

      int& point() { return uPoint; }
      const int& point() const { return uPoint; }
      virtual ReadResult operator()(TStateStack<TypeArguments>& parser, ParseArgument& args) 
         {  AssumeUncalled return RRFinished; }

      bool hasResult() const { return apeoResult.isValid(); }
      RuleResult& getResult() const { return *apeoResult; }
      void absorbResult(RuleResult* ruleResult)
         {  if (apeoResult.isValid()) {
               if (ruleResult) delete ruleResult;
               AssumeUncalled
            };
            apeoResult.absorbElement(ruleResult);
         }
      void changeAndAbsorbResult(RuleResult* ruleResult) { apeoResult.absorbElement(ruleResult); }
      void freeResult() { apeoResult.release(); }
      RuleResult* extractResult() { return apeoResult.extractElement(); }
   };

   template <class TypeObject, typename ReadPointerMethod>
   class TParseState : public VirtualParseState {
     private:
      ReadPointerMethod rpmReadMethod;
      TypeObject* poObject;
      typedef VirtualParseState inherited;
      typedef TParseState<TypeObject, ReadPointerMethod> thisType;

     protected:
      virtual ComparisonResult _compare(const EnhancedObject& asource) const override
         {  ComparisonResult result = inherited::_compare(asource);
            const thisType& source = (const thisType&) asource;
            return (rpmReadMethod != source.rpmReadMethod) ? CRNonComparable : result;
         }

     public:
      TParseState() : poObject(nullptr), rpmReadMethod(nullptr) {}
      TParseState(TypeObject& object, const ReadPointerMethod& readMethod)
         :  rpmReadMethod(readMethod), poObject(&object) {}
      TParseState(const thisType& source) = default;
      thisType& operator=(const thisType&) = default;
      Template2DefineCopy(TParseState, TypeObject, ReadPointerMethod)
      DTemplate2DefineAssign(TParseState, TypeObject, ReadPointerMethod)

      virtual ReadResult operator()(TStateStack<TypeArguments>& stateStack, ParseArgument& arguments) override
         {  return (poObject->*rpmReadMethod)(stateStack, arguments); }
      const ReadPointerMethod& getStateMethod() const { return rpmReadMethod; }
      void change(TypeObject& object, ReadPointerMethod readMethod, int point)
         {  poObject = &object;
            rpmReadMethod = readMethod;
            inherited::point() = point;
         }
      bool hasObjectRead(const TypeObject& object, ReadPointerMethod readMethod)
         {  return (poObject == &object) && (rpmReadMethod == readMethod); }
      bool hasMethodRead(ReadPointerMethod readMethod)
         {  return (rpmReadMethod == readMethod); }
   };

   template <class TypeObject, typename ReadPointerMethod, class TypeParseMultiState>
   class TLevelParseState : public VirtualParseState {
     private:
      ReadPointerMethod rpmReadMethod;
      TypeObject* poObject;
      typedef VirtualParseState inherited;
      typedef TLevelParseState<TypeObject, ReadPointerMethod, TypeParseMultiState> thisType;

     protected:
      virtual ComparisonResult _compare(const EnhancedObject& asource) const
         {  ComparisonResult result = inherited::_compare(asource);
            const thisType& source = (const thisType&) asource;
            return (rpmReadMethod != source.rpmReadMethod) ? CRNonComparable : result;
         }

     public:
      TLevelParseState() : poObject(nullptr), rpmReadMethod(nullptr) {}
      TLevelParseState(TypeObject& object, const ReadPointerMethod& readMethod)
         :  rpmReadMethod(readMethod), poObject(&object) {}
      TLevelParseState(const thisType& source) = default;
      Template3DefineCopy(TLevelParseState, TypeObject, ReadPointerMethod, TypeParseMultiState)
      DTemplate3DefineAssign(TLevelParseState, TypeObject, ReadPointerMethod, TypeParseMultiState)

      virtual ReadResult operator()(TStateStack<TypeArguments>& stateStack, ParseArgument& arguments) 
         { return (poObject->*rpmReadMethod)((TypeParseMultiState&) stateStack,
             (typename TypeParseMultiState::ParseArgument&) arguments);
         }
      const ReadPointerMethod& getStateMethod() const { return rpmReadMethod; }
      void change(TypeObject& object, ReadPointerMethod readMethod, int point)
         {  poObject = &object;
            rpmReadMethod = readMethod;
            inherited::point() = point;
         }
      bool hasObjectRead(const TypeObject& object, ReadPointerMethod readMethod)
         {  return (poObject == &object) && (rpmReadMethod == readMethod); }
      bool hasMethodRead(ReadPointerMethod readMethod)
         {  return (rpmReadMethod == readMethod); }
   };

  protected:
   typedef COL::TImplArray<VirtualParseState> ArrayParseStates;

  private:
   ArrayParseStates apsStates;

  protected:
   ArrayParseStates& states() { return apsStates; }
   const ArrayParseStates& states() const { return apsStates; }

   template <class TypeObject, typename ReadPointerMethod, class SpecializedThis>
   thisType& _shift(TypeObject& object, ReadPointerMethod parseMethod, SpecializedThis* thisState)
      {  apsStates.insertAtEnd(new TLevelParseState<TypeObject, ReadPointerMethod, SpecializedThis>(object, parseMethod));
         return *this;
      }
   template <class TypeObject, typename ReadPointerMethod, class SpecializedThis>
   SpecializedThis& _change(TypeObject& object, ReadPointerMethod parseMethod, int newPoint, SpecializedThis* thisState)
      {  typedef TLevelParseState<TypeObject, ReadPointerMethod, SpecializedThis> ParseState;
         VirtualParseState* lastState = apsStates.elementAt(apsStates.count()-1);
         AssumeCondition(dynamic_cast<ParseState*>(lastState) != nullptr)
         ((ParseState&) *lastState).change(object, parseMethod, newPoint);
         return (SpecializedThis&) *this;
      }
   template <class TypeObject, typename ReadPointerMethod, class SpecializedThis>
   bool _tisAlive(TypeObject& object, ReadPointerMethod parseMethod, int level, SpecializedThis* thisState)
      {  typedef TLevelParseState<TypeObject, ReadPointerMethod, SpecializedThis> ParseState;
         return (apsStates.count() > level) && dynamic_cast<ParseState*>(apsStates[level])
            && ((ParseState&) *apsStates[level]).hasObjectRead(object, parseMethod);
      }
   template <class TypeObject, typename ReadPointerMethod, class SpecializedThis>
   bool _tisAlive(TypeObject* object, ReadPointerMethod parseMethod, int level, SpecializedThis* thisState)
      {  typedef TLevelParseState<TypeObject, ReadPointerMethod, SpecializedThis> ParseState;
         return (apsStates.count() > level) && dynamic_cast<ParseState*>(apsStates[level])
            && ((ParseState&) *apsStates[level]).hasMethodRead(parseMethod);
      }
   template <class TypeObject, typename ReadPointerMethod, class SpecializedThis>
   bool _tisParentAlive(TypeObject* object, ReadPointerMethod parseMethod, SpecializedThis* thisState)
      {  typedef TLevelParseState<TypeObject, ReadPointerMethod, SpecializedThis> ParseState;
         return (apsStates.count() > 1) && dynamic_cast<ParseState*>(apsStates[apsStates.count()-2])
            && ((ParseState&) apsStates[apsStates.count()-2]).hasMethodRead(parseMethod);
      }

  public:
   TStateStack() {}
   TStateStack(thisType&& source) { apsStates.swap(source.apsStates); }
   TStateStack(const thisType& source) { apsStates.addAll(source.apsStates, 0, -1, -1, true); }
   thisType& operator=(const thisType& source) { return (thisType&) inherited::operator=(source); }
   virtual ~TStateStack() { apsStates.removeAll(true); }
   TemplateDefineCopy(TStateStack, TypeArguments)

   void clear() { apsStates.removeAll(true); }
   void swap(thisType& source) { apsStates.swap(source.apsStates); }

   ReadResult parse(ParseArgument& arguments)
      {  int count = apsStates.count();
         return (count != 0) ? (*apsStates[count-1])(*this, arguments) : RRFinished; }

   template <class TypeObject, typename ReadPointerMethod>
   thisType& shift(TypeObject& object, ReadPointerMethod parseMethod)
      {  apsStates.insertAtEnd(new TParseState<TypeObject, ReadPointerMethod>(object, parseMethod));
         return *this;
      }

   template <class TypeObject, typename ReadPointerMethod>
   thisType& change(TypeObject& object, ReadPointerMethod parseMethod, int newPoint)
      {  typedef TParseState<TypeObject, ReadPointerMethod> ParseState;
         VirtualParseState* lastState = apsStates.elementAt(apsStates.count()-1);
         AssumeCondition(dynamic_cast<ParseState*>(lastState) != nullptr)
         ((ParseState&) *lastState).change(object, parseMethod, newPoint);
         return *this;
      }
   template <class TypeObject, typename ReadPointerMethod>
   bool tisAlive(TypeObject& object, ReadPointerMethod parseMethod, int level)
      {  typedef TParseState<TypeObject, ReadPointerMethod> ParseState;
         return (apsStates.count() > level) && dynamic_cast<ParseState*>(apsStates[level])
            && ((ParseState&) *apsStates[level]).hasObjectRead(object, parseMethod);
      }
   template <class TypeObject, typename ReadPointerMethod>
   bool tisAlive(TypeObject* object, ReadPointerMethod parseMethod, int level)
      {  typedef TParseState<TypeObject, ReadPointerMethod> ParseState;
         return (apsStates.count() > level) && dynamic_cast<ParseState*>(apsStates[level])
            && ((ParseState&) *apsStates[level]).hasMethodRead(parseMethod);
      }
   template <class TypeObject, typename ReadPointerMethod>
   bool tisParentAlive(TypeObject* object, ReadPointerMethod parseMethod)
      {  typedef TParseState<TypeObject, ReadPointerMethod> ParseState;
         return (apsStates.count() > 1) && dynamic_cast<ParseState*>(apsStates[apsStates.count()-2])
            && ((ParseState&) *apsStates[apsStates.count()-2]).hasMethodRead(parseMethod);
      }
   bool isAlive(int level, int point) const
      {  return (apsStates.count() > level) && (apsStates[level]->point() == point); }
   bool isLessThan(int level, int point) const
      {  return (apsStates.count() > level) && (apsStates[level]->point() < point); }
   thisType& reduce()
      {  VirtualParseState* oldState = apsStates[apsStates.count()-1];
         if (oldState) delete oldState;
         apsStates.removeAtEnd();
         return *this;
      }

   const int& point() const { return apsStates.getLast().point(); }
   int& point() { return apsStates[apsStates.count()-1]->point(); }
   int getLevel() const { return apsStates.count()-1; }

   VirtualParseState& last() { return *apsStates[apsStates.count()-1]; }
   const VirtualParseState& last() const { return *apsStates[apsStates.count()-1]; }
   bool isEmpty() const { return apsStates.count() == 0; }
   const VirtualParseState& upLast() const { return *apsStates[apsStates.count()-2]; }

   void absorbRuleResult(RuleResult* result) { last().absorbResult(result); }
   void changeRuleResult(RuleResult* result) { last().changeAndAbsorbResult(result); }
   void freeRuleResult() { last().freeResult(); }
   bool hasRuleResult() const { return last().hasResult(); }
   bool hasParentRuleResult() const { return upLast().hasResult(); }

   class ObjectReference {
     private:
      RuleResult& eoResult;

     public:
      ObjectReference(RuleResult& result) : eoResult(result) {}
      ObjectReference(const ObjectReference& source) = default;
      template <class Type> operator Type*() const
         {  AssumeCondition(dynamic_cast<Type*>(&eoResult))
            return (Type*) &eoResult;
         }
      template <class TypeCastParameter>
      typename TypeCastParameter::Element& getUpCast(TypeCastParameter) const
         {  if (!dynamic_cast<typename TypeCastParameter::Element*>(TypeCastParameter::Cast::castFrom(&eoResult)))
               throw STG::EReadError();
            return (typename TypeCastParameter::Element&) TypeCastParameter::Cast::castFrom(eoResult);
         }
   };

   class ObjectKeepReference {
     private:
      EnhancedObject* peoResult;

     public:
      ObjectKeepReference(EnhancedObject* result) : peoResult(result) {}
      ObjectKeepReference(ObjectKeepReference&& source) : peoResult(source.peoResult)
         {  source.peoResult = nullptr; }
      ObjectKeepReference(const ObjectKeepReference& source) : peoResult(source.peoResult)
         {  const_cast<ObjectKeepReference&>(source).peoResult = nullptr; }
      ~ObjectKeepReference() { if (peoResult) delete peoResult; }

      template <class Type> operator Type*()
         {  AssumeCondition(dynamic_cast<Type*>(peoResult))
            Type* result = (Type*) peoResult;
            peoResult = nullptr;
            return result;
         }
      template <class TypeCastParameter>
      typename TypeCastParameter::Element* getUpCast(TypeCastParameter)
         {  AssumeCondition(dynamic_cast<typename TypeCastParameter::Element*>(TypeCastParameter::Cast::castFrom(peoResult)))
            typename TypeCastParameter::Element* result = (typename TypeCastParameter::Element*) TypeCastParameter::Cast::castFrom(peoResult);
            peoResult = nullptr;
            return result;
         }
   };

   class RuleAccess {
     public:
      template <class TypeObject>
      class TCastFromRule {
        private:
         TypeObject* poObject;
         
        public:
         explicit TCastFromRule(const ObjectReference& result) : poObject((TypeObject*) result) {}
         operator TypeObject&() const { return *poObject; }
         TypeObject& operator*() const { return *poObject; }
         TypeObject* operator->() const { return poObject; }
         TypeObject* get() const { return poObject; }
      };

      template <class TypeObject>
      class TMCastFromRule {
        private:
         TypeObject* poObject;
         
        public:
         explicit TMCastFromRule(const ObjectReference& result)
            :  poObject(&result.getUpCast(TemplateElementCastParameters<TypeObject, HandlerCopyCast<TypeObject> >())) {}
         operator TypeObject&() const { return *poObject; }
         TypeObject& operator*() const { return *poObject; }
         TypeObject* operator->() const { return poObject; }
         TypeObject* get() const { return poObject; }
      };

      template <class TypeCastParameter>
      class TCastParameterFromRule {
        private:
         typename TypeCastParameter::Element* peObject;
         
        public:
         explicit TCastParameterFromRule(const ObjectReference& orResult)
            :  peObject(&orResult.getUpCast(TypeCastParameter())) {}
         operator typename TypeCastParameter::Element&() const { return *peObject; }
         typename TypeCastParameter::Element& operator*() const { return *peObject; }
         typename TypeCastParameter::Element* operator->() const { return peObject; }
         typename TypeCastParameter::Element* get() const { return peObject; }
      };
   };
   

   RuleResult* extractRuleResult() { return last().extractResult(); }
   ObjectReference getRuleResult() const
      {  return ObjectReference(last().getResult()); }
   ObjectReference getRuleResultAt(int uLevel) const
      {  return ObjectReference(apsStates[uLevel]->getResult()); }
   ObjectKeepReference extractSRuleResult()
      {  return ObjectKeepReference(last().extractResult()); }
   ObjectReference getParentRuleResult() const
      {  return ObjectReference(upLast().getResult()); }
};

}} // end of namespace STG::Parser

#endif // DescentParserH

