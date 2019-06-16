//
// Created by donatojp on 16/06/19.
//

#include "Menu.h"
#include "MenuTwoPlayers.h"
#include "MenuFourPlayers.h"

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

}

void Menu::sendCursorUpdaterToClient(int clientSocket) {

}

void Menu::sendUpdaters(bool finalUpdater) {

}
