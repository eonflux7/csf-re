#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(int, __cdecl, RwRasterDestroy, 0x00627570, (uint32_t raster));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(int, __cdecl, Water_ReleaseBindings, 0x00709A80, (int a1, int a2, int a3, int a4));

// Releases the water raster and the sub-block stored at this+0xDC.
// Returns the low byte of the last eax value, as the original does.
static char __fastcall S_006fdbb0_ReleaseWaterResources(S_006fdbb0* self, void* /*edx*/)
{
    if (self->field_00f4)
        RwRasterDestroy(self->field_00f4);

    uint32_t firstValue = self->field_00e0;
    uint8_t* pad = reinterpret_cast<uint8_t*>(self) + 0xDC;
    uint32_t result = firstValue;

    if (firstValue)
    {
        Water_ReleaseBindings(static_cast<int>(firstValue),
                              CSF_AT(int, pad, 0x8),
                              reinterpret_cast<int>(pad),
                              reinterpret_cast<int>(self));
        result = CSF_AT(uint32_t, pad, 0x4);
        if (result)
            result = static_cast<uint32_t>(Mem_FreeDebug(reinterpret_cast<void*>(result)));
    }

    CSF_AT(uint32_t, pad, 0x4) = 0;
    CSF_AT(uint32_t, pad, 0x8) = 0;
    CSF_AT(uint32_t, pad, 0xC) = 0;

    return static_cast<char>(result);
}

HOOK(0x0070D3F0, S_006fdbb0_ReleaseWaterResources, "auto");
