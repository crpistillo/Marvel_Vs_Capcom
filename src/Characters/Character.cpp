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
		int width,
		int sobrante,
        bool isLookingLeft,
        string name
) {
    this->mPosX = mPosX;
    this->mPosY = mPosY;
    this->width = width;
    this->sobrante = sobrante;
    this->mVelX = 0;
    this->mVelY = 0;
    this->currentWalkingLeftSprite = 0;
    this->currentWalkingRightSprite = 0;
    this->currentStandingSprite = 0;
    this->currentWalkbackLeftSprite = 0;
    this->currentWalkbackRightSprite = 0;
    this->currentJumpingSprite = 0;
    this->currentJumpingRightSprite = 0;
    this->currentJumpingLeftSprite = 0;
    this->currentIntroSprite = 0;
    this->isLookingLeft = isLookingLeft;
    this->isStanding = true;
    this->isJumpingVertical = false;
    this->isJumpingRight = false;
    this->isJumpingLeft = false;
    this->isMakingIntro = false;
    this->loader = new ImageLoader();
    this->characterControls = NULL;

    this->lastTime = SDL_GetTicks();

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

void Character::render(SDL_Renderer *mRenderer, int camX, int camY, int posContrincante, bool suplente) {
    if (Character::mPosX > posContrincante) {
        isLookingLeft = true;
    } else {
        isLookingLeft = false;
    }

    if (isStanding)
        renderStandSprite(mRenderer);

    if (suplente) {
    	Uint8 r = 000;
    	Uint8 g = 255;
    	Uint8 b = 255;
    	m_Texture.setColor( r, g, b );
    }

    m_Texture.render(mPosX - camX, mPosY - camY, 1153, 865, mRenderer);
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

int Character::getWidth() {
    return width;
}

int Character::getSobrante() {
    return sobrante;
}

Character::~Character() {
    delete loader;
    m_Texture.free();
    this->free();
}

void Character::positionUpdate(int* x) {
    mPosX = *x;
}

void Character::setControls(Controls* controls) {
    characterControls = controls;
}

void Character::startIntro(){
	isMakingIntro = true;
}

Controls* Character::getControls()
{
	return this->characterControls;
}

bool Character::isMoving()
{
	return !(this->isStanding);
}



