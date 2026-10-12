#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(void*, __fastcall, LookupObjectById, 0x00652160, (void* self, void* edx));
CSF_FUNC(int, __fastcall, AppMain_GetDisplayRaster, 0x00534FA0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, GetAccountSelectionIndex, 0x004A2FC0, (void* self, void* edx));
CSF_FUNC(int, __cdecl, RwRasterSubRaster, 0x00627710, (int raster, int parentRaster, int* rectangle));

// Sets the camera viewport rectangles from (left, top, width, height) and
// applies them to the two RW rasters; the second call's result is returned.
static int __fastcall C_008655e8_SetCameraViewport(C_008655e8* self, void* /*edx*/,
                                                   int left, int top, int width, int height)
{
    int globalIntValue = GetGlobalInt();
    void* menu = LookupObjectById(reinterpret_cast<void*>(globalIntValue), nullptr);

    // Raster held by the first sub-object, at offset 0x60.
    uint32_t firstRaster = CSF_AT(uint32_t, reinterpret_cast<void*>(self->field_000c), 0x60);

    int rect[4] = { left, top, width, height };

    int displayRaster = AppMain_GetDisplayRaster(menu, nullptr);
    RwRasterSubRaster(static_cast<int>(firstRaster), displayRaster, rect);

    self->field_0038 = static_cast<uint32_t>(top);
    self->field_0034 = static_cast<uint32_t>(left);
    self->field_0040 = static_cast<uint32_t>(height + top);
    self->field_003c = static_cast<uint32_t>(width + left);

    // Raster held by the first sub-object, at offset 0x64.
    uint32_t secondRaster = CSF_AT(uint32_t, reinterpret_cast<void*>(self->field_000c), 0x64);

    int accountIndex = GetAccountSelectionIndex(menu, nullptr);
    return RwRasterSubRaster(static_cast<int>(secondRaster), accountIndex, rect);
}

HOOK(0x006D29F0, C_008655e8_SetCameraViewport, "auto");
