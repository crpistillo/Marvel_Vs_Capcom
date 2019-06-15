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



	xLeftBorder = this->getLeftX();
	xRightBorder = this->getRightX();
	yTopBorder = this->getTopY();
	yBottomBorder = this->getBottomY();

	updateBox();
}

Box::~Box(){};

void Box::setHeight(float height){
}

void Box::setWidth(float width){
	this->width=width;
}

void Box::setCenter(float x, float y){
	this->centerX=x;
	this->centerY=y;
}
new B
float Box::getTopY(){
	return (this->centerY)+(this->height/2);
}

float Box::getBottomY(){
	return (this->centerY)-(this->height/2);
}

/*Sin dibujar las cajas se hace dificil ver si estan bien ubicadas. Si el centro está bien ubicado,
 * creo qu deberia sumar y restar el ancho dividido dos, pero ahora me da la sensacion que queda
 * mejor sin la division, no se por que*/
float Box::getLeftX(){
	//return (this->centerX)-(this->width/2);
	return (this->centerX)-(this->width);
}

float Box::getRightX(){
	//return (this->centerX)+(this->width/2);
	return (this->centerX)+(this->width);
}

bool Box::contactoPorLadoIzquierdo(Box* anotherBox){
	return ((this->getRightX() > anotherBox->getRightX()) && (this->getLeftX() < anotherBox->getRightX()) && contactoEnEjeY(anotherBox));
}

bool Box::contactoPorLadoDerecho(Box* anotherBox){
	return ((this->getRightX() > anotherBox->getLeftX()) && (this->getLeftX() < anotherBox->getLeftX()) && contactoEnEjeY(anotherBox));
}

bool Box::contactoEnEjeY(Box* anotherBox){
	bool caso1Y = ((this->getBottomY() > anotherBox->getTopY()) && (this->getTopY() < anotherBox->getTopY()));

	//La proyeccion en y de la caja contacta a la otra caja por abajo
	bool caso2Y = ((this->getBottomY() > anotherBox->getBottomY()) && (this->getTopY() < anotherBox->getBottomY()));

	//La proyeccion en x de la caja está dentro de la otra caja
	bool caso3Y = ((this->getTopY() > anotherBox->getTopY()) && (this->getBottomY() < anotherBox->getBottomY()));

	//La proyeccion en y de la otra caja está dentro de esta
	bool caso4Y = ((this->getTopY() < anotherBox->getTopY()) && (this->getBottomY() > anotherBox->getBottomY()));

	//Perfectamnte en el mismo lugar
	bool caso5Y = ((this->getTopY() == anotherBox->getTopY()) && (this->getBottomY() == anotherBox->getBottomY()));

	return ((caso1Y || caso2Y || caso3Y || caso4Y || caso5Y));
}



bool Box::isColliding(Box* oponnentBox){
	bool contacto = false;

	/*Chequeo contacto sobre las proyecciones en el eje x*/
	//La proyeccion en x de la caja contacta a la otra caja por el lado izquierdo
	bool caso1X = ((this->getRightX() > oponnentBox->getLeftX()) && (this->getLeftX() < oponnentBox->getLeftX()));

	//La proyeccion en x de la caja contacta a la otra caja por el lado derecho
	bool caso2X = ((this->getRightX() > oponnentBox->getRightX()) && (this->getLeftX() < oponnentBox->getRightX()));

	//La proyeccion en x de la caja está dentro de la otra caja
	bool caso3X = ((this->getLeftX() > oponnentBox->getLeftX()) && (this->getRightX() < oponnentBox->getRightX()));

	//La proyeccion en x de la otra caja está dentro de esta
	bool caso4X = ((this->getLeftX() < oponnentBox->getLeftX()) && (this->getRightX() > oponnentBox->getRightX()));

	//Perfectamnte en el mismo lugar
	bool caso5X = ((this->getLeftX() == oponnentBox->getLeftX()) && (this->getRightX() == oponnentBox->getRightX()));

	// bool contactoEnX = (caso1X || caso2X || caso3X || caso4X || caso5X);
	bool contactoEnX = contactoEnEjeX(oponnentBox    );
	bool contactoEnY = contactoEnEjeY(oponnentBox);

	if (contactoEnX && contactoEnY) contacto = true;

	return contacto;
}

bool Box::contactoEnEjeX(Box *oponnentBox) {
    bool leftInsideBox = this->getLeftX();

    return false;
}

void Box::updateBox(int newWidth , int newHeight) {
    height = newHeight;
    width = newWidth;
}

