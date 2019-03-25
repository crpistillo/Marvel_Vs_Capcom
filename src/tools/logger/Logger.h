/*
 * Logger.h
 *
 *  Created on: 24 mar. 2019
 *      Author: sebastian
 */

#ifndef TOOLS_LOGGER_LOGGER_H_
#define TOOLS_LOGGER_LOGGER_H_

#include <iostream>
#include <fstream>
#include <string>
// User name
#include <unistd.h>
// Time
#include <chrono>
#include <ctime>

using namespace std;

class Logger {
private:
	string filePath;
	string logLevel;
	string getDate();
	bool hasToLog(string requiredLogLevel);
public:
	Logger(string filePath);
	void setLogLevel(string logLevel);
	void startSession();
	void log(string message, string requiredLogLevel);
	void finishSession();
};


#endif /* TOOLS_LOGGER_LOGGER_H_ */
