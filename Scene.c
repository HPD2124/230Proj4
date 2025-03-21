//------------------------------------------------------------------------------
//
// File Name:	Scene.c
// Author(s):	Harrison Parrish (harrison.parrish)
// Project:		Project 4
// Course:		CS230S25
//
// Copyright � 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "stdafx.h"
#include <assert.h>

#include "Trace.h"
#include "EntityContainer.h"
#include "Scene.h"
#include "SceneSystem.h"
#include "Entity.h"
#include "MeshLibrary.h"

//------------------------------------------------------------------------------
// Private Constants:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Typedefs:
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
static EntityContainer* entities = NULL;
//------------------------------------------------------------------------------
// Private Function Declarations:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// Verify that a scene is valid (no NULL pointers).
// Params:
//   scene	Pointer to the scene to be checked.
bool SceneIsValid(const Scene* scene)
{
	// This function should never be called with a "NULL" scene.
	assert(scene && "SceneIsValid Error: A NULL pointer was passed as a parameter.");

	if (scene->name && scene->load && scene->init && scene->update && scene->render && scene->exit && scene->unload)
	{
		return true;
	}

	return false;
}

// Load the scene.
void SceneLoad(const Scene* scene)
{

	// Verify that the function pointer is valid.
	if (scene && (scene->load != NULL))
	{
		entities = EntityContainerCreate();

		// TODO: Call TraceMessage, passing the format string "%s: Load" and the name of the scene.
		TraceMessage("%s: Load", scene -> name);
		MeshLibraryInit();
		// Execute the Load function.
		(*scene->load)();
	}
}

// Initialize the scene.
void SceneInit(const Scene* scene)
{
	// Verify that the function pointer is valid.
	if (scene && (scene->init != NULL))
	{
		// TODO: Call TraceMessage, passing the format string "%s: Init" and the name of the scene.
		TraceMessage("%s: Init", scene->name);
		// Execute the Init function.
		(*scene->init)();
	}
}

// Update the scene.
void SceneUpdate(const Scene* scene, float dt)
{
	// Verify that the function pointer is valid.
	if (scene && (scene->update != NULL))
	{
		// TODO: Call TraceMessage, passing the format string "%s: Update" and the name of the scene.
		TraceMessage("%s: Update", scene->name);
		// Execute the Update function.
		(*scene->update)(dt);
		EntityContainerUpdateAll(entities, dt);
	}
}

// Render the scene.
void SceneRender(const Scene* scene)
{
	// Verify that the function pointer is valid.
	if (scene && (scene->render != NULL))
	{
		// TODO: Call TraceMessage, passing the format string "%s: Render" and the name of the scene.
		TraceMessage("%s: Render", scene->name);
		// Execute the Render function.
		(*scene->render)();
		EntityContainerRenderAll(entities);
	}
}

// Exit the scene.
void SceneExit(const Scene* scene)
{
	// Verify that the function pointer is valid.
	if (scene && (scene->exit != NULL))
	{
		// TODO: Call TraceMessage, passing the format string "%s: Exit" and the name of the scene.
		TraceMessage("%s: Exit", scene->name);
		// Execute the Exit function.
		(*scene->exit)();
		EntityContainerFreeAll(entities);
	}
}

// Unload the scene.
void SceneUnload(const Scene* scene)
{
	// Verify that the function pointer is valid.
	if (scene && (scene->unload != NULL))
	{
		// TODO: Call TraceMessage, passing the format string "%s: Unload" and the name of the scene.
		TraceMessage("%s: Unload", scene->name);
		// Execute the Unload function.
		(*scene->unload)();
		EntityContainerFree(&entities);
		MeshLibraryFreeAll();
	}
}

// Restart the active scene.
void SceneRestart(void)
{
	// Tell the Scene System to restart the active scene.
	SceneSystemRestart();
}



void SceneAddEntity(Entity* entity) {
	if (!entities) {
		entities = EntityContainerCreate();
	}
	EntityContainerAddEntity(entities, entity);
}

//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------

