//------------------------------------------------------------------------------
//
// File Name:	Level1Scene.c
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
#include "DemoScene.h"
#include "Level1Scene.h"
#include "Level2Scene.h"
#include "SandboxScene.h"
#include "Animation.h"
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

static const float groundHeight = -150.0f;
static const float moveVelocity = 500.0f;
static const float jumpVelocity = 1000.0f;
static const Vector2D gravityNormal = { 0.0f, -1500.0f };
static const Vector2D gravityNone = { 0.0f, 0.0f };
static const float wallDistance = 462.0f;
static const float CheckSquareDistance = (75.0f * 75.0f);

//------------------------------------------------------------------------------
// Private Enums:
//------------------------------------------------------------------------------

typedef enum MonkeyStates {
	MonkeyInvalid = -1,
	MonkeyIdle,
	MonkeyWalk,
	MonkeyJump
} MonkeyStates;

//------------------------------------------------------------------------------
// Private Structures:
//------------------------------------------------------------------------------

typedef struct Level1Scene
{
	// WARNING: The base class must always be included first.
	Scene	base;
	Mesh* mesh1x1;
	Mesh* mesh;
	Mesh* mesh16x8;
	Mesh* mesh3x3;

	SpriteSource* spriteSourceIdle;
	SpriteSource* spriteSourceJump;
	SpriteSource* spriteSourceWalk;
	SpriteSource* spriteSourceLives;
	SpriteSource* spritesource;

	Entity* entity;
	Entity* monkeyEntity;
	Entity* livesTextEntity;
	// Add any scene-specific variables second.
	int numLives;
	MonkeyStates monkeyState;
	char livesBuffer[16];
} Level1Scene;

//------------------------------------------------------------------------------
// Private Variables:
//------------------------------------------------------------------------------



Entity* monkeyEntity = NULL;
Entity* livesTextEntity = NULL;
enum MonkeyStates monkeyState = MonkeyInvalid;

//------------------------------------------------------------------------------
// Private Function Declarations:
//------------------------------------------------------------------------------

static void Level1SceneLoad(void);
static void Level1SceneInit(void);
static void Level1SceneUpdate(float dt);
static void Level1SceneShutdown(void);
static void Level1SceneUnload(void);
static void Level1SceneRender(void);
static void Level1SceneMovementController(Entity* entity);
static void Level1SceneSetMonkeyState(Entity* entity, MonkeyStates newState);

//------------------------------------------------------------------------------
// Instance Variable:
//------------------------------------------------------------------------------

static Level1Scene instance =
{
	// Initialize the base structure:
	{ "Level1", Level1SceneLoad, Level1SceneInit, Level1SceneUpdate, Level1SceneRender, Level1SceneShutdown, Level1SceneUnload },

	// Initialize any scene-specific variables
	0
};

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

const Scene* Level1SceneGetInstance(void)
{
	return &(instance.base);
}

//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------

static void Level1SceneMovementController(Entity* entity) {
	Physics* physics = EntityGetPhysics(entity);
	Transform* transform = EntityGetTransform(entity);

	Vector2D velocity;
	Vector2D translation;

	if (physics && transform) {
		velocity = *(PhysicsGetVelocity(physics));

		if (DGL_Input_KeyDown(VK_RIGHT)) {
			velocity.x = moveVelocity;
			if (monkeyState != MonkeyJump) {
				Level1SceneSetMonkeyState(entity, MonkeyWalk);
			}
		}
		else if (DGL_Input_KeyDown(VK_LEFT)) {
			velocity.x = -moveVelocity;
			if (monkeyState != MonkeyJump) {
				Level1SceneSetMonkeyState(entity, MonkeyWalk);
			}
		}
		else {
			velocity.x = 0;
			if (monkeyState != MonkeyJump) {
				Level1SceneSetMonkeyState(entity, MonkeyIdle);
			}
		}

		if (DGL_Input_KeyTriggered(VK_UP)) {
			velocity.y = jumpVelocity;
			PhysicsSetAcceleration(physics, &gravityNormal);
			Level1SceneSetMonkeyState(entity, MonkeyJump);
		}

		translation = *(TransformGetTranslation(transform));

		if (translation.y < groundHeight) {
			translation.y = groundHeight;
			Vector2D tempVector = { translation.x, groundHeight };
			TransformSetTranslation(transform, &tempVector);
			velocity.y = 0;
			PhysicsSetAcceleration(physics, &gravityNone);
			Level1SceneSetMonkeyState(entity, MonkeyIdle);
	
		}
		
		PhysicsSetVelocity(physics, &velocity);
	}
}

static void Level1SceneSetMonkeyState(Entity* entity, MonkeyStates newState) {
	if (monkeyState != newState) {
		monkeyState = newState;

		Sprite* sprite = EntityGetSprite(entity);
		Animation* animation = EntityGetAnimation(entity);

		switch (newState) {
		case MonkeyIdle:
			SpriteSetMesh(sprite, instance.mesh1x1);
			SpriteSetSpriteSource(sprite, instance.spriteSourceIdle);
			AnimationPlay(animation, 1, 0.0f, false);
			break;

		case MonkeyWalk:
			SpriteSetMesh(sprite, instance.mesh3x3);
			SpriteSetSpriteSource(sprite, instance.spriteSourceWalk);
			AnimationPlay(animation, 8, 0.05f, true);
			break;

		case MonkeyJump:
			SpriteSetMesh(sprite, instance.mesh1x1);
			SpriteSetSpriteSource(sprite, instance.spriteSourceJump);
			AnimationPlay(animation, 1, 0.0f, false);
			break;

		default:
			break;
		}
	}
}
static void Level1SceneShutdown(void) {
	EntityFree(&monkeyEntity);
	EntityFree(&instance.entity);
	EntityFree(&livesTextEntity);
}

static void Level1SceneUnload(void) {
	SpriteSourceFree(&instance.spritesource);
	SpriteSourceFree(&instance.spriteSourceIdle);
	SpriteSourceFree(&instance.spriteSourceJump);
	SpriteSourceFree(&instance.spriteSourceLives);
	SpriteSourceFree(&instance.spriteSourceWalk);
	//SpriteSourceFree(&instance.spriteSourceLives);
	MeshFree(&instance.mesh1x1);
	MeshFree(&instance.mesh16x8);
	MeshFree(&instance.mesh3x3);
	MeshFree(&instance.mesh);
}
// Load any resources used by the scene.
static void Level1SceneLoad(void)
{
	Stream level1_lives = NULL;
	level1_lives = StreamOpen("Data/Level1_Lives.txt");
	if (level1_lives != NULL) {
		instance.numLives = StreamReadInt(level1_lives);
		StreamClose(&level1_lives);
	}

	instance.mesh1x1 = MeshCreate();
	MeshBuildQuad(instance.mesh1x1, 0.5f, 0.5f, 1.0f, 1.0f, "Mesh1x1");

	instance.mesh3x3 = MeshCreate();
	MeshBuildQuad(instance.mesh3x3, 0.5f, 0.5f, 1.0f / 3, 1.0f / 3, "Mesh3x3");
	instance.mesh16x8 = MeshCreate();
	MeshBuildQuad(instance.mesh16x8, 0.5f, 0.5f, 1.0f / 16, 1.0f / 8, "Mesh16x8");


	instance.spriteSourceIdle = SpriteSourceCreate();
	SpriteSourceLoadTexture(instance.spriteSourceIdle, 1, 1, "MonkeyIdle.png");

	instance.spriteSourceJump = SpriteSourceCreate();
	SpriteSourceLoadTexture(instance.spriteSourceJump, 1, 1, "MonkeyJump.png");

	instance.spriteSourceWalk = SpriteSourceCreate();
	SpriteSourceLoadTexture(instance.spriteSourceWalk, 3, 3, "MonkeyWalk.png");

	instance.spriteSourceLives = SpriteSourceCreate();
	SpriteSourceLoadTexture(instance.spriteSourceLives, 16, 8, "Roboto_Mono_black.png");

	instance.mesh = MeshCreate();
	MeshBuildQuad(instance.mesh, 0.5f, 0.5f, 1.0f, 1.0f, "Mesh1x1");

	instance.spritesource = SpriteSourceCreate();
	SpriteSourceLoadTexture(instance.spritesource, 1, 1, "PlanetTexture.png");
}

// Initialize the entities and variables used by the scene.
static void Level1SceneInit(void)
{
	instance.entity = EntityFactoryBuild("PlanetBounce");

	monkeyEntity = EntityFactoryBuild("Monkey");
	if (monkeyEntity) {
		monkeyState = MonkeyInvalid;
		Level1SceneSetMonkeyState(monkeyEntity, MonkeyIdle);
	}

	livesTextEntity = EntityFactoryBuild("MonkeyLivesText");
	if (livesTextEntity) {
		Sprite* livesSprite = EntityGetSprite(livesTextEntity);
		SpriteSetMesh(livesSprite, instance.mesh16x8);
		SpriteSetSpriteSource(livesSprite, instance.spriteSourceLives);
		sprintf_s(instance.livesBuffer, sizeof(instance.livesBuffer), "Lives: %d", instance.numLives);
		SpriteSetText(livesSprite, instance.livesBuffer);
	}

	if (instance.entity) {
		Sprite* planet = EntityGetSprite(instance.entity);
		SpriteSetMesh(planet, instance.mesh);
		SpriteSetSpriteSource(planet, instance.spritesource);
		SpriteSetFrame(planet, 0);
	}

	DGL_Graphics_SetBackgroundColor(&(DGL_Color) { 1.0f, 1.0f, 1.0f, 1.0f });
	DGL_Graphics_SetBlendMode(DGL_BM_BLEND);
}
// Update the the variables used by the scene.
// Params:
//	 dt = Change in time (in seconds) since the last game loop.
static bool Level1SceneIsColliding(const Entity* entityA, const Entity* entityB) {



	Vector2D monkeySpot = *TransformGetTranslation(EntityGetTransform(entityA));

	
	Vector2D planetSpot = *TransformGetTranslation(EntityGetTransform(entityB));

	float Distance = Vector2DSquareDistance(&monkeySpot, &planetSpot);

	if (Distance <= CheckSquareDistance)
		return TRUE;
	else {
		return FALSE;
	}
}

static void Level1SceneBounceController(Entity* entity)
{
	Vector2D position;
	Vector2D velocity;


	if (entity) {
		Physics* physics = EntityGetPhysics(entity);
		Transform* transform = EntityGetTransform(entity);
		if (physics && transform) {

			velocity = *PhysicsGetVelocity(physics);
			position = *TransformGetTranslation(transform);
			if (position.x <= -wallDistance) {
				position.x = -wallDistance;
				velocity.x = -velocity.x;

			}
			if (position.x >= wallDistance) {
				position.x = wallDistance;
				velocity.x = -velocity.x;
			}
			if (position.y <= groundHeight) {
				position.y = groundHeight + (groundHeight - position.y);
				velocity.y = -velocity.y;
			}
			PhysicsSetVelocity(physics, &velocity);
			TransformSetTranslation(transform, &position);
		}
	}
}



static void Level1SceneUpdate(float dt)
{
	
	EntityUpdate(instance.entity, dt);
	EntityUpdate(monkeyEntity, dt);
	EntityUpdate(livesTextEntity, dt);

	Level1SceneMovementController(monkeyEntity);
	Level1SceneBounceController(instance.entity);
		
	if (Level1SceneIsColliding(monkeyEntity, instance.entity)) {
		instance.numLives--;
		if (instance.numLives <= 0) {
			SceneSystemSetNext(Level2SceneGetInstance());
		}
		else {
			SceneSystemRestart();
		}
	}

}

void Level1SceneRender(void)
{
	EntityRender(monkeyEntity);
	EntityRender(instance.entity);

	EntityRender(livesTextEntity);
}