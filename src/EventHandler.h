//
// Created by IgVelasco on 6/16/19.
//

#ifndef MARVEL_VS_CAPCOM_EVENTHANDLER_H
#define MARVEL_VS_CAPCOM_EVENTHANDLER_H


#include <mutex>
#include "Team.h"
#include "Queue/Queue.h"
#include "Timer.h"

class EventHandler {

public:
    EventHandler(Team *team[2], mutex *mutex);

    Team **team;
    std::mutex *mutex;

    int *getTeamDistances();

    int computeDistance(CharacterServer *character1, CharacterServer *character2);

    character_updater_t * handleEvent(incoming_msg_t *msgToUpdate, int teamToUpdate, int enemyTeam);

    void manageInteractiveActions(Queue<incoming_msg_t *> *queue, int giver, int receiver, actions_t action);

    character_updater_t *makeUpdater(int teamToUpdate, actions_t action, round_action_t roundAction);

    void handleProjectiles(character_updater_t *updater, int teamToUpdate);

    void insertAction(Queue<incoming_msg_t *> *queue, actions action, int teamToInsert);

    void manageGrip(Queue<incoming_msg_t *> *queue, int receiver, int giver);

    character_updater_t *getRoundUpdaters(int toUpdate, Timer *pTimer);

    void manageDamage(int receiver, const actions_t &action) const;
};


#endif //MARVEL_VS_CAPCOM_EVENTHANDLER_H
