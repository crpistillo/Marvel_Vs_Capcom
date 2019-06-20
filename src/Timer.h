//
// Created by IgVelasco on 6/19/19.
//

#ifndef MARVEL_VS_CAPCOM_TIMER_H
#define MARVEL_VS_CAPCOM_TIMER_H


#include "data_structs.h"

class Timer {


public:
    Timer(int maxTime);


    int getFirstDigit();

    int getSecondDigit();

    void resetTimer();

    int getTimeThatPass();

    void setDigits(character_updater_t *updater);

private:
    int initialTime;
    int currentTime;
    int maxTime;

};


#endif //MARVEL_VS_CAPCOM_TIMER_H
