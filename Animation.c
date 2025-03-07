//------------------------------------------------------------------------------
//
// File Name:	Animation.c
// Author(s):	Harrison Parrish (harrison.parrish)
// Project:		Project 0
// Course:		CS230S25
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "Animation.h"
#include "Stream.h"
#include "Sprite.h"
#include "Entity.h"
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
static void AnimationAdvanceFrame(Animation* animation);
//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------

typedef struct Animation {
    Entity* parent;
    unsigned int frameIndex;
    unsigned int frameCount;
    float frameDelay;
    float frameDuration;
    bool isRunning;
    bool isLooping;
    bool isDone;
} Animation;

// Dynamically allocate a new Animation component.
Animation* AnimationCreate(void) {
    return (Animation*)calloc(1, sizeof(Animation));
}

// Free the memory associated with an Animation component.
void AnimationFree(Animation** animation) {
    if (animation && *animation) {
        free(*animation);
        *animation = NULL;
    }
}

// Read the properties of an Animation component from a file.
void AnimationRead(Animation* animation, Stream stream) {
    //if (!animation || !stream) return;
    //animation frame index
   animation->frameIndex = StreamReadInt(stream);
   animation->frameCount = StreamReadInt(stream);
   animation->frameDelay = StreamReadFloat(stream);
   animation->frameDuration = StreamReadFloat(stream);
   animation->isRunning = StreamReadBoolean(stream);
   animation->isLooping = StreamReadBoolean(stream);
}

// Set the parent Entity for an Animation component.
void AnimationSetParent(Animation* animation, Entity* parent) {
    if (!animation) return;
    animation->parent = parent;
}

// Play a simple animation sequence.
void AnimationPlay(Animation* animation, int frameCount, float frameDuration, bool isLooping) {
    if (!animation) return;
    
    animation->frameIndex = 0;
    animation->frameCount = frameCount;
    animation->frameDuration = frameDuration;
    animation->frameDelay = 0;
    animation->isLooping = isLooping;
    animation->isRunning = true;
    animation->isDone = false;
    SpriteSetFrame(EntityGetSprite(animation->parent), 0);
}

// Update the animation.
void AnimationUpdate(Animation* animation, float dt) {
    if (!animation || !animation->isRunning) return;
    
    animation->isDone = 0;
    if (animation->isRunning) {
        animation->frameDelay -= dt;
        if (animation->frameDelay <= 0) {
            AnimationAdvanceFrame(animation);
        }
    }

    // Update the animation.
// Params:
//	 animation = Pointer to the Animation component.
//	 dt = Change in time (in seconds) since the last game loop.
}
static void AnimationAdvanceFrame(Animation* animation) {
    if (animation) 
    {
        animation -> frameIndex++;
        if (animation->frameIndex >= animation->frameCount)
        {
            if (animation->isLooping) 
            {
                animation->frameIndex = 0;
         

            }
            else 
            {
                animation->frameIndex = animation->frameCount - 1;
                animation->isRunning = false;
            }
            animation->isDone = true;
        }
        if (animation->isRunning) 
        {
            SpriteSetFrame(EntityGetSprite(animation->parent), animation->frameIndex);
            animation->frameDelay += animation-> frameDuration;

        }
        else animation->frameDelay = 0;
    }
    else return;
}

// Determine if the animation has reached the end of its sequence.
bool AnimationIsDone(const Animation* animation) {

    // Determine if the animation has reached the end of its sequence.
    // Params:
    //	 animation = Pointer to the Animation component.
    // Returns:
    //	 If the Animation pointer is valid,
    //		then return the value in isDone,
    //		else return false.
    if (animation) {
        return animation->isDone;
    }
    else return false;
}
Animation* AnimationClone(const Animation* other) {
   
    if (!other) return NULL;


    Animation* clone = (Animation*)calloc(1, sizeof(Animation));
    if (!clone) return NULL; 


    clone->parent = other->parent;
    clone->frameIndex = other->frameIndex;
    clone->frameCount = other->frameCount;
    clone->frameDelay = other->frameDelay;
    clone->frameDuration = other->frameDuration;
    clone->isRunning = other->isRunning;
    clone->isLooping = other->isLooping;
    clone->isDone = other->isDone;


    return clone;
}
