//
// Created by IgVelasco on 6/17/19.
//

#include "ProjectileServer.h"

const int PROJECTILESPEED = 50;

ProjectileServer::ProjectileServer() {
    active = false;
    hitting = false;
    itWasActiveAndDied = false;
    posY = 70; // DEFAULTPOS Y para estar a la altura del disparo
}

void ProjectileServer::launch(int positionInX, int shotDirection, bool isLookingLeft) {
    this->isLookingLeft = isLookingLeft;
    active = true;
    impact = false;
    hitting = false;
    itWasActiveAndDied = false;
    if(!isLookingLeft)
        posX = positionInX + 150; //color para salir bien ubicado
    else
        posX = positionInX - 150;
    initialPosX = posX;
    direction = shotDirection;
    currentSprite = 0;
}

void ProjectileServer::travel() {

    currentSprite++;
    if(impact)
        hitting = true;

    if(hitting && currentSprite > lastSprite + lastHittingSprite){
        deactivate();
        return;
    }

    if(!hitting && currentSprite > lastSprite)
        currentSprite = 0;

    if(posX == initialPosX + 1000 || posX == initialPosX - 1000) //cantidad que viaja
        deactivate();

    posX = posX + direction * PROJECTILESPEED;

}

void ProjectileServer::deactivate() {
    itWasActiveAndDied = true;
    active = false;
    currentSprite = 0;
}

int ProjectileServer::getCollitionX() {
    int x = posX;
    if(!isLookingLeft)
        x += 603;  //al tneer otra resolucion los valores en x son distintos que los de spiderman por lo que tiene que tener un corrimiento
    else
        x += 453;

    return x;

}

void ProjectileServer::hit() {
    impact = true;
    currentSprite = lastSprite;


}






