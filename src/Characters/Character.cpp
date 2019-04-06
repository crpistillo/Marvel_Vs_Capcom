/*
 * Character.cpp
 *
 *  Created on: 2 abr. 2019
 *      Author: sebastian
 */

#include "Character.h"

// Protected
Character::Character(
	int mPosX,
	int mPosY,
	int currentWalkingLeftSprite,
	int currentWalkingRightSprite,
	int currentStandingSprite,
	int currentJumpingSprite,
	int currentJumpingRightSprite,
	int currentJumpingLeftSprite,
	int currentWalkbackLeftSprite,
	int currentWalkbackRightSprite,
	bool isLookingLeft,
	string name,
	int upKey,
	int downKey,
	int rightKey,
	int leftKey
) {
	this->mPosX = mPosX;
	this->mPosY = mPosY;
	this->mVelX = 0;
	this->mVelY = 0;
	this->currentWalkingLeftSprite = currentWalkingLeftSprite;
	this->currentWalkingRightSprite = currentWalkingRightSprite;
	this->currentStandingSprite = currentStandingSprite;
	this->currentWalkbackLeftSprite = currentWalkingLeftSprite;
	this->currentWalkbackRightSprite = currentWalkingRightSprite;
	this->currentJumpingSprite = currentJumpingSprite;
	this->currentJumpingRightSprite = currentJumpingRightSprite;
	this->currentJumpingLeftSprite = currentJumpingLeftSprite;
	this->isLookingLeft = isLookingLeft;
	this->isStanding = true;
	this->isJumpingVertical = false;
	this->isJumpingRight = false;
	this->isJumpingLeft = false;
	this->upKey = upKey;
	this->downKey = downKey;
	this->rightKey = rightKey;
	this->leftKey = leftKey;
}

// Public:
void Character::update(SDL_Renderer* renderer, int distance, int posContrincante) {
	InputManager* inputManager = InputManager::getInstance();

	if(isJumpingVertical){    //Si saltaba verticalmente, lo fuerzo a que siga con esa accion
		jump(renderer);
		return;
	}

	if(isJumpingRight){		//Si saltaba hacia la derecha, lo fuerzo a que siga con esa accion

		moveRight(renderer, distance, posContrincante);
		jumpRight(renderer);
		return;
	}

	if(isJumpingLeft){

		moveLeft(renderer, distance, posContrincante);
		jumpLeft(renderer);
		return;
	}

	//Acciones de dos teclas primero
	if(inputManager->isKeyDown(upKey) && inputManager->isKeyDown(rightKey)) jumpRight(renderer);
	else if(inputManager->isKeyDown(upKey) && inputManager->isKeyDown(leftKey)) jumpLeft(renderer);

	//Acciones de una sola tecla
	else if(inputManager->isKeyDown(upKey)) jump(renderer);
	else if(inputManager->isKeyDown(downKey)) renderDuckSprite(renderer);
	else if(inputManager->isKeyDown(rightKey)) moveRight(renderer, distance, posContrincante);
	else if(inputManager->isKeyDown(leftKey)) moveLeft(renderer, distance, posContrincante);





	if(
		(!inputManager->isKeyUp(upKey) &&
		!inputManager->isKeyUp(downKey) &&
		!inputManager->isKeyUp(rightKey) &&
		!inputManager->isKeyUp(leftKey))
		|| (inputManager->isKeyUp(rightKey) &&
		inputManager->isKeyUp(leftKey))
	) this->renderStandSprite(renderer);

	updateStand();
}

void Character::render(SDL_Renderer *mRenderer, int camX, int camY, int posContrincante) {
    if (Character::mPosX > posContrincante){
    	isLookingLeft = true;
    }
    else {
    	isLookingLeft = false;
    }

	if (isStanding)
        renderStandSprite(mRenderer);
    m_Texture.render(mPosX - camX, mPosY - camY, mRenderer);
}

void Character::free() {
    m_Texture.free();
}

int Character::getPosX() {
	return mPosX;
}

int Character::getPosY() {
    return mPosY;
}

Character::~Character() {
	this->free();
}


