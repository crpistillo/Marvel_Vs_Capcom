//
// Created by amaherok on 3/31/19.
//

#include "./Wolverine.h"
#include "../InputTable.h"

using namespace std;

const int LEVEL_WIDTH = 3200;
const int LEVEL_HEIGHT = 600;

//Por defecto, todos los sprites comienzan en 0.
const int INITIAL_POS_Y = 20;
const int LAST_STANDING_SPRITE = 16;
const int LAST_WALKING_SPRITE = 15;
const int LAST_JUMPING_SPRITE = 7;
const int LAST_JUMPING_RIGHT_SPRITE = 13;
const int LAST_JUMPING_LEFT_SPRITE = 13;
const int LAST_WALKBACK_SPRITE = 15;
const int LAST_INTRO_SPRITE = 25;

const unsigned int SECONDARY_RED = 255;
const unsigned int SECONDARY_GREEN = 255;
const unsigned int SECONDARY_BLUE = 0;

Wolverine::Wolverine(string name, int PosX, bool secondaryColor)
        : Character(
        	PosX,
        	INITIAL_POS_Y,
			Wolverine::CHARACTER_WIDTH,
			Wolverine::SOBRANTE,
        	false,
        	name
) {
	if(secondaryColor)
		this->loader = new ImageLoader(SECONDARY_RED, SECONDARY_GREEN, SECONDARY_BLUE);
	else
		this->loader = new ImageLoader(255,255,255);
}

void Wolverine::load(SDL_Renderer *renderer) {
	this->loader->loadActionSprite("images/wolverine/wolverine_standing_right/", "MVC2_Wolverine_", 0, ".png", renderer, &m_Texture);
}

void Wolverine::resetSpriteVariables() {
    mPosY = INITIAL_POS_Y;
    currentJumpingSprite = 0;
    currentWalkingRightSprite = 0;
    currentWalkingLeftSprite = 0;
    currentWalkbackRightSprite = 0;
    currentWalkbackLeftSprite = 0;
}

void Wolverine::renderDuckSprite(SDL_Renderer *renderer) {
    isStanding = false;
    if (isLookingLeft) {
    	this->loader->loadActionSprite("images/wolverine/wolverine_duck/", "MVC2_Wolverine_", 38, "_left.png", renderer, &m_Texture);
    } else {
    	this->loader->loadActionSprite("images/wolverine/wolverine_duck/", "MVC2_Wolverine_", 38, ".png", renderer, &m_Texture);
    }
}

void Wolverine::renderStandSprite(SDL_Renderer *renderer) {
    isStanding = true;
    this->resetSpriteVariables();
    if (isLookingLeft) {
        this->loadStandSprite(renderer, 'l');
    } else {
        this->loadStandSprite(renderer, 'r');
    }
}

void Wolverine::loadStandSprite(SDL_Renderer *renderer, char position) {
    string imagePath;
    if (currentStandingSprite > LAST_STANDING_SPRITE)
        currentStandingSprite = 0;

    if (position == 'r'){
    	this->loader->loadActionSprite("images/wolverine/wolverine_standing_right/", "MVC2_Wolverine_", currentStandingSprite, ".png", renderer, &m_Texture);
    }
    else
    	this->loader->loadActionSprite("images/wolverine/wolverine_standing_left/", "MVC2_Wolverine_", currentStandingSprite, ".png", renderer, &m_Texture);
}

void Wolverine::moveLeft(SDL_Renderer *renderer, int distance, int posContrincante) {
    isStanding = false;
    isLookingLeft = true;

    //Mover
    mPosX -= CHARACTER_VEL;

    if ((mPosX - CHARACTER_VEL <= -Wolverine::SOBRANTE) || (distance < (-800))) {
        isLookingLeft = false;
        //Move back
        mPosX += CHARACTER_VEL;
    }

    if (Wolverine::mPosX > posContrincante) {
        /*Esta función animación la cree para Spiderman ya que la animación donde
        * camina para atras es la misma que la de para adelante, pero haciendo eso
        * con la forma de caminar de Wolverine no queda muy bien, por lo que esta
        * función si bien puede quedar no es necesaria. Particularmente esta de
        * abajo hace lo que corresponde aca.*/
        animacionLeft(renderer);
    } else {
        if (currentWalkbackRightSprite > LAST_WALKBACK_SPRITE) {
            currentWalkbackRightSprite = 0;
        }

        this->loader->loadActionSprite("images/wolverine/wolverine_walkbackwards_right/", "MVC2_Wolverine_",
        		currentWalkbackRightSprite, ".png", renderer, &m_Texture);
        ++currentWalkbackRightSprite;
        isLookingLeft = false;
    }

    //If the dot is inside the screen move
    /*if(mPosX - DOT_VEL > 0) {
        mPosX -= DOT_VEL;
    }*/

//-3000
    //If the dot went too far to the left or right
    //if ((mPosX < -900) || (mPosX + 157 > LEVEL_WIDTH)) {
    /*if (mPosX < -900) {
        //Move back
        mPosX += CHARACTER_VEL;
    }*/
}

void Wolverine::moveRight(SDL_Renderer *renderer, int distance, int posContrincante) {
    isStanding = false;
    isLookingLeft = false;

    //Mover
    mPosX += CHARACTER_VEL;

    if ((mPosX + CHARACTER_VEL >= (LEVEL_WIDTH - Wolverine::SOBRANTE - Wolverine::CHARACTER_WIDTH)) ||
        (distance > 800)) {
        isLookingLeft = true;
        //Move back
        mPosX -= CHARACTER_VEL;
    }

    if (Wolverine::mPosX<posContrincante) {
        animacionRight(renderer);
    } else {
        if (currentWalkbackRightSprite > LAST_WALKBACK_SPRITE) {
            currentWalkbackRightSprite = 0;
        }

        this->loader->loadActionSprite("images/wolverine/wolverine_walkbackwards_left/", "MVC2_Wolverine_",
                		currentWalkbackRightSprite, ".png", renderer, &m_Texture);
        ++currentWalkbackRightSprite;
        isLookingLeft = true;
    }

    //If the dot is inside the screen move
    /*if( mPosX + DOT_VEL < LEVEL_WIDTH ) {
        mPosX += DOT_VEL;
    }*/


    //Para que los personajes no se vayan del camino.png
    //502 es la distancia desde el comienzo de la imagen hasta el personaje
    //y 157 el ancho del personaje
    // if ((mPosX < -502) || (mPosX + 502 + 157 > LEVEL_WIDTH)) {
    /*if (mPosX + 502 + 157 > LEVEL_WIDTH) {
        //Move back
        mPosX -= CHARACTER_VEL;
    }*/

}

void Wolverine::animacionRight(SDL_Renderer *renderer) {
    string imagePath;

    if (currentWalkingRightSprite > LAST_WALKING_SPRITE) {
        currentWalkingRightSprite = 0;
    }

    this->loader->loadActionSprite("images/wolverine/wolverine_walking_right/", "MVC2_Wolverine_",
    		currentWalkingRightSprite, ".png", renderer, &m_Texture);
    ++currentWalkingRightSprite;
}

void Wolverine::animacionLeft(SDL_Renderer *renderer) {
    string imagePath;

    if (currentWalkingLeftSprite > LAST_WALKING_SPRITE) {
        currentWalkingLeftSprite = 0;
    }

    this->loader->loadActionSprite("images/wolverine/wolverine_walking_left/", "MVC2_Wolverine_",
        		currentWalkingLeftSprite, ".png", renderer, &m_Texture);

    ++currentWalkingLeftSprite;
}

void Wolverine::jump(SDL_Renderer *renderer) {
    isStanding = false;
    isJumpingVertical = true;
    string imagePath;

    if (isLookingLeft) {

        this->loader->loadActionSprite("images/wolverine/wolverine_jumping_inverted/", "MVC2_Wolverine_",
                		currentJumpingSprite, ".png", renderer, &m_Texture);
    } else {

        this->loader->loadActionSprite("images/wolverine/wolverine_jumping/", "MVC2_Wolverine_",
                        		currentJumpingSprite, ".png", renderer, &m_Texture);
    }

    if (currentJumpingSprite < 83) {
        mPosY -= CHARACTER_VEL;
    }
    if (currentJumpingSprite >= 83) {
        mPosY += CHARACTER_VEL;
    }

    ++currentJumpingSprite;

    if (currentJumpingSprite > LAST_JUMPING_SPRITE) {
        currentJumpingSprite = 0;
        mPosY = INITIAL_POS_Y;

        isStanding = true;
        isJumpingVertical = false;
    }
}

void Wolverine::jumpRight(SDL_Renderer *renderer) {
    isStanding = false;
    isJumpingRight = true;

    string imagePath;

    if (isLookingLeft) {

        this->loader->loadActionSprite("images/wolverine/wolverine_jumping_right_inverted/", "MVC2_Wolverine_",
                                		currentJumpingRightSprite, "a.png", renderer, &m_Texture);
    } else {
        this->loader->loadActionSprite("images/wolverine/wolverine_jumping_right/", "MVC2_Wolverine_",
                                        		currentJumpingRightSprite, ".png", renderer, &m_Texture);
    }

    this->repositionHeightAfterJump('r');

    ++currentJumpingRightSprite;

    if (currentJumpingRightSprite >
        LAST_JUMPING_RIGHT_SPRITE) {    //Hasta que no termine de saltar, no cambio los booleanos.
        currentJumpingRightSprite = 0;
        mPosY = INITIAL_POS_Y;

        isStanding = true;
        isJumpingRight = false;
    }
}

void Wolverine::jumpLeft(SDL_Renderer *renderer) {
    isStanding = false;
    isJumpingLeft = true;

    string imagePath;

    if (isLookingLeft) {
        this->loader->loadActionSprite("images/wolverine/wolverine_jumping_left/", "MVC2_Wolverine_",
                                                		currentJumpingLeftSprite, ".png", renderer, &m_Texture);
    } else {
        this->loader->loadActionSprite("images/wolverine/wolverine_jumping_left_inverted/", "MVC2_Wolverine_",
                                                		currentJumpingLeftSprite, "a.png", renderer, &m_Texture);
    }


    this->repositionHeightAfterJump('l');

    ++currentJumpingLeftSprite;

    if (currentJumpingLeftSprite >
        LAST_JUMPING_LEFT_SPRITE) {    //Hasta que no termine de saltar, no cambio los booleanos.
        currentJumpingLeftSprite = 0;
        mPosY = INITIAL_POS_Y;

        isStanding = true;
        isJumpingLeft = false;
    }
}

void Wolverine::makeIntro(SDL_Renderer* renderer){
	isMakingIntro = true;
	isStanding = false;



	unsigned int currentTime = SDL_GetTicks();

	if( currentIntroSprite <= LAST_INTRO_SPRITE){
		this->loader->loadActionSprite("images/wolverine/wolverine_intro/", "MVC2_Wolverine_", currentIntroSprite, ".png",
							renderer, &m_Texture);
		++currentIntroSprite;
		lastTime = currentTime;
	}

	if(currentIntroSprite > LAST_INTRO_SPRITE && (currentTime - lastTime) > 500){
		currentIntroSprite = 0;
		isMakingIntro = false;
		isStanding = true;
		}
}

void Wolverine::repositionHeightAfterJump(char direction) {

    if (direction == 'r') {
        if (currentJumpingRightSprite < (( LAST_JUMPING_RIGHT_SPRITE) / 2)) {
            mPosY -= CHARACTER_VEL;
        } else {
            mPosY += CHARACTER_VEL;
        }
    } else {
        if (currentJumpingLeftSprite < (( LAST_JUMPING_LEFT_SPRITE) / 2)) {
            mPosY -= CHARACTER_VEL;
        } else {
            mPosY += CHARACTER_VEL;
        }

    }
}

void Wolverine::updateStand() {
    if (currentStandingSprite <= LAST_STANDING_SPRITE)
        currentStandingSprite++;
}
