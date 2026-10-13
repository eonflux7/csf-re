#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"

// Game heap free (game::Mem_Free), bound by address.
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

CSF_GLOBAL(int*, g_SectorObjects, 0x008A7E50);
CSF_GLOBAL(std::uint32_t, g_SectorObjectSecondary, 0x008A7E54);
CSF_GLOBAL(std::uint32_t, g_SectorObjectCount, 0x008A7E58);

// Clears the sector object list and the owner's "render state built" flag at +0x92.
// Returns the low byte of the free call's result (or 0 when nothing was freed).
static char __fastcall GfxScene_ResetRenderState(uint8_t* self, void* /*edx*/)
{
    char result = (char)(std::uintptr_t)g_SectorObjects;
    if (CSF_AT(uint8_t, self, 0x92)) {
        result = (char)(std::uintptr_t)g_SectorObjects;
        g_SectorObjectSecondary = 0;
        g_SectorObjectCount = 0;
        if (g_SectorObjects) {
            result = (char)Mem_Free((void*)g_SectorObjects);
            g_SectorObjects = nullptr;
        }
        CSF_AT(uint8_t, self, 0x92) = 0;
    }
    return result;
}

HOOK(0x006BC7F0, GfxScene_ResetRenderState, "auto");
