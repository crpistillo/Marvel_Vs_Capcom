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
    soundOn = false;
}

void Player::update(character_updater_t *updater, bool *isSending, bool becomeActive, int clientNumber, Mix_Music **gMusic ) {

    if (updater->action == RECONNECT) {
        //SI ES DE MI EQUIPO ME TENGO QUE FIJAR SI TENGO QUE ESTAR ACTIVO
        //SI EL CURRENTCHARACTER -> CLIENTE
    	cout<<"recibe RECONNECT"<<endl;
    	cout<<"becomeActive es "<<becomeActive<<endl;
        if (becomeActive) {
            m.lock();
            cout<<"is Sending del Player en RECONNECT -ANTES es"<<*isSending<<endl;
            *isSending = (clientNumber == currentCharacter->clientNumber);
            cout<<"isSending del Player en RECONNECT - DESPUES es: "<<*isSending<<endl;
            m.unlock();
        }

        return;
    }


    if (updater->action == DISCONNECTEDCLIENT) {
        m.lock();
        if (becomeActive)
        {
        	 cout<<"is Sending del Player en DISCONNECT -ANTES es"<<*isSending<<endl;
        	 *isSending = true;
        }

        m.unlock();
        //changeCharacter();  //send change character
    } else if (updater->action == CHANGEME_ONEPLAYER) {
        cout << "Se detecta changeme_onePlayer" << endl;
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
	} else if (updater->action == PLAYMUSIC) {
		m.lock();

		if (!soundOn) {
			if (Mix_PlayingMusic() == 0 && !soundOn) {
				//Play the music
				Mix_PlayMusic(*gMusic, -1);
			}
			//If music is being played
			else {
				//If the music is paused
				if (Mix_PausedMusic() == 1 && soundOn) {
					//Resume the music
					Mix_ResumeMusic();
				}
				//If the music is playing
				else {
					//Pause the music
					Mix_PauseMusic();
				}
			}
			soundOn = true;
		}
		else
		{
			Mix_HaltMusic();
			soundOn = false;
		}
    	m.unlock();
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



