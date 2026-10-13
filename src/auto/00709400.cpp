#include <cstdint>
#include "globals.h"
#include <cstddef>
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(int, __fastcall, ResolveRwManager, 0x006B13B0, (int managerId, void* edx));
CSF_FUNC(int, __fastcall, GfxScreenEffects_CreateManagedRaster, 0x006D0BA0, (void* self, void* edx, int a2, int a3, int a4, char a5, uint32_t* a6));

// Resolves the global render manager, creates the 512x512 24-bit managed raster
// through that manager, and stores the raster handle in field_0060. Returns 1.
static char __fastcall C_00862b58_InitScreenEffectRaster(C_00862b58* self, void* /*edx*/)
{
    int managerId = GetGlobalInt();
    int renderManager = ResolveRwManager(managerId, nullptr);

    int rasterHandle = GfxScreenEffects_CreateManagedRaster(
        reinterpret_cast<void*>(static_cast<intptr_t>(renderManager)),
        nullptr, 512, 512, 24, 1, nullptr);
    self->field_0060 = static_cast<uint32_t>(rasterHandle);
    return 1;
}

HOOK(0x709400, C_00862b58_InitScreenEffectRaster, "auto");
