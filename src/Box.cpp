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

Box::Box(float x, float y, float width,float height){
	this->centerX=x;
	this->centerY=y;
	this->width=width;
	this->height=height;
}

Box::~Box(){};

void Box::setCenter(float x, float y){
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

bool Box::contactoPorLadoIzquierdo(Box* anotherBox){
	return contactOnAxisX(anotherBox);
}

bool Box::contactoPorLadoDerecho(Box* anotherBox){
	return contactOnAxisX(anotherBox);
}



bool Box::isColliding(Box* oponnentBox){

	bool collidingInAxisX = contactOnAxisX(oponnentBox);
	bool collidingInAxisY = contactInAxisY(oponnentBox);

    return collidingInAxisX && collidingInAxisY;
}


bool Box::contactInAxisY(Box *oponnentBox){

    float bottomBorder = getLeft();
    float topBorder = getRight();

    float mediumValueOponnent = (oponnentBox->getBottom() + oponnentBox->getTop())/2; //if one sprite is bigger than other

    bool topInsideBox = isInsideParameters(bottomBorder,topBorder, oponnentBox->getTop());
    bool bottomInsideBox = isInsideParameters(bottomBorder,topBorder, oponnentBox->getBottom());
    bool allInsideBox = isInsideParameters(bottomBorder, topBorder , mediumValueOponnent);

    return topInsideBox || bottomInsideBox || mediumValueOponnent;
}

bool Box::contactOnAxisX(Box *oponnentBox) {

    float leftBorder = getLeft();
    float rightBorder = getRight();

    bool leftInsideBox = isInsideParameters(leftBorder,rightBorder, oponnentBox->getLeft());
    bool rightInsideBox = isInsideParameters(leftBorder,rightBorder, oponnentBox->getRight());

    return leftInsideBox || rightInsideBox;
}

bool Box::isInsideParameters(float minBorder, float maxBorder, float value) {
    return (minBorder <= value) && (maxBorder >= value);
}

void Box::updateBox(int newWidth, int newHeight) {
    height = newHeight;
    width = newWidth;
}
