--TEST--
Simple usage of DateTime::__construct() with timezone in time
--FILE--
<?php
date_default_timezone_set('Europe/Paris');

date_override('2015-07-12 15:02:00');

var_dump(new DateTime('now'));
var_dump(date_create('now'));
echo "\n";
var_dump(new DateTime('now UTC'));
var_dump(date_create('now UTC'));
echo "\n";
var_dump(new DateTime('yesterday UTC'));
var_dump(date_create('yesterday UTC'));
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
  string(26) "2015-07-12 13:02:00.000000"
  ["timezone_type"]=>
  int(3)
  ["timezone"]=>
  string(3) "UTC"
}
object(DateTime)#%d (3) {
  ["date"]=>
  string(26) "2015-07-12 13:02:00.000000"
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
