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

extern "C" {
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
}

#include "php_log4cxx.h"
#include <log4cxx/logger.h>
#include <log4cxx/propertyconfigurator.h>
#include <log4cxx/basicconfigurator.h>
#include <log4cxx/patternlayout.h>
#include <log4cxx/consoleappender.h>
#include <log4cxx/logmanager.h>

using namespace std;
using namespace log4cxx;
using namespace log4cxx::helpers;


ZEND_DECLARE_MODULE_GLOBALS(log4cxx)



static zend_class_entry *log4cxx_basicconfigurator_ce;
static zend_object_handlers log4cxx_basicconfigurator_handlers;

static zend_class_entry *log4cxx_propertyconfigurator_ce;
static zend_object_handlers log4cxx_propertyconfigurator_handlers;

static zend_class_entry *log4cxx_logmanager_ce;
static zend_object_handlers log4cxx_logmanager_handlers;


static zend_class_entry *logger_ce;
static zend_object_handlers logger_handlers;

typedef struct _logger_object {

	zend_object obj;
	LoggerPtr logger;

} logger_object;

#define HELLO_FETCH_OBJECT(zobj) (logger_object *)zend_object_store_get_object((zobj) TSRMLS_CC)



/* True global resources - no need for thread safety here */
static int le_log4cxx;

/* {{{ log4cxx_functions[]
 *
 * Every user visible function must have an entry in log4cxx_functions[].
 */
const zend_function_entry log4cxx_functions[] = {
	PHP_FE_END	/* Must be the last line in log4cxx_functions[] */
};
/* }}} */

/* {{{ log4cxx_module_entry
 */
zend_module_entry log4cxx_module_entry = {
#if ZEND_MODULE_API_NO >= 20010901
	STANDARD_MODULE_HEADER,
#endif
	"log4cxx",
	log4cxx_functions,
	PHP_MINIT(log4cxx),
	PHP_MSHUTDOWN(log4cxx),
	PHP_RINIT(log4cxx),		/* Replace with NULL if there's nothing to do at request start */
	PHP_RSHUTDOWN(log4cxx),	/* Replace with NULL if there's nothing to do at request end */
	PHP_MINFO(log4cxx),
#if ZEND_MODULE_API_NO >= 20010901
	PHP_LOG4CXX_VERSION,
#endif
	STANDARD_MODULE_PROPERTIES
};
/* }}} */

#ifdef COMPILE_DL_LOG4CXX
BEGIN_EXTERN_C()
ZEND_GET_MODULE(log4cxx)
END_EXTERN_C()
#endif


PHP_INI_BEGIN()
    STD_PHP_INI_ENTRY("log4cxx.property_file",      "", PHP_INI_ALL, OnUpdateString, property_file, zend_log4cxx_globals, log4cxx_globals)
PHP_INI_END()


static void log4cxx_load_property_configure(char *property_file) {
	log4cxx::PropertyConfigurator::configure(property_file);
}

static void log4cxx_load_basic_configure(char *pattern) {
	PatternLayoutPtr myLayoutPtr = new PatternLayout();
	ConsoleAppenderPtr myAppenderPtr = new ConsoleAppender(myLayoutPtr);
	myLayoutPtr->setConversionPattern(pattern);
	log4cxx::BasicConfigurator::configure(myAppenderPtr);
}

static void log4cxx_default_basic_configure() {
	log4cxx_load_basic_configure("%d %p [%t] (%F:%L) - %m%n");
}

static char* log4cxx_get_current_function_name() {
	const char *space = "";
	const char *class_name = get_active_class_name(&space TSRMLS_C);
	const char *function_name = get_active_function_name(TSRMLS_C);

	if (!class_name) {
		class_name = "";
	}

	if (!function_name) {
		function_name = "";
	}

	char buf[sizeof(space) + sizeof(class_name) + sizeof(function_name) + 4];
	char *ret = buf;
	sprintf(ret, "%s::%s::%s", space, class_name, function_name);
	return ret;
}

static log4cxx::spi::LocationInfo log4cxx_get_current_location() {

	const char *file_name = zend_get_executed_filename(TSRMLS_C);
	unsigned int lineno = zend_get_executed_lineno(TSRMLS_C);
	char *function_name = log4cxx_get_current_function_name();
	log4cxx::spi::LocationInfo location(file_name, function_name, lineno);

	return location;
}


ZEND_BEGIN_ARG_INFO(Logger___construct_arginfo, 0)
	ZEND_ARG_INFO(0, name)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(Logger_info_arginfo, 0)
	ZEND_ARG_INFO(0, message)
ZEND_END_ARG_INFO()

static PHP_METHOD(Log4cxx_Logger, __construct) {
	logger_object *objval =  HELLO_FETCH_OBJECT(getThis());
	char *name;
    int name_len;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &name, &name_len) == FAILURE) {
        RETURN_NULL();
    }

    // if (objval->logger != NULL) {
    // 	php_error_docref(NULL TSRMLS_CC, E_ERROR, "can not init logger.");
    // 	RETURN_NULL();
    // }

    //log4cxx_default_basic_configure();

    string name_str(name, name_len);
    LoggerPtr logger = Logger::getLogger(name_str);

    //设置中文编码
	//setlocale(LC_ALL, "zh_CN.UTF-8"); 

	objval->logger = logger;
}

static PHP_METHOD(Log4cxx_Logger, info) {
	logger_object *objval =  HELLO_FETCH_OBJECT(getThis());

	char *message;
	int message_len;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &message, &message_len) == FAILURE) {
        return;
    }

    LoggerPtr logger = objval->logger;

    if (logger && logger->isInfoEnabled()) {
    	 ::log4cxx::helpers::MessageBuffer oss_;
    	 logger->forcedLog(::log4cxx::Level::getInfo(), oss_.str(oss_ << message), log4cxx_get_current_location());
    }
}

static PHP_METHOD(Log4cxx_Logger, warn) {
	logger_object *objval =  HELLO_FETCH_OBJECT(getThis());

	char *message;
	int message_len;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &message, &message_len) == FAILURE) {
        return;
    }

    LoggerPtr logger = objval->logger;

    if (logger && logger->isWarnEnabled()) {
    	 ::log4cxx::helpers::MessageBuffer oss_;
    	 logger->forcedLog(::log4cxx::Level::getWarn(), oss_.str(oss_ << message), log4cxx_get_current_location());
    }
}

static PHP_METHOD(Log4cxx_Logger, debug) {
	logger_object *objval =  HELLO_FETCH_OBJECT(getThis());

	char *message;
	int message_len;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &message, &message_len) == FAILURE) {
        return;
    }

    LoggerPtr logger = objval->logger;

    if (LOG4CXX_UNLIKELY(logger && logger->isDebugEnabled())) {
    	 ::log4cxx::helpers::MessageBuffer oss_;
    	 logger->forcedLog(::log4cxx::Level::getDebug(), oss_.str(oss_ << message), log4cxx_get_current_location());
    }
}

static PHP_METHOD(Log4cxx_Logger, error) {
	logger_object *objval =  HELLO_FETCH_OBJECT(getThis());

	char *message;
	int message_len;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &message, &message_len) == FAILURE) {
        return;
    }

    LoggerPtr logger = objval->logger;

    if (logger && logger->isErrorEnabled()) {
    	 ::log4cxx::helpers::MessageBuffer oss_;
    	 logger->forcedLog(::log4cxx::Level::getError(), oss_.str(oss_ << message), log4cxx_get_current_location());
    }
}

static PHP_METHOD(Log4cxx_Logger, trace) {
	logger_object *objval =  HELLO_FETCH_OBJECT(getThis());

	char *message;
	int message_len;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &message, &message_len) == FAILURE) {
        return;
    }

    LoggerPtr logger = objval->logger;

    if (LOG4CXX_UNLIKELY(logger && logger->isTraceEnabled())) {
    	 ::log4cxx::helpers::MessageBuffer oss_;
    	 logger->forcedLog(::log4cxx::Level::getTrace(), oss_.str(oss_ << message), log4cxx_get_current_location());
    }
}

static zend_function_entry logger_methods[] = {
	PHP_ME(Log4cxx_Logger, __construct, Logger___construct_arginfo, ZEND_ACC_PUBLIC | ZEND_ACC_CTOR)
	PHP_ME(Log4cxx_Logger, info, 		Logger_info_arginfo, 		ZEND_ACC_PUBLIC)
	PHP_ME(Log4cxx_Logger, warn, 		Logger_info_arginfo, 		ZEND_ACC_PUBLIC)
	PHP_ME(Log4cxx_Logger, debug, 		Logger_info_arginfo, 		ZEND_ACC_PUBLIC)
	PHP_ME(Log4cxx_Logger, error, 		Logger_info_arginfo, 		ZEND_ACC_PUBLIC)
	PHP_ME(Log4cxx_Logger, trace, 		Logger_info_arginfo, 		ZEND_ACC_PUBLIC)
	{NULL, NULL, NULL}
};

void logger_dtor(void *object TSRMLS_DC) {
	logger_object *objval = (logger_object *)object;
    zend_object_std_dtor(&(objval->obj) TSRMLS_CC);
    efree(objval);
}


static zend_object_value logger_ctor(zend_class_entry *ce TSRMLS_DC) {
	logger_object *objval = (logger_object *)emalloc(sizeof(logger_object));
	memset(objval, 0, sizeof(logger_object));

	zend_object_value retval;
	zend_object_std_init(&(objval->obj), ce TSRMLS_CC);

	retval.handle = zend_objects_store_put(objval , 
                                            NULL, 
                                            (zend_objects_free_object_storage_t)logger_dtor, 
                                            NULL TSRMLS_CC);
    retval.handlers = &logger_handlers;

    return retval;
}

ZEND_BEGIN_ARG_INFO(basic_configurator_arginfo, 0)
	ZEND_ARG_INFO(0, pattern)
ZEND_END_ARG_INFO()
static PHP_METHOD(Log4cxx_BasicConfigurator, configure) {
	char *pattern;
	int pattern_len;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &pattern, &pattern_len) == FAILURE) {
        return;
    }
   	log4cxx_load_basic_configure(pattern);
}

static PHP_METHOD(Log4cxx_BasicConfigurator, defaultConfigure) {
	log4cxx_default_basic_configure();
}

static zend_function_entry log4cxx_basicconfigurator_methods[] = {
	PHP_ME(Log4cxx_BasicConfigurator, configure, basic_configurator_arginfo, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)
	PHP_ME(Log4cxx_BasicConfigurator, defaultConfigure, NULL, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)
	{NULL, NULL, NULL}
};


ZEND_BEGIN_ARG_INFO(property_configurator_arginfo, 0)
	ZEND_ARG_INFO(0, property_file)
ZEND_END_ARG_INFO()
static PHP_METHOD(Log4cxx_PropertyConfigurator, configure) {
	char *file;
	int file_len;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &file, &file_len) == FAILURE) {
        return;
    }
   	log4cxx_load_property_configure(file);
}

static zend_function_entry log4cxx_propertyconfigurator_methods[] = {
	PHP_ME(Log4cxx_PropertyConfigurator, configure, property_configurator_arginfo, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)
	{NULL, NULL, NULL}
};



ZEND_BEGIN_ARG_INFO(logmanager_getlogger_arginfo, 0)
	ZEND_ARG_INFO(0, name)
ZEND_END_ARG_INFO()
static PHP_METHOD(Log4cxx_LogManager, getLogger) {
	char *name;
	int name_len;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &name, &name_len) == FAILURE) {
        RETURN_NULL();
    }

    logger_object *objval =  HELLO_FETCH_OBJECT(getThis());
    // if (objval->logger != NULL) {
    // 	php_error_docref(NULL TSRMLS_CC, E_ERROR, "can not init logger.");
    // 	RETURN_NULL();
    // }

    //return object
    if (object_init_ex(return_value, logger_ce) != SUCCESS) {
    	RETURN_NULL();
    }

    string name_str(name, name_len);
    objval->logger = LogManager::getLogger(name_str);
}

static zend_function_entry log4cxx_logmanager_methods[] = {
	PHP_ME(Log4cxx_LogManager, getLogger, logmanager_getlogger_arginfo, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)
	{NULL, NULL, NULL}
};


/* {{{ PHP_MINIT_FUNCTION
 */
PHP_MINIT_FUNCTION(log4cxx)
{
	REGISTER_INI_ENTRIES();

	zend_class_entry ce;

    INIT_CLASS_ENTRY(ce, "Log4cxx_Logger", logger_methods)
    logger_ce = zend_register_internal_class(&ce TSRMLS_CC);
    logger_ce->create_object = logger_ctor;
    memcpy(&logger_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));
    logger_handlers.clone_obj = NULL;
	
    INIT_CLASS_ENTRY(ce, "Log4cxx_BasicConfigurator", log4cxx_basicconfigurator_methods);
    log4cxx_basicconfigurator_ce = zend_register_internal_class(&ce TSRMLS_CC);
    memcpy(&log4cxx_basicconfigurator_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));
    log4cxx_basicconfigurator_handlers.clone_obj = NULL;

    INIT_CLASS_ENTRY(ce, "Log4cxx_PropertyConfigurator", log4cxx_propertyconfigurator_methods);
    log4cxx_propertyconfigurator_ce = zend_register_internal_class(&ce TSRMLS_CC);
    memcpy(&log4cxx_propertyconfigurator_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));
    log4cxx_propertyconfigurator_handlers.clone_obj = NULL;

    INIT_CLASS_ENTRY(ce, "Log4cxx_LogManager", log4cxx_logmanager_methods);
    log4cxx_logmanager_ce = zend_register_internal_class(&ce TSRMLS_CC);
    memcpy(&log4cxx_logmanager_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));
    log4cxx_logmanager_handlers.clone_obj = NULL;


    char *property_file = LOG4CXX_G(property_file);
    if (strlen(property_file) > 0) {
    	log4cxx_load_property_configure(property_file);
    }
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MSHUTDOWN_FUNCTION
 */
PHP_MSHUTDOWN_FUNCTION(log4cxx)
{
	UNREGISTER_INI_ENTRIES();
	return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request start */
/* {{{ PHP_RINIT_FUNCTION
 */
PHP_RINIT_FUNCTION(log4cxx)
{
	return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request end */
/* {{{ PHP_RSHUTDOWN_FUNCTION
 */
PHP_RSHUTDOWN_FUNCTION(log4cxx)
{
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MINFO_FUNCTION
 */
PHP_MINFO_FUNCTION(log4cxx)
{
	php_info_print_table_start();
	php_info_print_table_header(2, "log4cxx support", "enabled");
	php_info_print_table_end();

	DISPLAY_INI_ENTRIES();
}
/* }}} */





/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
