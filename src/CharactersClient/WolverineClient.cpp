//
// Created by IgVelasco on 5/17/19.
//

#include "WolverineClient.h"


const unsigned int SECONDARY_RED = 255;
const unsigned int SECONDARY_GREEN = 255;
const unsigned int SECONDARY_BLUE = 0;

const string MVC_FILEPATH = "/MVC2_Wolverine_";
const string FILE_EXTENSION = ".png";




WolverineClient::WolverineClient(int PosX, bool secondaryColor, int width, int height, int sobrante, int ancho, int anchoPantalla)
        : CharacterClient(
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

void WolverineClient::load(SDL_Renderer *renderer, int posContrincante) {
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
            if (isLookingLeft) {
                this->loader->loadActionSprite(characterFilepath + "duck", MVC_FILEPATH, 39, FILE_EXTENSION, renderer, &m_Texture);
            } else {
                this->loader->loadActionSprite(characterFilepath + "duck", MVC_FILEPATH, 38, FILE_EXTENSION, renderer, &m_Texture);
            }
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
