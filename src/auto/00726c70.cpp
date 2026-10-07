#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, RwCameraCreate, 0x006290B0, ());
CSF_FUNC(int, __cdecl, RwRasterCreate, 0x00627770,
         (int width, int height, int depth, int flags));
CSF_FUNC(int, __cdecl, RwRasterSubRaster, 0x00627710,
         (int raster, int parentRaster, int* rectangle));

static int __fastcall C_008655e8_CreateCameraSubRasters(
    C_008655e8* self, void* /*edx*/,
    int colorParentRaster, int depthParentRaster, int width, int height)
{
    int result = RwCameraCreate();
    self->field_000c = static_cast<std::uint32_t>(result);

    if (result != 0)
    {
        const int colorRaster = RwRasterCreate(0, 0, 0, 2);
        int rectangle[4] = {0, 0, width, height};

        RwRasterSubRaster(colorRaster, colorParentRaster, rectangle);
        CSF_AT(std::uint32_t, reinterpret_cast<void*>(self->field_000c), 0x60) =
            static_cast<std::uint32_t>(colorRaster);

        const int depthRaster = RwRasterCreate(0, 0, 0, 1);
        result = RwRasterSubRaster(depthRaster, depthParentRaster, rectangle);
        CSF_AT(std::uint32_t, reinterpret_cast<void*>(self->field_000c), 0x64) =
            static_cast<std::uint32_t>(depthRaster);
    }

    return result;
}

HOOK(0x00726C70, C_008655e8_CreateCameraSubRasters, "auto");
