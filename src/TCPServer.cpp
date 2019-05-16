//
// Created by IgVelasco on 5/4/19.
//

#include "TCPServer.h"
#include "ServerThread.h"
#include "netdb.h"
#include <string>
#include <pthread.h>

string TCPServer::Message;
const int maxConnections = 4;
const string ERROR = "ERROR";
const string INFO = "INFO";
const string DEBUG = "DEBUG";

TCPServer::TCPServer()
{
	this->numberOfConnections = 0;
	this->port = 0;
	this->serverSocket = new Socket();
	this->newSockFd = new Socket();
}

void *TCPServer::Task(void *arg) {
    int n;
    int newsockfd = (long) arg;
    char msg[MAXPACKETSIZE];
    pthread_detach(pthread_self());
    while (1) {
        n = recv(newsockfd, msg, MAXPACKETSIZE, 0);
        if (n == 0) {
            close(newsockfd);
            break;
        }
        msg[n] = 0;
        //send(newsockfd,msg,n,0);
        Message = string(msg);
    }
    return 0;
}

bool TCPServer::setup(int port, Logger* logger) {

	this->logger = logger;

	logger->log("Comienza a iniciarse el servidor", INFO);
	logger->log("Se crea el socket de escucha del servidor", INFO);

	this->port = port;
	string msgInfo = "Serving on port " + to_string(this->port);
	cout<<msgInfo<<endl;
	logger->log(msgInfo, INFO);

	bool ret = this->serverSocket->initialize(logger, port, maxConnections);

	//this->setSockfd(this->serverSocket->get_fd());

	return ret;
}

void TCPServer::receive() {
    string str;
    while (1) {

    	struct sockaddr_in clientAddress;

        newSockFd->acceptConnection(this->serverSocket, &clientAddress, logger);
        logger->log("Nueva conexion aceptada", INFO);

        connection_information_t to_send;

        socklen_t clientAddress_len = sizeof(clientAddress);

        this->reportClientConnected(&clientAddress, clientAddress_len, logger);


        //Rechazar conexiones
        if(numberOfConnections == MAXPLAYERS){

        	to_send.nconnections = numberOfConnections;
        	to_send.status = NO_MORE_CONNECTIONS_ALLOWED;

        	send(newSockFd->get_fd(), &to_send, sizeof(connection_information_t) , 0);
        	close(newSockFd->get_fd());
        	continue;

        }
        clientsSockets[numberOfConnections] = newSockFd ->get_fd();
        numberOfConnections++;

        //Aceptar conexiones pero seguir esperando por mas
        if(numberOfConnections != MAXPLAYERS){

        	to_send.status = NOT_READY;
        	to_send.nconnections = numberOfConnections;


        	for(int i = 0; i < numberOfConnections; i++){
        		send(clientsSockets[i], &to_send, sizeof(connection_information_t),0 );
        	}
        }

        //Maximo de clientes alcanzado, se inicia el juego.
        else{
        	to_send.status = READY;
        	to_send.nconnections = numberOfConnections;
        	for(int i = 0; i < numberOfConnections; i++){
        		send(clientsSockets[i], &to_send, sizeof(connection_information_t),0 );
        	}
        }
        str = inet_ntoa(clientAddress.sin_addr);
        cout << str + "\n";

    }
}

string TCPServer::getMessage() {
    return Message;
}

void TCPServer::Send(string msg) {
    send(newSockFd->get_fd(), msg.c_str(), msg.length(), 0);
}

void TCPServer::clean() {
    Message = "";
    memset(msg, 0, MAXPACKETSIZE);
}

void TCPServer::detach() {
    close(this->serverSocket->get_fd() );
    close(newSockFd->get_fd());
}

void TCPServer::reportClientConnected(const struct sockaddr_in* clientAddress, socklen_t clientAddress_len, Logger* logger)
{

	char hostbuf[NI_MAXHOST];
	char portbuf[NI_MAXSERV];
    if (getnameinfo((struct sockaddr*)clientAddress, clientAddress_len,
    		hostbuf, NI_MAXHOST, portbuf, NI_MAXSERV, 0) == 0)
    {
    	string msg = "Client " + string(hostbuf) + ", " + string(portbuf) + " connected";
    	cout<<msg<<endl;
    	logger->log(msg, INFO);
	}
    else
    {
    	cout<<"Client unknown connected "<<endl;
    }
    return;
}

static void* wrapperReceive(void* args){
	TCPServer *server = (TCPServer*) args;
	server->receive();
	return 0;
}

int TCPServer::createAceptingThread() {
	return ( pthread_create( &(this->acceptThread), NULL, wrapperReceive, this) );
}

int TCPServer::getNumberOfConections(){
	return numberOfConnections;
}





