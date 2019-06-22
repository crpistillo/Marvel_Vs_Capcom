//
// Created by IgVelasco on 5/17/19.
//

#include "SpidermanServer.h"
#include "CharacterServer.h"
#include "Projectile.h"

const int LEVEL_WIDTH = 3200;
const int LEVEL_HEIGHT = 600;

const int LAST_STANDING_SPRITE = 8;  //done
const int LAST_WALKING_SPRITE = 10; //DONE
const int LAST_JUMPING_SPRITE = 21; //done
const int LAST_JUMPING_RIGHT_SPRITE = 19; //DONE
const int LAST_JUMPING_LEFT_SPRITE = 19; //DONE
const int LAST_INTRO_SPRITE = 41; //semi done
const int LAST_PUNCH_SPRITE = 5; // done  //llegue aca
const int LAST_KICK_SPRITE = 5; //done
const int LAST_PUNCH_DOWN_SPRITE = 7;
const int LAST_KICK_DOWN_SPRITE = 5; //done
const int LAST_HURTING_SPRITE = 5;  //done
const int LAST_HURTING_AIR_SPRITE = 29; //done
const int LAST_PUNCH_AIR_SPRITE = 8;
const int LAST_KICK_AIR_SPRITE = 8;
const int LAST_THROW_POWER_SPRITE = 9;
const int LAST_GRIP_SPRITE = 3;
const int LAST_THROW_SPRITE = 28;
const int LAST_FALLING_SPRITE = 52;

const int widthStanding = 110;
const int heightStanding = 96;
const int widthWalking = 87;
const int heightWalking = 91;
const int widthDuck = 100;
const int heightDuck = 52;
const int widthPunch = 152;
const int heightPunch = 77;
const int widthPunchDown = 140;
const int heightPunchDown = 51;
const int widthKick = 146;
const int heightKick = 93;
const int widthKickDown = 136;
const int heightKickDown = 65;



SpidermanServer::SpidermanServer(int PosX, int width, int height, int sobrante, int ancho, int anchoPantalla,
                                 int numberOfClient)
        : CharacterServer(
        PosX,
        556 - (height * 297 / 480),
        ancho,
        sobrante,
        false,
        width,
        height,
        anchoPantalla,
        numberOfClient) {
    lastStandingSprite = LAST_STANDING_SPRITE;
    lastWalkingSprite = LAST_WALKING_SPRITE;
    lastJumpingSprite = LAST_JUMPING_SPRITE;
    lastJumpingRightSprite = LAST_JUMPING_RIGHT_SPRITE;
    lastJumpingLeftSprite = LAST_JUMPING_LEFT_SPRITE;
    lastIntroSprite = LAST_INTRO_SPRITE;
    lastPunchSprite = LAST_PUNCH_SPRITE;
    lastKickSprite = LAST_KICK_SPRITE;
    lastPunchDownSprite = LAST_PUNCH_DOWN_SPRITE;
    lastKickDownSprite = LAST_KICK_DOWN_SPRITE;
    lastHurtingSprite = LAST_HURTING_SPRITE;
    lastHurtingAirSprite = LAST_HURTING_AIR_SPRITE;
    lastThrowSprite = LAST_THROW_SPRITE;
    lastPunchAirSprite = LAST_PUNCH_AIR_SPRITE;
    lastKickAirSprite = LAST_KICK_AIR_SPRITE;
    lastThrowPowerSprite = LAST_THROW_POWER_SPRITE;
    lastGripSprite = LAST_GRIP_SPRITE;
    lastFallingSprite = LAST_FALLING_SPRITE;

    this->projectile = new Projectile();


    //Box* objetoColisionable = new Box(this->getCentro(),mPosY,widthWalking,heightWalking);
}



void SpidermanServer::moveLeft(int distance, int vel) {
    currentAction = MOVINGLEFT;
    mPosX -= vel * CHARACTER_VEL;


    /*distance va de -800 a 800 (ancho de la pantalla)*/
    if ((mPosX - CHARACTER_VEL < -SpidermanServer::getSobrante()) || (distance < (-anchoPantalla))) {
        //Move back
        mPosX += CHARACTER_VEL;
    }

    characterBox->updateBox(widthWalking, heightWalking);
    walkingSpriteUpdate();
}


void SpidermanServer::moveRight(int distance, int vel) {
    currentAction = MOVINGRIGHT;

    mPosX += vel *CHARACTER_VEL;

    if ((mPosX + CHARACTER_VEL >= (LEVEL_WIDTH - SpidermanServer::getSobrante() - SpidermanServer::getWidth())) ||
        (distance > anchoPantalla)) {
        //Move back
        mPosX -= CHARACTER_VEL;
    }
    characterBox->updateBox(widthWalking, heightWalking);
    walkingSpriteUpdate();
}


void SpidermanServer::makeBuilderStruct(character_builder_t *builder, bool isFirstTeam) {
    //Completar
    builder->personaje = SPIDERMAN;
    builder->cliente = clientNumber;
    builder->sprite = 0;
    builder->action = STANDING;
    builder->isFirstTeam = isFirstTeam;
    builder->pos = this->mPosX;

}


int SpidermanServer::getSpriteNumber(){
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
        case PUNCHINGJUMPLEFT:
        case PUNCHINGJUMPRIGHT:
        case PUNCHINGVERTICAL:
        case PUNCHINGSTRONGJUMPLEFT:
        case PUNCHINGSTRONGJUMPRIGHT:
        case PUNCHINGSTRONGVERTICAL:
            spriteNumber = currentPunchAirSprite;
            break;
        case PUNCH:
        case PUNCHSTRONG:
            spriteNumber = currentPunchSprite;
            break;
        case KICKINGVERTICAL:
        case KICKINGJUMPRIGHT:
        case KICKINGJUMPLEFT:
        case KICKINGSTRONGVERTICAL:
        case KICKINGSTRONGJUMPRIGHT:
        case KICKINGSTRONGJUMPLEFT:
            spriteNumber = currentKickAirSprite;
            break;
        case KICK:
        case KICKSTRONG:
            spriteNumber = currentKickSprite;
            break;
        case PUNCHDOWN:
        case PUNCHSTRONGDOWN:
            spriteNumber = currentPunchDownSprite;
            break;
        case KICKDOWN:
        case KICKSTRONGDOWN:
            spriteNumber = currentKickDownSprite;
            break;
        case THROWPOWER:
            spriteNumber = currentThrowPowerSprite;
            break;
        case HURTINGAIR:
            spriteNumber = currentHurtingAirSprite;
            break;
        case HURTINGGROUND:
            spriteNumber = currentHurtingSprite;
            break;
		case GRIP:
			spriteNumber = currentGripSprite;
			break;
		case THROW:
			spriteNumber = currentThrowSprite;
			break;
		case FALLING:
			spriteNumber = currentFallingSprite;
			break;
        default:
            spriteNumber = 0;
            break;
    }
    return spriteNumber;
}



void SpidermanServer::stand() {
    currentAction = STANDING;
    resetSpriteVariables();
    if (currentStandingSprite >= lastStandingSprite)
        currentStandingSprite = 0;
    characterBox->updateBox(widthStanding, heightStanding);
}

void SpidermanServer::update(int distance, int posContrincante, actions_t actionRecieved, Box *boxContrincante) {
    if(projectile->active)
        projectile->travel();
    CharacterServer::update(distance, posContrincante, actionRecieved, boxContrincante);
}

void SpidermanServer::throwPower() {
    if(projectile->active)
        return;
    if(currentThrowPowerSprite== lastThrowPowerSprite)
        projectile->launch(this->getPosX(), isLookingLeft ? -1 : 1, isLookingLeft);
    CharacterServer::throwPower();

}

bool SpidermanServer::isProjectileActive() {
    return projectile->active || projectile->itWasActiveAndDied;
}

Projectile *SpidermanServer::getProjectile() {
    return projectile;
}

bool SpidermanServer::isProjectileHurting() {
    return !projectile->hitting && isProjectileActive();
}
