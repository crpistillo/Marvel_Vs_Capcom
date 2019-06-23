//
// Created by IgVelasco on 6/22/19.
//

#ifndef MARVEL_VS_CAPCOM_IRONMANCLIENT_H
#define MARVEL_VS_CAPCOM_IRONMANCLIENT_H
#include <SDL2/SDL_events.h>
#include "../Texture/Texture.h"
#include "./CharacterClient.h"

class IronManClient final: public CharacterClient{
public:
    //Initializes the variables
    void render(SDL_Renderer *mRenderer, int camX, int camY, int posContrincante) override ;

    IronManClient(int PosX, bool secondaryColor, int width, int height, int sobrante, int ancho,
                    int anchoPantalla, int numOfClient);
    ~IronManClient();
    void load(SDL_Renderer *renderer, int posContrincante) override;
    ProjectileClient* projectile;
    virtual ProjectileClient* getProjectile() override ;
    void loadBanner(SDL_Renderer *renderer);

};


#endif //MARVEL_VS_CAPCOM_IRONMANCLIENT_H
