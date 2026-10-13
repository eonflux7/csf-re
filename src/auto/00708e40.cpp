#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, RwRasterSubRaster, 0x00627710, (int raster, int parentRaster, int* rectangle));
CSF_FUNC(int, __cdecl, RwCameraEndUpdate, 0x00628E40, (int camera));
CSF_FUNC(int, __cdecl, RwCameraBeginUpdate, 0x00628E50, (int camera));
CSF_FUNC(int, __cdecl, RwCameraClear, 0x00628F60, (int camera, int color, int flags));
CSF_FUNC(void*, __fastcall, LookupObjectById, 0x00652160, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(int, __fastcall, GfxScreenEffects_DrawQuad, 0x00708AB0, (void* self, void* edx, int a2, char a3, int a4, int a5, int a6, uint8_t a7, float a8, float a9, float a10, float a11));

static char __fastcall C_00862cd4_ApplyLayerColor(
    C_00862cd4* self, void* /*edx*/, int layer, int parentRaster,
    int color, char mode, int clear, float* coordinates)
{
    LookupObjectById(reinterpret_cast<void*>(GetGlobalInt()), nullptr);

    alignas(8) uint8_t rectangleBytes[0x10];
    int* rectangle = reinterpret_cast<int*>(rectangleBytes);
    void* parent = reinterpret_cast<void*>(parentRaster);
    rectangle[3] = CSF_AT(int, parent, 0x10);
    rectangle[2] = CSF_AT(int, parent, 0x0C);
    rectangle[0] = 0;
    rectangle[1] = 0;

    int subRaster = RwRasterSubRaster(
        CSF_AT(int, reinterpret_cast<void*>(self->field_000c), 0x60),
        parentRaster, rectangle);
    CSF_AT(int, reinterpret_cast<void*>(self->field_000c), 0x60) = subRaster;

    void* camera = reinterpret_cast<void*>(self->field_000c);
    int savedDepthRaster = CSF_AT(int, camera, 0x64);
    CSF_AT(int, camera, 0x64) = 0;

    if (static_cast<uint8_t>(clear) != 0)
    {
        clear = 0;
        if (!RwCameraClear(
                static_cast<int>(self->field_000c),
                static_cast<int>(reinterpret_cast<intptr_t>(&clear)), 1))
        {
            uint32_t restoredCamera = self->field_000c;
            CSF_AT(int, reinterpret_cast<void*>(restoredCamera), 0x64) =
                savedDepthRaster;
            return static_cast<char>(restoredCamera);
        }
    }

    if (!RwCameraBeginUpdate(static_cast<int>(self->field_000c)))
    {
        CSF_AT(int, reinterpret_cast<void*>(self->field_000c), 0x64) =
            savedDepthRaster;
        return 0;
    }

    GfxScreenEffects_DrawQuad(
        self, nullptr, color, mode, rectangle[2], rectangle[3],
        layer, 1u, coordinates[0], coordinates[1], 1.0f, 1.0f);

    char result = static_cast<char>(
        RwCameraEndUpdate(static_cast<int>(self->field_000c)));
    CSF_AT(int, reinterpret_cast<void*>(self->field_000c), 0x64) =
        savedDepthRaster;
    return result;
}

HOOK(0x00708E40, C_00862cd4_ApplyLayerColor, "auto");
