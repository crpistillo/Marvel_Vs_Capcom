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
                                 int heightSprite, int anchoPantalla, int numberOfClient,
								 int widthStanding,
								 int heightStanding,
								 int widthWalking,
								 int heightWalking,
								 int widthDuck,
								 int heightDuck,
								 int widthPunch,
								 int heightPunch,
								 int widthPunchDown,
								 int heightPunchDown,
								 int widthKick,
								 int heightKick,
								 int widthKickDown,
								 int heightKickDown,
								 int widthKickAir,
								 int heightKickAir,
								 int widthPunchAir,
								 int heightPunchAir,
								 int widthJumping,
								 int heightJumping,
								 int widthJumpingLeft ,
								 int heightJumpingLeft) {
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
    this->currentKickAirSprite = 0;
    this->currentPunchAirSprite = 0;

    this->widthStanding = widthStanding;
    this->heightStanding = heightStanding;
    this->widthWalking = widthWalking;
    this->heightWalking = heightWalking;
    this->widthDuck = widthDuck;
    this->heightDuck = heightDuck;
    this->widthPunch = widthPunch;
    this->heightPunch = heightPunch;
    this->widthPunchDown = widthPunchDown;
    this->heightPunchDown = heightPunchDown;
    this->widthKick = widthKick;
    this->heightKick = heightKick;
    this->widthKickDown = widthKickDown;
    this->heightKickDown = heightKickDown;
    this->widthKickAir = widthKickAir;
	this->heightKickAir = heightKickAir;
	this->widthPunchAir = widthPunchAir;
	this->heightPunchAir = heightPunchAir;
	this->widthJumping = widthJumping;
	this->heightJumping = heightJumping;
	this->widthJumpingLeft = widthJumpingLeft;
	this->heightJumpingLeft = heightJumpingLeft;


    this->characterBox = new Box(mPosX, mPosY, widthSprite, heightSprite);


    this->lastTime = SDL_GetTicks();

}

// Public:
void CharacterServer::update(int distance, int posContrincante, actions_t actionRecieved, Box *boxContrincante) {
    if(this->getCentro() > posContrincante)
        isLookingLeft = true;
    else
        isLookingLeft = false;


    bool actionStarted = false;

    if (currentAction == MI || currentAction == JV || currentAction == JR ||
        currentAction == JL || currentAction == P || currentAction == K ||
        currentAction == PD || currentAction == KD || currentAction == HG ||
        currentAction == HA || currentAction == PV || currentAction == TH ||
        currentAction == PJR || currentAction == PJL || currentAction == KV
        || currentAction == KJR || currentAction == KJL)
        actionStarted = true;


    if (currentAction == MAKINGINTRO) {
        makeIntro();
    }    else if (currentAction == HURTINGGROUND || actionRecieved == HURTINGGROUND) {
        hurtingGround();
    }

    else if (currentAction == HURTINGAIR || actionRecieved == HURTINGAIR) {
        if (!isLookingLeft)
            moveLeft(distance, posContrincante);
        else
            moveRight(distance, posContrincante);
        hurtingAir();
    }

    else if (currentAction == JUMPINGVERTICAL || currentAction == PUNCHINGVERTICAL ||
               currentAction == KICKINGVERTICAL) {
        if (actionRecieved == PUNCH || currentAction == PUNCHINGVERTICAL || actionRecieved == PUNCHINGVERTICAL){
            jumpVertical();
            punchJumpVertical();
        }
        else if (actionRecieved == KICK || currentAction == KICKINGVERTICAL || actionRecieved == KICKINGVERTICAL){
            jumpVertical();
            kickJumpVertical();
        } else
            jumpVertical();



    }
    else if (currentAction == JUMPINGRIGHT || currentAction == PUNCHINGJUMPRIGHT ||
               currentAction == KICKINGJUMPRIGHT) {

        if (actionRecieved == PUNCH || currentAction == PUNCHINGJUMPRIGHT || actionRecieved == PUNCHINGJUMPRIGHT){
            moveRight(distance, posContrincante);
            jumpRight();
            punchJumpRight();
        }
        else if (actionRecieved == KICK || currentAction == KICKINGJUMPRIGHT || actionRecieved == KICKINGJUMPRIGHT){
            moveRight(distance, posContrincante);
            jumpRight();
            kickJumpRight();
        } else   {
            moveRight(distance, posContrincante);
            jumpRight();
        }



    }

    else if (currentAction == JUMPINGLEFT || currentAction == PUNCHINGJUMPLEFT || currentAction == KICKINGJUMPLEFT) {
        if (actionRecieved == PUNCH || currentAction == PUNCHINGJUMPLEFT || actionRecieved == PUNCHINGJUMPLEFT){
            moveLeft(distance, posContrincante);
            jumpLeft();
            punchJumpLeft();
        }
        else if (actionRecieved == KICK || currentAction == KICKINGJUMPLEFT || actionRecieved == KICKINGJUMPLEFT){
            moveLeft(distance, posContrincante);
            jumpLeft();
            kickJumpLeft();
        }
        else{
            moveLeft(distance, posContrincante);
            jumpLeft();
        }

    }
    else if (currentAction == PUNCH) {
        punch();
    } else if (currentAction == KICK) {
        kick();
    } else if (currentAction == PUNCHDOWN) {
        punchDown();
    } else if (currentAction == KICKDOWN) {
        kickDown();
    } else if (currentAction == THROW) {
        throwPower();
    }

    if (actionStarted) {
        this->characterBox->setCenter(getCentro(), mPosY);
        return;
    }

    this->currentAction = actionRecieved;

    //Acciones de dos teclas primero
    if (currentAction == JUMPINGRIGHT)
        jumpRight(); //send jump rigth
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
    else if (currentAction == THROW)
        throwPower();
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
    currentJumpingLeftSprite = 0;
    currentJumpingRightSprite = 0;
    currentWalkingSprite = 0;
    currentIntroSprite = 0;
    currentPunchSprite = 0;
    currentKickSprite = 0;
    currentPunchDownSprite = 0;
    currentKickDownSprite = 0;
    currentPunchAirSprite = 0;
    currentKickAirSprite = 0;
}


void CharacterServer::stand() {
    currentAction = STANDING;
    this->resetSpriteVariables();
    if (currentStandingSprite >= lastStandingSprite)
        currentStandingSprite = 0;
    characterBox->updateBox(widthStanding, heightStanding);
}


void CharacterServer::renderDuckSprite() {
	characterBox->updateBox(widthDuck, heightDuck);
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
	characterBox->updateBox(widthJumping, heightJumping);
    this->currentAction = JUMPINGVERTICAL;
    jump(&currentJumpingSprite, lastJumpingSprite);
}

void CharacterServer::jumpRight() {
	characterBox->updateBox(widthJumpingLeft, heightJumpingLeft);
    this->currentAction = JUMPINGRIGHT;
    jump(&currentJumpingRightSprite, lastJumpingRightSprite);

}


void CharacterServer::jumpLeft() {
	characterBox->updateBox(widthJumpingLeft, heightJumpingLeft);
    this->currentAction = JUMPINGLEFT;
    jump(&currentJumpingLeftSprite, lastJumpingLeftSprite);
}

void CharacterServer::punch() {
	characterBox->updateBox(widthPunch, heightPunch);
    this->currentAction = PUNCH;
    normalAction(&currentPunchSprite, &lastPunchSprite, STANDING);
}

void CharacterServer::kick() {
	characterBox->updateBox(widthKick, heightKick);
    this->currentAction = KICK;
    normalAction(&currentKickSprite, &lastKickSprite, STANDING);
}

void CharacterServer::punchDown() {
	characterBox->updateBox(widthPunchDown, heightPunchDown);
    this->currentAction = PUNCHDOWN;
    normalAction(&currentPunchDownSprite, &lastPunchDownSprite, DUCK);
}

void CharacterServer::kickDown() {
	characterBox->updateBox(widthKickDown, heightKickDown);
    this->currentAction = KICKDOWN;
    normalAction(&currentKickDownSprite, &lastKickDownSprite, DUCK);
}


//Tengo que ver si es normal
void CharacterServer::throwPower() {
    this->currentAction = THROW;
    currentThrowSprite++;
    if (currentThrowSprite > lastThrowSprite) {
        currentThrowSprite = 0;
        this->currentAction = STANDING;
        currentStandingSprite = 0;
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
    normalAction(&currentHurtingSprite, &lastHurtingSprite, STANDING);
    cout<<currentHurtingSprite<< endl;
}

bool CharacterServer::inTheGround() {
    return !(currentAction == JUMPINGRIGHT || currentAction == JUMPINGLEFT || currentAction == JUMPINGVERTICAL ||
                currentAction == PUNCHINGVERTICAL || currentAction == PUNCHINGJUMPRIGHT || currentAction == PUNCHINGJUMPLEFT ||
                 currentAction == KICKINGVERTICAL || currentAction == KICKINGJUMPRIGHT || currentAction == KICKINGJUMPLEFT );
}

//Not working
void CharacterServer::hurtingAir() {
    currentAction = HURTINGAIR;
    currentHurtingAirSprite < 6 ? (mPosY -= 2.5 * CHARACTER_VEL) : (mPosY += 2.5 * CHARACTER_VEL);
    currentHurtingAirSprite++;
    cout << currentHurtingAirSprite << endl;
    if (currentHurtingAirSprite > lastHurtingAirSprite) {
        cout << currentHurtingAirSprite << endl;
        currentHurtingAirSprite = 0;
        this->currentAction = STANDING;
        currentStandingSprite = 0;
        resetSpriteVariables();

        mPosY = this->INITIAL_POS_Y;
    }
}


void CharacterServer::normalAction(int *currentSprite, int *lastSprite, actions_t finalAction) {
    (*currentSprite)++;
    if (*currentSprite > *lastSprite) {
        *currentSprite = 0;
        this->currentAction = finalAction;
        currentStandingSprite = 0;
    }

}

void CharacterServer::airActions(int *currentSprite, int lastSprite, actions_t nextAction, int airSprite,
                                 int lastAirSprite) {
    (*currentSprite)++;
    if (*currentSprite  > lastSprite || 2 + airSprite + *currentSprite > lastAirSprite) { //not sure
        *currentSprite = 0;
        this->currentAction = nextAction; //falling
    }
    if (airSprite > lastAirSprite || airSprite == 0) {
        resetSpriteVariables();
        cout<<"ENDS"<<endl;
        currentAction = STANDING;
    }

}

void CharacterServer::punchJumpVertical() {
	characterBox->updateBox(widthPunchAir, heightPunchAir);
    currentAction = PUNCHINGVERTICAL;
    airActions(&currentPunchAirSprite, lastPunchAirSprite, JUMPINGVERTICAL, currentJumpingSprite, lastJumpingSprite);
    cout << currentPunchAirSprite << endl;
}

void CharacterServer::punchJumpLeft() {
	characterBox->updateBox(widthPunchAir, heightPunchAir);
    currentAction = PUNCHINGJUMPLEFT;
    airActions(&currentPunchAirSprite, lastPunchAirSprite, JUMPINGLEFT, currentJumpingLeftSprite, lastJumpingLeftSprite);
}

void CharacterServer::punchJumpRight() {
	characterBox->updateBox(widthPunchAir, heightPunchAir);
    currentAction = PUNCHINGJUMPRIGHT;
    airActions(&currentPunchAirSprite, lastPunchAirSprite, JUMPINGRIGHT, currentJumpingRightSprite, lastJumpingRightSprite);
}

void CharacterServer::kickJumpVertical() {
	characterBox->updateBox(widthKickAir, heightKickAir);
    currentAction = KICKINGVERTICAL;
    airActions(&currentKickAirSprite, lastKickAirSprite, JUMPINGVERTICAL, currentJumpingSprite, lastJumpingSprite);
}

void CharacterServer::kickJumpRight() {
	characterBox->updateBox(widthKickAir, heightKickAir);
    currentAction = KICKINGJUMPRIGHT;
    airActions(&currentKickAirSprite, lastKickAirSprite, JUMPINGRIGHT, currentJumpingRightSprite, lastJumpingRightSprite);
}

void CharacterServer::kickJumpLeft() {
	characterBox->updateBox(widthKickAir, heightKickAir);
    currentAction = KICKINGJUMPLEFT;
    airActions(&currentKickAirSprite, lastKickAirSprite, JUMPINGLEFT, currentJumpingLeftSprite, lastJumpingLeftSprite);
}





