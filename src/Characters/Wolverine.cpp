//
// Created by amaherok on 3/31/19.
//

#include "./Wolverine.h"
#include "../InputTable.h"

using namespace std;

const int LEVEL_WIDTH = 3200;
const int LEVEL_HEIGHT = 600;

const int INITIAL_POS_X = (1600-314)-43+200;
const int INITIAL_POS_Y = 270;

const int FIRST_STANDING_SPRITE = 21;
const int LAST_STANDING_SPRITE = 37;

const int FIRST_WALKING_SPRITE = 39;
const int LAST_WALKING_SPRITE = 54;

const int FIRST_JUMPING_SPRITE = 189;
const int LAST_JUMPING_SPRITE = 196;

Wolverine::Wolverine(string name)
: Character(
	INITIAL_POS_X,
	INITIAL_POS_Y,
	FIRST_WALKING_SPRITE,
	FIRST_WALKING_SPRITE,
	FIRST_STANDING_SPRITE,
	FIRST_JUMPING_SPRITE,
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
	m_Texture.loadFromFile("images/wolverine_stand_right.png", renderer);
}

void Wolverine::resetSpriteVariables() {
	mPosY = INITIAL_POS_Y;
	currentJumpingSprite = FIRST_JUMPING_SPRITE;
	currentWalkingRightSprite = FIRST_WALKING_SPRITE;
	currentWalkingLeftSprite = FIRST_WALKING_SPRITE;
}

void Wolverine::renderDuckSprite(SDL_Renderer *renderer) {
	isStanding = false;
	m_Texture.loadFromFile("images/MVC2_Wolverine_38.png", renderer);
}

void Wolverine::renderStandSprite(SDL_Renderer *renderer) {
	isStanding = true;
	this->resetSpriteVariables();
	 if (isLookingLeft) {
		 if (currentStandingSprite > LAST_STANDING_SPRITE)
		 	currentStandingSprite = FIRST_STANDING_SPRITE;
		string imagePath = "images/wolverine_standing_left/MVC2_Wolverine_" + to_string(currentStandingSprite) + ".png";
		m_Texture.loadFromFile(imagePath, renderer);
	} else {
		if (currentStandingSprite > LAST_STANDING_SPRITE)
			currentStandingSprite = FIRST_STANDING_SPRITE;
		string imagePath = "images/wolverine_standing_right/MVC2_Wolverine_" + to_string(currentStandingSprite) + ".png";
		m_Texture.loadFromFile(imagePath, renderer);
	}
}

void Wolverine::moveLeft(SDL_Renderer *renderer, int distance, int posContrincante) {
	isStanding = false;
    isLookingLeft = true;

    //Puse -320 en lugar de 0 porque la imagen del personaje es mas ancha que él.
    if((mPosX - CHARACTER_VEL <= -320) || (distance < (-600))){
    	isLookingLeft = false;
		return;
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
    	/*Esto debe ser corregido. Debe ir la animacion donde Wolverine camina hacia
    	 * atras mirando a la derecha.*/
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
    if ((mPosX < -3000) || (mPosX + CHARACTER_WIDTH > LEVEL_WIDTH)) {
        //Move back
        mPosX += CHARACTER_VEL;
    }
}

void Wolverine::moveRight(SDL_Renderer *renderer, int distance, int posContrincante) {
	isStanding = false;
    isLookingLeft = false;

	if((mPosX + CHARACTER_VEL >= (LEVEL_WIDTH-420)) || (distance > 600)) {
		isLookingLeft = true;
		return;
	}

    if (Wolverine::mPosX < posContrincante){
    	animacionRight(renderer);
    }
    else {
    	/*Esto debe ser corregido. Debe ir la animacion donde Wolverine camina hacia
    	* atras mirando a la izquierda.*/
    	animacionLeft(renderer);
    	isLookingLeft = true;
    }

    //If the dot is inside the screen move
    /*if( mPosX + DOT_VEL < LEVEL_WIDTH ) {
        mPosX += DOT_VEL;
    }*/

    //Move the dot left or right
    mPosX += CHARACTER_VEL;

    //If the dot went too far to the left or right
    if ((mPosX < -320) || (mPosX + CHARACTER_WIDTH > LEVEL_WIDTH)) {
        //Move back
        mPosX -= CHARACTER_VEL;
    }

}

void Wolverine::animacionRight(SDL_Renderer *renderer){
	string imagePath;

    if (currentWalkingRightSprite > LAST_WALKING_SPRITE) {
        currentWalkingRightSprite = FIRST_WALKING_SPRITE;
    }

    imagePath = "images/wolverine_walking_right/MVC2_Wolverine_" + to_string(currentWalkingRightSprite) + ".png";
    m_Texture.loadFromFile(imagePath, renderer);
    ++currentWalkingRightSprite;
}

void Wolverine::animacionLeft(SDL_Renderer *renderer){
	string imagePath;

    if (currentWalkingLeftSprite > LAST_WALKING_SPRITE) {
        currentWalkingLeftSprite = FIRST_WALKING_SPRITE;
    }

    imagePath = "images/wolverine_walking_left/MVC2_Wolverine_" + to_string(currentWalkingLeftSprite) + ".png";
    m_Texture.loadFromFile(imagePath, renderer);
    ++currentWalkingLeftSprite;
}

void Wolverine::jump(SDL_Renderer *renderer) {
	isStanding = false;
    if (currentJumpingSprite > LAST_JUMPING_SPRITE) {
        currentJumpingSprite = FIRST_JUMPING_SPRITE;
        mPosY = INITIAL_POS_Y;
    }

    string imagePath = "images/wolverine_jumping/MVC2_Wolverine_" + to_string(currentJumpingSprite) + ".png";
    m_Texture.loadFromFile(imagePath, renderer);

    if (currentJumpingSprite < 193) {
        mPosY -= CHARACTER_VEL;
    }
    if (currentJumpingSprite > 193) {
        mPosY += CHARACTER_VEL;
    }

    ++currentJumpingSprite;
}

void Wolverine::updateStand() {
    if (currentStandingSprite <= LAST_STANDING_SPRITE)
        currentStandingSprite++;
}
