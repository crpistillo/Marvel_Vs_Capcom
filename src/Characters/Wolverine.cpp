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
	this->loader->loadActionSprite(characterFilepath + "standing_right", MVC_FILEPATH, 0, FILE_EXTENSION, renderer, &m_Texture);
}

void Wolverine::resetSpriteVariables() {
    mPosY = this->INITIAL_POS_Y;
    currentJumpingSprite = 0;
    currentWalkingRightSprite = 0;
    currentWalkingLeftSprite = 0;
    currentWalkbackRightSprite = 0;
    currentWalkbackLeftSprite = 0;
}

void Wolverine::renderDuckSprite(SDL_Renderer *renderer) {
    agachado = true;
    if (isLookingLeft) {
    	this->loader->loadActionSprite(characterFilepath + "duck", MVC_FILEPATH, 39, FILE_EXTENSION, renderer, &m_Texture);
    } else {
    	this->loader->loadActionSprite(characterFilepath + "duck", MVC_FILEPATH, 38, FILE_EXTENSION, renderer, &m_Texture);
    }
}

void Wolverine::renderStandSprite(SDL_Renderer *renderer) {
    isStanding = true;
    agachado = false;
    this->resetSpriteVariables();
    if (isLookingLeft) {
        this->loadStandSprite(renderer);
    } else {
        this->loadStandSprite(renderer);
    }
}

void Wolverine::loadStandSprite(SDL_Renderer *renderer) {
    string imagePath;
    if (currentStandingSprite > LAST_STANDING_SPRITE)
        currentStandingSprite = 0;

    if (!isLookingLeft){
    	this->loader->loadActionSprite(characterFilepath + "standing_right", MVC_FILEPATH, currentStandingSprite, FILE_EXTENSION, renderer, &m_Texture);
    }
    else
    	this->loader->loadActionSprite(characterFilepath + "standing_left", MVC_FILEPATH, currentStandingSprite, FILE_EXTENSION, renderer, &m_Texture);
}

void Wolverine::moveLeft(SDL_Renderer *renderer, int distance, int posContrincante) {
    isStanding = false;
    isLookingLeft = true;

    //Mover
    mPosX -= CHARACTER_VEL;

    if ((mPosX - CHARACTER_VEL <= -Wolverine::getSobrante()) || (distance < (-anchoPantalla))) {
        isLookingLeft = false;
        //Move back
        mPosX += CHARACTER_VEL;
    }

    if (this->getCentro() > posContrincante) {
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

        this->loader->loadActionSprite(characterFilepath + "walkbackwards_right", MVC_FILEPATH,
        		currentWalkbackRightSprite, FILE_EXTENSION, renderer, &m_Texture);
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

    if ((mPosX + CHARACTER_VEL >= (LEVEL_WIDTH - Wolverine::getSobrante() - Wolverine::getWidth())) ||
        (distance > anchoPantalla)) {
        isLookingLeft = true;
        //Move back
        mPosX -= CHARACTER_VEL;
    }

    if (this->getCentro() < posContrincante) {
        animacionRight(renderer);
    } else {
        if (currentWalkbackRightSprite > LAST_WALKBACK_SPRITE) {
            currentWalkbackRightSprite = 0;
        }

        this->loader->loadActionSprite(characterFilepath + "walkbackwards_left", MVC_FILEPATH,
                		currentWalkbackRightSprite, FILE_EXTENSION, renderer, &m_Texture);
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

    this->loader->loadActionSprite(characterFilepath + "walking_right", MVC_FILEPATH,
    		currentWalkingRightSprite, FILE_EXTENSION, renderer, &m_Texture);
    ++currentWalkingRightSprite;
}

void Wolverine::animacionLeft(SDL_Renderer *renderer) {
    string imagePath;

    if (currentWalkingLeftSprite > LAST_WALKING_SPRITE) {
        currentWalkingLeftSprite = 0;
    }

    this->loader->loadActionSprite(characterFilepath + "walking_left", MVC_FILEPATH,
        		currentWalkingLeftSprite, FILE_EXTENSION, renderer, &m_Texture);

    ++currentWalkingLeftSprite;
}

void Wolverine::jump(SDL_Renderer *renderer) {
    isStanding = false;
    isJumpingVertical = true;
    string imagePath;

    if (isLookingLeft) {

        this->loader->loadActionSprite(characterFilepath + "jumping_inverted", MVC_FILEPATH,
                		currentJumpingSprite, FILE_EXTENSION, renderer, &m_Texture);
    } else {

        this->loader->loadActionSprite(characterFilepath + "jumping", MVC_FILEPATH,
                        		currentJumpingSprite, FILE_EXTENSION, renderer, &m_Texture);
    }

    if (currentJumpingSprite < 10) {
        mPosY -= 3*CHARACTER_VEL;
    }
    if (currentJumpingSprite >= 10) {
        mPosY += 3*CHARACTER_VEL;
    }

    ++currentJumpingSprite;

    if (currentJumpingSprite > LAST_JUMPING_SPRITE) {
        currentJumpingSprite = 0;
        mPosY = this->INITIAL_POS_Y;

        isStanding = true;
        isJumpingVertical = false;
    }
}

void Wolverine::jumpRight(SDL_Renderer *renderer) {
    isStanding = false;
    isJumpingRight = true;

    string imagePath;

    if (isLookingLeft) {

        this->loader->loadActionSprite(characterFilepath + "jumping_right_inverted", MVC_FILEPATH,
                                		currentJumpingRightSprite, FILE_EXTENSION, renderer, &m_Texture);
    } else {
        this->loader->loadActionSprite(characterFilepath + "jumping_right", MVC_FILEPATH,
                                        		currentJumpingRightSprite, FILE_EXTENSION, renderer, &m_Texture);
    }

    this->repositionHeightAfterJump('r');

    ++currentJumpingRightSprite;

    if (currentJumpingRightSprite >
        LAST_JUMPING_RIGHT_SPRITE) {    //Hasta que no termine de saltar, no cambio los booleanos.
        currentJumpingRightSprite = 0;
        mPosY = this->INITIAL_POS_Y;

        isStanding = true;
        isJumpingRight = false;
    }
}

void Wolverine::jumpLeft(SDL_Renderer *renderer) {
    isStanding = false;
    isJumpingLeft = true;

    string imagePath;

    if (isLookingLeft) {
        this->loader->loadActionSprite(characterFilepath + "jumping_left", MVC_FILEPATH,
                                                		currentJumpingLeftSprite, FILE_EXTENSION, renderer, &m_Texture);
    } else {
        this->loader->loadActionSprite(characterFilepath + "jumping_left_inverted", MVC_FILEPATH,
                                                		currentJumpingLeftSprite, FILE_EXTENSION, renderer, &m_Texture);
    }


    this->repositionHeightAfterJump('l');

    ++currentJumpingLeftSprite;

    if (currentJumpingLeftSprite >
        LAST_JUMPING_LEFT_SPRITE) {    //Hasta que no termine de saltar, no cambio los booleanos.
        currentJumpingLeftSprite = 0;
        mPosY = this->INITIAL_POS_Y;

        isStanding = true;
        isJumpingLeft = false;
    }
}

void Wolverine::makeIntro(SDL_Renderer* renderer){
	isMakingIntro = true;
	isStanding = false;



	unsigned int currentTime = SDL_GetTicks();

	if( currentIntroSprite <= LAST_INTRO_SPRITE){
		this->loader->loadActionSprite(characterFilepath + "intro", MVC_FILEPATH, currentIntroSprite, FILE_EXTENSION,
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
        if (currentJumpingRightSprite < 10) {
            mPosY -= 2.5*CHARACTER_VEL;
        } else {
            mPosY += 2.5*CHARACTER_VEL;
        }
    } else {
        if (currentJumpingLeftSprite < 10) {
            mPosY -= 2.5*CHARACTER_VEL;
        } else {
            mPosY += 2.5*CHARACTER_VEL;
        }

    }
}

void Wolverine::updateStand() {
    if (currentStandingSprite <= LAST_STANDING_SPRITE)
        currentStandingSprite++;
}
