//------------------------------------------------------------------------------
//
// File Name:	SpriteSource.c
// Author(s):	Harrison Parrish (harrison.parrish)
// Project:		Project 0
// Course:		CS230S25
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "SpriteSource.h"
#include "DGL.h"
#include "Vector2D.h"

//------------------------------------------------------------------------------
// Private Constants:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Structures:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Variables:
//------------------------------------------------------------------------------

typedef struct SpriteSource
{
	int	numRows;
	int	numCols;
	const DGL_Texture* texture;

} SpriteSource;

//------------------------------------------------------------------------------
// Private Variables:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Function Declarations:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------


SpriteSource* SpriteSourceCreate() {
	SpriteSource* sps = calloc(1, sizeof(SpriteSource));

	if (sps) {
		sps->numRows = 1;
		sps->numCols = 1;
	}

	return sps;
}

void SpriteSourceFree(SpriteSource** spriteSource) {
	SpriteSource* sps = *spriteSource;
	DGL_Texture** texture = &(sps->texture);
	DGL_Graphics_FreeTexture(texture);
	free(*spriteSource);
	*spriteSource = NULL;
}

void SpriteSourceLoadTexture(SpriteSource* spriteSource, int numCols, int numRows, const char* textureName) {
	if (spriteSource && textureName) {
		char filePath[128];
		sprintf_s(filePath, sizeof(filePath), "Assets/%s", textureName);

		spriteSource->texture = DGL_Graphics_LoadTexture(filePath);
		spriteSource->numCols = numCols;
		spriteSource->numRows = numRows;
	}
}

unsigned SpriteSourceGetFrameCount(const SpriteSource* spriteSource) {
	if (spriteSource) {
		return (spriteSource->numCols * spriteSource->numRows);
	}
	return 0;
}

void SpriteSourceGetUV(const SpriteSource* spriteSource, unsigned int frameIndex, float* u, float* v) {
	if (spriteSource) {
		float uSize = 1.0f / spriteSource->numCols;
		float vSize = 1.0f / spriteSource->numRows;

		*u = uSize * (frameIndex % spriteSource->numCols);
		*v = vSize * (frameIndex / spriteSource->numCols);
	}
}

void SpriteSourceSetTexture(const SpriteSource* spriteSource) {
	DGL_Graphics_SetTexture(spriteSource->texture);
}

void SpriteSourceSetTextureOffset(const SpriteSource* spriteSource, unsigned frameIndex) {
	Vector2D OffsetVector = { 0.0f,0.0f };


	SpriteSourceGetUV(spriteSource, frameIndex, &OffsetVector.x, &OffsetVector.y);

	DGL_Graphics_SetCB_TextureOffset(&OffsetVector);
}
