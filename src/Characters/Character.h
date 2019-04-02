/*
 * Character.h
 *
 *  Created on: 2 abr. 2019
 *      Author: sebastian
 */

#ifndef CHARACTERS_CHARACTER_H_
#define CHARACTERS_CHARACTER_H_

#include <SDL2/SDL_events.h>
#include "../Texture/Texture.h"

using namespace std;

class Character {
public:
	//The dimensions of the character
	static const int CHARACTER_WIDTH = 110;
	static const int CHARACTER_HEIGHT = 20;
	//Maximum axis velocity of the character
	static const int CHARACTER_VEL = 10;

	void free();
	virtual void load(SDL_Renderer* renderer) = 0;
	//Takes key presses and adjusts the character's velocity
	void handleEvent(SDL_Event &e, SDL_Renderer* renderer, int distance);
	//Shows the character on the screen
	void render(SDL_Renderer* mRenderer, int camX, int camY);
	int getPosX();
	int getPosY();
	virtual void update() = 0;

protected:
	Character(
		int mPosX,
		int mPosY,
		int currentWalkingLeftSprite,
		int currentWalkingRightSprite,
		int currentStandingSprite,
		int currentJumpingSprite,
		bool isLookingLeft,
		string name,
		SDL_Keycode upKey,
		SDL_Keycode downKey,
		SDL_Keycode rightKey,
		SDL_Keycode leftKey
	);
	~Character();

	//The X and Y offsets of the character
	int mPosX, mPosY;
	//The velocity of the character
	int mVelX, mVelY;

	Texture m_Texture;
	int currentWalkingLeftSprite;
	int currentStandingSprite;
	int currentWalkingRightSprite;
	int currentJumpingSprite;
	bool isLookingLeft;
	bool isStanding;
	string name;

private:
	SDL_Keycode upKey;
	SDL_Keycode downKey;
	SDL_Keycode rightKey;
	SDL_Keycode leftKey;

	virtual void resetSpriteVariables() = 0;
	virtual void renderStandSprite(SDL_Renderer *renderer) = 0;
	virtual void renderDuckSprite(SDL_Renderer *renderer) = 0;
	virtual void moveRight(SDL_Renderer *renderer, int distance) = 0;
	virtual void moveLeft(SDL_Renderer *renderer, int distance) = 0;
	virtual void jump(SDL_Renderer *renderer) = 0;
};

#endif /* CHARACTERS_CHARACTER_H_ */
