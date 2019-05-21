//
// Created by IgVelasco on 4/7/19.
//

#include "Controls.h"
#include "../InputManager.h"

Controls::Controls(int up, int down, int right, int left, int change) {
    upKey = up;
    downKey = down;
    rightKey = right;
    leftKey = left;
    changeKey = change;
}

actions_t Controls::getNewAction() {

    InputManager *inputManager = InputManager::getInstance();

    //Acciones de dos teclas primero

    if (inputManager->isKeyDown(upKey) && inputManager->isKeyDown(rightKey))
        return JUMPINGRIGHT;

    else if (inputManager->isKeyDown(upKey) && inputManager->isKeyDown(leftKey))
        return JUMPINGLEFT;

    //Acciones de una sola tecla

    else if(inputManager->isKeyDown(changeKey))
        return CHANGEME;

    else if (inputManager->isKeyDown(upKey))
        return JUMPINGVERTICAL;

    else if (inputManager->isKeyDown(downKey))
        return DUCK;

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
