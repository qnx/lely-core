bin =

LELY_COMPAT_LIBS =
if CODE_COVERAGE_ENABLED
LELY_COMPAT_LIBS += $(CODE_COVERAGE_LIBS)
endif CODE_COVERAGE_ENABLED

LELY_COMPAT_LIBS += $(top_builddir)/lib/compat/liblely-compat.la

if PLATFORM_WIN32
CPPUTEST_LIBS += -lpthread
endif PLATFORM_WIN32

LELY_UNIT_TEST_CAN_COMMON_LIBS =
LELY_UNIT_TEST_CAN_COMMON_LIBS += $(LELY_CAN_LIBS)
LELY_UNIT_TEST_CAN_COMMON_LIBS += $(LELY_UTIL_LIBS)
LELY_UNIT_TEST_CAN_COMMON_LIBS += $(LELY_COMPAT_LIBS)
LELY_UNIT_TEST_CAN_COMMON_LIBS += $(CPPUTEST_LIBS)

src_common =
src_common += tests-main.cpp

# CAN library tests

LELY_UTIL_LIBS = $(top_builddir)/lib/util/liblely-util.la
LELY_CAN_LIBS = $(top_builddir)/lib/can/liblely-can.la

bin += unit-test-can-buf
unit_test_can_buf_SOURCES = $(src_common) test-buf.cpp
unit_test_can_buf_LDADD = $(LELY_UNIT_TEST_CAN_COMMON_LIBS)

bin += unit-test-can-msg
unit_test_can_msg_SOURCES = $(src_common) test-msg.cpp
unit_test_can_msg_LDADD = $(LELY_UNIT_TEST_CAN_COMMON_LIBS)

bin += unit-test-can-net
unit_test_can_net_SOURCES = $(src_common) test-net.cpp
unit_test_can_net_LDADD = $(LELY_UNIT_TEST_CAN_COMMON_LIBS)

# Compilation flags

AM_CPPFLAGS = -I$(top_srcdir)/include -I$(top_srcdir)/unit-tests
AM_CPPFLAGS += -DTEST_SRCDIR=\"${srcdir}\"
if ENABLE_SHARED
AM_CPPFLAGS += -DLELY_ENABLE_SHARED=1
endif ENABLE_SHARED
AM_CPPFLAGS += $(CPPUTEST_CFLAGS)

if NO_HOSTED
# even in no-hosted unit-tests are compiled with libc
AM_CPPFLAGS += -DLELY_HAVE_TIMESPEC -DLELY_HAVE_ITIMERSPEC
endif NO_HOSTED

if CODE_COVERAGE_ENABLED
AM_CPPFLAGS += $(CODE_COVERAGE_CPPFLAGS)
endif CODE_COVERAGE_ENABLED

AM_CFLAGS =
if CODE_COVERAGE_ENABLED
AM_CFLAGS += $(CODE_COVERAGE_CFLAGS)
endif CODE_COVERAGE_ENABLED

AM_CXXFLAGS =
AM_CXXFLAGS += -fno-exceptions
if CODE_COVERAGE_ENABLED
AM_CXXFLAGS += $(CODE_COVERAGE_CXXFLAGS)
endif CODE_COVERAGE_ENABLED

AM_LDFLAGS =
if PLATFORM_WIN32
AM_LDFLAGS += -all-static
endif PLATFORM_WIN32

check_PROGRAMS = $(bin)

TESTS = $(check_PROGRAMS)
EXEC = $(SHELL) $(top_builddir)/exec-wrapper.sh

AM_TESTS_ENVIRONMENT = TEST_ARGS="-ojunit -v"; export TEST_ARGS;

if CODE_COVERAGE_ENABLED
LOG_COMPILER = $(SHELL) $(top_builddir)/lcov-wrapper.sh
AM_TESTS_ENVIRONMENT += EXEC_WRAPPER="$(EXEC)"; export EXEC_WRAPPER;
@CODE_COVERAGE_RULES@
else !CODE_COVERAGE_ENABLED
LOG_COMPILER = $(EXEC)
endif !CODE_COVERAGE_ENABLED

@VALGRIND_CHECK_RULES@
