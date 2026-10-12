#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, RwFrameDestroy, 0x00623B10, (int frame));
CSF_FUNC(int, __cdecl, RwRasterDestroy, 0x00627570, (uint32_t raster));
CSF_FUNC(int, __cdecl, RwCameraDestroy, 0x00629070, (int camera));
CSF_FUNC(int, __cdecl, _rwObjectHasFrameSetFrame, 0x0063CE30, (int object, int frame));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));
CSF_FUNC(void, __fastcall, S_006d2390_ClearLights, 0x006D2390, (void* self, void* edx));

// The packet also lists this direct-call address as a vtable reference.
[[maybe_unused]] static const auto g_ClearLightsVtable = CSF_VTBL(0x006D2390);

static int __fastcall S_006d24a0_ReleaseResources(
    S_006d24a0* self, void* /*edx*/)
{
    if (self->field_0008)
        RwRasterDestroy(self->field_0008);
    if (self->field_0004)
        RwRasterDestroy(self->field_0004);

    self->field_0004 = 0;
    self->field_0008 = 0;

    S_006d2390_ClearLights(
        reinterpret_cast<uint8_t*>(self) + 0x44, nullptr);
    S_006d2390_ClearLights(
        reinterpret_cast<uint8_t*>(self) + 0x84, nullptr);

    void* ownedObject = reinterpret_cast<void*>(self->field_00cc);
    if (ownedObject)
    {
        ((void(__fastcall*)(void*, void*, int))
            CSF_AT(void**, ownedObject, 0)[1])(ownedObject, nullptr, 1);
        self->field_00cc = 0;
    }

    uint32_t buffer = self->field_000c;
    self->field_001c = 0;
    self->field_0020 = 0;
    self->field_0010 = 0;
    self->field_0014 = 0;
    if (buffer)
    {
        Mem_Free(reinterpret_cast<void*>(buffer));
        self->field_000c = 0;
    }

    buffer = self->field_0024;
    self->field_0028 = 0;
    self->field_002c = 0;
    if (buffer)
    {
        Mem_Free(reinterpret_cast<void*>(buffer));
        self->field_0024 = 0;
    }

    buffer = self->field_00d8;
    self->field_00e8 = 0;
    self->field_00ec = 0;
    self->field_00dc = 0;
    self->field_00e0 = 0;
    if (buffer)
    {
        Mem_Free(reinterpret_cast<void*>(buffer));
        self->field_00d8 = 0;
    }

    buffer = self->field_00f0;
    self->field_0100 = 0;
    self->field_0104 = 0;
    self->field_00f4 = 0;
    self->field_00f8 = 0;
    if (buffer)
    {
        Mem_Free(reinterpret_cast<void*>(buffer));
        self->field_00f0 = 0;
    }

    int result = static_cast<int>(self->field_00c4);
    if (result)
    {
        int frame = CSF_AT(int, reinterpret_cast<void*>(result), 0x4);
        _rwObjectHasFrameSetFrame(result, 0);
        if (frame)
            RwFrameDestroy(frame);

        result = RwCameraDestroy(static_cast<int>(self->field_00c4));
        self->field_00c4 = 0;
    }
    return result;
}

HOOK(0x006DA9A0, S_006d24a0_ReleaseResources, "auto");
