--TEST--
Simple usage of DateTime::__construct() with timezone in time and second arg
--FILE--
<?php
date_default_timezone_set('America/Los_Angeles');

date_override('2015-07-12 15:02:00');

var_dump(new DateTime('yesterday UTC', new DateTimeZone('Asia/Muscat')));
var_dump(date_create('yesterday UTC', new DateTimeZone('Asia/Muscat')));
echo "\n";
var_dump(new DateTime('yesterday Europe/Paris', new DateTimeZone('UTC')));
var_dump(date_create('yesterday Europe/Paris', new DateTimeZone('UTC')));
?>
--EXPECTF--
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
