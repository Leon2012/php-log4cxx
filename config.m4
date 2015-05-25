PHP_ARG_WITH(log4cxx, for log4cxx support,
[  --with-log4cxx[=DIR]              Include log4cxx support])

if test "$PHP_LOG4CXX" != "no"; then
  PHP_REQUIRE_CXX   # c++ support

  SEARCH_PATH="/usr/local /usr"     
  SEARCH_FOR="/include/log4cxx/logger.h"  
  if test -r $PHP_LOG4CXX/$SEARCH_FOR; then 
    LOG4CXX_DIR=$PHP_LOG4CXX
  else 
    AC_MSG_CHECKING([for log4cxx files in default path])
    for i in $SEARCH_PATH ; do
      if test -r $i/$SEARCH_FOR; then
        LOG4CXX_DIR=$i
        AC_MSG_RESULT(found in $i)
      fi
    done
  fi
  
  if test -z "$LOG4CXX_DIR"; then
    AC_MSG_RESULT([not found])
    AC_MSG_ERROR([Please reinstall the log4cxx distribution])
  fi

  PHP_ADD_INCLUDE($LOG4CXX_DIR/include)  # -I/usr/local/include

  LIBNAME=log4cxx 
  LIBSYMBOL=_ZN7log4cxx17BasicConfigurator9configureEv  

  PHP_CHECK_LIBRARY($LIBNAME,$LIBSYMBOL,
  [
    PHP_ADD_LIBRARY_WITH_PATH($LIBNAME, $LOG4CXX_DIR/$PHP_LIBDIR, LOG4CXX_SHARED_LIBADD)  # -L/usr/local/lib
    AC_DEFINE(HAVE_LOG4CXXLIB,1,[ ])
  ],[
    AC_MSG_ERROR([wrong log4cxx lib version or lib not found])
  ],[
    -L$LOG4CXX_DIR/$PHP_LIBDIR -llog4cxx  # -L/usr/local/lib -llog4cxx
  ])
  
  PHP_ADD_LIBRARY(stdc++, "", EXTRA_LDFLAGS)
  PHP_SUBST(LOG4CXX_SHARED_LIBADD)
  CPPFILE="log4cxx.cpp"
  PHP_NEW_EXTENSION(log4cxx, $CPPFILE, $ext_shared)
fi
