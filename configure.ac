#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.59])
AC_INIT([lexstat], [1.0])
AC_CONFIG_SRCDIR([src/lexstat.h])
AM_INIT_AUTOMAKE()

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC
AC_LIBTOOL_WIN32_DLL
AC_PROG_LIBTOOL

AC_CHECK_LIB([curl],[curl_easy_init],[],[
    AC_MSG_ERROR(["Error! You need to have libcurl installed!"])])
AC_CHECK_LIB([qdbm],[vlopen],[],[
    AC_MSG_ERROR(["QDBM library is required!"]) ])
# PKG_CHECK_MODULES([libxml],[libxml-2.0],[],[
#     AC_MSG_ERROR(["Error! You need to have libxml2 installed!"])])

# Checks for libraries.
PKG_CHECK_EXISTS([libxml-2.0], [], [
    AC_MSG_ERROR(["Error! You need to have libglib2.0 installed."])
    ])

# PKG_CHECK_EXISTS([cppunit], [], [
#     AC_MSG_ERROR(["Error! You need to have libcppunit-dev installed. Try sudo apt-get install libcppunit-dev."])
#     ])

# Checks for header files.
AC_CHECK_HEADERS([stdlib.h stdio.h])
AC_CHECK_HEADERS([malloc.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL
AC_TYPE_SIZE_T

AC_ARG_WITH(debug,
    [AS_HELP_STRING([--with-debug], [enable debugging output])],
    [
        if test $withval != "no"; then
            AC_SUBST(CFLAGS, ["-g3 -O0 -Wall"])
            AC_SUBST(CXXFLAGS, ["-g3 -O0 -Wall"])
            AC_SUBST(CPPFLAGS, ["-g3 -O0 -Wall"])
            AC_DEFINE([WITH_DEBUG],1)
        fi
    ])

AC_ARG_WITH(conf,
    [AS_HELP_STRING([--with-conf], [set sysconfig path where *.conf is locate.])],
    [
        if test $withval != "no"; then
            AC_SUBST(sysconfig_dir, $withval)
        fi
    ])
AS_IF([test "x$sysconfig_dir" = "x"],
        [
            sysconfig_dir="/etc"
            AC_SUBST(sysconfig_dir)
    ])

# AC_ARG_WITH(data,
#     [AS_HELP_STRING([--with-data], [set data path where model/dict file is locate.])],
#     [
#         if test $withval != "no"; then
#             AC_SUBST(data_dir, $withval)
#         fi
#     ])
# AS_IF([test "x$data_dir" = "x"],
#         [
#             data_dir="/usr"
#             AC_SUBST(data_dir)
#     ])

sysconfig_dir="$sysconfig_dir/lexstat"
AC_SUBST(sysconfig_dir)

# data_dir="$data_dir/lexstat"
# AC_SUBST(data_dir)

LIBXML2_LIBS="`pkg-config --libs libxml-2.0`"
LIBXML2_CFLAGS="`pkg-config --cflags libxml-2.0`"
LIBXML2_CXXFLAGS="`pkg-config --cflags libxml-2.0`"
LIBXML2_CPPFLAGS="`pkg-config --cflags libxml-2.0`"

AC_SUBST(LIBXML2_LIBS)
AC_SUBST(LIBXML2_CFLAGS)
AC_SUBST(LIBXML2_CXXFLAGS)
AC_SUBST(LIBXML2_CPPFLAGS)

AC_SUBST(CFLAGS)
AC_SUBST(CPPFLAGS)
AC_SUBST(CXXFLAGS)
AC_SUBST(AM_LDFLAGS)
AC_SUBST(INSTALL_DATA, "${INSTALL} -m 777")
AC_SUBST(MKDIR_P, "/bin/mkdir -p -m 777")

# Checks for library functions.
AC_FUNC_MALLOC
AC_CHECK_FUNCS([memset])
AC_CHECK_FUNCS([strchr])
AC_CHECK_FUNCS([strstr])
AC_C_CONST

AC_DEFINE(_LARGEFILE64_SOURCE)
AC_DEFINE(_XOPEN_SOURCE, [500])
AC_DEFINE(_FILE_OFFSET_BITS, [64])
AC_DEFINE(_GNU_SOURCE)

AC_CONFIG_FILES([Makefile
                 src/Makefile
                 data/Makefile
                 cgi/Makefile])

AC_OUTPUT

