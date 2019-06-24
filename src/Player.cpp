//
// Created by amaherok on 4/6/19.
//

#include "Player.h"
#include "CharactersClient/ProjectileClient.h"

const string ERROR = "ERROR";
const string INFO = "INFO";
const string DEBUG = "DEBUG";

SDL_Color color(Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
		SDL_Color col = {r,g,b,a};
		return col;
}

const SDL_Color frontCurrentColor = color(255,255,0,1);
const SDL_Color backCurrentColor = color(167,000,0,0.57);
const SDL_Color frontSecondaryColor = color(153,128,0,0.4);
const SDL_Color backSecondaryColor = color(77,0,0,0.57);

Player::Player(CharacterClient *first, CharacterClient *second) {
    Logger *logger = Logger::getInstance();
    logger->log("Inicializacion de personajes para jugador.", DEBUG);
    currentCharacter = first;
    firstCharacter = first;
    secondCharacter = second;
	barraCurrent = new Barra();
	barraSecondary = new Barra();
	vidaSecondary = 50;
}

void Player::update(character_updater_t *updater, bool *isSending, bool becomeActive, int clientNumber) {

    ProjectileClient* projectile = currentCharacter->getProjectile();
    if(projectile)
        projectile->update(updater->projectile == PROJECTILEALIVE, updater);

    if (updater->action == RECONNECT) {
        //SI ES DE MI EQUIPO ME TENGO QUE FIJAR SI TENGO QUE ESTAR ACTIVO
        //SI EL CURRENTCHARACTER -> CLIENTE
        if (becomeActive) {
            m.lock();
            *isSending = (clientNumber == currentCharacter->clientNumber);
            m.unlock();
        }

        return;
    }


    if (updater->action == DISCONNECTEDCLIENT) {
        m.lock();
        if (becomeActive)
        {
        	 *isSending = true;
        }

        m.unlock();
        //changeCharacter();  //send change character
    } else if (updater->action == CHANGEME_ONEPLAYER) {
        changeCharacter();
        setCharacterToChanging();
    } else if (updater->action == CHANGEME) {
        m.lock();
        if (becomeActive)
        {
        	*isSending = !(*isSending);
        }
        m.unlock();
        changeCharacter();  //send change character
        setCharacterToChanging();
    }

    barraCurrent->update(updater->vida);
    if(currentCharacter == firstCharacter)
    	barraSecondary->update(secondCharacter->vida);
    else barraSecondary->update(firstCharacter->vida);

    currentCharacter->update(updater);
    Logger *logger = Logger::getInstance();
    InputManager *inputManager = InputManager::getInstance();
    logger->log("Detecta boton para cambio de personaje en Player.", DEBUG);

}


void Player::render(SDL_Renderer *mRenderer, int camX, int camY, int posContrincante) {
    Logger *logger = Logger::getInstance();
    logger->log("Renderizado de personaje - Render.", DEBUG);
    currentCharacter->render(mRenderer, camX, camY, posContrincante);
    this->barraCurrent->render(mRenderer,frontCurrentColor, backCurrentColor);
    this->barraSecondary->render(mRenderer,frontSecondaryColor,backSecondaryColor);

}

void Player::renderProyectiles(SDL_Renderer *mRenderer, int camX, int camY){
    ProjectileClient *projectile = currentCharacter->getProjectile();
    if (projectile)
        projectile->render(mRenderer, camX, camY);
}

void Player::renderBanner(SDL_Renderer *mRenderer)
{
	currentCharacter->renderBanner(mRenderer, true);
	if(currentCharacter == firstCharacter)
		secondCharacter->renderBanner(mRenderer,false);
	else firstCharacter->renderBanner(mRenderer,false);
}


void Player::free() {
    delete firstCharacter;
    delete secondCharacter;
}

void Player::changeCharacter() {
    /*En lugar de madarle al otro personaje la posicion en x del sprite,
     * se le envia el centro (justo la posicion del personaje donde debe estar)*/
    int updateX = currentCharacter->getCentro();

    if (currentCharacter == firstCharacter) {
        currentCharacter = secondCharacter;
    } else {
        currentCharacter = firstCharacter;
    }
    currentCharacter->positionUpdate(&updateX);
    //animacion

}

void Player::setCharacterToChanging() {
    currentCharacter->startIntro();
}

void Player::loads(SDL_Renderer *pRenderer, int posContrincante) {
    firstCharacter->load(pRenderer, posContrincante);
    secondCharacter->load(pRenderer, posContrincante);
}

int Player::getPosX() {
    return currentCharacter->getPosX();
}


int Player::getWidth() {
    return currentCharacter->getWidth();
}

int Player::getSobrante() {
    return currentCharacter->getSobrante();
}

int Player::getCentro() {
    return currentCharacter->getCentro();
}

CharacterClient *Player::getCurrentCharacter() {
    return this->currentCharacter;
}

Player::~Player() {
    delete controls;
    delete firstCharacter;
    delete secondCharacter;

}

int Player::getZIndex() {
    return this->currentCharacter->getZIndex();

}

void Player::load(SDL_Renderer *pRenderer, int posContrincante) {
    ProjectileClient* projectile = currentCharacter->getProjectile();
    if(projectile)
        projectile->load(pRenderer, false);
    currentCharacter->load(pRenderer, posContrincante);
}

void Player::setCurrentCharacter(int i, SDL_Renderer *renderer) {
    if(i == 0)
        currentCharacter = firstCharacter;
    else
        currentCharacter = secondCharacter;

    currentCharacter->loadBanner(renderer);

	if(currentCharacter == firstCharacter)
		secondCharacter->loadBanner(renderer);
	else firstCharacter->loadBanner(renderer);

}

void Player::setBarra(bool left)
{
	barraCurrent->setBarra(left,true);
	barraSecondary->setBarra(left,false);
}

void Player::resetLifeBanners() {
    barraCurrent->update(1);
    barraSecondary->update(1);
    firstCharacter->vida = 100;
    secondCharacter->vida = 100;
}

void Player::renderWinners(SDL_Renderer *mRenderer){
	firstCharacter->renderVictoryPose(mRenderer, -100);
	secondCharacter->renderVictoryPose(mRenderer, 300);
}



