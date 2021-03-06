//
// Created by IgVelasco on 6/18/19.
//

#include "ProjectileClient.h"
#include "../data_structs.h"

const Uint8 SECONDARY_RED = 255;
const Uint8 SECONDARY_GREEN = 200;
const Uint8 SECONDARY_BLUE = 000;

const string FILE_EXTENSION = ".png";

ProjectileClient::ProjectileClient(bool secondaryColor){
    active = false;
    posX = 0;
    posY = 0;
    currentSprite = 0;
    if(!secondaryColor)
        loader = new ImageLoader((Uint8) 255, (Uint8) 255, (Uint8) 255);
    else
        loader = new ImageLoader(SECONDARY_RED, SECONDARY_GREEN, SECONDARY_BLUE);
}

void ProjectileClient::render(SDL_Renderer *mRenderer, int camX, int camY) {
    if(!active)
        return;
    m_Texture.render(posX - camX + 240, posY + camY + 200, 500, 300,
                     mRenderer);

}

void ProjectileClient::update(bool activate, character_updater_t *updater) {
    active = activate;
    if(active){
        currentSprite = updater->currentProjectileSprite;
        posX = updater->pposX;
        posY = updater->pposY;
    }
}

void ProjectileClient::load(SDL_Renderer *renderer, bool isLookingLeft) {
    if(isLookingLeft)
        this->loader->loadActionSprite(projectileFilePath + "left", mvcFilePath,
                                   currentSprite, FILE_EXTENSION,
                                   renderer, &m_Texture);
    else
        this->loader->loadActionSprite(projectileFilePath + "right", mvcFilePath,
                                       currentSprite, FILE_EXTENSION,
                                       renderer, &m_Texture);
}

