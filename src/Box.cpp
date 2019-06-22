/*
 * Box.cpp
 *
 *  Created on: 3 jun. 2019
 *      Author: german
 */


#include "Box.h"

#include <stdlib.h>
#include <iostream>

using namespace std;

Box::Box(float x, float y, float width, float height, int posX) {
	this->centerX=x;
	this->centerY=y;
	this->width=width;
	this->height=height;
	this->characterPosX = posX;
}

Box::~Box(){};

void Box::setCenter(float x, float y, int posX) {
	this->centerX=x;
	this->centerY=y;
}

float Box::getTop(){
	return (this->centerY)+(this->height/2);
}

float Box::getBottom(){
	return (this->centerY)-(this->height/2);
}

/*Sin dibujar las cajas se hace dificil ver si estan bien ubicadas. Si el centro está bien ubicado,
 * creo qu deberia sumar y restar el ancho dividido dos, pero ahora me da la sensacion que queda
 * mejor sin la division, no se por que*/
float Box::getLeft(){
	//return (this->centerX)-(this->width/2);
	return (this->centerX)-(this->width);
}

float Box::getRight(){
	//return (this->centerX)+(this->width/2);
	return (this->centerX)+(this->width);
}

bool Box::contactFromRightSide(Box* anotherBox){
    float rightBorder = getRight();
    float oponnentLeftBorder = anotherBox->getLeft();
    float oponnentRightBorder = anotherBox->getRight();

    return isInsideParameters(oponnentLeftBorder,oponnentRightBorder, rightBorder);
}

bool Box::contactFromLeftSide(Box *anotherBox){
    float leftBorder = getLeft();
    float oponnentLeftBorder = anotherBox->getLeft();
    float oponnentRightBorder = anotherBox->getRight();

    return isInsideParameters(oponnentLeftBorder,oponnentRightBorder, leftBorder);
}



bool Box::isColliding(Box* oponnentBox){

	bool collidingInAxisX = contactOnAxisX(oponnentBox);
	bool collidingInAxisY = contactInAxisY(oponnentBox);

    return collidingInAxisX && collidingInAxisY;
}


bool Box::contactInAxisY(Box *oponnentBox){

    float bottomBorder = getBottom();
    float topBorder = getTop();

    float mediumValueOponnent = (oponnentBox->getBottom() + oponnentBox->getTop())/2; //if one sprite is bigger than other

    bool topInsideBox = isInsideParameters(bottomBorder,topBorder, oponnentBox->getTop());
    bool bottomInsideBox = isInsideParameters(bottomBorder,topBorder, oponnentBox->getBottom());
    bool allInsideBox = isInsideParameters(bottomBorder, topBorder , mediumValueOponnent);

    return topInsideBox || bottomInsideBox || allInsideBox;
}

bool Box::contactOnAxisX(Box *oponnentBox) {

    float leftBorder = getLeft();
    float rightBorder = getRight();
    float mediumValueOponnent = (oponnentBox->getRight() + oponnentBox->getLeft())/2; //if one sprite is bigger than other


    bool leftInsideBox = isInsideParameters(leftBorder,rightBorder, oponnentBox->getLeft());
    bool rightInsideBox = isInsideParameters(leftBorder,rightBorder, oponnentBox->getRight());
    bool allInsideBox = isInsideParameters(leftBorder, rightBorder , mediumValueOponnent);


    return leftInsideBox || rightInsideBox || allInsideBox;
}

bool Box::isInsideParameters(float minBorder, float maxBorder, float value) {
    return (minBorder <= value) && (maxBorder >= value);
}

void Box::updateBox(int newWidth, int newHeight) {
    height = newHeight;
    width = newWidth;
}

int Box::getCenter() {
    return centerX;
}

bool Box::isProjectileColliding(Projectile *projectile) {

    int positionX = projectile->getCollitionX();
    float leftBorder = getLeft();
    float rightBorder = getRight();

    float bottomBorder = getBottom();
    float topBorder = getTop();

    bool collition = isInsideParameters(bottomBorder, topBorder, 21) &&
            isInsideParameters(leftBorder, rightBorder, positionX);
    if(collition)
        projectile->hit();
    return collition;
}

void Box::fixCollision(int *characterX, Box *otherBox) {
    if(*characterX > otherBox->characterPosX)
        *characterX += 1 * 30;   //vel * CHARACTERVEL
    else
        *characterX -= 1* 30;
}


