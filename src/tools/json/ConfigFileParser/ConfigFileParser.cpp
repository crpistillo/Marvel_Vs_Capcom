/*
 * ConfigFileParser.cpp
 *
 *  Created on: 23 mar. 2019
 *      Author: sebastian
 */

#include "ConfigFileParser.h"

const string DEFAULT_CONFIG_FILE_PATH = "config/config_default.json";

ConfigFileParser::ConfigFileParser() {
	this->filePath = DEFAULT_CONFIG_FILE_PATH;
	cout << "Usando archivo de configuración por default ubicado en '" + DEFAULT_CONFIG_FILE_PATH + "'.\n"; // Esto va al Log
}

ConfigFileParser::ConfigFileParser(string path) {
	if(!this->fileExists(path)) {
		cout << "Usando archivo de configuración por default ubicado en '" + DEFAULT_CONFIG_FILE_PATH + "'.\n"; // Esto va al Log
		this->filePath = DEFAULT_CONFIG_FILE_PATH;
	}
	cout << "Usando archivo de configuración ubicado en '" + path + "'.\n"; // Esto va al Log
	this->filePath = path;
}

bool ConfigFileParser::fileExists(string path) {
	cout << "Verificando existencia del archivo de configuración '" + path + "'.\n"; // Esto va al Log
	try {
		ifstream i(path.c_str());
		if(i.good()) {
			cout << "El archivo '" + path + "' existe."; // Esto va al Log
			return true;
		}
		else {
			throw runtime_error("El archivo '" + path + "' no existe.");
		}
	} catch (exception& e) {
		cout << "**ERROR** " + string(e.what()) + "\n"; // Esto va al Log
		return false;
	}
}

void ConfigFileParser::parse() {
	cout << "Iniciando validación del archivo de configuración.\n"; // Esto va al Log
	ifstream i(this->filePath.c_str());
	json j = json::parse(i);
	this->config = j;
}

json ConfigFileParser::getConfig() {
	return this->config;
}
