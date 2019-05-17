/*
 * Character.cpp
 *
 *  Created on: 2 abr. 2019
 *      Author: sebastian
 */

#include "Character.h"
#include "../tools/logger/Logger.h"

const string ERROR = "ERROR";
const string INFO = "INFO";
const string DEBUG = "DEBUG";


// Protected
Character::Character(
        int mPosX,
        int mPosY,
		int width,
		int sobrante,
        bool isLookingLeft,
		int widthSprite,
		int heightSprite,
		int anchoPantalla
) {
    this->mPosX = mPosX;
    this->mPosY = mPosY;
    this->width = width;
    this->sobrante = sobrante;
    this->widthSprite = widthSprite;
    this->heightSprite = heightSprite;
    this->INITIAL_POS_Y = mPosY;
    this->mVelX = 0;
    this->mVelY = 0;
    this->ZIndex = 0;
    this->anchoPantalla = anchoPantalla;
    this->characterFilepath = "";
    this->currentWalkingSprite = 0;
    this->currentStandingSprite = 0;
    this->currentWalkbackSprite = 0;
    this->currentJumpingSprite = 0;
    this->currentJumpingRightSprite = 0;
    this->currentJumpingLeftSprite = 0;
    this->currentIntroSprite = 0;
    this->isLookingLeft = isLookingLeft;
    currentAction = STANDING;

    this->loader = NULL;
    this->characterControls = NULL;

    this->lastTime = SDL_GetTicks();

}

// Public:
void Character::update(SDL_Renderer *renderer, int distance, int posContrincante) {

    InputManager *inputManager = InputManager::getInstance();

    bool actionStarted = false;

    if(this->currentAction == MI || this->currentAction == JV || this->currentAction == JR || this ->currentAction == JL)
        actionStarted = true;

    if (currentAction == MAKINGINTRO){
    	makeIntro();
    }

    if (currentAction == JUMPINGVERTICAL) {    //Si saltaba verticalmente, lo fuerzo a que siga con esa accion
        jumpVertical();
    }

    if (currentAction == JUMPINGRIGHT) {        //Si saltaba hacia la derecha, lo fuerzo a que siga con esa accion

        moveRight(distance, posContrincante);
        jumpRight();
    }

    if (currentAction == JUMPINGLEFT) {
        moveLeft( distance, posContrincante);
        jumpLeft();
    }

    if(actionStarted){
        load(renderer, posContrincante);
        return;
    }

    //Acciones de dos teclas primero
    if (inputManager->isKeyDown(characterControls->upKey) && inputManager->isKeyDown(characterControls->rightKey))
        jumpRight();
    else if (inputManager->isKeyDown(characterControls->upKey) && inputManager->isKeyDown(characterControls->leftKey))
        jumpLeft();

        //Acciones de una sola tecla
    else if (inputManager->isKeyDown(characterControls->upKey))
        jumpVertical();
    else if (inputManager->isKeyDown(characterControls->downKey))
        renderDuckSprite();
    else if (inputManager->isKeyDown(characterControls->rightKey) && !inputManager->isKeyUp(characterControls->leftKey))
        moveRight(distance, posContrincante);
    else if (inputManager->isKeyDown(characterControls->leftKey) && !inputManager->isKeyUp(characterControls->rightKey))
        moveLeft(distance, posContrincante);


    if (
            (!inputManager->isKeyUp(characterControls->upKey) &&
             !inputManager->isKeyUp(characterControls->downKey) &&
             !inputManager->isKeyUp(characterControls->rightKey) &&
             !inputManager->isKeyUp(characterControls->leftKey))
            || (inputManager->isKeyUp(characterControls->rightKey) &&
                inputManager->isKeyUp(characterControls->leftKey))
            )
        this->stand();
    load(renderer, posContrincante);

    updateStand();
}

void Character::render(SDL_Renderer *mRenderer, int camX, int camY, int posContrincante) {
    isLookingLeft = this->getCentro() > posContrincante;
    m_Texture.render(mPosX - camX, mPosY - camY, widthSprite, heightSprite, mRenderer); //esto es los valores que se cambian la resolucion
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

int Character::getCentro() {
    int centro;
    centro = this->getPosX()+this->getSobrante()+(this->getWidth())/2;
    return centro;
}

Character::~Character() {
    delete loader;
    m_Texture.free();
}

void Character::positionUpdate(int* x) {
	/*x tiene el centro del personaje (ubicacion exacta del personaje)
	 * La posicion en x se calcula con eso*/
    mPosX = *x - this->getSobrante() - (this->getWidth()/2);
}

void Character::setControls(Controls* controls) {
    characterControls = controls;
}

void Character::startIntro(){
    currentAction = MAKINGINTRO;
}

Controls* Character::getControls()
{
	return this->characterControls;
}

bool Character::isMoving()
{
	return (!(this->currentAction == STANDING) || !(this->currentAction == DUCK));
}

int Character::getZIndex(){
	return this->ZIndex;
}

void Character::setZIndex(int z){
	this->ZIndex = z;
}

void Character::setFilepath(string fp){
	this->characterFilepath = fp;
}



void Character::resetSpriteVariables() {
    mPosY = this->INITIAL_POS_Y;
    currentJumpingSprite = 0;
    currentWalkingSprite = 0;
}




void Character::stand() {
    currentAction = STANDING;
    this->resetSpriteVariables();
    if (currentStandingSprite >= lastStandingSprite)
        currentStandingSprite = 0;
}



void Character::renderDuckSprite() {
    currentAction = DUCK;
}




void Character::jump(int *currentSprite, int lastSprite) {

    *currentSprite < 10 ? (mPosY -= 2.5 * CHARACTER_VEL) : (mPosY += 2.5 * CHARACTER_VEL);
    (*currentSprite)++;
    if (*currentSprite > lastSprite) {
        *currentSprite = 0;
        mPosY = this->INITIAL_POS_Y;
        this->currentAction = STANDING;
        currentStandingSprite = 0;
    }
}

void Character::jumpVertical() {
    this->currentAction = JUMPINGVERTICAL;
    jump(&currentJumpingSprite, lastJumpingSprite);
}

void Character::jumpRight() {
    this->currentAction = JUMPINGRIGHT;
    jump(&currentJumpingRightSprite, lastJumpingRightSprite);

}


void Character::jumpLeft() {
    this->currentAction = JUMPINGLEFT;
    jump(&currentJumpingLeftSprite, lastJumpingLeftSprite);
}


void Character::makeIntro() {
    currentAction = MAKINGINTRO;


    unsigned int currentTime = SDL_GetTicks();


    if (currentIntroSprite < lastIntroSprite) {
        ++currentIntroSprite;
        lastTime = currentTime;
    }


    if (currentIntroSprite >= lastIntroSprite && (currentTime - lastTime) > 500) {
        currentIntroSprite = 0;
        currentAction = STANDING;
        currentStandingSprite = 0;
    }

}

void Character::updateStand() {
    if (currentStandingSprite <= lastStandingSprite)
        currentStandingSprite++;
}






