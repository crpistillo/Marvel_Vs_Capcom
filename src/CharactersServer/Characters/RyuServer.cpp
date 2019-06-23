//
// Created by IgVelasco on 5/17/19.
//

#include "RyuServer.h"
#include "CharacterServer.h"
#include "../Projectiles/ProjectileServer.h"

const int LEVEL_WIDTH = 3200;
const int LEVEL_HEIGHT = 600;

const int LAST_STANDING_SPRITE = 8;
const int LAST_WALKING_SPRITE = 10;
const int LAST_JUMPING_SPRITE = 21;
const int LAST_JUMPING_RIGHT_SPRITE = 19;
const int LAST_JUMPING_LEFT_SPRITE = 19;
const int LAST_INTRO_SPRITE = 16;
const int LAST_PUNCH_SPRITE = 5;
const int LAST_KICK_SPRITE = 5;
const int LAST_PUNCH_DOWN_SPRITE = 7;
const int LAST_KICK_DOWN_SPRITE = 5;
const int LAST_HURTING_SPRITE = 5;
const int LAST_HURTING_AIR_SPRITE = 29;
const int LAST_PUNCH_AIR_SPRITE = 8;
const int LAST_KICK_AIR_SPRITE = 8;
const int LAST_THROW_POWER_SPRITE = 9;
const int LAST_GRIP_SPRITE = 3;
const int LAST_THROW_SPRITE = 28;
const int LAST_FALLING_SPRITE = 52;

const int widthStandingS = 90;
const int heightStandingS = 96;
const int widthWalkingS = 87;
const int heightWalkingS = 91;
const int widthDuckS = 100;
const int heightDuckS = 52;
const int widthPunchS = 152;
const int heightPunchS = 77;
const int widthPunchDownS = 150;
const int heightPunchDownS = 51;
const int widthKickS = 146;
const int heightKickS = 93;
const int widthKickDownS = 160;
const int heightKickDownS = 65;
const int widthKickAirS = 144;
const int heightKickAirS = 82;
const int widthPunchAirS = 84;
const int heightPunchAirS = 85;
const int widthJumpingS = 99;
const int heightJumpingS = 50;
const int widthJumpingLeftS = 70;
const int heightJumpingLeftS = 70;





RyuServer::RyuServer(int PosX, int width, int height, int sobrante, int ancho, int anchoPantalla,
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

    this->projectile = new ProjectileServer();

    widthStanding = widthStandingS;
    heightStanding = heightStandingS;
    widthWalking = widthWalkingS;
    heightWalking = heightWalkingS;
    widthDuck = widthDuckS;
    heightDuck = heightDuckS;
    widthPunch = widthPunchS;
    heightPunch = heightPunchS;
    widthPunchDown = widthPunchDownS;
    heightPunchDown = heightPunchDownS;
    widthKick = widthKickS;
    heightKick = heightKickS;
    widthKickDown = widthKickDownS;
    heightKickDown = heightKickDownS;
    widthKickAir = widthKickAirS;
    heightKickAir = heightKickAirS;
    widthPunchAir = widthPunchAirS;
    heightPunchAir = heightPunchAirS;
    widthJumping = widthJumpingS;
    heightJumping = heightJumpingS;
    widthJumpingLeft = widthJumpingLeftS;
    heightJumpingLeft = heightJumpingLeftS;


    //Box* objetoColisionable = new Box(this->getCentro(),mPosY,widthWalking,heightWalking);
}



void RyuServer::moveLeft(int distance, int vel, Box *boxOfEnemy, bool isGrounded) {
    currentAction = MOVINGLEFT;
    mPosX -= vel * CHARACTER_VEL;


    /*distance va de -800 a 800 (ancho de la pantalla)*/
    if ((mPosX - CHARACTER_VEL < -RyuServer::getSobrante()) || (distance < (-anchoPantalla))) {
        //Move back
        mPosX += CHARACTER_VEL;
    }

    characterBox->updateBox(widthWalking, heightWalking);
    walkingSpriteUpdate();
}


void RyuServer::moveRight(int distance, int vel, Box *boxOfEnemy, bool isGrounded) {
    currentAction = MOVINGRIGHT;

    mPosX += vel *CHARACTER_VEL;

    if ((mPosX + CHARACTER_VEL >= (LEVEL_WIDTH - RyuServer::getSobrante() - RyuServer::getWidth())) ||
        (distance > anchoPantalla)) {
        //Move back
        mPosX -= CHARACTER_VEL;
    }
    characterBox->updateBox(widthWalking, heightWalking);
    walkingSpriteUpdate();
}


void RyuServer::makeBuilderStruct(character_builder_t *builder, bool isFirstTeam) {
    //Completar
    builder->personaje = RYU;
    builder->cliente = clientNumber;
    builder->sprite = 0;
    builder->action = STANDING;
    builder->isFirstTeam = isFirstTeam;
    builder->pos = this->mPosX;

}


int RyuServer::getSpriteNumber(){
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



void RyuServer::stand() {
    currentAction = STANDING;
    resetSpriteVariables();
    if (currentStandingSprite >= lastStandingSprite)
        currentStandingSprite = 0;
    characterBox->updateBox(widthStanding, heightStanding);
}

void RyuServer::update(int distance, int posContrincante, actions_t actionRecieved, Box *boxEnemy) {
    if(projectile->active)
        projectile->travel();
    CharacterServer::update(distance, posContrincante, actionRecieved, boxEnemy);
}

void RyuServer::throwPower() {
    if(projectile->active)
        return;
    if(currentThrowPowerSprite== lastThrowPowerSprite)
        projectile->launch(this->getPosX(), isLookingLeft ? -1 : 1, isLookingLeft);
    CharacterServer::throwPower();

}

bool RyuServer::isProjectileActive() {
    return projectile->active || projectile->itWasActiveAndDied;
}

ProjectileServer *RyuServer::getProjectile() {
    return projectile;
}

bool RyuServer::isProjectileHurting() {
    return !projectile->hitting && isProjectileActive();
}
