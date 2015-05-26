# php-log4cxx

log4cxx的php扩展

## 安装:

1，安装log4cxx
	
	#for osx
	brew install log4cxx
	
	
	
	#for ubuntu
	
	wget http://mirrors.cnnic.cn/apache//apr/apr-1.5.2.tar.gz
	tar zxvf apr-1.5.2.tar.gz
	cd apr-1.5.2/
	./configure --prefix=/usr/local
	make&sudo make install
	
	wget http://mirrors.cnnic.cn/apache//apr/apr-util-1.5.4.tar.gz
	tar zxvf apr-util-1.5.4.tar.gz
	cd apr-util-1.5.4/
	./configure --prefix=/usr/local --with-apr=/usr/local
	make&sudo make install
	
	wget http://mirrors.cnnic.cn/apache/logging/log4cxx/0.10.0/apache-log4cxx-0.10.0.tar.gz
	tar zxvf apache-log4cxx-0.10.0.tar.gz
	cd apache-log4cxx-0.10.0/
	./configure --prefix=/usr/local --with-charset=utf-8 --with-logchar=utf-8
	make&sudo make install
	
	
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
	
