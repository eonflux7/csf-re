#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, RwRasterCreate, 0x00627770, (int width, int height, int depth, int flags));
CSF_FUNC(int, __cdecl, RwCameraCreate, 0x006290B0, ());
CSF_FUNC(int, __cdecl, _rwObjectHasFrameSetFrame, 0x0063CE30, (int object, int frame));
CSF_FUNC(void*, __fastcall, LookupObjectById, 0x00652160, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(int, __fastcall, GfxWater_SetCameraViewport, 0x006D29F0, (void* self, void* edx, int a2, int a3, int a4, int a5));

static int __fastcall C_008655e8_CreateCameraResources(
    C_008655e8* self, void* /*edx*/)
{
    self->field_000c = RwCameraCreate();

    LookupObjectById(reinterpret_cast<void*>(GetGlobalInt()), nullptr);

    const int colorRaster = RwRasterCreate(0, 0, 0, 2);
    CSF_AT(uint32_t, reinterpret_cast<void*>(self->field_000c), 0x60) =
        static_cast<uint32_t>(colorRaster);

    const int depthRaster = RwRasterCreate(0, 0, 0, 1);
    CSF_AT(uint32_t, reinterpret_cast<void*>(self->field_000c), 0x64) =
        static_cast<uint32_t>(depthRaster);

    GfxWater_SetCameraViewport(
        self, nullptr,
        static_cast<int>(self->field_0034),
        static_cast<int>(self->field_0038),
        static_cast<int>(self->field_003c - self->field_0034),
        static_cast<int>(self->field_0040 - self->field_0038));

    _rwObjectHasFrameSetFrame(
        static_cast<int>(self->field_000c),
        static_cast<int>(self->field_001c));

    self->vCreateTextureViewEx(static_cast<int>(self->field_004c));
    self->vCreateTextureWrapper(static_cast<int>(self->field_0048));
    self->vCreateResourceWrapper(CSF_AT(int, self, 0x44));

    return reinterpret_cast<int(__fastcall*)(C_008655e8*, void*, uint32_t)>(
        self->vtbl[0x20 / 4])(self, nullptr, self->field_0050);
}

HOOK(0x006D3250, C_008655e8_CreateCameraResources, "auto");
