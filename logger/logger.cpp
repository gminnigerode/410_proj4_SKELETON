#include "../includes/logger.h"
using namespace std;

//Constructor for Logger that sets filename field to given file name
Logger::Logger(std::string filename) {
	this->filename = filename;
}

//Logger destructor
Logger::~Logger() {
}

//Clears the content of the log file
void Logger::clearlogfile() {
	lock_guard<mutex> lg(m);
	myFile.open(filename, std::fstream::trunc);

	//close file
	if (myFile.is_open())
		myFile.close();
}

//Appends data to the end of the log file
void Logger::log(std::string data) {
	lock_guard<mutex> lg(m);
	myFile.open(filename, std::fstream::app);
	if (!myFile.is_open())
		return;

	std::string myline;

	myFile << data;

	//close file
	if (myFile.is_open())
		myFile.close();
}
