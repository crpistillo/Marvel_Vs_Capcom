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
		int currentIntroSprite,
        bool isLookingLeft,
        string name
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
    this->currentIntroSprite = currentIntroSprite;
    this->isLookingLeft = isLookingLeft;
    this->isStanding = true;
    this->isJumpingVertical = false;
    this->isJumpingRight = false;
    this->isJumpingLeft = false;
    this->isMakingIntro = false;
    this->loader = new ImageLoader();
    this->characterControls = NULL;

}

// Public:
void Character::update(SDL_Renderer *renderer, int distance, int posContrincante) {
    InputManager *inputManager = InputManager::getInstance();

    if (isMakingIntro){
    	makeIntro(renderer);
    	return;
    }

    if (isJumpingVertical) {    //Si saltaba verticalmente, lo fuerzo a que siga con esa accion
        jump(renderer);
        return;
    }

    if (isJumpingRight) {        //Si saltaba hacia la derecha, lo fuerzo a que siga con esa accion

        moveRight(renderer, distance, posContrincante);
        jumpRight(renderer);
        return;
    }

    if (isJumpingLeft) {

        moveLeft(renderer, distance, posContrincante);
        jumpLeft(renderer);
        return;
    }

    //Acciones de dos teclas primero
    if (inputManager->isKeyDown(characterControls->upKey) && inputManager->isKeyDown(characterControls->rightKey)) jumpRight(renderer);
    else if (inputManager->isKeyDown(characterControls->upKey) && inputManager->isKeyDown(characterControls->leftKey)) jumpLeft(renderer);

        //Acciones de una sola tecla
    else if (inputManager->isKeyDown(characterControls->upKey)) jump(renderer);
    else if (inputManager->isKeyDown(characterControls->downKey)) renderDuckSprite(renderer);
    else if (inputManager->isKeyDown(characterControls->rightKey)) moveRight(renderer, distance, posContrincante);
    else if (inputManager->isKeyDown(characterControls->leftKey)) moveLeft(renderer, distance, posContrincante);


    if (
            (!inputManager->isKeyUp(characterControls->upKey) &&
             !inputManager->isKeyUp(characterControls->downKey) &&
             !inputManager->isKeyUp(characterControls->rightKey) &&
             !inputManager->isKeyUp(characterControls->leftKey))
            || (inputManager->isKeyUp(characterControls->rightKey) &&
                inputManager->isKeyUp(characterControls->leftKey))
            )
        this->renderStandSprite(renderer);

    updateStand();
}

void Character::render(SDL_Renderer *mRenderer, int camX, int camY, int posContrincante) {
    if (Character::mPosX > posContrincante) {
        isLookingLeft = true;
    } else {
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

void Character::positionUpdate(int* x, int* y) {
    mPosX = *x;
    mPosY = *y;
}

void Character::setControls(Controls* controls) {
    characterControls = controls;
}

void Character::startIntro(){
	isMakingIntro = true;
}



