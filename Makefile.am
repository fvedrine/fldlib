ACLOCAL_AMFLAGS=-I m4
LIBTOOL_DEPS = @LIBTOOL_DEPS@
libtool: $(LIBTOOL_DEPS)
	$(SHELL) ./config.status libtool

dist_bin_SCRIPTS = tests/scan_accuracy_directory.py 
bin_SCRIPTS = tests/comp_float_diagnosis.sh
CLEANFILES = $(bin_SCRIPTS)
 
if FLOAT_LIB_INTERFACE
nobase_pkginclude_HEADERS =                                       \
   applications/FloatInstrumentation/ExactTypesSize.h             \
   applications/FloatInstrumentation/IntervalTypesSize.h          \
   applications/FloatInstrumentation/AffineTypesSize.h            \
   applications/FloatInstrumentation/FloatExactInterface.h        \
   applications/FloatInstrumentation/FloatIntervalInterface.h     \
   applications/FloatInstrumentation/FloatAffineInterface.h
else
nobase_pkginclude_HEADERS =                                       \
   utils/StandardClasses/DefineNew.h                              \
   utils/StandardClasses/UndefineNew.h                            \
   utils/StandardClasses/StandardMessage.h                        \
   utils/StandardClasses/StandardClasses.macro                    \
   utils/StandardClasses/StandardClasses.hpp                      \
   utils/StandardClasses/IOStreamMethods.inch                     \
   utils/StandardClasses/IOStreamMethods.incc                     \
   utils/StandardClasses/Persistence.h                            \
   utils/Pointer/Vector.h                                         \
   utils/Pointer/ImplList.h                                       \
   utils/Pointer/ImplArray.h                                      \
   utils/Pointer/Pointer.h                                        \
   utils/Pointer/MngPointer.h                                     \
   utils/Pointer/AutoPointer.h                                    \
   utils/Pointer/PassPointer.h                                    \
   utils/Pointer/SharedPointer.h                                  \
   utils/Pointer/Binary.h                                         \
   utils/Pointer/Binary.inch                                      \
   utils/Pointer/SharedCollection.h                               \
   utils/Pointer/Pointer.hpp                                      \
   utils/Pointer/ExtPointer.hpp                                   \
   utils/Pointer/LinearCollection.hpp                             \
   utils/Collection/ELMCollection.h                               \
   utils/Collection/ELMCollection.incfoot                         \
   utils/Collection/ELMCollection.inch                            \
   utils/Collection/ELMCollection.incheader                       \
   utils/Collection/VirtualCollection/AbstractCollection.macro    \
   utils/Collection/VirtualCollection/AbstractCollection.h        \
   utils/Collection/VirtualCollection/ExtendedParameters.h        \
   utils/Collection/VirtualCollection/VirtualCollection.h         \
   utils/Collection/VirtualCollection/VirtualCollection.inline    \
   utils/Collection/VirtualCollection/VirtualSortedCollection.h   \
   utils/Collection/ConcreteCollection/List.h                     \
   utils/Collection/ConcreteCollection/List.inline                \
   utils/Collection/ConcreteCollection/Array.h                    \
   utils/Collection/ConcreteCollection/Array.inch                 \
   utils/Collection/ConcreteCollection/Array.inline               \
   utils/Collection/ConcreteCollection/SortedArray.h              \
   utils/Collection/Collection.hpp                                \
   algorithms/ForwardNumerics/BaseIntegerCommon.h                 \
   algorithms/ForwardNumerics/BaseInteger.inch                    \
   algorithms/ForwardNumerics/BaseInteger.h                       \
   algorithms/ForwardNumerics/BaseIntegerGeneric.h                \
   algorithms/ForwardNumerics/BaseFloatingParameters.h            \
   algorithms/ForwardNumerics/BaseFloating.inch                   \
   algorithms/ForwardNumerics/BaseFloating.h                      \
   algorithms/ForwardNumerics/BaseFloatingGeneric.h               \
   algorithms/ForwardNumerics/BaseTaylorFloating.inch             \
   algorithms/ForwardNumerics/BaseTaylorFloating.h                \
   algorithms/ForwardNumerics/BaseTaylorFloatingGeneric.h         \
   algorithms/BackwardNumerics/BaseProjectionFloating.h           \
   algorithms/Numerics/Integer.h                                  \
   algorithms/Numerics/Floating.h                                 \
   algorithms/Numerics/TaylorFloating.h                           \
   algorithms/Numerics/Numerics.hpp                               \
   algorithms/NumericalLattices/FloatExactBaseTypes.inch          \
   algorithms/NumericalLattices/FloatExactBaseTypes.h             \
   algorithms/NumericalLattices/FloatExactBaseTypesGeneric.h      \
   algorithms/NumericalLattices/FloatIntervalBaseTypes.h          \
   algorithms/NumericalLattices/FloatAffineBaseCommon.h           \
   algorithms/NumericalLattices/FloatAffineBaseComponents.inch    \
   algorithms/NumericalLattices/FloatAffineBaseComponents.h       \
   algorithms/NumericalLattices/FloatAffineBaseComponentsGeneric.h\
   algorithms/NumericalDomains/FloatExactBase.inch                \
   algorithms/NumericalDomains/FloatExactBase.h                   \
   algorithms/NumericalDomains/FloatExactBaseGeneric.h            \
   algorithms/NumericalDomains/FloatIntervalBase.h                \
   algorithms/NumericalDomains/FloatAffineBase.inch               \
   algorithms/NumericalDomains/FloatAffineBase.h                  \
   algorithms/NumericalDomains/FloatAffineBaseGeneric.h           \
   applications/NumericalAnalysis/FloatExactExecutionPath.h       \
   applications/NumericalAnalysis/FloatIntervalExecutionPath.h    \
   applications/NumericalAnalysis/FloatAffineExecutionPath.h      \
   applications/FloatInstrumentation/BackTrace.h                  \
   applications/FloatInstrumentation/FloatExact.inch              \
   applications/FloatInstrumentation/FloatExact.h                 \
   applications/FloatInstrumentation/FloatInterval.inch           \
   applications/FloatInstrumentation/FloatInterval.h              \
   applications/FloatInstrumentation/FloatAffine.inch             \
   applications/FloatInstrumentation/FloatAffine.h                \
   utils/Pointer/Vector.template                                  \
   utils/Pointer/AutoPointer.template                             \
   utils/Pointer/ImplList.template                                \
   utils/Pointer/ImplArray.template                               \
   utils/Pointer/Binary.template                                  \
   utils/Collection/ELMCollection.template                        \
   utils/Collection/VirtualCollection/VirtualCollection.template  \
   algorithms/ForwardNumerics/BaseInteger.inctemplate             \
   algorithms/ForwardNumerics/BaseInteger.template                \
   algorithms/ForwardNumerics/BaseIntegerGeneric.template         \
   algorithms/ForwardNumerics/BaseFloating.inctemplate            \
   algorithms/ForwardNumerics/BaseFloating.template               \
   algorithms/ForwardNumerics/BaseFloatingGeneric.template        \
   algorithms/ForwardNumerics/BaseTaylorFloating.inctemplate      \
   algorithms/ForwardNumerics/BaseTaylorFloating.template         \
   algorithms/ForwardNumerics/BaseTaylorFloatingGeneric.template  \
   algorithms/BackwardNumerics/BaseProjectionFloating.template    \
   algorithms/Numerics/Integer.template                           \
   algorithms/Numerics/Floating.template                          \
   algorithms/Numerics/TaylorFloating.template                    \
   algorithms/NumericalLattices/FloatExactBaseTypes.inctemplate   \
   algorithms/NumericalLattices/FloatExactBaseTypes.template      \
   algorithms/NumericalLattices/FloatExactBaseTypesGeneric.template\
   algorithms/NumericalLattices/FloatAffineBaseTypes.inctemplate  \
   algorithms/NumericalLattices/FloatAffineBaseTypes.template     \
   algorithms/NumericalLattices/FloatAffineBaseTypesGeneric.template\
   algorithms/NumericalLattices/FloatAffineBaseComponents.inctemplate\
   algorithms/NumericalLattices/FloatAffineBaseComponents.template\
   algorithms/NumericalLattices/FloatAffineBaseComponentsGeneric.template\
   algorithms/NumericalDomains/FloatExactBase.inctemplate         \
   algorithms/NumericalDomains/FloatExactBase.template            \
   algorithms/NumericalDomains/FloatExactBaseGeneric.template     \
   algorithms/NumericalDomains/FloatIntervalBase.template         \
   algorithms/NumericalDomains/FloatAffineBase.inctemplate        \
   algorithms/NumericalDomains/FloatAffineBase.template           \
   algorithms/NumericalDomains/FloatAffineBaseGeneric.template    \
   applications/NumericalAnalysis/FloatExactExecutionPath.template\
   applications/NumericalAnalysis/FloatIntervalExecutionPath.template\
   applications/NumericalAnalysis/FloatAffineExecutionPath.template\
   config.h
endif

SOURCES =                                                         \
   utils/StandardClasses/StandardClasses.cpp                      \
   utils/StandardClasses/Persistence.cpp                          \
   utils/Pointer/ImplList.cpp                                     \
   utils/Pointer/ImplArray.cpp                                    \
   utils/Pointer/Pointer.cpp                                      \
   utils/Pointer/MngPointer.cpp                                   \
   utils/Pointer/SharedPointer.cpp                                \
   utils/Pointer/Binary.cpp                                       \
   utils/Pointer/SharedCollection.cpp                             \
   utils/Collection/VirtualCollection/VirtualCollection.cpp       \
   utils/Collection/ConcreteCollection/List.cpp                   \
   utils/Collection/ConcreteCollection/Array.cpp                  \
   utils/Collection/ConcreteCollection/SortedArray.cpp            \
   utils/Collection/ConcreteCollection/MultiArray.cpp             \
   algorithms/Numerics/Integer.cpp                                \
   applications/NumericalAnalysis/FloatCommonTypes.cpp            \
   applications/NumericalAnalysis/FloatExactTypes.cpp             \
   applications/NumericalAnalysis/FloatIntervalTypes.cpp          \
   applications/NumericalAnalysis/FloatAffineTypes.cpp            \
   applications/FloatInstrumentation/BackTrace.cpp

SOURCES_OPEN = $(SOURCES)                                         \
   applications/FloatInstrumentation/FloatExact.cpp               \
   applications/FloatInstrumentation/FloatInterval.cpp            \
   applications/FloatInstrumentation/FloatAffine.cpp

SOURCES_CLOSE = $(SOURCES)                                        \
   applications/FloatInstrumentation/FloatExactInterface.cpp      \
   applications/FloatInstrumentation/FloatIntervalInterface.cpp   \
   applications/FloatInstrumentation/FloatAffineInterface.cpp

if FLOAT_LIB_INTERFACE
noinst_LTLIBRARIES = libFloatDiagnosisCore.la 
pkglib_LTLIBRARIES = libFloatDiagnosisInterface.la
else
pkglib_LTLIBRARIES = libFloatDiagnosis.la
endif

pkginclude_HEADERS = float_diagnosis.h tests/std_header.h tests/feature.h

if FLOAT_LIB_INTERFACE
libFloatDiagnosisCore_la_SOURCES = $(SOURCES)
libFloatDiagnosisCore_la_CPPFLAGS = -I@top_srcdir@/utils -I@top_srcdir@/algorithms -I@top_srcdir@/applications
libFloatDiagnosisCore_la_CXXFLAGS = -Woverloaded-virtual -fmessage-length=0

noinst_bin_PROGRAMS = TypesSizes$(EXEEXT)
noinst_bindir = $(top_buildir)

TypesSizes_SOURCES = applications/FloatInstrumentation/TypesSizes.cpp
TypesSizes_CPPFLAGS = -I@top_srcdir@/utils -I@top_srcdir@/algorithms -I@top_srcdir@/applications
TypesSizes_CXXFLAGS = -Woverloaded-virtual -fmessage-length=0
TypesSizes_LDADD = libFloatDiagnosisCore.la

BUILT_SOURCES =                                                   \
   applications/FloatInstrumentation/ExactTypesSize.h             \
   applications/FloatInstrumentation/IntervalTypesSize.h          \
   applications/FloatInstrumentation/AffineTypesSize.h

applications/FloatInstrumentation/ExactTypesSize.h: TypesSizes$(EXEEXT)
	./TypesSizes$(EXEEXT)
applications/FloatInstrumentation/IntervalTypesSize.h: applications/FloatInstrumentation/ExactTypesSize.h
applications/FloatInstrumentation/AffineTypesSize.h: applications/FloatInstrumentation/ExactTypesSize.h

libFloatDiagnosisInterface_la_SOURCES =                           \
   applications/FloatInstrumentation/FloatExactInterface.cpp      \
   applications/FloatInstrumentation/FloatIntervalInterface.cpp   \
   applications/FloatInstrumentation/FloatAffineInterface.cpp

libFloatDiagnosisInterface_la_CPPFLAGS = -I@top_srcdir@/utils -I@top_srcdir@/algorithms -I@top_srcdir@/applications -I@top_builddir@/applications
libFloatDiagnosisInterface_la_CXXFLAGS = -Woverloaded-virtual -fmessage-length=0
libFloatDiagnosisInterface_la_LIBADD = $(libFloatDiagnosisCore_la_OBJECTS)

else
libFloatDiagnosis_la_SOURCES = $(SOURCES_OPEN)
libFloatDiagnosis_la_CPPFLAGS = -I@top_srcdir@/utils -I@top_srcdir@/algorithms -I@top_srcdir@/applications
libFloatDiagnosis_la_CXXFLAGS = -Woverloaded-virtual -fmessage-length=0
endif

