//
// Created by IgVelasco on 6/17/19.
//

#ifndef MARVEL_VS_CAPCOM_PROJECTILE_H
#define MARVEL_VS_CAPCOM_PROJECTILE_H


#include "SpidermanServer.h"

class Projectile final{

public:
    Projectile();

    bool active;

    void launch(int positionInX, int shotDirection);

    int posX;
    int direction;
    int posY;

    void travel();

    int initialPosX;
    int currentSprite;
    int lastSprite = 4;
    bool itWasActiveAndDied;
};


#endif //MARVEL_VS_CAPCOM_PROJECTILE_H
