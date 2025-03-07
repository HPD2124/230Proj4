//------------------------------------------------------------------------------
// File Name:	Level2Scene.c
// Author(s):	Harrison Parrish (harrison.parrish)
// Project:		Project 0
// Course:		CS230S25
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "stdafx.h"

#include "Scene.h"
#include "SceneSystem.h"
#include "Stream.h"
#include "Level2Scene.h"
#include "Level1Scene.h"
#include "SandboxScene.h"
#include "DemoScene.h"

#include "SpriteSource.h"
#include "Sprite.h"
#include "Mesh.h"
#include "Transform.h"
#include "Entity.h"
#include "EntityFactory.h"
#include "Vector2D.h"
#include "DGL.h"
#include "Physics.h"

//------------------------------------------------------------------------------
// Private Constants:
//------------------------------------------------------------------------------

static const float spaceshipSpeed = 500.0f;

//------------------------------------------------------------------------------
// Private Structures:
//------------------------------------------------------------------------------

typedef struct Level2Scene
{
	// WARNING: The base class must always be included first.
	Scene	base;

	Mesh* mesh;

	Entity* entity;

	// Add any scene-specific variables second.


} Level2Scene;

//------------------------------------------------------------------------------
// Public Variables:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Variables:
//------------------------------------------------------------------------------

Sprite* sprite;

//------------------------------------------------------------------------------
// Private Function Declarations:
//------------------------------------------------------------------------------

static void Level2SceneLoad(void);
static void Level2SceneInit(void);
static void Level2SceneUpdate(float dt);
static void Level2SceneExit(void);
static void Level2SceneUnload(void);
static void Level2SceneRender(void);

//------------------------------------------------------------------------------
// Instance Variable:
//------------------------------------------------------------------------------

static Level2Scene instance =
{
	// Initialize the base structure:
	{ "Level2", Level2SceneLoad, Level2SceneInit, Level2SceneUpdate, Level2SceneRender, Level2SceneExit, Level2SceneUnload },

	// Initialize any scene-specific variables:
	0,
	0
};

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// Get the instance of the Level2 Scene.
// Returns:
//	 Pointer to the base Scene structure for this derived Scene.
const Scene* Level2SceneGetInstance(void)
{
	return &(instance.base);
}

//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------


static void Level2SceneMovementController(Entity* entity) {
	Physics* physics = EntityGetPhysics(entity);
	Transform* transform = EntityGetTransform(entity);

	Vector2D velocity;
	Vector2D mousePosition;
	Vector2D translation;
	Vector2D shipToMouse;

	if (physics && transform) {
		mousePosition = DGL_Input_GetMousePosition();
		mousePosition = DGL_Camera_ScreenCoordToWorld(&mousePosition);

		translation = *TransformGetTranslation(transform);

		Vector2DSub(&shipToMouse, &mousePosition, &translation);
		Vector2DNormalize(&shipToMouse, &shipToMouse);
		TransformSetRotation(transform, Vector2DToAngleRad(&shipToMouse));
		Vector2DScale(&velocity, &shipToMouse, spaceshipSpeed);
		PhysicsSetVelocity(physics, &velocity);
	}
}


// Load any resources used by the scene.
static void Level2SceneLoad(void)
{
	instance.mesh = MeshCreate();
	MeshBuildSpaceship(instance.mesh);
}

// Initialize the entities and variables used by the scene.
static void Level2SceneInit()
{
	instance.entity = EntityFactoryBuild("SpaceshipHoming");

	if (instance.entity) {
		sprite = EntityGetSprite(instance.entity);
		SpriteSetMesh(sprite, instance.mesh);

		DGL_Graphics_SetBackgroundColor(&(DGL_Color) { 0.0f, 0.0f, 0.0f, 1.0f });
		DGL_Graphics_SetBlendMode(DGL_BM_BLEND);
	}
}

// Update the the variables used by the scene.
// Params:
//	 dt = Change in time (in seconds) since the last game loop.
static void Level2SceneUpdate(float dt)
{
	Level2SceneMovementController(instance.entity);
	EntityUpdate(instance.entity, dt);
}

// Render any objects associated with the scene.
void Level2SceneRender(void)
{
	EntityRender(instance.entity);
}

// Free any objects associated with the scene.
static void Level2SceneExit()
{
	EntityFree(&(instance.entity));
}

// Unload any resources used by the scene.
static void Level2SceneUnload(void)
{
	MeshFree(&(instance.mesh));
}