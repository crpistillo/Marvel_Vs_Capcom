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

}

typedef struct {
    TCPServer *server;
    int clientSocket;
} info_for_thread_t;


bool TCPServer::setup(int port, Logger *logger, int numberOfPlayers) {

    this->numberOfPlayers = numberOfPlayers;

    this->logger = logger;
    logger->log("Comienza a iniciarse el servidor", INFO);
    logger->log("Se crea el socket de escucha del servidor", INFO);

    this->port = port;
    string msgInfo = "Serving on port " + to_string(this->port);
    cout << msgInfo << endl;
    logger->log(msgInfo, INFO);

    bool ret = this->serverSocket->initialize(logger, port, numberOfPlayers);

    for (auto &clientsSocket : clientsSockets) {
        Socket *sock = new Socket();
        clientsSocket = sock;
    }

    if (numberOfPlayers == 2) { //good hardcodeo
        clientsSockets[0]->receivingFromClient = true;
        clientsSockets[1]->receivingFromClient = true;
    } else {
        clientsSockets[0]->receivingFromClient = true;
        clientsSockets[3]->receivingFromClient = true;
    }

    this->incoming_msges_queue = new Queue<incoming_msg_t *>;
    this->incoming_menu_actions_queue = new Queue<cliente_menu_t*>;

    for (int i = 0; i < numberOfPlayers; ++i) {
        this->client_updater_queue[i] = new Queue<character_updater_t *>;
        this->cursor_updater_queue[i] = new Queue<cursor_updater_t*>;
    }

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
        if (numberOfConnections == numberOfPlayers) {

            to_send.nconnections = numberOfConnections;
            to_send.status = NO_MORE_CONNECTIONS_ALLOWED;

            send(newSockFd->get_fd(), &to_send, sizeof(connection_information_t), 0);
            close(newSockFd->get_fd());
            continue;

        }
        clientsSockets[numberOfConnections]->fd = newSockFd->fd;
        numberOfConnections++;

        //Aceptar conexiones pero seguir esperando por mas
        if (numberOfConnections != numberOfPlayers) {

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



/*Funcion que lee del socket la informacion que los clientes le envian.
 * Esta deberia leer, codificar y encolar eventos en la cola del servidor
 *
 * Son los denominados "thread lectura cliente x"*/
void TCPServer::receiveFromClient(int clientSocket) {


    //Recibo los argumentos y los casteo en el orden que corresponde.
    Socket *socket = getClientSocket(clientSocket);

    char buf[sizeof(actions_t)];
    while (1) {

        socket->reciveData(buf, sizeof(actions_t));
        actions_t *accion = (actions_t *) buf;

        //Agrego elementos a la cola de mensajes entrantes
        //void* action = malloc(sizeof(incoming_msg_t));
        incoming_msg_t *msgQueue = new incoming_msg_t;
        msgQueue->action = *accion;
        msgQueue->client = clientSocket;
        this->incoming_msges_queue->insert(msgQueue);

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

    //Recibo los argumentos y los casteo en el orden que corresponde.
    Socket *socket = getClientSocket(clientSocket);


    while (1) {

        character_updater_t *updater;
        if(client_updater_queue[clientSocket]->empty_queue())
            continue;
        updater = client_updater_queue[clientSocket]->get_data();
        socket->sendData(updater, sizeof(character_updater_t));
        client_updater_queue[clientSocket]->delete_data();
    }
}


void TCPServer::runServer() {

    std::thread receive(&TCPServer::receive, this);
    while (getNumberOfConections() != numberOfPlayers)    //Espera hasta que se conecten MAXPLAYERS
        continue;

    cout << "Numero de jugadores alcanzado! \n";

    for (int j = 0; j < numberOfPlayers; ++j) {
        clientsSockets[j]->sendData(&numberOfPlayers,sizeof(int));
    }
    runMenuPhase();  //Pongo al servidor en modo "Menu"
    sendSelectedCharacters();

    std::thread receiveFromClienThreads[numberOfPlayers];
    std::thread sendToClientThreats[numberOfPlayers];

    for (int i = 0; i < numberOfPlayers; ++i) {
        receiveFromClienThreads[i] = std::thread(&TCPServer::receiveFromClient, this, i);
        sendToClientThreats[i] = std::thread(&TCPServer::sendToClient, this, i);
    }

    updateModel();
}



void TCPServer::sendSelectedCharacters(){

    int charactersPerClient;

    if (2 == numberOfPlayers)   // si el maximo de players es 2 elijen 2
        charactersPerClient = 2;
    else
        charactersPerClient = 1;

    CharacterServer *characters[MAXPLAYERS];
    character_builder_t builders[MAXPLAYERS];

    char character[9];

    int nclient = 0;
    int nCharacter = 0;
    for (int i = 0; i < numberOfPlayers; i++) {    // de 0 a 4  o de 0 a 2
        for (int j = 0; j < charactersPerClient; j++) {  // si characters es 1 entra 1 vez
            characters[nCharacter] = createServerCharacterFromCursor(serverCursors[nCharacter], nclient, nCharacter);
            characters[nCharacter]->makeBuilderStruct(&builders[nCharacter], nCharacter < 2);
            nCharacter++;
        }
        nclient++;
    }

    team[0] = new Team(characters[0], characters[1], 1, 1);
    team[1] = new Team(characters[2], characters[3], 1, 2);

    for (auto &builder : builders) {
        for (int i = 0; i < numberOfPlayers; ++i) {
            clientsSockets[i]->sendData(&builder, sizeof(character_builder_t));
        }
    }
}


//nclient = numero del cliente
//characterNumber  = numero del personaje es decir el team 1 tiene 1 y 2 y el team 2 tiene 3 y 4 no importa el numero
//de clientes
CharacterServer * TCPServer::createServerCharacterFromCursor(ServerCursor *cursor, int nclient, int characterNumber) {

    CharacterServer *characterServer;
    int pos;
    if (characterNumber < 2)
        pos = constants.INITIAL_POS_X_PLAYER_ONE;
    else
        pos = constants.INITIAL_POS_X_PLAYER_TWO;


    switch (cursor->getCharacterSelected()) {
        case SPIDERMAN:
            characterServer = new SpidermanServer(pos,
                                                  constants.heightSpiderman,
                                                  constants.spidermanSobrante,
                                                  constants.spidermanAncho,
                                                  constants.screenWidth,
                                                  nclient, 0);
            break;

        case WOLVERINE:
            characterServer = new WolverineServer(pos,
                                                  constants.heightWolverine,
                                                  constants.wolverineSobrante,
                                                  constants.wolverineAncho,
                                                  constants.screenWidth,
                                                  nclient, 0);
    }
    return characterServer;
}

void TCPServer::receiveMenuActionsFromClient(int clientSocket){


    Socket *socket = getClientSocket(clientSocket);

    char buf[sizeof(menu_action_t)];

    while (1) {

        socket->reciveData(buf, sizeof(menu_action_t));
        menu_action_t *accion = (menu_action_t *) buf;

        cout << "Accion del cliente " + to_string(clientSocket) + " : "
        		+ to_string(*accion) << endl;

        cliente_menu_t *msgMenuQueue = new cliente_menu_t;
        msgMenuQueue->cliente = clientSocket;
        msgMenuQueue->accion = *accion;

        this->incoming_menu_actions_queue->insert(msgMenuQueue);
    }

}

void TCPServer::sendCursorUpdaterToClient(int clientSocket){
    Socket *socket = getClientSocket(clientSocket);

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

        if (updater->menuTerminated)
        	break;
    }

    cout << "Bye" << endl;
}


void TCPServer::runMenuPhase(){

	//Crear hilos de escucha a los 4 clientes, que encolen en la cola de arriba

    std::thread receiveFromClientThreads[numberOfPlayers];
    std::thread sendToClientThreats[numberOfPlayers];

    for (int i = 0; i < numberOfPlayers; ++i) {
        receiveFromClientThreads[i] = std::thread(&TCPServer::receiveMenuActionsFromClient, this, i);
        receiveFromClientThreads[i].detach();
        sendToClientThreats[i] = std::thread(&TCPServer::sendCursorUpdaterToClient, this, i);
        sendToClientThreats[i].detach();
    }


	serverCursors[0] = new ServerCursor(97, 61);
	serverCursors[1] = new ServerCursor(449, 353);
	serverCursors[2] = new ServerCursor(97, 353);
	serverCursors[3] = new ServerCursor(449, 353);

	//Procesar eventos que vengan de incoming_menu_actions_queue
	while(1){
		cliente_menu_t *incoming_msg;
		if(this->incoming_menu_actions_queue->empty_queue())
			continue;
		incoming_msg = this->incoming_menu_actions_queue->get_data();


		/* Proceso el evento */
		bool validMenuAction = processMenuAction(incoming_msg);

		/* Solo envio información a los clientes si hubo algun cambio */
		if(validMenuAction)
			sendUpdaters(false);


		incoming_menu_actions_queue->delete_data();
		delete incoming_msg;

		/* Verifico si ya seleccionaron todos */
		int charactersSelected = getNumberOfCharactersSelected();
		if (charactersSelected == 2)
			break;
	}

    for (int i = 0; i < numberOfPlayers; ++i) {
        receiveFromClientThreads[i].~thread();
        sendToClientThreats[i].~thread();
    }

	sendUpdaters(true);
}

void TCPServer::sendUpdaters(bool finalUpdater){

	cursor_updater_t* update[4];

	for (int i = 0; i < 4; i++){
		update[i] = new cursor_updater_t;
		update[i]->cliente = i;
		update[i]->menuTerminated = finalUpdater;
		serverCursors[i]->makeMenuUpdater(update[i]);
	}

	menuClient.lock();
	for (int i = 0; i < numberOfPlayers; ++i) {
		for (int j = 0; j < numberOfPlayers; j++ ){
			this->cursor_updater_queue[i]->insert(update[j]);
		}
	}

	menuClient.unlock();
}



int TCPServer::getNumberOfCharactersSelected(){

	int n = 0;
	for (int i = 0; i < MAXPLAYERS; i++){
		if(serverCursors[i]->getFinalSelection())
			n++;
	}

	return n;

}

bool TCPServer::processMenuAction(cliente_menu_t *action_msg){
	return this->serverCursors[action_msg->cliente]->update(action_msg);
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

void TCPServer::updateModel() {

    while (1) {
        incoming_msg_t *incoming_msg;
        if (incoming_msges_queue->empty_queue())
            continue;
        incoming_msg = this->incoming_msges_queue->get_data();

        int distancia[2];
        distancia[0] = computeDistance(team[0]->get_currentCharacter(), team[1]->get_currentCharacter());
        distancia[1] = computeDistance(team[1]->get_currentCharacter(), team[0]->get_currentCharacter());

        character_updater_t *update_msg = new character_updater_t;

        int teamToUpdate;
        int enemyTeam;
        if (incoming_msg->client == 0 || incoming_msg->client == 1) {//team1 es de los clientes 1 y 2
            teamToUpdate = 0;
            enemyTeam = 1;
        } else {
            teamToUpdate = 1;
            enemyTeam = 0;
        }

        if (team[teamToUpdate]->get_currentCharacter()->isStanding() && incoming_msg->action == CHANGEME) {
            update_msg->action = CHANGEME;
            team[teamToUpdate]->update(distancia[teamToUpdate], team[teamToUpdate]->get_currentCharacter()->getPosX(),
                                       incoming_msg->action, this->clientsSockets);
        } else if (team[teamToUpdate]->invalidIntroAction() && incoming_msg->action == CHANGEME) {
            update_msg->action = team[teamToUpdate]->get_currentCharacter()->currentAction;
            team[teamToUpdate]->update(distancia[teamToUpdate], team[enemyTeam]->get_currentCharacter()->getPosX(),
                                       team[teamToUpdate]->get_currentCharacter()->currentAction, this->clientsSockets);
        } else {
            team[teamToUpdate]->update(distancia[teamToUpdate], team[enemyTeam]->get_currentCharacter()->getPosX(),
                                       incoming_msg->action, this->clientsSockets);
            update_msg->action = team[teamToUpdate]->get_currentCharacter()->getCurrentAction();
        }

        update_msg->posX = team[teamToUpdate]->get_currentCharacter()->getPosX();
        update_msg->posY = team[teamToUpdate]->get_currentCharacter()->getPosY();
        update_msg->team = teamToUpdate;
        update_msg->currentSprite = team[teamToUpdate]->get_currentCharacter()->getSpriteNumber();


        character_updater_t *update[numberOfPlayers];
        for (int j = 0; j < numberOfPlayers; ++j) {
            update[j] = new character_updater_t;
            update[j]->action = update_msg->action;
            update[j]->team = update_msg->team;
            update[j]->posX = update_msg->posX;
            update[j]->posY = update_msg->posY;
            update[j]->currentSprite = update_msg->currentSprite;
        }

        for (int i = 0; i < numberOfPlayers; ++i) {
            std::unique_lock<std::mutex> lock(m);
            this->client_updater_queue[i]->insert(update[i]);
        }

        incoming_msges_queue->delete_data();
    }
}








