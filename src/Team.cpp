/*
 * Team.cpp
 *
 *  Created on: 18 may. 2019
 *      Author: caropistillo
 */

#include "Team.h"
#include "CharactersServer/Characters/SpidermanServer.h"
#include "CharactersServer/Characters/WolverineServer.h"

const int MAX_PLAYERS = 4;

Team::Team(int teamSize) {
	this->sizeOfTeam = teamSize;
	this->roundsWon = 0;
}

void Team::changeCharacter() {
	int updateX = currentCharacter->getCentro();
	currentCharacter->getProjectile()->deactivate();


	if (currentCharacter == firstCharacter) {
		currentCharacter = secondCharacter;
	} else {
        currentCharacter = firstCharacter;
	}
	currentCharacter->positionUpdate(&updateX);
	currentCharacter->updateBox();
}

void Team::update(int distance, actions_t action, Box *boxContrincante) {

	if (action == DISCONNECTEDCLIENT) {
		disconnectClient();
	}

	if(action == RECONNECT){
	    connectClient();
	}

	if ((action == CHANGEME || action == CHANGEME_ONEPLAYER) && currentCharacter->currentAction == STANDING) {
        changeCharacter();
		if (sizeOfTeam == 2) {
			this->clientActive = currentCharacter->clientNumber;
			cout<<"El cliente activo es "<<clientActive<<endl;
		}
		this->currentCharacter->currentAction = MAKINGINTRO;
    }

	/// Remove comment to see contact or test
	/*if(currentCharacter->getColisionable()->isColliding(boxContrincante) && !currentCharacter->inTheGround()){
        cout << " mi caja " << currentCharacter->getColisionable()->getTop() << " and " << currentCharacter->getColisionable()->getBottom() << endl;
        cout << " su caja " << boxContrincante->getTop() << " and " << boxContrincante->getBottom() << endl;
	}*/


    //cout << (currentCharacter->getColisionable()->isColliding(boxContrincante) ? "CONTACT BIATCH" : "NO") << endl;

	currentCharacter->update(distance, boxContrincante->getCenter(), action, boxContrincante);
}

void Team::disconnectClient() {

	if (currentCharacter == firstCharacter) {
		this->clientActive = secondCharacter->clientNumber;
	}

	else {
		this->clientActive = firstCharacter->clientNumber;
	}
	this->sizeOfTeam--;
}

CharacterServer* Team::getCurrentCharacter() {
	return this->currentCharacter;
}

bool Team::invalidIntroAction() {
	actions_t action = currentCharacter->currentAction;

	return !(action == STANDING || action == CHANGEME || action == MAKINGINTRO);
}

CharacterServer * Team::getFirstCharacter() {
	return this->firstCharacter;

}

CharacterServer * Team::getSecondCharacter() {
	return this->secondCharacter;
}

void Team::setSize(int size) {
	this->sizeOfTeam = size;
}

void Team::setClientNumberToCurrentClient() {
	this->clientActive = currentCharacter->clientNumber;
}

int Team::get_currentCharacterNumber() {
    if(currentCharacter == firstCharacter)
        return 0;
    else
        return 1;

}

void Team::setCharacters(CharacterServer *firstCharact, CharacterServer *secondCharact) {
    this->currentCharacter = firstCharact;  //es el que esta jugando ahora
    this->firstCharacter = firstCharact;
    this->secondCharacter = secondCharact;
    clientActive = currentCharacter->clientNumber;

}

void Team::connectClient() {
    this->sizeOfTeam++;
    this->setClientNumberToCurrentClient();
}

bool Team::collidesWith(Team *enemyTeam) {
    if (currentCharacter->isProjectileActive()) {
        return enemyTeam->getCurrentCharacter()->getColisionable()->isProjectileColliding(
                getCurrentCharacter()->getProjectile());
    }
    return this->currentCharacter->getColisionable()->isColliding(enemyTeam->currentCharacter->getColisionable());
}

Box *Team::getCurrentBox() {
    return  currentCharacter->getColisionable();
}

float Team::getSumOfLife() {
    return firstCharacter->life + secondCharacter->life;
}

bool Team::partnerNotDead() {
    if(currentCharacter == firstCharacter)
        return secondCharacter->life > 0;
    else
        return firstCharacter->life > 0;
}

bool Team::areBothCharactersDead() {
    return firstCharacter->life <= 0 && secondCharacter->life <= 0;
}

void Team::resetCharacterLife() {
    firstCharacter->life = 100;
    secondCharacter->life = 100;
}

float Team::lifeOfCharacterInactive() {
    if(currentCharacter == firstCharacter)
        return secondCharacter->life;
    else
        return firstCharacter->life;
}

void Team::incrementRoundsWon() {
    this->roundsWon++;
}

int Team::getRoundsWon() {
    return this->roundsWon;
}


