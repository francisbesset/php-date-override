--TEST--
Simple usage of DateTime::__construct() with timestamp
--FILE--
<?php
date_default_timezone_set('Europe/Paris');

date_override('2015-07-12 15:02:00');

var_dump(new DateTime('@1234567890'));
var_dump(date_create('@1234567890'));
?>
--EXPECTF--
object(DateTime)#%d (3) {
  ["date"]=>
  string(26) "2009-02-13 23:31:30.000000"
  ["timezone_type"]=>
  int(1)
  ["timezone"]=>
  string(6) "+00:00"
}
object(DateTime)#%d (3) {
  ["date"]=>
  string(26) "2009-02-13 23:31:30.000000"
  ["timezone_type"]=>
  int(1)
  ["timezone"]=>
  string(6) "+00:00"
}
