//
// Created by IgVelasco on 6/17/19.
//

#ifndef MARVEL_VS_CAPCOM_PROJECTILESERVER_H
#define MARVEL_VS_CAPCOM_PROJECTILESERVER_H


class SpidermanServer;

class ProjectileServer {

public:
    ProjectileServer();

    bool active;

    void launch(int positionInX, int shotDirection, bool isLookingLeft);

    int posX;
    int direction;
    int posY;

    void travel();

    int initialPosX;
    int currentSprite = 0;
    bool itWasActiveAndDied;
    bool isLookingLeft;

    int getCollitionX();

    void hit();

    bool hitting;

    void deactivate();

protected:
    int lastSprite;
    int lastHittingSprite;
    bool impact;
};


#endif //MARVEL_VS_CAPCOM_PROJECTILESERVER_H
