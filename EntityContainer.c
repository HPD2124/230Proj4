//------------------------------------------------------------------------------
//
// File Name:	EntityContainer.c
// Author(s):	Harrison Parrish (harrison.parrish)
// Project:		Project 0
// Course:		CS230S25
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "EntityContainer.h"
#include "Entity.h"
#define entityArraySize 100

typedef struct EntityContainer
{
    // This variable is not required but could be used for tracking the number
    //   of Entities currently in the list.  Instructions on how to do this
    //   are included in the function headers.
    unsigned entityCount;

    // This variable is not required but could be used for different purposes.
    // - For storing the maximum size of the container.
    // - For tracking peak usage of the container, used for testing purposes.
    unsigned entityMax;

    // This list can be a fixed-length array (minimum size of 100 entries)
    // or a dynamically sized array, such as a linked list.
    // (NOTE: The implementation details are left up to the student.  However,
    //    it is your responsiblity to ensure that memory is handled correctly.)
    Entity* entities[entityArraySize];

} EntityContainer;
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
EntityContainer* EntityContainerCreate() {
    EntityContainer* container = (EntityContainer*)calloc(1, sizeof(EntityContainer));
    return container;
}

void EntityContainerFree(EntityContainer** entities) {
    if (entities && *entities) {
        EntityContainerFreeAll(*entities);
        free(*entities);
        *entities = NULL;
    }
}

bool EntityContainerAddEntity(EntityContainer* container, Entity* entity) {
    if (!container || !entity || container->entityCount >= container->entityMax) {
        return false;
    }
    container->entities[container->entityCount++] = entity;
    return true;
}

Entity* EntityContainerFindByName(const EntityContainer* container, const char* entityName) {
    if (!container || !entityName) {
        return NULL;
    }
    for (unsigned i = 0; i < container->entityCount; i++) {
        if (EntityIsNamed(container->entities[i], entityName)) {
            return container->entities[i];
        }
    }
    return NULL;
}



bool EntityContainerIsEmpty(const EntityContainer* container) {
    return container && container->entityCount == 0;
}

void EntityContainerUpdateAll(EntityContainer* container, float dt) {
    if (!container) return;
    for (unsigned i = 0; i < container->entityCount; i++) {
        EntityUpdate(container->entities[i], dt);
        if (EntityIsDestroyed(container->entities[i])) {
          
            EntityFree(&container->entities[i]);
            for (unsigned j = i; j < container->entityCount - 1; j++) {
                container->entities[j] = container->entities[j + 1];
            }
            EntityFree(&container->entities[i]);
            container->entities[container->entityCount - 1] = NULL; 
            container->entityCount--;
            i--;
        }
    }
}

void EntityContainerRenderAll(const EntityContainer* container) {
    if (!container) return;
    for (unsigned i = 0; i < container->entityCount; i++) {
        EntityRender(container->entities[i]);
    }
}

void EntityContainerFreeAll(EntityContainer* container) {
    if (!container) return;
    for (unsigned i = 0; i < container->entityCount; i++) {
        
        EntityFree(container->entities);
    }
    container->entityCount = 0;
}

// Initialize the ...
void EntityContainerInit()
{
}

// Update the ...
// Params:
//	 dt = Change in time (in seconds) since the last game loop.
void EntityContainerUpdate(float dt)
{
	/* Tell the compiler that the 'dt' variable is unused. */
	UNREFERENCED_PARAMETER(dt);
}

// Shutdown the ...
void EntityContainerExit()
{
}

//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------

