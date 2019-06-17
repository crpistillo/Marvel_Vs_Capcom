//
// Created by IgVelasco on 5/17/19.
//

#include "CharacterServer.h"
#include "../tools/logger/Logger.h"
#include "../Layer.h"

const string ERROR = "ERROR";
const string INFO = "INFO";
const string DEBUG = "DEBUG";


// Protected
CharacterServer::CharacterServer(int mPosX, int mPosY, int width, int sobrante, bool isLookingLeft, int widthSprite,
                                 int heightSprite, int anchoPantalla, int numberOfClient) {
    this->mPosX = mPosX;
    this->mPosY = mPosY;
    this->width = width;
    this->sobrante = sobrante;
    this->widthSprite = widthSprite;
    this->heightSprite = heightSprite;
    this->INITIAL_POS_Y = mPosY;
    this->mVelX = 0;
    this->mVelY = 0;
    this->anchoPantalla = anchoPantalla;
    this->currentWalkingSprite = 0;
    this->currentStandingSprite = 0;
    this->currentWalkbackSprite = 0;
    this->currentJumpingSprite = 0;
    this->currentJumpingRightSprite = 0;
    this->currentJumpingLeftSprite = 0;
    this->currentIntroSprite = 0;
    this->isLookingLeft = isLookingLeft;
    this->clientNumber = numberOfClient;
    currentAction = STANDING;
    this->currentClient = NULL;
    this->currentKickSprite = 0;
    this->currentPunchSprite = 0;
    this->currentKickDownSprite = 0;
    this->currentPunchDownSprite = 0;
    this->currentHurtingAirSprite = 0;
    this->currentHurtingSprite = 0;


    this->characterBox = new Box(mPosX, mPosY, widthSprite, heightSprite);


    this->lastTime = SDL_GetTicks();

}

// Public:
void CharacterServer::update(int distance, int posContrincante, actions_t actionRecieved, Box *boxContrincante) {

    bool actionStarted = false;
    bool inTheAir = false;

    if (currentAction == MI || currentAction == JV || currentAction == JR ||
        currentAction == JL || currentAction == P || currentAction == K ||
        currentAction == PD || currentAction == KD || currentAction == HG ||
        currentAction == HA)
        actionStarted = true;

    if (currentAction == JV || currentAction == JR ||
        currentAction == JL )
        inTheAir = true;


    if (currentAction == MAKINGINTRO) {
        makeIntro();
    }

    if (currentAction == PUNCH) {
        punch();
    }

    if (currentAction == KICK) {
        kick();
    }

    if (currentAction == PUNCHDOWN) {
        punchDown();
    }

    if (currentAction == KICKDOWN) {
        kickDown();
    }

    if (currentAction == JUMPINGVERTICAL ) {    //Si saltaba verticalmente, lo fuerzo a que siga con esa accion
        jumpVertical();
        if(actionRecieved == PUNCH)
            punchJumpVertical();
    }

    if (currentAction == JUMPINGRIGHT) {        //Si saltaba hacia la derecha, lo fuerzo a que siga con esa accion
        moveRight(distance, posContrincante);
        jumpRight();
    //    if(actionRecieved == PUNCH)
        //    punchJumpRight();
    }

    if (currentAction == JUMPINGLEFT) {
        moveLeft(distance, posContrincante);
        jumpLeft();
       // if(actionRecieved == PUNCH)
         //   punchJumpLeft();
    }

    if (currentAction == HURTINGGROUND) {
        hurtingGround();
    }

    if (currentAction == HURTINGAIR) {
        if (isLookingLeft)
            moveRight(distance, posContrincante);
        else
            moveLeft(distance, posContrincante);
        hurtingAir();
    }

    if(currentAction == PUNCHINGVERTICAL){
        punchJumpVertical();
    }



    if (actionStarted) {
        this->characterBox->setCenter(getCentro(), mPosY);
        return;
    }

    this->currentAction = actionRecieved;

    //Acciones de dos teclas primero
    if (currentAction == JUMPINGVERTICAL)
        jumpVertical(); //send jump rigth
    else if (currentAction == JUMPINGLEFT)
        jumpLeft(); //send jump left

        //Acciones de una sola tecla
    else if (currentAction == JUMPINGVERTICAL)
        jumpVertical(); //send jump vertical
    else if (currentAction == DUCK)
        renderDuckSprite();  // send duck
    else if (currentAction == BLOCK)
        renderBlockSprite();
    else if (currentAction == MOVINGRIGHT)
        moveRight(distance, posContrincante);   //send move right
    else if (currentAction == MOVINGLEFT)
        moveLeft(distance, posContrincante);    //send move left
    else if (currentAction == PUNCH)
        punch();
    else if (currentAction == KICK)
        kick();
    else if (currentAction == PUNCHDOWN)
        punchDown();
    else if (currentAction == KICKDOWN)
        kickDown();
    else if (currentAction == HURTINGGROUND)
        hurtingGround();


    if (currentAction == STANDING)
        this->stand(); //send stand

    this->characterBox->setCenter(getCentro(), mPosY);
    updateStand();
}


int CharacterServer::getPosX() {
    return mPosX;
}

int CharacterServer::getPosY() {
    return mPosY;
}

int CharacterServer::getWidth() {
    return width;
}

int CharacterServer::getSobrante() {
    return sobrante;
}

int CharacterServer::getCentro() {
    int centro;
    centro = this->getPosX() + this->getSobrante() + (this->getWidth()) / 2;
    return centro;
}

CharacterServer::~CharacterServer() {
}

void CharacterServer::positionUpdate(int *x) {
    /*x tiene el centro del personaje (ubicacion exacta del personaje)
     * La posicion en x se calcula con eso*/
    mPosX = *x - this->getSobrante() - (this->getWidth() / 2);
}

void CharacterServer::startIntro() {
    currentAction = MAKINGINTRO;
}

void CharacterServer::resetSpriteVariables() {
    mPosY = this->INITIAL_POS_Y;
    currentJumpingSprite = 0;
    currentWalkingSprite = 0;
    currentIntroSprite = 0;
    currentPunchSprite = 0;
    currentKickSprite = 0;
    currentPunchDownSprite = 0;
    currentKickDownSprite = 0;
}


void CharacterServer::stand() {
    currentAction = STANDING;
    this->resetSpriteVariables();
    if (currentStandingSprite >= lastStandingSprite)
        currentStandingSprite = 0;
}


void CharacterServer::renderDuckSprite() {
    currentAction = DUCK;
}

void CharacterServer::renderBlockSprite() {
    currentAction = BLOCK;
}

void CharacterServer::moveLeft(int distance, int posContrincante) {
    currentAction = MOVINGLEFT;
    mPosX -= CHARACTER_VEL;

    /*distance va de -800 a 800 (ancho de la pantalla)*/
    if ((mPosX - CHARACTER_VEL < -CharacterServer::getSobrante()) || (distance < (-anchoPantalla))) {
        //Move back
        mPosX += CHARACTER_VEL;
    }

    walkingSpriteUpdate();
}


void CharacterServer::moveRight(int distance, int posContrincante) {
    currentAction = MOVINGRIGHT;

    mPosX += CHARACTER_VEL;

    if ((mPosX + CHARACTER_VEL >= (LEVEL_WIDTH - CharacterServer::getSobrante() - CharacterServer::getWidth())) ||
        (distance > anchoPantalla)) {
        //Move back
        mPosX -= CHARACTER_VEL;
    }

    walkingSpriteUpdate();
}


void CharacterServer::jump(int *currentSprite, int lastSprite) {

    *currentSprite < 10 ? (mPosY -= 2.5 * CHARACTER_VEL) : (mPosY += 2.5 * CHARACTER_VEL);
    (*currentSprite)++;
    if (*currentSprite > lastSprite) {
        *currentSprite = 0;
        mPosY = this->INITIAL_POS_Y;
        this->currentAction = STANDING;
        currentStandingSprite = 0;
        resetSpriteVariables();
    }
}

void CharacterServer::jumpVertical() {
    this->currentAction = JUMPINGVERTICAL;
    jump(&currentJumpingSprite, lastJumpingSprite);
}

void CharacterServer::jumpRight() {
    this->currentAction = JUMPINGRIGHT;
    jump(&currentJumpingRightSprite, lastJumpingRightSprite);

}


void CharacterServer::jumpLeft() {
    this->currentAction = JUMPINGLEFT;
    jump(&currentJumpingLeftSprite, lastJumpingLeftSprite);
}

void CharacterServer::punch() {
    this->currentAction = PUNCH;
    currentPunchSprite++;
    if (currentPunchSprite > lastPunchSprite) {
        currentPunchSprite = 0;
        this->currentAction = STANDING;
        currentStandingSprite = 0;
    }
}

void CharacterServer::kick() {
    this->currentAction = KICK;
    currentKickSprite++;
    if (currentKickSprite > lastKickSprite) {
        currentKickSprite = 0;
        this->currentAction = STANDING;
        currentStandingSprite = 0;
    }
}

void CharacterServer::punchDown() {
    this->currentAction = PUNCHDOWN;
    currentPunchDownSprite++;
    if (currentPunchDownSprite > lastPunchDownSprite) {
        currentPunchDownSprite = 0;
        this->currentAction = DUCK;
    }
}

void CharacterServer::kickDown() {
    this->currentAction = KICKDOWN;
    currentKickDownSprite++;
    if (currentKickDownSprite > lastKickDownSprite) {
        currentKickDownSprite = 0;
        this->currentAction = DUCK;
    }
}


void CharacterServer::makeIntro() {
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


void CharacterServer::updateStand() {
    if (currentStandingSprite <= lastStandingSprite)
        currentStandingSprite++;
}

void CharacterServer::walkingSpriteUpdate() {
    ++currentWalkingSprite;

    if (currentWalkingSprite > lastWalkingSprite)
        currentWalkingSprite = 0;

}

bool CharacterServer::isStanding() {
    return this->currentAction == STANDING;
}

Box *CharacterServer::getColisionable() {
    return characterBox;
}

void CharacterServer::hurtingGround() {
    this->currentAction = HURTINGGROUND;
    currentHurtingSprite++;
    if (currentHurtingSprite > lastHurtingSprite) {
        currentHurtingSprite = 0;
        this->currentAction = STANDING;
        currentStandingSprite = 0;
    }
}

bool CharacterServer::inTheGround() {
    return !(currentAction == JUMPINGRIGHT || currentAction == JUMPINGLEFT || currentAction == JUMPINGLEFT);
}

void CharacterServer::hurtingAir() {
    currentHurtingAirSprite < 6 ? (mPosY -= 2.5 * CHARACTER_VEL) : (mPosY += 2.5 * CHARACTER_VEL);
    currentHurtingAirSprite++;
    if (currentHurtingAirSprite > lastHurtingAirSprite) {
        currentHurtingAirSprite = 0;
        mPosY = this->INITIAL_POS_Y;
        this->currentAction = STANDING;
        currentStandingSprite = 0;
    }

}

void CharacterServer::punchJumpVertical() {
    currentAction = PUNCHINGVERTICAL;
    jump(&currentJumpingSprite, lastJumpingSprite);

    currentPunchSprite++;
    if (currentPunchSprite > lastPunchSprite) {
        currentPunchSprite = 0;
        this->currentAction = STANDING; //falling
        currentStandingSprite = 0;
    }

}


