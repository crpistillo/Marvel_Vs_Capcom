//
// Created by amaherok on 3/28/19.
//

#include "./Spiderman.h"
#include "../InputTable.h"

using namespace std;

const int LEVEL_WIDTH = 3200;
const int LEVEL_HEIGHT = 600;

//Por defecto, todos los sprites comienzan en 0.
const int LAST_STANDING_SPRITE = 8;
const int LAST_WALKING_SPRITE = 10;
const int LAST_JUMPING_SPRITE = 7;
const int LAST_JUMPING_RIGHT_SPRITE = 12;
const int LAST_JUMPING_LEFT_SPRITE = 12;
const int LAST_INTRO_SPRITE = 16;

const unsigned int SECONDARY_RED = 255;
const unsigned int SECONDARY_GREEN = 200;
const unsigned int SECONDARY_BLUE = 000;

const string MVC_FILEPATH = "/MVC2_SpiderMan_";
const string FILE_EXTENSION = ".png";

Spiderman::Spiderman(int PosX, bool secondaryColor, int width, int height, int sobrante, int ancho)
: Character(
	PosX,
	556-(height*297/480),
	ancho,
	sobrante,
	false,
	width,
	height
){
	if(secondaryColor)
		loader = new ImageLoader(SECONDARY_RED, SECONDARY_GREEN, SECONDARY_BLUE);
	else
		loader = new ImageLoader(255,255,255);
}

Spiderman::~Spiderman(){
}

void Spiderman::load(SDL_Renderer* renderer) {
	this->loader->loadActionSprite(characterFilepath + "standing_right", MVC_FILEPATH, 0, FILE_EXTENSION,
	                                          renderer, &m_Texture);
}

void Spiderman::resetSpriteVariables() {
	mPosY = this->INITIAL_POS_Y;
	currentJumpingSprite = 0;
	currentWalkingRightSprite = 0;
	currentWalkingLeftSprite = 0;
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
		currentStandingSprite = 0;

	if (position == 'r'){
		this->loader->loadActionSprite(characterFilepath + "standing_right", MVC_FILEPATH, currentStandingSprite, FILE_EXTENSION,
			                                          renderer, &m_Texture);
	}
	else{
		this->loader->loadActionSprite(characterFilepath + "standing_left", MVC_FILEPATH, currentStandingSprite, FILE_EXTENSION,
					                                          renderer, &m_Texture);
	}
}

void Spiderman::renderDuckSprite(SDL_Renderer *renderer) {
	isStanding = false;
	if (isLookingLeft){
		this->loader->loadActionSprite(characterFilepath + "duck", MVC_FILEPATH, 220, FILE_EXTENSION,
							                                          renderer, &m_Texture);
	}else{
		this->loader->loadActionSprite(characterFilepath + "duck", MVC_FILEPATH, 219, FILE_EXTENSION,
									                                          renderer, &m_Texture);
	}
}

void Spiderman::moveLeft(SDL_Renderer *renderer, int distance, int posContrincante) {

	isStanding = false;
    isLookingLeft = true;

    //Mover
    mPosX -= CHARACTER_VEL;

    /*distance va de -800 a 800 (ancho de la pantalla)*/
    if ((mPosX - CHARACTER_VEL <= -Spiderman::getSobrante()) || (distance < (-800))) {
    	isLookingLeft = false;
        //Move back
        mPosX += CHARACTER_VEL;
    }


    if (this->getCentro() > posContrincante){
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

    if ((mPosX + CHARACTER_VEL >= (LEVEL_WIDTH - Spiderman::getSobrante() - Spiderman::getWidth())) || (distance > 800)) {
    	isLookingLeft = true;
        //Move back
        mPosX -= CHARACTER_VEL;
    }

    if (this->getCentro() < posContrincante){
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
        currentWalkingRightSprite = 0;
    }

	this->loader->loadActionSprite(characterFilepath + "walking_right", MVC_FILEPATH, currentWalkingRightSprite, FILE_EXTENSION,
										                                          renderer, &m_Texture);
	++currentWalkingRightSprite;
}

void Spiderman::animacionLeft(SDL_Renderer *renderer){
	string imagePath;

    if (currentWalkingLeftSprite > LAST_WALKING_SPRITE) {
        currentWalkingLeftSprite = 0;
    }

    this->loader->loadActionSprite(characterFilepath + "walking_left", MVC_FILEPATH, currentWalkingLeftSprite, FILE_EXTENSION,
    										                                          renderer, &m_Texture);
    ++currentWalkingLeftSprite;
}

void Spiderman::jump(SDL_Renderer *renderer) {
	isStanding = false;
	isJumpingVertical = true;
	string imagePath;

	if(isLookingLeft){

		this->loader->loadActionSprite(characterFilepath + "jumping_inverted", MVC_FILEPATH, currentJumpingSprite, FILE_EXTENSION,
						renderer, &m_Texture);

	}
	else{

		this->loader->loadActionSprite(characterFilepath + "jumping", MVC_FILEPATH, currentJumpingSprite, FILE_EXTENSION,
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
        currentJumpingSprite = 0;
        mPosY = this->INITIAL_POS_Y;
        isStanding = true;
        isJumpingVertical = false;
    }
}

void Spiderman::jumpRight(SDL_Renderer *renderer){
	isStanding = false;
	isJumpingRight = true;

	string imagePath;

	if(isLookingLeft){
		this->loader->loadActionSprite(characterFilepath + "jumping_right_inverted", MVC_FILEPATH, currentJumpingRightSprite, FILE_EXTENSION,
				renderer, &m_Texture);
	}

	else{
		this->loader->loadActionSprite(characterFilepath + "jumping_right", MVC_FILEPATH, currentJumpingRightSprite, FILE_EXTENSION,
				renderer, &m_Texture);
	}

	this->repositionHeightAfterJump('r');

	++currentJumpingRightSprite;

	if (currentJumpingRightSprite > LAST_JUMPING_RIGHT_SPRITE) {	//Hasta que no termine de saltar, no cambio los booleanos.
		currentJumpingRightSprite = 0;
	    mPosY = this->INITIAL_POS_Y;

	    isStanding = true;
	    isJumpingRight = false;
	}
}

void Spiderman::jumpLeft(SDL_Renderer* renderer){

	isStanding = false;
	isJumpingLeft = true;

	if(isLookingLeft){

		this->loader->loadActionSprite(characterFilepath + "jumping_left", MVC_FILEPATH, currentJumpingLeftSprite, FILE_EXTENSION,
						renderer, &m_Texture);
	}

	else{

		this->loader->loadActionSprite(characterFilepath + "jumping_left_inverted", MVC_FILEPATH, currentJumpingLeftSprite, FILE_EXTENSION,
						renderer, &m_Texture);
	}

	this->repositionHeightAfterJump('l');

	++currentJumpingLeftSprite;

	if (currentJumpingLeftSprite > LAST_JUMPING_LEFT_SPRITE) {	//Hasta que no termine de saltar, no cambio los booleanos.
		currentJumpingLeftSprite = 0;
	    mPosY = this->INITIAL_POS_Y;

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
            this->loader->loadActionSprite(characterFilepath + "intro", "/MVC2_SpiderManR_", currentIntroSprite, FILE_EXTENSION,
                                          renderer, &m_Texture);
        else
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

void Spiderman::repositionHeightAfterJump(char direction){

	if(direction == 'r'){
		if (currentJumpingRightSprite < ((LAST_JUMPING_RIGHT_SPRITE) / 2)) {
			mPosY -= CHARACTER_VEL;
		}
		else{
			mPosY += CHARACTER_VEL;
		}
	}
	else{
		if (currentJumpingLeftSprite < ((LAST_JUMPING_LEFT_SPRITE) / 2)) {
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
