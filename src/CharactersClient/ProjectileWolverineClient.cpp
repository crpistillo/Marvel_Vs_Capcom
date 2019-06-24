//
// Created by donatojp on 23/06/19.
//

#include "ProjectileWolverineClient.h"


ProjectileWolverineClient::ProjectileWolverineClient(bool secondaryColor):ProjectileClient(secondaryColor){
    mvcFilePath = "/MVC2_Wolverine_";
    projectileFilePath = "images/wolverine/wolverine_projectile_";
}

void ProjectileWolverineClient::render(SDL_Renderer *mRenderer, int camX, int camY) {
    if(!active)
        return;
    m_Texture.render(posX - camX + 240, posY + camY + 250, 500, 300,
                     mRenderer);
}
