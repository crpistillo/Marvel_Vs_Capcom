/*
 * Team.cpp
 *
 *  Created on: 18 may. 2019
 *      Author: caropistillo
 */

#include "Team.h"
#include "CharactersServer/SpidermanServer.h"
#include "CharactersServer/WolverineServer.h"

const int MAX_PLAYERS = 4;

Team::Team(CharacterServer* firstCharact, CharacterServer* secondCharact, int teamSize, int teamNumber )
{
	this->currentCharacter = firstCharact;  //es el que esta jugando ahora
	this->firstCharacter = firstCharact;
	this->secondCharacter = secondCharact;
	this->sizeOfTeam = teamSize;
	this->teamNumber = teamNumber;
	isChanging = false;
	clientActive = currentCharacter->clientNumber;

}

void Team::changeCharacter()
{

    int updateX = currentCharacter->getCentro();

    if(currentCharacter == firstCharacter) {
        currentCharacter = secondCharacter;
    }
    else {
        currentCharacter = firstCharacter;
    }
    currentCharacter->positionUpdate(&updateX);


	this->clientActive = currentCharacter->clientNumber;

}


void Team::update(int distance, int posContrincante, actions_t action) {

	if(action == CHANGEME)
	{
		changeCharacter();
		isChanging = true;
	}

    if(!(currentCharacter->currentAction == MAKINGINTRO))
        isChanging = false;

    currentCharacter->update(distance, posContrincante, action);
}

void Team::changeClient(){

    if(currentCharacter == firstCharacter)
        this->clientActive = secondCharacter->clientNumber;
    else
        this->clientActive = firstCharacter->clientNumber;

}

void Team::makeUpdater(character_updater_t* updater){
	updater->team = this->teamNumber;
	this->currentCharacter->makeUpdaterStruct(updater);
}

CharacterServer* Team::get_currentCharacter()
{
	return this->currentCharacter;
}
