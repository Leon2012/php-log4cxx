<?php
$br = (php_sapi_name() == "cli")? "":"<br>";

if(!extension_loaded('log4cxx')) {
	dl('log4cxx.' . PHP_SHLIB_SUFFIX);
}
$module = 'log4cxx';


//Log4cxx_BasicConfigurator::configure('%d %p [%t] (%F:%L) - %m%n');
//Log4cxx_BasicConfigurator::defaultConfigure();

Log4cxx_PropertyConfigurator::configure("./examples/log4j.properties");

$logger = new Log4cxx_Logger(__METHOD__);
$logger->info("中文测试 -- info");
$logger->warn("中文测试 -- warn");
$logger->debug("中文测试 -- debug");
$logger->error("中文测试 -- error");
$logger->trace("中文测试 -- trace");
?>
