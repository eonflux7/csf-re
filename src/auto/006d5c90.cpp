#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, RwTextureDestroy, 0x006317E0, (int texture));
CSF_FUNC(int, __cdecl, RwRasterCreate, 0x00627770, (int width, int height, int depth, int flags));
CSF_FUNC(int, __cdecl, RwTextureCreate, 0x00631780, (int a1));

// Destroys any existing texture, then creates a 24-bit raster (256x256 for
// size tiers 1..2, otherwise 512x512) and wraps it in a new texture.
// Returns 1 on success, 0 if the raster could not be created.
static char __fastcall S_006d5c90_RecreateTexture(S_006d5c90* self, void* /*edx*/)
{
    if (self->field_0090)
    {
        RwTextureDestroy(static_cast<int>(self->field_0090));
        self->field_0094 = 0;
    }

    const int sizeTier = static_cast<int>(self->field_0098);
    int size = 512;
    if (sizeTier > 0 && sizeTier <= 2)
        size = 256;

    const uint32_t raster = static_cast<uint32_t>(RwRasterCreate(size, size, 24, 5));
    self->field_0094 = raster;
    if (!raster)
        return 0;

    self->field_0090 = static_cast<uint32_t>(RwTextureCreate(static_cast<int>(raster)));
    return 1;
}

HOOK(0x006D5C90, S_006d5c90_RecreateTexture, "auto");
