/*
 * InputManager.cpp
 *
 *  Created on: 2 abr. 2019
 *      Author: sebastian
 */

#include "InputManager.h"

InputManager* InputManager::instance = nullptr;

InputManager::InputManager() {
	this->quit = false;
	this->keyboard = SDL_GetKeyboardState(nullptr);
}

InputManager* InputManager::getInstance() {
	if(!instance) {
		instance = new InputManager();
	}

	return instance;
}

void InputManager::update() {

	SDL_Event event;
	while(SDL_PollEvent(&event)) {
		switch (event.type) {
		case SDL_QUIT:
			this->quit = true;
			break;

		case SDL_KEYDOWN:
			this->keyboard = SDL_GetKeyboardState(nullptr);
			break;

		case SDL_KEYUP:
			this->keyboard = SDL_GetKeyboardState(nullptr);
			break;
		default:
			break;
		}
	}
}

bool InputManager::isKeyDown(int key) {
	if(key < 0 || key >= KEYBOARD_SIZE)
		return false;

	return this->keyboard[key];
}

bool InputManager::isKeyUp(int key) {
	if(key < 0 || key >= KEYBOARD_SIZE)
		return false;

	return this->keyboard[key];
}

bool InputManager::isKeyPressed(KeyboardKey key) {
	if(!(this->keyboard))
		return false;

	int sdlKey = static_cast<int>(key);

	if(this->keyboard[sdlKey])
		return true;

	return false;
}

bool InputManager::quitRequested() {
	return this->quit;
}
