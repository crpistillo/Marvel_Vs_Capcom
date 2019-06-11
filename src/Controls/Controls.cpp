//
// Created by IgVelasco on 4/7/19.
//

#include "Controls.h"
#include "../InputManager.h"
#include <iostream>

Controls::Controls(int up, int down, int right, int left, int change, int punch, int kick) {
    upKey = up;
    downKey = down;
    rightKey = right;
    leftKey = left;
    changeKey = change;
    punchKey = punch;
    kickKey = kick;
}

actions_t Controls::getNewAction() {

    InputManager *inputManager = InputManager::getInstance();

    //Acciones de dos teclas primero

    if (inputManager->closeWindowRequested())
    {
        inputManager->windowNotClosing();
        std::cout<<"Detecta window closed"<<std::endl;
        return DISCONNECTEDCLIENT;
    }

    else if (inputManager->isKeyDown(upKey) && inputManager->isKeyDown(rightKey))
        return JUMPINGRIGHT;

    else if (inputManager->isKeyDown(upKey) && inputManager->isKeyDown(leftKey))
        return JUMPINGLEFT;

    else if (inputManager->isKeyDown(downKey) && inputManager->isKeyDown(punchKey))
        return PUNCHDOWN;

    else if (inputManager->isKeyDown(downKey) && inputManager->isKeyDown(kickKey))
        return KICKDOWN;

    //Acciones de una sola tecla

    else if(inputManager->isKeyDown(changeKey))
        return CHANGEME;

    else if (inputManager->isKeyDown(upKey))
        return JUMPINGVERTICAL;

    else if (inputManager->isKeyDown(downKey))
        return DUCK;

    else if (inputManager->isKeyDown(punchKey))
        return PUNCH;

    else if (inputManager->isKeyDown(kickKey))
        return KICK;

    else if (inputManager->isKeyDown(rightKey) && !inputManager->isKeyUp(leftKey))
        return MOVINGRIGHT;

    else if (inputManager->isKeyDown(leftKey) && !inputManager->isKeyUp(rightKey))
        return MOVINGLEFT;  //add moveleft

    else if (
            (!inputManager->isKeyUp(upKey) &&
             !inputManager->isKeyUp(downKey) &&
             !inputManager->isKeyUp(rightKey) &&
             !inputManager->isKeyUp(leftKey))
            || (inputManager->isKeyUp(rightKey) &&
                inputManager->isKeyUp(leftKey))
            )
        return STANDING;
}

menu_action_t Controls::getNewMenuAction(){

	InputManager *inputManager = InputManager::getInstance();

	if (inputManager->closeWindowRequested())
	{
	    inputManager->windowNotClosing();
	    return MENU_WINDOWCLOSED;
	}

	else if(inputManager->isKeyDown(leftKey) && !inputManager->isKeyDown(rightKey))
		return LEFT;

	else if(!inputManager->isKeyDown(leftKey) && inputManager->isKeyDown(rightKey))
		return RIGHT;

	else if(inputManager->isKeyDown(KEY_RETURN))
		return ENTER;

	return INVALID_MENU_ACTION;
}
