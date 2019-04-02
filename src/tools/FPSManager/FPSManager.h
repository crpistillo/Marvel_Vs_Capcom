/*
 * Timer.h
 *
 *  Created on: 2 abr. 2019
 *      Author: sebastian
 */

#ifndef TOOLS_FPSMANAGER_FPSMANAGER_H_
#define TOOLS_FPSMANAGER_FPSMANAGER_H_

#include <SDL2/SDL.h>

class FPSManager {
public:
	FPSManager(int screenFPS);

	void start();
	void stop();

private:
	Uint32 t1;
	Uint32 t2;
	Uint32 s;
	Uint32 delta;
	Uint32 elapsedTime;
	Uint32 delayTime;
};


#endif /* TOOLS_FPSMANAGER_FPSMANAGER_H_ */
