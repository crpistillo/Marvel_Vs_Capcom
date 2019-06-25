//
// Created by IgVelasco on 5/17/19.
//

#ifndef MARVEL_VS_CAPCOM_IRONMANSERVER_H
#define MARVEL_VS_CAPCOM_IRONMANSERVER_H

#include "./CharacterServer.h"

class IronmanServer final : public CharacterServer {
public:
    IronmanServer(int PosX, int width, int height, int sobrante, int ancho, int anchoPantalla,
                    int numberOfClient);
    void makeBuilderStruct(character_builder_t *builder, bool isFirstTeam);
    character_number_t nameOfCharacter() override;



private:
    void moveRight(int distance, float vel, Box *boxOfEnemy, bool isGrounded, actions_t action);
    void moveLeft(int distance, float vel, Box *boxOfEnemy, bool isGrounded, actions_t action);
    void stand();
    void update(int distance, int posContrincante, actions_t actionRecieved, Box *boxEnemy) override;



    int getSpriteNumber();

};


#endif //MARVEL_VS_CAPCOM_IRONMANSERVER_H
