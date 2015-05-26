<?php
$br = (php_sapi_name() == "cli")? "":"<br>";

if(!extension_loaded('log4cxx')) {
	dl('log4cxx.' . PHP_SHLIB_SUFFIX);
}

use Log4cxx as A;


// Log4cxx\BasicConfigurator::configure('%d %p [%t] (%F:%L) - %m%n');
// Log4cxx\BasicConfigurator::defaultConfigure();

A\PropertyConfigurator::configure("./examples/log4cxx.properties");

//$logger = new Log4cxx_Logger(__METHOD__);



$logger = A\LogManager::getLogger(__METHOD__);

$logger->info("中文测试 -- info");
$logger->warn("中文测试 -- warn");
$logger->debug("中文测试 -- debug");
$logger->error("中文测试 -- error");
$logger->trace("中文测试 -- trace");


$logger = new A\Logger(__METHOD__);
$logger->error("中文测试11 -- error");
$logger->trace("中文测试11 -- trace");


?>
