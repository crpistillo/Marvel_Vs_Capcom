//
// Created by donatojp on 16/06/19.
//

#include "MenuFourPlayers.h"

MenuFourPlayers::MenuFourPlayers(TCPServer *pServer) : Menu(4, pServer) {

    this->team[0] = new Team(2);
    this->team[1] = new Team(2);
}

void MenuFourPlayers::runMenuFourPlayers() {
    serverCursors[0] = new ServerCursor(97, 61, true);
    serverCursors[1] = new ServerCursor(449, 61, true);
    serverCursors[2] = new ServerCursor(97, 353, true);
    serverCursors[3] = new ServerCursor(449, 353, true);
    sendUpdaters(false);

    int onlinePlayersTeamOne = 2;
    int onlinePlayersTeamTwo = 2;


    //Procesar eventos que vengan de incoming_menu_actions_queue
    while (1) {
        client_menu_t *incoming_msg;

        incoming_msg_mtx.lock();
        if (this->incoming_menu_actions_queue->empty_queue()) {
            incoming_msg_mtx.unlock();
            continue;
        }
        incoming_msg = this->incoming_menu_actions_queue->get_data();
        incoming_msg_mtx.unlock();

        if (incoming_msg->accion == DISCONNECTED_MENU) {
            if ((int) (incoming_msg->client / 2) == 0)
                onlinePlayersTeamOne--;
            else
                onlinePlayersTeamTwo--;

            if (!serverCursors[incoming_msg->client]->getFinalSelection())
                serverCursors[incoming_msg->client]->setVisible(false);
            sendUpdaters(false);
        } else if (incoming_msg->accion == RECONNECTION_MENU) {
            if ((int) (incoming_msg->client / 2) == 0)
                onlinePlayersTeamOne++;
            else
                onlinePlayersTeamTwo++;

            serverCursors[incoming_msg->client]->setVisible(true);
            sendUpdaters(false);
        } else {

            /* Proceso el evento */
            bool validMenuAction = processMenuAction(incoming_msg);

            /* Solo envio información a los clientes si hubo algun cambio */
            if (validMenuAction)
                sendUpdaters(false);
        }

        incoming_msg_mtx.lock();
        incoming_menu_actions_queue->delete_data();
        incoming_msg_mtx.unlock();

        delete incoming_msg;

        /* Verifico si ya seleccionaron todos */
        int charactersSelected = getNumberOfCharactersSelected();
        if (charactersSelected == MAXPLAYERS)
            break;

    }


}

void MenuFourPlayers::runCorrespondingMenu() {
    this->runMenuFourPlayers();
}

void MenuFourPlayers::sendSelectedCharacters(Constants *constants) {
    int charactersPerClient;

    if (2 == numberOfPlayers)   // si el maximo de players es 2 elijen 2
        charactersPerClient = 2;
    else
        charactersPerClient = 1;

    CharacterServer *characters[MAXPLAYERS];
    character_builder_t builders[MAXPLAYERS];


    int nclient = 0;
    int nCharacter = 0;
    double pos;

    for (int i = 0; i < numberOfPlayers; i++) {    // de 0 a 4  o de 0 a 2
        if (getTeamNumber(nclient) == 0)
            pos = constants->INITIAL_POS_X_PLAYER_ONE;
        else
            pos = constants->INITIAL_POS_X_PLAYER_TWO;

        if(numberOfPlayers == 3 && i == 2){
            charactersPerClient = 2;
        }

        for (int j = 0; j < charactersPerClient; j++) { // si characters es 1 entra 1 vez
            characters[nCharacter] = createServerCharacterFromCursor(
                    serverCursors[nCharacter], nclient, nCharacter, constants);
            characters[nCharacter]->makeBuilderStruct(&builders[nCharacter],
                                                      nCharacter < 2);
            nCharacter++;
        }
        nclient++;
    }


    team[0]->setCharacters(characters[0], characters[1]);
    team[1]->setCharacters(characters[2], characters[3]);


    for (auto &builder : builders) {
        for (int i = 0; i < numberOfPlayers; ++i) {
            this->server->getClientSocket(i)->sendData(&builder, sizeof(character_builder_t));
        }
    }


    int currentTeam0 = team[0]->get_currentCharacterNumber();
    int currentTeam1 = team[1]->get_currentCharacterNumber();
    for (int k = 0; k < numberOfPlayers; ++k) {

        this->server->getClientSocket(k)->sendData(&currentTeam0, sizeof(int));
        this->server->getClientSocket(k)->sendData(&currentTeam1, sizeof(int));
    }
    return;

}



