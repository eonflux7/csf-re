#include <cstdint>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, SceneLoader_GetSceneController, 0x00652010, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(char, __fastcall, GfxCollision_GetTriangleSurfaceFlag, 0x006B6BE0, (void* self, void* edx, uint16_t* a2, int a3));
CSF_FUNC(int, __cdecl, GfxCollision_IntersectWorldLine, 0x006EBA90, (int world, int intersection, int (__cdecl* callback)(int, int, float*, uint32_t, int), int userData));

CSF_GLOBAL(float, g_fReturnedConstant_0082E030, 0x0082E030);
CSF_GLOBAL(float, g_flt_0082E25C, 0x0082E25C);

static char __fastcall S_006b6be0_QueryGroundSurfaceFlag(
    S_006b6be0* self, void* /*edx*/, float* position, char* surfaceFlag)
{
    alignas(8) uint8_t intersectionBytes[0x1C];
    alignas(8) uint8_t hitBytes[0xC];

    const float x = position[0];
    CSF_AT(float, intersectionBytes, 0x00) = x;
    CSF_AT(float, intersectionBytes, 0x04) =
        position[1] + g_fReturnedConstant_0082E030;
    const float z = position[2];
    CSF_AT(float, intersectionBytes, 0x08) = z;
    CSF_AT(float, intersectionBytes, 0x0C) = x;
    CSF_AT(float, intersectionBytes, 0x10) =
        position[1] - g_flt_0082E25C;
    CSF_AT(float, intersectionBytes, 0x14) = z;
    CSF_AT(int, intersectionBytes, 0x18) = 1;

    CSF_AT(uint16_t*, hitBytes, 0x00) = nullptr;
    CSF_AT(int, hitBytes, 0x04) = 0;
    CSF_AT(float, hitBytes, 0x08) = FLT_MAX;

    const uint32_t world = self->field_0010;
    if (world)
    {
        GfxCollision_IntersectWorldLine(
            static_cast<int>(world),
            reinterpret_cast<int>(intersectionBytes),
            reinterpret_cast<int (__cdecl*)(int, int, float*, uint32_t, int)>(
                CSF_ADDR_FN(0x006B6C70)),
            reinterpret_cast<int>(hitBytes));
    }

    void* controller = reinterpret_cast<void*>(
        SceneLoader_GetSceneController(
            reinterpret_cast<void*>(GetGlobalInt()), nullptr));
    const uint32_t count =
        reinterpret_cast<uint32_t (__fastcall*)(void*, void*)>(
            CSF_AT(void**, controller, 0)[0x88 / 4])(controller, nullptr);

    for (uint32_t index = 0; index < count; ++index)
    {
        controller = reinterpret_cast<void*>(
            SceneLoader_GetSceneController(
                reinterpret_cast<void*>(GetGlobalInt()), nullptr));
        void* leaf = reinterpret_cast<void* (__fastcall*)(void*, void*, uint32_t)>(
            CSF_AT(void**, controller, 0)[0x90 / 4])(controller, nullptr, index);
        if (leaf)
        {
            const uint32_t leafWorld = CSF_AT(uint32_t, leaf, 0x10);
            if (leafWorld)
            {
                GfxCollision_IntersectWorldLine(
                    static_cast<int>(leafWorld),
                    reinterpret_cast<int>(intersectionBytes),
                    reinterpret_cast<int (__cdecl*)(int, int, float*, uint32_t, int)>(
                        CSF_ADDR_FN(0x006B6C70)),
                    reinterpret_cast<int>(hitBytes));
            }
        }
    }

    if (!CSF_AT(uint16_t*, hitBytes, 0x00))
        return 0;

    // The packet also lists this direct-call address as a vtable reference.
    (void)CSF_VTBL(0x006B6BE0);
    *surfaceFlag = GfxCollision_GetTriangleSurfaceFlag(
        self, nullptr,
        CSF_AT(uint16_t*, hitBytes, 0x00),
        CSF_AT(int, hitBytes, 0x04));
    return 1;
}

HOOK(0x006B6D70, S_006b6be0_QueryGroundSurfaceFlag, "auto");
