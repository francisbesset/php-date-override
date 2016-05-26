php-date-override
=================

The date_override extension provides an API to override the original date extension of PHP. **ZTS supported!**

[![Build Status](https://travis-ci.org/francisbesset/php-date-override.svg?branch=master)](https://travis-ci.org/francisbesset/php-date-override)


Table of contents
-----------------

1. [Installing/Configuring](#installingconfiguring)
  * [Installation](#installation)
2. [Usage](#usage)
  * [Overridden classes and functions](#overridden-classes-and-functions)
  * [Functions](#functions)
    * [date_override](#date_overridestring-time)


Installing/Configuring
----------------------

### Installation

```sh
phpize
./configure
make && make install
```

`make install` copies `date_override.so` to an appropriate location, but you still need to enable the module in the PHP config file or in command line.

#### File

Edit your php.ini or add a date_override.ini file in `/etc/php5/conf.d` with the following contents: `extension=date_override.so`.

#### Command line

You should use the `-d` option of php command: `php -dextension=date_override.so script.php`


Usage
-----

### Overridden classes and functions

  * Classes
    * [DateTime::__construct()](http://php.net/manual/datetime.construct.php)
  * Functions
    * [date_create()](http://php.net/manual/function.date-create.php)


### Functions

#### date_override(mixed $time)

Set the time should be used by overrided functions and classes.

  * **$time**: A date/time string or a boolean to false to reset

The function return `true` on success, `false` on failure.

```php
date_override('2015-07-12 15:02:00'); // set custom "now" value
var_dump(new DateTime());
/*
object(DateTime)#1 (3) {
  ["date"]=>
  string(26) "2015-07-12 15:02:00.000000"
  ["timezone_type"]=>
  int(3)
  ["timezone"]=>
  string(12) "Europe/Paris"
}
*/

date_override(false); // reset "now" value
var_dump(new DateTime());
/*
object(DateTime)#1 (3) {
  ["date"]=>
  string(26) "2016-05-26 08:51:37.000000"
  ["timezone_type"]=>
  int(3)
  ["timezone"]=>
  string(12) "Europe/Paris"
}
*/
```
