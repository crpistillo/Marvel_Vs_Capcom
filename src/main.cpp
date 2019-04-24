//============================================================================
// Name        : main.cpp
// Author      : Grupo Capcom
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include "tools/json/ConfigFileParser/ConfigFileParser.h"
#include "tools/logger/Logger.h"
#include "MCGame.h"

const string ERROR = "ERROR";
const string INFO = "INFO";
const string DEBUG = "DEBUG";

using namespace std;

json parseConfigFile(Logger* logger, string logPath) {
	ConfigFileParser* parser = new ConfigFileParser(logPath, logger);
	parser->parse();
	json config = parser->getConfig();
	delete parser;
	return config;
}

MCGame* mcGame = 0;

int main(int argc, char** argv) {
	Logger* logger = new Logger("marvel-vs-capcom-");
	logger->startSession();
	logger->log("Logger iniciado.", DEBUG);
	json config;
    if(argc != 2){
    	logger->log("Archivo de configuracion no especificado.", INFO);
    	config = parseConfigFile(logger, "config/config_default.json");
    }
    else{
    	string configPath = (string) argv[1];
    	logger->log("Procediento a utilizar archivo de configuracion especificado: " + configPath , INFO);
    	config = parseConfigFile(logger, configPath);
    }
	int ancho = config["window"]["width"];
	int alto = config["window"]["height"];
	logger->log("Configuracion Cargada - Inicio de Ciclo.", INFO);

    mcGame = new MCGame(logger, config, ancho, alto);
    mcGame->camera = { 0, 0, ancho, alto };
    mcGame->init("Marvel vs Capcom", 100, 100, ancho, alto, 0);
    mcGame->run();

    mcGame->clean();
    logger->log("Fin de ciclo de ejecucion.", INFO);
    logger->finishSession();
    delete logger;
    delete mcGame;
    return 0;
}
