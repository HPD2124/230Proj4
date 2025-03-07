//------------------------------------------------------------------------------
//
// File Name:	Behavior.c
// Author(s):	Harrison Parrish (harrison.parrish)
// Project:		Project 4
// Course:		CS230S25
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "Behavior.h"
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
void BehaviorInit()
{
}

// Update the ...
// Params:
//	 dt = Change in time (in seconds) since the last game loop.

// Shutdown the ...
void BehaviorExit()
{
}
Behavior* BehaviorClone(Behavior* other)
{
    if (other == NULL)
    {
        return NULL;  
    }

    Behavior* clone = (Behavior*)malloc(sizeof(Behavior));
    if (clone == NULL)
    {
        return NULL;  
    }

    clone->parent = other->parent;           
    clone->stateCurr = other->stateCurr;     
    clone->stateNext = other->stateNext;    
    clone->onInit = other->onInit;            
    clone->onUpdate = other->onUpdate;
    clone->onExit = other->onExit;
    clone->timer = other->timer;            

    return clone; 
}
void BehaviorFree(Behavior** behavior)
{
 
    if (behavior == NULL || *behavior == NULL)
    {
        return;  
    }

 
    free(*behavior);

   
    *behavior = NULL;
}
void BehaviorRead(Behavior* behavior, Stream stream)
{

    behavior->stateCurr = StreamReadInt(stream); 
    behavior->stateNext = StreamReadInt(stream);  


    behavior->timer = StreamReadFloat(stream);
}
void BehaviorSetParent(Behavior* behavior, Entity* parent)
{

    if (behavior != NULL)
    {
        behavior->parent = parent;  
    }
}
void BehaviorUpdate(Behavior* behavior, float dt)
{
    UNREFERENCED_PARAMETER(dt);
    if (behavior == NULL)
    {
        return;  
    }


    if (behavior->timer > 0)
    {
        behavior->timer -= dt; 
        if (behavior->timer < 0)
        {
            behavior->timer = 0; 
        }
    }


    if (behavior->onUpdate != NULL)
    {
        behavior->onUpdate(behavior, dt); 
    }
}


//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------

