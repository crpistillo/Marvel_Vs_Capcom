
// Created by IgVelasco on 5/17/19.
//
//
#ifndef MARVEL_VS_CAPCOM_WOLVERINESERVER_H
#define MARVEL_VS_CAPCOM_WOLVERINESERVER_H

#include "./CharacterServer.h"

class WolverineServer final : public CharacterServer {
public:
    WolverineServer(int PosX, int width, int height, int sobrante, int ancho, int anchoPantalla,
                    int numberOfClient,
					int widthStanding,
					int heightStanding,
					int widthWalking,
					int heightWalking,
					int widthDuck,
					int heightDuck,
					int widthPunch,
					int heightPunch,
					int widthPunchDown,
					int heightPunchDown,
					int widthKick,
					int heightKick,
					int widthKickDown,
					int heightKickDown,
					int widthKickAir,
					int heightKickAir,
					int widthPunchAir,
					int heightPunchAir,
					int widthJumping,
					int heightJumping,
					int widthJumpingLeft ,
					int heightJumpingLeft);
    void makeBuilderStruct(character_builder_t* builder, bool isFirstTeam);

private:
    void moveRight(int distance, int vel);
    void moveLeft(int distance, int vel);
    void stand();

    int getSpriteNumber();
};

#endif //MARVEL_VS_CAPCOM_WOLVERINESERVER_H
