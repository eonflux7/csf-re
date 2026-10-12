#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(void*, __fastcall, LookupObjectById, 0x00652160, (void* self, void* edx));
CSF_FUNC(int, __fastcall, ResolveRwManager, 0x006B13B0, (int managerId, void* edx));
CSF_FUNC(int, __fastcall, GfxScreenEffects_CreateManagedRaster, 0x006D0BA0,
         (void* self, void* edx, int a2, int a3, int a4, char a5, uint32_t* a6));

// Fills the five-level raster chain (256, 128, 64, 32, 16) stored at this+0x60.
// Each level is created through the render manager; the owner's virtual slot 6
// is notified after every creation. Returns 1 only if every raster was created.
static char __fastcall GfxScreenEffects_AllocateRasterPyramid(void* self, void* edx)
{
    char allCreated = 1;

    int globalIntValue = GetGlobalInt();
    int v3 = 0;
    void* lookedUpObject = LookupObjectById(reinterpret_cast<void*>(static_cast<uintptr_t>(globalIntValue)), nullptr);

    for (;;)
    {
        if (!allCreated)
            break;

        int rwManagerId = GetGlobalInt();
        int rasterManager = ResolveRwManager(rwManagerId, nullptr);

        int rasterSize = 256 >> v3;
        int raster = GfxScreenEffects_CreateManagedRaster(
            reinterpret_cast<void*>(static_cast<uintptr_t>(rasterManager)), nullptr,
            rasterSize, rasterSize, 24, 1, nullptr);
        CSF_AT(int, self, 0x60 + v3 * 4) = raster;

        ((void(__fastcall*)(void*, void*))CSF_AT(void**, lookedUpObject, 0)[24 / 4])(lookedUpObject, nullptr);

        if (CSF_AT(int, self, 0x60 + v3 * 4) == 0)
            allCreated = 0;

        ++v3;
        if (v3 >= 5)
            return allCreated;
    }
    return allCreated;
}

HOOK(0x006FBB80, GfxScreenEffects_AllocateRasterPyramid, "auto");
