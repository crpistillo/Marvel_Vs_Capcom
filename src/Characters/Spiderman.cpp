//
// Created by amaherok on 3/28/19.
//

#include "./Spiderman.h"
#include "../InputTable.h"

using namespace std;

const int LEVEL_WIDTH = 3200;
const int LEVEL_HEIGHT = 600;

const int INITIAL_POS_Y = 20;

const int FIRST_STANDING_SPRITE = 0;
const int LAST_STANDING_SPRITE = 8;

const int FIRST_WALKING_SPRITE = 0;
const int LAST_WALKING_SPRITE = 10;

const int FIRST_JUMPING_SPRITE = 0;
const int LAST_JUMPING_SPRITE = 7;

const int FIRST_JUMPING_RIGHT_SPRITE = 0;
const int LAST_JUMPING_RIGHT_SPRITE = 12;

const int FIRST_JUMPING_LEFT_SPRITE = 0;
const int LAST_JUMPING_LEFT_SPRITE = 12;

const int FIRST_INTRO_SPRITE = 0;
const int LAST_INTRO_SPRITE = 16;

Spiderman::Spiderman(string name, int PosX)
: Character(
	PosX,
	INITIAL_POS_Y,
	FIRST_WALKING_SPRITE,
	FIRST_WALKING_SPRITE,
	FIRST_STANDING_SPRITE,
	FIRST_JUMPING_SPRITE,
	FIRST_JUMPING_RIGHT_SPRITE,
	FIRST_JUMPING_LEFT_SPRITE,
	FIRST_WALKING_SPRITE,
	FIRST_WALKING_SPRITE,
	FIRST_INTRO_SPRITE,
	false,
	name
)
{
}

void Spiderman::load(SDL_Renderer* renderer) {
	m_Texture.loadFromFile("images/spiderman/spiderman_standing_right/MVC2_SpiderMan_159.png", renderer);
}

void Spiderman::resetSpriteVariables() {
	mPosY = INITIAL_POS_Y;
	currentJumpingSprite = FIRST_JUMPING_SPRITE;
	currentWalkingRightSprite = FIRST_WALKING_SPRITE;
	currentWalkingLeftSprite = FIRST_WALKING_SPRITE;
}

void Spiderman::renderStandSprite(SDL_Renderer *renderer) {
	isStanding = true;
	this->resetSpriteVariables();
    if (isLookingLeft) {
    	this->loadStandSprite(renderer, 'l');
    } else {
    	this->loadStandSprite(renderer, 'r');
    }
}

void Spiderman::loadStandSprite(SDL_Renderer *renderer, char position){
	string imagePath;
	if (currentStandingSprite > LAST_STANDING_SPRITE)
		currentStandingSprite = FIRST_STANDING_SPRITE;

	if (position == 'r')
		imagePath = "images/spiderman/spiderman_standing_right/MVC2_SpiderMan_" + to_string(currentStandingSprite) + ".png";
	else
		imagePath = "images/spiderman/spiderman_standing_left/MVC2_SpiderMan_" + to_string(currentStandingSprite) + ".png";
	m_Texture.loadFromFile(imagePath, renderer);

}

void Spiderman::renderDuckSprite(SDL_Renderer *renderer) {
	isStanding = false;
	if (isLookingLeft){
		m_Texture.loadFromFile("images/spiderman/spiderman_duck/MVC2_SpiderMan_219_left.png", renderer);
	}else{
		m_Texture.loadFromFile("images/spiderman/spiderman_duck/MVC2_SpiderMan_219.png", renderer);
	}
}

void Spiderman::moveLeft(SDL_Renderer *renderer, int distance, int posContrincante) {

	isStanding = false;
    isLookingLeft = true;

    //Mover
    mPosX -= CHARACTER_VEL;

    /*distance es la distancia entre personajes la cual pensada como si fuesen puntos
     * iria de -600 a 600, pero como los personajes tienen ancho (y diferente ancho),
     * se ven numeros (como ese -375) que fueron sacados a ojo*/
    if ((mPosX - CHARACTER_VEL <= -Spiderman::SOBRANTE) || (distance < (-375))) {
    	isLookingLeft = false;
        //Move back
        mPosX += CHARACTER_VEL;
    }


    if (Spiderman::mPosX > posContrincante){
    	animacionLeft(renderer);
    }
    else {
    	animacionRight(renderer);
    	isLookingLeft = false;
    }

    //If the dot is inside the screen move
    /*if(mPosX - DOT_VEL > 0) {
        mPosX -= DOT_VEL;
    }*/



    //If the dot went too far to the left or right
    /*if (mPosX < -900) {
        //Move back
        mPosX += CHARACTER_VEL;
    }*/
}

void Spiderman::moveRight(SDL_Renderer *renderer, int distance, int posContrincante) {

	isStanding = false;
    isLookingLeft = false;

    //Mover
    mPosX += CHARACTER_VEL;

    if ((mPosX + CHARACTER_VEL >= (LEVEL_WIDTH - Spiderman::SOBRANTE - Spiderman::CHARACTER_WIDTH)) || (distance > 800)) {
    	isLookingLeft = true;
        //Move back
        mPosX -= CHARACTER_VEL;
    }

    if (Spiderman::mPosX < posContrincante){
    	animacionRight(renderer);
    }
    else {
    	animacionLeft(renderer);
    	isLookingLeft = true;
    }


    //If the dot is inside the screen move
    /*if( mPosX + DOT_VEL < LEVEL_WIDTH ) {
        mPosX += DOT_VEL;
    }*/


    //Para que los personajes no se vayan del camino.png
    //437 es la distancia desde el comienzo de la imagen hasta el personaje
    //y 198 el ancho del personaje
    //if ((mPosX < -437) || (mPosX + 437 + 198 > LEVEL_WIDTH)) {
    /*if (mPosX + 437 + 198 > LEVEL_WIDTH) {
        //Move back
        mPosX -= CHARACTER_VEL;
    }*/

}

void Spiderman::animacionRight(SDL_Renderer *renderer){
	string imagePath;

	if (currentWalkingRightSprite > LAST_WALKING_SPRITE) {
        currentWalkingRightSprite = FIRST_WALKING_SPRITE;
    }

	imagePath = "images/spiderman/spiderman_walking_right/MVC2_SpiderMan_" + to_string(currentWalkingRightSprite) + ".png";
	m_Texture.loadFromFile(imagePath, renderer);
	++currentWalkingRightSprite;
}

void Spiderman::animacionLeft(SDL_Renderer *renderer){
	string imagePath;

    if (currentWalkingLeftSprite > LAST_WALKING_SPRITE) {
        currentWalkingLeftSprite = FIRST_WALKING_SPRITE;
    }

    imagePath = "images/spiderman/spiderman_walking_left/MVC2_SpiderMan_" + to_string(currentWalkingLeftSprite) + ".png";
    m_Texture.loadFromFile(imagePath, renderer);
    ++currentWalkingLeftSprite;
}

void Spiderman::jump(SDL_Renderer *renderer) {
	isStanding = false;
	isJumpingVertical = true;
	string imagePath;

	if(isLookingLeft){

		this->loader.loadActionSprite("images/spiderman/spiderman_jumping_inverted/", "MVC2_SpiderMan_", currentJumpingSprite, ".png",
						renderer, &m_Texture);

	}
	else{

		this->loader.loadActionSprite("images/spiderman/spiderman_jumping/", "MVC2_SpiderMan_", currentJumpingSprite, ".png",
								renderer, &m_Texture);


	}

    if (currentJumpingSprite < 193) {
        mPosY -= CHARACTER_VEL;
    }
    if (currentJumpingSprite > 193) {
        mPosY += CHARACTER_VEL;
    }

    ++currentJumpingSprite;

    if (currentJumpingSprite > LAST_JUMPING_SPRITE) {
        currentJumpingSprite = FIRST_JUMPING_SPRITE;
        mPosY = INITIAL_POS_Y;
        isStanding = true;
        isJumpingVertical = false;
    }
}

void Spiderman::jumpRight(SDL_Renderer *renderer){
	isStanding = false;
	isJumpingRight = true;

	string imagePath;

	if(isLookingLeft){
		this->loader.loadActionSprite("images/spiderman/spiderman_jumping_right_inverted/", "MVC2_SpiderMan_", currentJumpingRightSprite, "a.png",
				renderer, &m_Texture);
	}

	else{
		this->loader.loadActionSprite("images/spiderman/spiderman_jumping_right/", "MVC2_SpiderMan_", currentJumpingRightSprite, ".png",
				renderer, &m_Texture);
	}

	this->repositionHeightAfterJump('r');

	++currentJumpingRightSprite;

	if (currentJumpingRightSprite > LAST_JUMPING_RIGHT_SPRITE) {	//Hasta que no termine de saltar, no cambio los booleanos.
		currentJumpingRightSprite = FIRST_JUMPING_RIGHT_SPRITE;
	    mPosY = INITIAL_POS_Y;

	    isStanding = true;
	    isJumpingRight = false;
	}
}

void Spiderman::jumpLeft(SDL_Renderer* renderer){

	isStanding = false;
	isJumpingLeft = true;

	if(isLookingLeft){

		this->loader.loadActionSprite("images/spiderman/spiderman_jumping_left/", "MVC2_SpiderMan_", currentJumpingLeftSprite, ".png",
						renderer, &m_Texture);
	}

	else{

		this->loader.loadActionSprite("images/spiderman/spiderman_jumping_left_inverted/", "MVC2_SpiderMan_", currentJumpingLeftSprite, "a.png",
						renderer, &m_Texture);
	}

	this->repositionHeightAfterJump('l');

	++currentJumpingLeftSprite;

	if (currentJumpingLeftSprite > LAST_JUMPING_LEFT_SPRITE) {	//Hasta que no termine de saltar, no cambio los booleanos.
		currentJumpingLeftSprite = FIRST_JUMPING_LEFT_SPRITE;
	    mPosY = INITIAL_POS_Y;

	    isStanding = true;
	    isJumpingLeft = false;
	}
}

void Spiderman::makeIntro(SDL_Renderer* renderer){

	isMakingIntro = true;
	isStanding = false;



	unsigned int currentTime = SDL_GetTicks();

	if(currentIntroSprite <= LAST_INTRO_SPRITE){
        if(!isLookingLeft && 10 <= currentIntroSprite && currentIntroSprite<= 16)
            this->loader.loadActionSprite("images/spiderman/spiderman_intro/", "MVC2_SpiderManR_", currentIntroSprite, ".png",
                                          renderer, &m_Texture);
        else
            this->loader.loadActionSprite("images/spiderman/spiderman_intro/", "MVC2_SpiderMan_", currentIntroSprite, ".png",
                                          renderer, &m_Texture);

		++currentIntroSprite;
		lastTime = currentTime;
	}


	if(currentIntroSprite > LAST_INTRO_SPRITE && (currentTime - lastTime) > 500){
		currentIntroSprite = FIRST_INTRO_SPRITE;

		isMakingIntro = false;
		isStanding = true;
	}

}

void Spiderman::repositionHeightAfterJump(char direction){

	if(direction == 'r'){
		if (currentJumpingRightSprite < ((FIRST_JUMPING_RIGHT_SPRITE + LAST_JUMPING_RIGHT_SPRITE) / 2)) {
			mPosY -= CHARACTER_VEL;
		}
		else{
			mPosY += CHARACTER_VEL;
		}
	}
	else{
		if (currentJumpingLeftSprite < ((FIRST_JUMPING_LEFT_SPRITE + LAST_JUMPING_LEFT_SPRITE) / 2)) {
			mPosY -= CHARACTER_VEL;
		}
		else{
			mPosY += CHARACTER_VEL;
		}

	}
}



void Spiderman::updateStand() {
    if (currentStandingSprite <= LAST_STANDING_SPRITE)
        currentStandingSprite++;
}
