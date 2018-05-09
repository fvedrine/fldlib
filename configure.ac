#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT(FLDLib, 0.9.0, franck.vedrine@cea.fr)
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_AUX_DIR(config)
AC_CONFIG_SRCDIR([float_diagnosis.h])
AC_CONFIG_HEADERS([config.h])

AM_INIT_AUTOMAKE([subdir-objects tar-pax])

# Checks for programs.
AC_LANG([C++])
AC_PROG_CXX
AC_PROG_CC
AC_PROG_MAKE_SET
AX_CXX_COMPILE_STDCXX_11(noext,mandatory)
LT_PREREQ([2.4])
LT_INIT
AC_SUBST([LIBTOOL_DEPS])
AC_ENABLE_SHARED
AX_CXXFLAGS_WARN_ALL

# Checks for libraries.
# FIXME: Replace `main' with a function in `-lm':
AC_CHECK_LIB([m], [sqrt])
AC_CHECK_LIB([m], [sin])
AC_CHECK_LIB([m], [cos])
AC_CHECK_LIB([m], [asin])
AC_CHECK_LIB([m], [acos])
AC_CHECK_LIB([m], [tan])
AC_CHECK_LIB([m], [atan])
AC_CHECK_LIB([m], [exp])
AC_CHECK_LIB([m], [log])
AC_CHECK_LIB([m], [log10])
AC_CHECK_LIB([m], [pow])
AC_CHECK_LIB([m], [powf])
AC_CHECK_LIB([m], [atan2])
AC_CHECK_LIB([m], [fabs])

# Checks for header files.
AC_CHECK_HEADERS([float.h limits.h memory.h stddef.h stdlib.h string.h])

CHECK_CXXABI

# Checks for typedefs, structures, and compiler characteristics.
AC_CHECK_HEADER_STDBOOL
AC_C_INLINE
AC_TYPE_SIZE_T

# Checks for library functions.
AC_FUNC_MALLOC
AC_FUNC_REALLOC
AC_CHECK_FUNCS([fesetround floor memmove memset pow rint sqrt])

# AX_CHECK_COMPILE_FLAG([-std=c++11], [CXXFLAGS="$CXXFLAGS -std=c++11"])

# Checks for compiler vendor
AC_CHECK_DEFINE(__GNUC__, AC_SUBST([HAVE_GNUC],[1]), AC_SUBST([HAVE_GNUC],[0]))
AC_CHECK_DEFINE(__clang__, AC_SUBST([HAVE_CLANG],[1]), AC_SUBST([HAVE_CLANG],[0]))

AC_HEADER_ASSERT
AC_ARG_ENABLE([interface],
   [AS_HELP_STRING([--enable-interface], [use a simplified interface])],
   [], [enable_interface=no])
AC_ARG_ENABLE([readexception],
   [AS_HELP_STRING([--enable-readexception], [allow read exception if file_in is altered])],
   [], [enable_readexception=no])
AC_ARG_ENABLE([genericlong],
   [AS_HELP_STRING([--disable-genericlong], [disable the use of long int to store mantissa bits])],
   [], [enable_genericlong=yes])
AC_ARG_ENABLE([genericunsigned],
   [AS_HELP_STRING([--enable-genericunsigned], [enable the use of unsigned int to store mantissa bits])],
   [], [enable_genericunsigned=no])
AC_ARG_ENABLE([longwrite],
   [AS_HELP_STRING([--disable-longwrite], [disable the natural decimal writing of real numbers])],
   [], [enable_longwrite=yes])

AC_ARG_VAR(realbitsnumber,
	AS_HELP_STRING([number of bits for the mantissa of real numbers])
)

AC_ARG_ENABLE([simplex],
   [AS_HELP_STRING([--enable-simplex], [push the constraints onto a stack to apply them in an optimal but costly way])],
   [], [enable_simplex=no])
AC_ARG_ENABLE([absorbhighlevel],
   [AS_HELP_STRING([--enable-absorbhighlevel], [enable the systematic absorption of high level symbols into standard noise symbols])],
   [], [enable_absorbhighlevel=no])
AC_ARG_VAR(limitsymbolabsorption,
	AS_HELP_STRING([define the limit in binary digits from which a symbol is absorbed into a standard noise symbol])
)
AC_ARG_ENABLE([constantexclusionfromabsorption],
   [AS_HELP_STRING([--enable-constantexclusionfromabsorption], [exclude the constant part in the definition of negligeable symbols for their absorption into standard noise symbols])],
   [], [enable_constantexclusionfromabsorption=no])

AM_CONDITIONAL([FLOAT_LIB_INTERFACE],[test "x$enable_interface" != "xno"])
AS_IF([test "x$enable_interface" != "xno"], [
  AC_DEFINE([FLOAT_ALLOW_INTERFACE],[1],[Define to used a simplified interface file])
  AC_SUBST([FLOAT_ALLOW_INTERFACE],[-interface])
  AC_SUBST([FLOAT_LIB_NAME],[libFloatDiagnosisInterface.a])
  AC_SUBST([HAVE_FLOAT_ALLOW_INTERFACE],[1])
],[
  AC_SUBST([FLOAT_ALLOW_INTERFACE],[])
  AC_SUBST([FLOAT_LIB_NAME],[libFloatDiagnosis.a])
  AC_SUBST([HAVE_FLOAT_ALLOW_INTERFACE],[0])
])

AS_IF([test "x$enable_genericunsigned" != "xno"], [
  AC_DEFINE([FLOAT_GENERIC_BASE_UNSIGNED],[1],[Define to use unsigned int for storing mantissa])
  AC_SUBST([FLOAT_GENERIC_BASE_UNSIGNED],[-DFLOAT_GENERIC_BASE_UNSIGNED])
  AC_SUBST([HAVE_FLOAT_GENERIC_BASE_UNSIGNED],[1])
  enable_genericlong=no
],[
  AC_SUBST([HAVE_FLOAT_GENERIC_BASE_UNSIGNED],[0])
])

AS_IF([test "x$enable_genericlong" != "xno"], [
  AC_DEFINE([FLOAT_GENERIC_BASE_LONG],[1],[Define to use long int for storing mantissa])
  AC_SUBST([FLOAT_GENERIC_BASE_LONG],[-DFLOAT_GENERIC_BASE_LONG])
  AC_SUBST([HAVE_FLOAT_GENERIC_BASE_LONG],[1])
],[
  AC_SUBST([HAVE_FLOAT_GENERIC_BASE_LONG],[0])
])

AS_IF([test "x$enable_longwrite" != "xno"], [
  AC_DEFINE([FLOAT_LONG_WRITE],[1],[Define to write real numbers with the natural decimal writing])
  AC_SUBST([FLOAT_LONG_WRITE],[-DFLOAT_LONG_WRITE])
  AC_SUBST([HAVE_FLOAT_LONG_WRITE],[1])
],[
  AC_SUBST([HAVE_FLOAT_LONG_WRITE],[0])
])

AS_IF([test "x$enable_readexception" != "xno"], [
  AC_DEFINE([FLOAT_ALLOW_READ_EXCEPTION],[1],[Define to allow read exceptions])
  AC_SUBST([FLOAT_ALLOW_READ_EXCEPTION],[-DFLOAT_ALLOW_READ_EXCEPTION])
  AC_SUBST([HAVE_FLOAT_ALLOW_READ_EXCEPTION],[1])
],[
  AC_SUBST([HAVE_FLOAT_ALLOW_READ_EXCEPTION],[0])
])

AS_IF([test "x$enable_simplex" != "xno"], [
  AC_DEFINE([FLOAT_ZONOTOPE_ALLOW_SIMPLEX],[1],[Push the constraints onto a stack to apply them in an optimal but costly way])
  AC_SUBST([FLOAT_ZONOTOPE_ALLOW_SIMPLEX],[-DFLOAT_ZONOTOPE_ALLOW_SIMPLEX])
  AC_SUBST([HAVE_FLOAT_ZONOTOPE_ALLOW_SIMPLEX],[1])
  enable_genericlong=no
],[
  AC_SUBST([HAVE_FLOAT_ZONOTOPE_ALLOW_SIMPLEX],[0])
])

AS_IF([test "x$enable_absorbhighlevel" != "xno"], [
  AC_DEFINE([FLOAT_ZONOTOPE_DOES_ABSORB_HIGH_LEVEL],[1],[Enable the systematic absorption of high level symbols into standard noise symbols])
  AC_SUBST([FLOAT_ZONOTOPE_DOES_ABSORB_HIGH_LEVEL],[-DFLOAT_ZONOTOPE_DOES_ABSORB_HIGH_LEVEL])
  AC_SUBST([HAVE_FLOAT_ZONOTOPE_DOES_ABSORB_HIGH_LEVEL],[1])
  enable_genericlong=no
],[
  AC_SUBST([HAVE_FLOAT_ZONOTOPE_DOES_ABSORB_HIGH_LEVEL],[0])
])

AS_IF([test "$limitsymbolabsorption" != ""], [
  AC_DEFINE([FLOAT_ZONOTOPE_LIMIT_SYMBOL_ABSORPTION],[$limitsymbolabsorption],[Define the limit in binary digits from which a symbol is absorbed into a standard noise symbol])
  AC_SUBST([FLOAT_ZONOTOPE_LIMIT_SYMBOL_ABSORPTION],[-DFLOAT_ZONOTOPE_LIMIT_SYMBOL_ABSORPTION=$limitsymbolabsorption])
  AC_SUBST([HAVE_FLOAT_ZONOTOPE_LIMIT_SYMBOL_ABSORPTION],[1])
  AC_SUBST([VALUE_FLOAT_ZONOTOPE_LIMIT_SYMBOL_ABSORPTION],[$limitsymbolabsorption])
],[
  AC_SUBST([HAVE_FLOAT_ZONOTOPE_LIMIT_SYMBOL_ABSORPTION],[0])
])

AS_IF([test "x$enable_constantexclusionfromabsorption" != "xno"], [
  AC_DEFINE([FLOAT_ZONOTOPE_DOES_EXCLUDE_CONSTANT_FROM_SYMBOL_ABSORPTION],[1],[Exclude the constant part in the definition of negligeable symbols for their absorption into standard noise symbols])
  AC_SUBST([FLOAT_ZONOTOPE_DOES_EXCLUDE_CONSTANT_FROM_SYMBOL_ABSORPTION],[-DFLOAT_ZONOTOPE_DOES_EXCLUDE_CONSTANT_FROM_SYMBOL_ABSORPTION])
  AC_SUBST([HAVE_FLOAT_ZONOTOPE_DOES_EXCLUDE_CONSTANT_FROM_SYMBOL_ABSORPTION],[1])
  enable_genericlong=no
],[
  AC_SUBST([HAVE_FLOAT_ZONOTOPE_DOES_EXCLUDE_CONSTANT_FROM_SYMBOL_ABSORPTION],[0])
])

AS_IF([test "$realbitsnumber" != ""], [
  AC_DEFINE_UNQUOTED([FLOAT_REAL_BITS_NUMBER],[$realbitsnumber],[Define the number of bits for the mantissa of real numbers])
  AC_SUBST([FLOAT_REAL_BITS_NUMBER],[-DFLOAT_REAL_BITS_NUMBER=$realbitsnumber])
  AC_SUBST([HAVE_FLOAT_REAL_BITS_NUMBER],[1])
  AC_SUBST([VALUE_FLOAT_REAL_BITS_NUMBER],[$realbitsnumber])
],[
  AC_SUBST([HAVE_FLOAT_REAL_BITS_NUMBER],[0])
])

AC_CONFIG_FILES([Makefile])
AC_CONFIG_FILES([tests/Makefile])
AC_CONFIG_FILES([tests/feature.h])
AC_CONFIG_FILES([tests/comp_float_diagnosis.sh],
                [chmod a+x tests/comp_float_diagnosis.sh])

AC_OUTPUT
