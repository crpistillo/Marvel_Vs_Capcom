//
// Created by IgVelasco on 5/4/19.
//

#include "TCPServer.h"

string TCPServer::Message;
const int maxConnections = 4;
const string ERROR = "ERROR";
const string INFO = "INFO";
const string DEBUG = "DEBUG";

TCPServer::TCPServer()
{
	this->numberOfConnections = 0;
	this->port = 0;
	this->sockfd = 0;
	this->newsockfd = 0;
	this->serverSocket = new Socket();
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

	logger->log("Comienza a iniciarse el servidor", INFO);
	logger->log("Se crea el socket de escucha del servidor", INFO);

	this->setSockfd(this->serverSocket->create(logger));

	string msgInfo = "El numero de puerto es: " + to_string(this->port);
	logger->log(msgInfo, INFO);

	this->serverSocket->bindToAddress(port,logger);

	this->serverSocket->listenConnection(maxConnections,logger);

	return true;
}

string TCPServer::receive() {
    string str;
    while (1) {
        socklen_t sosize = sizeof(clientAddress);
        newsockfd = accept(this->serverSocket->get_fd(), (struct sockaddr *) &clientAddress, &sosize);
        clientsSockets[numberOfConnections] = newsockfd;
        numberOfConnections++;

        if(numberOfConnections != MAXPLAYERS){
        	char *msjNotReady = (char*) "NOT READY\n";
        	for(int i = 0; i < numberOfConnections; i++){
        		send(clientsSockets[i], msjNotReady, strlen(msjNotReady),0 );
        	}
        }
        else{
        	char *msjReady = (char*) "READY\n";
        	for(int i = 0; i < numberOfConnections; i++){
        		send(clientsSockets[i], msjReady, strlen(msjReady),0 );
        	}
        }
        str = inet_ntoa(clientAddress.sin_addr);
        cout << str;
        //pthread_create(&serverThread, NULL, &Task, (void *) newsockfd);
    }
    return str;
}

string TCPServer::getMessage() {
    return Message;
}

void TCPServer::Send(string msg) {
    send(newsockfd, msg.c_str(), msg.length(), 0);
}

void TCPServer::clean() {
    Message = "";
    memset(msg, 0, MAXPACKETSIZE);
}

void TCPServer::detach() {
    close(this->serverSocket->get_fd() );
    close(newsockfd);
}

void TCPServer::setSockfd(int sockfd)
{
	this->sockfd = sockfd;
}
