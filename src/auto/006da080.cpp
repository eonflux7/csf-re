#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, RwRasterDestroy, 0x00627570, (uint32_t raster));
CSF_FUNC(int, __cdecl, RwRasterSubRaster, 0x00627710, (int raster, int parentRaster, int* rectangle));
CSF_FUNC(int, __cdecl, RwRasterCreate, 0x00627770, (int width, int height, int depth, int flags));

static bool __fastcall S_006d24a0_RecreateRasters(
    S_006d24a0* self, void* /*edx*/)
{
    if (self->field_0008)
        RwRasterDestroy(self->field_0008);
    if (self->field_0004)
        RwRasterDestroy(self->field_0004);

    uint32_t scaleMode = self->field_00c8 - 1;
    int dimension = static_cast<int32_t>(self->field_0038);
    self->field_0004 = 0;
    self->field_0008 = 0;

    if (scaleMode == 0)
        dimension /= 2;
    else if (scaleMode == 1)
        dimension /= 4;

    int raster = RwRasterCreate(dimension, dimension, 0, 5);
    self->field_0004 = static_cast<uint32_t>(raster);
    if (!raster)
        return false;

    int subRaster = RwRasterCreate(0, 0, 0, 5);
    self->field_0008 = static_cast<uint32_t>(subRaster);
    uint32_t parentRaster = self->field_0004;
    void* parent = reinterpret_cast<void*>(parentRaster);

    alignas(8) uint8_t rectangleBytes[0x10];
    int* rectangle = reinterpret_cast<int*>(rectangleBytes);
    rectangle[0] = 1;
    rectangle[1] = 1;
    rectangle[2] = static_cast<int32_t>(CSF_AT(uint32_t, parent, 0x0C) - 2);
    rectangle[3] = static_cast<int32_t>(CSF_AT(uint32_t, parent, 0x10) - 2);

    RwRasterSubRaster(subRaster, static_cast<int>(parentRaster), rectangle);
    return true;
}

HOOK(0x006DA080, S_006d24a0_RecreateRasters, "auto");
