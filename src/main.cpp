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

json parseConfigFile(Logger* logger, char* logPath) {
	ConfigFileParser* parser = new ConfigFileParser(logPath, logger);
	parser->parse();
	json config = parser->getConfig();
	delete parser;
	return config;
}

MCGame* mcGame = 0;

int main(int argc, char** argv) {
    if(argc != 2)
        printf("error");//error
	Logger* logger = new Logger("marvel-vs-capcom.log");
	logger->startSession();
	logger->log("Logger iniciado.", DEBUG);
	json config = parseConfigFile(logger, argv[1]);
	logger->log("Configuracion Cargada - Inicio de Ciclo.", INFO);

    mcGame = new MCGame(logger, config);
    mcGame->init("Marvel vs Capcom", 100, 100, 800, 600, 0);
    mcGame->run();

    mcGame->clean();
    logger->log("Fin de ciclo de ejecucion.", INFO);
    logger->finishSession();
    delete logger;
    delete mcGame;
    return 0;
}
