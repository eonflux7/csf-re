#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, RwRasterDestroy, 0x00627570, (uint32_t raster));

// Destroys the four RwRaster handles stored from CRenderEngine+0x40 onward,
// clearing each slot after destroying it. Returns the last value the original leaves in eax.
static int __fastcall CRenderEngine_DestroyRasterSlots(CRenderEngine* self, void* /*edx*/)
{
    uint32_t* rasterSlot = &self->field_0040;
    int slotsLeft = 4;
    int result = 0;
    do
    {
        result = static_cast<int>(*rasterSlot);
        if (*rasterSlot)
        {
            result = RwRasterDestroy(*rasterSlot);
            *rasterSlot = 0;
        }
        ++rasterSlot;
        --slotsLeft;
    } while (slotsLeft != 0);
    return result;
}

HOOK(0x006DBCA0, CRenderEngine_DestroyRasterSlots, "auto");
