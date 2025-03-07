//------------------------------------------------------------------------------
//
// File Name:	SandboxScene.c
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
#include "SandboxScene.h"
#include "Vector2D.h"
#include "Stream.h"
#include "Trace.h"

//------------------------------------------------------------------------------
// Private Constants:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Structures:
//------------------------------------------------------------------------------

typedef struct SandboxScene
{
	// WARNING: The base class must always be included first.
	Scene	base;

	// Add any scene-specific variables second.

} SandboxScene;

//------------------------------------------------------------------------------
// Public Variables:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Variables:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Function Declarations:
//------------------------------------------------------------------------------

static void SandboxSceneLoad(void);
static void SandboxSceneInit(void);
static void SandboxSceneUpdate(float dt);
static void SandboxSceneExit(void);
static void SandboxSceneUnload(void);
static void SandboxSceneRender(void);

//------------------------------------------------------------------------------
// Instance Variable:
//------------------------------------------------------------------------------

static SandboxScene instance =
{
	// Initialize the base structure:
	{ "Sandbox", SandboxSceneLoad, SandboxSceneInit, SandboxSceneUpdate, SandboxSceneRender, SandboxSceneExit, SandboxSceneUnload },

	// Initialize any scene-specific variables:
};

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// Get the instance of the Sandbox Scene.
// Returns:
//	 Pointer to the base Scene structure for this derived Scene.
const Scene* SandboxSceneGetInstance(void)
{
	return &(instance.base);
}

//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------
static void SandboxSceneTraceFloat(const char* text, float f) {
    if (text != NULL) {
        TraceMessage("Vector Test: %s = %f", text, f);
    }
    else {
        TraceMessage("Null text provided to SandboxSceneTraceFloat ");
    }
}

static void SandboxSceneTraceVector(const char* text, const Vector2D* v)
{
    if (text == NULL || v == NULL)
    {
        TraceMessage("Error: Invalid input to SandboxSceneTraceVector (text or vector is NULL) ");
        return;
    }

    TraceMessage("Vector Test: %s = [%f, %f]", text, v->x, v->y);
}
// Load any resources used by the scene.
static void SandboxSceneLoad(void)
{
}

// Initialize the entities and variables used by the scene.
static void SandboxSceneInit()
{
}


// Update the the variables used by the scene.
// Params:
//	 dt = Change in time (in seconds) since the last game loop.


static void SandboxSceneUpdate(float dt) 
{
    
        UNREFERENCED_PARAMETER(dt);
        Stream vectorTests = StreamOpen("Data/VectorTests.txt");
        if (vectorTests != NULL) {
            Vector2D vec;
            Vector2DZero(&vec);
            SandboxSceneTraceVector("Vector2DZero", &vec);

            Vector2DSet(&vec, 1.5f, 1.0f);
            SandboxSceneTraceVector("Vector2DSet", &vec);

            Vector2DNeg(&vec, &vec);
            SandboxSceneTraceVector("Vector2DNeg", &vec);

            Vector2DAdd(&vec, &vec, &vec);
            SandboxSceneTraceVector("Vector2DAdd", &vec);

            Vector2DSub(&vec, &vec, &vec);
            SandboxSceneTraceVector("Vector2DSub", &vec);

            StreamReadVector2D(vectorTests,&vec);
            SandboxSceneTraceVector("StreamReadVector2D", &vec);

            Vector2DNormalize(&vec, &vec);
            SandboxSceneTraceVector("Vector2DNormalize", &vec);

            float scale = StreamReadFloat(vectorTests);
            SandboxSceneTraceFloat("StreamReadFloat", scale);

            Vector2DScale(&vec, &vec, scale);
            SandboxSceneTraceVector("Vector2DScale", &vec);

            Vector2DScaleAdd(&vec, &vec, scale, &vec);
            SandboxSceneTraceVector("Vector2DScaleAdd", &vec);

            Vector2DScaleSub(&vec, &vec, scale, &vec);
            SandboxSceneTraceVector("Vector2DScaleSub", &vec);

            float length = Vector2DLength(&vec);
            SandboxSceneTraceFloat("Vector2DLength", length);

            float squareLength = Vector2DSquareLength(&vec);
            SandboxSceneTraceFloat("Vector2DSquareLength", squareLength);

            Vector2D vec1, vec2;
            StreamReadVector2D(vectorTests, &vec1);
            SandboxSceneTraceVector("StreamReadVector2D", &vec1);
            StreamReadVector2D(vectorTests, &vec2);
            SandboxSceneTraceVector("StreamReadVector2D", &vec2);

            float distance = Vector2DDistance(&vec1, &vec2);
            SandboxSceneTraceFloat("Vector2DDistance", distance);

            float squareDistance = Vector2DSquareDistance(&vec1, &vec2);
            SandboxSceneTraceFloat("Vector2DSquareDistance", squareDistance);

            float dotProduct = Vector2DDotProduct(&vec1, &vec2);
            SandboxSceneTraceFloat("Vector2DDotProduct", dotProduct);

            Vector2D angleVec;
            float angleDeg = StreamReadFloat(vectorTests);
            SandboxSceneTraceFloat("StreamReadFloat", angleDeg);

            Vector2DFromAngleDeg(&angleVec, angleDeg);
            SandboxSceneTraceVector("Vector2DFromAngleDeg", &angleVec);

            float angleRad = StreamReadFloat(vectorTests);
            SandboxSceneTraceFloat("StreamReadFloat", angleRad);

            Vector2DFromAngleRad(&angleVec, angleRad);
            SandboxSceneTraceVector("Vector2DFromAngleRad", &angleVec);

            float resultAngleRad = Vector2DToAngleRad(&angleVec);
            SandboxSceneTraceFloat("Vector2DToAngleRad", resultAngleRad);

            StreamClose(&vectorTests);
        }

        SceneSystemSetNext(NULL);
    

}



// Render any objects associated with the scene.
void SandboxSceneRender(void)
{
}

// Free any objects associated with the scene.
static void SandboxSceneExit()
{
}

// Unload any resources used by the scene.
static void SandboxSceneUnload(void)
{
}

