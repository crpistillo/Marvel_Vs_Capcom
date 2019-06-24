//
// Created by IgVelasco on 5/17/19.
//

#include "IronmanServer.h"
#include "CharacterServer.h"
#include "../Projectiles/ProjectileServer.h"

const int LEVEL_WIDTH = 3200;
const int LEVEL_HEIGHT = 600;

const int LAST_STANDING_SPRITE = 8;
const int LAST_WALKING_SPRITE = 10;
const int LAST_JUMPING_SPRITE = 21;
const int LAST_JUMPING_RIGHT_SPRITE = 19;
const int LAST_JUMPING_LEFT_SPRITE = 19;
const int LAST_INTRO_SPRITE = 41;
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
const int LAST_THROW_SPRITE = 22;
const int LAST_FALLING_SPRITE = 52;


const int widthStandingI = 102;
const int heightStandingI = 103;
const int widthWalkingI = 96;
const int heightWalkingI = 116;
const int widthDuckI = 114;
const int heightDuckI = 72;
const int widthPunchI = 133;
const int heightPunchI = 102;
const int widthPunchDownI = 151;
const int heightPunchDownI = 75;
const int widthKickI = 187;
const int heightKickI = 90;
const int widthKickDownI = 143;
const int heightKickDownI = 64;
const int widthKickAirI = 123;
const int heightKickAirI = 105;
const int widthPunchAirI = 165;
const int heightPunchAirI = 122;
const int widthJumpingI = 101;
const int heightJumpingI = 103;
const int widthJumpingLeftI = 109;
const int heightJumpingLeftI = 145;





IronmanServer::IronmanServer(int PosX, int width, int height, int sobrante, int ancho, int anchoPantalla,
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

    widthStanding = widthStandingI;
    heightStanding = heightStandingI;
    widthWalking = widthWalkingI;
    heightWalking = heightWalkingI;
    widthDuck = widthDuckI;
    heightDuck = heightDuckI;
    widthPunch = widthPunchI;
    heightPunch = heightPunchI;
    widthPunchDown = widthPunchDownI;
    heightPunchDown = heightPunchDownI;
    widthKick = widthKickI;
    heightKick = heightKickI;
    widthKickDown = widthKickDownI;
    heightKickDown = heightKickDownI;
    widthKickAir = widthKickAirI;
    heightKickAir = heightKickAirI;
    widthPunchAir = widthPunchAirI;
    heightPunchAir = heightPunchAirI;
    widthJumping = widthJumpingI;
    heightJumping = heightJumpingI;
    widthJumpingLeft = widthJumpingLeftI;
    heightJumpingLeft = heightJumpingLeftI;


    //Box* objetoColisionable = new Box(this->getCentro(),mPosY,widthWalking,heightWalking);
}



void IronmanServer::moveLeft(int distance, float vel, Box *boxOfEnemy, bool isGrounded) {
    currentAction = MOVINGLEFT;
    mPosX -= int(vel * float(CHARACTER_VEL));

    if(this->characterBox->contactFromLeftSide(boxOfEnemy) && isGrounded)
        mPosX += int(vel * float(CHARACTER_VEL));


    /*distance va de -800 a 800 (ancho de la pantalla)*/
    if ((mPosX - CHARACTER_VEL < -IronmanServer::getSobrante()) || (distance < (-anchoPantalla))) {
        //Move back
        if(!isGrounded)
            vel += 0.1;
        mPosX += int(vel * float(CHARACTER_VEL));
    }


    characterBox->updateBox(widthWalking, heightWalking);
    walkingSpriteUpdate();
}


void IronmanServer::moveRight(int distance, float vel, Box *boxOfEnemy, bool isGrounded) {
    currentAction = MOVINGRIGHT;

    mPosX += int(vel * float(CHARACTER_VEL));

    if(this->characterBox->contactFromRightSide(boxOfEnemy) && isGrounded)
        mPosX -= int(vel * float(CHARACTER_VEL));


    if ((mPosX + CHARACTER_VEL >= (LEVEL_WIDTH - IronmanServer::getSobrante() - IronmanServer::getWidth())) ||
        (distance > anchoPantalla)) {
        //Move back
        if(!isGrounded)
            vel += 0.1;
        mPosX -= int(vel * float(CHARACTER_VEL));
    }

    characterBox->updateBox(widthWalking, heightWalking);
    walkingSpriteUpdate();
}


void IronmanServer::makeBuilderStruct(character_builder_t *builder, bool isFirstTeam) {
    //Completar
    builder->personaje = IRONMAN;
    builder->cliente = clientNumber;
    builder->sprite = 0;
    builder->action = STANDING;
    builder->isFirstTeam = isFirstTeam;
    builder->pos = this->mPosX;

}


int IronmanServer::getSpriteNumber(){
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



void IronmanServer::stand() {
    currentAction = STANDING;
    resetSpriteVariables();
    if (currentStandingSprite >= lastStandingSprite)
        currentStandingSprite = 0;
    characterBox->updateBox(widthStanding, heightStanding);
}

void IronmanServer::update(int distance, int posContrincante, actions_t actionRecieved, Box *boxEnemy) {
    if(projectile->active)
        projectile->travel();
    CharacterServer::update(distance, posContrincante, actionRecieved, boxEnemy);
}
