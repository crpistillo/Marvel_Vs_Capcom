/*
 * Socket.cpp
 *
 *  Created on: 11 may. 2019
 *      Author: caropistillo
 */

#include "Socket.h"

const string ERROR = "ERROR";
const string INFO = "INFO";
const string DEBUG = "DEBUG";

Socket::Socket()
{
	this->fd = 0;
	this->port = 0;
}

bool Socket::create(Logger* logger)
{
	this->fd = socket(AF_INET, SOCK_STREAM, 0); //AF_INET permite la comunicacion entre aplicaciones
												//corriendose en hosts conectados por IP version 4
							 //SOCK_STREAM es el tipo de socket: reliable, bidirectional, byte-stream
	if (fd == -1)
	{
		std::string msgError = "Error al crear socket.";
		cout << msgError << endl;
		logger->log(msgError, ERROR);
		return false;
	}
	return true;
}

bool Socket::bindToAddress(int port, Logger* logger) //cuidado con pasar sin referencia!!
{
	struct sockaddr_in serverAddress;
	this->port = port;
    memset(&serverAddress, 0, sizeof(struct sockaddr_in)); //Clear structure
    serverAddress.sin_family = AF_INET; //IP Domain Adress
    serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);
    serverAddress.sin_port = htons(this->port);
    if (bind(this->fd, (struct sockaddr *) &serverAddress, sizeof(struct sockaddr_in)) == -1)
    {
    	std::string msgError = "Error en apertura de conexion.";
        cout << msgError << endl;
        logger->log(msgError,ERROR);
        return false;
    }
    return true;
}

int Socket::get_fd()
{
	return this->fd;
}

void Socket::listenConnection(int maxConnections, Logger* logger)
{
	if(listen(this->fd,maxConnections)==-1)
	{
		std::string msgError = "Error al escuchar las conecciones entrantes.";
		cout << msgError << endl;
		logger->log(msgError,ERROR);
	}
}
