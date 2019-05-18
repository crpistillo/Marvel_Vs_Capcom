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

Team::Team(CharacterServer* firstCharact, CharacterServer* secondCharact, int teamSize )
{
	this->currentCharacter = firstCharact;  //es el que esta jugando ahora
	this->firstCharacter = firstCharact;
	this->secondCharacter = secondCharact;
	this->sizeOfTeam = teamSize;
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
    if(action == MAKINGINTRO && !(currentCharacter->currentAction == MAKINGINTRO) && currentCharacter->currentAction == STANDING){
        changeCharacter();  //send change character
//        setCharacterToChanging();
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
