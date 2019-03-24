/*
 * ConfigFileParser.h
 *
 *  Created on: 23 mar. 2019
 *      Author: sebastian
 */

#ifndef TOOLS_JSON_CONFIGFILEPARSER_CONFIGFILEPARSER_H_
#define TOOLS_JSON_CONFIGFILEPARSER_CONFIGFILEPARSER_H_

#include <iostream>
#include <fstream>
#include <string>
#include "../json.hpp"

using json = nlohmann::json;
using namespace std;

class ConfigFileParser {
private:
	string filePath;
	json config;
	bool fileExists(string path);
	void validateLogLevel(json defaultConfig);
	void validateBattlefield(json defaultConfig);
	void validateCharacters(json defaultConfig);
	void validateWindow(json defaultConfig);
	void logError(string key, string errorMsg);
	void replaceByDefault(string key, json defaultConfig, string subKey);

public:
	ConfigFileParser();
	ConfigFileParser(string path);
	void parse();
	json getConfig();
};

#endif /* TOOLS_JSON_CONFIGFILEPARSER_CONFIGFILEPARSER_H_ */
