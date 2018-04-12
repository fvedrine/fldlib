#if !defined(FLOAT_HEADER_INIT) && !defined(FLOAT_HEADER_END)

#ifndef FLOAT_DIAGNOSIS_H
#define FLOAT_DIAGNOSIS_H

#ifdef FLOAT_DIAGNOSIS
#define FEXACT(x) x
#define DEXACT(x) x
#ifdef FLOAT_AFFINE
#ifndef FLOAT_INTERFACE
#include "FloatInstrumentation/FloatAffine.h"
#define FLOAT_READ_ERROR STG::EReadError
#define FLOAT_PRECONDITION_ERROR ESPreconditionError
#else
#include "FloatInstrumentation/FloatAffineInterface.h"
#define FLOAT_READ_ERROR NumericalDomains::DAffineInterface::ExecutionPath::read_error
#define FLOAT_PRECONDITION_ERROR NumericalDomains::DAffineInterface::ExecutionPath::precondition_error
#define DAffine DAffineInterface
#endif // FLOAT_INTERFACE
typedef float old_float;
typedef double old_double;
typedef long double old_long_double;

#define float_fld NumericalDomains::FloatZonotope
#define double_fld NumericalDomains::DoubleZonotope
#define long_double_fld NumericalDomains::LongDoubleZonotope

#ifndef FLOAT_KEEP_DOUBLE
#define float NumericalDomains::FloatZonotope
#define double NumericalDomains::DoubleZonotope
#define long_double_fld NumericalDomains::LongDoubleZonotope
#endif

#define FBETWEEN(x,y) NumericalDomains::FloatZonotope((old_float) x, (old_float) y)
#define FBETWEEN_WITH_ERROR(x,y,errmin,errmax) NumericalDomains::FloatZonotope((old_float) x, (old_float) y, (old_float) errmin, (old_float) errmax)
#define DBETWEEN(x,y) NumericalDomains::DoubleZonotope((old_double) x, (old_double) y)
#define DBETWEEN_WITH_ERROR(x,y,errmin,errmax) NumericalDomains::DoubleZonotope((old_double) x, (old_double) y, (old_double) errmin, (old_double) errmax)
#define FPRINT(x) (x).lightPersist(#x":\t")
#define DPRINT(x) (x).lightPersist(#x":\t")
#define FAFFPRINT(x) (x).persist(#x":\t")
#define DAFFPRINT(x) (x).persist(#x":\t")
#define IPRINT(x) NumericalDomains::DoubleZonotope(x).lightPersist(#x":\t")
#define FCPRINT(name, x) (x).lightPersist((std::string(name) + ":\t").c_str())
#define DCPRINT(name, x) (x).lightPersist((std::string(name) + ":\t").c_str())
#define FCAFFPRINT(name, x) (x).persist((std::string(name) + ":\t").c_str())
#define DCAFFPRINT(name, x) (x).persist((std::string(name) + ":\t").c_str())
#define ICPRINT(name, x) NumericalDomains::DoubleZonotope(x).lightPersist(name":\t")
#define FSENSITIVITY(x)
#define DSENSITIVITY(x)
#define _FDL NumericalDomains::DAffine::BaseFloatAffine::setSourceLine(__FILE__, __LINE__);

#ifdef FLOAT_PRINT_PATH
#define FLOAT_PRINT_CURRENT_PATH(ident)                                                          \
   std::cout << "path" #ident ": ";                                                              \
   NumericalDomains::DAffine::ExecutionPath::writeCurrentPath(std::cout);                        \
   std::cout << std::endl;
#else
#define FLOAT_PRINT_CURRENT_PATH(ident) 
#endif

#ifndef FLOAT_LOOP_UNSTABLE

#define FLOAT_SPLIT_ALL(ident, merge, save) {                                                    \
   const char* _sourceFile##ident = __FILE__;                                                    \
   int _sourceLine##ident = __LINE__;                                                            \
   auto _oldSourceInfo##ident = NumericalDomains::DAffine::BaseFloatAffine::querySplitInfo();    \
   NumericalDomains::DAffine::BaseFloatAffine::splitBranches(__FILE__, __LINE__);
#define FLOAT_SPLIT_ALL_LOOP(ident)                                                              \
   _sourceFile##ident = __FILE__;                                                                \
   _sourceLine##ident = __LINE__;                                                                \
   _oldSourceInfo##ident = NumericalDomains::DAffine::BaseFloatAffine::querySplitInfo();         \
   NumericalDomains::DAffine::BaseFloatAffine::splitBranches(__FILE__, __LINE__);
#define FLOAT_SPLIT_ALL_OUTER(ident, merge, save)                                                \
   _sourceFile##ident = __FILE__;                                                                \
   _sourceLine##ident = __LINE__;                                                                \
   _oldSourceInfo##ident = NumericalDomains::DAffine::BaseFloatAffine::querySplitInfo();         \
   NumericalDomains::DAffine::BaseFloatAffine::splitBranches(__FILE__, __LINE__);
#define FLOAT_MERGE_ALL(ident, x, load)                                                          \
   FLOAT_PRINT_CURRENT_PATH(ident)                                                               \
   NumericalDomains::DAffine::MergeBranches(_sourceFile##ident, _sourceLine##ident) << x;        \
   NumericalDomains::DAffine::BaseFloatAffine::splitBranches(_oldSourceInfo##ident.first, _oldSourceInfo##ident.second); }
#define FLOAT_MERGE_ALL_OUTER(ident, x, load)                                                    \
   FLOAT_PRINT_CURRENT_PATH(ident)                                                               \
   NumericalDomains::DAffine::MergeBranches(_sourceFile##ident, _sourceLine##ident) << x;        \
   NumericalDomains::DAffine::BaseFloatAffine::splitBranches(_oldSourceInfo##ident.first, _oldSourceInfo##ident.second);
#else // FLOAT_LOOP_UNSTABLE

#define FLOAT_SPLIT_ALL(ident, merge, save) {                                                    \
   const char* _sourceFile##ident;                                                               \
   int _sourceLine##ident;                                                                       \
   auto* _oldPathExplorer##ident = NumericalDomains::DAffine::ExecutionPath::getCurrentPathExplorer(); \
   bool _oldDoesFollow##ident = NumericalDomains::DAffine::ExecutionPath::doesFollowFlow();      \
   NumericalDomains::DAffine::ExecutionPath::clearFollowFlow();                                  \
   auto* _oldInputTraceFile##ident = NumericalDomains::DAffine::ExecutionPath::inputTraceFile(); \
   const char* _oldSynchronisationFile##ident = NumericalDomains::DAffine::ExecutionPath::synchronisationFile();\
   int _oldSynchronisationLine##ident = NumericalDomains::DAffine::ExecutionPath::synchronisationLine();\
   bool _isCompleteFlow##ident = true;                                                           \
   NumericalDomains::DAffine::PathExplorer _pathExplorer##ident(                                 \
        NumericalDomains::DAffine::ExecutionPath::queryMode(_oldPathExplorer##ident));           \
   NumericalDomains::DAffine::ExecutionPath::setCurrentPathExplorer(&_pathExplorer##ident);      \
   auto _mergeMemory##ident = NumericalDomains::DAffine::MergeMemory() >> merge;                 \
   auto _saveMemory##ident = NumericalDomains::DAffine::SaveMemory() << save;                    \
   auto _oldSourceInfo##ident = NumericalDomains::DAffine::BaseFloatAffine::querySplitInfo();    \
   do {                                                                                          \
      try {                                                                                      \
         _sourceFile##ident = __FILE__;                                                          \
         _sourceLine##ident = __LINE__;                                                          \
         NumericalDomains::DAffine::BaseFloatAffine::splitBranches(__FILE__, __LINE__);

#define FLOAT_SPLIT_ALL_LOOP(ident)                                                              \
   _sourceFile##ident = __FILE__;                                                                \
   _sourceLine##ident = __LINE__;                                                                \
   _oldSourceInfo##ident = NumericalDomains::DAffine::BaseFloatAffine::querySplitInfo();         \
   NumericalDomains::DAffine::BaseFloatAffine::splitBranches(__FILE__, __LINE__);
#define FLOAT_SPLIT_ALL_OUTER(ident, merge, save)                                                \
   auto* _oldPathExplorer##ident = NumericalDomains::DAffine::ExecutionPath::getCurrentPathExplorer(); \
   bool _oldDoesFollow##ident = NumericalDomains::DAffine::ExecutionPath::doesFollowFlow();      \
   NumericalDomains::DAffine::ExecutionPath::clearFollowFlow();                                  \
   auto* _oldInputTraceFile##ident = NumericalDomains::DAffine::ExecutionPath::inputTraceFile(); \
   const char* _oldSynchronisationFile##ident = NumericalDomains::DAffine::ExecutionPath::synchronisationFile();\
   int _oldSynchronisationLine##ident = NumericalDomains::DAffine::ExecutionPath::synchronisationLine();\
   bool _isCompleteFlow##ident = true;                                                           \
   NumericalDomains::DAffine::PathExplorer _pathExplorer##ident(                                 \
        NumericalDomains::DAffine::ExecutionPath::queryMode(_oldPathExplorer##ident));           \
   NumericalDomains::DAffine::ExecutionPath::setCurrentPathExplorer(&_pathExplorer##ident);      \
   auto _mergeMemory##ident = NumericalDomains::DAffine::MergeMemory() >> merge;                 \
   auto _saveMemory##ident = NumericalDomains::DAffine::SaveMemory() << save;                    \
   auto _oldSourceInfo##ident = NumericalDomains::DAffine::BaseFloatAffine::querySplitInfo();    \
   do {                                                                                          \
      try {                                                                                      \
         _sourceFile##ident = __FILE__;                                                          \
         _sourceLine##ident = __LINE__;                                                          \
         NumericalDomains::DAffine::BaseFloatAffine::splitBranches(__FILE__, __LINE__);

#define FLOAT_MERGE_ALL(ident, x, load)                                                          \
         FLOAT_PRINT_CURRENT_PATH(ident)                                                         \
         _isCompleteFlow##ident = NumericalDomains::DAffine::MergeBranches(_sourceFile##ident, _sourceLine##ident) << x;\
      }                                                                                          \
      catch (double_fld::anticipated_termination&) {                                                 \
         _isCompleteFlow##ident = false;                                                         \
         NumericalDomains::DAffine::ExecutionPath::clearSynchronizationBranches();               \
      }                                                                                          \
      catch (FLOAT_READ_ERROR& error) {                                                          \
         if (const char* message = error.getMessage())                                           \
            std::cerr << "error: " << message << std::endl;                                      \
         else                                                                                    \
            std::cerr << "error while reading input file!" << std::endl;                         \
         _isCompleteFlow##ident = false;                                                         \
         NumericalDomains::DAffine::ExecutionPath::clearSynchronizationBranches();               \
      }                                                                                          \
      NumericalDomains::DAffine::ExecutionPath::setFollowFlow();                                 \
   } while ((_mergeMemory##ident.setCurrentComplete(_isCompleteFlow##ident) << x)                \
         && !(_saveMemory##ident.setCurrentResult(_pathExplorer##ident.isFinished(NumericalDomains::DAffine::ExecutionPath::queryMode(_oldPathExplorer##ident))) >> load));\
   NumericalDomains::DAffine::ExecutionPath::setFollowFlow(_oldDoesFollow##ident, _oldInputTraceFile##ident,\
         _oldSynchronisationFile##ident, _oldSynchronisationLine##ident);                        \
   NumericalDomains::DAffine::ExecutionPath::setCurrentPathExplorer(_oldPathExplorer##ident);    \
   NumericalDomains::DAffine::BaseFloatAffine::splitBranches(_oldSourceInfo##ident.first, _oldSourceInfo##ident.second);\
   if (_mergeMemory##ident.isFirst())                                                            \
      NumericalDomains::DAffine::ExecutionPath::throwEmptyBranch(true); }

#define FLOAT_MERGE_ALL_OUTER(ident, x, load)                                                    \
         FLOAT_PRINT_CURRENT_PATH(ident)                                                         \
         _isCompleteFlow##ident = NumericalDomains::DAffine::MergeBranches(_sourceFile##ident, _sourceLine##ident) << x;\
      }                                                                                          \
      catch (double_fld::anticipated_termination&) {                                                 \
         _isCompleteFlow##ident = false;                                                         \
         NumericalDomains::DAffine::ExecutionPath::clearSynchronizationBranches();               \
      }                                                                                          \
      catch (FLOAT_READ_ERROR& error) {                                                          \
         if (const char* message = error.getMessage())                                           \
            std::cerr << "error: " << message << std::endl;                                      \
         else                                                                                    \
            std::cerr << "error while reading input file!" << std::endl;                         \
         _isCompleteFlow##ident = false;                                                         \
         NumericalDomains::DAffine::ExecutionPath::clearSynchronizationBranches();               \
      }                                                                                          \
      NumericalDomains::DAffine::ExecutionPath::setFollowFlow();                                 \
   } while ((_mergeMemory##ident.setCurrentComplete(_isCompleteFlow##ident) << x)                \
         && !(_saveMemory##ident.setCurrentResult(_pathExplorer##ident.isFinished(_oldPathExplorer##ident->mode())) >> load));\
   NumericalDomains::DAffine::ExecutionPath::setFollowFlow(_oldDoesFollow##ident, _oldInputTraceFile##ident,\
         _oldSynchronisationFile##ident, _oldSynchronisationLine##ident);                        \
   NumericalDomains::DAffine::ExecutionPath::setCurrentPathExplorer(_oldPathExplorer##ident);    \
   NumericalDomains::DAffine::BaseFloatAffine::splitBranches(_oldSourceInfo##ident.first, _oldSourceInfo##ident.second);\
   if (_mergeMemory##ident.isFirst())                                                            \
      NumericalDomains::DAffine::ExecutionPath::throwEmptyBranch(true);

#endif // !FLOAT_LOOP_UNSTABLE
#elif defined(FLOAT_INTERVAL) // !FLOAT_AFFINE && FLOAT_INTERVAL
#ifndef FLOAT_INTERFACE
#include "FloatInstrumentation/FloatInterval.h"
#else
#include "FloatInstrumentation/FloatIntervalInterface.h"
#define DDoubleInterval DDoubleIntervalInterface
#endif // FLOAT_INTERFACE

typedef float old_float;
typedef double old_double;
typedef long double old_long_double;

#ifdef FLOAT_THRESHOLD_DETECTION
inline float middle_of_float(float x, float y) { return (x+y)/2.0; }
inline double middle_of_double(double x, double y) { return (x+y)/2.0; }
#define FBETWEEN(x,y) NumericalDomains::FloatInterval(middle_of_float((old_float) x, (old_float) y))
#define FBETWEEN_WITH_ERROR(x,y,errmin,errmax) NumericalDomains::FloatInterval(middle_of_float((old_float) x, (old_float) y))
#define DBETWEEN(x,y) NumericalDomains::DoubleInterval(middle_of_double((old_double) x, (old_double) y))
#define DBETWEEN_WITH_ERROR(x,y,errmin,errmax) NumericalDomains::DoubleInterval(middle_of_double((old_double) x, (old_double) y))
#else
#define FBETWEEN(x,y) NumericalDomains::FloatInterval((old_float) x, (old_float) y)
#define FBETWEEN_WITH_ERROR(x,y,errmin,errmax) NumericalDomains::FloatInterval((old_float) x, (old_float) y, (old_float) errmin, (old_float) errmax)
#define DBETWEEN(x,y) NumericalDomains::DoubleInterval((old_double) x, (old_double) y)
#define DBETWEEN_WITH_ERROR(x,y,errmin,errmax) NumericalDomains::DoubleInterval((old_double) x, (old_double) y, (old_double) errmin, (old_double) errmax)
#endif // FLOAT_THRESHOLD_DETECTION

#define float_fld NumericalDomains::FloatInterval
#define double_fld NumericalDomains::DoubleInterval
#define long_double_fld NumericalDomains::LongDoubleInterval

#ifndef FLOAT_KEEP_DOUBLE
#define float NumericalDomains::FloatInterval
#define double NumericalDomains::DoubleInterval
#define long_double_fld NumericalDomains::LongDoubleInterval
#endif

#define FPRINT(x) (x).persist(#x":\t")
#define DPRINT(x) (x).persist(#x":\t")
#define FAFFPRINT(x) (x).persist(#x":\t")
#define DAFFPRINT(x) (x).persist(#x":\t")
#define IPRINT(x) NumericalDomains::DoubleInterval(x).persist(#x":\t")
#define FCPRINT(name, x) (x).persist((std::string(name) + ":\t").c_str())
#define DCPRINT(name, x) (x).persist((std::string(name) + ":\t").c_str())
#define FCAFFPRINT(name, x) (x).persist((std::string(name) + ":\t").c_str())
#define DCAFFPRINT(name, x) (x).persist((std::string(name) + ":\t").c_str())
#define ICPRINT(name, x) NumericalDomains::DoubleInterval(x).persist(name":\t")
#define FSENSITIVITY(x)
#define DSENSITIVITY(x)
#define _FDL NumericalDomains::DDoubleInterval::ExecutionPath::setSourceLine(__FILE__, __LINE__);

#ifdef FLOAT_PRINT_PATH
#define FLOAT_PRINT_CURRENT_PATH(ident)                                                          \
   std::cout << "path" #ident ": ";                                                              \
   NumericalDomains::DDoubleInterval::ExecutionPath::writeCurrentPath(std::cout);                \
   std::cout << std::endl;
#else
#define FLOAT_PRINT_CURRENT_PATH(ident)
#endif // FLOAT_PRINT_PATH

#ifndef FLOAT_LOOP_UNSTABLE
#define FLOAT_SPLIT_ALL(ident, merge, save) 
#define FLOAT_SPLIT_ALL_LOOP(ident) 
#define FLOAT_SPLIT_ALL_OUTER(ident, merge, save) 
#define FLOAT_MERGE_ALL(ident, x, load) {}
#define FLOAT_MERGE_ALL_OUTER(ident, x, load) {}
#else // FLOAT_LOOP_UNSTABLE
#define FLOAT_SPLIT_ALL(ident, merge, save) {                                                    \
   auto* _oldPathExplorer##ident = NumericalDomains::DDoubleInterval::ExecutionPath::getCurrentPathExplorer(); \
   bool _oldDoesFollow##ident = NumericalDomains::DDoubleInterval::ExecutionPath::doesFollowFlow();\
   NumericalDomains::DDoubleInterval::ExecutionPath::clearFollowFlow();                          \
   auto* _oldInputTraceFile##ident = NumericalDomains::DDoubleInterval::ExecutionPath::inputTraceFile();\
   NumericalDomains::DDoubleInterval::PathExplorer _pathExplorer##ident;                         \
   NumericalDomains::DDoubleInterval::ExecutionPath::setCurrentPathExplorer(&_pathExplorer##ident);\
   auto _mergeMemory##ident = NumericalDomains::DDoubleInterval::MergeMemory() >> merge;         \
   auto _saveMemory##ident = NumericalDomains::DDoubleInterval::SaveMemory() << save;            \
   do {
#define FLOAT_SPLIT_ALL_LOOP(ident)
#define FLOAT_SPLIT_ALL_OUTER(ident, merge, save)                                                \
   NumericalDomains::DDoubleInterval::PathExplorer* _oldPathExplorer##ident = NumericalDomains::DDoubleInterval::ExecutionPath::getCurrentPathExplorer(); \
   bool _oldDoesFollow##ident = NumericalDomains::DDoubleInterval::ExecutionPath::doesFollowFlow();\
   NumericalDomains::DDoubleInterval::ExecutionPath::clearFollowFlow();                          \
   auto* _oldInputTraceFile##ident = NumericalDomains::DDoubleInterval::ExecutionPath::inputTraceFile();\
   NumericalDomains::DDoubleInterval::PathExplorer _pathExplorer##ident;                         \
   NumericalDomains::DDoubleInterval::ExecutionPath::setCurrentPathExplorer(&_pathExplorer##ident);\
   auto _mergeMemory##ident = NumericalDomains::DDoubleInterval::MergeMemory() >> merge;         \
   auto _saveMemory##ident = NumericalDomains::DDoubleInterval::SaveMemory() << save;            \
   do {

#define FLOAT_MERGE_ALL(ident, x, load)                                                          \
      FLOAT_PRINT_CURRENT_PATH(ident)                                                            \
      NumericalDomains::DDoubleInterval::ExecutionPath::setFollowFlow();                         \
   } while ((_mergeMemory##ident << x)                                                           \
         && !(_saveMemory##ident.setCurrentResult(_pathExplorer##ident.isFinished()) >> load));  \
   NumericalDomains::DDoubleInterval::ExecutionPath::setFollowFlow(_oldDoesFollow##ident, _oldInputTraceFile##ident); \
   NumericalDomains::DDoubleInterval::ExecutionPath::setCurrentPathExplorer(_oldPathExplorer##ident); }
#define FLOAT_MERGE_ALL_OUTER(ident, x, load)                                                    \
      FLOAT_PRINT_CURRENT_PATH(ident)                                                            \
      NumericalDomains::DDoubleInterval::ExecutionPath::setFollowFlow();                         \
   } while ((_mergeMemory##ident << x)                                                           \
         && !(_saveMemory##ident.setCurrentResult(_pathExplorer##ident.isFinished()) >> load));  \
   NumericalDomains::DDoubleInterval::ExecutionPath::setFollowFlow(_oldDoesFollow##ident, _oldInputTraceFile##ident); \
   NumericalDomains::DDoubleInterval::ExecutionPath::setCurrentPathExplorer(_oldPathExplorer##ident);
#endif // FLOAT_LOOP_UNSTABLE
#else // !FLOAT_AFFINE && !FLOAT_INTERVAL
#ifndef FLOAT_INTERFACE
#include "FloatInstrumentation/FloatExact.h"
#define FLOAT_READ_ERROR STG::EReadError
#define FLOAT_PRECONDITION_ERROR ESPreconditionError
#else
#include "FloatInstrumentation/FloatExactInterface.h"
#define FLOAT_READ_ERROR NumericalDomains::DDoubleExact::ExecutionPath::read_error
#define FLOAT_PRECONDITION_ERROR NumericalDomains::DDoubleExact::ExecutionPath::precondition_error
#define DDoubleExact DDoubleExactInterface
#endif // FLOAT_INTERFACE

inline float middle_of_float(float x, float y) { return (x+y)/2.0; }
inline double middle_of_double(double x, double y) { return (x+y)/2.0; }

typedef float old_float;
typedef double old_double;
typedef long double old_long_double;

#define float_fld NumericalDomains::FloatExact
#define double_fld NumericalDomains::DoubleExact
#define long_double_fld NumericalDomains::LongDoubleExact

#ifndef FLOAT_KEEP_DOUBLE
#define float NumericalDomains::FloatExact
#define double NumericalDomains::DoubleExact
#define long_double_fld NumericalDomains::LongDoubleExact
#endif

#define FBETWEEN(x,y) NumericalDomains::FloatExact(middle_of_float((old_float) x, (old_float) y))
#define FBETWEEN_WITH_ERROR(x,y,errmin,errmax) NumericalDomains::FloatExact(middle_of_float((old_float) x, (old_float) y), middle_of_float((old_float) errmin, (old_float) errmax), NumericalDomains::FloatExact::ErrorParameter())
#define DBETWEEN(x,y) NumericalDomains::DoubleExact(middle_of_double((old_double) x, (old_double) y))
#define DBETWEEN_WITH_ERROR(x,y,errmin,errmax) NumericalDomains::DoubleExact(middle_of_double((old_double) x, (old_double) y), middle_of_double((old_double) errmin, (old_double) errmax), NumericalDomains::DoubleExact::ErrorParameter())
#define FPRINT(x) (x).persist(#x":\t")
#define DPRINT(x) (x).persist(#x":\t")
#define FAFFPRINT(x) (x).persist(#x":\t")
#define DAFFPRINT(x) (x).persist(#x":\t")
#define IPRINT(x) NumericalDomains::DoubleExact(x).persist(#x":\t")
#define FCPRINT(name, x) (x).persist((std::string(name) + ":\t").c_str())
#define DCPRINT(name, x) (x).persist((std::string(name) + ":\t").c_str())
#define FCAFFPRINT(name, x) (x).persist((std::string(name) + ":\t").c_str())
#define DCAFFPRINT(name, x) (x).persist((std::string(name) + ":\t").c_str())
#define ICPRINT(name, x) NumericalDomains::DoubleExact(x).persist(name":\t")
#define FSENSITIVITY(x)
#define DSENSITIVITY(x)
#define _FDL NumericalDomains::DDoubleExact::BaseFloatExact::setSourceLine(__FILE__, __LINE__);

#ifdef FLOAT_PRINT_PATH
#define FLOAT_PRINT_CURRENT_PATH(ident)                                                          \
   std::cout << "path" #ident ": ";                                                              \
   NumericalDomains::DDoubleExact::ExecutionPath::writeCurrentPath(std::cout);                   \
   std::cout << std::endl;
#else
#define FLOAT_PRINT_CURRENT_PATH(ident)
#endif // FLOAT_PRINT_PATH


#ifndef FLOAT_LOOP_UNSTABLE
#define FLOAT_SPLIT_ALL(ident, merge, save) {                                                    \
   const char* _sourceFile##ident = __FILE__;                                                    \
   int _sourceLine##ident = __LINE__;                                                            \
   auto _oldSourceInfo##ident = NumericalDomains::DDoubleExact::BaseFloatExact::querySplitInfo();\
   NumericalDomains::DDoubleExact::BaseFloatExact::splitBranches(__FILE__, __LINE__);
#define FLOAT_SPLIT_ALL_OUTER(ident, merge, save)                                                \
   _sourceFile##ident = __FILE__;                                                                \
   _sourceLine##ident = __LINE__;                                                                \
   _oldSourceInfo##ident = NumericalDomains::DDoubleExact::BaseFloatExact::querySplitInfo();     \
   NumericalDomains::DDoubleExact::BaseFloatExact::splitBranches(__FILE__, __LINE__);
#define FLOAT_SPLIT_ALL_LOOP(ident)                                                              \
   _sourceFile##ident = __FILE__;                                                                \
   _sourceLine##ident = __LINE__;                                                                \
   _oldSourceInfo##ident = NumericalDomains::DDoubleExact::BaseFloatExact::querySplitInfo();     \
   NumericalDomains::DDoubleExact::BaseFloatExact::splitBranches(__FILE__, __LINE__);
#define FLOAT_MERGE_ALL(ident, x, load)                                                          \
   NumericalDomains::DDoubleExact::MergeBranches(_sourceFile##ident, _sourceLine##ident) << x;   \
   NumericalDomains::DDoubleExact::BaseFloatExact::splitBranches(_oldSourceInfo##ident.first, _oldSourceInfo##ident.second); }
#define FLOAT_MERGE_ALL_OUTER(ident, x, load)                                                    \
   NumericalDomains::DDoubleExact::MergeBranches(_sourceFile##ident, _sourceLine##ident) << x;   \
   NumericalDomains::DDoubleExact::BaseFloatExact::splitBranches(_oldSourceInfo##ident.first, _oldSourceInfo##ident.second);
#else // FLOAT_LOOP_UNSTABLE
#define FLOAT_SPLIT_ALL(ident, merge, save) {                                                    \
   const char* _sourceFile##ident;                                                               \
   int _sourceLine##ident;                                                                       \
   bool _isCompleteFlow##ident = true;                                                           \
   auto _saveMemory##ident = NumericalDomains::DDoubleExact::SaveMemory() << save;               \
   bool _oldDoesFollow##ident = NumericalDomains::DDoubleExact::ExecutionPath::doesFollowFlow(); \
   NumericalDomains::DDoubleExact::ExecutionPath::clearFollowFlow();                             \
   auto _oldSourceInfo##ident = NumericalDomains::DDoubleExact::BaseFloatExact::querySplitInfo();\
   do {                                                                                          \
      _sourceFile##ident = __FILE__;                                                             \
      _sourceLine##ident = __LINE__;                                                             \
      NumericalDomains::DDoubleExact::BaseFloatExact::splitBranches(__FILE__, __LINE__);

#define FLOAT_SPLIT_ALL_LOOP(ident)                                                              \
   _sourceFile##ident = __FILE__;                                                                \
   _sourceLine##ident = __LINE__;                                                                \
   _oldSourceInfo##ident = NumericalDomains::DDoubleExact::BaseFloatExact::querySplitInfo();     \
   NumericalDomains::DDoubleExact::BaseFloatExact::splitBranches(__FILE__, __LINE__);
#define FLOAT_SPLIT_ALL_OUTER(ident, merge, save)                                                \
   bool _isCompleteFlow##ident = true;                                                           \
   auto _saveMemory##ident = NumericalDomains::DDoubleExact::SaveMemory() << save;               \
   bool _oldDoesFollow##ident = NumericalDomains::DDoubleExact::ExecutionPath::doesFollowFlow(); \
   NumericalDomains::DDoubleExact::ExecutionPath::clearFollowFlow();                             \
   auto _oldSourceInfo##ident = NumericalDomains::DDoubleExact::BaseFloatExact::querySplitInfo();\
   do {                                                                                          \
      _sourceFile##ident = __FILE__;                                                             \
      _sourceLine##ident = __LINE__;                                                             \
      NumericalDomains::DDoubleExact::BaseFloatExact::splitBranches(__FILE__, __LINE__);

#define FLOAT_MERGE_ALL(ident, x, load) {}                                                       \
      FLOAT_PRINT_CURRENT_PATH(ident)                                                            \
      _isCompleteFlow##ident = NumericalDomains::DDoubleExact::MergeBranches(_sourceFile##ident, _sourceLine##ident) << x;\
      NumericalDomains::DDoubleExact::ExecutionPath::setFollowFlow();                            \
   } while(!(_saveMemory##ident.setCurrent(_isCompleteFlow##ident) >> load));                    \
   NumericalDomains::DDoubleExact::ExecutionPath::setFollowFlow(_oldDoesFollow##ident);          \
   NumericalDomains::DDoubleExact::BaseFloatExact::splitBranches(_oldSourceInfo##ident.first, _oldSourceInfo##ident.second); }

#define FLOAT_MERGE_ALL_OUTER(ident, x, load) {}                                                 \
      FLOAT_PRINT_CURRENT_PATH(ident)                                                            \
      _isCompleteFlow##ident = NumericalDomains::DDoubleExact::MergeBranches(_sourceFile##ident, _sourceLine##ident) << x;\
      NumericalDomains::DDoubleExact::ExecutionPath::setFollowFlow();                            \
   } while(!(_saveMemory##ident.setCurrent(_isCompleteFlow##ident) >> load));                    \
   NumericalDomains::DDoubleExact::ExecutionPath::setFollowFlow(_oldDoesFollow##ident);          \
   NumericalDomains::DDoubleExact::BaseFloatExact::splitBranches(_oldSourceInfo##ident.first, _oldSourceInfo##ident.second);

#endif // FLOAT_LOOP_UNSTABLE
#endif // !FLOAT_AFFINE && !FLOAT_INTERVAL

#elif !defined(_TWO_TOOL) // !FLOAT_DIAGNOSIS

inline float middle_of_float(float x, float y) { return (x+y)/2.0; }
inline double middle_of_double(double x, double y) { return (x+y)/2.0; }
#define FBETWEEN middle_of_float
#define FBETWEEN_WITH_ERROR(x,y,errmin,errmax) middle_of_float(x,y)
#define DBETWEEN middle_of_double
#define DBETWEEN_WITH_ERROR(x,y,errmin,errmax) middle_of_double(x,y)
#define FPRINT(x) printf("%e\n", x)
#define DPRINT(x) printf("%e\n", x)
#define IPRINT(x) printf("%d\n", x)
#define FAFFPRINT(x) printf("%e\n", x)
#define DAFFPRINT(x) printf("%e\n", x)
#define FCPRINT(name, x) printf("%e\n", x)
#define DCPRINT(name, x) printf("%e\n", x)
#define ICPRINT(name, x) printf("%d\n", x)
#define FCAFFPRINT(name, x) printf("%e\n", x)
#define DCAFFPRINT(name, x) printf("%e\n", x)
#define FSENSITIVITY(x)
#define DSENSITIVITY(x)
#define _FDL 
#define FLOAT_SPLIT_ALL(ident, merge, save) 
#define FLOAT_SPLIT_ALL_LOOP(ident) 
#define FLOAT_SPLIT_ALL_OUTER(ident, merge, save) 
#define FLOAT_MERGE_ALL(ident, x, load) {}
#define FLOAT_MERGE_ALL_OUTER(ident, x, load) {}

#else // !FLOAT_DIAGNOSIS && !_TWO_TOOL

#define _FDL 
#define FLOAT_SPLIT_ALL(ident, merge, save) 
#define FLOAT_SPLIT_ALL_LOOP(ident) 
#define FLOAT_SPLIT_ALL_OUTER(ident, merge, save) 
#define FLOAT_MERGE_ALL(ident, x, load) {}
#define FLOAT_MERGE_ALL_OUTER(ident, x, load) {}

#endif // !FLOAT_DIAGNOSIS && !_TWO_TOOL

#endif // FLOAT_DIAGNOSIS_H

#elif !defined(FLOAT_HEADER_END) /* FLOAT_HEADER_INIT */

#ifdef FLOAT_DIAGNOSIS
#ifdef FLOAT_AFFINE
  NumericalDomains::FloatZonotope::Initialization init;
  init.setResultFile(FLOAT_HEADER_INIT"_aff");
#elif defined(FLOAT_INTERVAL)
  NumericalDomains::FloatInterval::Initialization init;
  init.setResultFile(FLOAT_HEADER_INIT"_int");
#else
  NumericalDomains::FloatExact::Initialization init;
  init.setResultFile(FLOAT_HEADER_INIT"_exact");
#endif // FLOAT_AFFINE
#if !defined(FLOAT_INTERVAL)
  try {
#endif // FLOAT_INTERVAL
#endif // FLOAT_DIAGNOSIS

#else /* FLOAT_HEADER_END */

#ifdef FLOAT_DIAGNOSIS
#if !defined(FLOAT_INTERVAL)
  }
  catch (double_fld::anticipated_termination&) {}
#ifndef FLOAT_INTERFACE
  catch (STG::EReadError& error) {
    if (error.getMessage())
      std::cerr << "error: " << error.getMessage() << std::endl;
    else
      std::cerr << "error while reading input file!" << std::endl;
  }
#else
#ifdef FLOAT_AFFINE
  catch (NumericalDomains::DAffine::ExecutionPath::read_error& error)
#else
  catch (NumericalDomains::DDoubleExact::ExecutionPath::read_error& error)
#endif
  {
    if (error.getMessage())
      std::cerr << "error: " << error.getMessage() << std::endl;
    else
      std::cerr << "error while reading input file!" << std::endl;
  }
#endif // FLOAT_INTERFACE
#endif // FLOAT_INTERVAL
#endif // FLOAT_DIAGNOSIS

#endif
