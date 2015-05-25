//g++ -Wall -pedantic -I/usr/local/include -L/usr/local/lib  -llog4cxx ex1.cpp -o ex1

#include <cstdlib>
#include <log4cxx/logger.h>
#include <log4cxx/basicconfigurator.h>
#include <log4cxx/propertyconfigurator.h>


// Instantiate logger
static log4cxx::LoggerPtr logger(log4cxx::Logger::getLogger("myApp"));

int main() {
 
  // Initialise logger. Use the configuration file named 'log4cxx.properties'.
  log4cxx::BasicConfigurator::configure();
  log4cxx::File propertyFile("./log4cxx.properties");
  log4cxx::PropertyConfigurator::configure(propertyFile);
 
  // Write to the log
  LOG4CXX_INFO(logger, "Hello World!");
 
  return EXIT_SUCCESS;
}