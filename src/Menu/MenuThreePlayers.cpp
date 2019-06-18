//
// Created by donatojp on 18/06/19.
//

#include "MenuThreePlayers.h"

MenuThreePlayers::MenuThreePlayers(TCPServer *pServer) : Menu(3, pServer) {
    //Team 1 por defecto es el de la persona solitaria
    this->team[0] = new Team(2);
    this->team[1] = new Team(1);
}

void MenuThreePlayers::runCorrespondingMenu() {
    runMenuThreePlayers();
}

void MenuThreePlayers::runMenuThreePlayers() {
    //Los dos cursores del primer cliente
    serverCursors[0] = new ServerCursor(97, 61, true);
    serverCursors[1] = new ServerCursor(449, 61, true);

    //Los cursores de los clientes que son compañeros
    serverCursors[2] = new ServerCursor(97, 353, true);
    serverCursors[3] = new ServerCursor(449, 353, false);

    ServerCursor *actualCursorFirstClient = serverCursors[0];
    ServerCursor *actualCursorSecondClient = serverCursors[1];
    ServerCursor *actualCursorThirdClient = serverCursors[2];

    ServerCursor *cursorToActualize;
    bool validMenuAction;
    client_menu_t *incoming_msg;

    int onlinePlayersTeamOne = 2;
    int onlinePlayersTeamTwo = 1;

    while (1) {

        if (this->incoming_menu_actions_queue->empty_queue())
            continue;
        incoming_msg = this->incoming_menu_actions_queue->get_data();

        /*Identifico el cursor a modificar*/
        switch (incoming_msg->client){
            case 0:
                cursorToActualize = actualCursorFirstClient;
                break;
            case 1:
                cursorToActualize = actualCursorSecondClient;
                break;
            case 2:
                cursorToActualize = actualCursorThirdClient;
                break;
        }


        /* Proceso el evento */
        validMenuAction = cursorToActualize->update(incoming_msg);

        if (actualCursorThirdClient->getFinalSelection()) {
            actualCursorThirdClient = serverCursors[3];
            actualCursorThirdClient->setVisible(true);
        }


        /* Proceso el desconexiones */

        if (incoming_msg->accion == DISCONNECTED_MENU) {
            if ((int) (incoming_msg->client / 1.5) == 0)
                onlinePlayersTeamOne--;
            else
                onlinePlayersTeamTwo--;

            cursorToActualize->setVisible(false);
            sendUpdaters(false);

        } else if (incoming_msg->accion == RECONNECTION_MENU) {
            if ((int) (incoming_msg->client / 1.5) == 0)
                onlinePlayersTeamOne++;
            else
                onlinePlayersTeamTwo++;

            cursorToActualize->setVisible(true);
            sendUpdaters(false);
        }

        /* Solo envio información a los clientes si hubo algun cambio */
        if (validMenuAction)
            sendUpdaters(false);

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
