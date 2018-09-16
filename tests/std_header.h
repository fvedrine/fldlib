#ifndef _FLTD_STD_HEADERH
#define _FLTD_STD_HEADERH

/* FLUCTUAT */
#if defined(_TWO_TOOL)

#include "daed_builtins.h"
#include "fluctuat_math.h"
#include "float_diagnosis.h"

#define DECLARE_RESOURCES 
#define INIT_MAIN 
#define END_MAIN 

#define FLOAT_IMPLEMENTATION 

/* FLOAT_DIAGNOSIS */
#elif defined(FLOAT_DIAGNOSIS)

#include <errno.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <float.h>

#include <math.h>
#include "float_diagnosis.h"
#include "feature.h"

#define EXTERN_DECLARE_RESOURCES 
#define DECLARE_RESOURCES 
#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)

#ifdef FLOAT_THRESHOLD_DETECTION
#define FLOAT_INIT_THRESHOLD init.setSupportThreshold();
#else
#define FLOAT_INIT_THRESHOLD  
#endif

#ifdef FLOAT_ATOMIC
#define FLOAT_INIT_ATOMIC init.setSupportAtomic();
#else
#define FLOAT_INIT_ATOMIC  
#endif

#ifdef FLOAT_LOOP_UNSTABLE
#define FLOAT_INIT_LOOP_UNSTABLE init.setSupportUnstableInLoop();
#else
#define FLOAT_INIT_LOOP_UNSTABLE  
#endif

#ifdef FLOAT_SILENT_COMPUTATIONS
#define FLOAT_INIT_VERBOSE 
#else
#define FLOAT_INIT_VERBOSE init.setSupportVerbose();
#endif

#ifdef FLOAT_FIRST_FOLLOW_EXE
#define FLOAT_INIT_FIRST_FOLLOW_EXE init.setSupportFirstFollowFloat();
#else
#define FLOAT_INIT_FIRST_FOLLOW_EXE 
#endif

#ifdef FLOAT_BACKTRACE
#define FLOAT_INIT_BACKTRACE init.setSupportBacktrace();
#else
#define FLOAT_INIT_BACKTRACE 
#endif

#ifdef FLOAT_AFFINE
#ifdef FLOAT_SCENARIO
#define FLOAT_PROG_SUFFIX "_scenario"
#else
#define FLOAT_PROG_SUFFIX ""
#endif

#ifdef FLOAT_TRIGGER_PERCENT
#define FLOAT_INIT_SIMPLIFICATION init.setSimplificationTriggerPercent(FLOAT_TRIGGER_PERCENT);
#else
#define FLOAT_INIT_SIMPLIFICATION 
#endif

#ifdef FLOAT_PURE_ZONOTOPE
#define FLOAT_INIT_PURE_ZONOTOPE init.setSupportPureZonotope();
#else
#define FLOAT_INIT_PURE_ZONOTOPE 
#endif

#ifdef FLOAT_TRACK
#define FLOAT_INIT_TRACK init.setTrackErrorOrigin();
#else
#define FLOAT_INIT_TRACK 
#endif

#ifdef FLOAT_LIMIT_NOISE_SYMBOLS_NUMBER
#define FLOAT_INIT_LIMIT_NOISE_SYMBOLS_NUMBER init.setLimitNoiseSymbolsNumber(FLOAT_LIMIT_NOISE_SYMBOLS_NUMBER);
#else
#define FLOAT_INIT_LIMIT_NOISE_SYMBOLS_NUMBER 
#endif

#ifdef FLOAT_MAP_SYMBOLS
#define FLOAT_INIT_MAP_SYMBOLS(filename) init.setSupportMapSymbols(filename);
#else
#define FLOAT_INIT_MAP_SYMBOLS(filename) 
#endif

#ifdef FLOAT_LOOP_UNSTABLE
#define INIT_MAIN                                                                                \
  NumericalDomains::FloatZonotope::Initialization init;                                          \
  FLOAT_INIT_ATOMIC                                                                              \
  FLOAT_INIT_LOOP_UNSTABLE                                                                       \
  FLOAT_INIT_VERBOSE                                                                             \
  FLOAT_INIT_THRESHOLD                                                                           \
  FLOAT_INIT_FIRST_FOLLOW_EXE                                                                    \
  FLOAT_INIT_SIMPLIFICATION                                                                      \
  FLOAT_INIT_LIMIT_NOISE_SYMBOLS_NUMBER                                                          \
  FLOAT_INIT_PURE_ZONOTOPE                                                                       \
  FLOAT_INIT_BACKTRACE                                                                           \
  FLOAT_INIT_TRACK                                                                               \
  init.setResultFile(TOSTRING(PROG_NAME) "_diag_aff" FLOAT_PROG_SUFFIX);                         \
  FLOAT_INIT_MAP_SYMBOLS(TOSTRING(PROG_NAME) FLOAT_PROG_SUFFIX "_map")                           \
  std::cout << FLOAT_INIT_MESSAGE << std::endl;                                                  \
  try {                                                                                          \
     FLOAT_SPLIT_ALL(main, double::end(), double::end())

#define END_MAIN                                                                                 \
     FLOAT_MERGE_ALL(main, double::end(), double::end())                                         \
  }                                                                                              \
  catch (double::anticipated_termination&) {}                                                    \
  catch (FLOAT_READ_ERROR& error) {                                                              \
    if (error.getMessage())                                                                      \
      std::cerr << "error: " << error.getMessage() << std::endl;                                 \
    else                                                                                         \
      std::cerr << "error while reading input file!" << std::endl;                               \
  }                                                                                              \
  catch (FLOAT_PRECONDITION_ERROR& error) {                                                      \
      error.print(std::cout);                                                                    \
  }

#else // FLOAT_LOOP_UNSTABLE

#define INIT_MAIN                                                                                \
  NumericalDomains::FloatZonotope::Initialization init;                                          \
  FLOAT_INIT_ATOMIC                                                                              \
  FLOAT_INIT_LOOP_UNSTABLE                                                                       \
  FLOAT_INIT_VERBOSE                                                                             \
  FLOAT_INIT_THRESHOLD                                                                           \
  FLOAT_INIT_FIRST_FOLLOW_EXE                                                                    \
  FLOAT_INIT_SIMPLIFICATION                                                                      \
  FLOAT_INIT_LIMIT_NOISE_SYMBOLS_NUMBER                                                          \
  FLOAT_INIT_PURE_ZONOTOPE                                                                       \
  FLOAT_INIT_BACKTRACE                                                                           \
  FLOAT_INIT_TRACK                                                                               \
  init.setResultFile(TOSTRING(PROG_NAME) "_diag_aff" FLOAT_PROG_SUFFIX);                         \
  FLOAT_INIT_MAP_SYMBOLS(TOSTRING(PROG_NAME) FLOAT_PROG_SUFFIX "_map")                           \
  std::cout << FLOAT_INIT_MESSAGE << std::endl;                                                  \
  try {
#define END_MAIN                                                                                 \
  }                                                                                              \
  catch (double::anticipated_termination&) {}                                                    \
  catch (FLOAT_READ_ERROR& error) {                                                              \
    if (error.getMessage())                                                                      \
      std::cerr << "error: " << error.getMessage() << std::endl;                                 \
    else                                                                                         \
      std::cerr << "error while reading input file!" << std::endl;                               \
  }                                                                                              \
  catch (FLOAT_PRECONDITION_ERROR& error) {                                                      \
      error.print(std::cout);                                                                    \
  }

#endif // FLOAT_LOOP_UNSTABLE

#ifdef FLOAT_SCENARIO

#undef FBETWEEN
#undef FBETWEEN_WITH_ERROR
#undef DBETWEEN
#undef DBETWEEN_WITH_ERROR

static inline NumericalDomains::FloatZonotope middle_of_float(old_float x, old_float y)
   {  old_float res = x + (y-x)*((old_float) rand() / RAND_MAX);
      old_float res_min = res - 1e-4, res_max = res + 1e-4;
      if (res_min < x)
         res_min = x;
      if (res_max > y)
         res_min = y;
      return NumericalDomains::FloatZonotope(res_min, res_max);
   }
static inline NumericalDomains::FloatZonotope middle_of_float_with_error(old_float x, old_float y, old_float errmin, old_float errmax)
   {  old_float res = x + (y-x)*((old_float) rand() / RAND_MAX);
      old_float res_min = res - 1e-4, res_max = res + 1e-4;
      if (res_min < x)
         res_min = x;
      if (res_max > y)
         res_min = y;
      return NumericalDomains::FloatZonotope(res_min, res_max, errmin, errmax);
   }
static inline NumericalDomains::DoubleZonotope middle_of_double(old_double x, old_double y)
   {  old_double res = x + (y-x)*((old_double) rand() / RAND_MAX);
      old_double res_min = res - 1e-8, res_max = res + 1e-8;
      if (res_min < x)
         res_min = x;
      if (res_max > y)
         res_min = y;
      return NumericalDomains::DoubleZonotope(res_min, res_max);
   }
static inline NumericalDomains::DoubleZonotope middle_of_double_with_error(old_double x, old_double y, old_double errmin, old_double errmax)
   {  old_double res = x + (y-x)*((old_double) rand() / RAND_MAX);
      old_double res_min = res - 1e-8, res_max = res + 1e-8;
      if (res_min < x)
         res_min = x;
      if (res_max > y)
         res_min = y;
      return NumericalDomains::DoubleZonotope(res_min, res_max, errmin, errmax);
   }

#define FBETWEEN(x,y) middle_of_float((old_float) x, (old_float) y)
#define FBETWEEN_WITH_ERROR(x,y,errmin,errmax) middle_of_float_with_error((old_float) x, (old_float) y, (old_float) errmin, (old_float) errmax)
#define DBETWEEN(x,y) middle_of_double((old_double) x, (old_double) y)
#define DBETWEEN_WITH_ERROR(x,y,errmin,errmax) middle_of_double_with_error((old_double) x, (old_double) y, (old_double) errmin, (old_double) errmax)

#endif // FLOAT_SCENARIO

#define fld_implementation(x) (x).asImplementation()
#define FLOAT_IMPLEMENTATION fld_implementation

#define FLD_FFROM_STRING NumericalDomains::ParseFloatZonotope
#define FLD_DFROM_STRING NumericalDomains::ParseDoubleZonotope

#elif defined(FLOAT_INTERVAL)
#if !defined(FLOAT_LOOP_UNSTABLE)
#define INIT_MAIN                                                                                \
  NumericalDomains::FloatInterval::Initialization init;                                          \
  FLOAT_INIT_ATOMIC                                                                              \
  FLOAT_INIT_LOOP_UNSTABLE                                                                       \
  FLOAT_INIT_VERBOSE                                                                             \
  FLOAT_INIT_THRESHOLD                                                                           \
  FLOAT_INIT_FIRST_FOLLOW_EXE                                                                    \
  FLOAT_INIT_BACKTRACE                                                                           \
  init.setResultFile(TOSTRING(PROG_NAME) "_diag_int");                                           \
  std::cout << FLOAT_INIT_MESSAGE << std::endl;                                                  \
  FLOAT_SPLIT_ALL(main, double::end(), double::end())
#define END_MAIN                                                                                 \
  FLOAT_MERGE_ALL(main, double::end(), double::endl())
#else // FLOAT_LOOP_UNSTABLE
#define INIT_MAIN                                                                                \
  NumericalDomains::FloatInterval::Initialization init;                                          \
  FLOAT_INIT_ATOMIC                                                                              \
  FLOAT_INIT_LOOP_UNSTABLE                                                                       \
  FLOAT_INIT_VERBOSE                                                                             \
  FLOAT_INIT_THRESHOLD                                                                           \
  FLOAT_INIT_FIRST_FOLLOW_EXE                                                                    \
  FLOAT_INIT_BACKTRACE                                                                           \
  init.setResultFile(TOSTRING(PROG_NAME) "_diag_int");                                           \
  std::cout << FLOAT_INIT_MESSAGE << std::endl;
#define END_MAIN 
#endif // FLOAT_LOOP_UNSTABLE

#define fld_implementation(x) (x).asImplementation()
#define FLOAT_IMPLEMENTATION fld_implementation

#define FLD_FFROM_STRING NumericalDomains::ParseFloatInterval
#define FLD_DFROM_STRING NumericalDomains::ParseDoubleInterval

#else // FLOAT_EXACT

#ifdef FLOAT_LOOP_UNSTABLE
#define INIT_MAIN                                                                                \
  NumericalDomains::FloatExact::Initialization init;                                             \
  FLOAT_INIT_ATOMIC                                                                              \
  FLOAT_INIT_LOOP_UNSTABLE                                                                       \
  FLOAT_INIT_VERBOSE                                                                             \
  FLOAT_INIT_THRESHOLD                                                                           \
  FLOAT_INIT_FIRST_FOLLOW_EXE                                                                    \
  FLOAT_INIT_BACKTRACE                                                                           \
  init.setResultFile(TOSTRING(PROG_NAME) "_diag_exact");                                         \
  std::cout << FLOAT_INIT_MESSAGE << std::endl;                                                  \
  try {                                                                                          \
     FLOAT_SPLIT_ALL(main, double::end(), double::end())

#define END_MAIN                                                                                 \
     FLOAT_MERGE_ALL(main, double::end(), double::end())                                         \
  }                                                                                              \
  catch (double::anticipated_termination&) {}                                                    \
  catch (FLOAT_READ_ERROR& error) {                                                              \
    if (error.getMessage())                                                                      \
      std::cerr << "error: " << error.getMessage() << std::endl;                                 \
    else                                                                                         \
      std::cerr << "error while reading input file!" << std::endl;                               \
  }                                                                                              \
  catch (FLOAT_PRECONDITION_ERROR& error) {                                                      \
      error.print(std::cout);                                                                    \
  }
#else // FLOAT_LOOP_UNSTABLE
#define INIT_MAIN                                                                                \
  NumericalDomains::FloatExact::Initialization init;                                             \
  FLOAT_INIT_ATOMIC                                                                              \
  FLOAT_INIT_LOOP_UNSTABLE                                                                       \
  FLOAT_INIT_VERBOSE                                                                             \
  FLOAT_INIT_THRESHOLD                                                                           \
  FLOAT_INIT_FIRST_FOLLOW_EXE                                                                    \
  FLOAT_INIT_BACKTRACE                                                                           \
  init.setResultFile(TOSTRING(PROG_NAME) "_diag_exact");                                         \
  std::cout << FLOAT_INIT_MESSAGE << std::endl;                                                  \
  try {
#define END_MAIN                                                                                 \
  }                                                                                              \
  catch (double::anticipated_termination&) {}                                                    \
  catch (FLOAT_READ_ERROR& error) {                                                              \
    if (error.getMessage())                                                                      \
      std::cerr << "error: " << error.getMessage() << std::endl;                                 \
    else                                                                                         \
      std::cerr << "error while reading input file!" << std::endl;                               \
  }                                                                                              \
  catch (FLOAT_PRECONDITION_ERROR& error) {                                                      \
      error.print(std::cout);                                                                    \
  }
#endif // FLOAT_LOOP_UNSTABLE

#undef FBETWEEN
#undef FBETWEEN_WITH_ERROR
#undef DBETWEEN
#undef DBETWEEN_WITH_ERROR

static inline old_float random_float(old_float x, old_float y)
   {  return x + (y-x)*((old_float) rand() / RAND_MAX); }
static inline old_double random_double(old_double x, old_double y)
   {  return x + (y-x)*((old_double) rand() / RAND_MAX); }

#define FBETWEEN(x,y) NumericalDomains::FloatExact(random_float((old_float) x, (old_float) y))
#define FBETWEEN_WITH_ERROR(x,y,errmin,errmax) NumericalDomains::FloatExact(random_float((old_float) x, (old_float) y), random_float((old_float) errmin, (old_float) errmax), NumericalDomains::FloatExact::ErrorParameter())
#define DBETWEEN(x,y) NumericalDomains::DoubleExact(random_double((old_double) x, (old_double) y))
#define DBETWEEN_WITH_ERROR(x,y,errmin,errmax) NumericalDomains::DoubleExact(random_double((old_double) x, (old_double) y), random_double((old_double) errmin, (old_double) errmax), NumericalDomains::DoubleExact::ErrorParameter())

#define fld_implementation(x) (x).asImplementation()
#define FLOAT_IMPLEMENTATION fld_implementation

#define FLD_FFROM_STRING NumericalDomains::ParseFloatExact
#define FLD_DFROM_STRING NumericalDomains::ParseDoubleExact

#endif // FLOAT_INTERVAL && FLOAT_AFFINE && FLOAT_EXACT

/* standard execution */
#else

#include <math.h>
#include <float.h>
#include <stdlib.h>
#include <stdio.h>
#include "float_diagnosis.h"

#define float_fld float
#define double_fld double
typedef float old_float;
typedef double old_double;
typedef long double old_long_double;

#define FLOAT_IMPLEMENTATION 
static inline float fld_float_from_string(const char* x) {
   float result = 0;
   sscanf(x, "%f", &result);
   return result;
}

static inline double fld_double_from_string(const char* x) {
   double result = 0;
   sscanf(x, "%lf", &result);
   return result;
}

#define FLD_FFROM_STRING fld_float_from_string
#define FLD_DFROM_STRING fld_float_from_string

static inline float rand_of_float(float x, float y)
   {  return x + (y-x)*((float) rand() / RAND_MAX); }
static inline double rand_of_double(double x, double y)
   {  return x + (y-x)*((double) rand() / RAND_MAX); }

#undef FBETWEEN
#undef FBETWEEN_WITH_ERROR
#undef DBETWEEN
#undef DBETWEEN_WITH_ERROR

// static inline float rand_of_float(float x, float y) { return (x+y)/2.0; }
// static inline double rand_of_double(double x, double y) { return (x+y)/2.0; }
#define FBETWEEN rand_of_float
#define FBETWEEN_WITH_ERROR(x,y,errmin,errmax) rand_of_float(x,y)
#define DBETWEEN rand_of_double
#define DBETWEEN_WITH_ERROR(x,y,errmin,errmax) rand_of_double(x,y)

#define FEXACT 
#define DEXACT 

/*
#define FPRINT(x) 
#define DPRINT(x) 
#define IPRINT(x) 
*/
/*
#define FPRINT(x) printf("%s: %e\n", #x, x)
#define DPRINT(x) printf("%s: %e\n", #x, x)
#define IPRINT(x) printf("%s: %d\n", #x, x)
*/
#define EXTERN_DECLARE_RESOURCES 
#define DECLARE_RESOURCES 
#define INIT_MAIN 
#define END_MAIN 

#endif

#endif
