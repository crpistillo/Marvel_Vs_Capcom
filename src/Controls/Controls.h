//
// Created by IgVelasco on 4/7/19.
//

#ifndef MARVEL_VS_CAPCOM_CONTROLS_H
#define MARVEL_VS_CAPCOM_CONTROLS_H


#include "../orientation_constants.h"

class Controls {
public:
    int upKey;
    int downKey;
    int rightKey;
    int leftKey;
    int changeKey;

    actions_t getNewAction();

protected:
    Controls(int upKey, int downKey, int rightKey, int leftKey, int changeKey);
};


#endif //MARVEL_VS_CAPCOM_CONTROLS_H
