//------------------------------------------------------------------------------
//
// File Name:	Mesh.c
// Author(s):	Harrison Parrish (harrison.parrish)
// Project:		Project 0
// Course:		CS230S25
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------
#include "stdafx.h"
#include "stdlib.h"
#include "DGL.h"
#include "Mesh.h"
#include "assert.h"
#include "Vector2D.h"
#include "Stream.h"
#include "Transform.h"
#include "Trace.h"
//------------------------------------------------------------------------------
// Private Constants:
//------------------------------------------------------------------------------
const DGL_Color ColorRed = { 1.0f, 0.0f, 0.0f, 1.0f };
const DGL_Color ColorGreen = { 0.0f, 1.0f, 0.0f, 1.0f };
const DGL_Color ColorBlue = { 0.0f, 0.0f, 1.0f, 1.0f };
const DGL_Color ColorWhite = { 1.0f, 1.0f, 1.0f, 1.0f };
const DGL_Color Clear = { 0.0f, 0.0f, 0.0f, 0.0f };
//------------------------------------------------------------------------------
// Private Structures:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Variables:
//------------------------------------------------------------------------------

typedef struct Mesh
{
	char name[256]; // The name of the Mesh.  This will be used later in the semester.
	const DGL_Mesh* meshResource; // Pointer to the DGL_Mesh resource.
	DGL_DrawMode drawMode; // The draw mode to use when rendering the mesh (Usually "DGL_DM_TRIANGLELIST").
} Mesh;

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------


Mesh* MeshCreate() {
	Mesh* mesh = (Mesh*)calloc(1, sizeof(Mesh));
	return mesh;
}


void MeshBuildQuad(Mesh* mesh, float xHalfSize, float yHalfSize, float uSize, float vSize, const char* name) {
	mesh->drawMode = DGL_DM_TRIANGLELIST;

	DGL_Graphics_StartMesh();

	DGL_Graphics_AddTriangle(
		&(DGL_Vec2) { -xHalfSize, -yHalfSize }, & Clear, & (DGL_Vec2){ 0.0f, vSize },
		& (DGL_Vec2) {
		xHalfSize, yHalfSize
	}, & Clear, & (DGL_Vec2){ uSize, 0.0f },
			& (DGL_Vec2) {
			xHalfSize, -yHalfSize
		}, & Clear, & (DGL_Vec2){ uSize, vSize }
				);
	DGL_Graphics_AddTriangle(
		&(DGL_Vec2) { -xHalfSize, -yHalfSize }, & Clear, & (DGL_Vec2){ 0.0f, vSize },
		& (DGL_Vec2) {
		-xHalfSize, yHalfSize
	}, & Clear, & (DGL_Vec2){ 0.0f, 0.0f },
			& (DGL_Vec2) {
			xHalfSize, yHalfSize
		}, & Clear, & (DGL_Vec2){ uSize, 0.0f }
				);

	strcpy_s(mesh->name, _countof(mesh->name), name);

	mesh->meshResource = DGL_Graphics_EndMesh();
	assert(mesh->meshResource != NULL);
}

void MeshBuildSpaceship(Mesh* mesh) {
	mesh->drawMode = DGL_DM_TRIANGLELIST;

	DGL_Graphics_StartMesh();
	DGL_Graphics_AddTriangle(
		&(DGL_Vec2) { 0.5f, 0.0f }, & (DGL_Color) { 1.0f, 1.0f, 0.0f, 1.0f }, & (DGL_Vec2) { 0.0f, 0.0f },
		& (DGL_Vec2) {
		-0.5f, -0.5f
	}, & (DGL_Color) {
		1.0f, 0.0f, 0.0f, 1.0f
	}, & (DGL_Vec2) {
			0.0f, 0.0f
		},
			& (DGL_Vec2) {
			-0.5f, 0.5f
		}, & (DGL_Color) {
			1.0f, 0.0f, 0.0f, 1.0f
		}, & (DGL_Vec2) {
				0.0f, 0.0f
			}
				);

	strcpy_s(mesh->name, _countof(mesh->name), "spaceship");

	mesh->meshResource = DGL_Graphics_EndMesh();
}


void MeshRender(const Mesh* mesh) {
	if (mesh == NULL || mesh->meshResource == NULL) return;

	DGL_Graphics_DrawMesh(mesh->meshResource, mesh->drawMode);
	
}


void MeshFree(Mesh** mesh) {
	if (*mesh) {
		DGL_Mesh** mpptr = &((*mesh)->meshResource);
		DGL_Graphics_FreeMesh(mpptr);
		free(*mesh);
		*mesh = NULL;
	}
}
 //Read the properties of a Mesh object from a file.
 //Params:
 //  mesh = Pointer to the Mesh.
 //  stream = The data stream used for reading.
void MeshRead(Mesh* mesh, Stream stream) {
	DGL_Color color;
	DGL_Vec2 position;
	DGL_Vec2 uv;

	if (mesh == NULL || stream == NULL) {
		TraceMessage("MeshRead: Invalid mesh or stream pointer.");
		return;
	}


	char token[256];


	StreamReadToken(stream); 

	strcpy_s(token, sizeof(token), StreamReadToken(stream));

	if (strcmp(token, "Mesh") != 0) {
		TraceMessage("MeshRead: Expected token 'Mesh', found '%s'.", token);
		return;
	}

	StreamReadToken(stream); 
	strcpy_s(mesh->name, sizeof(mesh->name), token);

	int numVertices = StreamReadInt(stream);


	for (int i = 0; i < numVertices; ++i) {
		StreamReadVector2D(stream, &position);
		StreamReadColor(stream, &color);
		StreamReadVector2D(stream, &uv);

		DGL_Graphics_AddVertex(&position, &color, &uv);
	}
}



bool MeshIsNamed(const Mesh* mesh, const char* name) {
	
	if (mesh == NULL || name == NULL) {
		return false;
	}


	return (strcmp(mesh->name, name) == 0); 
}