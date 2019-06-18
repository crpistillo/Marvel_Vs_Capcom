//
// Created by IgVelasco on 6/17/19.
//

#include "Projectile.h"
const int RIGHTSHOT = 1;
const int LEFTSHOT = -1;
const int PROJECTILESPEED = 50;

Projectile::Projectile() {
    active = false;
    itWasActiveAndDied = false;
    posY = 70; // DEFAULTPOS Y
}

void Projectile::launch(int positionInX, int shotDirection) {
    active = true;
    itWasActiveAndDied = false;
    posX = positionInX + 150;
    initialPosX = positionInX;
    direction = shotDirection;
    currentSprite = 0;
}

void Projectile::travel() {
    posX = posX + direction * PROJECTILESPEED;
    currentSprite++;
    if(currentSprite > lastSprite)
        currentSprite = 0;

    if(posX == initialPosX + 1000 || posX == initialPosX - 1000){
        itWasActiveAndDied = true;
        active = false;
        currentSprite = 0;
    }
}





