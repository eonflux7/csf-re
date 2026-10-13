#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_GLOBAL(int, g_SectorStateCounter, 0x008BB494);

CSF_FUNC(int, __cdecl, GetGlobalValue_6C5760, 0x006C5760, ());
CSF_FUNC(void*, __fastcall, GetCachedObjectProvider, 0x00686C10, (void* self, void* edx));
CSF_FUNC(int, __cdecl, RpWorldForAllWorldSectors, 0x0058CFF0, (int a1, int (__cdecl* a2)(int*, int), int a3));
CSF_FUNC(int, __cdecl, RwCameraForAllWorldSectors, 0x0058DEF0, (int a1, int (__cdecl* a2)(uint32_t, int), int a3));

// Sets the per-call serial counter (wrapping at INT_MAX), then walks the world sectors
// with the sector callback, either through the camera provider (flag set) or the world (flag clear).
static int __fastcall GfxScene_SetClumpState(void* self, void* /*edx*/, char clumpFlag)
{
    int result = CSF_AT(int, self, 0x4);
    if (result)
    {
        if (g_SectorStateCounter >= 0x7FFFFFFF)
            g_SectorStateCounter = 0;
        else
            g_SectorStateCounter = g_SectorStateCounter + 1;

        int stateSerial = g_SectorStateCounter;
        char storedFlag = clumpFlag;

        uint8_t* globals = reinterpret_cast<uint8_t*>(GetGlobalValue_6C5760());
        C_0085e7b8* sceneClump = CSF_AT(C_0085e7b8*, globals, 0xF0);

        if (clumpFlag)
        {
            void* provider = GetCachedObjectProvider(sceneClump, nullptr);
            return RwCameraForAllWorldSectors(
                reinterpret_cast<int>(provider),
                reinterpret_cast<int (__cdecl*)(uint32_t, int)>(CSF_ADDR_FN(0x006BC8F0)),
                reinterpret_cast<int>(&sceneClump));
        }
        else
        {
            return RpWorldForAllWorldSectors(
                CSF_AT(int, self, 0x4),
                reinterpret_cast<int (__cdecl*)(int*, int)>(CSF_ADDR_FN(0x006BC8F0)),
                reinterpret_cast<int>(&sceneClump));
        }
    }
    return result;
}

HOOK(0x006BC970, GfxScene_SetClumpState, "auto");
