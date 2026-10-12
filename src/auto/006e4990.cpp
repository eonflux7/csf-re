#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, SceneLoader_GetSceneController, 0x00652010, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(int, __fastcall, SceneLoader_GetWorldObjectCollection, 0x006B6AF0, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GfxCollision_IntersectWorldLine, 0x006EBA90, (int world, int intersection, int (__cdecl* callback)(int, int, float*, uint32_t, int), int userData));

CSF_GLOBAL(float, g_fReturnedConstant_0082E030, 0x0082E030);
CSF_GLOBAL(float, g_flt_0082E25C, 0x0082E25C);

static float* __fastcall S_006e48a0_UpdateGroundPosition(
    S_006e48a0* self, void* /*edx*/)
{
    alignas(8) uint8_t lineBytes[0x40] = {};
    alignas(8) uint8_t hitBytes[0xC] = {};

    const float upperY = self->field_0008 + g_fReturnedConstant_0082E030;
    const uint32_t xBits = self->field_0004;
    const uint32_t zBits = CSF_AT(uint32_t, self, 0x0C);
    const float lowerY = self->field_0008 - g_flt_0082E25C;

    CSF_AT(uint32_t, lineBytes, 0x00) = xBits;
    CSF_AT(float, lineBytes, 0x04) = upperY;
    CSF_AT(uint32_t, lineBytes, 0x08) = zBits;
    CSF_AT(uint32_t, lineBytes, 0x0C) = xBits;
    CSF_AT(float, lineBytes, 0x10) = lowerY;
    CSF_AT(uint32_t, lineBytes, 0x14) = zBits;
    CSF_AT(uint32_t, lineBytes, 0x18) = 1;

    // Keep the endpoint copies in the same storage passed to the collision query.
    CSF_AT(uint32_t, lineBytes, 0x28) = xBits;
    CSF_AT(float, lineBytes, 0x2C) = lowerY;
    CSF_AT(uint32_t, lineBytes, 0x30) = zBits;
    CSF_AT(uint32_t, lineBytes, 0x34) = xBits;
    CSF_AT(float, lineBytes, 0x38) = upperY;
    CSF_AT(uint32_t, lineBytes, 0x3C) = zBits;

    CSF_AT(int, hitBytes, 0x00) = 0;
    CSF_AT(float, hitBytes, 0x04) = FLT_MAX;
    CSF_AT(S_006e48a0*, hitBytes, 0x08) = self;

    using CollisionCallback = int (__cdecl*)(int, int, float*, uint32_t, int);

    void* controller = reinterpret_cast<void*>(
        SceneLoader_GetSceneController(reinterpret_cast<void*>(GetGlobalInt()), nullptr));
    void* scene = ((void* (__fastcall*)(void*, void*))
        CSF_AT(void**, controller, 0)[0x44 / 4])(controller, nullptr);
    int world = SceneLoader_GetWorldObjectCollection(scene, nullptr);
    if (world)
    {
        GfxCollision_IntersectWorldLine(
            world, reinterpret_cast<int>(lineBytes),
            reinterpret_cast<CollisionCallback>(CSF_ADDR_FN(0x006E4450)),
            reinterpret_cast<int>(hitBytes));
    }

    controller = reinterpret_cast<void*>(
        SceneLoader_GetSceneController(reinterpret_cast<void*>(GetGlobalInt()), nullptr));
    const unsigned int sceneCount = ((unsigned int (__fastcall*)(void*, void*))
        CSF_AT(void**, controller, 0)[0x88 / 4])(controller, nullptr);

    for (unsigned int index = 0; index < sceneCount; ++index)
    {
        controller = reinterpret_cast<void*>(
            SceneLoader_GetSceneController(reinterpret_cast<void*>(GetGlobalInt()), nullptr));
        scene = ((void* (__fastcall*)(void*, void*, unsigned int))
            CSF_AT(void**, controller, 0)[0x90 / 4])(controller, nullptr, index);
        if (scene)
        {
            world = SceneLoader_GetWorldObjectCollection(scene, nullptr);
            if (world)
            {
                GfxCollision_IntersectWorldLine(
                    world, reinterpret_cast<int>(lineBytes),
                    reinterpret_cast<CollisionCallback>(CSF_ADDR_FN(0x006E4450)),
                    reinterpret_cast<int>(hitBytes));
            }
        }
    }

    S_006e48a0* destination = CSF_AT(S_006e48a0*, hitBytes, 0x08);
    if (CSF_AT(int, hitBytes, 0x00))
    {
        const float fraction = CSF_AT(float, hitBytes, 0x04);
        const float startX = CSF_AT(float, lineBytes, 0x34);
        const float startY = CSF_AT(float, lineBytes, 0x38);
        const float startZ = CSF_AT(float, lineBytes, 0x3C);
        const float deltaX = CSF_AT(float, lineBytes, 0x28) - startX;
        const float deltaY = CSF_AT(float, lineBytes, 0x2C) - startY;
        const float deltaZ = CSF_AT(float, lineBytes, 0x30) - startZ;

        CSF_AT(float, destination, 0x10) = startX + deltaX * fraction;
        CSF_AT(float, destination, 0x14) = startY + deltaY * fraction;
        CSF_AT(float, destination, 0x18) = startZ + deltaZ * fraction;
        return &CSF_AT(float, destination, 0x10);
    }

    const uint32_t originalX = self->field_0004;
    uint32_t originalY;
    std::memcpy(&originalY, &self->field_0008, sizeof(originalY));
    CSF_AT(uint32_t, destination, 0x10) = originalX;
    const uint32_t originalZ = CSF_AT(uint32_t, self, 0x0C);
    CSF_AT(uint32_t, destination, 0x14) = originalY;
    CSF_AT(uint32_t, destination, 0x18) = originalZ;

    // The original returns the Z component's raw bits on the no-hit path.
    return reinterpret_cast<float*>(originalZ);
}

HOOK(0x006E4990, S_006e48a0_UpdateGroundPosition, "auto");
