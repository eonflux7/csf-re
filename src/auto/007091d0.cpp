#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, AppMain_GetDisplayRaster, 0x00534FA0, (void* self, void* edx));
CSF_FUNC(void*, __cdecl, RwFrameCreate, 0x006235D0, ());
CSF_FUNC(int, __cdecl, RwRasterSubRaster, 0x00627710, (int raster, int parentRaster, int* rectangle));
CSF_FUNC(int, __cdecl, RwRasterCreate, 0x00627770, (int width, int height, int depth, int flags));
CSF_FUNC(int, __cdecl, RwCameraCreate, 0x006290B0, ());
CSF_FUNC(int, __cdecl, _rwObjectHasFrameSetFrame, 0x0063CE30, (int object, int frame));
CSF_FUNC(void*, __fastcall, LookupObjectById, 0x00652160, (void* self, void* edx));
CSF_FUNC(int, __cdecl, AllocateAlignedBuffer, 0x00690EB0, (std::size_t size, int alignment, int option1, int option2));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(char, __fastcall, ScreenEffects_UpdateBase, 0x007090F0, (void* self, void* edx));

static char __fastcall C_00862b58_InitializeScreenEffect(
    C_00862b58* self, void* /*edx*/, int /*option*/)
{
    ScreenEffects_UpdateBase(self, nullptr);

    self->field_0038 = -1090519040;
    self->field_0034 = -1090519040;
    self->field_000c = 0;
    self->field_0010 = 0;
    self->field_0018 = 0;
    self->field_0014 = 0;
    self->field_0030 = 1;
    self->field_0040 = 0;
    self->field_003c = 0.0f;
    self->field_0048 = 1.0f;
    self->field_0044 = 1.0f;
    self->field_004c = 1;
    self->field_001c = 0;
    self->field_0009 = 1;

    void* globalObject = reinterpret_cast<void*>(GetGlobalInt());
    void* display = LookupObjectById(globalObject, nullptr);

    alignas(8) std::uint8_t rectangleBytes[0x14];
    int* rectangle = reinterpret_cast<int*>(rectangleBytes);
    rectangle[0] = 0;
    rectangle[1] = 0;
    rectangle[2] =
        reinterpret_cast<int (__fastcall*)(void*, void*)>(
            CSF_AT(void**, display, 0)[0x20 / 4])(display, nullptr);
    rectangle[3] =
        reinterpret_cast<int (__fastcall*)(void*, void*)>(
            CSF_AT(void**, display, 0)[0x24 / 4])(display, nullptr);

    int camera = RwCameraCreate();
    self->field_000c = camera;
    if (camera)
    {
        void* frame = RwFrameCreate();
        _rwObjectHasFrameSetFrame(
            static_cast<int>(self->field_000c), reinterpret_cast<int>(frame));
    }

    int raster = RwRasterCreate(0, 0, 0, 2);
    CSF_AT(int, reinterpret_cast<void*>(self->field_000c), 0x60) = raster;
    rectangle[4] = CSF_AT(int, reinterpret_cast<void*>(self->field_000c), 0x60);
    int parentRaster = AppMain_GetDisplayRaster(display, nullptr);
    RwRasterSubRaster(rectangle[4], parentRaster, rectangle);

    camera = RwCameraCreate();
    self->field_0010 = camera;
    if (camera)
    {
        void* frame = RwFrameCreate();
        _rwObjectHasFrameSetFrame(
            static_cast<int>(self->field_0010), reinterpret_cast<int>(frame));
    }

    raster = RwRasterCreate(0, 0, 0, 2);
    CSF_AT(int, reinterpret_cast<void*>(self->field_0010), 0x60) = raster;
    rectangle[4] = CSF_AT(int, reinterpret_cast<void*>(self->field_0010), 0x60);
    parentRaster = AppMain_GetDisplayRaster(display, nullptr);
    RwRasterSubRaster(rectangle[4], parentRaster, rectangle);

    self->field_0018 = AllocateAlignedBuffer(0x70u, 64, 0, 0);
    self->field_0008 = 1;
    return 1;
}

HOOK(0x007091D0, C_00862b58_InitializeScreenEffect, "auto");
