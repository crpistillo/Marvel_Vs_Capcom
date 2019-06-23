//
// Created by IgVelasco on 5/17/19.
//

#include "CharacterServer.h"
#include "../tools/logger/Logger.h"
#include "../Layer.h"

const string ERROR = "ERROR";
const string INFO = "INFO";
const string DEBUG = "DEBUG";

bool initialLookingLeft;

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
    this->currentThrowPowerSprite = 0;
    this->currentGripSprite = 0;
    this->currentFallingSprite = 0;
    this->vida = 100;

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

    if (this->getCentro() > posContrincante)
        isLookingLeft = true;
    else
        isLookingLeft = false;


    bool actionStarted = false;

    if (currentAction == MI || currentAction == JV || currentAction == JR ||
            currentAction == JL || currentAction == P || currentAction == K ||
            currentAction == PD || currentAction == KD || currentAction == HG ||
            currentAction == HA || currentAction == PV || currentAction == THP ||
            currentAction == PJR || currentAction == PJL || currentAction == KV ||
            currentAction == KJR || currentAction == KJL || currentAction == THP ||
            currentAction == FA || currentAction == TH ||currentAction == PS ||
            currentAction == PSD || currentAction == PSV || currentAction == PSJR ||
            currentAction == PSJL || currentAction == KS || currentAction == KSD ||
            currentAction == KSV || currentAction == KSJR || currentAction == KSJL)
        actionStarted = true;


    if (currentAction == MAKINGINTRO) {
        makeIntro();
    } else if (currentAction == HURTINGGROUND || actionRecieved == HURTINGGROUND) {
        hurtingGround();
    } else if (currentAction == HURTINGAIR || actionRecieved == HURTINGAIR) {
        if (!isLookingLeft)
            moveLeft(distance, 1);
        else
            moveRight(distance, 1);
        hurtingAir();
    }
    else if (currentAction == JUMPINGVERTICAL || currentAction == PUNCHINGVERTICAL ||
               currentAction == KICKINGVERTICAL || currentAction == PUNCHINGSTRONGVERTICAL
			   || currentAction == KICKINGSTRONGVERTICAL) {
        if (actionRecieved == PUNCH || currentAction == PUNCHINGVERTICAL || actionRecieved == PUNCHINGVERTICAL) {
            //jumpVertical();
            punchJumpVertical(PUNCHINGVERTICAL);
        }
		else if (actionRecieved == PUNCHSTRONG || currentAction == PUNCHINGSTRONGVERTICAL
				|| actionRecieved == PUNCHINGSTRONGVERTICAL) {
			punchJumpVertical(PUNCHINGSTRONGVERTICAL);
		}
        else if (actionRecieved == KICK || currentAction == KICKINGVERTICAL || actionRecieved == KICKINGVERTICAL) {kickJumpVertical(KICKINGVERTICAL);
        }
            else if (actionRecieved == KICKSTRONG || currentAction == KICKINGSTRONGVERTICAL || actionRecieved == KICKINGSTRONGVERTICAL) {
                kickJumpVertical(KICKINGSTRONGVERTICAL);
        } else if (currentAction == JUMPINGVERTICAL)
            jumpVertical();


    } else if (currentAction == JUMPINGRIGHT || currentAction == PUNCHINGJUMPRIGHT ||
               currentAction == KICKINGJUMPRIGHT || currentAction == PUNCHINGSTRONGJUMPRIGHT
			   || currentAction == KICKINGSTRONGJUMPRIGHT) {

        if (actionRecieved == PUNCH || currentAction == PUNCHINGJUMPRIGHT || actionRecieved == PUNCHINGJUMPRIGHT) {
            moveRight(distance, 1);
            jumpRight();
            punchJumpRight(PUNCHINGJUMPRIGHT);
        } else if (actionRecieved == PUNCHSTRONG || currentAction == PUNCHINGSTRONGJUMPRIGHT || actionRecieved == PUNCHINGSTRONGJUMPRIGHT) {
            moveRight(distance, 1);
            jumpRight();
            punchJumpRight(PUNCHINGSTRONGJUMPRIGHT);
        } else if (actionRecieved == KICK || currentAction == KICKINGJUMPRIGHT || actionRecieved == KICKINGJUMPRIGHT) {
            moveRight(distance, 1);
            kickJumpRight(KICKINGJUMPRIGHT);
        } else if (actionRecieved == KICKSTRONG || currentAction == KICKINGSTRONGJUMPRIGHT || actionRecieved == KICKINGSTRONGJUMPRIGHT) {
            moveRight(distance, 1);
            kickJumpRight(KICKINGSTRONGJUMPRIGHT);
        } else {
            moveRight(distance, 1);
            jumpRight();
        }


    } else if (currentAction == JUMPINGLEFT || currentAction == PUNCHINGJUMPLEFT || currentAction == KICKINGJUMPLEFT
    		|| currentAction == PUNCHINGSTRONGJUMPLEFT || currentAction == KICKINGSTRONGJUMPLEFT) {
        if (actionRecieved == PUNCH || currentAction == PUNCHINGJUMPLEFT || actionRecieved == PUNCHINGJUMPLEFT) {
            moveLeft(distance, 1);
            jumpLeft();
            punchJumpLeft(PUNCHINGJUMPLEFT);
        } else if (actionRecieved == PUNCHSTRONG || currentAction == PUNCHINGSTRONGJUMPLEFT || actionRecieved == PUNCHINGSTRONGJUMPLEFT) {
            moveLeft(distance, 1);
            jumpLeft();
            punchJumpLeft(PUNCHINGSTRONGJUMPLEFT);
        } else if (actionRecieved == KICK || currentAction == KICKINGJUMPLEFT || actionRecieved == KICKINGJUMPLEFT) {
            moveLeft(distance, 1);
            jumpLeft();
            kickJumpLeft(KICKINGJUMPLEFT);
        } else if (actionRecieved == KICKSTRONG || currentAction == KICKINGSTRONGJUMPLEFT || actionRecieved == KICKINGSTRONGJUMPLEFT) {
            moveLeft(distance, 1);
            jumpLeft();
            kickJumpLeft(KICKINGSTRONGJUMPLEFT);
        } else {
            moveLeft(distance, 1);
            jumpLeft();
        }

    } else if (currentAction == PUNCH) {
        punch(PUNCH);
    } else if (currentAction == PUNCHSTRONG) {
        punch(PUNCHSTRONG);
    } else if (currentAction == KICK) {
        kick(KICK);
    } else if (currentAction == KICKSTRONG) {
        kick(KICKSTRONG);
    } else if (currentAction == PUNCHDOWN) {
        punchDown(PUNCHDOWN);
    } else if (currentAction == PUNCHSTRONGDOWN) {
        punchDown(PUNCHSTRONGDOWN);
    } else if (currentAction == KICKDOWN) {
        kickDown(KICKDOWN);
    } else if (currentAction == KICKSTRONGDOWN) {
        kickDown(KICKSTRONGDOWN);
    } else if (currentAction == THROWPOWER) {
        throwPower();
    } else if (currentAction == GRIP) {
        grip();
    } else if (currentAction == THROW) {
        throwCharacter();
    } else if (currentAction == FALLING) {
        falling(distance, posContrincante);
    }

    if (actionStarted) {
        this->characterBox->setCenter(getCentro(), mPosY);
        return;
    }

    this->currentAction = actionRecieved;

    //Acciones de dos teclas primero
    if (currentAction == JUMPINGRIGHT)
        jumpRight(); //send jump rigth
        //moveRight(distance, posContrincante, 1);
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
        moveRight(distance, 1);   //send move right
    else if (currentAction == MOVINGLEFT)
        moveLeft(distance, 1);    //send move left
    else if (currentAction == PUNCH)
        punch(PUNCH);
    else if (currentAction == PUNCHSTRONG)
        punch(PUNCHSTRONG);
    else if (currentAction == KICK)
        kick(KICK);
    else if (currentAction == KICKSTRONG)
        kick(KICKSTRONG);
    else if (currentAction == PUNCHDOWN)
        punchDown(PUNCHDOWN);
    else if (currentAction == PUNCHSTRONGDOWN)
        punchDown(PUNCHSTRONGDOWN);
    else if (currentAction == KICKDOWN)
        kickDown(KICKDOWN);
    else if (currentAction == KICKSTRONGDOWN)
        kickDown(KICKSTRONGDOWN);
    else if (currentAction == THROWPOWER)
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
    currentThrowPowerSprite = 0;
    currentFallingSprite = 0;
    currentThrowSprite = 0;
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

void CharacterServer::moveLeft(int distance, int vel) {
    currentAction = MOVINGLEFT;
    mPosX -= vel * CHARACTER_VEL;

    /*distance va de -800 a 800 (ancho de la pantalla)*/
    if ((mPosX - CHARACTER_VEL < -CharacterServer::getSobrante()) || (distance < (-anchoPantalla))) {
        //Move back
        mPosX += CHARACTER_VEL;
    }

    walkingSpriteUpdate();
}


void CharacterServer::moveRight(int distance, int vel) {
    currentAction = MOVINGRIGHT;

    mPosX += vel * CHARACTER_VEL;

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

void CharacterServer::punch(actions_t punch) {
	characterBox->updateBox(widthPunch, heightPunch);
    this->currentAction = punch;
    normalAction(&currentPunchSprite, &lastPunchSprite, STANDING);
}

void CharacterServer::kick(actions_t kick) {
	characterBox->updateBox(widthKick, heightKick);
    this->currentAction = kick;
    normalAction(&currentKickSprite, &lastKickSprite, STANDING);
}

void CharacterServer::punchDown(actions_t punchDown) {
	characterBox->updateBox(widthPunchDown, heightPunchDown);
    this->currentAction = punchDown;
    normalAction(&currentPunchDownSprite, &lastPunchDownSprite, DUCK);
}

void CharacterServer::kickDown(actions_t kickDown) {
	characterBox->updateBox(widthKickDown, heightKickDown);
    this->currentAction = kickDown;
    normalAction(&currentKickDownSprite, &lastKickDownSprite, DUCK);
}


//Tengo que ver si es normal
void CharacterServer::throwPower() {
    this->currentAction = THROWPOWER;
    normalAction(&currentThrowPowerSprite,&lastThrowPowerSprite,STANDING);
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
}

bool CharacterServer::inTheGround() {
    return !(currentAction == JUMPINGRIGHT || currentAction == JUMPINGLEFT || currentAction == JUMPINGVERTICAL ||
             currentAction == PUNCHINGVERTICAL || currentAction == PUNCHINGJUMPRIGHT ||
             currentAction == PUNCHINGJUMPLEFT ||
             currentAction == KICKINGVERTICAL || currentAction == KICKINGJUMPRIGHT || currentAction == KICKINGJUMPLEFT
             || currentAction == HURTINGAIR);
}

//Not working
void CharacterServer::hurtingAir() {
    currentAction = HURTINGAIR;
    currentHurtingAirSprite < 10 ? (mPosY -= 2.5 * CHARACTER_VEL) : (mPosY += 2.5 * CHARACTER_VEL);
    if(mPosY > INITIAL_POS_Y){
        mPosY -= 3 * CHARACTER_VEL;
    }
    currentHurtingAirSprite++;
    if (currentHurtingAirSprite > lastHurtingAirSprite) {
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
    cout<<"Punch: "<<currentPunchSprite<<endl;

}

void CharacterServer::airActions(int *currentSprite, int lastSprite, actions_t nextAction, int airSprite,
                                 int lastAirSprite) {
    (*currentSprite)++;
    if (*currentSprite > lastSprite || 2 + airSprite + *currentSprite > lastAirSprite) { //not sure
        *currentSprite = 0;
        this->currentAction = nextAction; //falling
    }
    if (airSprite > lastAirSprite || airSprite == 0) {
        resetSpriteVariables();
        currentAction = STANDING;
    }

}

void CharacterServer::punchJumpVertical(actions_t punchVertical) {
	characterBox->updateBox(widthPunchAir, heightPunchAir);
    currentAction = punchVertical;
    jump(&currentJumpingSprite, lastJumpingSprite);
    airActions(&currentPunchAirSprite, lastPunchAirSprite, JUMPINGVERTICAL, currentJumpingSprite, lastJumpingSprite);
}

void CharacterServer::punchJumpLeft(actions_t punchJumpLeft) {
	characterBox->updateBox(widthPunchAir, heightPunchAir);
    currentAction = punchJumpLeft;
    airActions(&currentPunchAirSprite, lastPunchAirSprite, JUMPINGLEFT, currentJumpingLeftSprite,
               lastJumpingLeftSprite);
}

void CharacterServer::punchJumpRight(actions_t punchJumpRight) {
	characterBox->updateBox(widthPunchAir, heightPunchAir);
    currentAction = punchJumpRight;
    airActions(&currentPunchAirSprite, lastPunchAirSprite, JUMPINGRIGHT, currentJumpingRightSprite,
               lastJumpingRightSprite);
}

void CharacterServer::kickJumpVertical(actions_t kickVertical) {
	characterBox->updateBox(widthKickAir, heightKickAir);
    currentAction = kickVertical;
    jump(&currentJumpingSprite, lastJumpingSprite);
    airActions(&currentKickAirSprite, lastKickAirSprite, JUMPINGVERTICAL, currentJumpingSprite, lastJumpingSprite);
}

void CharacterServer::kickJumpRight(actions_t kickJumpRight) {
	characterBox->updateBox(widthKickAir, heightKickAir);
    currentAction = kickJumpRight;
    jump(&currentJumpingRightSprite, lastJumpingRightSprite);
    airActions(&currentKickAirSprite, lastKickAirSprite, JUMPINGRIGHT, currentJumpingRightSprite,
               lastJumpingRightSprite);
}

void CharacterServer::kickJumpLeft(actions_t kickJumpLeft) {
	characterBox->updateBox(widthKickAir, heightKickAir);
    currentAction = kickJumpLeft;
    airActions(&currentKickAirSprite, lastKickAirSprite, JUMPINGLEFT, currentJumpingLeftSprite, lastJumpingLeftSprite);
}


//default is false
bool CharacterServer::isProjectileActive() {
    return false;
}

Projectile *CharacterServer::getProjectile() {
    return nullptr;
}

void CharacterServer::throwCharacter() {
    this->currentAction = THROW;
    normalAction(&currentThrowSprite, &lastThrowSprite, STANDING);
    currentThrowSprite++;
}

void CharacterServer::grip() {
    this->currentAction = GRIP;
    normalAction(&currentGripSprite, &lastGripSprite, STANDING);
}

void CharacterServer::falling(int distance, int posContrincante) {
    //guardo el isLookingLeft inicial en el primer sprite
    m.lock();
    if (currentFallingSprite == 0) {
        if (isLookingLeft)
            initialLookingLeft = true;
        else
            initialLookingLeft = false;
    }
    isLookingLeft = initialLookingLeft;
    m.unlock();


    if (currentFallingSprite >= 0 && currentFallingSprite < 8) {
        if (isLookingLeft) {
            mPosX = posContrincante - sobrante - 150;
        } else
            mPosX = posContrincante - sobrante - 90;
    } else if (currentFallingSprite == 8) {
        if (isLookingLeft)
            mPosX = posContrincante - sobrante;

    }

    if (currentFallingSprite > 8 && currentFallingSprite < 36) {
        if (isLookingLeft) {
            moveRight(distance, 2);
        } else
            moveLeft(distance, 2);
    }

    if (currentFallingSprite > 8 && currentFallingSprite < 17) {
        mPosY -= 4 * CHARACTER_VEL;
    } else if (currentFallingSprite > 17 && currentFallingSprite < 36) {
        mPosY += 1.7 * CHARACTER_VEL;
    }

    if (mPosX <= -418)
        mPosX = -418;
    else if (mPosX >= 2552)
        mPosX = 2552;

    this->currentAction = FALLING;
    currentFallingSprite++;
    if (currentFallingSprite > lastFallingSprite) {
        currentFallingSprite = 0;
        this->currentAction = STANDING;
        currentStandingSprite = 0;
        mPosY = this->INITIAL_POS_Y;
    }

}

bool CharacterServer::isProjectileHurting() {
    return false;
}

bool CharacterServer::isHurting() {
    return currentAction == HURTINGAIR || currentAction == HURTINGGROUND;
}

void CharacterServer::quitarVida(float danio) {
    if ((vida-danio)<0) vida = 0;
    else vida = vida - danio;
}

float CharacterServer::getVida() {
    return vida;
}



