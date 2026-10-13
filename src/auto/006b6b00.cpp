#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(uint32_t*, __cdecl, RpWorldCreate, 0x0058CCB0, (uint32_t* a1));

// Lazily creates the visibility world (RpWorld) for this scene, with a
// fixed bounding box: min = (-1, -1, -1), max = (1, 1, 1).
static uint32_t* __fastcall GfxScene_InitializeVisibilityResources(C_00860388* self, void* /*edx*/)
{
    uint32_t* result = reinterpret_cast<uint32_t*>(self->field_0010);
    if (!result)
    {
        // Bounds as raw float bit patterns: {1,1,1, -1,-1,-1}
        uint32_t bounds[6];
        bounds[0] = 0x3f800000u;  // 1.0f
        bounds[1] = 0x3f800000u;  // 1.0f
        bounds[2] = 0x3f800000u;  // 1.0f
        bounds[3] = 0xbf800000u;  // -1.0f
        bounds[4] = 0xbf800000u;  // -1.0f
        bounds[5] = 0xbf800000u;  // -1.0f

        result = RpWorldCreate(bounds);
        self->field_0010 = reinterpret_cast<uint32_t>(result);
    }
    return result;
}

HOOK(0x006B6B00, GfxScene_InitializeVisibilityResources, "auto");
