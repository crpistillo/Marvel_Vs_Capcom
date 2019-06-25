/*
 * actions_bools.h
 *
 *  Created on: 22 jun. 2019
 *      Author: caropistillo
 */

#ifndef ACTIONS_BOOLS_H_
#define ACTIONS_BOOLS_H_

#include "action_constants.h"

bool isWeakPunch(actions_t action)
{
	return (action == PUNCH || action == PUNCHDOWN || action == PUNCHINGVERTICAL
			|| action == PUNCHINGJUMPRIGHT || action == PUNCHINGJUMPLEFT);
}

bool isWeakKick(actions_t action)
{
	return (action == KICK || action == KICKDOWN || action == KICKINGVERTICAL
			|| action == KICKINGJUMPRIGHT || action == KICKINGJUMPLEFT);
}

bool isHurting(actions_t action)
{
	return (action == HURTINGGROUND || action == HURTINGAIR);
}

bool isStrongPunch(actions_t action)
{
	return (action == PUNCHSTRONG || action == PUNCHSTRONGDOWN || action == PUNCHINGSTRONGVERTICAL
			||action == PUNCHINGSTRONGJUMPRIGHT || action == PUNCHINGSTRONGJUMPLEFT);
}

bool isStrongKick(actions_t action)
{
	return (action == KICKSTRONG || action == KICKSTRONGDOWN || action == KICKINGSTRONGVERTICAL
			||action == KICKINGSTRONGJUMPRIGHT || action == KICKINGSTRONGJUMPLEFT);
}

bool isHit(actions_t action)
{
	return isWeakPunch(action) || isStrongPunch(action) || isWeakKick(action) || isStrongKick(action);
}

bool isJump(actions_t action)
{
	return (action == JUMPINGLEFT || action == JUMPINGRIGHT || action == JUMPINGVERTICAL);
}

#endif /* ACTIONS_BOOLS_H_ */
