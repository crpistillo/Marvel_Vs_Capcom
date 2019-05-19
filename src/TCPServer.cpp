//
// Created by IgVelasco on 5/4/19.
//

#include "TCPServer.h"
#include "ServerThread.h"
#include "netdb.h"
#include "CharactersServer/SpidermanServer.h"
#include "CharactersServer/WolverineServer.h"
#include <string>
#include <pthread.h>
#include "Constants.h"

Constants constants;
const int LEVEL_WIDTH = 3200;
const int LEVEL_HEIGHT = 600;

string TCPServer::Message;
const int maxConnections = 4;
const string ERROR = "ERROR";
const string INFO = "INFO";
const string DEBUG = "DEBUG";

pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;

TCPServer::TCPServer()
{
    this->logger = Logger::getInstance();
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

    for (auto & clientsSocket : clientsSockets) {
        Socket* sock = new Socket();
        clientsSocket = sock;
    }

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
        clientsSockets[numberOfConnections]->fd = newSockFd->fd;
        numberOfConnections++;

        //Aceptar conexiones pero seguir esperando por mas
        if(numberOfConnections != MAXPLAYERS){

        	to_send.status = NOT_READY;
        	to_send.nconnections = numberOfConnections;


        	for(int i = 0; i < numberOfConnections; i++){
        	    clientsSockets[i]->sendData(&to_send, sizeof(connection_information_t));

        		//send(clientsSockets[i], &to_send, sizeof(connection_information_t),0 );
        	}
        }

        //Maximo de clientes alcanzado, se inicia el juego.
        else{
        	to_send.status = READY;
        	to_send.nconnections = numberOfConnections;
        	for(int i = 0; i < numberOfConnections; i++){
                clientsSockets[i]->sendData(&to_send, sizeof(connection_information_t));
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

int computeDistance(CharacterServer* character1, CharacterServer* character2)
{
	int distancia;
	if (character1->getCentro() > character2->getCentro())
	{
		distancia = character1->getPosX() + character1->getSobrante() + character1->getWidth()
						- (character2->getPosX() + character2->getSobrante());
	}
	else
	{
		distancia = character1->getPosX() + character1->getSobrante()
				- (character2->getPosX() + character2->getSobrante() + character2->getWidth());
	}
	return distancia;
}


void TCPServer::clientReceive(int socket){

	Socket* socket_to_read = clientsSockets[socket];
	pthread_t asd = pthread_self();
	cout << "Hola! Soy el hilo: " + to_string(asd) + ". Soy el encargado de leer del socket: "
	+ to_string(socket_to_read->fd) +"\n";

	while(1)
	{
		//pop msges
	   	incoming_msg_t incoming_msg = this->incoming_msges_queue.get_data();
	   	this->incoming_msges_queue.delete_data();

	   	//update del team
	    int distancia = computeDistance(team1->get_currentCharacter(),team2->get_currentCharacter());

	   	character_updater_t update_msg;

	    if(incoming_msg.client == 0 || incoming_msg.client == 1)//team1 es de los clientes 1 y 2
	    {
	    	team1->update(distancia,team2->get_currentCharacter()->getPosX(), incoming_msg.action);
	    	update_msg.posX = team1->get_currentCharacter()->getPosX();
	    	update_msg.posY = team1->get_currentCharacter()->getPosY();
	    	update_msg.team = 1; //faltan mandar cosas?
	    }
	    else
	    {
	    	team2->update(distancia,team2->get_currentCharacter()->getPosX(),incoming_msg.action);
	    	update_msg.posX = team2->get_currentCharacter()->getPosX();
	        update_msg.posY = team2->get_currentCharacter()->getPosY();
	        update_msg.team = 2;
	    }

	   	this->character_updater_queue[incoming_msg.client]->insert(update_msg);

	   	continue;
	}
}

Socket** TCPServer::getClientsSockets(){
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

    Socket* socket_to_read = (server->getClientsSockets())[socket];
    char accion[sizeof(actions_t)];
    socket_to_read->reciveData(accion,sizeof(actions_t));

    /*cout << "Hola! Soy el hilo: " + to_string(hilo) + ". Soy el encargado de leer del socket: "
    + to_string(socket_to_read->fd) + " y el mensaje que recibo es: "+ accion +"\n";*/

    while(1){
        socket_to_read->reciveData(accion,sizeof(actions_t));
        actions_t* n = (actions_t*) accion;

        //Agrego elementos a la cola de mensajes entrantes
        incoming_msg_t aux;
        aux.action = *n;
        aux.client = arg->clientSocket;
        server->incoming_msges_queue.insert(aux);
        //////////////////////////////////////////////////

        string accion_palabra;
        switch (*n){
            case STANDING:
                accion_palabra = "STANDING";
                break;
            case MOVINGRIGHT:
                accion_palabra = "MOVINGRIGHT";
                break;
            case MOVINGLEFT:
                accion_palabra = "MOVINGLEFT";
                break;
            case JUMPINGVERTICAL:
                accion_palabra = "JUMPINGVERTICAL";
                break;
            case DUCK:
                accion_palabra = "DUCK";
                break;
            case JUMPINGRIGHT:
                accion_palabra = "JUMPINGRIGHT";
                break;
            case JUMPINGLEFT:
                accion_palabra = "JUMPINGLEFT";
                break;
            default:
                accion_palabra = "ALGUNA OTRA COSA";
                break;
        }
        cout << accion_palabra << endl;
        continue;
    }

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

	Socket* socket_to_send = (server->getClientsSockets())[socket];
	cout << "Hola! Soy el hilo: " + to_string(hilo) + ". Soy el encargado de mandar datos al socket: "
	+ to_string(socket_to_send->fd) +"\n";

	character_updater_t updater;


	while(1){
		server->teamOneMakeUpdater(&updater);
		socket_to_send->sendData(&updater, sizeof(character_updater_t));

		server->teamTwoMakeUpdater(&updater);
		socket_to_send->sendData(&updater, sizeof(character_updater_t));
	}

	return NULL;
}

void TCPServer::teamOneMakeUpdater(character_updater_t* updater){
	this->team1->makeUpdater(updater);
}

void TCPServer::teamTwoMakeUpdater(character_updater_t* updater){
	this->team2->makeUpdater(updater);
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

    char character[9];
    character_builder_t builder1;
    character_builder_t builder2;
    character_builder_t builder3;
    character_builder_t builder4;

    clientsSockets[0]->reciveData(character,9);
    CharacterServer* character1 = createServerCharacter(character, 1);
    character1->makeBuilderStruct(&builder1);

    cout << character << endl;


    clientsSockets[0]->reciveData(character, 9);
    CharacterServer* character2 = createServerCharacter(character, 1);
    character2->makeBuilderStruct(&builder2);
    cout << character << endl;


    clientsSockets[1]->reciveData(character,9);
    CharacterServer* character3 = createServerCharacter(character, 2);
    character3->makeBuilderStruct(&builder3);

    cout << character << endl;

    clientsSockets[1]->reciveData(character, 9);
    CharacterServer* character4 = createServerCharacter(character, 2);
    character4->makeBuilderStruct(&builder4);

    cout << character << endl;

    team1 = new Team(character1, character2, 1, 1);
    team2 = new Team(character3, character4, 1, 2);


    clientsSockets[0]->sendData(&builder1, sizeof(character_builder_t));
    clientsSockets[0]->sendData(&builder2, sizeof(character_builder_t));
    clientsSockets[0]->sendData(&builder3, sizeof(character_builder_t));
    clientsSockets[0]->sendData(&builder4, sizeof(character_builder_t));

    clientsSockets[1]->sendData(&builder1, sizeof(character_builder_t));
    clientsSockets[1]->sendData(&builder2, sizeof(character_builder_t));
    clientsSockets[1]->sendData(&builder3, sizeof(character_builder_t));
    clientsSockets[1]->sendData(&builder4, sizeof(character_builder_t));

    createReceivingThreadPerClient();
    createSendingThreadPerClient();

    while(1)
    	continue;

}

CharacterServer* TCPServer::createServerCharacter(char *character, int nclient) {

	CharacterServer* characterServer;
	character_number_t character_n;

	if( !strcmp(character,"Spiderman") )
		character_n = SPIDERMAN;
	if (!strcmp(character,"Wolverine"))
		character_n = WOLVERINE;


	switch(character_n){
	case SPIDERMAN:
		if(nclient < 2)
			characterServer = new SpidermanServer(constants.INITIAL_POS_X_PLAYER_ONE,
					false,
					constants.widthSpiderman,
					constants.heightSpiderman,
					constants.spidermanSobrante,
					constants.spidermanAncho,
					constants.screenWidth,
					nclient
					);
		else
			characterServer = new SpidermanServer(constants.INITIAL_POS_X_PLAYER_TWO,
					false,
					constants.widthSpiderman,
					constants.heightSpiderman,
					constants.spidermanSobrante,
					constants.spidermanAncho,
					constants.screenWidth,
					nclient
					);
		break;

	case WOLVERINE:
		if(nclient < 2)
			characterServer =  new WolverineServer(constants.INITIAL_POS_X_PLAYER_ONE,
					false,
					constants.widthWolverine,
					constants.heightWolverine,
					constants.wolverineSobrante,
					constants.wolverineAncho,
					constants.screenWidth,
					nclient
					);
		else
			characterServer = new WolverineServer(constants.INITIAL_POS_X_PLAYER_ONE,
				false,
				constants.widthWolverine,
				constants.heightWolverine,
				constants.wolverineSobrante,
				constants.wolverineAncho,
				constants.screenWidth,
				nclient
				);
	}

	return characterServer;
}

void* receiveInfo(void *infor){

}

void TCPServer::configJson(json config)
{
	this->logger = Logger::getInstance();

	this->config = config;

	json spidermanConfig = config["characters"][0];
	json wolverineConfig = config["characters"][1];

	string msj;

	if (spidermanConfig["name"] != "spiderman") {
		string name = spidermanConfig["name"];
	    string filepath = spidermanConfig["filepath"];
	    msj = "No se reconoce al personaje '" + name + "'."
	         + " Se intentara cargar las imagenes correspondiente al filepath: " + filepath
	         + " como las imagenes del personaje 'spiderman'.";
	    logger->log(msj, ERROR);
	}

	if (wolverineConfig["name"] != "wolverine") {
		string name = wolverineConfig["name"];
	    string filepath = wolverineConfig["filepath"];
	    msj = "No se reconoce al personaje '" + name + "'."
	         + " Se cargaran las imagenes correspondiente al filepath: " + filepath
	         + " como las imagenes del personaje 'wolverine'.";
	   logger->log(msj, ERROR);
    }

    constants.widthSpiderman = spidermanConfig["width"];
    constants.heightSpiderman = spidermanConfig["height"];
    constants.widthWolverine = wolverineConfig["width"];
    constants.heightWolverine = wolverineConfig["height"];

    string spidermanPath = spidermanConfig["filepath"];
    if (spidermanPath != "images/spiderman/spiderman_")
        logger->log("Filepath para personaje Spiderman incorrecto. Error al cargar imagenes.", ERROR);
    string wolverinePath = wolverineConfig["filepath"];
    if (wolverinePath != "images/wolverine/wolverine_")
        logger->log("Filepath para personaje Wolverine incorrecto. Error al cargar imagenes.", ERROR);

    constants.spidermanSobrante = constants.widthSpiderman * 242 / 640;
    constants.spidermanAncho = constants.widthSpiderman * 110 / 640;
    constants.wolverineSobrante = constants.widthWolverine * 278 / 640;
    constants.wolverineAncho = constants.widthWolverine * 87 / 640;

    constants.INITIAL_POS_X_PLAYER_ONE = ((LEVEL_WIDTH / 2) - constants.spidermanSobrante) - (constants.spidermanAncho / 2) - 200;
    constants.INITIAL_POS_X_PLAYER_TWO = ((LEVEL_WIDTH / 2) - constants.wolverineSobrante) - (constants.wolverineAncho / 2) + 200;

    constants.screenWidth = config["window"]["width"];
    constants.screenHeight = config["window"]["height"];

    logger->log("Creacion de personajes.", DEBUG);

}








