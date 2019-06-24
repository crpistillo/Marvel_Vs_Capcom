//
// Created by IgVelasco on 5/17/19.
//

#ifndef MARVEL_VS_CAPCOM_WOLVERINECLIENT_H
#define MARVEL_VS_CAPCOM_WOLVERINECLIENT_H

#include <SDL2/SDL_events.h>
#include "../Texture/Texture.h"
#include "./CharacterClient.h"
#include "ProjectileWolverineClient.h"

class WolverineClient final: public CharacterClient {
public:
    //Initializes the variables
    WolverineClient(int PosX, bool secondaryColor, int width, int height, int sobrante, int ancho,
                    int anchoPantalla, int numOfClient);

    void load(SDL_Renderer *renderer, int posContrincante);
    void loadBanner(SDL_Renderer *renderer);

    void renderVictoryPose(SDL_Renderer *mRenderer, int posX);
};

#endif //MARVEL_VS_CAPCOM_WOLVERINECLIENT_H

