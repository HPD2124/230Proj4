//------------------------------------------------------------------------------
//
// File Name:	Matrix2D.c
// Author(s):	Harrison Parrish (harrison.parrish)
// Project:		Project 0
// Course:		CS230S25
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "Matrix2D.h"
#include <DGL.h>

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
void Matrix2DInit()
{
}

// Update the ...
// Params:
//	 dt = Change in time (in seconds) since the last game loop.
void Matrix2DUpdate(float dt)
{
    /* Tell the compiler that the 'dt' variable is unused. */
    UNREFERENCED_PARAMETER(dt);
}

// Shutdown the ...
void Matrix2DExit()
{
}

//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------


#define _USE_MATH_DEFINES
#include <math.h>

void Matrix2DIdentity(Matrix2D* pResult) {
    *pResult = (Matrix2D){
        .m = {
            {1, 0, 0, 0},
            {0, 1, 0, 0},
            {0, 0, 1, 0},
            {0, 0, 0, 1}
        }
    };
}

void Matrix2DTranspose(Matrix2D* pResult, const Matrix2D* pMtx) {
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            pResult->m[i][j] = pMtx->m[j][i];
        }
    }
}

void Matrix2DConcat(Matrix2D* pResult, const Matrix2D* pMtx0, const Matrix2D* pMtx1) {
    Matrix2D temp = { 0 };
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            for (int k = 0; k < 4; ++k) {
                temp.m[i][j] += pMtx0->m[i][k] * pMtx1->m[k][j];
            }
        }
    }
    *pResult = temp;
}

void Matrix2DTranslate(Matrix2D* pResult, float x, float y) {
    Matrix2DIdentity(pResult);
    pResult->m[0][3] = x;
    pResult->m[1][3] = y;
}

void Matrix2DScale(Matrix2D* pResult, float x, float y) {
    Matrix2DIdentity(pResult);
    pResult->m[0][0] = x;
    pResult->m[1][1] = y;
}

void Matrix2DRotDeg(Matrix2D* pResult, float angle) {
    float rad = (float)((angle * M_PI) / 180.0f);

    Matrix2DRotRad(pResult, rad);
}

void Matrix2DRotRad(Matrix2D* pResult, float angle) {
    float cosA = cosf(angle);
    float sinA = sinf(angle);
    Matrix2DIdentity(pResult);
    pResult->m[0][0] = cosA;
    pResult->m[0][1] = -sinA;
    pResult->m[1][0] = sinA;
    pResult->m[1][1] = cosA;
}

void Matrix2DMultVec(Vector2D* pResult, const Matrix2D* pMtx, const Vector2D* pVec) {
    float x = pMtx->m[0][0] * pVec->x + pMtx->m[0][1] * pVec->y + pMtx->m[0][3];
    float y = pMtx->m[1][0] * pVec->x + pMtx->m[1][1] * pVec->y + pMtx->m[1][3];
    pResult->x = x;
    pResult->y = y;
}
