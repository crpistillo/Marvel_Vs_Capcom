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
	this->currentJumpingSprite = currentJumpingSprite;
	this->isLookingLeft = isLookingLeft;
	this->isStanding = true;
	this->upKey = upKey;
	this->downKey = downKey;
	this->rightKey = rightKey;
	this->leftKey = leftKey;
}

// Public:
void Character::update(SDL_Renderer* renderer, int distance, int posContrincante) {
	InputManager* inputManager = InputManager::getInstance();

	if(inputManager->isKeyDown(upKey)) jump(renderer);
	if(inputManager->isKeyDown(downKey)) renderDuckSprite(renderer);
	if(inputManager->isKeyDown(rightKey)) moveRight(renderer, distance, posContrincante);
	if(inputManager->isKeyDown(leftKey)) moveLeft(renderer, distance, posContrincante);

	if(
		inputManager->isKeyUp(upKey) ||
		inputManager->isKeyUp(downKey) ||
		inputManager->isKeyUp(rightKey) ||
		inputManager->isKeyUp(leftKey)
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
    //243 es una corrección debida a que el ancho de la imagen es mas grande que el personaje
	return mPosX;
}

int Character::getPosY() {
    return mPosY;
}

Character::~Character() {
	this->free();
}


