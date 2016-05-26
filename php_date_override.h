
#ifndef PHP_DATE_OVERRIDE_H
#define PHP_DATE_OVERRIDE_H

#define DATE_OVERRIDE_EXT_VERSION "1.0.0"

extern zend_module_entry date_override_module_entry;

PHP_MINIT_FUNCTION(date_override);
PHP_RINIT_FUNCTION(date_override);
PHP_RSHUTDOWN_FUNCTION(date_override);
PHP_MINFO_FUNCTION(date_override);

PHP_FUNCTION(date_override);
PHP_FUNCTION(date_override_date_create);

PHP_METHOD(DateOverride, __construct);

#endif /* PHP_OVERRIDE_H */
