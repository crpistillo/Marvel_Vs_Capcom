//
// Created by IgVelasco on 5/17/19.
//

#ifndef MARVEL_VS_CAPCOM_SPIDERMANCLIENT_H
#define MARVEL_VS_CAPCOM_SPIDERMANCLIENT_H
#include <SDL2/SDL_events.h>
#include "../Texture/Texture.h"
#include "./CharacterClient.h"

class SpidermanClient final: public CharacterClient{
public:
    //Initializes the variables
    SpidermanClient(int PosX, bool secondaryColor, int width, int height, int sobrante, int ancho,
                    int anchoPantalla, int numOfClient);
    ~SpidermanClient();
    void load(SDL_Renderer *renderer, int posContrincante);
    void loadBanner(SDL_Renderer *renderer);

};


#endif //MARVEL_VS_CAPCOM_SPIDERMANCLIENT_H
