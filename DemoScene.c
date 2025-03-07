#include "stdafx.h"
#include "DGL.h"
#include "DemoScene.h"
#include "Scene.h"
#include "SceneSystem.h"
#include "Level1Scene.h"
#include "Level2Scene.h"
#include "SandboxScene.h"

//------------------------------------------------------------------------------
// Private Structures:
//------------------------------------------------------------------------------

typedef struct DemoScene
{
    Scene base;

} DemoScene;

//------------------------------------------------------------------------------
// Public Variables:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Constants:
//------------------------------------------------------------------------------

static const DGL_Color DGL_Color_Black = { 0.0f, 0.0f, 0.0f, 1.0f };
static const DGL_Color DGL_Color_White = { 1.0f, 1.0f, 1.0f, 1.0f };
static const DGL_Color DGL_Color_Red = { 1.0f, 0.0f, 0.0f, 1.0f };
static const DGL_Color DGL_Color_Green = { 0.0f, 1.0f, 0.0f, 1.0f };
static const DGL_Color DGL_Color_Blue = { 0.0f, 0.0f, 1.0f, 1.0f };
static const DGL_Color DGL_Color_Cyan = { 0.0f, 1.0f, 1.0f, 1.0f };
static const DGL_Color DGL_Color_Magenta = { 1.0f, 0.0f, 1.0f, 1.0f };
static const DGL_Color DGL_Color_Yellow = { 1.0f, 1.0f, 0.0f, 1.0f };

static const DGL_Vec2 posColored = { -200.f, 200.f };
static const DGL_Vec2 posTinted = { 200.f, 200.f };
static const DGL_Vec2 posTexturedBall = { -200.f, -200.f };
static const DGL_Vec2 posTexturedSquare = { 200.f, -200.f };

static const DGL_Vec2 scaleColored = { 100.f, 100.f };
static const DGL_Vec2 scaleTextured = { 100.f, 100.f };

static const float cameraScrollSpeed = 2.0f;
static const float spaceshipScrollSpeed = 4.0f;
static const float uvScrollSpeed = 0.01f;
static const float rotationSpeed = 0.5f;

//------------------------------------------------------------------------------
// Private Variables:
//------------------------------------------------------------------------------

static DGL_Mesh* meshColor = NULL;
static DGL_Mesh* meshTexture = NULL;
static DGL_Mesh* meshSpaceship = NULL;
static DGL_Mesh* meshLine = NULL;

static DGL_Texture* textureBall = NULL;
static DGL_Texture* textureSquare = NULL;

static DGL_Vec2 posCamera = { 0, 0 };
static DGL_Vec2 posSpaceship = { 0.f, 0.f };

static DGL_Vec2 uvOffset = { 0.f, 0.f };

static float alpha = 1.0f;
static float rotation = 0.0f;

//------------------------------------------------------------------------------
// Private Function Declarations:
//------------------------------------------------------------------------------

static void DemoSceneLoad(void);
static void DemoSceneInit(void);
static void DemoSceneUpdate(float dt);
static void DemoSceneExit(void);
static void DemoSceneUnload(void);
static void DemoSceneRender(void);

//------------------------------------------------------------------------------
// Instance Variable:
//------------------------------------------------------------------------------

static DemoScene instance =
{
    { "Demo", DemoSceneLoad, DemoSceneInit, DemoSceneUpdate, DemoSceneRender, DemoSceneExit, DemoSceneUnload },
};

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

const Scene* DemoSceneGetInstance(void)
{
    return &(instance.base);
}

//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------

static void DemoSceneLoad(void)
{
    DGL_Graphics_StartMesh();
    DGL_Graphics_AddTriangle(
        &(DGL_Vec2) { -0.5f, -0.5f }, & DGL_Color_Red, & (DGL_Vec2){ 0.0f, 0.0f },
        & (DGL_Vec2) {0.5f, 0.5f}, & DGL_Color_Green, & (DGL_Vec2){ 0.0f, 0.0f },
        & (DGL_Vec2) {0.5f, -0.5f}, & DGL_Color_Blue, & (DGL_Vec2){ 0.0f, 0.0f });
        DGL_Graphics_AddTriangle(
            &(DGL_Vec2) { -0.5f, -0.5f }, & DGL_Color_Cyan, & (DGL_Vec2){ 0.0f, 0.0f },
            & (DGL_Vec2) {
            -0.5f, 0.5f
        }, & DGL_Color_Magenta, & (DGL_Vec2){ 0.0f, 0.0f },
                & (DGL_Vec2) {0.5f, 0.5f}, & DGL_Color_Yellow, & (DGL_Vec2){ 0.0f, 0.0f });
            meshColor = DGL_Graphics_EndMesh();

            DGL_Graphics_StartMesh();
            DGL_Graphics_AddTriangle(
                &(DGL_Vec2) { -0.5f, -0.5f }, & DGL_Color_Black, & (DGL_Vec2){ 0.0f, 1.0f },
                & (DGL_Vec2) {
                0.5f, 0.5f
            }, & DGL_Color_Black, & (DGL_Vec2){ 1.0f, 0.0f },
                    & (DGL_Vec2) {
                    0.5f, -0.5f
                }, & DGL_Color_Black, & (DGL_Vec2){ 1.0f, 1.0f });
                DGL_Graphics_AddTriangle(
                    &(DGL_Vec2) { -0.5f, -0.5f }, & DGL_Color_Black, & (DGL_Vec2){ 0.0f, 1.0f },
                    & (DGL_Vec2) {
                    -0.5f, 0.5f
                }, & DGL_Color_Black, & (DGL_Vec2){ 0.0f, 0.0f },
                        & (DGL_Vec2) {
                        0.5f, 0.5f
                    }, & DGL_Color_Black, & (DGL_Vec2){ 1.0f, 0.0f });
                    meshTexture = DGL_Graphics_EndMesh();

                    DGL_Graphics_StartMesh();
                    DGL_Graphics_AddTriangle(
                        &(DGL_Vec2) { 0.5f, 0.0f }, & DGL_Color_Yellow, & (DGL_Vec2){ 0.0f, 0.0f },
                        & (DGL_Vec2) {
                        -0.5f, -0.5f
                    }, & DGL_Color_Red, & (DGL_Vec2){ 0.0f, 0.0f },
                            & (DGL_Vec2) {
                            -0.5f, 0.5f
                        }, & DGL_Color_Red, & (DGL_Vec2){ 0.0f, 0.0f });
                        meshSpaceship = DGL_Graphics_EndMesh();

                        DGL_Graphics_StartMesh();
                        DGL_Graphics_AddVertex(&(DGL_Vec2) { 0.5f, 0.5f }, & DGL_Color_White, & (DGL_Vec2){ 0.0f, 0.0f });
                        DGL_Graphics_AddVertex(&(DGL_Vec2) { -0.5f, 0.5f }, & DGL_Color_White, & (DGL_Vec2){ 0.0f, 0.0f });
                        DGL_Graphics_AddVertex(&(DGL_Vec2) { -0.5f, -0.5f }, & DGL_Color_White, & (DGL_Vec2){ 0.0f, 0.0f });
                        DGL_Graphics_AddVertex(&(DGL_Vec2) { 0.5f, -0.5f }, & DGL_Color_White, & (DGL_Vec2){ 0.0f, 0.0f });
                        DGL_Graphics_AddVertex(&(DGL_Vec2) { 0.5f, 0.5f }, & DGL_Color_White, & (DGL_Vec2){ 0.0f, 0.0f });
                        meshLine = DGL_Graphics_EndMesh();

                        textureBall = DGL_Graphics_LoadTexture("Assets/ball.png");

                        const unsigned char colorsSquare[16] =
                        {
                            255,   0,   0, 255,
                              0, 255,   0, 255,
                              0,   0, 255, 255,
                            255, 255, 255, 255
                        };
                        textureSquare = DGL_Graphics_LoadTextureFromMemory(colorsSquare, 2, 2);
}

static void DemoSceneInit()
{
    posCamera.x = 0;
    posCamera.y = 0;
    posSpaceship.x = 0.0f;
    posSpaceship.y = 0.0f;

    uvOffset.x = 0.0f;
    uvOffset.y = 0.0f;

    alpha = 1.0f;
    rotation = 1.0f;

    DGL_Graphics_SetBackgroundColor(&(DGL_Color) { 0.0f, 0.0f, 0.0f, 0.0f });
    DGL_Graphics_SetBlendMode(DGL_BM_BLEND);
}

static void DemoSceneUpdate(float dt)
{
    UNREFERENCED_PARAMETER(dt);

    if (DGL_Window_HasFocus())
    {
        if (DGL_Input_KeyDown('A'))
        {
            posCamera.x -= cameraScrollSpeed;
        }
        else if (DGL_Input_KeyDown('D'))
        {
            posCamera.x += cameraScrollSpeed;
        }
        if (DGL_Input_KeyDown('W'))
        {
            posCamera.y += cameraScrollSpeed;
        }
        else if (DGL_Input_KeyDown('S'))
        {
            posCamera.y -= cameraScrollSpeed;
        }

        if (DGL_Input_KeyDown(VK_UP))
            posSpaceship.y += spaceshipScrollSpeed;
        else if (DGL_Input_KeyDown(VK_DOWN))
            posSpaceship.y -= spaceshipScrollSpeed;
        if (DGL_Input_KeyDown(VK_LEFT))
            posSpaceship.x -= spaceshipScrollSpeed;
        else if (DGL_Input_KeyDown(VK_RIGHT))
            posSpaceship.x += spaceshipScrollSpeed;

        if (DGL_Input_KeyDown('Z'))
        {
            alpha = max(0.0f, alpha - 0.02f);
        }
        else if (DGL_Input_KeyDown('X'))
        {
            alpha = min(1.0f, alpha + 0.02f);
        }

        if (DGL_Input_KeyDown('J'))
            uvOffset.x -= uvScrollSpeed;
        else if (DGL_Input_KeyDown('L'))
            uvOffset.x += uvScrollSpeed;
        if (DGL_Input_KeyDown('I'))
            uvOffset.y -= uvScrollSpeed;
        else if (DGL_Input_KeyDown('K'))
            uvOffset.y += uvScrollSpeed;

        rotation += rotationSpeed * dt;

    }
}

void DemoSceneRender(void)
{
    DGL_Camera_SetPosition(&posCamera);



    DGL_Graphics_SetShaderMode(DGL_PSM_COLOR, DGL_VSM_DEFAULT);
    DGL_Graphics_SetTexture(NULL);
    DGL_Graphics_SetCB_Alpha(1.0f);
    DGL_Graphics_SetCB_TintColor(&(DGL_Color) { 0.0f, 0.0f, 0.0f, 0.0f });
    DGL_Graphics_SetCB_TransformData(&posColored, &scaleColored, 0.f);
    DGL_Graphics_DrawMesh(meshColor, DGL_DM_TRIANGLELIST);

    DGL_Graphics_SetShaderMode(DGL_PSM_COLOR, DGL_VSM_DEFAULT);
    DGL_Graphics_SetCB_Alpha(1.0f);
    DGL_Graphics_SetCB_TintColor(&(DGL_Color) { 1.0f, 0.0f, 0.0f, 0.5f });
    DGL_Graphics_SetCB_TransformData(&posTinted, &scaleColored, rotation);
    DGL_Graphics_DrawMesh(meshColor, DGL_DM_TRIANGLELIST);

    DGL_Graphics_SetShaderMode(DGL_PSM_TEXTURE, DGL_VSM_DEFAULT);
    DGL_Graphics_SetTexture(textureBall);
    DGL_Graphics_SetCB_Alpha(1.0f);
    DGL_Graphics_SetCB_TextureOffset(&uvOffset);
    DGL_Graphics_SetCB_TintColor(&(DGL_Color) { 0.0f, 0.0f, 0.0f, 0.0f });
    DGL_Graphics_SetCB_TransformData(&posTexturedBall, &scaleTextured, 0.f);
    DGL_Graphics_DrawMesh(meshTexture, DGL_DM_TRIANGLELIST);

    DGL_Graphics_SetShaderMode(DGL_PSM_TEXTURE, DGL_VSM_DEFAULT);
    DGL_Graphics_SetTextureSamplerData(DGL_TSM_POINT, DGL_AM_WRAP);
    DGL_Graphics_SetTexture(textureSquare);
    DGL_Graphics_SetCB_Alpha(1.0f);
    DGL_Graphics_SetCB_TextureOffset(&(DGL_Vec2) { 0.f, 0.f });
    DGL_Graphics_SetCB_TintColor(&(DGL_Color) { 0.0f, 0.0f, 0.0f, 0.0f });
    DGL_Graphics_SetCB_TransformData(&posTexturedSquare, &scaleTextured, 0.f);
    DGL_Graphics_DrawMesh(meshTexture, DGL_DM_TRIANGLELIST);
    DGL_Graphics_SetTextureSamplerData(DGL_TSM_LINEAR, DGL_AM_WRAP);

    DGL_Graphics_SetShaderMode(DGL_PSM_COLOR, DGL_VSM_DEFAULT);
    DGL_Graphics_SetCB_Alpha(alpha);
    DGL_Graphics_SetCB_TintColor(&(DGL_Color) { 0.0f, 0.0f, 0.0f, 0.0f });
    DGL_Graphics_SetCB_TransformData(&posSpaceship, &scaleColored, 0.f);
    DGL_Graphics_DrawMesh(meshSpaceship, DGL_DM_TRIANGLELIST);

    DGL_Graphics_SetShaderMode(DGL_PSM_COLOR, DGL_VSM_DEFAULT);
    DGL_Graphics_SetCB_Alpha(1.0f);
    DGL_Graphics_SetCB_TintColor(&(DGL_Color) { 0.0f, 0.0f, 0.0f, 0.0f });
    DGL_Graphics_SetCB_TransformData(&(DGL_Vec2) { 0.f, 0.f }, & (DGL_Vec2) { 800.f, 600.f }, 0.f);
    DGL_Graphics_DrawMesh(meshLine, DGL_DM_LINESTRIP);

  
}

static void DemoSceneExit()
{
}

static void DemoSceneUnload(void)
{
    DGL_Graphics_FreeMesh(&meshColor);
    DGL_Graphics_FreeMesh(&meshTexture);
    DGL_Graphics_FreeMesh(&meshSpaceship);
    DGL_Graphics_FreeMesh(&meshLine);

    DGL_Graphics_FreeTexture(&textureBall);
    DGL_Graphics_FreeTexture(&textureSquare);
}