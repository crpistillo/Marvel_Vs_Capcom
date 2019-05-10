//
// Created by amaherok on 3/31/19.
//

#include "./Wolverine.h"
#include "../InputTable.h"

using namespace std;

const int LEVEL_WIDTH = 3200;
const int LEVEL_HEIGHT = 600;

//Por defecto, todos los sprites comienzan en 0.
const int LAST_STANDING_SPRITE = 16;
const int LAST_WALKING_SPRITE = 15;
const int LAST_JUMPING_SPRITE = 20;
const int LAST_JUMPING_RIGHT_SPRITE = 19;
const int LAST_JUMPING_LEFT_SPRITE = 19;
const int LAST_WALKBACK_SPRITE = 15;
const int LAST_INTRO_SPRITE = 25;

const unsigned int SECONDARY_RED = 255;
const unsigned int SECONDARY_GREEN = 255;
const unsigned int SECONDARY_BLUE = 0;

const string MVC_FILEPATH = "/MVC2_Wolverine_";
const string FILE_EXTENSION = ".png";

Wolverine::Wolverine(int PosX, bool secondaryColor, int width, int height, int sobrante, int ancho, int anchoPantalla)
        : Character(
        	PosX,
			556-(height*297/480),
			ancho,
			sobrante,
        	false,
			width,
			height,
			anchoPantalla
) {
	if(secondaryColor)
		this->loader = new ImageLoader(SECONDARY_RED, SECONDARY_GREEN, SECONDARY_BLUE);
	else
		this->loader = new ImageLoader(255,255,255);
}

void Wolverine::load(SDL_Renderer *renderer) {
    switch (this->currentAction) {
        case STANDING:
            if (isLookingLeft)
                this->loader->loadActionSprite(characterFilepath + "standing_left", MVC_FILEPATH, currentStandingSprite,
                                               FILE_EXTENSION,
                                               renderer, &m_Texture);
            else
                this->loader->loadActionSprite(characterFilepath + "standing_right", MVC_FILEPATH,
                                               currentStandingSprite, FILE_EXTENSION,
                                               renderer, &m_Texture);
            break;
        case DUCK:
            if (isLookingLeft)
                this->loader->loadActionSprite(characterFilepath + "duck", MVC_FILEPATH, 220, FILE_EXTENSION,
                                               renderer, &m_Texture);
            else
                this->loader->loadActionSprite(characterFilepath + "duck", MVC_FILEPATH, 219, FILE_EXTENSION,
                                               renderer, &m_Texture);
            break;
        case JUMPINGVERTICAL:
            if (isLookingLeft)
                this->loader->loadActionSprite(characterFilepath + "jumping_inverted", MVC_FILEPATH,
                                               currentJumpingSprite,
                                               FILE_EXTENSION,
                                               renderer, &m_Texture);
            else
                this->loader->loadActionSprite(characterFilepath + "jumping", MVC_FILEPATH, currentJumpingSprite,
                                               FILE_EXTENSION,
                                               renderer, &m_Texture);
            break;
        case JUMPINGRIGHT:
            if (isLookingLeft)
                this->loader->loadActionSprite(characterFilepath + "jumping_right_inverted", MVC_FILEPATH,
                                               currentJumpingRightSprite, FILE_EXTENSION,
                                               renderer, &m_Texture);
            else
                this->loader->loadActionSprite(characterFilepath + "jumping_right", MVC_FILEPATH,
                                               currentJumpingRightSprite, FILE_EXTENSION,
                                               renderer, &m_Texture);
            break;
        case JUMPINGLEFT:
            if (isLookingLeft)
                this->loader->loadActionSprite(characterFilepath + "jumping_left", MVC_FILEPATH,
                                               currentJumpingLeftSprite, FILE_EXTENSION,
                                               renderer, &m_Texture);
            else
                this->loader->loadActionSprite(characterFilepath + "jumping_left_inverted", MVC_FILEPATH,
                                               currentJumpingLeftSprite, FILE_EXTENSION,
                                               renderer, &m_Texture);
            break;

        case MAKINGINTRO:
            this->loader->loadActionSprite(characterFilepath + "intro", MVC_FILEPATH, currentIntroSprite, FILE_EXTENSION,
                                           renderer, &m_Texture);
            break;
        case MOVING:
            if (isLookingLeft)
                this->loader->loadActionSprite(characterFilepath + "walking_left", MVC_FILEPATH,
                                               currentWalkingSprite, FILE_EXTENSION,
                                               renderer, &m_Texture);
            else
                this->loader->loadActionSprite(characterFilepath + "walking_right", MVC_FILEPATH,
                                               currentWalkingSprite, FILE_EXTENSION,
                                               renderer, &m_Texture);
            break;
        case WALKBACK:
            if(isLookingLeft)

                this->loader->loadActionSprite(characterFilepath + "walkbackwards_left", MVC_FILEPATH,
                                               currentWalkbackSprite, FILE_EXTENSION, renderer, &m_Texture);
            else
                this->loader->loadActionSprite(characterFilepath + "walkbackwards_right", MVC_FILEPATH,
                                               currentWalkbackSprite, FILE_EXTENSION, renderer, &m_Texture);
            break;

    }

}

void Wolverine::resetSpriteVariables() {
    mPosY = this->INITIAL_POS_Y;
    currentJumpingSprite = 0;
    currentWalkbackSprite = 0;
    currentJumpingRightSprite = 0;
    currentJumpingLeftSprite = 0;
    currentWalkingSprite = 0;
}

void Wolverine::renderDuckSprite() {
    currentAction = DUCK;
}

void Wolverine::stand() {
    currentAction = STANDING;
    this->resetSpriteVariables();
    if (currentStandingSprite >= LAST_STANDING_SPRITE)
        currentStandingSprite = 0;
}



void Wolverine::moveLeft(int distance, int posContrincante) {
    currentAction = MOVING;

    //Mover
    mPosX -= CHARACTER_VEL;

    if ((mPosX - CHARACTER_VEL <= -Wolverine::getSobrante()) || (distance < (-anchoPantalla))) {
        isLookingLeft = false;
        //Move back
        mPosX += CHARACTER_VEL;
    }

    if (this->getCentro() > posContrincante) {
        walkingSpriteUpdate();
    } else {
        if (currentWalkbackSprite > LAST_WALKBACK_SPRITE)
            currentWalkbackSprite = 0;

        ++currentWalkbackSprite;
        isLookingLeft = false;
    }
}

void Wolverine::moveRight(int distance, int posContrincante) {
    currentAction = MOVING;

    //Mover
    mPosX += CHARACTER_VEL;

    if ((mPosX + CHARACTER_VEL >= (LEVEL_WIDTH - Wolverine::getSobrante() - Wolverine::getWidth())) ||
        (distance > anchoPantalla)) {
        isLookingLeft = true;
        //Move back
        mPosX -= CHARACTER_VEL;
    }

    if (this->getCentro() < posContrincante) {
        walkingSpriteUpdate();
    } else {
        if (currentWalkbackSprite > LAST_WALKBACK_SPRITE)
            currentWalkbackSprite = 0;

        ++currentWalkbackSprite;
        isLookingLeft = true;
    }

}

void Wolverine::walkingSpriteUpdate() {

    if (currentWalkingSprite > LAST_WALKING_SPRITE)
        currentWalkingSprite = 0;

    ++currentWalkingSprite;
}


void Wolverine::jump(int *currentSprite, int lastSprite) {

    *currentSprite < 10 ? (mPosY -= 3 * CHARACTER_VEL) : (mPosY += 3 * CHARACTER_VEL);
    (*currentSprite)++;
    if (*currentSprite > lastSprite) {
        *currentSprite = 0;
        mPosY = this->INITIAL_POS_Y;
        this->currentAction = STANDING;
        currentStandingSprite = 0;
    }
}

void Wolverine::jumpVertical() {
    this->currentAction = JUMPINGVERTICAL;
    jump(&currentJumpingSprite, LAST_JUMPING_SPRITE);
}

void Wolverine::jumpRight() {
    this->currentAction = JUMPINGRIGHT;
    jump(&currentJumpingRightSprite, LAST_JUMPING_RIGHT_SPRITE);

}


void Wolverine::jumpLeft() {
    this->currentAction = JUMPINGLEFT;
    jump(&currentJumpingRightSprite, LAST_JUMPING_LEFT_SPRITE);
}




void Wolverine::makeIntro(){

    currentAction=MAKINGINTRO;

	unsigned int currentTime = SDL_GetTicks();


	if( currentIntroSprite < LAST_INTRO_SPRITE){
		++currentIntroSprite;
		lastTime = currentTime;
	}

	if(currentIntroSprite >= LAST_INTRO_SPRITE && (currentTime - lastTime) > 500){
		currentIntroSprite = 0;
		currentAction = STANDING;
        currentStandingSprite = 0;
    }
}

void Wolverine::updateStand() {
    if (currentStandingSprite <= LAST_STANDING_SPRITE)
        currentStandingSprite++;
}
