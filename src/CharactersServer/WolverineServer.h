
// Created by IgVelasco on 5/17/19.
//
//
#ifndef MARVEL_VS_CAPCOM_WOLVERINESERVER_H
#define MARVEL_VS_CAPCOM_WOLVERINESERVER_H

#include "./CharacterServer.h"

class WolverineServer final : public CharacterServer {
public:
    WolverineServer(int PosX, int width, int height, int sobrante, int ancho, int anchoPantalla,
                    int numberOfClient);
    void makeBuilderStruct(character_builder_t* builder, bool isFirstTeam, double pos);

private:
    void moveRight(int distance, int vel);
    void moveLeft(int distance, int vel);
    void stand();

    int getSpriteNumber();
};

#endif //MARVEL_VS_CAPCOM_WOLVERINESERVER_H
