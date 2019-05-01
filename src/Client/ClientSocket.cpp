/*
 * ClientSocket.cpp
 *
 *  Created on: 28 abr. 2019
 *      Author: sebastian
 */

#include "ClientSocket.h"

const string ERROR = "ERROR";
const string INFO = "INFO";
const string DEBUG = "DEBUG";

ClientSocket::ClientSocket(string *host, int port) {
	this->host = *host;
	this->port = port;
	this->address = (sockaddr_in*) malloc(sizeof(sockaddr_in*)); //free

	_socket = -1;
	initialized = false;

	if (!this->initializeAddress()) { return; }
	if (!this->initializeSocket()) { return; }

	initialized = true;

	this->connectToSocket();
}

ClientSocket::~ClientSocket() {
	disconnectSocket();
}

bool ClientSocket::isInitialized() {
	return initialized;
}

bool ClientSocket::isConnected() {
	return connected;
}

bool ClientSocket::initializeAddress() {
	address->sin_family = AF_INET;
	address->sin_port = htons((int)this->port);
	address->sin_addr.s_addr = INADDR_ANY;
	return true;
}

bool ClientSocket::initializeSocket() {
	Logger* logger = Logger::getInstance();

	_socket = socket(address->sin_family, SOCK_STREAM, 0);
	if(_socket == -1) {
		logger->log("Error en inicializar socket del cliente", ERROR);
		return false;
	}

	return true;
}

void ClientSocket::connectToSocket() {
	Logger* logger = Logger::getInstance();

	connected = (connect(_socket,(struct sockaddr *)&address, sizeof(address)) != -1);
	if (!connected) {
		logger->log("Error el conectar socket del cliente", ERROR);
	}
}

bool ClientSocket::sendMessage(string message) {
	Logger* logger = Logger::getInstance();

	if (!initialized || !connected) {
		return false;
	}

	int byteCount = send(_socket, message.c_str(), message.size(), 0);
	if (byteCount == -1) {
		logger->log("Error al enviar mensaje desde el cliente.", ERROR);
		return false;
	}

	return true;
}

bool ClientSocket::receiveMessage(char* receivedMessage, int receivedMessageLength) {
//	Logger* logger = Logger::getInstance();
//
//	if (!initialized || !connected) {
//		return false;
//	}
//
//	memset(receivedMessage, 0, receivedMessageLength);
//	int bytecount = recv(_socket, receivedMessage, receivedMessageLength, 0);
//	if (bytecount == SOCKET_ERROR) {
//		logger->log("Error al recibir mensaje desde el servidor.", ERROR);
//		return false;
//	}
//
	return true;
}

void ClientSocket::disconnectSocket() {
	connected = false;
	if (_socket != -1) {
		shutdown(_socket, SHUT_RDWR);
		_socket = -1;
	}
}
