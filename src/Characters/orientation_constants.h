//
// Created by IgVelasco on 5/9/19.
//

#ifndef MARVEL_VS_CAPCOM_ORIENTATION_CONSTANTS_H
#define MARVEL_VS_CAPCOM_ORIENTATION_CONSTANTS_H

//TODO ADD MOVINGLEFT AND MOVINGRIGHT
#define S 0
#define JR 1
#define JL 2
#define JV 3
#define MI 4
#define D 5
#define M 6
#define WB 7

#define R 0
#define L 1


typedef enum actions {
    STANDING = S, JUMPINGLEFT = JL, JUMPINGRIGHT = JR, JUMPINGVERTICAL = JV, MAKINGINTRO = MI, DUCK = D,
    MOVING = M, WALKBACK = WB
} actions_t;

#endif //MARVEL_VS_CAPCOM_ORIENTATION_CONSTANTS_H
