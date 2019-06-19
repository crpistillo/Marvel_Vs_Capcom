//
// Created by IgVelasco on 6/19/19.
//

#ifndef MARVEL_VS_CAPCOM_TIMER_H
#define MARVEL_VS_CAPCOM_TIMER_H


class Timer {


public:
    Timer(int maxTime);


    int getFirstDigit();

    int getSecondDigit();

    void resetTimer();

private:
    int initialTime;
    int currentTime;
    int maxTime;

};


#endif //MARVEL_VS_CAPCOM_TIMER_H
