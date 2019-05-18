//
// Created by IgVelasco on 5/4/19.
//

#include "TCPClient.h"
#include "../tools/logger/Logger.h"

TCPClient::TCPClient() {
    socketClient = new Socket();
    port = 0;
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

