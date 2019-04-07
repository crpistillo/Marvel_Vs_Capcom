//
// Created by amaherok on 3/31/19.
//

#include "./Wolverine.h"
#include "../InputTable.h"

using namespace std;

const int LEVEL_WIDTH = 3200;
const int LEVEL_HEIGHT = 600;

const int INITIAL_POS_Y = 20;

const int FIRST_STANDING_SPRITE = 21;
const int LAST_STANDING_SPRITE = 37;

const int FIRST_WALKING_SPRITE = 39;
const int LAST_WALKING_SPRITE = 54;

const int FIRST_JUMPING_SPRITE = 79;
const int LAST_JUMPING_SPRITE = 86;

const int FIRST_JUMPING_RIGHT_SPRITE = 85;
const int LAST_JUMPING_RIGHT_SPRITE = 98;

const int FIRST_JUMPING_LEFT_SPRITE = 85;
const int LAST_JUMPING_LEFT_SPRITE = 98;

const int FIRST_WALKBACK_SPRITE = 55;
const int LAST_WALKBACK_SPRITE = 70;

Wolverine::Wolverine(string name, int PosX)
: Character(
	PosX,
	INITIAL_POS_Y,
	FIRST_WALKING_SPRITE,
	FIRST_WALKING_SPRITE,
	FIRST_STANDING_SPRITE,
	FIRST_JUMPING_SPRITE,
	FIRST_JUMPING_RIGHT_SPRITE,
	FIRST_JUMPING_LEFT_SPRITE,
	FIRST_WALKBACK_SPRITE,
	FIRST_WALKBACK_SPRITE,
	true,
	name,
	KEY_W,
	KEY_S,
	KEY_D,
	KEY_A
)
{
}

void Wolverine::load(SDL_Renderer* renderer) {
	m_Texture.loadFromFile("images/wolverine/wolverine_stand_right.png", renderer);
}

void Wolverine::resetSpriteVariables() {
	mPosY = INITIAL_POS_Y;
	currentJumpingSprite = FIRST_JUMPING_SPRITE;
	currentWalkingRightSprite = FIRST_WALKING_SPRITE;
	currentWalkingLeftSprite = FIRST_WALKING_SPRITE;
	currentWalkbackRightSprite = FIRST_WALKBACK_SPRITE;
	currentWalkbackLeftSprite = FIRST_WALKBACK_SPRITE;
}

void Wolverine::renderDuckSprite(SDL_Renderer *renderer) {
	isStanding = false;
	if (isLookingLeft){
		m_Texture.loadFromFile("images/wolverine/wolverine_duck/MVC2_Wolverine_38_left.png", renderer);
	}else{
		m_Texture.loadFromFile("images/wolverine/wolverine_duck/MVC2_Wolverine_38.png", renderer);
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

void Wolverine::loadStandSprite(SDL_Renderer *renderer, char position){
	string imagePath;
	if (currentStandingSprite > LAST_STANDING_SPRITE)
		currentStandingSprite = FIRST_STANDING_SPRITE;

	if (position == 'r')
		imagePath = "images/wolverine/wolverine_standing_right/MVC2_Wolverine_" + to_string(currentStandingSprite) + ".png";
	else
		imagePath = "images/wolverine/wolverine_standing_left/MVC2_Wolverine_" + to_string(currentStandingSprite) + ".png";
	m_Texture.loadFromFile(imagePath, renderer);

}

void Wolverine::moveLeft(SDL_Renderer *renderer, int distance, int posContrincante) {
	isStanding = false;
    isLookingLeft = true;

    //Mover
    mPosX -= CHARACTER_VEL;

    if((mPosX - CHARACTER_VEL <= -Wolverine::SOBRANTE) || (distance < (-600))){
    	isLookingLeft = false;
        //Move back
        mPosX += CHARACTER_VEL;
	}

    if (Wolverine::mPosX > posContrincante){
    	/*Esta función animación la cree para Spiderman ya que la animación donde
    	* camina para atras es la misma que la de para adelante, pero haciendo eso
    	* con la forma de caminar de Wolverine no queda muy bien, por lo que esta
    	* función si bien puede quedar no es necesaria. Particularmente esta de
    	* abajo hace lo que corresponde aca.*/
    	animacionLeft(renderer);
    }
    else {
        if (currentWalkbackRightSprite > LAST_WALKBACK_SPRITE) {
            currentWalkbackRightSprite = FIRST_WALKBACK_SPRITE;
        }

        string imagePath = "images/wolverine/wolverine_walkbackwards_right/MVC2_Wolverine_" + to_string(currentWalkbackRightSprite) + ".png";
        m_Texture.loadFromFile(imagePath, renderer);
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

    if((mPosX + CHARACTER_VEL >= (LEVEL_WIDTH-Wolverine::SOBRANTE-Wolverine::CHARACTER_WIDTH)) || (distance > 600)) {
		isLookingLeft = true;
        //Move back
        mPosX -= CHARACTER_VEL;
	}

    if (Wolverine::mPosX < posContrincante){
    	animacionRight(renderer);
    }
    else {
        if (currentWalkbackRightSprite > LAST_WALKBACK_SPRITE) {
            currentWalkbackRightSprite = FIRST_WALKBACK_SPRITE;
        }

        string imagePath = "images/wolverine/wolverine_walkbackwards_left/MVC2_Wolverine_" + to_string(currentWalkbackRightSprite) + ".png";
        m_Texture.loadFromFile(imagePath, renderer);
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

void Wolverine::animacionRight(SDL_Renderer *renderer){
	string imagePath;

    if (currentWalkingRightSprite > LAST_WALKING_SPRITE) {
        currentWalkingRightSprite = FIRST_WALKING_SPRITE;
    }

    imagePath = "images/wolverine/wolverine_walking_right/MVC2_Wolverine_" + to_string(currentWalkingRightSprite) + ".png";
    m_Texture.loadFromFile(imagePath, renderer);
    ++currentWalkingRightSprite;
}

void Wolverine::animacionLeft(SDL_Renderer *renderer){
	string imagePath;

    if (currentWalkingLeftSprite > LAST_WALKING_SPRITE) {
        currentWalkingLeftSprite = FIRST_WALKING_SPRITE;
    }

    imagePath = "images/wolverine/wolverine_walking_left/MVC2_Wolverine_" + to_string(currentWalkingLeftSprite) + ".png";
    m_Texture.loadFromFile(imagePath, renderer);
    ++currentWalkingLeftSprite;
}

void Wolverine::jump(SDL_Renderer *renderer) {
	isStanding = false;
	isJumpingVertical = true;
	string imagePath;

	if(isLookingLeft){
		imagePath = "images/wolverine/wolverine_jumping_inverted/MVC2_Wolverine_" + to_string(currentJumpingSprite) + ".png";
		m_Texture.loadFromFile(imagePath, renderer);
	}
	else{
		imagePath = "images/wolverine/wolverine_jumping/MVC2_Wolverine_" + to_string(currentJumpingSprite) + ".png";
		m_Texture.loadFromFile(imagePath, renderer);
	}

    if (currentJumpingSprite < 83) {
        mPosY -= CHARACTER_VEL;
    }
    if (currentJumpingSprite >= 83) {
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

void Wolverine::jumpRight(SDL_Renderer *renderer){
	isStanding = false;
	isJumpingRight = true;

	string imagePath;

	if(isLookingLeft){
		imagePath = "images/wolverine/wolverine_jumping_right_inverted/MVC2_Wolverine_" + to_string(currentJumpingRightSprite) + "a" + ".png";
		m_Texture.loadFromFile(imagePath, renderer);
	}

	else{
		imagePath = "images/wolverine/wolverine_jumping_right/MVC2_Wolverine_" + to_string(currentJumpingRightSprite) + ".png";
		m_Texture.loadFromFile(imagePath, renderer);
	}

	this->repositionHeightAfterJump('r');

	++currentJumpingRightSprite;

	if (currentJumpingRightSprite > LAST_JUMPING_RIGHT_SPRITE) {	//Hasta que no termine de saltar, no cambio los booleanos.
		currentJumpingRightSprite = FIRST_JUMPING_RIGHT_SPRITE;
	    mPosY = INITIAL_POS_Y;

	    isStanding = true;
	    isJumpingRight = false;
	}}

void Wolverine::jumpLeft(SDL_Renderer *renderer){
	isStanding = false;
	isJumpingLeft = true;

	string imagePath;

	if(isLookingLeft){
		imagePath = "images/wolverine/wolverine_jumping_left/MVC2_Wolverine_" + to_string(currentJumpingLeftSprite) + ".png";
		m_Texture.loadFromFile(imagePath, renderer);
	}

	else{
		imagePath = "images/wolverine/wolverine_jumping_left_inverted/MVC2_Wolverine_" + to_string(currentJumpingLeftSprite) + "a" + ".png";
		m_Texture.loadFromFile(imagePath, renderer);
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

void Wolverine::repositionHeightAfterJump(char direction){

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

void Wolverine::updateStand() {
    if (currentStandingSprite <= LAST_STANDING_SPRITE)
        currentStandingSprite++;
}
