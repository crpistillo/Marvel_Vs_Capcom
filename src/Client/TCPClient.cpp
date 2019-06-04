//
// Created by IgVelasco on 5/4/19.
//

#include "TCPClient.h"
#include "../tools/logger/Logger.h"
#include "../data_structs.h"
#include "../MCGame.h"

MCGame* mcGame;


TCPClient::TCPClient() {
    socketClient = new Socket();
    port = 0;
    this->isPipeBroken = false;
    address = "";
}

bool TCPClient::setup(string addressToConnect, int listenPort) {
    Logger* logger = Logger::getInstance();
    this->address = addressToConnect;
    socketClient->create(logger);
    if(!socketClient->connectTo(address, listenPort)){
        cout <<"Connection error" << endl;
        return false;
    }
    cout << "Connection succesfull" << endl;
    return true;
}

bool TCPClient::Send(void* data, size_t size_data) {
    return socketClient->sendData(data, size_data);
}

void* TCPClient::receive(int size) {
    void *buffer =  malloc(size);
    memset(buffer, 0, size);
    this->socketClient->reciveData(buffer,size);
    return buffer;
}

string TCPClient::read() {
    char buffer[1] = {};
    string reply;
    while (buffer[0] != '\n') {
        if (recv(sock, buffer, sizeof(buffer), 0) < 0) {
            cout << "receive failed!" << endl;
            return nullptr;
        }
        reply += buffer[0];
    }
    return reply;
}

void TCPClient::exit() {
    //socketClient->close();
    //delete socketClient;
}

void TCPClient::run() {

    initializer_t initializer;
    socketClient->reciveData(&initializer,sizeof(initializer_t));

    nclient = initializer.client;
    numberOfPlayers = initializer.players;


    mcGame = new MCGame(config, ancho, alto, this);
    mcGame->camera = { 0, 0, ancho, alto };
    mcGame->init("Marvel vs Capcom", 100, 100, ancho, alto, 0);


    if(initializer.instance == BEGINNING)
        runFromBeginning();
    else if(initializer.instance == MENU_PHASE)
        runFromMenu();
    else if(initializer.instance == FIGHT_PHASE)
        runFromFight();

}

void TCPClient::runFromBeginning() {
    int connection = -1;
    while(1){

        connection_information_t buf;
        socketClient->reciveData(&buf, sizeof(connection_information_t));

        if(buf.status == NO_MORE_CONNECTIONS_ALLOWED){
            cout << "Connection not allowed. No more players. \n";
            return;
        }

        if(buf.status == READY)
            break;
        else
            cout << "Not ready to launch. Players: " << buf.nconnections << "/" << numberOfPlayers << endl;
    }

    mcGame->runMenu();

    cout << "RUNMENU TERMINADO" << endl;
    runAfterMenu();

}


//PARA CAGONES COMO JUAMPI QUE CORREN
void TCPClient::runFromFight() {
    runAfterMenu();
}

void TCPClient::runFromMenu() {
    //recibo las cosas
	cout << "Recibo que debo empezar desde el menu" << endl;
	/*cursor_updater_t* updater;
	for(int i = 0; i < 4; i++){
		mcGame->updateMenu();
	}*/

    mcGame->runMenu();
    runAfterMenu();
}

void TCPClient::runAfterMenu() const {
    mcGame->loadSelectedCharacters();
    mcGame->loadInitialTextures();
    mcGame->render();
    mcGame->run();
}

TCPClient *TCPClient::getInstance() {
    if(instance == 0){
        instance = new TCPClient();
    }
    return instance;
}

void TCPClient::signalHandlerClient(int signum) {
    if(signum == SIGPIPE){
        isPipeBroken = true;
    }

}


