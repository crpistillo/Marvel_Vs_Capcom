//
// Created by IgVelasco on 5/17/19.
//

#ifndef MARVEL_VS_CAPCOM_SPIDERMANSERVER_H
#define MARVEL_VS_CAPCOM_SPIDERMANSERVER_H

#include "./CharacterServer.h"

class SpidermanServer final : public CharacterServer {
public:
    SpidermanServer(int PosX, int width, int height, int sobrante, int ancho, int anchoPantalla,
                    int numberOfClient);
    void makeBuilderStruct(character_builder_t *builder, bool isFirstTeam);

private:
    void moveRight(int distance, int posContrincante, Box* boxContrincante);
    void moveLeft(int distance, int posContrincante, Box* boxContrincante);
    void resetSpriteVariables();

    int getSpriteNumber();
};


#endif //MARVEL_VS_CAPCOM_SPIDERMANSERVER_H
