//g++ -Wall -pedantic -I/usr/local/include -L/usr/local/lib  -llog4cxx ex2.cpp -o ex2

#include <log4cxx/logger.h>
#include <log4cxx/PropertyConfigurator.h>

using namespace std;
using namespace log4cxx;
using namespace log4cxx::helpers;

int main() {

	string trace = "AP1";
	string propertyFile = "./log4j.properties";

	log4cxx::PropertyConfigurator::configure(propertyFile);
	LoggerPtr logger = Logger::getLogger(trace);

	//设置中文编码
	//setlocale(LC_ALL, "zh_CN.UTF-8"); 

	logger->info("How to use?");
	LOG4CXX_INFO(logger, ("你说hello"));

}
