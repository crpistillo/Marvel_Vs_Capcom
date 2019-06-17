//
// Created by IgVelasco on 6/16/19.
//

#include "EventHandler.h"

EventHandler::EventHandler(Team **team, std::mutex *mutex) {
    this->team = team;
    this->mutex = mutex;
}

int *EventHandler::getTeamDistances() {
    int *distancia = new int[2];
    int posPlayers[2];

    mutex->lock();
    posPlayers[0] = team[0]->getCurrentCharacter()->getPosX();
    posPlayers[1] = team[1]->getCurrentCharacter()->getPosX();
    distancia[0] = computeDistance(team[0]->getCurrentCharacter(), team[1]->getCurrentCharacter());
    distancia[1] = computeDistance(team[1]->getCurrentCharacter(), team[0]->getCurrentCharacter());
    mutex->unlock();

    return distancia;

}

int EventHandler::computeDistance(CharacterServer *character1, CharacterServer *character2) {
    int distancia;
    if (character1->getCentro() > character2->getCentro()) {
        distancia = character1->getPosX() + character1->getSobrante()
                    + character1->getWidth()
                    - (character2->getPosX() + character2->getSobrante());
    } else {
        distancia = character1->getPosX() + character1->getSobrante()
                    - (character2->getPosX() + character2->getSobrante()
                       + character2->getWidth());
    }
    return distancia;
}


character_updater_t * EventHandler::handleEvent(incoming_msg_t *msgToUpdate, int teamToUpdate, int enemyTeam) {
    int *distancia = getTeamDistances();
    actions_t actionToUpdate;

    mutex->lock();

    if (msgToUpdate->action == CHANGEME) {
        if (team[teamToUpdate]->getCurrentCharacter()->isStanding()) {
            if (team[teamToUpdate]->sizeOfTeam == 1)
                actionToUpdate = CHANGEME_ONEPLAYER;
            else
                actionToUpdate = CHANGEME;
        } else                      //why ?
            actionToUpdate =
                    team[teamToUpdate]->getCurrentCharacter()->currentAction;
        team[teamToUpdate]->update(distancia[teamToUpdate], actionToUpdate, team[enemyTeam]->getCurrentBox());

    } else {
        team[teamToUpdate]->update(distancia[teamToUpdate], msgToUpdate->action, team[enemyTeam]->getCurrentBox());
        actionToUpdate = team[teamToUpdate]->getCurrentCharacter()->currentAction;
    }

    character_updater_t *updater = makeUpdater(teamToUpdate, actionToUpdate);

    mutex->unlock();

    delete distancia;
    return updater;
}

void EventHandler::manageInteractiveActions(Queue<incoming_msg_t *> *queue, int receiver) {
    //basic hit

    cout<<"asdddddddd"<<endl;
    if (team[receiver]->getCurrentCharacter()->inTheGround()) {
        incoming_msg_t *beingHurtGround = new incoming_msg_t;
        beingHurtGround->client = team[receiver]->getCurrentCharacter()->clientNumber;
        beingHurtGround->action = HURTINGGROUND;
        queue->insert(beingHurtGround);
    }
    else if(!team[receiver]->getCurrentCharacter()->inTheGround()){
        incoming_msg_t *beingHurtGround = new incoming_msg_t;
        beingHurtGround->client = team[receiver]->getCurrentCharacter()->clientNumber;
        cout<< "asd" << endl;
        beingHurtGround->action = HURTINGAIR;
        queue->insert(beingHurtGround);

    }

    //in the air




    //grabbing


}

character_updater_t * EventHandler::makeUpdater(int teamToUpdate, actions_t action) {
    character_updater_t *updater = new character_updater_t;

    updater->posX =
            team[teamToUpdate]->getCurrentCharacter()->getPosX();
    updater->posY =
            team[teamToUpdate]->getCurrentCharacter()->getPosY();
    updater->team = teamToUpdate;
    updater->currentSprite =
            team[teamToUpdate]->getCurrentCharacter()->getSpriteNumber();
    updater->action = action;

    return updater;
}
