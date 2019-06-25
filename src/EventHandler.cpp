//
// Created by IgVelasco on 6/16/19.
//

#include "EventHandler.h"
#include "CharactersServer/Projectiles/ProjectileServer.h"
#include "actions_bools.h"

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


character_updater_t *EventHandler::handleEvent(incoming_msg_t *msgToUpdate, int teamToUpdate, int enemyTeam) {
    int *distancia = getTeamDistances();
    actions_t actionToUpdate;
    music_action_t effect;
    effect = this->handleEffects(msgToUpdate,teamToUpdate,enemyTeam);

    mutex->lock();

    if (msgToUpdate->action == CHANGEME) {
        if (team[teamToUpdate]->getCurrentCharacter()->isStanding() && (team[teamToUpdate]->partnerNotDead() || debugMode)) {
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


    character_updater_t *updater = makeUpdater(teamToUpdate, actionToUpdate, FIGHTING, effect);

    mutex->unlock();

    delete distancia;
    return updater;
}

void EventHandler::manageInteractiveActions(Queue<incoming_msg_t *> *queue, int giver, int receiver, actions_t action) {
    //grip

    if (team[receiver]->getCurrentCharacter()->isHurting() || team[receiver]->getCurrentCharacter()->currentAction == MI)
        return;

    if (action == GRIP) {
        manageGrip(queue, receiver, giver);
        return;
    }

    //No hacer daño si golpeas (estando parado) y tu contrincante está agachado
    if ((action == P || action == K) && team[receiver]->getCurrentCharacter()->currentAction == DUCK)
    	return;

    //Daño solo al primer sprite de la animacion del golpe
    int spriteNumber;
    if (action == P || action == K || action == PD || action == KD || action == PV ||
    	action == KV || action == PS || action == PSD || action == PSV || action == KS ||
		action == KSD || action == KSV){
    	spriteNumber = team[giver]->getCurrentCharacter()->getSpriteNumber();
    	if (spriteNumber!=1) return;
    }

    //Disparo
    if (team[receiver]->getCurrentCharacter()->getColisionable()->isProjectileColliding(team[giver]->getCurrentCharacter()->getProjectile()))
    	if (team[receiver]->getCurrentCharacter()->currentAction == BLOCK)
    		team[receiver]->getCurrentCharacter()->quitarVida(1);
    	else
    		team[receiver]->getCurrentCharacter()->quitarVida(5);

    manageDamage(receiver, action);


    //if mod != debug
    if(team[receiver]->getCurrentCharacter()->life <= 0 && !debugMode){
        team[receiver]->getCurrentCharacter()->currentAction = HURTINGAIR;
        return;
    }

    //any other interaction
    if (team[receiver]->getCurrentCharacter()->inTheGround())
    	if (team[receiver]->getCurrentCharacter()->currentAction == BLOCK) return;
    	else	insertAction(queue, HURTINGGROUND, receiver);
    else
        insertAction(queue, HURTINGAIR, receiver);
}

void EventHandler::manageDamage(int receiver, const actions_t &action) const {

    if (action == P || action == K || action == PD || action == KD || action == PV || action == PJR || action == PJL ||
        action == KV || action == KJL || action == KJR)
        if (team[receiver]->getCurrentCharacter()->currentAction == BLOCK)
            team[receiver]->getCurrentCharacter()->quitarVida(1);
        else
            team[receiver]->getCurrentCharacter()->quitarVida(5);

    if (action == PS || action == PSD || action == PSV || action == PSJR || action == PSJL ||
        action == KS || action == KSD || action == KSV || action == KSJR || action == KSJL)
        if (team[receiver]->getCurrentCharacter()->currentAction == BLOCK)
            team[receiver]->getCurrentCharacter()->quitarVida(2);
        else
            team[receiver]->getCurrentCharacter()->quitarVida(10);
}

character_updater_t *EventHandler::makeUpdater(int teamToUpdate, actions_t action, round_action_t roundAction, music_action effect) {
    character_updater_t *updater = new character_updater_t;

    updater->round.roundInfo = roundAction;
    updater->posX =
            team[teamToUpdate]->getCurrentCharacter()->getPosX();
    updater->posY =
            team[teamToUpdate]->getCurrentCharacter()->getPosY();
    updater->team = teamToUpdate;
    updater->currentSprite =
            team[teamToUpdate]->getCurrentCharacter()->getSpriteNumber();
    updater->action = action;
    updater->effect = effect;
    updater->vida = team[teamToUpdate]->getCurrentCharacter()->getVida();


    return updater;
}

void EventHandler::handleProjectiles(character_updater_t *updater, int teamToUpdate) {
    if (!team[teamToUpdate]->getCurrentCharacter()->isProjectileActive()) {
        updater->projectile = PROJECTILEDEAD;
        updater->currentProjectileSprite = 0;
        updater->pposX = 0;
        updater->pposY = 0;
    } else {
        ProjectileServer *projectile = team[teamToUpdate]->getCurrentCharacter()->getProjectile();
        updater->projectile = projectile->itWasActiveAndDied ? PROJECTILEDEAD : PROJECTILEALIVE;
        updater->currentProjectileSprite = projectile->currentSprite;
        updater->pposX = projectile->posX;
        updater->pposY = projectile->posY;
        if (projectile->itWasActiveAndDied)
            projectile->itWasActiveAndDied = false;
    }
}

void EventHandler::insertAction(Queue<incoming_msg_t *> *queue, actions action, int teamToInsert) {
    incoming_msg_t *beingHurtGround = new incoming_msg_t;
    beingHurtGround->client = team[teamToInsert]->getCurrentCharacter()->clientNumber;
    beingHurtGround->action = action;
    queue->insert(beingHurtGround);

}

void EventHandler::manageGrip(Queue<incoming_msg_t *> *queue, int receiver, int giver) {
    if (team[receiver]->getCurrentCharacter()->inTheGround()) {
        insertAction(queue, THROW, giver);
        insertAction(queue, FALLING, receiver);
        team[receiver]->getCurrentCharacter()->quitarVida(5);
    }
}

character_updater_t *EventHandler::getRoundUpdaters(int toUpdate, Timer *timer) {
    int otherTeam;
    otherTeam = (toUpdate == 1 )? 0 : 1;
    round_action_t roundAction ;
    int *distancia = getTeamDistances();


    if (timer->getTimeThatPass() == 0)
        roundAction = ROUNDWINNER;
    //tell who won
    else if (timer->getTimeThatPass() == 1)
        roundAction = NUMBEROFROUND;
        //tell number of round
    else
        roundAction = FIGHT;

    team[toUpdate]->update(distancia[toUpdate], STANDING, team[otherTeam]->getCurrentBox());
    actions_t actionToUpdate = team[toUpdate]->getCurrentCharacter()->currentAction;


    character_updater_t* roundUpdater = makeUpdater(toUpdate,actionToUpdate , roundAction, NEW_ROUND);
    roundUpdater->firstDigitOfTime = 9;
    roundUpdater->secondDigitOfTime = 9;
}

void EventHandler::manageDeadCharacter(int receiver, Queue<incoming_msg_t *> *queue) {
    if(team[receiver]->getCurrentCharacter()->life <= 0 && !debugMode && team[receiver]->getCurrentCharacter()->isStanding()){
        insertAction(queue,STANDING,receiver);
        insertAction(queue, CHANGEME, receiver);
        return;
    }
}

void EventHandler::switchDebug() {
    debugMode = !debugMode;

}

music_action_t EventHandler::handleEffects(incoming_msg_t *msgToUpdate, int teamToUpdate, int enemyTeam)
{
	music_action_t effect;
    if(isWeakPunch(msgToUpdate->action) && isHurting(team[enemyTeam]->getCurrentCharacter()->currentAction))
    //|| isHurting(msgToUpdate->action) && isWeakPunch(team[enemyTeam]->getCurrentCharacter()->currentAction))
    	effect = WEAK_PUNCH;




    else if (isWeakKick(msgToUpdate->action) && isHurting(team[enemyTeam]->getCurrentCharacter()->currentAction))
    //|| isHurting(msgToUpdate->action) && isWeakKick(team[enemyTeam]->getCurrentCharacter()->currentAction))
    	effect = WEAK_KICK;

    else if(isHit(msgToUpdate->action) && !isHurting(team[enemyTeam]->getCurrentCharacter()->currentAction)
            && team[teamToUpdate]->getCurrentCharacter()->getSpriteNumber() == 1)// ||
			//!isHurting(msgToUpdate->action) && isHit(team[enemyTeam]->getCurrentCharacter()->currentAction))
    	effect = HIT_MISS;

    else if(isStrongPunch(msgToUpdate->action) && isHurting(team[enemyTeam]->getCurrentCharacter()->currentAction))
    // || isHurting(msgToUpdate->action) && isStrongPunch(team[enemyTeam]->getCurrentCharacter()->currentAction))
     	effect = STRONG_PUNCH;

    else if (isStrongKick(msgToUpdate->action) && isHurting(team[enemyTeam]->getCurrentCharacter()->currentAction))
    //|| isHurting(msgToUpdate->action) && isStrongKick(team[enemyTeam]->getCurrentCharacter()->currentAction))
    	effect = STRONG_KICK;

    else if(isJump(msgToUpdate->action) &&
    		 team[teamToUpdate]->getCurrentCharacter()->getSpriteNumber() == 1)
		effect = JUMP;

    else if(msgToUpdate->action == FALLING)
    	effect  = FALL;

    else if(msgToUpdate->action == THROW)
    	effect = THROWS;

	else if (msgToUpdate->action == CHANGEME && team[teamToUpdate]->getCurrentCharacter()->isStanding() && (team[teamToUpdate]->partnerNotDead()))
	{
		switch (team[teamToUpdate]->getCharacterInactive()->nameOfCharacter()){
            case SPIDERMAN:
                effect = SPIDERINTRO;
                break;
            case WOLVERINE:
                effect = WOLVERINTRO;
                break;
            case IRONMAN:
            case RYU:
                effect = WOLVERINTRO;
                break;
        }
	}
    return effect;
}


