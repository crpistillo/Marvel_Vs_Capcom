//
// Created by donatojp on 16/06/19.
//

#include "Menu.h"
#include "../Constants.h"
#include "../CharactersServer/Characters/SpidermanServer.h"
#include "../CharactersServer/Characters/WolverineServer.h"
#include "../CharactersServer/Characters/IronmanServer.h"
#include "../CharactersServer/Characters/RyuServer.h"



using namespace std;

Menu::Menu(int numberOfPlayers, TCPServer *pServer) {
    this->numberOfPlayers = numberOfPlayers;
    this->server = pServer;

    this->incoming_menu_actions_queue = new Queue<client_menu_t *>;

    for (int i = 0; i < numberOfPlayers; ++i) {
        this->cursor_updater_queue[i] = new Queue<cursor_updater_t *>;
    }
}

void Menu::setRunningMenuPhase(bool condition) {
    runningMenuPhase_mtx.lock();
    runningMenuPhase = condition;
    runningMenuPhase_mtx.unlock();
}

void Menu::receiveMenuActionsFromClient(int clientSocket) {
    char buf[sizeof(menu_action_t)];

    int timeout = (3 * 1000);


    while (getRunningMenuPhase()) {
        Socket *socket = this->server->getClientSocket(clientSocket);
        connection_mtx[clientSocket].lock();
        struct pollfd fds[1];
        memset(fds, 0, sizeof(fds));
        fds[0].fd = socket->get_fd();
        fds[0].events = POLLIN;

        if (!this->server->iplist[clientSocket].isActive) {
            connection_mtx[clientSocket].unlock();
            continue;
        }
        connection_mtx[clientSocket].unlock();


        //Me fijo si el socket esta apto para recibir

        int rc = poll(fds, 1, timeout);

        if (rc < 0)
            cout << "Error en poll" << endl;

        else if (rc > 0 && fds[0].revents == POLLIN) {
            socket->reciveData(buf, sizeof(menu_action_t));   //Reveer mtx;
            menu_action_t *accion = (menu_action_t *) buf;

            if (*accion == ALIVE_MENU) {
                continue;
            } else {
                client_menu_t *msgMenuQueue = new client_menu_t;
                msgMenuQueue->client = clientSocket;
                msgMenuQueue->accion = *accion;
                incoming_msg_mtx.lock();
                this->incoming_menu_actions_queue->insert(msgMenuQueue);
                incoming_msg_mtx.unlock();
                //if (*accion == ENTER)
                //	return;
            }
        } else {

            //Reporto en el servidor que el cliente se desconecto
            client_menu_t *msgMenuQueue = new client_menu_t;
            msgMenuQueue->client = clientSocket;
            msgMenuQueue->accion = DISCONNECTED_MENU;
            incoming_msg_mtx.lock();
            this->incoming_menu_actions_queue->insert(msgMenuQueue);
            incoming_msg_mtx.unlock();

            //Cierro su socket, y reporto la desconexion

            socket->closeConnection();
            socket->closeFd();


            connection_mtx[clientSocket].lock();
            this->server->iplist[clientSocket].isActive = false;
            connection_mtx[clientSocket].unlock();

            numberOfConnections_mtx.lock();
            this->server->numberOfConnections--;
            numberOfConnections_mtx.unlock();
        }
    }

}

void Menu::sendCursorUpdaterToClient(int clientSocket) {
    Socket *socket = this->server->getClientSocket(clientSocket);

    while (1) {

        cursor_updater_t *updater;
        updaters_queue_mtx[clientSocket].lock();
        if (cursor_updater_queue[clientSocket]->empty_queue()) {
            updaters_queue_mtx[clientSocket].unlock();
            continue;
        }
        updater = cursor_updater_queue[clientSocket]->get_data();
        updaters_queue_mtx[clientSocket].unlock();

        socket->sendData(updater, sizeof(cursor_updater_t));

        updaters_queue_mtx[clientSocket].lock();
        cursor_updater_queue[clientSocket]->delete_data();
        updaters_queue_mtx[clientSocket].unlock();

        if (updater->menuTerminated)
            break;
    }

}

void Menu::sendUpdaters(bool finalUpdater) {
    cursor_updater_t *update[4];

    for (int i = 0; i < 4; i++) {
        update[i] = new cursor_updater_t;
        update[i]->cliente = i;
        update[i]->menuTerminated = finalUpdater;
        serverCursors[i]->makeMenuUpdater(update[i]);
    }

    menuClient.lock();
    for (int i = 0; i < numberOfPlayers; ++i) {
        for (int j = 0; j < MAXPLAYERS; j++) {
            std::unique_lock<std::mutex> lock(updaters_queue_mtx[i]);
            this->cursor_updater_queue[i]->insert(update[j]);
        }
    }

    menuClient.unlock();
}

void Menu::runMenuPhase() {
    //Crear hilos de escucha a los 4 clientes, que encolen en la cola de arriba

    setRunningMenuPhase(true);

    std::thread receiveFromClientThreads[this->numberOfPlayers];
    std::thread sendToClientThreads[this->numberOfPlayers];

    for (int i = 0; i < numberOfPlayers; ++i) {
        receiveFromClientThreads[i] = std::thread(
                &Menu::receiveMenuActionsFromClient, this, i);
        sendToClientThreads[i] = std::thread(
                &Menu::sendCursorUpdaterToClient, this, i);
    }

    this->runCorrespondingMenu();

    setRunningMenuPhase(false);

    for (int i = 0; i < numberOfPlayers; ++i) {
        receiveFromClientThreads[i].join();
        receiveFromClientThreads[i].~thread();
    }


    for (int i = 0; i < numberOfPlayers; ++i) {
        receiveFromClientThreads[i].~thread();
    }
    sendUpdaters(true);

    for (int i = 0; i < numberOfPlayers; ++i) {
        sendToClientThreads[i].join();
        sendToClientThreads[i].~thread();
        delete cursor_updater_queue[i];

    }
    delete incoming_menu_actions_queue;

}

bool Menu::getRunningMenuPhase() {
    runningMenuPhase_mtx.lock();
    bool var = runningMenuPhase;
    runningMenuPhase_mtx.unlock();
    return var;
}

bool Menu::processMenuAction(client_menu_t *action_msg) {
    return this->serverCursors[action_msg->client]->update(action_msg);
}

ServerCursor *Menu::getServerCursor(int i) {
    return this->serverCursors[i];
}

int Menu::getNumberOfCharactersSelected() {

    int n = 0;
    for (int i = 0; i < MAXPLAYERS; i++) {
        if (serverCursors[i]->getFinalSelection())
            n++;
    }

    return n;

}


int Menu::getTeamNumber(int nclient) {
    if (numberOfPlayers == 4) {
        if (nclient == 0 || nclient == 1)
            return 0;
        else return 1;
    } else if (numberOfPlayers == 2) {
        if (nclient == 0)
            return 0;
        else return 1;
    } else if(numberOfPlayers == 3){
        if(nclient == 2)
            return 1;
        else return 0;
    }
}

CharacterServer *Menu::createServerCharacterFromCursor(ServerCursor *pCursor, int nclient, int character, Constants* constants) {
    CharacterServer *characterServer;
    Box *caja;
    int pos;
    if (character < 2)
        pos = constants->INITIAL_POS_X_PLAYER_ONE;
    else
        pos = constants->INITIAL_POS_X_PLAYER_TWO;

    switch (pCursor->getCharacterSelected()) {
        case SPIDERMAN:
            characterServer = new SpidermanServer(pos, constants->widthSpiderman,
                                                  constants->heightSpiderman, constants->spidermanSobrante,
                                                  constants->spidermanAncho, constants->screenWidth, nclient);

            break;

        case WOLVERINE:
            characterServer = new WolverineServer(pos, constants->widthWolverine,
                                                  constants->heightWolverine, constants->wolverineSobrante,
                                                  constants->wolverineAncho, constants->screenWidth, nclient);
            break;

        case IRONMAN:
        	characterServer = new IronmanServer(pos, constants->widthSpiderman,
        	                                                  constants->heightSpiderman, 276,
        	                                                  102, constants->screenWidth, nclient);
        	break;

        case RYU:
        	characterServer = new RyuServer(pos, constants->widthSpiderman,
        	                                                  constants->heightSpiderman, 274,
        	                                                  66, constants->screenWidth, nclient);
    }
    return characterServer;
}

void Menu::buildTeams(Team **teams) {
    teams[0] = this->team[0];
    teams[1] = this->team[1];
}

void Menu::reportReconnection(client_menu_t *recon_info) {
    incoming_msg_mtx.lock();
    incoming_menu_actions_queue->insert(recon_info);
    incoming_msg_mtx.unlock();
}
