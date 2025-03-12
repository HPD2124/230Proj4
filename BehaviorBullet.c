//------------------------------------------------------------------------------
//
// File Name:	BehaviorBullet.c
// Author(s):	Harrison Parrish (harrison.parrish)
// Project:		Project 4
// Course:		CS230S25
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "BehaviorBullet.h"
#include "Behavior.h"
#include "Entity.h"

//------------------------------------------------------------------------------
// Private Constants:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Structures:
//------------------------------------------------------------------------------
typedef enum {
    cBulletInvalid = -1,
    cBulletIdle
} BulletState;
//------------------------------------------------------------------------------
// Public Variables:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Variables:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Function Declarations:
//------------------------------------------------------------------------------
static void BehaviorBulletInit(Behavior* behavior);
static void BehaviorBulletUpdate(Behavior* behavior, float dt);
static void BehaviorBulletExit(Behavior* behavior);
static void BehaviorBulletUpdateLifeTimer(Behavior* behavior, float dt);
//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

Behavior* BehaviorBulletCreate() {
    Behavior* behavior = (Behavior*)calloc(1, sizeof(Behavior));
    if (behavior) {
        behavior->stateCurr = cBulletInvalid;
        behavior->stateNext = cBulletInvalid;
        behavior->onInit = BehaviorBulletInit;
        behavior->onUpdate = BehaviorBulletUpdate;
        behavior->onExit = BehaviorBulletExit;
    }
    return behavior;
}

// Initialize the ...
void BehaviorBulletInit(Behavior* behavior) {
    UNREFERENCED_PARAMETER(behavior);
}


// Update the ...
// Params:
//	 dt = Change in time (in seconds) since the last game loop.
void BehaviorBulletUpdate(Behavior* behavior, float dt) {
    switch (behavior->stateCurr) {
    case cBulletIdle:
        BehaviorBulletUpdateLifeTimer(behavior, dt);
    
        break;
    default:
        break;
    }
}
// Shutdown the ...
void BehaviorBulletExit(Behavior* behavior) {
    UNREFERENCED_PARAMETER(behavior);
}
static void BehaviorBulletUpdateLifeTimer(Behavior* behavior, float dt) {
    if (behavior->timer > 0) {
        behavior->timer =- dt;
        if (behavior->timer <= 0) {
            EntityDestroy(behavior->parent);
        }
    }
}


//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------

