/**************************************************************************/
/*                                                                        */
/*  This file is part of FLDLib                                           */
/*                                                                        */
/*  Copyright (C) 2005-2017                                               */
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
// Library   : ForwardNumerics
// Unit      : Floating
// File      : BaseFloatingParameters.h
// Description :
//   Definition of the floating point flags
//

#ifndef Numerics_BaseFloatingParametersH
#define Numerics_BaseFloatingParametersH

namespace Numerics { namespace DDouble {

class Access : public DInteger::Access {
  public:
#ifdef StandardClassesHPP
   class ReadParameters : public STG::IOObject::FormatParameters {
     private:
      typedef STG::IOObject::FormatParameters inherited;

     public:
      enum RoundMode { RMNearest, RMLowest, RMHighest, RMZero };
      enum Approximation { AExact, ADownApproximate, AUpApproximate };
      enum ReadResult { RRTotal, RRPartial };
      enum Context { CMinDown, CMaxDown, CMinUp, CMaxUp };

     protected:
      DefineExtendedParameters(27, STG::IOObject::FormatParameters)
     public:
      DefineSubExtendedParameters(StaticMachine, 13, INHERITED)
     protected:
      DefineSubExtendedParameters(AvoidInfty, 1, INHERITED)
      DefineSubExtendedParameters(KeepNaNSign, 1, AvoidInfty)
      DefineSubExtendedParameters(ProduceDivNaNPositive, 1, KeepNaNSign)
      DefineSubExtendedParameters(RoundToEven, 1, ProduceDivNaNPositive)
      DefineSubExtendedParameters(PositiveZeroMAdd, 1, RoundToEven)
      DefineSubExtendedParameters(UpApproximateInfty, 1, PositiveZeroMAdd)
      DefineSubExtendedParameters(UpApproximateInversionForNear, 1, UpApproximateInfty)
      DefineSubExtendedParameters(ChooseNaNAddBeforeMult, 1, UpApproximateInversionForNear)
      DefineSubExtendedParameters(ConvertNaNNegative, 1, ChooseNaNAddBeforeMult)
      DefineSubExtendedParameters(RefuseMinusZero, 1, ConvertNaNNegative)
      DefineSubExtendedParameters(Context, 2, RefuseMinusZero)
      DefineSubExtendedParameters(InverseContext, 1, Context)

     public:
      DefineSubExtendedParameters(StaticMode, 3, InverseContext)
     protected:
      DefineSubExtendedParameters(Round, 2, InverseContext)
      DefineSubExtendedParameters(KeepSignalingConversion, 1, Round)

     public:
      DefineSubExtendedParameters(DynamicWrite, 11, KeepSignalingConversion)
      DefineSubExtendedParameters(Approximation, 2, KeepSignalingConversion)
      DefineSubExtendedParameters(ReadResult, 1, Approximation)
      DefineSubExtendedParameters(EffectiveRoundToEven, 1, ReadResult)
      DefineSubExtendedParameters(SNaNOperand, 1, EffectiveRoundToEven)
      DefineSubExtendedParameters(QNaNResult, 3, SNaNOperand)
      DefineSubExtendedParameters(FlowException, 2, QNaNResult)
      DefineSubExtendedParameters(DivisionByZero, 1, FlowException)

     protected:
      enum FlowException { FENoException, FEOverflow, FEUnderflow, FEEnd };
      enum QNaNResult
         {  QNNRUndefined, QNNRInftyMinusInfty, QNNRInftyOnInfty, QNNRZeroOnZero,
            QNNRInftyMultZero
         };
      
     public:
      ReadParameters() : inherited() {}
      ReadParameters(const ReadParameters& source) : inherited(source) {}
      ReadParameters& operator=(const ReadParameters& source)
         { setDynamicWriteField(source.queryDynamicWriteField()); return *this; }

      // Static parameters
      ReadParameters& setRoundToEven()
         {  setRoundField(RMNearest); mergeRoundToEvenField(1); return *this; }
      ReadParameters& clearRoundToEven() {  clearRoundToEvenField(); return *this; }
      ReadParameters& setPositiveZeroMAdd() { mergePositiveZeroMAddField(1); return *this; }
      ReadParameters& avoidInfty() { mergeAvoidInftyField(1); return *this; }
      ReadParameters& clearAvoidInfty() { clearAvoidInftyField(); return *this; }
      ReadParameters& setKeepNaNSign() { mergeKeepNaNSignField(1); return *this; }
      ReadParameters& setProduceDivNaNPositive() { mergeProduceDivNaNPositiveField(1); return *this; }
      ReadParameters& setUpApproximateInfty() { mergeUpApproximateInftyField(1); return *this; }
      ReadParameters& setUpApproximateInversionForNear() { mergeUpApproximateInversionForNearField(1); return *this; }
      ReadParameters& setChooseNaNAddBeforeMult() { mergeChooseNaNAddBeforeMultField(1); return *this; }
      ReadParameters& setConvertNaNNegative() { mergeConvertNaNNegativeField(1); return *this; }
      ReadParameters& setAcceptMinusZero() { clearRefuseMinusZeroField(); return *this; }
      ReadParameters& setRefuseMinusZero() { mergeRefuseMinusZeroField(1); return *this; }

      ReadParameters& setMinDown() { setContextField(CMinDown); return *this; }
      ReadParameters& setMinUp() { setContextField(CMinUp); return *this; }
      ReadParameters& setMaxDown() { setContextField(CMaxDown); return *this; }
      ReadParameters& setMaxUp() { setContextField(CMaxUp); return *this; }
      ReadParameters& setInverseContext() { mergeInverseContextField(1); return *this; }
      ReadParameters& clearInverseContext() { clearInverseContextField(); return *this; }

      bool isContextUp() const { return queryContextField() & CMinUp; }
      bool isContextMax() const { return queryContextField() & CMaxDown; }
      bool doesInverseContext() const { return hasInverseContextField(); }

      bool doesSupportInstantiation() const { return !hasOwnField(); }
      bool isRoundToEven() const { return hasRoundToEvenField() && isNearestRound(); }
      bool isPositiveZeroMAdd() { return hasPositiveZeroMAddField(); }
      bool isInftyAvoided() const { return hasAvoidInftyField(); }
      bool doesAvoidInfty(bool isNegative) const
         {  AssumeCondition(hasAvoidInftyField())
            RoundMode mode = (RoundMode) queryRoundField();
            return isNegative ? (mode >= RMHighest) : (mode & RMLowest);
         }
      bool keepNaNSign() const { return hasKeepNaNSignField(); }
      bool produceDivNaNPositive() const { return hasProduceDivNaNPositiveField(); }
      bool upApproximateInfty() const { return hasUpApproximateInftyField(); }
      bool upApproximateInversionForNear() const { return hasUpApproximateInversionForNearField(); }
      bool chooseNaNAddBeforeMult() const { return hasChooseNaNAddBeforeMultField(); }
      bool isConvertNaNNegative() const { return hasConvertNaNNegativeField(); }
      bool acceptMinusZero() const { return !hasRefuseMinusZeroField(); }

      // dynamic read parameters
      ReadParameters& setRoundMode(RoundMode mode) { setRoundField(mode); return *this; }
      ReadParameters& setNearestRound()   { setRoundField(RMNearest); return *this; }
      ReadParameters& setHighestRound()   { setRoundField(RMHighest); return *this; }
      ReadParameters& setLowestRound()    { setRoundField(RMLowest); return *this; }
      ReadParameters& setZeroRound()      { setRoundField(RMZero); return *this; }

      bool isLowestRound() const { return queryRoundField() == RMLowest; }
      bool isNearestRound() const { return queryRoundField() == RMNearest; }
      bool isHighestRound() const { return queryRoundField() == RMHighest; }
      bool isZeroRound() const { return queryRoundField() == RMZero; }
      RoundMode getRoundMode() const { return (RoundMode) queryRoundField(); }

      ReadParameters& setKeepSignalingConversion() { mergeKeepSignalingConversionField(1); return *this; }
      ReadParameters& clearKeepSignalingConversion() { clearKeepSignalingConversionField(); return *this; }
      bool keepSignalingConversion() const { return hasKeepSignalingConversionField(); }

      // dynamic write parameters
      bool isApproximate() const { return hasApproximationField(); }
      bool isDownApproximate() const { return queryApproximationField() == ADownApproximate; }
      bool isUpApproximate() const { return queryApproximationField() == AUpApproximate; }
      void setDownApproximate() { setApproximationField(ADownApproximate); }
      void setUpApproximate() { setApproximationField(AUpApproximate); }
      void clearApproximate() { clearApproximationField(); }
      enum Direction { Down, Up };
      void setApproximate(Direction direction)
         {  setApproximationField((direction == Down) ? ADownApproximate : AUpApproximate); }
      bool isApproximate(Direction direction) const
         {  return queryApproximationField() == ((direction == Down) ? ADownApproximate : AUpApproximate); }
      bool hasSameApproximation(const ReadParameters& source) const
         {  return queryApproximationField() == source.queryApproximationField(); }
      bool hasIncrementFraction(bool isNegative) const
         {  return isNegative ? isDownApproximate() : isUpApproximate(); }

      void setEffectiveRoundToEven() { mergeEffectiveRoundToEvenField(1); }
      void clearEffectiveRoundToEven() { clearEffectiveRoundToEvenField(); }
      bool hasEffectiveRoundToEven() { return hasEffectiveRoundToEvenField(); }

      void setPartialRead() { setReadResultField(RRPartial); }
      void setTotalRead() { setReadResultField(RRTotal); }
      bool isPartialRead() const { return queryReadResultField() == RRPartial; }
      bool isTotalRead() const { return queryReadResultField() == RRTotal; }
      bool hasPartialRead() const { return hasReadResultField(); }

      void setSNaNOperand() { mergeSNaNOperandField(1); }
      bool hasSNaNOperand() const { return hasSNaNOperandField(); }
      
      void setInftyMinusInfty() { AssumeCondition(!hasQNaNResultField()) mergeQNaNResultField(QNNRInftyMinusInfty); }
      void setInftyOnInfty() { AssumeCondition(!hasQNaNResultField()) mergeQNaNResultField(QNNRInftyOnInfty); }
      void setZeroOnZero() { AssumeCondition(!hasQNaNResultField()) mergeQNaNResultField(QNNRZeroOnZero); }
      void setInftyMultZero() { AssumeCondition(!hasQNaNResultField()) mergeQNaNResultField(QNNRInftyMultZero); }
      bool hasQNaNResult() const { return hasQNaNResultField(); }
      bool isInftyMinusInfty() const { return queryQNaNResultField() == QNNRInftyMinusInfty; }
      bool isInftyOnInfty() const { return queryQNaNResultField() == QNNRInftyOnInfty; }
      bool isZeroOnZero() const { return queryQNaNResultField() == QNNRZeroOnZero; }
      bool isInftyMultZero() const { return queryQNaNResultField() == QNNRInftyMultZero; }

      void clear() { clearDynamicWriteField(); }
      
      bool isDivisionByZero() const { return hasDivisionByZeroField(); }
      void setDivisionByZero() { mergeDivisionByZeroField(1); }
      bool hasFlowException() const { return hasFlowExceptionField(); }
      void clearFlowException() { clearFlowExceptionField(); }
      void setOverflow() { mergeFlowExceptionField(FEOverflow); }
      void setUnderflow() { mergeFlowExceptionField(FEUnderflow); }
      bool isOverflow() const { return queryFlowExceptionField() & FEOverflow; }
      bool isUnderflow() const { return queryFlowExceptionField() & FEUnderflow; }
      void clearUnderflow() { clearFlowExceptionField(); }
   };

   class WriteParameters : public STG::IOObject::FormatParameters {
     private:
      typedef STG::IOObject::FormatParameters inherited;
      enum Type { TDecimal, TBinary };
      
     protected:
      DefineExtendedParameters(1, STG::IOObject::FormatParameters)
         
     public:
      WriteParameters() : inherited() {}
      WriteParameters(const WriteParameters& params) = default;

      WriteParameters& setDecimal() { setOwnField(TDecimal); return *this; }
      WriteParameters& setBinary() { setOwnField(TBinary); return *this; }
      WriteParameters& setNative() { return (WriteParameters&) inherited::setNative(); }

      bool isDecimal() const { return queryOwnField() == TDecimal; }
      bool isBinary() const { return queryOwnField() == TBinary; }
   };
#else // !StandardClassesHPP
   class ReadParameters {
     public:
      enum RoundMode { RMNearest, RMLowest, RMHighest, RMZero };
      enum Approximation { AExact, ADownApproximate, AUpApproximate };
      enum ReadResult { RRTotal, RRPartial };
      enum Context { CMinDown, CMaxDown, CMinUp, CMaxUp };

     public:
      unsigned getStaticMachine() const
         {  return fAvoidInfty
               | (fKeepNaNSign << 1)
               | (fProduceDivNaNPositive << 2)
               | (fRoundToEven << 3)
               | (fPositiveZeroMAdd << 4)
               | (fUpApproximateInfty << 5)
               | (fUpApproximateInversionForNear << 6)
               | (fChooseNaNAddBeforeMult << 7)
               | (fConvertNaNNegative << 8)
               | (fRefuseMinusZero << 9)
               | (cContext << 10)
               | (fInverseContext << 12);
         }
     protected:
      bool fAvoidInfty : 1;
      bool fKeepNaNSign : 1;
      bool fProduceDivNaNPositive : 1;
      bool fRoundToEven : 1;
      bool fPositiveZeroMAdd : 1;
      bool fUpApproximateInfty : 1;
      bool fUpApproximateInversionForNear : 1;
      bool fChooseNaNAddBeforeMult : 1;
      bool fConvertNaNNegative : 1;
      bool fRefuseMinusZero : 1;
      Context cContext : 2;
      bool fInverseContext : 1;

     public:
      unsigned getStaticMode() const
         {  return rmRoundMode | (fKeepSignalingConversion << 2); }

     protected:
      RoundMode rmRoundMode : 2;
      bool fKeepSignalingConversion : 1;

     public:
      unsigned getDynamicWrite() const
         {  return aApproximation
               | (rrReadResult << 2)
               | (fEffectiveRoundToEven << 3)
               | (fSNaNOperand << 4)
               | (qrQNaNResult << 5)
               | (feFlowException << 8)
               | (fDivisionByZero << 10);
         }
      void setDynamicWrite(unsigned code)
         {  aApproximation = (Approximation) (code & 0x3);
            rrReadResult = (ReadResult) ((code >> 2) & 0x1);
            fEffectiveRoundToEven = (bool) ((code >> 3) & 0x1);
            fSNaNOperand = (bool) ((code >> 4) & 0x1);
            qrQNaNResult = (QNaNResult) ((code >> 5) & 0x7);
            feFlowException = (FlowException) ((code >> 8) & 0x3);
            fDivisionByZero = (bool) ((code >> 10) & 0x1);
         }

     protected:
      enum FlowException { FENoException, FEOverflow, FEUnderflow, FEEnd };
      enum QNaNResult
         {  QNNRUndefined, QNNRInftyMinusInfty, QNNRInftyOnInfty, QNNRZeroOnZero,
            QNNRInftyMultZero
         };
      
      Approximation aApproximation : 2;
      ReadResult rrReadResult : 1;
      bool fEffectiveRoundToEven : 1;
      bool fSNaNOperand : 1;
      QNaNResult qrQNaNResult : 3;
      FlowException feFlowException : 2;
      bool fDivisionByZero : 1;

     public:
      ReadParameters() { memset(this, 0, sizeof(ReadParameters)); }
      ReadParameters(const ReadParameters& source) = default;
      ReadParameters& operator=(const ReadParameters& source)
         { setDynamicWrite(source.getDynamicWrite()); return *this; }

      ReadParameters& setNative() { return (ReadParameters&) inherited::setNative(); }

      // Static parameters
      ReadParameters& setRoundToEven()
         {  rmRoundMode = RMNearest; fRoundToEven = true; return *this; }
      ReadParameters& clearRoundToEven() {  fRoundToEven = false; return *this; }
      ReadParameters& setPositiveZeroMAdd() { fPositiveZeroMAdd = true; return *this; }
      ReadParameters& avoidInfty() { fAvoidInfty = true; return *this; }
      ReadParameters& clearAvoidInfty() { fAvoidInfty = false; return *this; }
      ReadParameters& setKeepNaNSign() { fKeepNaNSign = true; return *this; }
      ReadParameters& setProduceDivNaNPositive() { fProduceDivNaNPositive = true; return *this; }
      ReadParameters& setUpApproximateInfty() { fUpApproximateInfty = true; return *this; }
      ReadParameters& setUpApproximateInversionForNear() { fUpApproximateInversionForNear = true; return *this; }
      ReadParameters& setChooseNaNAddBeforeMult() { fChooseNaNAddBeforeMult = true; return *this; }
      ReadParameters& setConvertNaNNegative() { fConvertNaNNegative = true; return *this; }
      ReadParameters& setAcceptMinusZero() { fRefuseMinusZero = false; return *this; }
      ReadParameters& setRefuseMinusZero() { fRefuseMinusZero = true; return *this; }

      ReadParameters& setMinDown() { cContext = CMinDown; return *this; }
      ReadParameters& setMinUp() { cContext = CMinUp; return *this; }
      ReadParameters& setMaxDown() { cContext = CMaxDown; return *this; }
      ReadParameters& setMaxUp() { cContext = CMaxUp; return *this; }
      ReadParameters& setInverseContext() { fInverseContext = true; return *this; }
      ReadParameters& clearInverseContext() { fInverseContext = false; return *this; }

      bool isContextUp() const { return cContext & CMinUp; }
      bool isContextMax() const { return cContext & CMaxDown; }
      bool doesInverseContext() const { return fInverseContext; }

      bool doesSupportInstantiation() const
         {  return getStaticMachine() == 0 && getStaticMode() == 0 && getDynamicWrite() == 0; }
      bool isRoundToEven() const { return fRoundToEven && isNearestRound(); }
      bool isPositiveZeroMAdd() { return fPositiveZeroMAdd; }
      bool isInftyAvoided() const { return fAvoidInfty; }
      bool doesAvoidInfty(bool isNegative) const
         {  AssumeCondition(fAvoidInfty)
            return isNegative ? (rmRoundMode >= RMHighest) : (rmRoundMode & RMLowest);
         }
      bool keepNaNSign() const { return fKeepNaNSign; }
      bool produceDivNaNPositive() const { return fProduceDivNaNPositive; }
      bool upApproximateInfty() const { return fUpApproximateInfty; }
      bool upApproximateInversionForNear() const { return fUpApproximateInversionForNear; }
      bool chooseNaNAddBeforeMult() const { return fChooseNaNAddBeforeMult; }
      bool isConvertNaNNegative() const { return fConvertNaNNegative; }
      bool acceptMinusZero() const { return !fRefuseMinusZero; }

      // dynamic read parameters
      ReadParameters& setRoundMode(RoundMode mode) { rmRoundMode = mode; return *this; }
      ReadParameters& setNearestRound()   { rmRoundMode = RMNearest; return *this; }
      ReadParameters& setHighestRound()   { rmRoundMode = RMHighest; return *this; }
      ReadParameters& setLowestRound()    { rmRoundMode = RMLowest; return *this; }
      ReadParameters& setZeroRound()      { rmRoundMode = RMZero; return *this; }

      bool isLowestRound() const { return rmRoundMode == RMLowest; }
      bool isNearestRound() const { return rmRoundMode == RMNearest; }
      bool isHighestRound() const { return rmRoundMode == RMHighest; }
      bool isZeroRound() const { return rmRoundMode == RMZero; }

      ReadParameters& setKeepSignalingConversion() { fKeepSignalingConversion = true; return *this; }
      ReadParameters& clearKeepSignalingConversion() { fKeepSignalingConversion = false; return *this; }
      bool keepSignalingConversion() const { return fKeepSignalingConversion; }

      // dynamic write parameters
      bool isApproximate() const { return aApproximation != AExact; }
      bool isDownApproximate() const { return aApproximation == ADownApproximate; }
      bool isUpApproximate() const { return aApproximation == AUpApproximate; }
      void setDownApproximate() { aApproximation = ADownApproximate; }
      void setUpApproximate() { aApproximation = AUpApproximate; }
      void clearApproximate() { aApproximation = AExact; }

      enum Direction { Down, Up };
      void setApproximate(Direction direction)
         {  aApproximation = (direction == Down) ? ADownApproximate : AUpApproximate; }
      bool isApproximate(Direction direction) const
         {  return aApproximation == ((direction == Down) ? ADownApproximate : AUpApproximate); }
      bool hasSameApproximation(const ReadParameters& source) const
         {  return aApproximation == source.aApproximation; }
      bool hasIncrementFraction(bool isNegative) const
         {  return isNegative ? isDownApproximate() : isUpApproximate(); }

      void setEffectiveRoundToEven() { fEffectiveRoundToEven = true; }
      void clearEffectiveRoundToEven() { fEffectiveRoundToEven = false; }
      bool hasEffectiveRoundToEven() { return fEffectiveRoundToEven; }

      void setPartialRead() { rrReadResult = RRPartial; }
      void setTotalRead() { rrReadResult = RRTotal; }
      bool isPartialRead() const { return rrReadResult == RRPartial; }
      bool isTotalRead() const { return rrReadResult == RRTotal; }
      bool hasPartialRead() const { return rrReadResult == RRPartial; }

      void setSNaNOperand() { fSNaNOperand = true; }
      bool hasSNaNOperand() const { return fSNaNOperand; }
      
      void setInftyMinusInfty() { AssumeCondition(qrQNaNResult == QNNRUndefined) qrQNaNResult = QNNRInftyMinusInfty; }
      void setInftyOnInfty() { AssumeCondition(qrQNaNResult == QNNRUndefined) qrQNaNResult = QNNRInftyOnInfty; }
      void setZeroOnZero() { AssumeCondition(qrQNaNResult == QNNRUndefined) qrQNaNResult = QNNRZeroOnZero; }
      void setInftyMultZero() { AssumeCondition(qrQNaNResult == QNNRUndefined) qrQNaNResult = QNNRInftyMultZero; }
      bool hasQNaNResult() const { return qrQNaNResult; }
      bool isInftyMinusInfty() const { return qrQNaNResult == QNNRInftyMinusInfty; }
      bool isInftyOnInfty() const { return qrQNaNResult == QNNRInftyOnInfty; }
      bool isZeroOnZero() const { return qrQNaNResult == QNNRZeroOnZero; }
      bool isInftyMultZero() const { return qrQNaNResult == QNNRInftyMultZero; }

      void clear() { setDynamicWrite(0); }
      
      bool isDivisionByZero() const { return fDivisionByZero; }
      void setDivisionByZero() { fDivisionByZero = true; }
      bool hasFlowException() const { return feFlowException != FENoException; }
      void clearFlowException() { feFlowException = FENoException; }
      void setOverflow() { feFlowException = FEOverflow; }
      void setUnderflow() { feFlowException = FEUnderflow; }
      bool isOverflow() const { return feFlowException & FEOverflow; }
      bool isUnderflow() const { return feFlowException & FEUnderflow; }
      void clearUnderflow() { feFlowException = FENoException; }
   };

   class WriteParameters {
     private:
      enum Type { TDecimal, TBinary };
      
     protected:
      Type tType : 1;
         
     public:
      WriteParameters() = default;
      WriteParameters(const WriteParameters& params) = default;

      WriteParameters& setDecimal() { tType = TDecimal; return *this; }
      WriteParameters& setBinary() { tType = TBinary; return *this; }

      bool isDecimal() const { return tType == TDecimal; }
      bool isBinary() const { return tType == TBinary; }
   };
#endif // StandardClassesHPP

   class Carry {
     private:
      bool fCarry;

     public:
      Carry() : fCarry(false) {}
      Carry(const Carry& source) = default;
      bool& carry() { return fCarry; }
      const bool& carry() const { return fCarry; }
      bool hasCarry() const { return fCarry; }
   };
   
   static bool isBigEndian()
      {
#if defined(__GNUC__) && !defined(__MINGW32__)
#if BYTE_ORDER == BIG_ENDIAN
         return true;
#else
         return false;
#endif
#else
         int dummy = 0x1234;
         unsigned char cdummy[4];
         memcpy((unsigned char*) cdummy, &dummy, 4);
         return *cdummy == 0x12;
#endif
      }
};

}

/****************************************/
/* Definition of the class DoubleErrors */
/****************************************/

class DoubleErrors {
  private:
   unsigned int uErrors;
   enum Type { TPositiveOverflow, TNegativeOverflow, TQNaN, TSNaN, TPositiveUnderflow, TNegativeUnderflow };

  public:
   DoubleErrors() : uErrors(0) {}
   DoubleErrors(const DoubleErrors& source) = default;
   DoubleErrors& operator=(const DoubleErrors& source) = default;

#define DefineIsError(TypeError) \
   bool is##TypeError() const { return (bool) (uErrors & (1U << T##TypeError)); }
   
   DefineIsError(PositiveOverflow)
   DefineIsError(NegativeOverflow)
   DefineIsError(QNaN)
   DefineIsError(SNaN)
   DefineIsError(PositiveUnderflow)
   DefineIsError(NegativeUnderflow)
#undef DefineIsError
   bool isNaN() const { return (bool) (uErrors & (3U << TQNaN)); }

#define DefineSetError(TypeError) \
   DoubleErrors& set##TypeError() { uErrors |= (1U << T##TypeError); return *this; }
   
   DefineSetError(PositiveOverflow)
   DefineSetError(NegativeOverflow)
   DefineSetError(QNaN)
   DefineSetError(SNaN)
   DefineSetError(PositiveUnderflow)
   DefineSetError(NegativeUnderflow)
#undef DefineSetError
};

} // end of namespace Numerics::DDouble

#endif // Numerics_BaseFloatingParametersH

