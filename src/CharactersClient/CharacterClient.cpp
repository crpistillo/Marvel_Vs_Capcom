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
                                 int heightSprite, int anchoPantalla, int numOfClient) {
    this->mPosX = mPosX;
    this->mPosY = mPosY;
    this->width = width;
    this->sobrante = sobrante;
    this->widthSprite = widthSprite;
    this->heightSprite = heightSprite;
    this->ZIndex = 0;
    this->anchoPantalla = anchoPantalla;
    this->characterFilePath = "";
    this->currentSprite = 0;
    this->isLookingLeft = isLookingLeft;
    currentAction = STANDING;
    this->clientNumber = numOfClient;

    this->loader = NULL;
    this->characterControls = NULL;

    this->lastTime = SDL_GetTicks();
    this->vida = 100;

}

void CharacterClient::render(SDL_Renderer *mRenderer, int camX, int camY, int posContrincante) {
    isLookingLeft = this->getCentro() > posContrincante;
    m_Texture.render(mPosX - camX, mPosY - camY, widthSprite, heightSprite,
                     mRenderer); //esto es los valores que se cambian la resolucion
}

void CharacterClient::renderBanner(SDL_Renderer *mRenderer)
{
	if(clientNumber == 0 || clientNumber == 1)
		characterLeftBanner.render(0,29,368,72,mRenderer);
	else characterRightBanner.render(432,29,368,72,mRenderer);
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
    characterLeftBanner.free();
    characterRightBanner.free();
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
	return !((this->currentAction == STANDING) || (this->currentAction == DUCK)
			|| (this->currentAction == JUMPINGVERTICAL) || (this->currentAction == MAKINGINTRO)
			|| (this->currentAction == CHANGEME) || (this->currentAction == KICK)
			|| (this->currentAction == PUNCH) || (this->currentAction == PUNCHDOWN)
			|| (this->currentAction == KICKDOWN) || (this->currentAction == BLOCK)
			|| (this->currentAction == THROWPOWER) || (this->currentAction == GRIP)
			|| (this->currentAction == FALLING) || (this->currentAction == THROW)
			|| (this->currentAction == PUNCHSTRONG) || (this->currentAction == PUNCHSTRONGDOWN)
			|| (this->currentAction == KICKSTRONG) || (this->currentAction == KICKSTRONGDOWN));
}

int CharacterClient::getZIndex() {
    return this->ZIndex;
}

void CharacterClient::setZIndex(int z) {
    this->ZIndex = z;
}

void CharacterClient::setFilepath(string fp) {
    this->characterFilePath = fp;
}

void CharacterClient::update(character_updater_t *updater) {
    if (updater->action != CHANGEME)
        currentAction = updater->action;
    mPosX = updater->posX;
    mPosY = updater->posY;
    currentSprite = updater->currentSprite;
    vida = updater->vida;
}

ProjectileClient *CharacterClient::getProjectile() {
    return nullptr;
}

bool CharacterClient::getDirection() {
    return isLookingLeft;
}



