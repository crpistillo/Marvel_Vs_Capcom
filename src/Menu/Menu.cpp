//
// Created by donatojp on 16/06/19.
//

#include "Menu.h"


using namespace std;

Menu::Menu(int numberOfPlayers) {
    this->numberOfPlayers = numberOfPlayers;

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
        Socket *socket = getClientSocket(clientSocket);
        connection_mtx[clientSocket].lock();
        struct pollfd fds[1];
        memset(fds, 0, sizeof(fds));
        fds[0].fd = socket->get_fd();
        fds[0].events = POLLIN;

        if (!iplist[clientSocket].isActive) {
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
            iplist[clientSocket].isActive = false;
            connection_mtx[clientSocket].unlock();

            numberOfConnections_mtx.lock();
            (*numberOfConnections)--;
            numberOfConnections_mtx.unlock();
        }
    }

}

void Menu::sendCursorUpdaterToClient(int clientSocket) {

}

void Menu::sendUpdaters(bool finalUpdater) {

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

Socket *Menu::getClientSocket(int socket) {
    std::unique_lock<std::mutex> lock(connection_mtx[socket]);
    return clientsSockets[socket];
}

void Menu::setClientsSocket(Socket **pSocket) {
    for(int i = 0; i < numberOfPlayers; i++){
        this->clientsSockets[i] = pSocket[i];
    }
}

void Menu::setIpList(ip_status_t *iplist) {
    for(int i = 0; i < numberOfPlayers; i++){
        this->iplist[i] = iplist[i];
    }

}

void Menu::setNumberOfConnections(int *pNumberOfConnections) {
    this->numberOfConnections = pNumberOfConnections;
}
