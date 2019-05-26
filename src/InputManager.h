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
	bool isHoldingKey(int key);
	bool quitRequested();
	bool closeWindowRequested();
	void windowNotClosing();
	bool windowHasClosed();
	void windowHasntClosed();

	InputManager();

private:
	static InputManager* instance;
	const uint8_t* keyboard;
	bool quit;
	bool windowIsClosing;
	bool windowClosed;

	uint8_t keyStatus[KEYBOARD_SIZE];
	uint8_t keyHold[KEYBOARD_SIZE];
};

#endif /* INPUTMANAGER_H_ */
