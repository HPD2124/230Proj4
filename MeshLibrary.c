//------------------------------------------------------------------------------
//
// File Name:	MeshLibrary.c
// Author(s):	Harrison Parrish (harrison.parrish)
// Project:		Project 4
// Course:		CS230S25
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "MeshLibrary.h"
#include "Mesh.h" 
#include "Stream.h"


#define MESH_LIST_SIZE 10



typedef struct {
    unsigned int meshCount;
    const Mesh* meshList[MESH_LIST_SIZE];
} MeshLibrary;
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
static MeshLibrary meshLibrary;
//------------------------------------------------------------------------------
// Private Function Declarations:
//------------------------------------------------------------------------------
static void MeshLibraryAdd(const Mesh* mesh);
static const Mesh* MeshLibraryFind(const char* meshName);

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// Initialize the Mesh Library.
// (NOTE: Make sure to initialize all memory to zero.)
void MeshLibraryInit() {
    memset(&meshLibrary, 0, sizeof(MeshLibrary));
}
static const Mesh* MeshLibraryFind(const char* meshName) {
    for (unsigned int i = 0; i < meshLibrary.meshCount; i++) {
        if (MeshIsNamed(meshLibrary.meshList[i], meshName)) {
            return meshLibrary.meshList[i];
        }
    }
    return NULL;
}
static void MeshLibraryAdd(const Mesh* mesh) {
    if (meshLibrary.meshCount < MESH_LIST_SIZE) {
        meshLibrary.meshList[meshLibrary.meshCount] = mesh;
        meshLibrary.meshCount++;
    }
}

// Build a mesh and add it to the mesh library, if it doesn't already exist.
//   1: Verify that a valid name was specified (not NULL).
//   2: Search for an existing mesh with a matching name.
//   3: If an existing mesh was NOT found, then create a new one from the data file:
//      a: Use sprintf_s() to construct a path name using meshName.
//	       (HINT: The correct path name should be constructed using "Data/%s.txt".)
//      b: Call StreamOpen(), passing the pathname.
//      c: If the stream was opened successfully,
//	       1: Call MeshCreate() to create an empty Mesh object.
//         2: Call MeshRead() to read the contents of the mesh from the file.
//	       3: Call MeshLibraryAdd(), passing the created mesh.
//	       4: Close the stream.
//   4: Return the mesh (either existed, created, or NULL).
// Params:
//	 meshName = The name of the mesh to be created.
// Returns:
//	 If the mesh already existed or was created successfully,
//	   then return a pointer to the mesh,
//	   else return NULL.
const Mesh* MeshLibraryBuild(const char* meshName) {
    if (!meshName) {
        return NULL;
    }

    const Mesh* existingMesh = MeshLibraryFind(meshName);
    if (existingMesh) {
        return existingMesh;
    }

    char path[256]; 
    if (sprintf_s(path, 256, "Data/%s.txt", meshName) < 0) {
        return NULL;
    }

    Stream stream = StreamOpen(path);
    if (!stream) {
        return NULL;
    }

    Mesh* newMesh = MeshCreate();
    if (!newMesh) {
        StreamClose(&stream);
        return NULL;
    }

    MeshRead(newMesh, stream);
    MeshLibraryAdd(newMesh);
    StreamClose(&stream);

    return newMesh;
}






 
void MeshLibraryFreeAll() {
    for (unsigned int i = 0; i < meshLibrary.meshCount; i++) {
        MeshFree((Mesh**)&meshLibrary.meshList[i]);
        meshLibrary.meshList[i] = NULL;
    }
    meshLibrary.meshCount = 0;
}
// Update the ...
// Params:
//	 dt = Change in time (in seconds) since the last game loop.
void MeshLibraryUpdate(float dt)
{
	/* Tell the compiler that the 'dt' variable is unused. */
	UNREFERENCED_PARAMETER(dt);
}

// Shutdown the ...
void MeshLibraryExit()
{
}

//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------

