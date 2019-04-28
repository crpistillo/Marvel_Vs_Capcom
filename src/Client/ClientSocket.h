/*
 * ClientSocket.h
 *
 *  Created on: 28 abr. 2019
 *      Author: sebastian
 */

#ifndef CLIENT_CLIENTSOCKET_H_
#define CLIENT_CLIENTSOCKET_H_

#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <string>
#include "../tools/logger/Logger.h"

class ClientSocket {
public:
	ClientSocket(string host, int port);
	~ClientSocket();
	bool isInitialized();
	bool isConnected();
	bool sendMessage(string message);
	bool receiveMessage(char* receivedMessage, int receivedMessageLength);
	void disconnectSocket();

private:
	int _socket;
	string host;
	int port;
	struct sockaddr_in* address;
	bool initialized;
	bool connected;

	bool initializeAddress();
	bool initializeSocket();
	void connectToSocket();
};


#endif /* CLIENT_CLIENTSOCKET_H_ */
