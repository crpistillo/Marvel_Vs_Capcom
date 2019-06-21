//
// Created by IgVelasco on 6/18/19.
//

#include "ProjectileClient.h"
#include "../data_structs.h"

const Uint8 SECONDARY_RED = 255;
const Uint8 SECONDARY_GREEN = 200;
const Uint8 SECONDARY_BLUE = 000;

const string projectileFilePath = "images/spiderman/spiderman_projectile_";
const string MVC_FILEPATH = "/MVC2_SpiderMan_";
const string FILE_EXTENSION = ".png";

ProjectileClient::ProjectileClient(bool secondaryColor) {
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
    m_Texture.render(posX - camX, posY - camY, 620, 480,
                     mRenderer);

}

void ProjectileClient::update(bool activate, character_updater_t *updater, bool isLookingLeft) {
    if(!active)
        this->isLookingLeft = isLookingLeft;
    active = activate;
    if(active){
        currentSprite = updater->currentProjectileSprite;
        posX = updater->pposX;
        posY = updater->pposY;
    }
}

void ProjectileClient::load(SDL_Renderer *renderer) {
    if(isLookingLeft)
        this->loader->loadActionSprite(projectileFilePath + "left", MVC_FILEPATH,
                                   currentSprite, FILE_EXTENSION,
                                   renderer, &m_Texture);
    else
        this->loader->loadActionSprite(projectileFilePath + "right", MVC_FILEPATH,
                                       currentSprite, FILE_EXTENSION,
                                       renderer, &m_Texture);
}
