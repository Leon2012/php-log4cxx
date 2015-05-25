//g++ -Wall -pedantic -I/usr/local/include -L/usr/local/lib  -llog4cxx ex3.cpp -o ex3

#include <log4cxx/logger.h>
#include <log4cxx/PropertyConfigurator.h>
#include <log4cxx/BasicConfigurator.h>
#include <log4cxx/patternlayout.h>
#include <log4cxx/consoleappender.h>

using namespace std;
using namespace log4cxx;
using namespace log4cxx::helpers;



int main() {

	PatternLayoutPtr myLayoutPtr = new PatternLayout("%r [%t] %-5p %c- %m%n %x");
	ConsoleAppenderPtr myAppenderPtr = new ConsoleAppender(myLayoutPtr);

	string trace = "ex3";
	myLayoutPtr->setConversionPattern("%d %p [%t] (%F:%L) - %m%n");
	log4cxx::BasicConfigurator::configure(myAppenderPtr);

	LoggerPtr logger = Logger::getLogger(trace);

	//设置中文编码
	setlocale(LC_ALL, "zh_CN.UTF-8"); 

	if (logger->isInfoEnabled()) {
		::log4cxx::helpers::MessageBuffer oss_;
		::log4cxx::spi::LocationInfo location(__FILE__,__func__,__LINE__);

		logger->forcedLog(::log4cxx::Level::getInfo(), oss_.str(oss_ << "你说hello"), location);
	}

	//logger->info("How to use?");
	//LOG4CXX_INFO(logger, ("你说hello"));

}