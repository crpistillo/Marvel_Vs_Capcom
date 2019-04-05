//
// Created by amaherok on 3/28/19.
//

#include "./Spiderman.h"
#include "../InputTable.h"

using namespace std;

const int LEVEL_WIDTH = 3200;
const int LEVEL_HEIGHT = 600;

/*1600 es el centro de la pantalla, 437 es la distancia desde el comienzo de la imagen hasta el personaje,
 * 99 es la mitad del ancho del personaje, y 200 es el corrimiento a la izquierda desde el centro*/
const int INITIAL_POS_X = (1600-437)-99-200;
const int INITIAL_POS_Y = 50;

const int FIRST_STANDING_SPRITE = 159;
const int LAST_STANDING_SPRITE = 167;

const int FIRST_WALKING_SPRITE = 170;
const int LAST_WALKING_SPRITE = 180;

const int FIRST_JUMPING_SPRITE = 189;
const int LAST_JUMPING_SPRITE = 196;

const int FIRST_JUMPING_RIGHT_SPRITE = 198;
const int LAST_JUMPING_RIGHT_SPRITE = 210;

Spiderman::Spiderman(string name)
: Character(
	INITIAL_POS_X,
	INITIAL_POS_Y,
	FIRST_WALKING_SPRITE,
	FIRST_WALKING_SPRITE,
	FIRST_STANDING_SPRITE,
	FIRST_JUMPING_SPRITE,
	FIRST_JUMPING_RIGHT_SPRITE,
	false,
	name,
	KEY_UP,
	KEY_DOWN,
	KEY_RIGHT,
	KEY_LEFT
)
{
}

void Spiderman::load(SDL_Renderer* renderer) {
	m_Texture.loadFromFile("images/spiderman_standing_right/MVC2_SpiderMan_159.png", renderer);
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
		imagePath = "images/spiderman_standing_right/MVC2_SpiderMan_" + to_string(currentStandingSprite) + ".png";
	else
		imagePath = "images/spiderman_standing_left/MVC2_SpiderMan_" + to_string(currentStandingSprite) + ".png";
	m_Texture.loadFromFile(imagePath, renderer);

}

void Spiderman::renderDuckSprite(SDL_Renderer *renderer) {
	isStanding = false;
	m_Texture.loadFromFile("images/MVC2_SpiderMan_219.png", renderer);
}

void Spiderman::moveLeft(SDL_Renderer *renderer, int distance, int posContrincante) {
	isStanding = false;
    isLookingLeft = true;

    //437 es la distancia desde el comienzo de la imagen hasta el personaje
    //La distancia es la distancia entre personajes.
    if ((mPosX - CHARACTER_VEL <= -437) || (distance < (-600))) {
    	isLookingLeft = false;
        return;
    }

    animacionLeft(renderer);

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

    //Move the dot left or right
    mPosX -= CHARACTER_VEL;

    //If the dot went too far to the left or right
    /*if (mPosX < -900) {
        //Move back
        mPosX += CHARACTER_VEL;
    }*/
}

void Spiderman::moveRight(SDL_Renderer *renderer, int distance, int posContrincante) {
	isStanding = false;
    isLookingLeft = false;

    //437 es la distancia desde el comienzo de la imagen hasta el personaje
    //198 es el ancho de spiderman y 75 es un numero magico que corrige una pequeño error
    if ((mPosX + CHARACTER_VEL >= (LEVEL_WIDTH - 437 - 198 - 75)) || (distance > 600)) {
    	isLookingLeft = true;
        return;
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

    //Move the dot left or right
    mPosX += CHARACTER_VEL;

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

	imagePath = "images/spiderman_walking_right/MVC2_SpiderMan_" + to_string(currentWalkingRightSprite) + ".png";
	m_Texture.loadFromFile(imagePath, renderer);
	++currentWalkingRightSprite;
}

void Spiderman::animacionLeft(SDL_Renderer *renderer){
	string imagePath;

    if (currentWalkingLeftSprite > LAST_WALKING_SPRITE) {
        currentWalkingLeftSprite = FIRST_WALKING_SPRITE;
    }

    imagePath = "images/spiderman_walking_left/MVC2_SpiderMan_" + to_string(currentWalkingLeftSprite) + ".png";
    m_Texture.loadFromFile(imagePath, renderer);
    ++currentWalkingLeftSprite;
}

void Spiderman::jump(SDL_Renderer *renderer) {
	isStanding = false;
    if (currentJumpingSprite > LAST_JUMPING_SPRITE) {
        currentJumpingSprite = FIRST_JUMPING_SPRITE;
        mPosY = INITIAL_POS_Y;
    }

    string imagePath = "images/spiderman_jumping/MVC2_SpiderMan_" + to_string(currentJumpingSprite) + ".png";
    m_Texture.loadFromFile(imagePath, renderer);

    if (currentJumpingSprite < 193) {
        mPosY -= CHARACTER_VEL;
    }
    if (currentJumpingSprite > 193) {
        mPosY += CHARACTER_VEL;
    }

    ++currentJumpingSprite;
}

void Spiderman::jumpRight(SDL_Renderer *renderer){
	isStanding = false;
	if (currentJumpingRightSprite > LAST_JUMPING_RIGHT_SPRITE) {
		currentJumpingRightSprite = FIRST_JUMPING_RIGHT_SPRITE;
	    mPosY = INITIAL_POS_Y;
	}

	string imagePath = "images/spiderman_jumping_right/MVC2_SpiderMan_" + to_string(currentJumpingRightSprite) + ".png";
	m_Texture.loadFromFile(imagePath, renderer);

	if (currentJumpingRightSprite < 204) {
		mPosY -= CHARACTER_VEL;
	}
	if (currentJumpingRightSprite >= 204) {
		mPosY += CHARACTER_VEL;
	}

	++currentJumpingRightSprite;
}



void Spiderman::updateStand() {
    if (currentStandingSprite <= LAST_STANDING_SPRITE)
        currentStandingSprite++;
}
