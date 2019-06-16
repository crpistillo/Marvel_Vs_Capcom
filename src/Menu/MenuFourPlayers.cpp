//
// Created by donatojp on 16/06/19.
//

#include "MenuFourPlayers.h"

MenuFourPlayers::MenuFourPlayers() : Menu(4) {
}

void MenuFourPlayers::runMenuPhase() {
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

    this->runMenuFourPlayers();

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

void MenuFourPlayers::runMenuFourPlayers() {
}
