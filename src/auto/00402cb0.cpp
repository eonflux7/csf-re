#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(char, __cdecl, Collision_TestTriangle, 0x004026E0, (float* arg0, float* a3, void* arg8, int a4, int a5, int a6));
CSF_FUNC(int, __cdecl, RpWorldSectorGetWorld, 0x0058D1A0, (unsigned int a1));
CSF_FUNC(int, __fastcall, SceneLoader_GetSceneController, 0x00652010, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(char, __fastcall, GfxCollision_GetTriangleSurfaceFlag, 0x006B6BE0, (void* self, void* edx, uint16_t* a2, int a3));
CSF_FUNC(int, __cdecl, GfxObjectProps_GetObjectSectorData, 0x006B87C0, (int objectIndex));

CSF_GLOBAL(float, g_TransitionOne, 0x0082D4D4);
CSF_GLOBAL(int, g_RayCollisionMask, 0x00869C5C);

// The packet also identifies this address as a vtable reference.
static const auto kTriangleSurfaceVtable = CSF_VTBL(0x006B6BE0);

static uint32_t* __cdecl Collision_TestSectorTriangle(
    int /*intersection*/,
    int sectorAddress,
    uint32_t* collisionTriangle,
    int /*distance*/,
    int contextAddress)
{
    void* sector = reinterpret_cast<void*>(sectorAddress);
    auto* context = reinterpret_cast<uint8_t*>(contextAddress);
    const uint32_t triangleIndex = collisionTriangle[6];
    auto* sectorTriangles =
        CSF_AT(uint8_t*, sector, 0x04);

    alignas(8) uint8_t sectorTriangleBytes[0x08];
    CSF_AT(uint32_t, sectorTriangleBytes, 0x00) =
        CSF_AT(uint32_t, sectorTriangles + triangleIndex * 8, 0x00);
    CSF_AT(uint32_t, sectorTriangleBytes, 0x04) =
        CSF_AT(uint32_t, sectorTriangles + triangleIndex * 8, 0x04);
    const uint8_t materialOffset =
        CSF_AT(uint8_t, sectorTriangleBytes, 0x06);

    void* world = reinterpret_cast<void*>(
        RpWorldSectorGetWorld(static_cast<unsigned int>(sectorAddress)));
    const int materialIndex =
        CSF_AT(uint16_t, sector, 0x80) + materialOffset;
    auto* worldMaterials = CSF_AT(uint8_t*, world, 0x10);
    void* objectSectorData = reinterpret_cast<void*>(
        GfxObjectProps_GetObjectSectorData(
            CSF_AT(int, worldMaterials + materialIndex * 4, 0x00)));

    const int collisionMask = CSF_AT(int, context, 0x30);
    if (collisionMask != g_RayCollisionMask &&
        objectSectorData &&
        (collisionMask & CSF_AT(int, objectSectorData, 0x0C)) == 0)
    {
        return collisionTriangle;
    }

    const float inverseScaleX =
        g_TransitionOne / CSF_AT(float, context, 0x1C);

    alignas(8) uint8_t triangleBytes[0x2C] = {};
    float* triangle = reinterpret_cast<float*>(triangleBytes);

    auto* firstVertex = reinterpret_cast<float*>(collisionTriangle[7]);
    triangle[0] = firstVertex[0];
    triangle[1] = firstVertex[1];
    triangle[2] = firstVertex[2];

    auto* secondVertex = reinterpret_cast<float*>(collisionTriangle[8]);
    triangle[3] = secondVertex[0];
    triangle[4] = secondVertex[1];
    triangle[5] = secondVertex[2];

    auto* thirdVertex = reinterpret_cast<float*>(collisionTriangle[9]);
    triangle[6] = thirdVertex[0];
    triangle[7] = thirdVertex[1];
    triangle[8] = thirdVertex[2];

    const float inverseScaleY =
        g_TransitionOne / CSF_AT(float, context, 0x20);
    const float inverseScaleZ =
        g_TransitionOne / CSF_AT(float, context, 0x24);

    triangle[0] *= inverseScaleX;
    triangle[1] *= inverseScaleY;
    triangle[2] *= inverseScaleZ;
    triangle[3] *= inverseScaleX;
    triangle[4] *= inverseScaleY;
    triangle[5] *= inverseScaleZ;
    triangle[6] *= inverseScaleX;
    triangle[7] *= inverseScaleY;
    triangle[8] *= inverseScaleZ;

    CSF_AT(int, triangleBytes, 0x24) =
        objectSectorData ? CSF_AT(int, objectSectorData, 0x08) : -1;

    void* globalState = reinterpret_cast<void*>(GetGlobalInt());
    void* sceneController = reinterpret_cast<void*>(
        SceneLoader_GetSceneController(globalState, nullptr));
    void* collisionScene =
        reinterpret_cast<void* (__fastcall*)(void*, void*)>(
            CSF_AT(void**, sceneController, 0x00)[0x44 / 4])(
                sceneController, nullptr);

    const char surfaceFlag = GfxCollision_GetTriangleSurfaceFlag(
        collisionScene, nullptr,
        reinterpret_cast<uint16_t*>(sectorTriangleBytes), sectorAddress);
    const int collisionTarget = CSF_AT(int, context, 0x00);
    CSF_AT(char, triangleBytes, 0x28) = surfaceFlag;

    return Collision_TestTriangle(
        triangle, reinterpret_cast<float*>(context + 0x04),
        context + 0x10, 1, 0, collisionTarget) != 0
        ? collisionTriangle
        : nullptr;
}

HOOK(0x00402CB0, Collision_TestSectorTriangle, "auto");
