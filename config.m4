dnl config.m4 for extension date_override

PHP_ARG_ENABLE(date_override, whether to enable date_override support,
[  --enable-date-override  Enable date_override date support])

if test "$PHP_DATE_OVERRIDE" != "no"; then
  PHP_NEW_EXTENSION(date_override, date_override.c, $ext_shared)
fi
