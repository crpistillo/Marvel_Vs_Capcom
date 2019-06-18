//
// Created by IgVelasco on 6/18/19.
//

#include "ProjectileClient.h"
#include "../data_structs.h"
const string projectileFilePath = "images/spiderman/spiderman_projectile_right";
const string MVC_FILEPATH = "/MVC2_SpiderMan_";
const string FILE_EXTENSION = ".png";

ProjectileClient::ProjectileClient() {
    active = false;
    posX = 0;
    posY = 0;
    currentSprite = 0;
    loader = new ImageLoader((Uint8) 255, (Uint8) 255, (Uint8) 255);
}

void ProjectileClient::render(SDL_Renderer *mRenderer, int camX, int camY) {
    if(!active)
        return;
    m_Texture.render(posX - camX, posY - camY, 1000, 700,
                     mRenderer);

}

void ProjectileClient::update(bool activate, character_updater_t* updater) {
    active = activate;
    if(active){
        currentSprite = updater->currentProjectileSprite;
        posX = updater->pposX;
        posY = updater->pposY;
    }
}

void ProjectileClient::load(SDL_Renderer *renderer) {
    this->loader->loadActionSprite(projectileFilePath , MVC_FILEPATH,
                                   currentSprite, FILE_EXTENSION,
                                   renderer, &m_Texture);
}
