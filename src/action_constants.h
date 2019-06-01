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
#define ML 6
#define MR 7
#define WB 8
#define CM 9
#define CMO 10
#define DC 11

#define R 0
#define L 1


typedef enum actions {
    STANDING = S, JUMPINGLEFT = JL, JUMPINGRIGHT = JR, JUMPINGVERTICAL = JV, MAKINGINTRO = MI, DUCK = D,
    MOVINGRIGHT = MR, MOVINGLEFT = ML, WALKBACK = WB, CHANGEME = CM, CHANGEME_ONEPLAYER = CMO, DISCONNECTEDCLIENT = DC
} actions_t;

typedef enum menuAction{
    SELECT = 94,
	MENU_WINDOWCLOSED = 95,
    RIGHT = 96,
    LEFT = 97,
    ENTER = 98,
    INVALID_MENU_ACTION = 99
} menu_action_t;

#endif //MARVEL_VS_CAPCOM_ORIENTATION_CONSTANTS_H
