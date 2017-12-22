ACLOCAL_AMFLAGS=-I m4
LIBTOOL_DEPS = @LIBTOOL_DEPS@
libtool: $(LIBTOOL_DEPS)
	$(SHELL) ./config.status libtool

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

#  utils/Collection/ConcreteCollection/Array.incc                 \
#  utils/Collection/VirtualCollection/VirtualCollection.incc      \
#  utils/Collection/ELMCollection.incc                            \
#  utils/Pointer/Binary.incc                                      \
#  utils/StandardClasses/IOStreamMethods.incc                     \
#  applications/FloatInstrumentation/FloatExact.incc              \
#  applications/FloatInstrumentation/FloatInterval.incc           \
#  applications/FloatInstrumentation/FloatAffine.incc
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

myincludesubdir=$(pkgincludedir)/subdir
pkginclude_HEADERS = float_diagnosis.h tests/std_header.h tests/feature.h

#    libFloatAtomicThresholdLUnstableOptimVerbose.la \
#    libFloatAtomicThresholdLUnstableOptim.la \
#    libFloatAtomicThresholdLUnstableVerbose.la \
#    libFloatAtomicThresholdLUnstable.la \
#    libFloatAtomicThresholdOptimVerbose.la \
#    libFloatAtomicThresholdOptim.la \
#    libFloatAtomicThresholdVerbose.la \
#    libFloatAtomicThreshold.la \
#    libFloatAtomicLUnstableOptimVerbose.la \
#    libFloatAtomicLUnstableOptim.la \
#    libFloatAtomicLUnstableVerbose.la \
#    libFloatAtomicLUnstable.la \
#    libFloatAtomicOptimVerbose.la \
#    libFloatAtomicOptim.la \
#    libFloatAtomicVerbose.la \
#    libFloatAtomic.la \
#    libFloatThresholdLUnstableOptimVerbose.la \
#    libFloatThresholdLUnstableOptim.la \
#    libFloatThresholdLUnstableVerbose.la \
#    libFloatThresholdLUnstable.la \
#    libFloatThresholdOptimVerbose.la \
#    libFloatThresholdOptim.la \
#    libFloatThresholdVerbose.la \
#    libFloatThreshold.la \
#    libFloatLUnstableOptimVerbose.la \
#    libFloatLUnstableOptim.la \
#    libFloatLUnstableVerbose.la \
#    libFloatLUnstable.la \
#    libFloatOptimVerbose.la \
#    libFloatOptim.la \
#    libFloatVerbose.la \
#    libFloat.la

if FLOAT_LIB_INTERFACE
libFloatDiagnosisCore_la_SOURCES = $(SOURCES)
libFloatDiagnosisCore_la_CPPFLAGS = -I@top_srcdir@/utils -I@top_srcdir@/algorithms -I@top_srcdir@/applications
libFloatDiagnosisCore_la_CXXFLAGS = -Woverloaded-virtual -fmessage-length=0

noinst_bin_PROGRAMS = TypesSizes$(EXEEXT)
noinst_bindir = $(top_buildir)
dist_bin_SCRIPTS = tests/comp_float_diagnosis.sh
 
TypesSizes_SOURCES = applications/FloatInstrumentation/TypesSizes.cpp
TypesSizes_CPPFLAGS = -I@top_srcdir@/utils -I@top_srcdir@/algorithms -I@top_srcdir@/applications
TypesSizes_CXXFLAGS = -Woverloaded-virtual -fmessage-length=0
TypesSizes_LDADD = libFloatDiagnosisCore.la

BUILT_SOURCES =                                                   \
   applications/FloatInstrumentation/ExactTypesSize.h             \
   applications/FloatInstrumentation/IntervalTypesSize.h          \
   applications/FloatInstrumentation/AffineTypesSize.h

#applications/FloatInstrumentation/TypesSizes.exe: applications/FloatInstrumentation/TypesSizes.cpp Makefile .libs/libFloatDiagnosisCore.la
#	$(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(libFloatDiagnosisInterface_la_CPPFLAGS) $(CPPFLAGS) $(libFloatDiagnosisInterface_la_CXXFLAGS) $(CXXFLAGS) -o applications/FloatInstrumentation/TypesSizes.exe $< -L .libs $(LDFLAGS) $(AM_LDFLAGS) -lFloatDiagnosisCore

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

#  tests:
#     cd tests; ...

#  libFloatAtomicThresholdLUnstableOptimVerbose_la_SOURCES = $(SOURCES_OPEN)
#  libFloatAtomicThresholdLUnstableOptimVerbose_la_CPPFLAGS = -Iutils -Ialgorithms -Iapplications
#  libFloatAtomicThresholdLUnstableOptimVerbose_la_CXXFLAGS = -Woverloaded-virtual -fmessage-length=0

#  libFloatAtomicThresholdLUnstableOptim_la_SOURCES = $(SOURCES_OPEN)
#  libFloatAtomicThresholdLUnstableOptim_la_CPPFLAGS = -Iutils -Ialgorithms -Iapplications
#  libFloatAtomicThresholdLUnstableOptim_la_CXXFLAGS = -Woverloaded-virtual -fmessage-length=0

#  libFloatAtomicThresholdLUnstableVerbose_la_SOURCES = $(SOURCES_OPEN)
#  libFloatAtomicThresholdLUnstableVerbose_la_CPPFLAGS = -Iutils -Ialgorithms -Iapplications
#  libFloatAtomicThresholdLUnstableVerbose_la_CXXFLAGS = -Woverloaded-virtual -fmessage-length=0

#  libFloatAtomicThresholdLUnstable_la_SOURCES = $(SOURCES_OPEN)
#  libFloatAtomicThresholdLUnstable_la_CPPFLAGS = -Iutils -Ialgorithms -Iapplications
#  libFloatAtomicThresholdLUnstable_la_CXXFLAGS = -Woverloaded-virtual -fmessage-length=0

#  libFloatAtomicThresholdOptimVerbose_la_SOURCES = $(SOURCES_OPEN)
#  libFloatAtomicThresholdOptimVerbose_la_CPPFLAGS = -Iutils -Ialgorithms -Iapplications
#  libFloatAtomicThresholdOptimVerbose_la_CXXFLAGS = -Woverloaded-virtual -fmessage-length=0

#  libFloatAtomicThresholdOptim_la_SOURCES = $(SOURCES_OPEN)
#  libFloatAtomicThresholdOptim_la_CPPFLAGS = -Iutils -Ialgorithms -Iapplications
#  libFloatAtomicThresholdOptim_la_CXXFLAGS = -Woverloaded-virtual -fmessage-length=0

#  libFloatAtomicThresholdVerbose_la_SOURCES = $(SOURCES_OPEN)
#  libFloatAtomicThresholdVerbose_la_CPPFLAGS = -Iutils -Ialgorithms -Iapplications
#  libFloatAtomicThresholdVerbose_la_CXXFLAGS = -Woverloaded-virtual -fmessage-length=0

#  libFloatAtomicThreshold_la_SOURCES = $(SOURCES_OPEN)
#  libFloatAtomicThreshold_la_CPPFLAGS = -Iutils -Ialgorithms -Iapplications
#  libFloatAtomicThreshold_la_CXXFLAGS = -Woverloaded-virtual -fmessage-length=0

#  libFloatAtomicLUnstableOptimVerbose_la_SOURCES = $(SOURCES_OPEN)
#  libFloatAtomicLUnstableOptimVerbose_la_CPPFLAGS = -Iutils -Ialgorithms -Iapplications
#  libFloatAtomicLUnstableOptimVerbose_la_CXXFLAGS = -Woverloaded-virtual -fmessage-length=0

#  libFloatAtomicLUnstableOptim_la_SOURCES = $(SOURCES_OPEN)
#  libFloatAtomicLUnstableOptim_la_CPPFLAGS = -Iutils -Ialgorithms -Iapplications
#  libFloatAtomicLUnstableOptim_la_CXXFLAGS = -Woverloaded-virtual -fmessage-length=0

#  libFloatAtomicLUnstableVerbose_la_SOURCES = $(SOURCES_OPEN)
#  libFloatAtomicLUnstableVerbose_la_CPPFLAGS = -Iutils -Ialgorithms -Iapplications
#  libFloatAtomicLUnstableVerbose_la_CXXFLAGS = -Woverloaded-virtual -fmessage-length=0

#  libFloatAtomicLUnstable_la_SOURCES = $(SOURCES_OPEN)
#  libFloatAtomicLUnstable_la_CPPFLAGS = -Iutils -Ialgorithms -Iapplications
#  libFloatAtomicLUnstable_la_CXXFLAGS = -Woverloaded-virtual -fmessage-length=0

#  libFloatAtomicOptimVerbose_la_SOURCES = $(SOURCES_OPEN)
#  libFloatAtomicOptimVerbose_la_CPPFLAGS = -Iutils -Ialgorithms -Iapplications
#  libFloatAtomicOptimVerbose_la_CXXFLAGS = -Woverloaded-virtual -fmessage-length=0

#  libFloatAtomicOptim_la_SOURCES = $(SOURCES_OPEN)
#  libFloatAtomicOptim_la_CPPFLAGS = -Iutils -Ialgorithms -Iapplications
#  libFloatAtomicOptim_la_CXXFLAGS = -Woverloaded-virtual -fmessage-length=0

#  libFloatAtomicVerbose_la_SOURCES = $(SOURCES_OPEN)
#  libFloatAtomicVerbose_la_CPPFLAGS = -Iutils -Ialgorithms -Iapplications
#  libFloatAtomicVerbose_la_CXXFLAGS = -Woverloaded-virtual -fmessage-length=0

#  libFloatAtomic_la_SOURCES = $(SOURCES_OPEN)
#  libFloatAtomic_la_CPPFLAGS = -Iutils -Ialgorithms -Iapplications
#  libFloatAtomic_la_CXXFLAGS = -Woverloaded-virtual -fmessage-length=0

#  libFloatThresholdLUnstableOptimVerbose_la_SOURCES = $(SOURCES_OPEN)
#  libFloatThresholdLUnstableOptimVerbose_la_CPPFLAGS = -Iutils -Ialgorithms -Iapplications
#  libFloatThresholdLUnstableOptimVerbose_la_CXXFLAGS = -Woverloaded-virtual -fmessage-length=0

#  libFloatThresholdLUnstableOptim_la_SOURCES = $(SOURCES_OPEN)
#  libFloatThresholdLUnstableOptim_la_CPPFLAGS = -Iutils -Ialgorithms -Iapplications
#  libFloatThresholdLUnstableOptim_la_CXXFLAGS = -Woverloaded-virtual -fmessage-length=0

#  libFloatThresholdLUnstableVerbose_la_SOURCES = $(SOURCES_OPEN)
#  libFloatThresholdLUnstableVerbose_la_CPPFLAGS = -Iutils -Ialgorithms -Iapplications
#  libFloatThresholdLUnstableVerbose_la_CXXFLAGS = -Woverloaded-virtual -fmessage-length=0

#  libFloatThresholdLUnstable_la_SOURCES = $(SOURCES_OPEN)
#  libFloatThresholdLUnstable_la_CPPFLAGS = -Iutils -Ialgorithms -Iapplications
#  libFloatThresholdLUnstable_la_CXXFLAGS = -Woverloaded-virtual -fmessage-length=0

#  libFloatThresholdOptimVerbose_la_SOURCES = $(SOURCES_OPEN)
#  libFloatThresholdOptimVerbose_la_CPPFLAGS = -Iutils -Ialgorithms -Iapplications
#  libFloatThresholdOptimVerbose_la_CXXFLAGS = -Woverloaded-virtual -fmessage-length=0

#  libFloatThresholdOptim_la_SOURCES = $(SOURCES_OPEN)
#  libFloatThresholdOptim_la_CPPFLAGS = -Iutils -Ialgorithms -Iapplications
#  libFloatThresholdOptim_la_CXXFLAGS = -Woverloaded-virtual -fmessage-length=0

#  libFloatThresholdVerbose_la_SOURCES = $(SOURCES_OPEN)
#  libFloatThresholdVerbose_la_CPPFLAGS = -Iutils -Ialgorithms -Iapplications
#  libFloatThresholdVerbose_la_CXXFLAGS = -Woverloaded-virtual -fmessage-length=0

#  libFloatThreshold_la_SOURCES = $(SOURCES_OPEN)
#  libFloatThreshold_la_CPPFLAGS = -Iutils -Ialgorithms -Iapplications
#  libFloatThreshold_la_CXXFLAGS = -Woverloaded-virtual -fmessage-length=0

#  libFloatLUnstableOptimVerbose_la_SOURCES = $(SOURCES_OPEN)
#  libFloatLUnstableOptimVerbose_la_CPPFLAGS = -Iutils -Ialgorithms -Iapplications
#  libFloatLUnstableOptimVerbose_la_CXXFLAGS = -Woverloaded-virtual -fmessage-length=0

#  libFloatLUnstableOptim_la_SOURCES = $(SOURCES_OPEN)
#  libFloatLUnstableOptim_la_CPPFLAGS = -Iutils -Ialgorithms -Iapplications
#  libFloatLUnstableOptim_la_CXXFLAGS = -Woverloaded-virtual -fmessage-length=0

#  libFloatLUnstableVerbose_la_SOURCES = $(SOURCES_OPEN)
#  libFloatLUnstableVerbose_la_CPPFLAGS = -Iutils -Ialgorithms -Iapplications
#  libFloatLUnstableVerbose_la_CXXFLAGS = -Woverloaded-virtual -fmessage-length=0

#  libFloatLUnstable_la_SOURCES = $(SOURCES_OPEN)
#  libFloatLUnstable_la_CPPFLAGS = -Iutils -Ialgorithms -Iapplications
#  libFloatLUnstable_la_CXXFLAGS = -Woverloaded-virtual -fmessage-length=0

#  libFloatOptimVerbose_la_SOURCES = $(SOURCES_OPEN)
#  libFloatOptimVerbose_la_CPPFLAGS = -Iutils -Ialgorithms -Iapplications
#  libFloatOptimVerbose_la_CXXFLAGS = -Woverloaded-virtual -fmessage-length=0

#  libFloatOptim_la_SOURCES = $(SOURCES_OPEN)
#  libFloatOptim_la_CPPFLAGS = -Iutils -Ialgorithms -Iapplications
#  libFloatOptim_la_CXXFLAGS = -Woverloaded-virtual -fmessage-length=0

#  libFloatVerbose_la_SOURCES = $(SOURCES_OPEN)
#  libFloatVerbose_la_CPPFLAGS = -Iutils -Ialgorithms -Iapplications
#  libFloatVerbose_la_CXXFLAGS = -Woverloaded-virtual -fmessage-length=0

#  libFloat_la_SOURCES = $(SOURCES_OPEN)
#  libFloat_la_CPPFLAGS = -Iutils -Ialgorithms -Iapplications
#  libFloat_la_CXXFLAGS = -Woverloaded-virtual -fmessage-length=0
