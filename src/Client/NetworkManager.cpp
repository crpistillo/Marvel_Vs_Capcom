/*
 * NetworkManager.cpp
 *
 *  Created on: 28 abr. 2019
 *      Author: sebastian
 */

#include "NetworkManager.h"

const string ERROR = "ERROR";
const string INFO = "INFO";
const string DEBUG = "DEBUG";

NetworkManager* NetworkManager::getInstance() {
    if(!instance) {
        instance = new NetworkManager();
    }

    return instance;
}

void NetworkManager::sendMessage(string message) {
	Logger* logger = Logger::getInstance();

	if (!client->sendMessage(message)) {
		logger->log("Falló el envío de mensaje al servidor.", ERROR);
	}
}

bool NetworkManager::connectToServer(string host, int port) {
	Logger* logger = Logger::getInstance();

	client = new ClientSocket(host, port);

	if (!client->isConnected()) {
		logger->log("Error al intentar conectar con el servidor.", ERROR);
		delete client;
		client = NULL;
		return false;
	}
	logger->log("Cliente conectado a servidor. Host: " + string(host) + " - Puerto: " + to_string(port), INFO);

	return true;
}
