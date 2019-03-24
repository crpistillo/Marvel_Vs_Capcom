/*
 * ConfigFileParser.cpp
 *
 *  Created on: 23 mar. 2019
 *      Author: sebastian
 */

#include "ConfigFileParser.h"

const string DEFAULT_CONFIG_FILE_PATH = "config/config_default.json";
const string ERROR = "ERROR";
const string INFO = "INFO";
const string DEBUG = "DEBUG";

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
			cout << "El archivo '" + path + "' existe.\n"; // Esto va al Log
			return true;
		}
		else {
			throw runtime_error("El archivo '" + path + "' no existe.");
		}
	} catch(exception& e) {
		cout << "**ERROR** " + string(e.what()) + "\n"; // Esto va al Log
		return false;
	}
}

void ConfigFileParser::parse() {
	cout << "Iniciando validación del archivo de configuración.\n"; // Esto va al Log
	ifstream i(this->filePath.c_str());
	try {
		this->config = json::parse(i);
	} catch(exception& e) {
		cout << "**ERROR** " + string(e.what()) + "\n"; // Esto va al Log
	}

	json defaultConfig;
	if(this->filePath != DEFAULT_CONFIG_FILE_PATH) {
		ifstream i(DEFAULT_CONFIG_FILE_PATH.c_str());
		defaultConfig = json::parse(i);
	}

	this->validateLogLevel(defaultConfig);
	this->validateBattlefield(defaultConfig);
}

void ConfigFileParser::validateLogLevel(json defaultConfig) {
	cout << "Validando nivel de log...\n"; // Esto va al Log
	if(this->config.find("log") == this->config.end()) {
		cout << "**ERROR** Nivel de log no especificado.\n"; // Esto va al Log
		cout << "Usando nivel de log por default.\n"; // Esto va al Log
		this->config["log"] = defaultConfig["log"];
	} else {
		string logLevel = this->config["log"];
		if(logLevel != ERROR && logLevel != INFO && logLevel != DEBUG) {
			cout << "**ERROR** No existe el nivel de log especificado.\n"; // Esto va al Log
			cout << "Usando nivel de log por default.\n"; // Esto va al Log
			this->config["log"] = defaultConfig["log"];
		}
	}
	cout << "Nivel de log validado OK.\n"; // Esto va al Log
}

void ConfigFileParser::validateBattlefield(json defaultConfig) {
	cout << "Validando battlefield...\n"; // Esto va al Log
	if(this->config.find("battlefield") == this->config.end()) {
		cout << "**ERROR** Battlefield no especificado.\n"; // Esto va al Log
		cout << "Usando battlefield por default.\n"; // Esto va al Log
		this->config["battlefield"] = defaultConfig["battlefield"];
	} else {
		json battlefield = this->config["battlefield"];
		if(!battlefield.is_array()) {
			cout << "**ERROR** Battlefield no es un array.\n"; // Esto va al Log
			cout << "Usando battlefield por default.\n"; // Esto va al Log
			this->config["battlefield"] = defaultConfig["battlefield"];
		} else {
			if(battlefield.empty()) {
				cout << "**ERROR** Battlefield está vacío.\n"; // Esto va al Log
				cout << "Usando battlefield por default.\n"; // Esto va al Log
				this->config["battlefield"] = defaultConfig["battlefield"];
			} else {
				cout << "Verificando si todos los elementos tienen el atributo 'background' y dentro de este un 'filepath'.\n"; // Esto va al Log
				for(json::iterator it = battlefield.begin(); it != battlefield.end(); ++it) {
					if(it->find("background") == it->end()) {
						cout << "**ERROR** Hay un background no especificado.\n"; // Esto va al Log
						cout << "Usando battlefield por default.\n"; // Esto va al Log
						this->config["battlefield"] = defaultConfig["battlefield"];
						break;
					} else {
						if((*it)["background"].find("filepath") == (*it)["background"].end()) {
							cout << "**ERROR** Hay un filepath no especificado.\n"; // Esto va al Log
							cout << "Usando battlefield por default.\n"; // Esto va al Log
							this->config["battlefield"] = defaultConfig["battlefield"];
							break;
						}
					}
				}
			}
		}
	}
	cout << "Battlefield validado OK.\n"; // Esto va al Log
}

json ConfigFileParser::getConfig() {
	return this->config;
}
