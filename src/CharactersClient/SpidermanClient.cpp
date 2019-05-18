//
// Created by IgVelasco on 5/17/19.
//

#include "SpidermanClient.h"
#include "../InputTable.h"

using namespace std;

const unsigned int SECONDARY_RED = 255;
const unsigned int SECONDARY_GREEN = 200;
const unsigned int SECONDARY_BLUE = 000;

const string MVC_FILEPATH = "/MVC2_SpiderMan_";
const string FILE_EXTENSION = ".png";

SpidermanClient::SpidermanClient(int PosX, bool secondaryColor, int width, int height, int sobrante, int ancho, int anchoPantalla)
        : CharacterClient(
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

SpidermanClient::~SpidermanClient() {
}

void SpidermanClient::load(SDL_Renderer *renderer, int posContrincante) {

    isLookingLeft = this->getCentro() > posContrincante;
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
        case MOVINGLEFT:
        case MOVINGRIGHT:
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


