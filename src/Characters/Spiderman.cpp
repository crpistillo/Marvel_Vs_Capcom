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
const int LAST_JUMPING_SPRITE = 21;
const int LAST_JUMPING_RIGHT_SPRITE = 19;
const int LAST_JUMPING_LEFT_SPRITE = 19;
const int LAST_INTRO_SPRITE = 16;

const unsigned int SECONDARY_RED = 255;
const unsigned int SECONDARY_GREEN = 200;
const unsigned int SECONDARY_BLUE = 000;

const string MVC_FILEPATH = "/MVC2_SpiderMan_";
const string FILE_EXTENSION = ".png";

Spiderman::Spiderman(int PosX, bool secondaryColor, int width, int height, int sobrante, int ancho, int anchoPantalla)
        : Character(
        PosX,
        556 - (height * 297 / 480),
        ancho,
        sobrante,
        false,
        width,
        height,
        anchoPantalla
) {
    if (secondaryColor)
        loader = new ImageLoader(SECONDARY_RED, SECONDARY_GREEN, SECONDARY_BLUE);
    else
        loader = new ImageLoader(255, 255, 255);
    currentAction = STANDING;
}

Spiderman::~Spiderman() {
}

void Spiderman::load(SDL_Renderer *renderer) {


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
            if (!isLookingLeft && 10 <= currentIntroSprite && currentIntroSprite <= 16)
                this->loader->loadActionSprite(characterFilepath + "intro", "/MVC2_SpiderManR_", currentIntroSprite,
                                               FILE_EXTENSION,
                                               renderer, &m_Texture);
            else
                this->loader->loadActionSprite(characterFilepath + "intro", MVC_FILEPATH, currentIntroSprite,
                                               FILE_EXTENSION,
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
    }

}

void Spiderman::resetSpriteVariables() {
    mPosY = this->INITIAL_POS_Y;
    currentJumpingSprite = 0;
    currentWalkingSprite = 0;
}




void Spiderman::stand() {
    currentAction = STANDING;
    this->resetSpriteVariables();
    if (currentStandingSprite >= LAST_STANDING_SPRITE)
        currentStandingSprite = 0;
}



void Spiderman::renderDuckSprite() {
    currentAction = DUCK;
}

void Spiderman::moveLeft(int distance, int posContrincante) {

    mPosX -= CHARACTER_VEL;

    /*distance va de -800 a 800 (ancho de la pantalla)*/
    if ((mPosX - CHARACTER_VEL <= -Spiderman::getSobrante()) || (distance < (-anchoPantalla))) {
        isLookingLeft = false;
        //Move back
        mPosX += CHARACTER_VEL;
    }

    if (this->getCentro() > posContrincante) {
        walkingSpriteUpdate();
    } else {
        walkingSpriteUpdate();
        isLookingLeft = false;
    }
}


void Spiderman::moveRight(int distance, int posContrincante) {

    currentAction = MOVING;
    isLookingLeft = false;

    mPosX += CHARACTER_VEL;

    if ((mPosX + CHARACTER_VEL >= (LEVEL_WIDTH - Spiderman::getSobrante() - Spiderman::getWidth())) ||
        (distance > anchoPantalla)) {
        isLookingLeft = true;
        //Move back
        mPosX -= CHARACTER_VEL;
    }

    if (this->getCentro() < posContrincante) {
        walkingSpriteUpdate();
    } else {
        walkingSpriteUpdate();
        isLookingLeft = true;
    }
}


//void animacion
void Spiderman::walkingSpriteUpdate() {
    ++currentWalkingSprite;

    if (currentWalkingSprite >= LAST_WALKING_SPRITE)
        currentWalkingSprite = 0;
}


void Spiderman::jump(int *currentSprite, int lastSprite) {

    *currentSprite < 10 ? (mPosY -= 2.5 * CHARACTER_VEL) : (mPosY += 2.5 * CHARACTER_VEL);
    (*currentSprite)++;
    if (*currentSprite > lastSprite) {
        *currentSprite = 0;
        mPosY = this->INITIAL_POS_Y;
        this->currentAction = STANDING;
        currentStandingSprite = 0;
    }
}

void Spiderman::jumpVertical() {
    this->currentAction = JUMPINGVERTICAL;
    jump(&currentJumpingSprite, LAST_JUMPING_SPRITE);
}

void Spiderman::jumpRight() {
    this->currentAction = JUMPINGRIGHT;
    jump(&currentJumpingRightSprite, LAST_JUMPING_RIGHT_SPRITE);

}


void Spiderman::jumpLeft() {
    this->currentAction = JUMPINGLEFT;
    jump(&currentJumpingRightSprite, LAST_JUMPING_LEFT_SPRITE);
}


void Spiderman::makeIntro() {
    currentAction = MAKINGINTRO;


    unsigned int currentTime = SDL_GetTicks();


    if (currentIntroSprite <= LAST_INTRO_SPRITE) {
        ++currentIntroSprite;
        lastTime = currentTime;
    }


    if (currentIntroSprite > LAST_INTRO_SPRITE && (currentTime - lastTime) > 500) {
        currentIntroSprite = 0;
        currentAction = STANDING;
        currentStandingSprite = 0;
    }

}



void Spiderman::updateStand() {
    if (currentStandingSprite <= LAST_STANDING_SPRITE)
        currentStandingSprite++;
}


