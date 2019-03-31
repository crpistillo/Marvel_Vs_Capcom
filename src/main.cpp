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
#include "MCGame/MCGame.h"

using namespace std;

json parseConfigFile(Logger* logger) {
	ConfigFileParser* parser = new ConfigFileParser("config/config.json", logger);
	parser->parse();
	json config = parser->getConfig();
	delete parser;
	return config;
}

MCGame* mcGame = 0;

int main() {
	Logger* logger = new Logger("marvel-vs-capcom.log");
	logger->startSession();
	/*json config = parseConfigFile(logger);
	cout << config.dump(4) << endl;

	logger->finishSession();
	delete logger;
*/
    mcGame = new MCGame(logger);
    mcGame->init("Marvel vs Capcom", 100, 100, 800, 600, 0);
    while(mcGame->running()) {
        mcGame->handleEvents();
        mcGame->update();
        mcGame->render();
    }
    mcGame->clean();

    logger->finishSession();
    delete logger;
    return 0;
}
