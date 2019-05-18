//
// Created by IgVelasco on 5/17/19.
//

#ifndef MARVEL_VS_CAPCOM_SPIDERMANSERVER_H
#define MARVEL_VS_CAPCOM_SPIDERMANSERVER_H

#include "./CharacterServer.h"

class SpidermanServer final : public CharacterServer {
public:
    SpidermanServer(int PosX, bool secondaryColor, int width, int height, int sobrante, int ancho, int anchoPantalla);

private:
    void moveRight(int distance, int posContrincante);
    void moveLeft(int distance, int posContrincante);
};


#endif //MARVEL_VS_CAPCOM_SPIDERMANSERVER_H