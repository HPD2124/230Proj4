//------------------------------------------------------------------------------
//
// File Name:	Transform.c
// Author(s):	Harrison Parrish (harrison.parrish)
// Project:		Project 0
// Course:		CS230S25
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "Transform.h"
#include "Vector2D.h"
#include "Stream.h"
#include "Trace.h"
#include "Matrix2D.h"

//------------------------------------------------------------------------------
// Private Constants:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Structures:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Variables:
//------------------------------------------------------------------------------

typedef struct Transform
{
	Vector2D	translation;
	float	rotation;
	Vector2D	scale;
	Matrix2D	matrix;
	bool	isDirty;

} Transform;

//------------------------------------------------------------------------------
// Private Variables:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Function Declarations:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// Allocate memory for Transform component
Transform* TransformCreate(void) {
	Transform* t = calloc(1, sizeof(Transform));

	if (t != NULL) {
		t->scale.x = 1.0f; t->scale.y = 1.0f;
		t->isDirty = true;
	}

	return t;
}

// Free memory for Transform
void TransformFree(Transform** transform) {
	if (transform != NULL && *transform != NULL) {
		free(*transform);
		*transform = NULL;
	}

}

// Read transform data from a stream
void TransformRead(Transform* transform, Stream stream) {
	if (transform != NULL && stream != NULL) {
		StreamReadVector2D(stream, &(transform->translation));
		transform->rotation = StreamReadFloat(stream);
		StreamReadVector2D(stream, &(transform->scale));
		transform->isDirty = true;
	}
	else {
		printf("Error in TransformRead: Invalid argument\n");
	}
}

// Get translation
const Vector2D* TransformGetTranslation(const Transform* transform) {
	if (transform != NULL) {
		return &(transform->translation);
	}
	return NULL;
}

// Get rotation
float TransformGetRotation(const Transform* transform) {
	if (transform != NULL) {
		return transform->rotation;
	}
	return 0.0f;
}

// Get scale
const Vector2D* TransformGetScale(const Transform* transform) {
	if (transform != NULL) {
		return &(transform->scale);
	}
	return NULL;
}

// Set translation
void TransformSetTranslation(Transform* transform, const Vector2D* translation) {
	if (transform != NULL) {
		transform->translation.x = translation->x;
		transform->translation.y = translation->y;
		transform->isDirty = true;
	}
	else {
		printf("Error in TransformSetTranslation: Invalid argument\n");
	}
}

// Set rotation
void TransformSetRotation(Transform* transform, float rotation) {
	if (transform != NULL) {
		transform->rotation = rotation;
		transform->isDirty = true;
	}
	else {
		printf("Error in TransformSetRotation: Invalid argument\n");
	}
}

// Set scale
void TransformSetScale(Transform* transform, const Vector2D* scale) {
	if (transform != NULL && scale != NULL) {
		transform->scale.x = scale->x;
		transform->scale.y = scale->y;
		transform->isDirty = true;
	}
	else {
		printf("Error in TransformSetScale: Invalid argument\n");
	}
}

// Get the transform matrix
const Matrix2D* TransformGetMatrix(Transform* transform) {
	if (transform == NULL) {
		return NULL;
	}

	if (transform->isDirty) {
		Matrix2D scaleMat, rotMat, transMat, resultMat;

		Matrix2DScale(&scaleMat, transform->scale.x, transform->scale.y);
		Matrix2DRotRad(&rotMat, transform->rotation);
		Matrix2DTranslate(&transMat, transform->translation.x, transform->translation.y);

		Matrix2DConcat(&resultMat, &rotMat, &scaleMat);
		Matrix2DConcat(&transform->matrix, &transMat, &resultMat);


		transform->isDirty = false;
	}

	return &transform->matrix;
}
Transform* TransformClone(const Transform* other) {
	if (other == NULL) {
		return NULL;
	}

	Transform* clone = (Transform*)malloc(sizeof(Transform));
	if (clone == NULL) {
		return NULL;
	}


	*clone = *other;

	return clone;
}

//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------
