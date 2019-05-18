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


WolverineServer::WolverineServer(int PosX, bool secondaryColor, int width, int height, int sobrante, int ancho,
                                 int anchoPantalla)
        : CharacterServer(
        PosX,
        556 - (height * 297 / 480),
        ancho,
        sobrante,
        false,
        width,
        height,
        anchoPantalla
) {
    lastStandingSprite = LAST_STANDING_SPRITE;
    lastWalkingSprite = LAST_WALKING_SPRITE;
    lastJumpingSprite = LAST_JUMPING_SPRITE;
    lastJumpingRightSprite = LAST_JUMPING_RIGHT_SPRITE;
    lastJumpingLeftSprite = LAST_JUMPING_LEFT_SPRITE;
    lastWalkbackSprite = LAST_WALKBACK_SPRITE;
    lastIntroSprite = LAST_INTRO_SPRITE;
}


void WolverineServer::moveLeft(int distance, int posContrincante) {
    currentAction = MOVINGLEFT;

    //Mover
    mPosX -= CHARACTER_VEL;

    if ((mPosX - CHARACTER_VEL <= -WolverineServer::getSobrante()) || (distance < (-anchoPantalla))) {
        isLookingLeft = false;
        //Move back
        mPosX += CHARACTER_VEL;
    }

    if (this->getCentro() > posContrincante) {
        walkingSpriteUpdate();
    } else {
        if (currentWalkbackSprite >= LAST_WALKBACK_SPRITE)
            currentWalkbackSprite = 0;
        currentAction = WALKBACK;

        ++currentWalkbackSprite;
        isLookingLeft = false;
    }
}

void WolverineServer::moveRight(int distance, int posContrincante) {
    currentAction = MOVINGRIGHT;

    //Mover
    mPosX += CHARACTER_VEL;

    if ((mPosX + CHARACTER_VEL >= (LEVEL_WIDTH - WolverineServer::getSobrante() - WolverineServer::getWidth())) ||
        (distance > anchoPantalla)) {
        isLookingLeft = true;
        //Move back
        mPosX -= CHARACTER_VEL;
    }

    if (this->getCentro() < posContrincante) {
        walkingSpriteUpdate();
    } else {
        if (currentWalkbackSprite >= LAST_WALKBACK_SPRITE)
            currentWalkbackSprite = 0;
        currentAction = WALKBACK;

        ++currentWalkbackSprite;
        isLookingLeft = true;
    }

}

void WolverineServer::resetSpriteVariables(){
	return;
}

void WolverineServer::makeBuilderStruct(character_builder_t* builder){
	builder->personaje = WOLVERINE;
	builder->sprite = 0;
	builder->action = STANDING;
}

