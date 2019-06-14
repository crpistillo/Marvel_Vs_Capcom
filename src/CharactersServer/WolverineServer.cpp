//
// Created by IgVelasco on 5/17/19.
//

#include "WolverineServer.h"


const int LEVEL_WIDTH = 3200;
const int LEVEL_HEIGHT = 600;

const int LAST_STANDING_SPRITE = 16;
const int LAST_WALKING_SPRITE = 15;
const int LAST_JUMPING_SPRITE = 20;
const int LAST_JUMPING_RIGHT_SPRITE = 19;
const int LAST_JUMPING_LEFT_SPRITE = 19;
const int LAST_WALKBACK_SPRITE = 15;
const int LAST_INTRO_SPRITE = 25;
const int LAST_PUNCH_SPRITE = 2;
const int LAST_KICK_SPRITE = 3;
const int LAST_PUNCH_DOWN_SPRITE = 1;
const int LAST_KICK_DOWN_SPRITE = 2;

const int widthStanding = 87;
const int heightStanding = 84;
const int widthWalking = 88;
const int heightWalking = 86;
const int widthDuck = 92;
const int heightDUck = 59;


WolverineServer::WolverineServer(int PosX, int width, int height, int sobrante, int ancho, int anchoPantalla,
                                 int numberOfClient, Box* caja)
        : CharacterServer(
        PosX,
        556 - (height * 297 / 480),
        ancho,
        sobrante,
        false,
        width,
        height,
        anchoPantalla,
        numberOfClient,
		caja
) {
    lastStandingSprite = LAST_STANDING_SPRITE;
    lastWalkingSprite = LAST_WALKING_SPRITE;
    lastJumpingSprite = LAST_JUMPING_SPRITE;
    lastJumpingRightSprite = LAST_JUMPING_RIGHT_SPRITE;
    lastJumpingLeftSprite = LAST_JUMPING_LEFT_SPRITE;
    lastWalkbackSprite = LAST_WALKBACK_SPRITE;
    lastIntroSprite = LAST_INTRO_SPRITE;
    lastPunchSprite = LAST_PUNCH_SPRITE;
    lastKickSprite = LAST_KICK_SPRITE;
    lastPunchDownSprite = LAST_PUNCH_DOWN_SPRITE;
    lastKickDownSprite = LAST_KICK_DOWN_SPRITE;

    //Box* objetoColisionable = new Box(this->getCentro(),mPosY,widthStanding,heightStanding);
    //Probablemnte a ese mPosY hay que sumarle la mitad de la altura, pero no estoy seguro
}


void WolverineServer::moveLeft(int distance, int posContrincante, Box* boxContrincante) {
	if (!(caja->isColliding(boxContrincante))){
    currentAction = MOVINGLEFT;

    //Mover
    mPosX -= CHARACTER_VEL;

    if ((mPosX - CHARACTER_VEL <= -WolverineServer::getSobrante()) || (distance < (-anchoPantalla))) {
        isLookingLeft = false;
        //Move back
        mPosX += CHARACTER_VEL;
    }

    if (this->getPosX() > posContrincante) {
        walkingSpriteUpdate();
    } else {
        if (currentWalkbackSprite >= LAST_WALKBACK_SPRITE)
            currentWalkbackSprite = 0;
        currentAction = WALKBACK;

        ++currentWalkbackSprite;
        isLookingLeft = false;
    }
    caja->setWidth(widthWalking);
    caja->setHeight(heightWalking);
    moverColisionable();
	}
}

void WolverineServer::moveRight(int distance, int posContrincante, Box* boxContrincante) {
	if (!(caja->isColliding(boxContrincante))){
    currentAction = MOVINGRIGHT;

    //Mover
    mPosX += CHARACTER_VEL;

    if ((mPosX + CHARACTER_VEL >= (LEVEL_WIDTH - WolverineServer::getSobrante() - WolverineServer::getWidth())) ||
        (distance > anchoPantalla)) {
        isLookingLeft = true;
        //Move back
        mPosX -= CHARACTER_VEL;
    }

    if (this->getPosX() < posContrincante) {
        walkingSpriteUpdate();
    } else {
        if (currentWalkbackSprite >= LAST_WALKBACK_SPRITE)
            currentWalkbackSprite = 0;
        currentAction = WALKBACK;

        ++currentWalkbackSprite;
        isLookingLeft = true;
	}
    caja->setWidth(widthWalking);
    caja->setHeight(heightWalking);
    moverColisionable();
	}
}

void WolverineServer::resetSpriteVariables(){
	return;
}

void WolverineServer::makeBuilderStruct(character_builder_t *builder, bool isFirstTeam, double pos) {
    builder->personaje = WOLVERINE;
    builder->cliente = clientNumber;
    builder->sprite = 0;
    builder->action = STANDING;
    builder->isFirstTeam = isFirstTeam;
    builder->pos = pos;
}


int WolverineServer::getSpriteNumber(){
    int spriteNumber;
    switch (this->currentAction){
        case STANDING:
            spriteNumber = currentStandingSprite;
            break;
        case JUMPINGVERTICAL:
            spriteNumber = currentJumpingSprite;
            break;
        case JUMPINGLEFT:
            spriteNumber = currentJumpingLeftSprite;
            break;
        case JUMPINGRIGHT:
            spriteNumber = currentJumpingRightSprite;
            break;
        case MOVINGRIGHT:
        case MOVINGLEFT:
            spriteNumber = currentWalkingSprite;
            break;
        case MAKINGINTRO:
            spriteNumber = currentIntroSprite;
            break;
        case WALKBACK:
            spriteNumber = currentWalkbackSprite;
            break;
        case PUNCH:
            spriteNumber = currentPunchSprite;
            break;
        case KICK:
            spriteNumber = currentKickSprite;
            break;
        case PUNCHDOWN:
            spriteNumber = currentPunchDownSprite;
            break;
        case KICKDOWN:
            spriteNumber = currentKickDownSprite;
            break;
        default:
            spriteNumber = 0;
            break;
    }
    return spriteNumber;
}

void WolverineServer::stand() {
    currentAction = STANDING;
    this->resetSpriteVariables();
    if (currentStandingSprite >= lastStandingSprite)
        currentStandingSprite = 0;
    caja->setWidth(widthStanding);
    caja->setHeight(heightStanding);
}


