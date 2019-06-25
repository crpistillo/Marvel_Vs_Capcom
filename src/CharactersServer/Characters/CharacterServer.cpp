//
// Created by IgVelasco on 5/17/19.
//

#include "CharacterServer.h"
#include "../../tools/logger/Logger.h"
#include "../../Layer.h"

const string ERROR = "ERROR";
const string INFO = "INFO";
const string DEBUG = "DEBUG";

bool initialLookingLeft;

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
    this->currentKickAirSprite = 0;
    this->currentPunchAirSprite = 0;
    this->currentThrowPowerSprite = 0;
    this->currentGripSprite = 0;
    this->currentFallingSprite = 0;
    this->life = 100;


    this->characterBox = new Box(mPosX, mPosY, widthSprite, heightSprite);


    this->lastTime = SDL_GetTicks();

}

// Public:
void CharacterServer::update(int distance, int posEnemy, actions_t actionRecieved, Box *boxEnemy) {

    if (this->getCentro() > posEnemy)
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
            moveLeft(distance, 1, boxEnemy, false, MOVINGLEFT);
        else
            moveRight(distance, 1, boxEnemy, false, MOVINGRIGHT);
        hurtingAir();
    }
    else if (currentAction == JUMPINGVERTICAL || currentAction == PUNCHINGVERTICAL ||
               currentAction == KICKINGVERTICAL || currentAction == PUNCHINGSTRONGVERTICAL
			   || currentAction == KICKINGSTRONGVERTICAL) {
        if (actionRecieved == PUNCH || currentAction == PUNCHINGVERTICAL || actionRecieved == PUNCHINGVERTICAL) {
            punchJumpVertical(PUNCHINGVERTICAL, boxEnemy);
        }
		else if (actionRecieved == PUNCHSTRONG || currentAction == PUNCHINGSTRONGVERTICAL
				|| actionRecieved == PUNCHINGSTRONGVERTICAL) {
            punchJumpVertical(PUNCHINGSTRONGVERTICAL, boxEnemy);
		}
        else if (actionRecieved == KICK || currentAction == KICKINGVERTICAL || actionRecieved == KICKINGVERTICAL) {
            kickJumpVertical(
                    KICKINGVERTICAL, boxEnemy);
        }
            else if (actionRecieved == KICKSTRONG || currentAction == KICKINGSTRONGVERTICAL || actionRecieved == KICKINGSTRONGVERTICAL) {
            kickJumpVertical(KICKINGSTRONGVERTICAL, boxEnemy);
        } else if (currentAction == JUMPINGVERTICAL)
            jumpVertical(boxEnemy);


    } else if (currentAction == JUMPINGRIGHT || currentAction == PUNCHINGJUMPRIGHT ||
               currentAction == KICKINGJUMPRIGHT || currentAction == PUNCHINGSTRONGJUMPRIGHT
			   || currentAction == KICKINGSTRONGJUMPRIGHT) {

        if (actionRecieved == PUNCH || currentAction == PUNCHINGJUMPRIGHT || actionRecieved == PUNCHINGJUMPRIGHT) {
            punchJumpRight(PUNCHINGJUMPRIGHT, distance, boxEnemy);
        } else if (actionRecieved == PUNCHSTRONG || currentAction == PUNCHINGSTRONGJUMPRIGHT || actionRecieved == PUNCHINGSTRONGJUMPRIGHT) {
            punchJumpRight(PUNCHINGSTRONGJUMPRIGHT, distance, boxEnemy);
        } else if (actionRecieved == KICK || currentAction == KICKINGJUMPRIGHT || actionRecieved == KICKINGJUMPRIGHT) {
            kickJumpRight(KICKINGJUMPRIGHT, distance, boxEnemy);
        } else if (actionRecieved == KICKSTRONG || currentAction == KICKINGSTRONGJUMPRIGHT || actionRecieved == KICKINGSTRONGJUMPRIGHT) {
            kickJumpRight(KICKINGSTRONGJUMPRIGHT, distance, boxEnemy);
        } else {
            jumpRight(distance, boxEnemy);
        }


    } else if (currentAction == JUMPINGLEFT || currentAction == PUNCHINGJUMPLEFT || currentAction == KICKINGJUMPLEFT
    		|| currentAction == PUNCHINGSTRONGJUMPLEFT || currentAction == KICKINGSTRONGJUMPLEFT) {
        if (actionRecieved == PUNCH || currentAction == PUNCHINGJUMPLEFT || actionRecieved == PUNCHINGJUMPLEFT) {
            punchJumpLeft(PUNCHINGJUMPLEFT, distance, boxEnemy);
        } else if (actionRecieved == PUNCHSTRONG || currentAction == PUNCHINGSTRONGJUMPLEFT || actionRecieved == PUNCHINGSTRONGJUMPLEFT) {
            punchJumpLeft(PUNCHINGSTRONGJUMPLEFT, distance, boxEnemy);
        } else if (actionRecieved == KICK || currentAction == KICKINGJUMPLEFT || actionRecieved == KICKINGJUMPLEFT) {
            kickJumpLeft(KICKINGJUMPLEFT, distance, boxEnemy);
        } else if (actionRecieved == KICKSTRONG || currentAction == KICKINGSTRONGJUMPLEFT || actionRecieved == KICKINGSTRONGJUMPLEFT) {
            kickJumpLeft(KICKINGSTRONGJUMPLEFT, distance, boxEnemy);
        } else {
            jumpLeft(distance, boxEnemy);
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
        falling(distance, posEnemy, boxEnemy);
    }

    if (actionStarted) {
        this->characterBox->setCenter(getCentro(), mPosY);
        return;
    }

    this->currentAction = actionRecieved;

    //Acciones de dos teclas primero
    if (currentAction == JUMPINGRIGHT)
        jumpRight(0, boxEnemy); //send jump rigth
        //moveRight(distance, posContrincante, 1);
    else if (currentAction == JUMPINGLEFT)
        jumpLeft(0, boxEnemy); //send jump left
        //Acciones de una sola tecla
    else if (currentAction == JUMPINGVERTICAL)
        jumpVertical(boxEnemy); //send jump vertical
    else if (currentAction == DUCK)
        renderDuckSprite();  // send duck
    else if (currentAction == BLOCK)
        renderBlockSprite();
    else if (currentAction == MOVINGRIGHT)
        moveRight(distance, 1, boxEnemy, true, MOVINGRIGHT);   //send move right
    else if (currentAction == MOVINGLEFT)
        moveLeft(distance, 1, boxEnemy, true, MOVINGLEFT);    //send move left
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
	characterBox->updateBox(widthBlock, heightBlock);
    currentAction = BLOCK;
}

void CharacterServer::moveLeft(int distance, float vel, Box *boxOfEnemy, bool isGrounded, actions_t action) {
    currentAction = action;
    mPosX -= vel * CHARACTER_VEL;
    cout<<isGrounded<<endl;
    if(characterBox){
        if(this->characterBox->contactFromLeftSide(boxOfEnemy) && isGrounded)
            mPosX += vel *CHARACTER_VEL;
    }


    /*distance va de -800 a 800 (ancho de la pantalla)*/
    if ((mPosX - CHARACTER_VEL < -CharacterServer::getSobrante()) || (distance < (-anchoPantalla))) {
        //Move back
        mPosX += CHARACTER_VEL;
    }

    walkingSpriteUpdate();
}


void CharacterServer::moveRight(int distance, float vel, Box *boxOfEnemy, bool isGrounded, actions_t action) {
    currentAction = action;

    mPosX += vel *CHARACTER_VEL;
    /*if(this->characterBox->contactFromRightSide(boxOfEnemy) && isGrounded)
        mPosX -= vel *CHARACTER_VEL;*/


    if ((mPosX + CHARACTER_VEL >= (LEVEL_WIDTH - CharacterServer::getSobrante() - CharacterServer::getWidth())) ||
        (distance > anchoPantalla)) {
        //Move back
        mPosX -= CHARACTER_VEL;
    }
    characterBox->updateBox(widthWalking, heightWalking);
    walkingSpriteUpdate();
}


void CharacterServer::jump(int *currentSprite, int lastSprite, Box *boxOfEnemy) {

    *currentSprite < 10 ? (mPosY -= 2.5 * CHARACTER_VEL) : (mPosY += 2.5 * CHARACTER_VEL);
    (*currentSprite)++;
    if (*currentSprite > lastSprite) {
        *currentSprite = 0;
        mPosY = this->INITIAL_POS_Y;
        this->currentAction = STANDING;
        currentStandingSprite = 0;
        resetSpriteVariables();
    }
    if(characterBox->isColliding(boxOfEnemy) && *currentSprite > 7)
        characterBox->fixCollision(&mPosX, boxOfEnemy);
}

void CharacterServer::jumpVertical(Box *boxOfEnemy) {
	characterBox->updateBox(widthJumping, heightJumping);
    this->currentAction = JUMPINGVERTICAL;
    jump(&currentJumpingSprite, lastJumpingSprite, boxOfEnemy);
}

void CharacterServer::jumpRight(int distance, Box *boxOfEnemy) {
    moveRight(distance, 1.5, boxOfEnemy, false, JUMPINGRIGHT);
    characterBox->updateBox(widthJumpingLeft, heightJumpingLeft);
    jump(&currentJumpingRightSprite, lastJumpingRightSprite, boxOfEnemy);
}


void CharacterServer::jumpLeft(int distance, Box *boxOfEnemy) {
    moveLeft(distance, 1.5, boxOfEnemy, false, JUMPINGLEFT);
    characterBox->updateBox(widthJumpingLeft, heightJumpingLeft);
    jump(&currentJumpingLeftSprite, lastJumpingLeftSprite, boxOfEnemy);
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
    if(projectile->active)
        return;
    if(currentThrowPowerSprite== lastThrowPowerSprite)
        projectile->launch(this->getPosX(), isLookingLeft ? -1 : 1, isLookingLeft);
    normalAction(&currentThrowPowerSprite,&lastThrowPowerSprite,STANDING);
}

bool CharacterServer::isProjectileActive() {
    return projectile->active || projectile->itWasActiveAndDied;
}

ProjectileServer *CharacterServer::getProjectile() {
    return projectile;
}

bool CharacterServer::isProjectileHurting() {
    return !projectile->hitting && isProjectileActive();
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
    characterBox->updateBox(widthStanding, heightStanding);
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
             currentAction == PUNCHINGJUMPLEFT || currentAction == KICKINGVERTICAL ||
             currentAction == KICKINGJUMPRIGHT || currentAction == KICKINGJUMPLEFT
             || currentAction == HURTINGAIR || currentAction == KICKINGSTRONGJUMPLEFT
             ||currentAction == KICKINGJUMPRIGHT|| currentAction == KICKINGSTRONGVERTICAL
             ||currentAction ==  PUNCHINGSTRONGJUMPLEFT ||currentAction == PUNCHINGSTRONGJUMPRIGHT
             || currentAction == PUNCHINGSTRONGVERTICAL );
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
}

void CharacterServer::airActions(int *currentSprite, int lastSprite, actions_t nextAction, int airSprite,
                                 int lastAirSprite) {
    (*currentSprite)++;
    if (*currentSprite > lastSprite ) { //not sure  || 2 + airSprite + *currentSprite > lastAirSprite
        *currentSprite = 0;
        this->currentAction = nextAction; //falling
    }
    if (airSprite > lastAirSprite || airSprite == 0) {
        resetSpriteVariables();
        currentAction = STANDING;
    }

}

void CharacterServer::punchJumpVertical(actions_t punchVertical, Box *boxOfEnemy) {
	characterBox->updateBox(widthPunchAir, heightPunchAir);
    currentAction = punchVertical;
    jump(&currentJumpingSprite, lastJumpingSprite, boxOfEnemy);
    airActions(&currentPunchAirSprite, lastPunchAirSprite, JUMPINGVERTICAL, currentJumpingSprite, lastJumpingSprite);
}

void CharacterServer::punchJumpLeft(actions_t punchJumpLeft, int distance, Box *boxOfEnemy) {
    moveLeft(distance, 1.5, boxOfEnemy, false, punchJumpLeft);
    characterBox->updateBox(widthPunchAir, heightPunchAir);
    jump(&currentJumpingLeftSprite, lastJumpingLeftSprite, boxOfEnemy);
    airActions(&currentPunchAirSprite, lastPunchAirSprite, JUMPINGLEFT, currentJumpingLeftSprite,
               lastJumpingLeftSprite);
}

void CharacterServer::punchJumpRight(actions_t punchJumpRight, int distance, Box *boxOfEnemy) {
    moveRight(distance, 1.5, boxOfEnemy, false, punchJumpRight);
    characterBox->updateBox(widthPunchAir, heightPunchAir);
    jump(&currentJumpingRightSprite, lastJumpingRightSprite, boxOfEnemy);
    airActions(&currentPunchAirSprite, lastPunchAirSprite, JUMPINGRIGHT, currentJumpingRightSprite,
               lastJumpingRightSprite);
}

void CharacterServer::kickJumpVertical(actions_t kickVertical, Box *boxOfEnemy) {
	characterBox->updateBox(widthKickAir, heightKickAir);
    currentAction = kickVertical;
    jump(&currentJumpingSprite, lastJumpingSprite, boxOfEnemy);
    airActions(&currentKickAirSprite, lastKickAirSprite, JUMPINGVERTICAL, currentJumpingSprite, lastJumpingSprite);
}

void CharacterServer::kickJumpRight(actions_t kickJumpRight, int distance, Box *boxOfEnemy) {
    moveRight(distance, 1.5, boxOfEnemy, false, kickJumpRight);
    characterBox->updateBox(widthKickAir, heightKickAir);
    jump(&currentJumpingRightSprite, lastJumpingRightSprite, boxOfEnemy);
    airActions(&currentKickAirSprite, lastKickAirSprite, JUMPINGRIGHT, currentJumpingRightSprite,
               lastJumpingRightSprite);
}

void CharacterServer::kickJumpLeft(actions_t kickJumpLeft, int distance, Box *boxOfEnemy) {
    moveLeft(distance, 1.5, boxOfEnemy, false, kickJumpLeft);
    characterBox->updateBox(widthKickAir, heightKickAir);
    jump(&currentJumpingLeftSprite, lastJumpingLeftSprite, boxOfEnemy);
    airActions(&currentKickAirSprite, lastKickAirSprite, JUMPINGLEFT, currentJumpingLeftSprite, lastJumpingLeftSprite);
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

void CharacterServer::falling(int distance, int posContrincante, Box *boxOfEnemy) {
    //guardo el isLookingLeft inicial en el primer sprite
    if (currentFallingSprite == 0) {
        if (isLookingLeft)
            initialLookingLeft = true;
        else
            initialLookingLeft = false;
    }
    isLookingLeft = initialLookingLeft;


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
            moveRight(distance, 2, boxOfEnemy, false, MOVINGRIGHT);
        } else
            moveLeft(distance, 2, boxOfEnemy, false, MOVINGLEFT);
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


bool CharacterServer::isHurting() {
    return currentAction == HURTINGAIR || currentAction == HURTINGGROUND;
}

void CharacterServer::quitarVida(float danio) {
    if ((life-danio)<0) life = 0;
    else life = life - danio;
}

float CharacterServer::getVida() {
    return life;
}

void CharacterServer::updateBox() {
    characterBox->setCenter(getCentro(),mPosY);
}



