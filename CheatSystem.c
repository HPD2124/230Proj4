//------------------------------------------------------------------------------
//
// File Name:	CheatSystem.c
// Author(s):	Harrison Parrish (harrison.parrish)
// Project:		Project 0
// Course:		CS230S25
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "stdafx.h"
#include <Windows.h>
#include "SceneSystem.h"
#include "BaseSystem.h"
#include "CheatSystem.h"
#include "Level1Scene.h"
#include "Level2Scene.h"
#include "SandboxScene.h"
#include "DemoScene.h"
#include "AsteroidsScene.h"
//------------------------------------------------------------------------------
// Private Constants:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Structures:
//------------------------------------------------------------------------------
typedef struct
{
	char key;
	const Scene* (*getInstance)(void);
} CheatSystemKeyBinding;




static const CheatSystemKeyBinding keyBindings[] = {
	{'3', AsteroidsSceneGetInstance },
	{ '1', Level1SceneGetInstance },
	{ '2', Level2SceneGetInstance },
	{ '9', SandboxSceneGetInstance },
	{ '0', DemoSceneGetInstance }
};

//------------------------------------------------------------------------------
// Public Variables:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Function Declarations:
//------------------------------------------------------------------------------

//  System-specific State functions:
static bool CheatSystemInit(void);
static void CheatSystemUpdate(float dt);
static void CheatSystemRender(void);
static void CheatSystemExit(void);

//------------------------------------------------------------------------------
// Private Variables:
//------------------------------------------------------------------------------

static BaseSystem instance =
{
	"CheatSystem", CheatSystemInit, CheatSystemUpdate, CheatSystemRender, CheatSystemExit
};

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// Get the instance of the Cheat System.
// Returns:
//	 Pointer to the abstract BaseSystem for this derived System.
BaseSystem* CheatSystemGetInstance()
{
	return &instance;
}


//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------

// Initialize the Cheat System.
// Returns:
//   Return "true" if the system was initialized successfully, otherwise return "false".
static bool CheatSystemInit(void)
{
	// Return false if the system was NOT initialized successfully.

	// Return true if the system was initialized successfully.
	return true;
}

// Update the Cheat System.
// Params:
//	 dt = Change in time (in seconds) since the last game loop.
static void CheatSystemUpdate(float dt)
{
	UNREFERENCED_PARAMETER(dt);
	for (int i = 0; i < sizeof(keyBindings) / sizeof(keyBindings[0]); i++)
	{
		if (GetAsyncKeyState(keyBindings[i].key))
		{
			SceneSystemSetNext(keyBindings[i].getInstance());
			break;
		}
	}
}


// Render any objects associated with the Cheat System.
static void CheatSystemRender(void)
{
}

// Shutdown the Cheat System.
//   (HINT: Free any allocated resources and/or close any opened files).
static void CheatSystemExit(void)
{
}

