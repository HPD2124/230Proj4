//------------------------------------------------------------------------------
//
// File Name:	Entity.c
// Author(s):	Harrison Parrish (harrison.parrish)
// Project:		Project 0
// Course:		CS230S25
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "stdafx.h"
#include "Entity.h"
#include "Sprite.h"
#include "Stream.h"
#include "Transform.h"
#include "Physics.h"
#include "Animation.h"
#include "Behavior.h"
#include "BehaviorBullet.h"
#include "BehaviorSpaceship.h"
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

typedef struct Entity Entity;
typedef struct Physics Physics;
typedef struct Sprite Sprite;
typedef struct Transform Transform;
typedef struct Animation Animation;
typedef FILE* Stream;

struct Entity {
    char name[256];
    Physics* physics;
    Sprite* sprite;
    Transform* transform;
    Animation* animation;
    bool isDestroyed;
    Behavior* behavior;
};

Entity* EntityCreate(void) {
    Entity* entity = (Entity*)calloc(1, sizeof(Entity));
    return entity;
}

void EntityFree(Entity** entity) {
    if (entity && *entity) {
        free((*entity)->physics);
        free((*entity)->sprite);
        free((*entity)->transform);
        free((*entity)->animation);
        free((*entity)->behavior);
        free(*entity);
        
        *entity = NULL;
    }
}

void EntityAddPhysics(Entity* entity, Physics* physics) {
    if (entity) {
        entity->physics = physics;
    }
}

void EntityAddSprite(Entity* entity, Sprite* sprite) {
    if (entity) {
        entity->sprite = sprite;
    }
}

void EntityAddTransform(Entity* entity, Transform* transform) {
    if (entity) {
        entity->transform = transform;
    }
}

void EntityAddAnimation(Entity* entity, Animation* animation) {
    if (entity) {
        entity->animation = animation;
        AnimationSetParent(entity->animation, entity);
    }
}

void EntitySetName(Entity* entity, const char* name) {
    if (entity && name) {
        strcpy_s(entity->name, _countof(entity->name), name);
        entity->name[sizeof(entity->name) - 1] = '\0';
    }
}

const char* EntityGetName(const Entity* entity) {
    return entity ? entity->name : NULL;
}

Physics* EntityGetPhysics(const Entity* entity) {
    return entity ? entity->physics : NULL;
}

Sprite* EntityGetSprite(const Entity* entity) {
    return entity ? entity->sprite : NULL;
}

Transform* EntityGetTransform(const Entity* entity) {
    return entity ? entity->transform : NULL;
}

Animation* EntityGetAnimation(const Entity* entity) {
    return entity ? entity->animation : NULL;
}

void EntityUpdate(Entity* entity, float dt) {
    if (entity) {
        if (entity->physics && entity->transform) {
            PhysicsUpdate(entity->physics, entity->transform, dt);
        }
        if (entity->animation) {
            AnimationUpdate(entity->animation, dt);
        }
        if (entity->behavior) {
            BehaviorUpdate(entity->behavior, dt);
        }
    }
}

void EntityRender(Entity* entity) {
    if (entity && entity->sprite && entity->transform) {
        SpriteRender(entity->sprite, entity->transform);
    }
}

void EntityRead(Entity* entity, Stream stream) {
    if (entity && stream) {
        const char* token = StreamReadToken(stream);
        EntitySetName(entity, token);

        while (1) {
            token = StreamReadToken(stream);
            if (!token || strlen(token) == 0) {
                break;
            }

            if (strstr(token, "Transform") != NULL) {
                Transform* transform = TransformCreate();
                TransformRead(transform, stream);
                EntityAddTransform(entity, transform);
            }
            else if (strstr(token, "Physics") != NULL) {
                Physics* physics = PhysicsCreate();
                PhysicsRead(physics, stream);
                EntityAddPhysics(entity, physics);
            }
            else if (strstr(token, "Sprite") != NULL) {
                Sprite* sprite = SpriteCreate();
                SpriteRead(sprite, stream);
                EntityAddSprite(entity, sprite);
            }
            else if (strstr(token, "Animation") != NULL) {
                Animation* animation = AnimationCreate();
                AnimationRead(animation, stream);
                EntityAddAnimation(entity, animation);
            }
            else if (strstr(token, "BehaviorSpaceship") != NULL) {
                Behavior* behavior = BehaviorSpaceshipCreate();
                BehaviorRead(behavior, stream);
                EntityAddBehavior(entity, behavior);
            }
            else if (strstr(token, "BehaviorBullet") != NULL) {
                Behavior* behavior = BehaviorBulletCreate();
                BehaviorRead(behavior, stream);
                EntityAddBehavior(entity, behavior);
            } 

        } 

        StreamClose(&stream); 
    }
} 

Entity* EntityClone(const Entity* other) {
    if (!other) {
        return NULL;
    }

    Entity* clone = EntityCreate();
    if (!clone) {
        return NULL;
    }

    
    EntitySetName(clone, other->name);

  
    if (other->physics) {
        Physics* clonedPhysics = PhysicsClone(other->physics);
        EntityAddPhysics(clone, clonedPhysics);
    }

    if (other->sprite) {
        Sprite* clonedSprite = SpriteClone(other->sprite);
        EntityAddSprite(clone, clonedSprite);
    }

    if (other->transform) {
        Transform* clonedTransform = TransformClone(other->transform);
        EntityAddTransform(clone, clonedTransform);
    }

    if (other->animation) {
        Animation* clonedAnimation = AnimationClone(other->animation);
        EntityAddAnimation(clone, clonedAnimation);
    }
    if (other->behavior) {
        Behavior* clonedBehavior = BehaviorClone(other->behavior);
        EntityAddBehavior(clone, clonedBehavior);
    }

    return clone;
}

bool EntityIsDestroyed(const Entity* entity) {
    return entity ? entity->isDestroyed : false;
}

void EntityDestroy(Entity* entity) {
    if (entity) {
        entity->isDestroyed = 1;
    }
}
Behavior* EntityGetBehavior(const Entity* entity) {
    return entity ? entity->behavior : NULL;
}
bool EntityIsNamed(const Entity* entity, const char* name) {
    return (entity && name) ? strcmp(entity->name, name) == 0 : false;
}
void EntityAddBehavior(Entity* entity, Behavior* behavior) {
    if (entity && behavior) {
        entity->behavior = behavior;
        BehaviorSetParent(behavior, entity);
    }
}


void EntityInit()
{
}

void EntityExit()
{
}

//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------
