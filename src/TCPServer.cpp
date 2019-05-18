//
// Created by IgVelasco on 5/4/19.
//

#include "TCPServer.h"
#include "ServerThread.h"
#include "netdb.h"
#include "CharactersServer/SpidermanServer.h"
#include "Characters/Wolverine.h"
#include "CharactersServer/WolverineServer.h"
#include <string>
#include <pthread.h>

string TCPServer::Message;
const int maxConnections = 4;
const string ERROR = "ERROR";
const string INFO = "INFO";
const string DEBUG = "DEBUG";

pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;

TCPServer::TCPServer()
{
	this->numberOfConnections = 0;
	this->port = 0;
	this->serverSocket = new Socket();
	this->newSockFd = new Socket();
	this->team1 = NULL;
	this->team2 = NULL;
}

typedef struct{
	TCPServer* server;
	int clientSocket;
} info_for_thread_t;


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

void TCPServer::clientReceive(int socket){

	int socket_to_read = clientsSockets[socket];
	pthread_t asd = pthread_self();
	cout << "Hola! Soy el hilo: " + to_string(asd) + ". Soy el encargado de leer del socket: "
	+ to_string(socket_to_read) +"\n";

	while(1)
		continue;
}

int* TCPServer::getClientsSockets(){
	return this->clientsSockets;
}




/*Funcion que lee del socket la informacion que los clientes le envian.
 * Esta deberia leer, codificar y encolar eventos en la cola del servidor
 *
 * Son los denominados "thread lectura cliente x"*/
static void* ClientReceive(void* args){

	pthread_t hilo = pthread_self();

	//Recibo los argumentos y los casteo en el orden que corresponde.
	pthread_mutex_lock(&mtx);
	info_for_thread_t* arg = (info_for_thread_t*) args;
	TCPServer* server = arg->server;
	int socket = arg->clientSocket;
	pthread_mutex_unlock(&mtx);

	int socket_to_read = (server->getClientsSockets())[socket];
	char personaje[55];
	recv(socket_to_read, personaje, 55, 0);

	cout << "Hola! Soy el hilo: " + to_string(hilo) + ". Soy el encargado de leer del socket: "
	+ to_string(socket_to_read) + " y el mensaje que recibo es: "+ personaje +"\n";

	while(1)
		continue;

	return NULL;
}

int TCPServer::createReceivingThreadPerClient(){

	//Preparo los MAXPLAYERS argumentos a pasarle a la funcion del thread.
	info_for_thread_t *args[MAXPLAYERS];

	for(int i = 0; i < MAXPLAYERS; i++){
		info_for_thread_t* arg = (info_for_thread_t*) malloc(sizeof(info_for_thread_t));

		arg->clientSocket = i;
		arg->server = this;

		args[i] = arg;
	}

	//Creo MAXPLAYERS threads, uno por cada cliente conectado
	for(int i = 0; i < MAXPLAYERS; i++){

		pthread_mutex_lock(&mtx);
		if( pthread_create(&(this->clientsThreads[i]), NULL , ClientReceive , args[i] ) )
			return -1;
		pthread_mutex_unlock(&mtx);
	}

	return 0;
}

/*Esta funcion se encarga de desencolar datos de las colas de los clientes
 * y mandarle dicha informacion a cada cliente por el socket
 *
 * Son los denominados "thread escritura cliente x"
 * */
static void* ClientSend(void* args){

	pthread_t hilo = pthread_self();

	//Recibo los argumentos y los casteo en el orden que corresponde.
	pthread_mutex_lock(&mtx);
	info_for_thread_t* arg = (info_for_thread_t*) args;
	TCPServer* server = arg->server;
	int socket = arg->clientSocket;
	pthread_mutex_unlock(&mtx);

	int socket_to_send = (server->getClientsSockets())[socket];
	cout << "Hola! Soy el hilo: " + to_string(hilo) + ". Soy el encargado de mandar datos al socket: "
	+ to_string(socket_to_send) +"\n";

	while(1)
		continue;

	return NULL;
}

int TCPServer::createSendingThreadPerClient(){
	info_for_thread_t* args[MAXPLAYERS];

	for(int i = 0; i < MAXPLAYERS; i++){
		info_for_thread_t* arg = (info_for_thread_t*) malloc(sizeof(info_for_thread_t));

		arg->clientSocket = i;
		arg->server = this;

		args[i] = arg;
	}

	//Creo MAXPLAYERS threads, uno por cada cliente conectado
	for(int i = 0; i < MAXPLAYERS; i++){

		pthread_mutex_lock(&mtx);
		if( pthread_create(&(this->clientsThreads[i]), NULL , ClientSend , args[i] ) )
			return -1;
		pthread_mutex_unlock(&mtx);
	}

}

void TCPServer::runServer() {


    Socket *clientSocket1 = new Socket();
    Socket *clientSocket2 = new Socket();

    clientSocket1->fd = clientsSockets[0];
    clientSocket2->fd = clientsSockets[1];

    char character[9];

    clientSocket1->reciveData(character,9);
    CharacterServer* character1 = createServerCharacter(character);

    cout << character << endl;


    clientSocket1->reciveData(character, 9);
    CharacterServer* character2 = createServerCharacter(character);

    cout << character << endl;


    clientSocket2->reciveData(character,9);
    CharacterServer* character3 = createServerCharacter(character);

    cout << character << endl;

    clientSocket2->reciveData(character, 9);
    CharacterServer* character4 = createServerCharacter(character);

    cout << character << endl;
/*
    team1 = new Team(character1, character2, 2);
    team2 = new Team(character3, character4, 2);
*/







    //  createReceivingThreadPerClient();
    //createSendingThreadPerClient();

}

CharacterServer* TCPServer::createServerCharacter(char *character, int clientNumber ) {

 /*   int spidermanSobrante = widthSpiderman*242/640;
    int spidermanAncho= widthSpiderman*110/640;
    int wolverineSobrante = widthWolverine*278/640;
    int wolverineAncho= widthWolverine*87/640;

    int INITIAL_POS_X_PLAYER_ONE = ((LEVEL_WIDTH/2)-spidermanSobrante)-(spidermanAncho/2)-200;
    int INITIAL_POS_X_PLAYER_TWO = ((LEVEL_WIDTH/2)-wolverineSobrante)-(wolverineAncho/2)+200;

    if(string(character) == "Spiderman")
        SpidermanServer* characterToReturn = new SpidermanServer();
    else
        WolverineServer* characterToReturn = new WolverineServer()*/
 return NULL;

}

void* receiveInfo(void *infor){

}








