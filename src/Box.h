/*
 * Box.h
 *
 *  Created on: 3 jun. 2019
 *      Author: german
 */

#ifndef BOX_H_
#define BOX_H_


#include <string>
#include <stdio.h>
#include <map>
#include <iostream>
using namespace std;

class Box {
private:
	int height;
	int width;
	float centerX;
	float centerY;

public:
	Box();
	Box(float x, float y,float width,float height);
	virtual ~Box();
	void setHeight(float height);
	void setWidth(float width);
	void setCenter(float x, float y);
	float getTopY();
	float getBottomY();
	float getLeftX();
	float getRightX();
	bool isColliding(Box* box);
};


#endif /* BOX_H_ */
