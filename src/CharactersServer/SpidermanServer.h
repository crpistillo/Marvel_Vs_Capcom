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
    void makeBuilderStruct(character_builder_t *builder, bool isFirstTeam) override ;
    Projectile* projectile;
    void throwPower() override ;
    bool isProjectileActive() override;
    bool isProjectileHurting() override;
    virtual Projectile *getProjectile();


private:
    void moveRight(int distance, int vel) override ;
    void moveLeft(int distance, int vel) override ;
    void stand() override ;
    void update(int distance, int posContrincante, actions_t actionRecieved, Box* boxContrincante) override;



    int getSpriteNumber() override ;

};


#endif //MARVEL_VS_CAPCOM_SPIDERMANSERVER_H
