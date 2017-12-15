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
// Library   : Standard classes
// Unit      : Basic object
// File      : Persistence.h
// Description :
//   Definition of basic material for persistence. Each object that needs serialization
//   is likely to inherits for STG::IOObject.
//

#ifndef STG_PersistenceH
#define STG_PersistenceH

#include "StandardClasses/StandardClasses.hpp"
#include <cstring>

namespace STG {

/***************************************************/
/* Definition of exceptions related to persistence */
/***************************************************/

class EReadError {
  private:
   int uPosition;
   const char* szMessage;

  public:
#if DefineDebugLevel > 1
   EReadError(const char* message = nullptr);
   EReadError(int position, const char* message = nullptr);
#else
   EReadError(const char* message = nullptr) : uPosition(0), szMessage(message) {}
   EReadError(int position, const char* message = nullptr) : uPosition(position), szMessage(message) {}
#endif
   EReadError(const EReadError& source) = default;
   EReadError& operator=(const EReadError& source) = default;

   bool isValid() const { return uPosition >= 0; }
   const int& getPosition() const { return uPosition; }
   const char* getMessage() const { return szMessage; }
};

class EWriteError {
  private:
   const char* szMessage;
   
  public:
#if DefineDebugLevel > 1
   EWriteError(const char* message = nullptr);
#else
   EWriteError(const char* message = nullptr) : szMessage(message) {}
#endif
   EWriteError(const EWriteError& source) = default;
   EWriteError& operator=(const EWriteError& source) = default;
   bool isValid() const { return szMessage != nullptr; }
   const char* getMessage() const { return szMessage; }
};

template <typename TypeChar>
struct TChunk {
  private:
   typedef TChunk<TypeChar> thisType;

  public:
   TypeChar* string;
   size_t length;

   TChunk() : string(nullptr), length(0) {}
   TChunk(const TChunk<TypeChar>& source) = default;
   thisType& setLocalLength(size_t localLength) { length = localLength; return *this; }
   thisType& setString(TypeChar* content) { string = content; return *this; }
};

/* Definition of persistence classes */

class VirtualStringProperty {
  public:
   VirtualStringProperty() {}

   virtual void copy(void* chunk) const = 0;
   virtual void cat(void* chunk) const = 0;
   virtual ComparisonResult compareBound(void* chunk, int& chars, bool isCaseInsensitive) const = 0;
   virtual int length() const = 0;
   virtual void* getChunk() const = 0;
   virtual bool setToFirstChunk() const = 0;
   virtual bool setToNextChunk() const = 0;
   virtual void read(void* std_in, int maxChars) const = 0;
   virtual void readall(void* std_in) const = 0;
   virtual void write(void* std_out) const = 0;
};

class StringProperty {
  public:
   const VirtualStringProperty& property;
   bool isAll;

   StringProperty(const VirtualStringProperty& aproperty, bool all)
      :  property(aproperty), isAll(all) {}
   StringProperty(const StringProperty& source) = default;
};

class AllStringProperty : public StringProperty {
  public:
   AllStringProperty(const VirtualStringProperty& aproperty) : StringProperty(aproperty, true) {}
   AllStringProperty(const AllStringProperty& source) = default;
};

class SubStringProperty : public StringProperty {
  public:
   SubStringProperty(const VirtualStringProperty& aproperty) : StringProperty(aproperty, false) {}
   SubStringProperty(const SubStringProperty& source) = default;
};
   
class IOObject : public EnhancedObject {
  public:
   class FormatParameters : public ExtendedParameters {
     public:
      enum TextualFormat { TFText, TFRaw, TFJSon, TFXML, TFYAML, TFNative };

     protected:
      DefineExtendedParameters(4, ExtendedParameters)
      DefineSubExtendedParameters(Format, 3, INHERITED)
      DefineSubExtendedParameters(Wide, 1, Format)

     public:
      FormatParameters() {}
      FormatParameters(const FormatParameters& source) : ExtendedParameters(source) {}

      FormatParameters& assignPart(const FormatParameters& source)
         {  setOwnField(source.queryOwnField()); return *this; }
      FormatParameters& setRaw() { AssumeCondition(!hasFormatField()) mergeFormatField(TFRaw); return *this; }
      FormatParameters& setText() { AssumeCondition(!hasFormatField()) return *this; }
      FormatParameters& setJSon() { AssumeCondition(!hasFormatField()) mergeFormatField(TFJSon); return *this; }
      FormatParameters& setXML() { AssumeCondition(!hasFormatField()) mergeFormatField(TFXML); return *this; }
      FormatParameters& setYAML() { AssumeCondition(!hasFormatField()) mergeFormatField(TFYAML); return *this; }
      FormatParameters& setNative() { AssumeCondition(!hasFormatField()) mergeFormatField(TFNative); return *this; }

      bool isText() const { return !hasFormatField(); }
      bool isRaw() const { return queryFormatField() == TFRaw; }
      bool isJSon() const { return queryFormatField() == TFJSon; }
      bool isXML() const { return queryFormatField() == TFXML; }
      bool isYAML() const { return queryFormatField() == TFYAML; }
      bool isNative() const { return queryFormatField() == TFNative; }

      FormatParameters& setWide() { mergeWideField(1); return *this; }
      FormatParameters& clearWide() { clearWideField(); return *this; }
      bool isWide() const { return hasWideField(); }
   };

#ifndef EOF
#define HasDefinedEOF
#define EOF -1
#endif
   class ISBase {
     public:
      virtual ~ISBase() {}
      ISBase& operator>>(bool& b) { return read(b, false); }
      ISBase& operator>>(char& n) { return read(n, false); }
      ISBase& operator>>(int& n) { return read(n, false); }
      ISBase& operator>>(unsigned int& n) { return read(n, false); }
      ISBase& operator>>(long int& n) { return read(n, false); }
      ISBase& operator>>(unsigned long int& n) { return read(n, false); }
      ISBase& operator>>(double& f) { return read(f, false); }
      ISBase& operator>>(const StringProperty& property)
         {  return property.isAll ? readall(property.property) : read(property.property, false); }

      virtual bool isStreamBased() const { return false; }
      virtual bool isStringBased() const { return false; }
      virtual int gcount() { return 0; }
      virtual int get() { return EOF; }
      virtual ISBase& get(char& ch) { return *this; }
      virtual int peek() { return EOF; }
      virtual int readsome(const STG::VirtualStringProperty& source, int n) { return 0; }
      virtual ISBase& unget() { return *this; }
      virtual ISBase& ignorespaces() { return *this; }
      virtual bool canread() { return true; }
      virtual bool good() { return true; }
      virtual int tellg() { return 0; }
      virtual ISBase& seekg(int pos) { return *this; }
      virtual ISBase& seekg(int pos, int origin) { return *this; }

      virtual ISBase& assume(char ch) { return *this; }
      virtual ISBase& assume(void* chunk) { return *this; }
      ISBase& assume(const char* text)
         {  TChunk<char> chunk;
            chunk.string = const_cast<char*>(text);
            chunk.length = (int) strlen(text);
            assume((void*) &chunk);
            return *this;
         }
      virtual ISBase& assume(const VirtualStringProperty& text) { return *this; }
      virtual ISBase& read(bool& b, bool isRaw) { return *this; }
      virtual ISBase& read(char& ch, bool isRaw) { return *this; }
      virtual ISBase& read(int& n, bool isRaw) { return *this; }
      virtual ISBase& readHexa(int& n) { return *this; }
      virtual ISBase& read(unsigned int& n, bool isRaw) { return *this; }
      virtual ISBase& readHexa(unsigned int& n) { return *this; }
      virtual ISBase& read(long int& n, bool isRaw) { return *this; }
      virtual ISBase& readHexa(long int& n) { return *this; }
      virtual ISBase& read(unsigned long int& n, bool isRaw) { return *this; }
      virtual ISBase& readHexa(unsigned long int& n) { return *this; }
      virtual ISBase& read(double& f, bool isRaw) { return *this; }
      virtual ISBase& readchunk(void* chunk) { return *this; }
      int readsome(char* buffer, int n)
         {  TChunk<char> chunk;
            chunk.string = buffer;
            chunk.length = n;
            readchunk((void*) &chunk);
            return chunk.length;
         }
      virtual ISBase& read(const VirtualStringProperty& source, bool isRaw) { return *this; }
      virtual ISBase& read(const VirtualStringProperty& source, int n, bool isRaw) { return *this; }
      virtual ISBase& readall(const VirtualStringProperty& source) { return *this; }
      ISBase& readint(char& chState, bool isRaw)
         {  if (isRaw)
               read(chState, isRaw);
            else {
               int state = 0;
               read(state, isRaw);
               chState = state;
            };
            return *this;
         }
   };
#ifdef HasDefinedEOF
#undef HasDefinedEOF
#undef EOF
#endif

   class OSBase {
     public:
      virtual ~OSBase() {}
      
      virtual bool isStreamBased() const { return false; }
      virtual bool isStringBased() const { return false; }
      
      OSBase& operator<<(bool b) { return write(b, false); }
      OSBase& operator<<(char c) { return write(c, false); }
      OSBase& operator<<(int n) { return write(n, false); }
      OSBase& operator<<(unsigned int n) { return write(n, false); }
      OSBase& operator<<(long int n) { return write(n, false); }
      OSBase& operator<<(unsigned long int n) { return write(n, false); }
      OSBase& operator<<(double f) { return write(f, false); }
      OSBase& operator<<(const char* source)
         {  TChunk<char> chunk;
            chunk.string = const_cast<char*>(source);
            chunk.length = (int) strlen(source);
            return writechunk((void*) &chunk);
         }
      OSBase& operator<<(const StringProperty& property)
         {  return property.isAll ? writeall(property.property) : write(property.property, false); }

      virtual OSBase& put(char c) { return *this; }
      virtual int tellp() { return 0; }
      virtual OSBase& seekp(int pos) { return *this; }
      virtual OSBase& seekp(int pos, int uOrigin) { return *this; }
      virtual void assumealloc(int incsize) {}

      virtual OSBase& write(char ch, bool isRaw) { return *this; }
      virtual OSBase& write(int n, bool isRaw) { return *this; }
      virtual OSBase& writeHexa(int n) { return *this; }
      virtual OSBase& write(unsigned int n, bool isRaw) { return *this; }
      virtual OSBase& writeHexa(unsigned int n) { return *this; }
      virtual OSBase& write(long int n, bool isRaw) { return *this; }
      virtual OSBase& writeHexa(long int n) { return *this; }
      virtual OSBase& write(unsigned long int n, bool isRaw) { return *this; }
      virtual OSBase& writeHexa(unsigned long int n) { return *this; }
      virtual OSBase& write(double f, bool isRaw) { return *this; }
      virtual OSBase& write(bool b, bool isRaw) { return *this; }
      virtual OSBase& writechunk(void* chunk) { return *this; }
      OSBase& writesome(const char* source)
         {  TChunk<char> chunk;
            chunk.string = const_cast<char*>(source);
            chunk.length = (int) strlen(source);
            return writechunk((void*) &chunk);
         }
      virtual OSBase& write(const VirtualStringProperty& source, bool isRaw) { return *this; }
      virtual OSBase& writeall(const VirtualStringProperty& source) { return *this; }
      virtual OSBase& flush() { return *this; } 
      OSBase& writeint(char state, bool isRaw=false)
         {  return isRaw ? write(state, isRaw) : write((int) state, isRaw); }
   };

   class ISStream : public ISBase {
     private:
      std::istream* pisIn;

     public:
      ISStream(std::istream& source) : pisIn(&source) {}
      ISStream(const ISStream& source) = default;

      bool isValid() const { return (bool) pisIn; }
      virtual bool isStreamBased() const { return true; }
      #define DefineDeclareIMethods
      #include "StandardClasses/IOStreamMethods.inch"
      #undef DefineDeclareIMethods
   };

   class OSStream : public OSBase {
     private:
      std::ostream* posOut;

     public:
      OSStream(std::ostream& source) : posOut(&source) {}
      OSStream(const OSStream& source) = default;

      bool isValid() const { return (bool) posOut; }
      virtual bool isStreamBased() const { return true; }
      #define DefineDeclareOMethods
      #include "StandardClasses/IOStreamMethods.inch"
      #undef DefineDeclareOMethods
   };

  protected:
   virtual void _read(ISBase& in, const FormatParameters& params) {}
   virtual void _write(OSBase& out, const FormatParameters& params) const {}

  public:
   IOObject() {}
   IOObject(const IOObject& source) : EnhancedObject(source) {}
   DefineCopy(IOObject)

   void read(ISBase& in, const FormatParameters& params) { _read(in, params); }
   void write(OSBase& out, const FormatParameters& params) const { _write(out, params); }
   void read(std::istream& in, const FormatParameters& params) { ISStream instream(in); _read(instream, params); }
   void read(std::istream& in) { ISStream instream(in); _read(instream, FormatParameters()); }
   void write(std::ostream& out, const FormatParameters& params) const { OSStream outstream(out); _write(outstream, params); }
   void write(std::ostream& out) const { OSStream outstream(out); _write(outstream, FormatParameters()); }
};

#define DefineInlineIStreamMethods

#define DefineTypeObject IOObject::ISStream
#define DefineLinkStream (*pisIn)
#include "StandardClasses/IOStreamMethods.inch"
#undef DefineLinkStream
#undef DefineTypeObject

#undef DefineInlineIStreamMethods

#define DefineInlineOStreamMethods

#define DefineTypeObject IOObject::OSStream
#define DefineLinkStream (*posOut)
#include "StandardClasses/IOStreamMethods.inch"
#undef DefineLinkStream
#undef DefineTypeObject

#undef DefineInlineOStreamMethods

namespace Lexer {

class Base {
  public:
   typedef EnhancedObject RuleResult;
   enum ReadResult
      {  RRNeedChars, RRContinue, RRHasToken, RRFinished };
   enum WriteResult
      {  WRNeedEvent, WRNeedPlace, WRNeedWrite, WRFinished };
};

}} // end of namespace STG::Lexer

#endif // STG_PersistenceH
