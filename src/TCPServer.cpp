//
// Created by IgVelasco on 5/4/19.
//

#include "TCPServer.h"
#include "netdb.h"
#include "CharactersServer/SpidermanServer.h"
#include "CharactersServer/WolverineServer.h"
#include <string>
#include <pthread.h>
#include <thread>
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

TCPServer::TCPServer() {
    this->logger = Logger::getInstance();
    this->numberOfConnections = 0;
    this->port = 0;
    this->serverSocket = new Socket();
    this->newSockFd = new Socket();
    this->team1 = NULL;
    this->team2 = NULL;
}

typedef struct {
    TCPServer *server;
    int clientSocket;
} info_for_thread_t;


bool TCPServer::setup(int port, Logger *logger) {

    this->logger = logger;

    logger->log("Comienza a iniciarse el servidor", INFO);
    logger->log("Se crea el socket de escucha del servidor", INFO);

    this->port = port;
    string msgInfo = "Serving on port " + to_string(this->port);
    cout << msgInfo << endl;
    logger->log(msgInfo, INFO);

    bool ret = this->serverSocket->initialize(logger, port, maxConnections);

    for (auto &clientsSocket : clientsSockets) {
        Socket *sock = new Socket();
        clientsSocket = sock;
    }

    this->incoming_msges_queue = new Queue<incoming_msg_t *>;
    this->incoming_menu_actions_queue = new Queue<cliente_menu_t*>;


    for (int i = 0; i < MAXPLAYERS; ++i) {
        this->character_updater_queue[i] = new Queue<character_updater_t *>;
    }

    for (int i = 0; i < MAXPLAYERS; ++i) {
        this->cursor_updater_queue[i] = new Queue<cursor_updater_t*>;
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
        if (numberOfConnections == MAXPLAYERS) {

            to_send.nconnections = numberOfConnections;
            to_send.status = NO_MORE_CONNECTIONS_ALLOWED;

            send(newSockFd->get_fd(), &to_send, sizeof(connection_information_t), 0);
            close(newSockFd->get_fd());
            continue;

        }
        clientsSockets[numberOfConnections]->fd = newSockFd->fd;
        numberOfConnections++;

        //Aceptar conexiones pero seguir esperando por mas
        if (numberOfConnections != MAXPLAYERS) {

            to_send.status = NOT_READY;
            to_send.nconnections = numberOfConnections;


            for (int i = 0; i < numberOfConnections; i++) {
                clientsSockets[i]->sendData(&to_send, sizeof(connection_information_t));

                //send(clientsSockets[i], &to_send, sizeof(connection_information_t),0 );
            }
        }

            //Maximo de clientes alcanzado, se inicia el juego.
        else {
            to_send.status = READY;
            to_send.nconnections = numberOfConnections;
            for (int i = 0; i < numberOfConnections; i++) {
                clientsSockets[i]->sendData(&to_send, sizeof(connection_information_t));
            }
        }
        str = inet_ntoa(clientAddress.sin_addr);
        cout << str + "\n";

    }
}

void TCPServer::clean() {
    Message = "";
    memset(msg, 0, MAXPACKETSIZE);
}

void TCPServer::detach() {
    close(this->serverSocket->get_fd());
    close(newSockFd->get_fd());
}

void
TCPServer::reportClientConnected(const struct sockaddr_in *clientAddress, socklen_t clientAddress_len, Logger *logger) {

    char hostbuf[NI_MAXHOST];using namespace std;
    char portbuf[NI_MAXSERV];
    if (getnameinfo((struct sockaddr *) clientAddress, clientAddress_len,
                    hostbuf, NI_MAXHOST, portbuf, NI_MAXSERV, 0) == 0) {
        string msg = "Client " + string(hostbuf) + ", " + string(portbuf) + " connected";
        cout << msg << endl;
        logger->log(msg, INFO);
    } else {
        cout << "Client unknown connected " << endl;
    }
    return;
}

int TCPServer::getNumberOfConections() {
    std::unique_lock<std::mutex> lock(m);
    return numberOfConnections;
}

int computeDistance(CharacterServer *character1, CharacterServer *character2) {
    int distancia;
    if (character1->getCentro() > character2->getCentro()) {
        distancia = character1->getPosX() + character1->getSobrante() + character1->getWidth()
                    - (character2->getPosX() + character2->getSobrante());
    } else {
        distancia = character1->getPosX() + character1->getSobrante()
                    - (character2->getPosX() + character2->getSobrante() + character2->getWidth());
    }
    return distancia;
}

int computeDistance2(CharacterServer *character1, CharacterServer *character2)
{
	int distancia2;
	if (character1->getCentro() > character2->getCentro())
	{
			distancia2 = character2->getPosX()+character2->getSobrante()
					- (character1->getPosX()+character1->getSobrante()+character1->getWidth());
	}
	else
	{
		distancia2 = character2->getPosX()+character2->getSobrante()
				 + character2->getWidth() - (character1->getPosX() + character1->getSobrante());
	}

	return distancia2;

}

/*Funcion que lee del socket la informacion que los clientes le envian.
 * Esta deberia leer, codificar y encolar eventos en la cola del servidor
 *
 * Son los denominados "thread lectura cliente x"*/
void TCPServer::receiveFromClient(int clientSocket) {


    //Recibo los argumentos y los casteo en el orden que corresponde.
    pthread_mutex_lock(&mtx);
    Socket *socket = getClientSocket(clientSocket);
    pthread_mutex_unlock(&mtx);

    char buf[sizeof(actions_t)];

    /*cout << "Hola! Soy el hilo: " + to_string(hilo) + ". Soy el encargado de leer del socket: "
    + to_string(socket_to_read->fd) + " y el mensaje que recibo es: "+ accion +"\n";*/

    while (1) {

        socket->reciveData(buf, sizeof(actions_t));
        actions_t *accion = (actions_t *) buf;

        //Agrego elementos a la cola de mensajes entrantes
        //void* action = malloc(sizeof(incoming_msg_t));
        incoming_msg_t *msgQueue = new incoming_msg_t;
        msgQueue->action = *accion;
        msgQueue->client = clientSocket;
        this->incoming_msges_queue->insert(msgQueue);

        continue;
    }
}

/*Esta funcion se encarga de desencolar datos de las colas de los clientes
 * y mandarle dicha informacion a cada cliente por el socket
 *
 * Son los denominados "thread escritura cliente x"
 * */

Socket* TCPServer::getClientSocket(int i){
    std::unique_lock<std::mutex> lock(m);
    return clientsSockets[i];
}

void TCPServer::sendToClient(int clientSocket) {

    pthread_t hilo = pthread_self();

    //Recibo los argumentos y los casteo en el orden que corresponde.
    pthread_mutex_lock(&mtx);
    Socket *socket = getClientSocket(clientSocket);
    pthread_mutex_unlock(&mtx);

    cout << "Hola! Soy el hilo: " + to_string(hilo) + ". Soy el encargado de mandar datos al socket: "
            + to_string(socket->fd) + "\n";

    //character_updater_t updater;

    while (1) {

        character_updater_t *updater;
        if(character_updater_queue[clientSocket]->empty_queue())
            continue;
        updater = character_updater_queue[clientSocket]->get_data();
        socket->sendData(updater, sizeof(character_updater_t));
        character_updater_queue[clientSocket]->delete_data();
    }
}


void TCPServer::runServer() {

    std::thread receive(&TCPServer::receive, this);
    while (getNumberOfConections() != MAXPLAYERS)    //Espera hasta que se conecten MAXPLAYERS
        continue;

    cout << "Numero de jugadores alcanzado! \n";


    char character[9];
    character_builder_t builder1;
    character_builder_t builder2;
    character_builder_t builder3;
    character_builder_t builder4;

    clientsSockets[0]->reciveData(character, 9);
    CharacterServer *character1 = createServerCharacter(character, 1);
    character1->makeBuilderStruct(&builder1);

    cout << character << endl;


    clientsSockets[0]->reciveData(character, 9);
    CharacterServer *character2 = createServerCharacter(character, 1);
    character2->makeBuilderStruct(&builder2);
    cout << character << endl;


    clientsSockets[1]->reciveData(character, 9);
    CharacterServer *character3 = createServerCharacter(character, 2);
    character3->makeBuilderStruct(&builder3);

    cout << character << endl;

    clientsSockets[1]->reciveData(character, 9);
    CharacterServer *character4 = createServerCharacter(character, 2);
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


    runMenuPhase();  //Pongo al servidor en modo "Menu"

    //Cuando termina el menu, empieza el juego
    std::thread receive1(&TCPServer::receiveFromClient, this, 0);
    std::thread receive2(&TCPServer::receiveFromClient, this, 1);

    std::thread send1(&TCPServer::sendToClient, this, 0);
    std::thread send2(&TCPServer::sendToClient, this, 1);

    while (1) {
        incoming_msg_t *incoming_msg;
        if(incoming_msges_queue->empty_queue())
            continue;
        incoming_msg = this->incoming_msges_queue->get_data();

        int distancia = computeDistance(team1->get_currentCharacter(), team2->get_currentCharacter());

        int distancia2 = computeDistance2(team1->get_currentCharacter(),team2->get_currentCharacter());

        character_updater_t *update_msg = new character_updater_t;

        bool changeAvailable = false;

        if (incoming_msg->client == 0)//team1 es de los clientes 1 y 2
        {
            if(team1->get_currentCharacter()->isStanding() && incoming_msg->action == CHANGEME){
                update_msg->action = CHANGEME;
                team1->update(distancia, team1->get_currentCharacter()->getPosX(), incoming_msg->action);
            }
            else if (team1->invalidIntroAction() && incoming_msg->action == CHANGEME){
            	update_msg->action = team1->get_currentCharacter()->currentAction;
            	team1->update(distancia, team2->get_currentCharacter()->getPosX(),
            			team1->get_currentCharacter()->currentAction);
            }
            else{
                team1->update(distancia, team2->get_currentCharacter()->getPosX(), incoming_msg->action);
                update_msg->action = team1->get_currentCharacter()->getCurrentAction();
            }
            update_msg->posX = team1->get_currentCharacter()->getPosX();
            update_msg->posY = team1->get_currentCharacter()->getPosY();
            update_msg->team = 1;
            update_msg->currentSprite = team1->get_currentCharacter()->getSpriteNumber();
        } else {
            if(team2->get_currentCharacter()->isStanding() && incoming_msg->action == CHANGEME)
            {
                update_msg->action = CHANGEME;
                team2->update(distancia2, team1->get_currentCharacter()->getPosX(), incoming_msg->action);
            }
            else if (team2->invalidIntroAction() && incoming_msg->action == CHANGEME)
            {
                update_msg->action = team2->get_currentCharacter()->currentAction;
                team2->update(distancia2, team1->get_currentCharacter()->getPosX(),
                		team2->get_currentCharacter()->currentAction);
            }
            else
            {
                team2->update(distancia2, team1->get_currentCharacter()->getPosX(), incoming_msg->action);
                update_msg->action = team2->get_currentCharacter()->getCurrentAction();
            }
            update_msg->posX = team2->get_currentCharacter()->getPosX();
            update_msg->posY = team2->get_currentCharacter()->getPosY();
            update_msg->team = 2;
            update_msg->currentSprite = team2->get_currentCharacter()->getSpriteNumber();
        }

        character_updater_t* update[MAXPLAYERS];
        for (int j = 0; j < MAXPLAYERS; ++j) {
            update[j] = new character_updater_t;
            update[j]->action = update_msg->action;
            update[j]->team = update_msg->team;
            update[j]->posX = update_msg->posX;
            update[j]->posY = update_msg->posY;
            update[j]->currentSprite = update_msg->currentSprite;
        }

        for (int i = 0; i < MAXPLAYERS; ++i) {
            std::unique_lock<std::mutex> lock(m);
            this->character_updater_queue[i]->insert(update[i]);
        }

        incoming_msges_queue->delete_data();
    }

}

void TCPServer::receiveMenuActionsFromClient(int clientSocket){

	pthread_mutex_lock(&mtx);
    Socket *socket = getClientSocket(clientSocket);
    pthread_mutex_unlock(&mtx);

    char buf[sizeof(menu_action_t)];

    while (1) {

        socket->reciveData(buf, sizeof(menu_action_t));
        menu_action_t *accion = (menu_action_t *) buf;

        //cout << "Accion del cliente " + to_string(clientSocket) + " : "
        //		+ to_string(*accion) << endl;

        cliente_menu_t *msgMenuQueue = new cliente_menu_t;
        msgMenuQueue->cliente = clientSocket;
        msgMenuQueue->accion = *accion;
        this->incoming_menu_actions_queue->insert(msgMenuQueue);
    }

}

void TCPServer::sendCursorUpdaterToClient(int clientSocket){
	pthread_mutex_lock(&mtx);
    Socket *socket = getClientSocket(clientSocket);
    pthread_mutex_unlock(&mtx);

    while(1){

        cursor_updater_t *updater;
        if(cursor_updater_queue[clientSocket]->empty_queue())
            continue;

        menuClient.lock();
        updater = cursor_updater_queue[clientSocket]->get_data();
        menuClient.unlock();

        socket->sendData(updater, sizeof(cursor_updater_t));
        menuClient.lock();
        cursor_updater_queue[clientSocket]->delete_data();
        menuClient.unlock();
    }
}


void TCPServer::runMenuPhase(){

	//Crear hilos de escucha a los 4 clientes, que encolen en la cola de arriba
	std::thread threadReceiveCliente1 (&TCPServer::receiveMenuActionsFromClient, this, 0);
	threadReceiveCliente1.detach();
	std::thread threadReceiveCliente2 (&TCPServer::receiveMenuActionsFromClient, this, 1);
	threadReceiveCliente2.detach();

	//Crear hilos de escritura a los clientes
	std::thread threadSendCliente1 (&TCPServer::sendCursorUpdaterToClient, this, 0);
	threadSendCliente1.detach();
	std::thread threadSendCliente2 (&TCPServer::sendCursorUpdaterToClient, this, 1);
	threadSendCliente2.detach();

	serverCursors[0] = new ServerCursor(97, 61);
	serverCursors[1] = new ServerCursor(449, 61);
	serverCursors[2] = new ServerCursor(97, 353);
	serverCursors[3] = new ServerCursor(449, 353);

	//Procesar eventos que vengan de incoming_menu_actions_queue
	while(1){
		cliente_menu_t *incoming_msg;
		if(this->incoming_menu_actions_queue->empty_queue())
			continue;
		incoming_msg = this->incoming_menu_actions_queue->get_data();

		cout << "Accion del cliente " + to_string(incoming_msg->cliente) + " : "
				+ to_string(incoming_msg->accion) << endl;



		/*======================================
		 * Aca
		 * Procesar el evento del menu que viene
		 *
		 =======================================*/
		processMenuAction(incoming_msg);


        cursor_updater_t* update[4];

        for (int i = 0; i < 4; i++){
        	update[i] = new cursor_updater_t;
        	update[i]->cliente = i;
        	update[i]->menuTerminated = false;
        	serverCursors[i]->makeMenuUpdater(update[i]);
        }


        /*for (int j = 0; j < 4; ++j) {
            update[j] = new cursor_updater_t;
            update[j]->cliente = j;
            if( j == 0 ){
            update[j]->posX = 97;
            update[j]->posY = 61;
            }
            else if ( j == 1 ){
                update[j]->posX = 449;
                update[j]->posY = 61;
            }
            else if ( j == 2 ){
                update[j]->posX = 97;
                update[j]->posY = 353;
            }
            else if ( j == 3 ){
                update[j]->posX = 449;
                update[j]->posY = 353;
            }

            update[j]->finalSelection = false;
            update[j]->menuTerminated = false;
        }
        */


        menuClient.lock();
        for (int i = 0; i < MAXPLAYERS; ++i) {
        	for (int j = 0; j < 4; j++ ){
        		this->cursor_updater_queue[i]->insert(update[j]);
        	}
        }

        menuClient.unlock();


		incoming_menu_actions_queue->delete_data();
		delete incoming_msg;
	}

	while(1)
		continue;


}

void TCPServer::processMenuAction(cliente_menu_t *action_msg){
	this->serverCursors[action_msg->cliente]->update(action_msg);
}

CharacterServer *TCPServer::createServerCharacter(char *character, int nclient) {

    CharacterServer *characterServer;
    character_number_t character_n;

    if (!strcmp(character, "Spiderman"))
        character_n = SPIDERMAN;
    if (!strcmp(character, "Wolverine"))
        character_n = WOLVERINE;


    switch (character_n) {
        case SPIDERMAN:
            if (nclient < 2)
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
            if (nclient < 2)
                characterServer = new WolverineServer(constants.INITIAL_POS_X_PLAYER_ONE,
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

void TCPServer::configJson(json config) {
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

    constants.INITIAL_POS_X_PLAYER_ONE =
            ((LEVEL_WIDTH / 2) - constants.spidermanSobrante) - (constants.spidermanAncho / 2) - 200;
    constants.INITIAL_POS_X_PLAYER_TWO =
            ((LEVEL_WIDTH / 2) - constants.wolverineSobrante) - (constants.wolverineAncho / 2) + 200;

    constants.screenWidth = config["window"]["width"];
    constants.screenHeight = config["window"]["height"];

    logger->log("Creacion de personajes.", DEBUG);

}








