/*
  +----------------------------------------------------------------------+
  | PHP Version 5                                                        |
  +----------------------------------------------------------------------+
  | Copyright (c) 1997-2015 The PHP Group                                |
  +----------------------------------------------------------------------+
  | This source file is subject to version 3.01 of the PHP license,      |
  | that is bundled with this package in the file LICENSE, and is        |
  | available through the world-wide-web at the following url:           |
  | http://www.php.net/license/3_01.txt                                  |
  | If you did not receive a copy of the PHP license and are unable to   |
  | obtain it through the world-wide-web, please send a note to          |
  | license@php.net so we can mail you a copy immediately.               |
  +----------------------------------------------------------------------+
  | Author:                                                              |
  +----------------------------------------------------------------------+
*/

/* $Id$ */

#ifndef PHP_LOG4CXX_H
#define PHP_LOG4CXX_H

extern zend_module_entry log4cxx_module_entry;
#define phpext_log4cxx_ptr &log4cxx_module_entry

#define PHP_LOG4CXX_VERSION "0.1.0" /* Replace with version number for your extension */

#ifdef PHP_WIN32
#	define PHP_LOG4CXX_API __declspec(dllexport)
#elif defined(__GNUC__) && __GNUC__ >= 4
#	define PHP_LOG4CXX_API __attribute__ ((visibility("default")))
#else
#	define PHP_LOG4CXX_API
#endif

extern "C" {
#ifdef ZTS
#include "TSRM.h"
#endif
}


PHP_MINIT_FUNCTION(log4cxx);
PHP_MSHUTDOWN_FUNCTION(log4cxx);
PHP_RINIT_FUNCTION(log4cxx);
PHP_RSHUTDOWN_FUNCTION(log4cxx);
PHP_MINFO_FUNCTION(log4cxx);

ZEND_BEGIN_MODULE_GLOBALS(log4cxx)
  char *init_properties_file;
ZEND_END_MODULE_GLOBALS(log4cxx)

/* In every utility function you add that needs to use variables 
   in php_log4cxx_globals, call TSRMLS_FETCH(); after declaring other 
   variables used by that function, or better yet, pass in TSRMLS_CC
   after the last function argument and declare your utility function
   with TSRMLS_DC after the last declared argument.  Always refer to
   the globals in your function as LOG4CXX_G(variable).  You are 
   encouraged to rename these macros something shorter, see
   examples in any other php module directory.
*/

#ifdef ZTS
#define LOG4CXX_G(v) TSRMG(log4cxx_globals_id, zend_log4cxx_globals *, v)
#else
#define LOG4CXX_G(v) (log4cxx_globals.v)
#endif

#endif	/* PHP_LOG4CXX_H */


/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
