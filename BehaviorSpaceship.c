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
#include "BehaviorSpaceship.h"
#include <math.h>
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif


typedef enum {
    cSpaceshipInvalid = -1,
    cSpaceshipIdle,
    cSpaceshipThrust
} SpaceshipState;

//------------------------------------------------------------------------------
// Private Constants:
//------------------------------------------------------------------------------
static const float spaceshipAcceleration = 150.0f;
static const float spaceshipSpeedMax = 500.0f;
static const float spaceshipTurnRateMax = (float)(M_PI / 1.5f);

static const float spaceshipWeaponCooldownTime = 0.034f;
static const float spaceshipWeaponBulletSpeed = 750.0f;

//------------------------------------------------------------------------------
// Private Function Declarations:
//------------------------------------------------------------------------------
static void BehaviorSpaceshipOnInit(Behavior* behavior);
static void BehaviorSpaceshipOnUpdate(Behavior* behavior, float dt);
static void BehaviorSpaceshipOnExit(Behavior* behavior);
static void BehaviorSpaceshipUpdateRotation(Behavior* behavior, float dt);
static void BehaviorSpaceshipUpdateVelocity(Behavior* behavior, float dt);
static void BehaviorSpaceshipUpdateWeapon(Behavior* behavior, float dt);
static void BehaviorSpaceshipSpawnBullet(Behavior* behavior);

//------------------------------------------------------------------------------
// Public Variables:
//------------------------------------------------------------------------------
EntityContainer* container;

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

Behavior* BehaviorSpaceshipCreate(void) {
    Behavior* Spaceship = (Behavior*)calloc(1, sizeof(Behavior));
    if (!Spaceship) {
        return NULL;
    }

    Spaceship->stateCurr = cSpaceshipInvalid;
    Spaceship->stateNext = cSpaceshipInvalid;
    Spaceship->onInit = BehaviorSpaceshipOnInit;
    Spaceship->onUpdate = BehaviorSpaceshipOnUpdate;
    Spaceship->onExit = BehaviorSpaceshipOnExit;

    return Spaceship;
}

static void BehaviorSpaceshipOnInit(Behavior* behavior) {
    UNREFERENCED_PARAMETER(behavior);
}

static void BehaviorSpaceshipOnUpdate(Behavior* behavior, float dt) {
    UNREFERENCED_PARAMETER(dt);
    behavior->stateCurr = behavior->stateNext;
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
    default:
        break;

    }
}

static void BehaviorSpaceshipOnExit(Behavior* behavior) {
    UNREFERENCED_PARAMETER(behavior);
}

static void BehaviorSpaceshipUpdateRotation(Behavior* behavior, float dt) {
    UNREFERENCED_PARAMETER(dt);
    Physics* phys = EntityGetPhysics(behavior->parent);
    if (!phys) return;



    if (DGL_Input_KeyDown(VK_LEFT)) {
        PhysicsSetRotationalVelocity(phys, spaceshipTurnRateMax);
    }
    else if (DGL_Input_KeyDown(VK_RIGHT)) {
        PhysicsSetRotationalVelocity(phys, -spaceshipTurnRateMax);
    }
    else {
        PhysicsSetRotationalVelocity(phys, 0);
    }
}

static void BehaviorSpaceshipUpdateVelocity(Behavior* behavior, float dt) {
    Transform* transform = EntityGetTransform(behavior->parent);
    Physics* phys = EntityGetPhysics(behavior->parent);

    if (!transform || !phys) return;

    float spaceshipRotation = TransformGetRotation(transform);
    Vector2D rotation2D;
    Vector2DFromAngleRad(&rotation2D, spaceshipRotation);
    Vector2DNormalize(&rotation2D, &rotation2D);

    Vector2D velocity;
    velocity = *PhysicsGetVelocity(phys);

    float accel = spaceshipAcceleration * dt;
    Vector2DScale(&rotation2D, &rotation2D, accel);
    Vector2DAdd(&velocity, &velocity, &rotation2D);

    float speed = Vector2DLength(&velocity);
    if (speed > spaceshipSpeedMax) {
        Vector2DScale(&velocity, &velocity, spaceshipSpeedMax / speed);
    }

    PhysicsSetVelocity(phys, &velocity);
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
    if (!spaceship) return;

    Transform* spaceshipTransform = EntityGetTransform(spaceship);
    float spaceshipRotation = TransformGetRotation(spaceshipTransform);
    Transform* bulletTransform = TransformClone(spaceshipTransform);
    TransformSetTranslation(bulletTransform, TransformGetTranslation(spaceshipTransform));
    TransformSetRotation(bulletTransform, spaceshipRotation);

    Physics* bulletPhysics = EntityGetPhysics(bullet);
    Vector2D direction;
    Vector2DFromAngleRad(&direction, spaceshipRotation);

    Vector2D velocity;
    Vector2DScale(&velocity, &direction, spaceshipWeaponBulletSpeed);
    PhysicsSetVelocity(bulletPhysics, &velocity);

    EntityContainerAddEntity(container, bullet);
}
