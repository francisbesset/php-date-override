--TEST--
Simple usage of DateTime::__construct() with default timezone
--FILE--
<?php
date_default_timezone_set('Europe/Paris');

date_override('2015-07-12 15:02:00');

var_dump(new DateTime());
var_dump(date_create());
var_dump(new DateTime('now'));
var_dump(date_create('now'));
var_dump(new DateTime('yesterday'));
var_dump(date_create('yesterday'));

echo "\n";

date_default_timezone_set('UTC');
var_dump(new DateTime());
var_dump(date_create());
var_dump(new DateTime('now'));
var_dump(date_create('now'));
var_dump(new DateTime('yesterday'));
var_dump(date_create('yesterday'));
?>
--EXPECTF--
object(DateTime)#%d (3) {
  ["date"]=>
  string(26) "2015-07-12 15:02:00.000000"
  ["timezone_type"]=>
  int(3)
  ["timezone"]=>
  string(12) "Europe/Paris"
}
object(DateTime)#%d (3) {
  ["date"]=>
  string(26) "2015-07-12 15:02:00.000000"
  ["timezone_type"]=>
  int(3)
  ["timezone"]=>
  string(12) "Europe/Paris"
}
object(DateTime)#%d (3) {
  ["date"]=>
  string(26) "2015-07-12 15:02:00.000000"
  ["timezone_type"]=>
  int(3)
  ["timezone"]=>
  string(12) "Europe/Paris"
}
object(DateTime)#%d (3) {
  ["date"]=>
  string(26) "2015-07-12 15:02:00.000000"
  ["timezone_type"]=>
  int(3)
  ["timezone"]=>
  string(12) "Europe/Paris"
}
object(DateTime)#%d (3) {
  ["date"]=>
  string(26) "2015-07-11 00:00:00.000000"
  ["timezone_type"]=>
  int(3)
  ["timezone"]=>
  string(12) "Europe/Paris"
}
object(DateTime)#%d (3) {
  ["date"]=>
  string(26) "2015-07-11 00:00:00.000000"
  ["timezone_type"]=>
  int(3)
  ["timezone"]=>
  string(12) "Europe/Paris"
}

object(DateTime)#%d (3) {
  ["date"]=>
  string(26) "2015-07-12 15:02:00.000000"
  ["timezone_type"]=>
  int(3)
  ["timezone"]=>
  string(3) "UTC"
}
object(DateTime)#%d (3) {
  ["date"]=>
  string(26) "2015-07-12 15:02:00.000000"
  ["timezone_type"]=>
  int(3)
  ["timezone"]=>
  string(3) "UTC"
}
object(DateTime)#%d (3) {
  ["date"]=>
  string(26) "2015-07-12 15:02:00.000000"
  ["timezone_type"]=>
  int(3)
  ["timezone"]=>
  string(3) "UTC"
}
object(DateTime)#%d (3) {
  ["date"]=>
  string(26) "2015-07-12 15:02:00.000000"
  ["timezone_type"]=>
  int(3)
  ["timezone"]=>
  string(3) "UTC"
}
object(DateTime)#%d (3) {
  ["date"]=>
  string(26) "2015-07-11 00:00:00.000000"
  ["timezone_type"]=>
  int(3)
  ["timezone"]=>
  string(3) "UTC"
}
object(DateTime)#%d (3) {
  ["date"]=>
  string(26) "2015-07-11 00:00:00.000000"
  ["timezone_type"]=>
  int(3)
  ["timezone"]=>
  string(3) "UTC"
}
