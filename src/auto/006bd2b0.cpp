#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __cdecl, S_006fdfe0_FindWorldFromClump, 0x0058DF40, (int a1));
CSF_FUNC(int, __cdecl, RpWorldRemoveClump, 0x0058E310, (int a1, int a2));
CSF_FUNC(int, __cdecl, RpClumpForAllLights, 0x0058EF90,
         (int a1, int (__cdecl *a2)(char *, int), int a3));
CSF_FUNC(int, __cdecl, RpClumpRemoveLight, 0x0058F1D0, (int a1, int a2));
CSF_FUNC(int, __cdecl, RpClumpDestroy, 0x0058F740, (int a1));
CSF_FUNC(int, __cdecl, RwFrameRemoveChild, 0x00623750, (int a1));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));
CSF_FUNC(int, __cdecl, GetGlobalValue_6C5760, 0x006C5760, ());
CSF_FUNC(int, __stdcall, GfxSkin_ForwardWithSkinCallback, 0x006C5CB0, (int a1));

static int __stdcall GfxEnvironment_DestroyClumpArray(
    int* clumps, char skipSkinCleanup)
{
    alignas(8) uint8_t lightArrayBytes[0x18];
    const uint32_t clumpCount = CSF_AT(uint32_t, clumps, 0x8);

    CSF_AT(void*, lightArrayBytes, 0x0) = nullptr;
    CSF_AT(uint32_t, lightArrayBytes, 0x4) = 0;
    CSF_AT(uint32_t, lightArrayBytes, 0x8) = 0;
    CSF_AT(int, lightArrayBytes, 0xC) = 10;

    for (uint32_t clumpIndex = 0; clumpIndex < clumpCount; ++clumpIndex)
    {
        const int clump = CSF_AT(int*, clumps, 0x0)[clumpIndex];

        if (S_006fdfe0_FindWorldFromClump(clump))
        {
            const int world = S_006fdfe0_FindWorldFromClump(clump);
            RpWorldRemoveClump(world, clump);
        }

        CSF_AT(uint32_t, lightArrayBytes, 0x8) = 0;
        RpClumpForAllLights(
            clump,
            reinterpret_cast<int (__cdecl *)(char*, int)>(
                CSF_ADDR_FN(0x006BD100)),
            reinterpret_cast<int>(lightArrayBytes));

        if (CSF_AT(uint32_t, lightArrayBytes, 0x8))
        {
            uint32_t lightIndex = 0;
            do
            {
                const int light =
                    CSF_AT(int*, lightArrayBytes, 0x0)[lightIndex];
                RpClumpRemoveLight(clump, light);
                RwFrameRemoveChild(
                    CSF_AT(int, reinterpret_cast<void*>(light), 0x4));
                ++lightIndex;
            }
            while (lightIndex < CSF_AT(uint32_t, lightArrayBytes, 0x8));
        }

        if (!skipSkinCleanup)
        {
            GetGlobalValue_6C5760();
            GfxSkin_ForwardWithSkinCallback(clump);
        }

        RpClumpDestroy(clump);
    }

    void* clumpStorage = CSF_AT(void*, clumps, 0x0);
    CSF_AT(uint32_t, clumps, 0x4) = 0;
    CSF_AT(uint32_t, clumps, 0x8) = 0;
    if (clumpStorage)
    {
        Mem_Free(clumpStorage);
        CSF_AT(void*, clumps, 0x0) = nullptr;
    }

    void* lightStorage = CSF_AT(void*, lightArrayBytes, 0x0);
    CSF_AT(uint32_t, lightArrayBytes, 0x4) = 0;
    CSF_AT(uint32_t, lightArrayBytes, 0x8) = 0;
    if (lightStorage)
        return Mem_Free(lightStorage);

    return 0;
}

HOOK(0x006BD2B0, GfxEnvironment_DestroyClumpArray, "auto");
