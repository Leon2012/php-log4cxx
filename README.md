# php-log4cxx

log4cxx的php扩展

## 安装:

1，安装log4cxx

	brew install log4cxx
	
2，安装php-log4cxx

	git clone https://github.com/Leon2012/php-log4cxx.git
	cd php-log4cxx
	phpize
	./configure 
	make&sudo make install
	
3，修改php.ini

	vi php.ini
	#add
	extension=log4cxx.so
	
## 用法：

	<?php
	
	//Log4cxx_BasicConfigurator::configure('%d %p [%t] (%F:%L) - %m%n'); #
	//Log4cxx_BasicConfigurator::defaultConfigure();
	
	Log4cxx_PropertyConfigurator::configure("./examples/log4j.properties");#加载配置文件
	
	$logger = new Log4cxx_Logger(__METHOD__);
	$logger->info("中文测试 -- info");
	$logger->warn("中文测试 -- warn");
	$logger->debug("中文测试 -- debug");
	$logger->error("中文测试 -- error");
	$logger->trace("中文测试 -- trace");
	
