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
    this->currentThrowPowerSprite = 0;
    this->currentGripSprite = 0;
    this->currentFallingSprite = 0;
    this->currentThrowSprite = 0;

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
		currentAction == THP || currentAction == FA
		|| currentAction == TH)
        actionStarted = true;


    if (currentAction == MAKINGINTRO) {
        makeIntro();
    }

    if (currentAction == PUNCH) {
        punch();
    }

    if (currentAction == KICK) {
        kick();
    }

    if (currentAction == PUNCHDOWN) {
        punchDown();
    }

    if (currentAction == KICKDOWN) {
        kickDown();
    }

    if (currentAction == THROWPOWER) {
        throwPower();
    }

    if (currentAction == JUMPINGVERTICAL) {    //Si saltaba verticalmente, lo fuerzo a que siga con esa accion
        jumpVertical();
    }

    if (currentAction == JUMPINGRIGHT) {        //Si saltaba hacia la derecha, lo fuerzo a que siga con esa accion

        moveRight(distance, posContrincante, 1);
        jumpRight();
    }

    if (currentAction == JUMPINGLEFT) {
        moveLeft(distance, posContrincante, 1);
        jumpLeft();
    }

    if(currentAction == HURTINGGROUND){
        hurting();
    }

    if(currentAction == GRIP)
    {
    	grip();
    }

    if(currentAction == THROW)
    {
    	throwCharacter();
    }

    if(currentAction == FALLING)
    {
    	falling(distance,posContrincante);
    }

    if (actionStarted) {
        this->characterBox->setCenter(getCentro(), mPosY);
        return;
    }

    this->currentAction = actionRecieved;

    //Acciones de dos teclas primero
    if (currentAction == JUMPINGVERTICAL)
        jumpVertical(); //send jump rigth
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
        moveRight(distance, posContrincante, 1);   //send move right
    else if (currentAction == MOVINGLEFT)
        moveLeft(distance, posContrincante, 1);    //send move left
    else if (currentAction == PUNCH)
        punch();
    else if (currentAction == KICK)
        kick();
    else if (currentAction == PUNCHDOWN)
        punchDown();
    else if (currentAction == KICKDOWN)
        kickDown();
    else if (currentAction == THROWPOWER)
        throwPower();
    else if (currentAction == HURTINGGROUND)
        hurting();

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
    currentWalkingSprite = 0;
    currentIntroSprite = 0;
    currentPunchSprite = 0;
    currentKickSprite = 0;
    currentPunchDownSprite = 0;
    currentKickDownSprite = 0;
    currentThrowPowerSprite = 0;
    currentFallingSprite = 0;
    currentThrowSprite = 0;
}


void CharacterServer::stand() {
    currentAction = STANDING;
    this->resetSpriteVariables();
    if (currentStandingSprite >= lastStandingSprite)
        currentStandingSprite = 0;
}


void CharacterServer::renderDuckSprite() {
    currentAction = DUCK;
}

void CharacterServer::renderBlockSprite() {
    currentAction = BLOCK;
}

void CharacterServer::moveLeft(int distance, int posContrincante, int vel) {
    currentAction = MOVINGLEFT;
    mPosX -= vel * CHARACTER_VEL;

    /*distance va de -800 a 800 (ancho de la pantalla)*/
    if ((mPosX - CHARACTER_VEL < -CharacterServer::getSobrante()) || (distance < (-anchoPantalla))) {
        //Move back
        mPosX += CHARACTER_VEL;
    }

    walkingSpriteUpdate();
}


void CharacterServer::moveRight(int distance, int posContrincante, int vel) {
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
    }
}

void CharacterServer::jumpVertical() {
    this->currentAction = JUMPINGVERTICAL;
    jump(&currentJumpingSprite, lastJumpingSprite);
    cout<<" mi caja "<<this->characterBox->getTop() <<" and " << this->characterBox->getBottom()<< endl;
}

void CharacterServer::jumpRight() {
    this->currentAction = JUMPINGRIGHT;
    jump(&currentJumpingRightSprite, lastJumpingRightSprite);

}


void CharacterServer::jumpLeft() {
    this->currentAction = JUMPINGLEFT;
    jump(&currentJumpingLeftSprite, lastJumpingLeftSprite);
}

void CharacterServer::throwCharacter()
{
	this->currentAction = THROW;
	currentThrowSprite++;
	if(currentThrowSprite > lastThrowSprite)
	{
		currentThrowSprite = 0;
		this->currentAction = STANDING;
		currentStandingSprite = 0;
	}
}

void CharacterServer::grip()
{
	this->currentAction = GRIP;
	currentGripSprite++;
	if(currentGripSprite > lastGripSprite)
	{
		currentGripSprite = 0;
		this->currentAction = STANDING;
		currentStandingSprite = 0;
	}
}

void CharacterServer::falling(int distance, int posContrincante)
{
	//CASO SPIDERMAN FALLING

	bool initialLookingLeft;

	if(currentFallingSprite>=0 && currentFallingSprite<8)
	{
		if(isLookingLeft)
		{
			initialLookingLeft = true;
			mPosX = posContrincante-sobrante-150;
		}
		else initialLookingLeft = false;
	}


	else if(currentFallingSprite==8)
	{
		if (isLookingLeft) {
			initialLookingLeft = true;
			mPosX = posContrincante - sobrante;
		} else
			initialLookingLeft = false;
	}
	if (currentFallingSprite > 8 && currentFallingSprite<36)
	{
		if (isLookingLeft) {
			moveRight(distance, posContrincante, 2);
		} else
			moveRight(distance, posContrincante, 2);
	}

	if(currentFallingSprite>8 && currentFallingSprite<17)
	{
		mPosY -= 4 * CHARACTER_VEL;
	}

	else if(currentFallingSprite>17 && currentFallingSprite<36)
	{
		mPosY += 1.7 * CHARACTER_VEL;
	}

	this->currentAction = FALLING;
	currentFallingSprite++;
	if(currentFallingSprite > lastFallingSprite)
	{
		currentFallingSprite = 0;
		this->currentAction = STANDING;
		currentStandingSprite = 0;
		mPosY = this->INITIAL_POS_Y;
	}

	isLookingLeft = initialLookingLeft;

}

void CharacterServer::punch() {
    this->currentAction = PUNCH;
    currentPunchSprite++;
    if (currentPunchSprite > lastPunchSprite) {
        currentPunchSprite = 0;
        this->currentAction = STANDING;
        currentStandingSprite = 0;
    }
}

void CharacterServer::kick() {
    this->currentAction = KICK;
    currentKickSprite++;
    if (currentKickSprite > lastKickSprite) {
        currentKickSprite = 0;
        this->currentAction = STANDING;
        currentStandingSprite = 0;
    }
}

void CharacterServer::throwPower() {
    this->currentAction = THROWPOWER;
    currentThrowPowerSprite++;
    if (currentThrowPowerSprite > lastThrowPowerSprite) {
        currentThrowPowerSprite = 0;
        this->currentAction = STANDING;
        currentStandingSprite = 0;
    }
}

void CharacterServer::punchDown() {
    this->currentAction = PUNCHDOWN;
    currentPunchDownSprite++;
    if (currentPunchDownSprite > lastPunchDownSprite) {
        currentPunchDownSprite = 0;
        this->currentAction = DUCK;
    }
}

void CharacterServer::kickDown() {
    this->currentAction = KICKDOWN;
    currentKickDownSprite++;
    if (currentKickDownSprite > lastKickDownSprite) {
        currentKickDownSprite = 0;
        this->currentAction = DUCK;
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

void CharacterServer::makeUpdaterStruct(character_updater_t *updater) {
    updater->posX = this->mPosX;
    updater->posY = this->mPosY;
    updater->action = this->currentAction;

    switch (this->currentAction) {
        case STANDING:
            updater->currentSprite = this->currentStandingSprite;
            break;
        case PUNCH:
            updater->currentSprite = this->currentPunchSprite;
            break;
        case KICK:
            updater->currentSprite = this->currentKickSprite;
            break;
        case PUNCHDOWN:
            updater->currentSprite = this->currentPunchDownSprite;
            break;
        case KICKDOWN:
            updater->currentSprite = this->currentKickDownSprite;
            break;
        case THROWPOWER:
            updater->currentSprite = this->currentThrowPowerSprite;
            break;
        case GRIP:
        	updater->currentSprite = this->currentGripSprite;
        	break;
        case THROW:
        	updater->currentSprite = this->currentThrowSprite;
        	break;
        case FALLING:
        	updater->currentSprite = this->currentFallingSprite;
        	break;
        case JUMPINGLEFT:
            updater->currentSprite = this->currentJumpingLeftSprite;
            break;
        case JUMPINGRIGHT:
            updater->currentSprite = this->currentJumpingRightSprite;
            break;
        case JUMPINGVERTICAL:
            updater->currentSprite = this->currentJumpingSprite;
            break;
        case MAKINGINTRO:
            updater->currentSprite = this->currentIntroSprite;
            break;
        case DUCK:
            updater->currentSprite = 0;
            break;
        case BLOCK:
            updater->currentSprite = 0;
            break;
        case MOVINGRIGHT:
            updater->currentSprite = this->currentWalkingSprite;
            break;
        case MOVINGLEFT:
            updater->currentSprite = this->currentWalkingSprite;
            break;
        case WALKBACK:
            updater->currentSprite = this->currentWalkbackSprite;
            break;
    }

}

actions_t CharacterServer::getCurrentAction() {
    return this->currentAction;
}

bool CharacterServer::isStanding() {
    return this->currentAction == STANDING;
}

Box *CharacterServer::getColisionable() {
    return characterBox;
}

void CharacterServer::hurting() {
    this->currentAction = HURTINGGROUND;
    currentHurtingSprite++;
    if (currentHurtingSprite > lastHurtingSprite) {
        currentHurtingSprite = 0;
        this->currentAction = STANDING;
        currentStandingSprite = 0;
    }
}

bool CharacterServer::inTheGround() {
    return !(currentAction == JUMPINGRIGHT || currentAction == JUMPINGLEFT || currentAction == JUMPINGLEFT);
}


