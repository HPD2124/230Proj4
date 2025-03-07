//------------------------------------------------------------------------------
//
// File Name:	Sprite.c
// Author(s):	Harrison Parrish (harrison.parrish)
// Project:		Project 0
// Course:		CS230S25
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "Sprite.h"
#include "Mesh.h"
#include "DGL.h"

#include "Vector2D.h"
#include "Transform.h"
#include "SpriteSource.h"
#include "Stream.h"
#include "Trace.h"
#include "Matrix2D.h"
#include"MeshLibrary.h"


//------------------------------------------------------------------------------
// Private Constants:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Structures:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Variables:
//------------------------------------------------------------------------------

typedef struct Sprite
{
	unsigned int frameIndex;
	float alpha;
	const SpriteSource* spriteSource;
	const Mesh* mesh;
	char* text;  // Added for text display functionality

} Sprite;

//------------------------------------------------------------------------------
// Private Variables:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Function Declarations:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------


Sprite* SpriteCreate(void) {
	Sprite* sprite = calloc(1, sizeof(Sprite));

	if (sprite) {
		sprite->alpha = 1.0f;
		sprite->text = NULL; 
	}

	return sprite;
}

void SpriteFree(Sprite** sprite) {
	if (sprite && *sprite) {
		free((*sprite)->text);  
		free(*sprite);
		*sprite = NULL;
	}
}

void SpriteRead(Sprite* sprite, Stream stream) {
	if (sprite && stream) {
		sprite->frameIndex = StreamReadInt(stream);
		sprite->alpha = StreamReadFloat(stream);

		char meshName[256];
		const char* token = StreamReadToken(stream);
		strcpy_s(meshName, sizeof(meshName),token);

		const Mesh* mesh = MeshLibraryBuild(meshName); 
		if (mesh) {
			SpriteSetMesh(sprite, mesh);
		}
	}
}



void SpriteRender(const Sprite* sprite, Transform* transform) {
	if (sprite && transform) {
		if (sprite->spriteSource) {
			DGL_Graphics_SetShaderMode(DGL_PSM_TEXTURE, DGL_VSM_DEFAULT);
			SpriteSourceSetTexture(sprite->spriteSource);
			SpriteSourceSetTextureOffset(sprite->spriteSource, sprite->frameIndex);
		}
		else {
			DGL_Graphics_SetShaderMode(DGL_PSM_COLOR, DGL_VSM_DEFAULT);
		}
		DGL_Graphics_SetCB_Alpha(sprite->alpha);
		DGL_Graphics_SetCB_TintColor(&(DGL_Color) { 0.0f, 0.0f, 0.0f, 0.0f });

		if (sprite->text == NULL) {
			Matrix2D matrix = *TransformGetMatrix(transform);
			DGL_Graphics_SetCB_TransformMatrix(&matrix);
			//DGL_Graphics_SetCB_TransformData(TransformGetTranslation(transform), TransformGetScale(transform), TransformGetRotation(transform));
			MeshRender(sprite->mesh);
		}
		else {
			Matrix2D matrix = *TransformGetMatrix(transform);
			
			Matrix2D offset;
			Matrix2DTranslate(&offset, TransformGetScale(transform)->x, 0);
			//const Vector2D* scale = TransformGetScale(transform);
			Matrix2DConcat(&matrix, &offset, &matrix);


			const char* text = sprite->text;
			//while (*text) {
			//	unsigned int frameIndex = *text - ' ';
			//	SpriteSourceSetTextureOffset(sprite->spriteSource, frameIndex);


			//	MeshRender(sprite->mesh);

			//	text++;
			//	Matrix2D finalMatrix;
			//	Matrix2DConcat(&finalMatrix, &offset, &matrix);
			//	DGL_Graphics_SetCB_TransformMatrix(&finalMatrix);
			//	Matrix2DConcat(&matrix, &offset, &matrix);
			while (*text) {
				
				unsigned int frameIndex = *text - ' ';

			
				SpriteSourceSetTextureOffset(sprite->spriteSource, frameIndex);

			
				Matrix2D finalMatrix;
				Matrix2DConcat(&finalMatrix, &offset, &matrix);

				DGL_Graphics_SetCB_TransformMatrix(&finalMatrix);

				MeshRender(sprite->mesh);

			
				text++;

				
				Matrix2DConcat(&matrix, &offset, &matrix);
			}
		}
	}
}



float SpriteGetAlpha(const Sprite* sprite) {
	if (sprite) {
		return sprite->alpha;
	}
	return 0.0f;
}

void SpriteSetAlpha(Sprite* sprite, float alpha) {
	if (sprite) {
		sprite->alpha = max(0.0f, min(alpha, 1.0f));
	}
}

void SpriteSetFrame(Sprite* sprite, unsigned int frameIndex) {
	if (sprite) {
		if (frameIndex >= 0 && frameIndex < SpriteSourceGetFrameCount(sprite->spriteSource) - 1) {
			sprite->frameIndex = frameIndex;
			TraceMessage("SpriteSetFrame: frame index = %d", frameIndex);
		}
	}
	// Set a Sprite's current frame.
// (NOTE: You must verify that the frameIndex parameter is within the
//	range [0 to frame count - 1] before changing the Sprite's frame index!)
// Params:
//	 sprite = Pointer to the Sprite component.
//   frameIndex = New frame index for the Sprite (0 .. frame count - 1).
// ADDITIONAL REQUIREMENTS:
// - This function must make the following function call:
//     TraceMessage("SpriteSetFrame: frame index = %d", frameIndex);
}

void SpriteSetMesh(Sprite* sprite, const Mesh* mesh) {
	if (sprite && mesh) {
		sprite->mesh = mesh;
	}
}

void SpriteSetSpriteSource(Sprite* sprite, const SpriteSource* spriteSource) {
	if (sprite) {
		sprite->spriteSource = spriteSource;
	}
}


void SpriteSetText(Sprite* sprite, const char* text) {
	if (sprite == NULL) {
		return;  
	}


	if (text == NULL) {
		if (sprite->text != NULL) {
			free(sprite->text);  
			sprite->text = NULL; 
		}
	}
	else {
		
		size_t textLength = strlen(text) + 1; 
		char* newText = (char*)malloc(textLength);
		if (newText) {
			strcpy_s(newText, textLength, text);
			sprite->text = newText;  
		}
	}
}
Sprite* SpriteClone(const Sprite* other) {
	if (!other) {
		return NULL;
	}


	Sprite* clone = (Sprite*)calloc(1, sizeof(Sprite));
	if (!clone) {
		return NULL;
	}


	clone->frameIndex = other->frameIndex;
	clone->alpha = other->alpha;
	clone->spriteSource = other->spriteSource;
	clone->mesh = other->mesh;


	if (other->text) {
		size_t textLength = strlen(other->text) + 1;
		clone->text = (char*)malloc(textLength);
		if (clone->text) {
			strcpy_s(clone->text, textLength, other->text);
		}
		else {
			free(clone);  
			return NULL;
		}
	}
	else {
		clone->text = NULL;
	}

	return clone;
}
