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
// File      : StandardMessage.h
// Description :
//   Definition of the interface for the error messages.
//

#ifndef StandardMessageH
#define StandardMessageH

#define DefineSTD_InvalidPrecondition "Invalid precondition"
#define DefineSTD_InvalidObject "Invalid object"
#define DefineSTD_InvalidParameter "Invalid parameter"
#define DefineSTD_NotImplemented "Unimplemented method"
#define DefineSTD_UserError "User error"
#define DefineSTD_PreconditionError "Precondition error"
#define DefineSTD_File "file"
#define DefineSTD_Line "line"

class EUserError {
  public:
   EUserError() {}
   virtual ~EUserError() {}
   virtual std::ostream& print(std::ostream& out) const
      {  return out << DefineSTD_UserError << std::endl; }
};

class ESPreconditionError : public EUserError {
  private:
   int uLine;
   const char* szFile;
   const char* szText;

  protected:
   const int& queryLine() const { return uLine; }
   const char* queryFile() const { return szFile; }
   const char* queryText() const { return szText; }

  public:
   ESPreconditionError(const char *text)
      :  uLine(0), szFile(nullptr), szText(text) { szFile = ""; }
   ESPreconditionError(const char *text, int line, const char* file)
#if DefineDebugLevel <= 1
      :  uLine(line), szFile(file), szText(text) {}
#else
      ;
#endif
   virtual std::ostream& print(std::ostream& out) const
      {  return out << DefineSTD_PreconditionError << ' ' << szText << '\n' << DefineSTD_File << ' '
            << szFile << ", " << DefineSTD_Line << " " << uLine << std::endl;
      }
};

class ENotImplemented : public EUserError {
  public:
   ENotImplemented() {}
   virtual std::ostream& print(std::ostream& out) const
      {  return out << DefineSTD_NotImplemented << ' ' << std::endl; }
};

inline std::ostream& operator<<(std::ostream& osOut, const EUserError& epeError)
   {  return epeError.print(osOut); }

class EMemoryExhausted {};

#endif // StandardMessageH
