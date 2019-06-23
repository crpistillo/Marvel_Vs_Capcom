//
// Created by donatojp on 16/06/19.
//

#include "MenuTwoPlayers.h"

MenuTwoPlayers::MenuTwoPlayers(TCPServer *pServer) : Menu(2, pServer) {
    this->team[0] = new Team(1);
    this->team[1] = new Team(1);
}


void MenuTwoPlayers::runMenuTwoPlayers() {
    //Los dos cursores del primer cliente
    serverCursors[0] = new ServerCursor(SPIDERMAN_POSITION, TEAM_ONE_POSITION, true);
    serverCursors[1] = new ServerCursor(RYU_POSITION, TEAM_ONE_POSITION, false);

    //Los dos cursores del segundo cliente
    serverCursors[2] = new ServerCursor(SPIDERMAN_POSITION, TEAM_TWO_POSITION, true);
    serverCursors[3] = new ServerCursor(RYU_POSITION, TEAM_TWO_POSITION, false);

    ServerCursor *actualCursorFirstClient = serverCursors[0];
    ServerCursor *actualCursorSecondClient = serverCursors[2];

    //Procesar eventos que vengan de incoming_menu_actions_queue
    while (1) {
        client_menu_t *incoming_msg;
        if (this->incoming_menu_actions_queue->empty_queue())
            continue;
        incoming_msg = this->incoming_menu_actions_queue->get_data();

        /* Proceso el evento */

        bool validMenuAction;

        if (incoming_msg->client == 0)
            validMenuAction = actualCursorFirstClient->update(incoming_msg);
        if (incoming_msg->client == 1)
            validMenuAction = actualCursorSecondClient->update(incoming_msg);

        if (actualCursorFirstClient->getFinalSelection()) {
            actualCursorFirstClient = serverCursors[1];
            actualCursorFirstClient->setVisible(true);
        }
        if (actualCursorSecondClient->getFinalSelection()) {
            actualCursorSecondClient = serverCursors[3];
            actualCursorSecondClient->setVisible(true);
        }

        /* Solo envio información a los clientes si hubo algun cambio */
        if (validMenuAction)
            sendUpdaters(false);


        incoming_menu_actions_queue->delete_data();
        delete incoming_msg;

        /* Verifico si ya seleccionaron todos */
        int charactersSelected = getNumberOfCharactersSelected();
        if (charactersSelected == MAXPLAYERS)
            break;

    }
}

void MenuTwoPlayers::runCorrespondingMenu() {
    this->runMenuTwoPlayers();
}

void MenuTwoPlayers::sendSelectedCharacters(Constants *constants) {

    int charactersPerClient = 2;

    CharacterServer *characters[MAXPLAYERS];
    character_builder_t builders[MAXPLAYERS];


    int nclient = 0;
    int nCharacter = 0;

    for (int i = 0; i < numberOfPlayers; i++) {    // de 0 a 4  o de 0 a 2

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
}
