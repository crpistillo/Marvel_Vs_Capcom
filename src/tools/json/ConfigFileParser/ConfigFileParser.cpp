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

ConfigFileParser::ConfigFileParser(string path) {
	this->logger = Logger::getInstance();
	if(!this->fileExists(path)) {
		this->logger->log("Usando archivo de configuración por default ubicado en '" + DEFAULT_CONFIG_FILE_PATH, INFO);
		this->filePath = DEFAULT_CONFIG_FILE_PATH;
	} else {
		this->logger->log("Usando archivo de configuración ubicado en '" + path + "'.", INFO);
		this->filePath = path;
	}
}

bool ConfigFileParser::fileExists(string path) {
	this->logger->log("Verificando existencia del archivo de configuración '" + path + "'.", INFO);
	try {
		ifstream i(path.c_str());
		if(i.good()) {
			this->logger->log("El archivo '" + path + "' existe.", INFO);
			return true;
		}
		else {
			throw runtime_error("El archivo '" + path + "' no existe.");
		}
	} catch(exception& e) {
		this->logger->log(string(e.what()), ERROR);
		return false;
	}
}

void ConfigFileParser::parse() {
	this->logger->log("Iniciando validación del archivo de configuración.", INFO);
	ifstream i(this->filePath.c_str());
	try {
		this->config = json::parse(i);
	} catch(exception& e) {
		this->logger->log(string(e.what()), ERROR);
	}

	json defaultConfig;
	if(this->filePath != DEFAULT_CONFIG_FILE_PATH) {
		ifstream i(DEFAULT_CONFIG_FILE_PATH.c_str());
		defaultConfig = json::parse(i);
	}

	this->validateLogLevel(defaultConfig);
	this->logger->setLogLevel(this->config["log"]);
	this->validateBattlefield(defaultConfig);
	this->validateCharacters(defaultConfig);
	this->validateWindow(defaultConfig);
}

void ConfigFileParser::validateLogLevel(json defaultConfig) {
	this->logger->log("Validando nivel de log...", INFO);
	if(this->config.find("log") == this->config.end()) {
		this->logError("log", "Nivel de log no especificado.");
		this->replaceByDefault("log", defaultConfig);
	}
	if(!this->config.find("log")->is_string()) {
		this->logError("log", "Campo 'log' invalido. Debe ser un string.");
		this->replaceByDefault("log", defaultConfig);
	}
	else {
		string logErrorLevel = this->config["log"];
		if(logErrorLevel != ERROR && logErrorLevel != INFO && logErrorLevel != DEBUG) {
			this->logError("log", "No existe el nivel de log especificado.");
			this->replaceByDefault("log", defaultConfig);
		}
	}
	this->logger->log("Nivel de log validado OK.", INFO);
}

void ConfigFileParser::validateBattlefield(json defaultConfig) {
	this->logger->log("Validando battlefield...", INFO);
	if(this->config.find("battlefield") == this->config.end()) {
		this->logError("battlefield", "Battlefield no especificado.");
		this->replaceByDefault("battlefield", defaultConfig);
	} else {
		json battlefield = this->config["battlefield"];
		if(!battlefield.is_array()) {
			this->logError("battlefield", "Battlefield no es un array.");
			this->replaceByDefault("battlefield", defaultConfig);
		} else {
			if(battlefield.empty()) {
				this->logError("battlefield", "Battlefield está vacío.");
				this->replaceByDefault("battlefield", defaultConfig);
			}
			if((int) battlefield.size() != 3) {
				this->logError("battlefield", "Battlefield no contiene la cantidad de backgrounds necesarios. Deben ser 3.");
				this->replaceByDefault("battlefield", defaultConfig);
			}
			else {
				this->logger->log("Verificando si todos los elementos tienen el atributo 'background' y dentro de este un 'filepath'", INFO);
				int count = 0;
				for(json::iterator it = battlefield.begin(); it != battlefield.end(); ++it) {
					if(it->find("background") == it->end()) {
						this->logError("battlefield bloque " + to_string(count), "Hay un background no especificado.");
						this->replaceByDefault("battlefield", defaultConfig,"",count);

					} else {

						//Filepath verification
						if((*it)["background"].find("filepath") == (*it)["background"].end()) {
							this->logError("battlefield bloque " + to_string(count), "Hay un filepath no especificado.");
                            this->replaceByDefault("battlefield", defaultConfig,"",count);

						}
						else if(! (*it)["background"].find("filepath")->is_string()) {
							this->logError("battlefield bloque " + to_string(count), "Uno de los campos 'filepath' no es válido. Debe ser un string.");
                            this->replaceByDefault("battlefield", defaultConfig,"",count);

						}

						//Z Index verification
						else if((*it)["background"].find("zindex") == (*it)["background"].end()) {
							this->logError("battlefield bloque " + to_string(count), "Hay un z index no especificado.");
                            this->replaceByDefault("battlefield", defaultConfig,"",count);

						}
						else if(! (*it)["background"].find("zindex")->is_number_unsigned()) {
							this->logError("battlefield bloque " + to_string(count), "Uno de los campos 'z index' no es válido. Debe ser un número positivo.");
                            this->replaceByDefault("battlefield", defaultConfig,"",count);

						}
					}
					count++;
				}
			}
		}
	}
	this->logger->log("Battlefield validado OK.", INFO);
}

void ConfigFileParser::validateCharacters(json defaultConfig) {
	this->logger->log("Validando characters...", INFO);
	if(this->config.find("characters") == this->config.end()) {
		this->logError("characters", "Characters no especificado.");
		this->replaceByDefault("characters", defaultConfig);
	} else {
		json characters = this->config["characters"];
		if(!characters.is_array()) {
			this->logError("characters", "Characters no es un array.");
			this->replaceByDefault("characters", defaultConfig);
		} else {
			if(characters.empty()) {
				this->logError("characters", "Characters está vacío.");
				this->replaceByDefault("characters", defaultConfig);
			} else {
				this->logger->log("Verificando si todos los elementos tienen los atributos necesarios.", INFO);
				int count = 0;
				for(json::iterator it = characters.begin(); it != characters.end(); ++it) {

					//Name verification
					if(it->find("name") == it->end()) {
						this->logError("characters bloque " + to_string(count), "Hay un name no especificado.");
						this->replaceByDefault("characters", defaultConfig,"", count);

					}
					else if(!it->find("name")->is_string()){
						this->logError("characters bloque " + to_string(count), "Uno de los campos 'name' no es válido. Debe ser un string.");
						this->replaceByDefault("characters", defaultConfig,"",count);

					}

					//Filepath verification
					else if(it->find("filepath") == it->end()) {
						this->logError("characters bloque " + to_string(count), "Hay un filepath no especificado.");
						this->replaceByDefault("characters", defaultConfig,"",count);

					}
					else if(!it->find("filepath")->is_string()) {
						this->logError("characters bloque " + to_string(count), "Uno de los campos 'filepath' no es válido. Debe ser un string.");
						this->replaceByDefault("characters", defaultConfig,"",count);

					}

					//Height verification
					else if(it->find("height") == it->end()) {
						this->logError("characters bloque " + to_string(count), "Hay un height no especificado.");
						this->replaceByDefault("characters", defaultConfig,"",count);

					}
					else if(!it->find("height")->is_number_unsigned()) {
						this->logError("characters bloque " + to_string(count), "Uno de los campos 'height' no es válido. Debe ser un número positivo.");
						this->replaceByDefault("characters", defaultConfig,"",count);

					}

					//Width verification
					else if(it->find("width") == it->end()) {
						this->logError("characters bloque " + to_string(count), "Hay un width no especificado.");
						this->replaceByDefault("characters", defaultConfig,"",count);

					}
					else if(!it->find("width")->is_number_unsigned()) {
						this->logError("characters bloque " + to_string(count), "Uno de los campos 'width' no es válido. Debe ser un número positivo.");
						this->replaceByDefault("characters", defaultConfig,"",count);

					}

					//Zindex verification
					else if(it->find("zindex") == it->end()) {
						this->logError("characters bloque " + to_string(count), "Hay un zindex no especificado.");
						this->replaceByDefault("characters", defaultConfig,"" ,count);

					}
					else if(!it->find("zindex")->is_number_unsigned()) {
						this->logError("characters bloque " + to_string(count), "Uno de los campos 'zindex' no es válido. Debe ser un número positivo.");
						this->replaceByDefault("characters", defaultConfig,"",count);

					}

					count++;

				}
			}
		}
	}
	this->logger->log("Characters validado OK.", INFO);
}

void ConfigFileParser::validateWindow(json defaultConfig) {
	this->logger->log("Validando window...", INFO);
	if(this->config.find("window") == this->config.end()) {
		this->logError("window", "Window no especificado.");
		this->replaceByDefault("window", defaultConfig);
	} else {
		json window = this->config["window"];
		if(window.find("height") == window.end()) {
			this->logError("window", "Height no especificado.");
			this->replaceByDefault("window", defaultConfig);
		}
		if(window.find("width") == window.end()) {
			this->logError("window", "Width no especificado.");
			this->replaceByDefault("window", defaultConfig);
		}
	}
	this->logger->log("Window validado OK.", INFO);
}

void ConfigFileParser::logError(string key, string errorMsg) {
	this->logger->log(errorMsg, ERROR);
	this->logger->log("Usando " + key + " por default.", ERROR);
}

void ConfigFileParser::replaceByDefault(string key, json defaultConfig, string subKey, int posSubKey) {
	if(!subKey.empty() && posSubKey < 0) {
		this->config[key][subKey] = defaultConfig[key][subKey];
	}
	else if(subKey.empty() && posSubKey >= 0){
		this->config[key][posSubKey] = defaultConfig[key][posSubKey];
	}
	else {
		this->config[key] = defaultConfig[key];
	}
}

json ConfigFileParser::getConfig() {
	return this->config;
}
