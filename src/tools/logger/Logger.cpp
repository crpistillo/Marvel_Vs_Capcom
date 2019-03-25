/*
 * Logger.cpp
 *
 *  Created on: 24 mar. 2019
 *      Author: sebastian
 */

#include "Logger.h"

const string ERROR = "ERROR";
const string INFO = "INFO";
const string DEBUG = "DEBUG";
const string INITIAL_LOG_LEVEL = INFO;

ofstream ofs;
const string separator = "==========================================================================================================\n";

Logger::Logger(string filePath) {
	this->filePath = filePath;
	this->logLevel = INITIAL_LOG_LEVEL;
}

string Logger::getDate() {
    time_t timeT = time(nullptr);
    string timeString = asctime(localtime(&timeT));
    unsigned int length = timeString.size();
    timeString.resize(--length);
    return timeString;
}

void Logger::startSession() {
	ofs.open(this->filePath.c_str(), ofstream::app);
	ofs << separator;
	ofs << "Game started at " + getDate() + "\n";
}

void Logger::finishSession() {
	ofs << "Game finished at " + getDate()+ "\n";
	ofs.close();
}

void Logger::setLogLevel(string logLevel) {
	this->logLevel = logLevel;
}

bool Logger::hasToLog(string requiredLogLevel) {
	if(requiredLogLevel != ERROR && requiredLogLevel != INFO && requiredLogLevel != DEBUG) return false;
	if(this->logLevel == ERROR && requiredLogLevel != ERROR) return false;
	if(this->logLevel == INFO && requiredLogLevel == DEBUG) return false;
	return true;
}

void Logger::log(string message, string requiredLogLevel) {
	if(this->hasToLog(requiredLogLevel)) {
		ofs << "[" + this->getDate() + "]-[" + requiredLogLevel + "] : " + message + "\n";
	}
}
