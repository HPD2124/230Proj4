//------------------------------------------------------------------------------
//
// File Name:	Physics.c
// Author(s):	Harrison Parrish (harrison.parrish)
// Project:		Project 0
// Course:		CS230S25
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "Physics.h"
#include "Transform.h"
#include "Vector2D.h"
#include "Stream.h"
//------------------------------------------------------------------------------
// Private Constants:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Structures:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Variables:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Variables:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Function Declarations:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// Initialize the ...

typedef struct Physics {
    Vector2D acceleration;
    Vector2D velocity;
    Vector2D oldTranslation;
    float rotationalVelocity;
} Physics;


void PhysicsFree(Physics** physics) {
    if (physics && *physics) {
        free(*physics);
        *physics = NULL;
    }
}

void PhysicsRead(Physics* physics, Stream stream) {
    if (physics && stream) {
        StreamReadVector2D(stream, &(physics->acceleration));
        StreamReadVector2D(stream, &(physics->velocity));
    }
}

const Vector2D* PhysicsGetAcceleration(const Physics* physics) {
    return physics ? &physics->acceleration : NULL;
}

const Vector2D* PhysicsGetVelocity(const Physics* physics) {
    return physics ? &physics->velocity : NULL;
}

const Vector2D* PhysicsGetOldTranslation(Physics* physics) {
    return physics ? &physics->oldTranslation : NULL;
}

void PhysicsSetAcceleration(Physics* physics, const Vector2D* acceleration) {
    if (physics && acceleration) {
        physics->acceleration = *acceleration;
    }
}

void PhysicsSetVelocity(Physics* physics, const Vector2D* velocity) {
    if (physics && velocity) {
        physics->velocity = *velocity;
    }
}

void PhysicsUpdate(Physics* physics, Transform* transform, float dt) {
    if (physics && transform) {
        Vector2D newTranslation = { 0, 0 };
        float newRotation = 0.0f;

        newRotation = TransformGetRotation(transform) + (physics->rotationalVelocity * dt);
        TransformSetRotation(transform, newRotation);

        physics->oldTranslation = *TransformGetTranslation(transform);


        Vector2DScaleAdd(&(physics->velocity), &(physics->acceleration), dt, &(physics->velocity));
        Vector2DScale(&physics->velocity, &physics->velocity, 0.99f);
        Vector2DScaleAdd(&newTranslation, &(physics->velocity), dt, &(physics->oldTranslation));
        TransformSetTranslation(transform, &newTranslation);
        
    }
}

void PhysicsSetRotationalVelocity(Physics* physics, float rotationalVelocity) {
    if (physics) {
        physics->rotationalVelocity = rotationalVelocity;
    }
}
Physics* PhysicsClone(const Physics* other) {
    if (!other) return NULL;

    Physics* clone = (Physics*)malloc(sizeof(Physics));
    if (!clone) return NULL;

    *clone = *other; 
    return clone;
}
Physics* PhysicsCreate(void) {
    Physics* physics = (Physics*)calloc(1, sizeof(Physics));
    if (physics) {
        physics->rotationalVelocity = 0.0f;
    }
    return physics;
}
float PhysicsGetRotationalVelocity(const Physics* physics) {

    if (physics) {
        return physics->rotationalVelocity;
    }

    return 0.0f;
}
void PhysicsInit()
{
}


// Shutdown the ...
void PhysicsExit()
{
}

//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------

