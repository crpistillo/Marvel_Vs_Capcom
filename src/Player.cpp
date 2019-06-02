//
// Created by amaherok on 4/6/19.
//

#include "Player.h"

const string ERROR = "ERROR";
const string INFO = "INFO";
const string DEBUG = "DEBUG";

Player::Player(CharacterClient *first, CharacterClient *second) {
    Logger *logger = Logger::getInstance();
    logger->log("Inicializacion de personajes para jugador.", DEBUG);
    currentCharacter = first;
    firstCharacter = first;
    secondCharacter = second;
}

void Player::update(character_updater_t *updater, bool *isSending, bool becomeActive, int clientNumber) {

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
            *isSending = !(*isSending);
        m.unlock();
        //changeCharacter();  //send change character
    } else if (updater->action == CHANGEME_ONEPLAYER) {
        cout << "Se detecta changeme_onePlayer" << endl;
        changeCharacter();
        setCharacterToChanging();
    } else if (updater->action == CHANGEME) {
        m.lock();
        if (becomeActive)
            *isSending = !(*isSending);
        m.unlock();
        changeCharacter();  //send change character
        setCharacterToChanging();
    }

    currentCharacter->update(updater);
    Logger *logger = Logger::getInstance();
    InputManager *inputManager = InputManager::getInstance();
    logger->log("Detecta boton para cambio de personaje en Player.", DEBUG);

}


void Player::render(SDL_Renderer *mRenderer, int camX, int camY, int posContrincante) {
    Logger *logger = Logger::getInstance();
    logger->log("Renderizado de personaje - Render.", DEBUG);
    currentCharacter->render(mRenderer, camX, camY, posContrincante);
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
    currentCharacter->load(pRenderer, posContrincante);
}

void Player::setCurrentCharacter(int i) {
    if(i == 0)
        currentCharacter = firstCharacter;
    else
        currentCharacter = secondCharacter;
}



