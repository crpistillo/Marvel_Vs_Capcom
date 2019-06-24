//
// Created by IgVelasco on 5/4/19.
//

#include "TCPServer.h"
#include "netdb.h"
#include "CharactersServer/Characters/SpidermanServer.h"
#include "CharactersServer/Characters/WolverineServer.h"
#include "CharactersServer/Characters/IronmanServer.h"
#include "CharactersServer/Characters/RyuServer.h"
#include <string>
#include <pthread.h>
#include <thread>
#include "Constants.h"
#include <sys/poll.h>
#include "signal_handler.h"
#include "Menu/MenuTwoPlayers.h"
#include "Menu/MenuFourPlayers.h"
#include "Menu/MenuThreePlayers.h"
#include "EventHandler.h"
#include "Timer.h"
#include "tools/FPSManager/FPSManager.h"


Constants constants;
const int LEVEL_WIDTH = 3200;
const int LEVEL_HEIGHT = 600;



const string ERROR = "ERROR";
const string INFO = "INFO";
const string DEBUG = "DEBUG";


TCPServer::TCPServer() {
    signal(SIGPIPE, signalHandler);
    this->logger = Logger::getInstance();
    this->numberOfConnections = 0;
    this->port = 0;
    this->serverSocket = new Socket();
    this->newSockFd = new Socket();
    for (int i = 0; i < 4; i++) //hardcodeo
    {
        this->activeClients[i] = true;
    }
    server_state = BEGINNING;
    endgame = false;
}

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


    switch (numberOfPlayers){
    case TWO_PLAYERS:
        this->menu = new MenuTwoPlayers(this);
        break;
    case THREE_PLAYERS:
        this->menu = new MenuThreePlayers(this);
        break;
    case FOUR_PLAYERS:
        this->menu = new MenuFourPlayers(this);
        break;
    }

    this->incoming_msges_queue = new Queue<incoming_msg_t *>;


    for (int i = 0; i < numberOfPlayers; ++i) {
        this->client_updater_queue[i] = new Queue<character_updater_t *>;
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


        clientsSockets[numberOfConnections]->fd = newSockFd->fd;


        iplist[numberOfConnections].ip = inet_ntoa(clientAddress.sin_addr);
        iplist[numberOfConnections].isActive = true;


        initializer_t *initializer = new initializer_t;
        initializer->instance = server_state;
        initializer->client = numberOfConnections;
        initializer->players = numberOfPlayers;

        clientsSockets[numberOfConnections]->sendData(initializer, sizeof(initializer_t));

        numberOfConnections++;

        //Aceptar conexiones pero seguir esperando por mas
        if (numberOfConnections != numberOfPlayers) {

            to_send.status = NOT_READY;
            to_send.nconnections = numberOfConnections;

            for (int i = 0; i < numberOfConnections; i++) {
                clientsSockets[i]->sendData(&to_send,
                                            sizeof(connection_information_t));

            }
        } else {
            to_send.status = READY;
            to_send.nconnections = numberOfConnections;
            for (int i = 0; i < numberOfConnections; i++) {
                clientsSockets[i]->sendData(&to_send,
                                            sizeof(connection_information_t));
            }
            break;
        }

    }
    reconnections();
}

void TCPServer::reconnections() {

    game_instance_t server_state_local;
    while (1) {


        //TODO Make as fun
        struct sockaddr_in clientAddress;

        newSockFd->acceptConnection(this->serverSocket, &clientAddress, logger);
        logger->log("Nueva conexion aceptada", INFO);

        initializer_t initializer;

        numberOfConnections_mtx.lock();
        if (numberOfConnections == numberOfPlayers) {

            numberOfConnections_mtx.unlock();
            initializer.client = -1;
            initializer.players = -1;
            initializer.instance = NO_MORE_PLAYERS_ALLOWED;

            send(newSockFd->get_fd(), &initializer,
                 sizeof(connection_information_t), 0);
            close(newSockFd->get_fd());

            continue;

        }
        numberOfConnections_mtx.unlock();


        int socketToReconnect = -1;
        string str = inet_ntoa(clientAddress.sin_addr);


        for (int i = 0; i < numberOfPlayers; ++i) {

            connection_mtx[i].lock();
            if (iplist[i].ip == str && !iplist[i].isActive) {
                socketToReconnect = i; //debug case
            }
            connection_mtx[i].unlock();
        }

        if (socketToReconnect == -1)
            continue;


        connection_mtx[socketToReconnect].lock();
        clientsSockets[socketToReconnect]->fd = newSockFd->fd;
        connection_mtx[socketToReconnect].unlock();

        //check server status



        initializer.client = socketToReconnect;
        initializer.players = numberOfPlayers;

        server_state_mtx.lock();
        server_state_local = server_state;
        server_state_mtx.unlock();

        initializer.instance = server_state_local;


        if (server_state_local == FIGHT_PHASE) {

            clientsSockets[socketToReconnect]->sendData(&initializer, sizeof(initializer_t));
            sendCharacterBuildersToSocket(socketToReconnect);

            connection_mtx[socketToReconnect].lock();
            iplist[socketToReconnect].isActive = true;
            connection_mtx[socketToReconnect].unlock();

            incoming_msg_t *recon = new incoming_msg_t;

            recon->action = RECONNECT;
            recon->client = socketToReconnect;

            incoming_msg_mtx.lock();
            incoming_msges_queue->insert(recon);
            incoming_msg_mtx.unlock();

            teams_mtx.lock();
            int currentTeam0 = team[0]->get_currentCharacterNumber();
            int currentTeam1 = team[1]->get_currentCharacterNumber();
            teams_mtx.unlock();

            clientsSockets[socketToReconnect]->sendData(&currentTeam0, sizeof(int));
            clientsSockets[socketToReconnect]->sendData(&currentTeam1, sizeof(int));

            numberOfConnections_mtx.lock();
            numberOfConnections++;
            numberOfConnections_mtx.unlock();
        } else if (server_state_local == MENU_PHASE) {

            Socket *socketToSend;
            connection_mtx[socketToReconnect].lock();
            socketToSend = clientsSockets[socketToReconnect];
            connection_mtx[socketToReconnect].unlock();

            socketToSend->sendData(&initializer, sizeof(initializer_t));

            connection_mtx[socketToReconnect].lock();
            iplist[socketToReconnect].isActive = true;
            connection_mtx[socketToReconnect].unlock();

            client_menu_t *recon = new client_menu_t;
            recon->client = socketToReconnect;
            recon->accion = RECONNECTION_MENU;

            //TODO clase menu se encarga de las desconexiones
            this->menu->reportReconnection(recon);

            std::unique_lock<std::mutex> lock(numberOfConnections_mtx);
            numberOfConnections++;
        }

    }
}


void TCPServer::sendCharacterBuildersToSocket(int socketNumber) {
    character_builder_t builders[MAXPLAYERS];

    team[0]->getFirstCharacter()->makeBuilderStruct(&builders[0], true);
    team[0]->getSecondCharacter()->makeBuilderStruct(&builders[1], true);
    team[1]->getFirstCharacter()->makeBuilderStruct(&builders[2], false);
    team[1]->getSecondCharacter()->makeBuilderStruct(&builders[3], false);

    for (auto &builder : builders) {
        clientsSockets[socketNumber]->sendData(&builder, sizeof(character_builder_t));
    }
}

void TCPServer::clean() {
    memset(msg, 0, MAXPACKETSIZE);
}

void TCPServer::detach() {
    close(this->serverSocket->get_fd());
    close(newSockFd->get_fd());
}

void TCPServer::reportClientConnected(const struct sockaddr_in *clientAddress,
                                      socklen_t clientAddress_len, Logger *logger) {

    char hostbuf[NI_MAXHOST];
    using namespace std;
    char portbuf[NI_MAXSERV];
    if (getnameinfo((struct sockaddr *) clientAddress, clientAddress_len,
                    hostbuf, NI_MAXHOST, portbuf, NI_MAXSERV, 0) == 0) {
        logger->log(msg, INFO);
    }
}

int TCPServer::getNumberOfConections() {
    std::unique_lock<std::mutex> lock(numberOfConnections_mtx);
    return numberOfConnections;
}


/*Funcion que lee del socket la informacion que los clientes le envian.
 * Esta deberia leer, codificar y encolar eventos en la cola del servidor
 *
 * Son los denominados "thread lectura cliente x"*/
void TCPServer::receiveFromClient(int clientSocket) {
    //Recibo los argumentos y los casteo en el orden que corresponde.

    char bufAction[sizeof(actions_t)];
    bool clientConnected = true;

    int timeout = (1 * 100);
    int maxTimeouts = 0;

    while (!getEndgame()) {

        Socket *socket = getClientSocket(clientSocket);

        struct pollfd fds[1];
        memset(fds, 0, sizeof(fds));
        fds[0].fd = socket->get_fd();
        fds[0].events = POLLIN;

        //Me fijo si el socket esta apto para recibir
        int rc = poll(fds, 1, timeout);

        if (rc > 0 && fds[0].revents == POLLIN) {
            maxTimeouts = 0;
            connection_mtx[clientSocket].lock();
            iplist[clientSocket].isActive = true;
            connection_mtx[clientSocket].unlock();

            actions_t *accion = new actions_t;

            if (socket->reciveData(bufAction, sizeof(actions_t)))
                accion = (actions_t *) bufAction;
            else
                *accion = DISCONNECTEDCLIENT;

            //Agrego elementos a la cola de mensajes entrantes
            incoming_msg_t *msgQueue = new incoming_msg_t;
            msgQueue->action = *accion;
            msgQueue->client = clientSocket;
            if (msgQueue->action == DISCONNECTEDCLIENT && clientConnected) {
                this->manageDisconnection(clientSocket);
                disconnectSocket(clientSocket, socket);
                clientConnected = false;
            } else if (this->clientIsActive(clientSocket)) {
                incoming_msg_mtx.lock();
                this->incoming_msges_queue->insert(msgQueue);
                incoming_msg_mtx.unlock();
                clientConnected = true;
            }
        } else if ((rc == 0 || fds[0].revents != POLLIN)) {
            maxTimeouts++;
            if (maxTimeouts < 150) {
                connection_mtx[clientSocket].lock();
                iplist[clientSocket].isActive = false;
                connection_mtx[clientSocket].unlock();
            }
            if (maxTimeouts == 150 && clientConnected) {
                this->manageDisconnection(clientSocket);
                disconnectSocket(clientSocket, socket);
                clientConnected = false;
                continue;
            }
        }
    }
}

void TCPServer::disconnectSocket(int clientSocket, Socket *socket) {
    socket->closeConnection();
    socket->closeFd();
    activeClients[clientSocket] = false;

    connection_mtx[clientSocket].lock();
    iplist[clientSocket].isActive = false;
    connection_mtx[clientSocket].unlock();

    numberOfConnections_mtx.lock();
    numberOfConnections--;
    numberOfConnections_mtx.unlock();

}

/*Esta funcion se encarga de desencolar datos de las colas de los clientes
 * y mandarle dicha informacion a cada cliente por el socket
 *
 * Son los denominados "thread escritura cliente x"
 * */

Socket *TCPServer::getClientSocket(int i) {
    std::unique_lock<std::mutex> lock(connection_mtx[i]);
    return clientsSockets[i];
}

void TCPServer::sendToClient(int clientSocket) {

    //Recibo los argumentos y los casteo en el orden que corresponde.
    Socket *socket = getClientSocket(clientSocket);

    while (1) {

        connection_mtx[clientSocket].lock();
        if (!iplist[clientSocket].isActive) {

            if (!client_updater_queue[clientSocket]->empty_queue()) {

                if (client_updater_queue[clientSocket]->get_data()->gameFinishedByDisconnections) {
                    connection_mtx[clientSocket].unlock();
                    break;
                }
                client_updater_queue[clientSocket]->delete_data();
            }
            connection_mtx[clientSocket].unlock();
            continue;
        }
        connection_mtx[clientSocket].unlock();


        character_updater_t *updater;

        updaters_queue_mtx[clientSocket].lock();
        if (client_updater_queue[clientSocket]->empty_queue()) {
            updaters_queue_mtx[clientSocket].unlock();
            continue;
        }
        updater = client_updater_queue[clientSocket]->get_data();
        updaters_queue_mtx[clientSocket].unlock();

        socket->sendData(updater, sizeof(character_updater_t));

        std::unique_lock<std::mutex> lock(updaters_queue_mtx[clientSocket]);
        client_updater_queue[clientSocket]->delete_data();

        if (updater->gameFinishedByDisconnections)
            break;
    }
}

void TCPServer::runServer() {

    std::thread receive(&TCPServer::receive, this);
    receive.detach();
    while (getNumberOfConections() != numberOfPlayers) //Espera hasta que se conecten MAXPLAYERS
        continue;


    cout << "Numero de jugadores alcanzado! \n";

    server_state_mtx.lock();
    this->server_state = MENU_PHASE;
    server_state_mtx.unlock();

    //TODO MENU PHASE CON CLASE MENU
    this->menu->runMenuPhase();
    this->menu->buildTeams(team);
    this->menu->sendSelectedCharacters(&constants);


    //treatDisconnectionsAfterSelection();

    server_state_mtx.lock();
    this->server_state = FIGHT_PHASE;
    server_state_mtx.unlock();

    for (int i = 0; i < numberOfPlayers; ++i) {
        receiveFromClientThreads[i] = std::thread(&TCPServer::receiveFromClient,
                                                  this, i);
        sendToClientThreads[i] = std::thread(&TCPServer::sendToClient, this, i);
    }


    updateModel();

    for (int i = 0; i < numberOfPlayers; i++) {
        sendToClientThreads[i].join();
    }

    for (int i = 0; i < numberOfPlayers; i++) {
        receiveFromClientThreads[i].join();
    }

    this->serverSocket->closeFd();
    this->serverSocket->closeConnection();

}

//nclient = numero del cliente
//characterNumber  = numero del personaje es decir el team 1 tiene 1 y 2 y el team 2 tiene 3 y 4 no importa el numero
//de clientes
CharacterServer *TCPServer::createServerCharacterFromCursor(
        ServerCursor *cursor, int nclient, int characterNumber) {

    CharacterServer *characterServer;
    Box *caja;
    int pos;
    if (characterNumber < 2)
        pos = constants.INITIAL_POS_X_PLAYER_ONE;
    else
        pos = constants.INITIAL_POS_X_PLAYER_TWO;

    switch (cursor->getCharacterSelected()) {
        case SPIDERMAN:
            //caja = new Box(0,0,100,100);
            characterServer = new SpidermanServer(pos, constants.widthSpiderman,
                                                  constants.heightSpiderman, constants.spidermanSobrante,
                                                  constants.spidermanAncho, constants.screenWidth, nclient);

            break;

        case WOLVERINE:
            //	caja = new Box(600,0,100,100);
            characterServer = new WolverineServer(pos, constants.widthWolverine,
                                                  constants.heightWolverine, constants.wolverineSobrante,
                                                  constants.wolverineAncho, constants.screenWidth, nclient);
    }
    // characterServer->moverColisionable(); ??
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
        msj =
                "No se reconoce al personaje '" + name + "'."
                + " Se intentara cargar las imagenes correspondiente al filepath: "
                + filepath
                + " como las imagenes del personaje 'spiderman'.";
        logger->log(msj, ERROR);
    }

    if (wolverineConfig["name"] != "wolverine") {
        string name = wolverineConfig["name"];
        string filepath = wolverineConfig["filepath"];
        msj = "No se reconoce al personaje '" + name + "'."
              + " Se cargaran las imagenes correspondiente al filepath: "
              + filepath + " como las imagenes del personaje 'wolverine'.";
        logger->log(msj, ERROR);
    }

    constants.widthSpiderman = spidermanConfig["width"];
    constants.heightSpiderman = spidermanConfig["height"];
    constants.widthWolverine = wolverineConfig["width"];
    constants.heightWolverine = wolverineConfig["height"];

    string spidermanPath = spidermanConfig["filepath"];
    if (spidermanPath != "images/spiderman/spiderman_")
        logger->log(
                "Filepath para personaje Spiderman incorrecto. Error al cargar imagenes.",
                ERROR);
    string wolverinePath = wolverineConfig["filepath"];
    if (wolverinePath != "images/wolverine/wolverine_")
        logger->log(
                "Filepath para personaje Wolverine incorrecto. Error al cargar imagenes.",
                ERROR);

    constants.spidermanSobrante = constants.widthSpiderman * 242 / 640;
    constants.spidermanAncho = constants.widthSpiderman * 110 / 640;
    constants.wolverineSobrante = constants.widthWolverine * 278 / 640;
    constants.wolverineAncho = constants.widthWolverine * 87 / 640;

    constants.INITIAL_POS_X_PLAYER_ONE = ((LEVEL_WIDTH / 2)
                                          - constants.spidermanSobrante) - (constants.spidermanAncho / 2)
                                         - 200;
    constants.INITIAL_POS_X_PLAYER_TWO = ((LEVEL_WIDTH / 2)
                                          - constants.wolverineSobrante) - (constants.wolverineAncho / 2)
                                         + 200;

    constants.screenWidth = config["window"]["width"];
    constants.screenHeight = config["window"]["height"];

    logger->log("Creacion de personajes.", DEBUG);

}

void TCPServer::updateModel() {
    EventHandler *eventHandler = new EventHandler(team, &teams_mtx);
    Timer *timer = new Timer(5);
    int roundsPlayed = 0;
    while (1) {

        if (timer->getTimeLeft() == 0 || roundsPlayed == 0 || anyTeamLost()) {

            roundsPlayed++;
            if(roundsPlayed != 4)
                roundRun(getCurrentWinner(), eventHandler, roundsPlayed);
            resetCharactersLife();
            timer->resetTimer();
            ignoreMessages = false;
        }

        if(team[0]->getRoundsWon() == 2){
            endgameByWinningTeam(&team[0], 0);
            break;
        }
        else if(team[1]->getRoundsWon() == 2){
            endgameByWinningTeam(&team[1], 1);
            break;
        }


        if (numberOfConnections == 0) {
            cout << "Se han desconectado todos los clientes. Server se desconecta" << endl;
            break;
        }

        incoming_msg_t *incoming_msg;
        if (incoming_msges_queue->empty_queue())
            continue;
        incoming_msg = this->incoming_msges_queue->get_data();
        if(!incoming_msg)
            continue;

        int teamToUpdate;
        int enemyTeam;
        getTeams(&teamToUpdate, &enemyTeam, incoming_msg->client);

        character_updater_t *update_msg = eventHandler->handleEvent(incoming_msg, teamToUpdate, enemyTeam);
        update_msg->round.winner = getCurrentWinner();
        update_msg->round.numberOfRound = roundsPlayed;
        timer->setDigits(update_msg);

        if (collition(teamToUpdate, enemyTeam, update_msg->action)) {
            std::unique_lock<std::mutex> lock_queue(incoming_msg_mtx);
            std::unique_lock<std::mutex> lock_team(teams_mtx);
            eventHandler->manageInteractiveActions(incoming_msges_queue, teamToUpdate, enemyTeam, update_msg->action);
        }

        if (team[teamToUpdate]->sizeOfTeam == 0 || team[enemyTeam]->sizeOfTeam == 0 || roundsPlayed == 4) {
            endgameForDisconnections();
            break;
        }
        update_msg->round.roundInfo = FIGHTING;
        update_msg->round.winner = 0;
        update_msg->round.numberOfRound = roundsPlayed;
        eventHandler->handleProjectiles(update_msg, teamToUpdate);
        putUpdatersInEachQueue(update_msg, incoming_msg->client);

        std::unique_lock<std::mutex> lock(incoming_msg_mtx);
        incoming_msges_queue->delete_data();

    }

    setEndgame(true);
}


bool TCPServer::clientIsActive(int clientSocket) {
    std::unique_lock<std::mutex> lock(teams_mtx);
    return (team[0]->clientActive == clientSocket
            || team[1]->clientActive == clientSocket) && !ignoreMessages;
}

void TCPServer::manageDisconnection(int clientSocket) {
    if (this->clientIsActive(clientSocket)) {
        incoming_msg_t *msgQueue = new incoming_msg_t;
        msgQueue->action = DISCONNECTEDCLIENT;
        msgQueue->client = clientSocket;
        this->incoming_msges_queue->insert(msgQueue);
    } else {
        if (this->getTeamNumber(clientSocket) == 0) {
            teams_mtx.lock();
            team[0]->sizeOfTeam--;
            teams_mtx.unlock();

        } else {
            teams_mtx.lock();
            team[1]->sizeOfTeam--;
            teams_mtx.unlock();
        }
    }
}


void TCPServer::getTeams(int *teamToUpdate, int *enemyTeam, int client) {
    if (this->getTeamNumber(client) == 0) {
        *teamToUpdate = 0;
        *enemyTeam = 1;
    } else {
        *teamToUpdate = 1;
        *enemyTeam = 0;
    }
    return;
}

int TCPServer::getTeamNumber(int client) {

    if (numberOfPlayers == 4) {
        if (client == 0 || client == 1)
            return 0;
        else return 1;
    } else if (numberOfPlayers == 2) {
        if (client == 0)
            return 0;
        else return 1;
    } else if (numberOfPlayers == 3){
        if (client == 2)
            return 1;
        else return 0;
    }
}


//Super hardcode no tuve muchas ganas de pensar algo lindo
//Se me ocurrio usar cuando se me deconectaban en el receive pero tenia que manejarlo en las reconnections
//y me dio paja
void TCPServer::treatDisconnectionsAfterSelection() {
    if (numberOfConnections < 4 && numberOfPlayers == 4) {
        if (!iplist[0].isActive || !iplist[1].isActive) {
            if (!iplist[0].isActive) {
                manageDisconnection(0);
            } else {
                manageDisconnection(1);
            }
        }
        if (!iplist[2].isActive || !iplist[3].isActive) {
            if (!iplist[2].isActive) {
                manageDisconnection(2);
            } else {
                manageDisconnection(3);
            }
        }
    }

}

void TCPServer::endgameForDisconnections() {

    character_updater_t *update[numberOfPlayers];
    for (int j = 0; j < numberOfPlayers; ++j) {
        update[j] = new character_updater_t;
        memset(update[j], 0, sizeof(character_updater_t));
        update[j]->gameFinishedByDisconnections = true;
    }

    for (int i = 0; i < numberOfPlayers; ++i) {
        updaters_queue_mtx[i].lock();
        this->client_updater_queue[i]->insert(update[i]);
        updaters_queue_mtx[i].unlock();
    }
}

void TCPServer::endgameByWinningTeam(Team **winningTeam, int teamno) {
    character_updater_t *update[numberOfPlayers];
    for (int j = 0; j < numberOfPlayers; ++j) {
        update[j] = new character_updater_t;
        memset(update[j], 0, sizeof(character_updater_t));
        update[j]->gameFinishedByWinningTeam = true;
        update[j]->winningTeam = teamno;
    }

    for (int i = 0; i < numberOfPlayers; ++i) {
        updaters_queue_mtx[i].lock();
        this->client_updater_queue[i]->insert(update[i]);
        updaters_queue_mtx[i].unlock();
    }
}


bool TCPServer::getEndgame() {
    bool var;
    endgame_mtx.lock();
    var = endgame;
    endgame_mtx.unlock();
    return var;
}

void TCPServer::setEndgame(bool condition) {
    endgame_mtx.lock();
    endgame = condition;
    endgame_mtx.unlock();
}


void TCPServer::putUpdatersInEachQueue(character_updater_t *update_msg, int clientNumber) {
    character_updater_t *update[numberOfPlayers];
    for (int j = 0; j < numberOfPlayers; ++j) {

        update[j] = new character_updater_t;
        memcpy( update[j], update_msg, sizeof(character_updater_t));
        update[j]->client = clientNumber;
        update[j]->gameFinishedByDisconnections = false;
    }

    for (int i = 0; i < numberOfPlayers; ++i) {
        connection_mtx[i].lock();
        if (iplist[i].isActive) {
            updaters_queue_mtx[i].lock();
            this->client_updater_queue[i]->insert(update[i]);
            updaters_queue_mtx[i].unlock();
        }
        connection_mtx[i].unlock();
    }

}


// Accion interactiva ( KICK || PUNCH || PROJECTIL ACTIVO)
// Esta colisionando...
// Y no esta colisionando ya...
bool TCPServer::collition(int teamToUpdate, int enemyTeam, actions_t action) {
    return isActionInteractive(action, teamToUpdate) && isColliding(teamToUpdate, enemyTeam) &&
           !isAlreadyInteracting(enemyTeam);
}

bool TCPServer::isColliding(int giver, int receiver) {
    return team[giver]->collidesWith(team[receiver]);
}

bool TCPServer::isAlreadyInteracting(int teamToCheck) {
    return team[teamToCheck]->getCurrentCharacter()->currentAction == HURTINGGROUND &&
           team[teamToCheck]->getCurrentCharacter()->currentAction == HURTINGAIR &&
           team[teamToCheck]->getCurrentCharacter()->currentAction == MAKINGINTRO; //not sure
}

bool TCPServer::isActionInteractive(actions_t actions, int teamToUpdate) {
    return isActionPunch(actions) || isActionKick(actions) || isProjectileActive(teamToUpdate) || actions == GRIP;
}

bool TCPServer::isProjectileActive(int teamToCheck) {
    return team[teamToCheck]->getCurrentCharacter()->isProjectileHurting();
}

bool TCPServer::isActionPunch(actions_t actions) {
    return actions == PUNCH || actions == PUNCHDOWN || actions == PUNCHINGJUMPLEFT || actions == PUNCHINGJUMPRIGHT ||
           actions == PUNCHINGVERTICAL || actions == PUNCHSTRONG || actions == PUNCHSTRONGDOWN ||
           actions == PUNCHINGSTRONGJUMPLEFT || actions == PUNCHINGSTRONGJUMPRIGHT || actions == PUNCHINGSTRONGVERTICAL;
}

bool TCPServer::isActionKick(actions_t action) {
    return action == KICK || action == KICKDOWN || action == KICKINGJUMPLEFT || action == KICKINGJUMPRIGHT ||
           action == KICKINGVERTICAL || action == KICKSTRONG || action == KICKSTRONGDOWN ||
           action == KICKINGSTRONGJUMPLEFT || action == KICKINGSTRONGJUMPRIGHT ||
           action == KICKINGSTRONGVERTICAL;
}

void TCPServer::resetRound() {

}

void TCPServer::roundRun(int whoWon, EventHandler *handler, int roundNum) {
    ignoreMessages = true;
    incoming_msg_mtx.lock();
    while(!incoming_msges_queue->empty_queue())
        incoming_msges_queue->delete_data();
    incoming_msg_mtx.unlock();

    if(roundNum != 1){
		this->team[whoWon]->incrementRoundsWon();
		if(this->team[whoWon]->getRoundsWon() == 2)
			return;
    }

    FPSManager* fpsManager = new FPSManager(30);
    Timer *timer = new Timer(3);
    while (timer->getTimeThatPass() < 3) {
        fpsManager->start();
        for (int i = 0; i < 2; ++i) {
            character_updater_t *roundUpdater = handler->getRoundUpdaters(i, timer);
            roundUpdater->gameFinishedByWinningTeam = false;
            roundUpdater->round.winner = whoWon;
            roundUpdater->round.numberOfRound = roundNum;
            roundUpdater->firstDigitOfTime = 9;
            roundUpdater->secondDigitOfTime = 9;
            putUpdatersInEachQueue(roundUpdater, team[i]->getCurrentCharacter()->clientNumber);
        }
        fpsManager->stop();
    }
    delete timer;
}

int TCPServer::getCurrentWinner() {
    bool didTeamOneWon = (team[0]->getSumOfLife() > team[1]->getSumOfLife());
    return didTeamOneWon ? 0 : 1;
}

bool TCPServer::anyTeamLost() {
    return team[0]->areBothCharactersDead() || team[1]->areBothCharactersDead();
}

void TCPServer::resetCharactersLife() {
    team[0]->resetCharacterLife();
    team[1]->resetCharacterLife();

}
