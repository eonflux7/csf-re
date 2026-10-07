#include <cstdint>
#include "globals.h"
#include "types/auto_classes.h"
#include "hook.h"

// int __cdecl RwRasterPushContext(int raster) — returns nonzero on success
CSF_FUNC(int, __cdecl, RwRasterPushContext, 0x6275B0, (int raster));

// Pushes the render context of the raster referenced by the object stored at field_000c
// (that object holds the raster pointer at +0x60). Returns whether the push succeeded.
static int __fastcall C_008655e8_PushRasterContext(C_008655e8* self, void* /*edx*/)
{
    uint32_t contextOwner = self->field_000c;
    int raster = static_cast<int>(CSF_AT(uint32_t, reinterpret_cast<void*>(contextOwner), 0x60));

    int pushed = RwRasterPushContext(raster);
    return pushed != 0;
}

HOOK(0x726A50, C_008655e8_PushRasterContext, "auto");
