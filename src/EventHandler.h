//
// Created by IgVelasco on 6/16/19.
//

#ifndef MARVEL_VS_CAPCOM_EVENTHANDLER_H
#define MARVEL_VS_CAPCOM_EVENTHANDLER_H


#include <mutex>
#include "Team.h"
#include "Queue/Queue.h"

class EventHandler {

public:
    EventHandler(Team *team[2], mutex *mutex);

    Team **team;
    std::mutex *mutex;

    int *getTeamDistances();

    int computeDistance(CharacterServer *character1, CharacterServer *character2);

    character_updater_t * handleEvent(incoming_msg_t *msgToUpdate, int teamToUpdate, int enemyTeam);

    void manageInteractiveActions(Queue<incoming_msg_t *> *queue, int receiver);

    character_updater_t * makeUpdater(int teamToUpdate, actions_t action);

    void handleProjectiles(character_updater_t *updater, int teamToUpdate);
};


#endif //MARVEL_VS_CAPCOM_EVENTHANDLER_H
