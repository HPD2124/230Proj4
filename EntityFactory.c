//------------------------------------------------------------------------------
//
// File Name:	EntityFactory.c
// Author(s):	Harrison Parrish (harrison.parrish)
// Project:		Project 0
// Course:		CS230S25
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "EntityFactory.h"
#include "Entity.h"
#include "Stream.h"
#include "string.h"
#include "EntityContainer.h"

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
static EntityContainer* archetypes = NULL;
//------------------------------------------------------------------------------
// Private Function Declarations:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------
Entity* EntityFactoryBuild(const char* entityName) {
    if (!entityName) {
        return NULL;
    }


    if (!archetypes) {
        archetypes = EntityContainerCreate();
    }


    Entity* archetype = EntityContainerFindByName(archetypes, entityName);
    if (!archetype) {
        char pathName[FILENAME_MAX] = "";
        sprintf_s(pathName, _countof(pathName), "Data/%s.txt", entityName);

        Stream stream = StreamOpen(pathName);
        if (!stream) {
            return NULL;
        }

        const char* token = StreamReadToken(stream);
        if (token && strncmp(token, "Entity", _countof("Entity") - 1) == 0) {
            archetype = EntityCreate();
            EntityRead(archetype, stream);
            EntityContainerAddEntity(archetypes, archetype);
        }

        StreamClose(&stream);
    }

    return archetype ? EntityClone(archetype) : NULL;
}

void EntityFactoryFreeAll() {
    if (archetypes) {
        EntityContainerFreeAll(archetypes);
        archetypes = NULL;
    }
}
// Initialize the ...
void EntityFactoryInit()
{
}

// Update the ...
// Params:
//	 dt = Change in time (in seconds) since the last game loop.
void EntityFactoryUpdate(float dt)
{
	/* Tell the compiler that the 'dt' variable is unused. */
	UNREFERENCED_PARAMETER(dt);
}

// Shutdown the ...
void EntityFactoryExit()
{
}

//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------

