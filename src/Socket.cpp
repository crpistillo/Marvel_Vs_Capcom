/*
 * Socket.cpp
 *
 *  Created on: 11 may. 2019
 *      Author: caropistillo
 */

#include "Socket.h"
#include <sys/socket.h>

const string ERROR = "ERROR";
const string INFO = "INFO";
const string DEBUG = "DEBUG";

Socket::Socket() {
    this->fd = 0;
    this->port = 0;
}

bool Socket::create(Logger *logger) {
    this->fd = socket(AF_INET, SOCK_STREAM, 0); //AF_INET permite la comunicacion entre aplicaciones
    //corriendose en hosts conectados por IP version 4
    //SOCK_STREAM es el tipo de socket: reliable, bidirectional, byte-stream
    if (fd == -1) {
        std::string msgError = "Error al crear socket.";
        cout << msgError << endl;
        logger->log(msgError, ERROR);
        return false;
    }
    return true;
}

bool Socket::bindToAddress(int port, Logger *logger) //cuidado con pasar sin referencia!!
{
    this->port = port;
    struct sockaddr_in serverAddress;
    memset(&serverAddress, 0, sizeof(struct sockaddr_in)); //Clear structure
    serverAddress.sin_family = AF_INET; //IP Domain Adress
    serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);
    serverAddress.sin_port = htons(this->port);

    if (bind(this->fd, (struct sockaddr *) &serverAddress, sizeof(struct sockaddr_in)) == -1) {
        std::string msgError = "Error en apertura de conexion.";
        cout << msgError << endl;
        logger->log(msgError, ERROR);
        return false;
    }
    return true;
}

int Socket::get_fd() {
    return this->fd;
}

bool Socket::listenConnection(int maxConnections, Logger *logger) {
    if (listen(this->fd, maxConnections) == -1) {
        std::string msgError = "Error al escuchar las conecciones entrantes.";
        cout << msgError << endl;
        logger->log(msgError, ERROR);
        return false;
    }
    return true;
}

bool Socket::initialize(Logger *logger, int port, int maxConnections) {
    return (this->create(logger) && this->bindToAddress(port, logger) &&
            this->listenConnection(maxConnections, logger));

}

void Socket::acceptConnection(Socket *otherSocket, struct sockaddr_in *clientAddress, Logger *logger) {
    socklen_t sosize = sizeof(*clientAddress);
    this->fd = accept(otherSocket->get_fd(), (struct sockaddr *) &(*clientAddress), &sosize);
    if (this->fd == -1) {
        std::string msgError = "Error al escuchar las conecciones entrantes.";
        cout << msgError << endl;
        logger->log(msgError, ERROR);
    }
}

bool Socket::connectTo(string address, int listenPort) {

    struct sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = inet_addr(address.data());
    serverAddress.sin_port = htons(listenPort);
    socklen_t serverSize = sizeof(serverAddress);

    /*VERIFICACION DE ERRORES*/
    if (connect(fd, (struct sockaddr *) &serverAddress, serverSize) == -1) {
        string error = strerror(errno);
        //cout << "Error al conectar con el servidor " << strerror(errno) << endl;
        return false;
    }
    return true;
}

bool Socket::sendData(void *to_send, size_t size) {
    send(this->fd, to_send, size, 0);
}

bool Socket::reciveData(void* data, int size) {
    int totalRecived = 0, lastQuantityRecived = 0;
    while (totalRecived < size) {
        lastQuantityRecived = recv(fd, data, size, 0);
        if (lastQuantityRecived <= 0) {
            data = nullptr;
            return false;
        } else
            totalRecived += lastQuantityRecived;
    }
    return true;
}

void Socket::closeFd()
{
	close(this->fd);
}

void Socket::closeConnection()
{
	shutdown(this->fd, SHUT_RDWR);
}

