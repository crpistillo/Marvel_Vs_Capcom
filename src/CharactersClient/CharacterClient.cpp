//
// Created by IgVelasco on 5/17/19.
//

#include "CharacterClient.h"
#include "../tools/logger/Logger.h"
#include "CharacterClient.h"

const string ERROR = "ERROR";
const string INFO = "INFO";
const string DEBUG = "DEBUG";


// Protected
CharacterClient::CharacterClient(int mPosX, int mPosY, int width, int sobrante, bool isLookingLeft, int widthSprite,
                                 int heightSprite, int anchoPantalla, int numberOfClient) {
    this->mPosX = mPosX;
    this->mPosY = mPosY;
    this->width = width;
    this->sobrante = sobrante;
    this->widthSprite = widthSprite;
    this->heightSprite = heightSprite;
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

    this->clientNumber = numberOfClient;

    this->loader = NULL;
    this->characterControls = NULL;

    this->lastTime = SDL_GetTicks();

}

// Public:
actions_t CharacterClient::getNewAction(SDL_Renderer *renderer, int distance, int posContrincante) {

    InputManager *inputManager = InputManager::getInstance();


    if (currentAction == MAKINGINTRO)
        return MAKINGINTRO;

    if (currentAction == JUMPINGVERTICAL)     //Si saltaba verticalmente, lo fuerzo a que siga con esa accion
        return JUMPINGVERTICAL;

    if (currentAction == JUMPINGRIGHT)        //Si saltaba hacia la derecha, lo fuerzo a que siga con esa accion
        return JUMPINGRIGHT;

    if (currentAction == JUMPINGLEFT)
        return JUMPINGLEFT;



    //Acciones de dos teclas primero
    if (inputManager->isKeyDown(characterControls->upKey) && inputManager->isKeyDown(characterControls->rightKey))
        return JUMPINGRIGHT;

    else if (inputManager->isKeyDown(characterControls->upKey) && inputManager->isKeyDown(characterControls->leftKey))
        return JUMPINGLEFT;
        //Acciones de una sola tecla

    else if (inputManager->isKeyDown(characterControls->upKey))
        return JUMPINGVERTICAL;

    else if (inputManager->isKeyDown(characterControls->downKey))
        return DUCK;

    else if (inputManager->isKeyDown(characterControls->rightKey) && !inputManager->isKeyUp(characterControls->leftKey))
        return MOVINGRIGHT;

    else if (inputManager->isKeyDown(characterControls->leftKey) && !inputManager->isKeyUp(characterControls->rightKey))
        return MOVINGLEFT;  //add moveleft



    else if (
            (!inputManager->isKeyUp(characterControls->upKey) &&
             !inputManager->isKeyUp(characterControls->downKey) &&
             !inputManager->isKeyUp(characterControls->rightKey) &&
             !inputManager->isKeyUp(characterControls->leftKey))
            || (inputManager->isKeyUp(characterControls->rightKey) &&
                inputManager->isKeyUp(characterControls->leftKey))
            )
        return STANDING;
}

void CharacterClient::render(SDL_Renderer *mRenderer, int camX, int camY, int posContrincante) {
    isLookingLeft = this->getCentro() > posContrincante;
    m_Texture.render(mPosX - camX, mPosY - camY, widthSprite, heightSprite,
                     mRenderer); //esto es los valores que se cambian la resolucion
}


int CharacterClient::getPosX() {
    return mPosX;
}

int CharacterClient::getPosY() {
    return mPosY;
}

int CharacterClient::getWidth() {
    return width;
}

int CharacterClient::getSobrante() {
    return sobrante;
}

int CharacterClient::getCentro() {
    int centro;
    centro = this->getPosX() + this->getSobrante() + (this->getWidth()) / 2;
    return centro;
}

CharacterClient::~CharacterClient() {
    delete loader;
    m_Texture.free();
}

void CharacterClient::positionUpdate(int *x) {
    /*x tiene el centro del personaje (ubicacion exacta del personaje)
     * La posicion en x se calcula con eso*/
    mPosX = *x - this->getSobrante() - (this->getWidth() / 2);
}

void CharacterClient::setControls(Controls *controls) {
    characterControls = controls;
}

void CharacterClient::startIntro() {
    currentAction = MAKINGINTRO;
}

Controls *CharacterClient::getControls() {
    return this->characterControls;
}

bool CharacterClient::isMoving() {
    return (!(this->currentAction == STANDING) || !(this->currentAction == DUCK));
}

int CharacterClient::getZIndex() {
    return this->ZIndex;
}

void CharacterClient::setZIndex(int z) {
    this->ZIndex = z;
}

void CharacterClient::setFilepath(string fp) {
    this->characterFilepath = fp;
}





