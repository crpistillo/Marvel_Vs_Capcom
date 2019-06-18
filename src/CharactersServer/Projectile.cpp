//
// Created by IgVelasco on 6/17/19.
//

#include "Projectile.h"

const int PROJECTILESPEED = 50;

Projectile::Projectile() {
    active = false;
    itWasActiveAndDied = false;
    posY = 70; // DEFAULTPOS Y para estar a la altura del disparo
}

void Projectile::launch(int positionInX, int shotDirection, bool isLookingLeft) {
    this->isLookingLeft = isLookingLeft;
    active = true;
    itWasActiveAndDied = false;
    if(!isLookingLeft)
        posX = positionInX + 150; //volor para salir bien ubicado
    else
        posX = positionInX - 150;
    initialPosX = posX;
    direction = shotDirection;
    currentSprite = 0;
}

void Projectile::travel() {
    posX = posX + direction * PROJECTILESPEED;
    currentSprite++;
    if(currentSprite > lastSprite)
        currentSprite = 0;

    if(posX == initialPosX + 1000 || posX == initialPosX - 1000){ //cantidad que viaja
        itWasActiveAndDied = true;
        active = false;
        currentSprite = 0;
    }
}

int Projectile::getCollitionX() {
    int x = posX;
    if(!isLookingLeft)
        x += 303;  //al tneer otra resolucion los valores en x son distintos que los de spiderman por lo que tiene que tener un corrimiento
    else
        x += 600;

    return x;

}






