#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Bink close entry point, read through the global pointer slot at 0x82D390 (indirect call in the original).
CSF_GLOBAL(void*, g_BinkCloseFn, 0x0082D390);

CSF_FUNC(int, __cdecl, RwRasterDestroy, 0x00627570, (uint32_t raster));
CSF_FUNC(char, __fastcall, C_008619e0_ConsumeFlag0008, 0x006E03D0, (C_008619e0* self, void* edx));

// Releases the Bink video handle and the RenderWare raster if they were created,
// then clears the flag at +0x08 and tail-calls the base-class flag consumer.
static char __fastcall C_00861a00_ReleaseMediaHandles(C_00861a00* self, void* edx)
{
    char result = CSF_AT(char, self, 0x08);
    if (result)
    {
        uint32_t binkHandle = self->field_000c;
        if (binkHandle)
        {
            // The original keeps ecx = this from entry when it calls through [0x82D390].
            // Pass self in ecx and the entry edx through; the single stack argument is the Bink handle.
            reinterpret_cast<void(__fastcall*)(C_00861a00*, void*, uint32_t)>(g_BinkCloseFn)(self, edx, binkHandle);
        }

        uint32_t raster = self->field_0014;
        if (raster)
        {
            RwRasterDestroy(raster);
        }

        return C_008619e0_ConsumeFlag0008(reinterpret_cast<C_008619e0*>(self), nullptr);
    }
    return result;
}

HOOK(0x702B80, C_00861a00_ReleaseMediaHandles, "auto");
