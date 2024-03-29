#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([violas-sdk], [1.0], [sht@pallimus.org])
AC_CONFIG_SRCDIR([config.h.in])
AC_CONFIG_HEADERS([config.h])

# Checks for programs.
AC_PROG_CXX
AC_PROG_AWK
AC_PROG_CC
AC_PROG_CPP
AC_PROG_INSTALL
AC_PROG_LN_S
AC_PROG_MAKE_SET
AC_PROG_RANLIB
AM_PROG_AR

# Checks for libraries.
# FIXME: Replace `main' with a function in `-lboost_filesystem':
#AC_CHECK_LIB([boost_filesystem], [main])
# FIXME: Replace `main' with a function in `-lboost_system':
#AC_CHECK_LIB([boost_system], [main])
# FIXME: Replace `main' with a function in `-lclient_proxy':
AC_CHECK_LIB([client_proxy], [main])
# FIXME: Replace `main' with a function in `-lcppSdk':
AC_CHECK_LIB([cppSdk], [main])
# FIXME: Replace `main' with a function in `-ldl':
AC_CHECK_LIB([dl], [main])
# FIXME: Replace `main' with a function in `-llog':
AC_CHECK_LIB([log], [main])
# FIXME: Replace `main' with a function in `-lpthread':
AC_CHECK_LIB([pthread], [main])

# Checks for header files.

# Checks for typedefs, structures, and compiler characteristics.
AC_CHECK_HEADER_STDBOOL
AC_TYPE_INT64_T
AC_TYPE_SIZE_T
AC_TYPE_UINT16_T
AC_TYPE_UINT32_T
AC_TYPE_UINT64_T
AC_TYPE_UINT8_T

# Checks for library functions.

# check if debug is enable and set condition DEBUG with it
AC_ARG_ENABLE([debug],
    [ --enable-debug Turn on debugging],
    [case "${enableval}" in
        yes) debug=true ;;
        no) debug=false ;;
        *) AC_MSG_ERROR([bad value ${enableval} for --enable-debug]) ;;
    esac],
    [debug=false])
    
AM_CONDITIONAL([DEBUG], [test x$debug = xtrue])
AM_CONDITIONAL([ANDROID], [test x$android = xtrue])

AM_INIT_AUTOMAKE([-Wall -Werror foreign])
AC_CONFIG_MACRO_DIR([m4])
AM_SILENT_RULES([yes])
LT_INIT

AC_CONFIG_FILES([Makefile
                cpp/src/Makefile
                cpp/violas_cli/src/Makefile               
                cpp/testnet/src/Makefile
                java/src/jni/Makefile])
AC_OUTPUT