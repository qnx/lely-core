bin =

if PLATFORM_WIN32
CPPUTEST_LIBS += -lpthread
endif

# CppUTest library self-test

bin += test-cpputest
test_cpputest_SOURCES = test-cpputest.cpp
test_cpputest_LDADD = $(CPPUTEST_LIBS)

# Compilation flags

AM_CPPFLAGS = -I$(top_srcdir)/include
AM_CPPFLAGS += -DTEST_SRCDIR=\"${srcdir}\"
if ENABLE_SHARED
AM_CPPFLAGS += -DLELY_ENABLE_SHARED=1
endif
AM_CPPFLAGS += $(CPPUTEST_CFLAGS)

if CODE_COVERAGE_ENABLED
AM_CPPFLAGS += $(CODE_COVERAGE_CPPFLAGS)
endif

AM_CFLAGS =
if CODE_COVERAGE_ENABLED
AM_CFLAGS += $(CODE_COVERAGE_CFLAGS)
endif

AM_CXXFLAGS =
if CODE_COVERAGE_ENABLED
AM_CXXFLAGS += $(CODE_COVERAGE_CXXFLAGS)
endif

AM_LDFLAGS =
if PLATFORM_WIN32
AM_LDFLAGS += -all-static
endif

check_PROGRAMS = $(bin)

TESTS = $(check_PROGRAMS)
EXEC = $(SHELL) $(top_builddir)/exec-wrapper.sh

AM_TESTS_ENVIRONMENT = TEST_ARGS="-ojunit -v"; export TEST_ARGS;

if CODE_COVERAGE_ENABLED
LOG_COMPILER = $(SHELL) $(top_builddir)/lcov-wrapper.sh
AM_TESTS_ENVIRONMENT += EXEC_WRAPPER="$(EXEC)"; export EXEC_WRAPPER;
@CODE_COVERAGE_RULES@
else
LOG_COMPILER = $(EXEC)
endif

@VALGRIND_CHECK_RULES@
