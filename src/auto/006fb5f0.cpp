#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __cdecl, RwRasterCreate, 0x00627770, (int width, int height, int depth, int flags));
CSF_FUNC(void*, __fastcall, LookupObjectById, 0x00652160, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(int, __fastcall, ResolveRwManager, 0x006B13B0, (int managerId, void* edx));
CSF_FUNC(int, __fastcall, GfxScreenEffects_CreateManagedRaster, 0x006D0BA0,
         (void* self, void* edx, int a2, int a3, int a4, char a5, uint32_t* a6));

static char __fastcall GfxScreenEffects_CreateRasters(void* self, void* /*edx*/)
{
    LookupObjectById(reinterpret_cast<void*>(GetGlobalInt()), nullptr);

    int raster = RwRasterCreate(512, 512, 24, 5);
    CSF_AT(int, self, 0x64) = raster;
    if (!raster)
        return 0;

    void* manager = reinterpret_cast<void*>(ResolveRwManager(GetGlobalInt(), nullptr));
    int managedRaster = GfxScreenEffects_CreateManagedRaster(
        manager, nullptr, 512, 512, 24, 1, nullptr);
    CSF_AT(int, self, 0x60) = managedRaster;
    return managedRaster ? 1 : 0;
}

HOOK(0x006FB5F0, GfxScreenEffects_CreateRasters, "auto");
