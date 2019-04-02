/*
 * InputManager.h
 *
 *  Created on: 2 abr. 2019
 *      Author: sebastian
 */

#ifndef INPUTMANAGER_H_
#define INPUTMANAGER_H_

#include "SDL2/SDL.h"
#include "InputTable.h"

class InputManager {
public:
	static InputManager* getInstance();
	void update();
	bool isKeyDown(int key);
	bool isKeyUp(int key);
	bool isKeyPressed(KeyboardKey key);
	bool quitRequested();

	InputManager();

private:
	static InputManager* instance;
	const uint8_t* keyboard;
	bool keyDown[KEYBOARD_SIZE];
	bool keyUp[KEYBOARD_SIZE];
	bool quit;
};

#endif /* INPUTMANAGER_H_ */
