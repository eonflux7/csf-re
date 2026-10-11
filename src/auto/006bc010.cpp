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

// Engine (RenderWare-style) world API.
CSF_FUNC(uint32_t*, __cdecl, RpWorldCreate, 0x0058CCB0, (uint32_t* a1));
CSF_FUNC(int, __cdecl, RpWorldGetSectorRenderCallBack, 0x0058CC80, (int a1));

// Cached sector render callback shared by all visibility controllers.
CSF_GLOBAL(int, g_SectorRenderCallBack, 0x008BB48C);

// Creates the controller's world on first use and caches its sector render callback.
static int __fastcall GfxScene_InitializeVisibilityController(uint32_t* self, void* /*edx*/)
{
    if (CSF_AT(uint32_t, self, 0x4) == 0)
    {
        // Sector bounds: three +100000.0f followed by three -100000.0f (bit patterns as in the original).
        uint32_t sectorBounds[6];
        sectorBounds[0] = 0x47C35000u;
        sectorBounds[1] = 0x47C35000u;
        sectorBounds[2] = 0x47C35000u;
        sectorBounds[3] = 0xC7C35000u;
        sectorBounds[4] = 0xC7C35000u;
        sectorBounds[5] = 0xC7C35000u;

        CSF_AT(uint32_t, self, 0x4) = static_cast<uint32_t>(
            reinterpret_cast<uintptr_t>(RpWorldCreate(sectorBounds)));
    }

    int result = g_SectorRenderCallBack;
    if (result == 0)
    {
        result = RpWorldGetSectorRenderCallBack(static_cast<int>(CSF_AT(uint32_t, self, 0x4)));
        g_SectorRenderCallBack = result;
    }
    return result;
}

HOOK(0x006BC010, GfxScene_InitializeVisibilityController, "auto");
