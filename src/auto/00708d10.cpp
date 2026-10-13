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

CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(void*, __fastcall, LookupObjectById, 0x00652160, (void* self, void* edx));
CSF_FUNC(int, __fastcall, AppMain_GetDisplayRaster, 0x00534FA0, (void* self, void* edx));
CSF_FUNC(int, __cdecl, RwRasterSubRaster, 0x00627710, (int raster, int parentRaster, int* rectangle));
CSF_FUNC(int, __cdecl, RwCameraBeginUpdate, 0x00628E50, (int camera));

// Looks up the menu network object currently selected by the global int.
static void* LookupCurrentMenuObject()
{
    int globalInt = GetGlobalInt();
    return LookupObjectById(reinterpret_cast<void*>(static_cast<uintptr_t>(globalInt)), nullptr);
}

// Reinterprets a raster handle returned as int as a byte pointer for field reads.
static uint8_t* RasterBytes(int raster)
{
    return reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(raster));
}

static int __fastcall C_00862a80_BeginDrawing(C_00862a80* self, void* /*edx*/)
{
    void* menuObject = LookupCurrentMenuObject();

    CSF_AT(uint32_t, self, 0x20) = 0;
    self->field_0024 = 0;
    self->field_0028 = CSF_AT(uint32_t, RasterBytes(AppMain_GetDisplayRaster(menuObject, nullptr)), 12);
    self->field_002c = CSF_AT(uint32_t, RasterBytes(AppMain_GetDisplayRaster(menuObject, nullptr)), 16);

    void* menuObjectForRaster = LookupCurrentMenuObject();
    uint8_t* camera = reinterpret_cast<uint8_t*>(self->field_0010);
    int cameraSubRasterSource = CSF_AT(int, camera, 0x60);
    int parentRaster = AppMain_GetDisplayRaster(menuObjectForRaster, nullptr);
    RwRasterSubRaster(cameraSubRasterSource, parentRaster,
                      reinterpret_cast<int*>(reinterpret_cast<uint8_t*>(self) + 0x20));

    camera = reinterpret_cast<uint8_t*>(self->field_0010);
    self->field_001c = CSF_AT(uint32_t, camera, 0x64);
    CSF_AT(uint32_t, camera, 0x64) = 0;

    return RwCameraBeginUpdate(static_cast<int>(self->field_0010));
}

HOOK(0x00708D10, C_00862a80_BeginDrawing, "auto");
