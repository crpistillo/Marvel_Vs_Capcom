//
// Created by IgVelasco on 5/17/19.
//

#ifndef MARVEL_VS_CAPCOM_RYU_H
#define MARVEL_VS_CAPCOM_RYU_H
#include <SDL2/SDL_events.h>
#include "../Texture/Texture.h"
#include "./CharacterClient.h"

class RyuClient final: public CharacterClient{
public:
    //Initializes the variables
    RyuClient(int PosX, bool secondaryColor, int width, int height, int sobrante, int ancho,
                    int anchoPantalla, int numOfClient);
    ~RyuClient();
    void load(SDL_Renderer *renderer, int posContrincante);
    ProjectileClient* projectile;
    virtual ProjectileClient* getProjectile() override ;
    void loadBanner(SDL_Renderer *renderer);

};


#endif //MARVEL_VS_CAPCOM_RYU_H
