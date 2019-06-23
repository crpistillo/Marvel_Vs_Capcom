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
const int LAST_PUNCH_SPRITE = 5;
const int LAST_KICK_SPRITE = 5;
const int LAST_PUNCH_DOWN_SPRITE = 5;
const int LAST_KICK_DOWN_SPRITE = 5;
const int LAST_HURTING_SPRITE = 5;
const int LAST_HURTING_AIR_SPRITE = 29;
const int LAST_PUNCH_AIR_SPRITE = 5;
const int LAST_KICK_AIR_SPRITE = 5;
const int LAST_THROW_POWER_SPRITE = 7;
const int LAST_GRIP_SPRITE = 3;
const int LAST_FALLING_SPRITE = 43;
const int LAST_THROW_SPRITE = 26;

const int widthStandingW = 80;
const int heightStandingW = 84;
const int widthWalkingW = 88;
const int heightWalkingW = 86;
const int widthDuckW = 92;
const int heightDuckW = 59;
const int widthPunchW = 131;
const int heightPunchW = 95;
const int widthPunchDownW = 156;
const int heightPunchDownW = 56;
const int widthKickW = 164;
const int heightKickW = 96;
const int widthKickDownW = 146;
const int heightKickDownW = 51;
const int widthKickAirW = 164;
const int heightKickAirW = 97;
const int widthPunchAirW = 191;
const int heightPunchAirW = 64;
const int widthJumpingW = 119;
const int heightJumpingW = 52;
const int widthJumpingLeftW = 104;
const int heightJumpingLeftW = 104;




WolverineServer::WolverineServer(int PosX, int width, int height, int sobrante, int ancho, int anchoPantalla,
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
    lastWalkbackSprite = LAST_WALKBACK_SPRITE;
    lastIntroSprite = LAST_INTRO_SPRITE;
    lastPunchSprite = LAST_PUNCH_SPRITE;
    lastKickSprite = LAST_KICK_SPRITE;
    lastPunchDownSprite = LAST_PUNCH_DOWN_SPRITE;
    lastKickDownSprite = LAST_KICK_DOWN_SPRITE;
    lastHurtingSprite = LAST_HURTING_SPRITE;
    lastHurtingAirSprite = LAST_HURTING_AIR_SPRITE;
    lastPunchAirSprite = LAST_PUNCH_AIR_SPRITE;
    lastKickAirSprite = LAST_KICK_AIR_SPRITE;
	lastThrowPowerSprite = LAST_THROW_POWER_SPRITE;
	lastGripSprite = LAST_GRIP_SPRITE;
	lastFallingSprite = LAST_FALLING_SPRITE;
    lastThrowSprite = LAST_THROW_SPRITE;

	widthStanding = widthStandingW;
	heightStanding = heightStandingW;
	widthWalking = widthWalkingW;
	heightWalking = heightWalkingW;
	widthDuck = widthDuckW;
	heightDuck = heightDuckW;
	widthPunch = widthPunchW;
	heightPunch = heightPunchW;
	widthPunchDown = widthPunchDownW;
	heightPunchDown = heightPunchDownW;
	widthKick = widthKickW;
	heightKick = heightKickW;
	widthKickDown = widthKickDownW;
	heightKickDown = heightKickDownW;
	widthKickAir = widthKickAirW;
	heightKickAir = heightKickAirW;
	widthPunchAir = widthPunchAirW;
	heightPunchAir = heightPunchAirW;
	widthJumping = widthJumpingW;
	heightJumping = heightJumpingW;
	widthJumpingLeft = widthJumpingLeftW;
	heightJumpingLeft = heightJumpingLeftW;

    //Box* objetoColisionable = new Box(this->getCentro(),mPosY,widthStanding,heightStanding);
    //Probablemnte a ese mPosY hay que sumarle la mitad de la altura, pero no estoy seguro
}


void WolverineServer::moveLeft(int distance, int vel, Box *boxOfEnemy, bool isGrounded) {
    currentAction = MOVINGLEFT;

    //Mover
    mPosX -= vel * CHARACTER_VEL;


    if ((mPosX - CHARACTER_VEL <= -WolverineServer::getSobrante()) || (distance < (-anchoPantalla))) {
        isLookingLeft = false;
        //Move back
        mPosX += CHARACTER_VEL;
    }

    if (isLookingLeft) {
        walkingSpriteUpdate();
    } else {
        if (currentWalkbackSprite >= LAST_WALKBACK_SPRITE)
            currentWalkbackSprite = 0;
        currentAction = WALKBACK;

        ++currentWalkbackSprite;
        isLookingLeft = false;
    }

    characterBox->updateBox(widthWalking, heightWalking);
}

void WolverineServer::moveRight(int distance, int vel, Box *boxOfEnemy, bool isGrounded) {
    currentAction = MOVINGRIGHT;

    //Mover
    mPosX += vel * CHARACTER_VEL;

    if ((mPosX + CHARACTER_VEL >= (LEVEL_WIDTH - WolverineServer::getSobrante() - WolverineServer::getWidth())) ||
        (distance > anchoPantalla)) {
        isLookingLeft = true;
        //Move back
        mPosX -= CHARACTER_VEL;
    }

    if (!isLookingLeft) {
        walkingSpriteUpdate();
    } else {
        if (currentWalkbackSprite >= LAST_WALKBACK_SPRITE)
            currentWalkbackSprite = 0;
        currentAction = WALKBACK;

        ++currentWalkbackSprite;
        isLookingLeft = true;
	}

    characterBox->updateBox(widthWalking, heightWalking);
}


void WolverineServer::makeBuilderStruct(character_builder_t *builder, bool isFirstTeam) {
    builder->personaje = WOLVERINE;
    builder->cliente = clientNumber;
    builder->sprite = 0;
    builder->action = STANDING;
    builder->isFirstTeam = isFirstTeam;
    builder->pos = this->mPosX;
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
        case HURTINGAIR:
            spriteNumber = currentHurtingAirSprite;
            break;
        case HURTINGGROUND:
            spriteNumber = currentHurtingAirSprite;
            break;
		case THROW:
			spriteNumber = currentThrowSprite;
			break;
		case THROWPOWER:
			spriteNumber = currentThrowPowerSprite;
			break;
		case GRIP:
			spriteNumber = currentGripSprite;
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

void WolverineServer::stand() {
    currentAction = STANDING;
    resetSpriteVariables();
    if (currentStandingSprite >= lastStandingSprite)
        currentStandingSprite = 0;
    characterBox->updateBox(widthStanding, heightStanding);
}


