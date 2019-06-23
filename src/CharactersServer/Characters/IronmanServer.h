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
    ProjectileServer* projectile;
    void throwPower() override ;
    bool isProjectileActive() override;
    bool isProjectileHurting() override;
    virtual ProjectileServer *getProjectile();


private:
    void moveRight(int distance, int vel, Box *boxOfEnemy, bool isGrounded);
    void moveLeft(int distance, int vel, Box *boxOfEnemy, bool isGrounded);
    void stand();
    void update(int distance, int posContrincante, actions_t actionRecieved, Box *boxEnemy) override;



    int getSpriteNumber();

};


#endif //MARVEL_VS_CAPCOM_IRONMANSERVER_H
