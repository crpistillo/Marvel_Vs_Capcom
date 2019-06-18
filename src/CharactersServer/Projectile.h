//
// Created by IgVelasco on 6/17/19.
//

#ifndef MARVEL_VS_CAPCOM_PROJECTILE_H
#define MARVEL_VS_CAPCOM_PROJECTILE_H


class SpidermanServer;

class Projectile {

public:
    Projectile();

    bool active;

    void launch(int positionInX, int shotDirection, bool isLookingLeft);

    int posX;
    int direction;
    int posY;

    void travel();

    int initialPosX;
    int currentSprite;
    int lastSprite = 4;
    bool itWasActiveAndDied;
    bool isLookingLeft;

    int getCollitionX();
};


#endif //MARVEL_VS_CAPCOM_PROJECTILE_H
