//------------------------------------------------------------------------------
//
// File Name:	BehaviorSpaceship.c
// Author(s):	Harrison Parrish (harrison.parrish)
// Project:		Project 4
// Course:		CS230S25
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------
#include "stdafx.h"
#include <stdlib.h>
#include "Behavior.h"
#include "Entity.h"
#include "Physics.h"
#include "Transform.h"
#include "Vector2D.h"
#include "EntityContainer.h"
#include "Trace.h"
#include "EntityFactory.h"


//------------------------------------------------------------------------------
// Private Constants:
//------------------------------------------------------------------------------
static const float spaceshipAcceleration = 150.0f;
static const float spaceshipSpeedMax = 500.0f;
static const float spaceshipTurnRateMax = 3.14159f / 1.5f;
static const float spaceshipWeaponCooldownTime = 0.034f;
static const float spaceshipWeaponBulletSpeed = 750.0f;
//------------------------------------------------------------------------------
// Private Structures:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Variables:
//------------------------------------------------------------------------------
EntityContainer* container;
//------------------------------------------------------------------------------
// Private Variables:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Function Declarations:
//------------------------------------------------------------------------------
static void BehaviorSpaceshiponInit(Behavior* behavior);
static void BehaviorSpaceshiponUpdate(Behavior* behavior, float dt);
static void BehaviorSpaceshiponExit(Behavior* behavior);
static void BehaviorSpaceshipUpdateRotation(Behavior* behavior, float dt);
static void BehaviorSpaceshipUpdateVelocity(Behavior* behavior, float dt);
static void BehaviorSpaceshipUpdateWeapon(Behavior* behavior, float dt);
static void BehaviorSpaceshipSpawnBullet(Behavior* behavior);
//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------
typedef enum {
    cSpaceshipInvalid = -1,
    cSpaceshipIdle,
    cSpaceshipThrust
} SpaceshipState;

Behavior* BehaviorSpaceshipCreate(void) {
    Behavior* newSpaceship = (Behavior*)calloc(1, sizeof(Behavior));
    if (!newSpaceship) {
        return NULL;
    }

    newSpaceship->stateCurr = cSpaceshipInvalid;
    newSpaceship->stateNext = cSpaceshipInvalid;
    newSpaceship->onInit = BehaviorSpaceshiponInit;
    newSpaceship->onUpdate = BehaviorSpaceshiponUpdate;
    newSpaceship->onExit = BehaviorSpaceshiponExit;

    return newSpaceship;
}
static void BehaviorSpaceshiponInit(Behavior* behavior) {
    UNREFERENCED_PARAMETER(behavior);
}
// Update the ...
// Params:
//	 dt = Change in time (in seconds) since the last game loop.
static void BehaviorSpaceshiponUpdate(Behavior* behavior, float dt) {
    switch (behavior->stateCurr) {
    case cSpaceshipIdle:
        BehaviorSpaceshipUpdateRotation(behavior, dt);
        BehaviorSpaceshipUpdateWeapon(behavior, dt);
        if (DGL_Input_KeyDown(VK_UP)) {
            behavior->stateNext = cSpaceshipThrust;
        }
        break;
    case cSpaceshipThrust:
        BehaviorSpaceshipUpdateRotation(behavior, dt);
        BehaviorSpaceshipUpdateVelocity(behavior, dt);
        BehaviorSpaceshipUpdateWeapon(behavior, dt);
        if (!DGL_Input_KeyDown(VK_UP)) {
            behavior->stateNext = cSpaceshipIdle;
        }
        break;
    }
}

// Shutdown the ...
static void BehaviorSpaceshiponExit(Behavior* behavior) {
    UNREFERENCED_PARAMETER(behavior);
}
static void BehaviorSpaceshipUpdateRotation(Behavior* behavior, float dt) {
    UNREFERENCED_PARAMETER(behavior);
    Entity* spaceship = EntityContainerFindByName(container, "Spaceship");

    if (spaceship) {
        Transform* spaceshipTransform = EntityGetTransform(spaceship);

        if (spaceshipTransform) {
            if (DGL_Input_KeyDown(VK_LEFT)) {
                TransformSetRotation(spaceshipTransform, spaceshipTurnRateMax * dt);
            }
            else if (DGL_Input_KeyDown(VK_RIGHT)) {
                TransformSetRotation(spaceshipTransform, -spaceshipTurnRateMax * dt);
            }
            else {
                TransformSetRotation(spaceshipTransform, 0);
            }
        }
    }
}





void BehaviorSpaceshipUpdateVelocity(Behavior* behavior, float dt) {
    UNREFERENCED_PARAMETER(dt);

    Transform* transform = EntityGetTransform(behavior->parent);
    Physics* phys = EntityGetPhysics(behavior->parent);

    if (!transform || !phys) return;

    float spaceshipRotation = TransformGetRotation(transform);

    Vector2D direction;
    Vector2DFromAngleRad(&direction, spaceshipRotation);

    const Vector2D* velocity = PhysicsGetVelocity(phys);
    Vector2DScaleAdd((Vector2D*)velocity, velocity, spaceshipAcceleration, &direction);



    float speed = Vector2DLength(velocity);

   
    if (speed > spaceshipSpeedMax) {
      
        Vector2D* modifiableVelocity = (Vector2D*)velocity;
        Vector2DScale(modifiableVelocity, &direction, spaceshipSpeedMax / speed);
    }
    

}



static void BehaviorSpaceshipUpdateWeapon(Behavior* behavior, float dt) {
    if (behavior->timer > 0) {
        behavior->timer -= dt;
        if (behavior->timer < 0) {
            behavior->timer = 0;
        }
    }

    if (DGL_Input_KeyDown(' ')) {
        if (behavior->timer <= 0) {
            BehaviorSpaceshipSpawnBullet(behavior);
            behavior->timer = spaceshipWeaponCooldownTime;
        }
    }
}
static void BehaviorSpaceshipSpawnBullet(Behavior* behavior) {
    UNREFERENCED_PARAMETER(behavior);


    Entity* bullet = EntityFactoryBuild("Bullet");
    if (!bullet) return;



    Entity* spaceship = EntityContainerFindByName(container, "Spaceship");
    if (!spaceship) {
        TraceMessage("BehaviorSpaceshipSpawnBullet: Unable to find spaceship entity.");
        return;
    }


    Transform* spaceshipTransform = EntityGetTransform(spaceship);
    if (!spaceshipTransform) {
        TraceMessage("BehaviorSpaceshipSpawnBullet: Unable to find spaceship transform.");
        return;
    }


    float spaceshipRotation = TransformGetRotation(spaceshipTransform);


    Transform* bulletTransform = TransformClone(spaceshipTransform);
    TransformSetTranslation(bulletTransform, TransformGetTranslation(spaceshipTransform));
    TransformSetRotation(bulletTransform, spaceshipRotation);


    Physics* bulletPhysics = EntityGetPhysics(bullet);
    if (!bulletPhysics) {
        TraceMessage("BehaviorSpaceshipSpawnBullet: Unable to find bullet physics.");
        return;
    }


    Vector2D direction;
    Vector2DFromAngleDeg(&direction, spaceshipRotation);

    const Vector2D* velocity = PhysicsGetVelocity(bulletPhysics);
    Vector2DScale((Vector2D*)velocity, velocity, spaceshipWeaponBulletSpeed);


    EntityContainerAddEntity(container, bullet);
}



//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------

