/*
 * Socket.h
 *
 *  Created on: 11 may. 2019
 *      Author: caropistillo
 */

#ifndef SOCKET_H_
#define SOCKET_H_

#include <iostream>
#include <cstring>
#include "tools/logger/Logger.h"
#include <arpa/inet.h>

class Socket
{
private:
	 //fileDescriptor
	int port;

public:
	int fd;

	Socket();

	bool create(Logger* logger);

	bool bindToAddress(int port, Logger* logger);

	int get_fd();

	bool listenConnection(int maxConnections, Logger* logger);

	bool initialize(Logger* logger, int port, int maxConnections);

	void acceptConnection(Socket* otherSocket, struct sockaddr_in *clientAddress, Logger* logger);

};



#endif /* SOCKET_H_ */
