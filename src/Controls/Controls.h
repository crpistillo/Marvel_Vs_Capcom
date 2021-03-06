//
// Created by IgVelasco on 4/7/19.
//

#ifndef MARVEL_VS_CAPCOM_CONTROLS_H
#define MARVEL_VS_CAPCOM_CONTROLS_H


#include "../action_constants.h"
#include "../data_structs.h"
#include "../Timer.h"

class Controls {
public:
    int upKey;
    int downKey;
    int rightKey;
    int leftKey;
    int changeKey;
    int punchKey;
    int kickKey;
    int blockKey;
    int throwKey;
    int gripKey;
    int punchStrongKey;
    int kickStrongKey;
    int soundKey;
    int debugKey;

    actions_t getNewAction();
    menu_action_t getNewMenuAction();

protected:
    Controls(int upKey, int downKey, int rightKey, int leftKey, int changeKey, int punchKey,
    		int kickKey, int blockKey, int throwKey, int grip, int punchStrong, int kickStrong, int debug, int sound);

    Timer *timer;
};


#endif //MARVEL_VS_CAPCOM_CONTROLS_H
