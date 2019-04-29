AC_PREREQ([2.69])
AC_INIT([render_mbtiles], [1.0], [pedro.vicente@space-research.org])
AM_INIT_AUTOMAKE([foreign])
AM_SILENT_RULES([no])
AC_PROG_CXX
AC_PROG_CC

WX_CONFIG=wx-config
AC_ARG_WITH([wx-config],
            [AS_HELP_STRING([--with-wx-config=PATH],[Use the given PATH to wx-config])],
            [
              if test "$withval" != "yes" -a "$withval" != ""; then
              WX_CONFIG=$withval
              fi
            ],
            [])
AC_MSG_CHECKING([wxWidgets version])
if wxversion=`$WX_CONFIG --version`; then
    AC_MSG_RESULT([$wxversion])
else
    AC_MSG_ERROR([wxWidgets not found. Try --with-wx-config.])
fi
WX_CPPFLAGS="`$WX_CONFIG --cppflags`"
WX_CXXFLAGS="`$WX_CONFIG --cxxflags`"
WX_LIBS="`$WX_CONFIG --libs` -ldl"


CPPFLAGS="$CPPFLAGS $WX_CPPFLAGS"
CXXFLAGS="$CXXFLAGS $WX_CXXFLAGS -std=c++0x"
EXPLORER_LIBS="$WX_LIBS"
AC_SUBST(EXPLORER_LIBS)
AC_CONFIG_FILES([Makefile])
AC_OUTPUT
