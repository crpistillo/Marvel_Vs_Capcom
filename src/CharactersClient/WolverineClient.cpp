//
// Created by IgVelasco on 5/17/19.
//

#include "WolverineClient.h"


const Uint8 SECONDARY_RED = 255;
const Uint8 SECONDARY_GREEN = 255;
const Uint8 SECONDARY_BLUE = 0;
const string wolverinePath = "images/wolverine/wolverine_";
const string MVC_FILEPATH = "/MVC2_Wolverine_";
const string FILE_EXTENSION = ".png";




WolverineClient::WolverineClient(int PosX, bool secondaryColor, int width, int height, int sobrante, int ancho,
                                 int anchoPantalla)
        : CharacterClient(
        PosX,
        556 - (height * 297 / 480),
        ancho,
        sobrante,
        false,
        width,
        height,
        anchoPantalla) {
    if(secondaryColor)
        this->loader = new ImageLoader((Uint8)SECONDARY_RED, (Uint8)SECONDARY_GREEN,(Uint8) SECONDARY_BLUE);
    else
        this->loader = new ImageLoader((Uint8)255,(Uint8)255,(Uint8)255);
}

void WolverineClient::load(SDL_Renderer *renderer, int posContrincante) {
    switch (this->currentAction) {
        case STANDING:
            if (isLookingLeft)
                this->loader->loadActionSprite(wolverinePath + "standing_left", MVC_FILEPATH, currentSprite,
                                               FILE_EXTENSION,
                                               renderer, &m_Texture);
            else
                this->loader->loadActionSprite(wolverinePath + "standing_right", MVC_FILEPATH,
                                               currentSprite, FILE_EXTENSION,
                                               renderer, &m_Texture);
            break;
        case DUCK:
            if (isLookingLeft) {
                this->loader->loadActionSprite(wolverinePath + "duck", MVC_FILEPATH, 39, FILE_EXTENSION, renderer, &m_Texture);
            } else {
                this->loader->loadActionSprite(wolverinePath + "duck", MVC_FILEPATH, 38, FILE_EXTENSION, renderer, &m_Texture);
            }
            break;
        case JUMPINGVERTICAL:
            if (isLookingLeft)
                this->loader->loadActionSprite(wolverinePath + "jumping_inverted", MVC_FILEPATH,
                                               currentSprite,
                                               FILE_EXTENSION,
                                               renderer, &m_Texture);
            else
                this->loader->loadActionSprite(wolverinePath + "jumping", MVC_FILEPATH, currentSprite,
                                               FILE_EXTENSION,
                                               renderer, &m_Texture);
            break;
        case JUMPINGRIGHT:
            if (isLookingLeft)
                this->loader->loadActionSprite(wolverinePath + "jumping_right_inverted", MVC_FILEPATH,
                                               currentSprite, FILE_EXTENSION,
                                               renderer, &m_Texture);
            else
                this->loader->loadActionSprite(wolverinePath + "jumping_right", MVC_FILEPATH,
                                               currentSprite, FILE_EXTENSION,
                                               renderer, &m_Texture);
            break;
        case JUMPINGLEFT:
            if (isLookingLeft)
                this->loader->loadActionSprite(wolverinePath + "jumping_left", MVC_FILEPATH,
                                               currentSprite, FILE_EXTENSION,
                                               renderer, &m_Texture);
            else
                this->loader->loadActionSprite(wolverinePath + "jumping_left_inverted", MVC_FILEPATH,
                                               currentSprite, FILE_EXTENSION,
                                               renderer, &m_Texture);
            break;

        case MAKINGINTRO:
            this->loader->loadActionSprite(wolverinePath + "intro", MVC_FILEPATH, currentSprite, FILE_EXTENSION,
                                           renderer, &m_Texture);
            break;
        case MOVINGLEFT:
        case MOVINGRIGHT:
            if (isLookingLeft)
                this->loader->loadActionSprite(wolverinePath + "walking_left", MVC_FILEPATH,
                                               currentSprite, FILE_EXTENSION,
                                               renderer, &m_Texture);
            else
                this->loader->loadActionSprite(wolverinePath + "walking_right", MVC_FILEPATH,
                                               currentSprite, FILE_EXTENSION,
                                               renderer, &m_Texture);
            break;
        case WALKBACK:
            if(isLookingLeft)

                this->loader->loadActionSprite(wolverinePath + "walkbackwards_left", MVC_FILEPATH,
                                               currentSprite, FILE_EXTENSION, renderer, &m_Texture);
            else
                this->loader->loadActionSprite(wolverinePath + "walkbackwards_right", MVC_FILEPATH,
                                               currentSprite, FILE_EXTENSION, renderer, &m_Texture);
            break;

    }

}
