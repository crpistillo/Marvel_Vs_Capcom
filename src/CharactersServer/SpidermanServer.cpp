//
// Created by IgVelasco on 5/17/19.
//

#include "SpidermanServer.h"
#include "CharacterServer.h"
#include "Projectile.h"

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





SpidermanServer::SpidermanServer(int PosX, int width, int height, int sobrante, int ancho, int anchoPantalla,
                                 int numberOfClient,
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
								 int heightJumpingLeft)
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
		widthStanding,
		heightStanding,
		widthWalking,
		heightWalking,
		widthDuck,
		heightDuck,
		widthPunch,
		heightPunch,
		widthPunchDown,
		heightPunchDown,
		widthKick,
		heightKick,
		widthKickDown,
		heightKickDown,
		widthKickAir,
		heightKickAir,
		widthPunchAir,
		heightPunchAir,
		widthJumping,
		heightJumping,
		widthJumpingLeft ,
		heightJumpingLeft) {
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
    this->name = "spiderman";


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
