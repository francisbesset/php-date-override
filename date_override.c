
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_date_override.h"
#include "ext/date/php_date.h"
#include "ext/standard/info.h"


static int php_date_override_date_initialize(zval *datetime_object, char *time_str, int time_str_len, zval *timezone_object, int ctor TSRMLS_DC);
static int php_date_override_is_full_date(char *time_str, int time_str_len TSRMLS_DC);
static int php_date_override_get_tz(char **tz, char *time_str, int time_str_len TSRMLS_DC);
static void php_date_override_set_timezone(zval *datetime_object, zval *timezone_object TSRMLS_DC);

static int php_date_override_is_enabled(TSRMLS_D);
static void php_date_override_reset(TSRMLS_D);

static void replace_function(char *dfunc_str, char *sfunc_str TSRMLS_DC);
static void replace_method(char *dclass_str, char *sclass_str, char *method_str TSRMLS_DC);


static const zend_module_dep date_override_deps[] = {
  ZEND_MOD_REQUIRED("date")
  ZEND_MOD_END
};

zend_function_entry date_override_functions[] = {
  PHP_FE(date_override, NULL)
  PHP_FE(date_override_date_create, NULL)
  PHP_FE_END
};
zend_class_entry *ce_DateOverride;


const zend_function_entry date_override_date_override_functions[] = {
  PHP_ME(DateOverride, __construct, NULL, ZEND_ACC_CTOR|ZEND_ACC_PUBLIC)
  PHP_FE_END
};
zend_class_entry *ce_DateOverride;


zend_module_entry date_override_module_entry = {
  STANDARD_MODULE_HEADER_EX,
  NULL,
  date_override_deps,
  "date_override",
  date_override_functions,
  PHP_MINIT(date_override),
  NULL,
  PHP_RINIT(date_override),
  PHP_RSHUTDOWN(date_override),
  PHP_MINFO(date_override),
  DATE_OVERRIDE_EXT_VERSION,
  STANDARD_MODULE_PROPERTIES
};

PHP_MINIT_FUNCTION(date_override)
{
  zend_class_entry ce;
  INIT_CLASS_ENTRY(ce, "DateOverrideDateTime", date_override_date_override_functions);
  ce_DateOverride = zend_register_internal_class(&ce TSRMLS_CC);

  return SUCCESS;
}

static int date_overrided = 0;
static char *date_override_time_str = NULL;

PHP_RINIT_FUNCTION(date_override)
{
  if (date_overrided == 1) {
    return SUCCESS;
  }

  date_overrided = 1;

  /* date_create() */
  replace_function("date_create", "date_override_date_create" TSRMLS_CC);

  /* DateTime::construct() */
  replace_method("datetime", "dateoverridedatetime", "__construct" TSRMLS_CC);

  return SUCCESS;
}

PHP_RSHUTDOWN_FUNCTION(date_override)
{
  php_date_override_reset(TSRMLS_C);

  return SUCCESS;
}

PHP_MINFO_FUNCTION(date_override)
{
  php_info_print_table_start();
  php_info_print_table_colspan_header(2, "Back to the Future (without DeLorean)");
  php_info_print_table_row(2, "version", DATE_OVERRIDE_EXT_VERSION);
  php_info_print_table_row(2, "casting", "Francis Besset");
  php_info_print_table_end();
}


PHP_FUNCTION(date_override)
{
  char *time_str = NULL;
  int   time_str_len = 0;

  if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &time_str, &time_str_len) == FAILURE) {
    RETURN_FALSE;
  }

  php_date_override_reset(TSRMLS_C);

  if (time_str_len != 0) {
    date_override_time_str = estrndup(time_str, time_str_len);
  }

  RETURN_TRUE;
}

PHP_FUNCTION(date_override_date_create)
{
  zval datetime_object, *timezone_object = NULL;
  char *time_str = NULL;
  int   time_str_len = 0;

  if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "|sO!", &time_str, &time_str_len, &timezone_object, php_date_get_timezone_ce()) == FAILURE) {
    RETURN_FALSE;
  }

  php_date_instantiate(php_date_get_date_ce(), &datetime_object TSRMLS_CC);
  if (!php_date_override_date_initialize(&datetime_object, time_str, time_str_len, timezone_object, 0 TSRMLS_CC)) {
      zval_dtor(&datetime_object);
      RETURN_FALSE;
  }

  RETVAL_ZVAL(&datetime_object, 0, 0);
}


PHP_METHOD(DateOverride, __construct)
{
  zval *timezone_object = NULL;
  char *time_str = NULL;
  int   time_str_len = 0;
  zend_error_handling error_handling;

  zend_replace_error_handling(EH_THROW, NULL, &error_handling TSRMLS_CC);

  if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "|sO!", &time_str, &time_str_len, &timezone_object, php_date_get_timezone_ce()) == SUCCESS) {
    php_date_override_date_initialize(getThis(), time_str, time_str_len, timezone_object, 1 TSRMLS_CC);
  }

  zend_restore_error_handling(&error_handling TSRMLS_CC);
}


static void replace_function(char *dfunc_str, char *sfunc_str TSRMLS_DC)
{
  int dfunc_str_len = strlen(dfunc_str) + 1;
  int sfunc_str_len = strlen(sfunc_str) + 1;

  zend_function *sfunc_fe;

  /* delete original function */
  zend_hash_del(EG(function_table), dfunc_str, dfunc_str_len);

  /* find source function */
  zend_hash_find(EG(function_table), sfunc_str, sfunc_str_len, (void **) &sfunc_fe);

  /* add source function with the name of destination function */
  zend_hash_add(EG(function_table), dfunc_str, dfunc_str_len, &*sfunc_fe, sizeof(zend_function), NULL);

  /* delete source function */
  zend_hash_del(EG(function_table), sfunc_str, sfunc_str_len);
}

static void replace_method(char *dclass_str, char *sclass_str, char *method_str TSRMLS_DC)
{
  int method_str_len = strlen(method_str);

  zend_class_entry **dclass_entry, **sclass_entry;
  zend_function *method_fe;

  /* find destination class in class_table */
  zend_hash_find(EG(class_table), dclass_str, strlen(dclass_str) + 1, (void **) &dclass_entry);

  /* remove original method on destination class */
  zend_hash_del(&(*dclass_entry)->function_table, method_str, method_str_len + 1);

  /* find source class in class_table */
  zend_hash_find(EG(class_table), sclass_str, strlen(sclass_str) + 1, (void **) &sclass_entry);

  /* find method in source class */
  zend_hash_find(&(*sclass_entry)->function_table, method_str, method_str_len + 1, (void **) &method_fe);

  zend_hash_add(&(*dclass_entry)->function_table, method_str, method_str_len + 1, method_fe, sizeof(zend_function), NULL);
  if (0 == strcmp(method_str, "__construct")) {
    method_fe->common.fn_flags |= ZEND_ACC_CTOR;
    method_fe->common.fn_flags &= ~ZEND_ACC_ALLOW_STATIC;
    method_fe->op_array.fn_flags |= ZEND_ACC_PUBLIC;
    method_fe->common.function_name = "__construct";
    (*dclass_entry)->constructor = method_fe;
  }
}


static int php_date_override_date_initialize(zval *datetime_object, char *time_str, int time_str_len, zval *timezone_object, int ctor TSRMLS_DC)
{
  zval function_name, *params[1], *retval;
  char *tz_str = NULL;
  int initialize, is_default_tz;

  if (!php_date_override_is_enabled(TSRMLS_C) || php_date_override_is_full_date(time_str, time_str_len TSRMLS_CC)) {
    return php_date_initialize(zend_object_store_get_object(datetime_object TSRMLS_CC), time_str, time_str_len, NULL, timezone_object, ctor TSRMLS_CC);
  }

  initialize = php_date_initialize(zend_object_store_get_object(datetime_object TSRMLS_CC), date_override_time_str, strlen(date_override_time_str), NULL, timezone_object, ctor TSRMLS_CC);

  if (time_str_len == 0) {
    return initialize;
  }

  /* get timezone in time_str */
  is_default_tz = php_date_override_get_tz(&tz_str, time_str, time_str_len TSRMLS_CC);
  if (!is_default_tz) {
    /* instanciate user tz */
    ZVAL_STRING(&function_name, "timezone_open", 0);

    MAKE_STD_ZVAL(params[0]);
    ZVAL_STRING(params[0], tz_str, 0);

    MAKE_STD_ZVAL(timezone_object);
    call_user_function(CG(function_table), NULL, &function_name, timezone_object, 1, params TSRMLS_CC);

    efree(params[0]);

    /* set user tz */
    ZVAL_STRING(&function_name, "setTimeZone", 0);

    MAKE_STD_ZVAL(params[0]);
    ZVAL_ZVAL(params[0], timezone_object, 0, 0);
    efree(timezone_object);

    MAKE_STD_ZVAL(retval);
    call_user_function(NULL, &datetime_object, &function_name, retval, 1, params TSRMLS_CC);

    efree(params[0]);
    efree(retval);
  }

  ZVAL_STRING(&function_name, "modify", 0);

  MAKE_STD_ZVAL(params[0]);
  ZVAL_STRINGL(params[0], time_str, time_str_len, 0);

  MAKE_STD_ZVAL(retval);
  call_user_function(NULL, &datetime_object, &function_name, retval, 1, params TSRMLS_CC);

  efree(params[0]);
  efree(retval);

  return initialize;
}

static int php_date_override_is_full_date(char *time_str, int time_str_len TSRMLS_DC)
{
  zval function_name, *retval, *param[1], **element;
  HashTable *retval_ht;

  /* parse user date */
  INIT_ZVAL(function_name);
  ZVAL_STRING(&function_name, "date_parse", 0);

  MAKE_STD_ZVAL(param[0]);
  ZVAL_STRINGL(param[0], time_str, time_str_len, 0);

  MAKE_STD_ZVAL(retval);
  call_user_function(EG(function_table), NULL, &function_name, retval, 1, param TSRMLS_CC);
  efree(param[0]);

  retval_ht = Z_ARRVAL_P(retval);

#define PHP_DATE_OVERRIDE_HAVE_TIME_ELEMENT(name) \
  if (zend_hash_find(retval_ht, #name, sizeof(#name), (void **)&element) == SUCCESS) { \
    if (Z_TYPE_PP(element) == IS_BOOL) { \
      zval_dtor(retval); \
      FREE_ZVAL(retval); \
      return 0; \
    } \
  }

  PHP_DATE_OVERRIDE_HAVE_TIME_ELEMENT(year);
  PHP_DATE_OVERRIDE_HAVE_TIME_ELEMENT(month);
  PHP_DATE_OVERRIDE_HAVE_TIME_ELEMENT(day);
  PHP_DATE_OVERRIDE_HAVE_TIME_ELEMENT(hour);
  PHP_DATE_OVERRIDE_HAVE_TIME_ELEMENT(minute);
  PHP_DATE_OVERRIDE_HAVE_TIME_ELEMENT(second);
  PHP_DATE_OVERRIDE_HAVE_TIME_ELEMENT(fraction);

  zval_dtor(retval);
  FREE_ZVAL(retval);

  return 1;
}

static int php_date_override_get_tz(char **tz_str, char *time_str, int time_str_len TSRMLS_DC)
{
  int not_found = 1;

  if (time_str_len) {
    zval function_name, *retval, *param[1], **tz_name;

    /* parse user date */
    INIT_ZVAL(function_name);
    ZVAL_STRING(&function_name, "date_parse", 0);

    MAKE_STD_ZVAL(param[0]);
    ZVAL_STRING(param[0], time_str, 0);

    MAKE_STD_ZVAL(retval);


    call_user_function(EG(function_table), NULL, &function_name, retval, 1, param TSRMLS_CC);
    efree(param[0]);

    HashTable *retval_ht;
    retval_ht = Z_ARRVAL_P(retval);
    if (zend_hash_find(retval_ht, "tz_id", sizeof("tz_id"), (void **)&tz_name) == SUCCESS) {
      not_found = 0;
      *tz_str = strdup(Z_STRVAL_PP(tz_name));
    }

    zval_dtor(retval);
    FREE_ZVAL(retval);
  }

  if (not_found == 1) {
    /* get default tz */
    *tz_str = strdup(get_timezone_info(TSRMLS_C)->name);
  }

  return not_found;
}

static void php_date_override_set_timezone(zval *datetime_object, zval *timezone_object TSRMLS_DC)
{
  zval function_name, *retval, *param[1];

  INIT_ZVAL(function_name);
  ZVAL_STRING(&function_name, "setTimeZone", 0);

  MAKE_STD_ZVAL(retval);
  call_user_function(NULL, &datetime_object, &function_name, retval, 1, &timezone_object TSRMLS_CC);

  zval_dtor(retval);
  FREE_ZVAL(retval);
}


static int php_date_override_is_enabled(TSRMLS_D)
{
  return date_override_time_str == NULL ? 0 : 1;
}

static void php_date_override_reset(TSRMLS_D)
{
  if (php_date_override_is_enabled(TSRMLS_C)) {
    efree(date_override_time_str);
    date_override_time_str = NULL;
  }
}

#ifdef COMPILE_DL_DATE_OVERRIDE
ZEND_GET_MODULE(date_override)
#endif
