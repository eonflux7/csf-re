#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, RwFrameDestroy, 0x00623B10, (int frame));
CSF_FUNC(int, __cdecl, RwRasterDestroy, 0x00627570, (uint32_t raster));
CSF_FUNC(int, __cdecl, RwCameraDestroy, 0x00629070, (int camera));
CSF_FUNC(int, __cdecl, _rwObjectHasFrameSetFrame, 0x0063CE30, (int object, int frame));
CSF_FUNC(int, __cdecl, FreeBlock_690F70, 0x00690F70, (void* block));
CSF_FUNC(int, __cdecl, CoreMemory_Free, 0x00691110, (void* block));
CSF_FUNC(char, __cdecl, ScreenEffects_ReleaseEmbeddedResources, 0x007090A0, (int* a1));

static char __fastcall C_008629a4_ReleaseResources(
    C_008629a4* self, void* /*edx*/)
{
    char result = CSF_AT(char, self, 0x08);
    if (!result)
        return result;

    if (CSF_AT(uint32_t, self, 0x0C))
    {
        void* camera = reinterpret_cast<void*>(CSF_AT(uint32_t, self, 0x0C));
        int frame = CSF_AT(int, camera, 0x04);
        CSF_AT(uint32_t, camera, 0x60) = 0;
        _rwObjectHasFrameSetFrame(CSF_AT(int, self, 0x0C), 0);
        if (frame)
            RwFrameDestroy(frame);
        RwCameraDestroy(CSF_AT(int, self, 0x0C));
        CSF_AT(uint32_t, self, 0x0C) = 0;
    }

    if (CSF_AT(uint32_t, self, 0x10))
    {
        void* camera = reinterpret_cast<void*>(CSF_AT(uint32_t, self, 0x10));
        int frame = CSF_AT(int, camera, 0x04);
        CSF_AT(uint32_t, camera, 0x60) = 0;
        _rwObjectHasFrameSetFrame(CSF_AT(int, self, 0x10), 0);
        if (frame)
            RwFrameDestroy(frame);
        RwCameraDestroy(CSF_AT(int, self, 0x10));
        CSF_AT(uint32_t, self, 0x10) = 0;
    }

    if (CSF_AT(uint32_t, self, 0x14))
    {
        RwRasterDestroy(CSF_AT(uint32_t, self, 0x14));
        CSF_AT(uint32_t, self, 0x14) = 0;
    }

    ScreenEffects_ReleaseEmbeddedResources(
        reinterpret_cast<int*>(reinterpret_cast<uint8_t*>(self) + 0x50));

    if (CSF_AT(uint32_t, self, 0x0C))
    {
        CoreMemory_Free(
            reinterpret_cast<void*>(CSF_AT(uint32_t, self, 0x0C)));
        CSF_AT(uint32_t, self, 0x0C) = 0;
    }

    if (CSF_AT(uint32_t, self, 0x10))
    {
        CoreMemory_Free(
            reinterpret_cast<void*>(CSF_AT(uint32_t, self, 0x10)));
        CSF_AT(uint32_t, self, 0x10) = 0;
    }

    result = static_cast<char>(FreeBlock_690F70(
        reinterpret_cast<void*>(CSF_AT(uint32_t, self, 0x18))));
    CSF_AT(char, self, 0x08) = 0;
    return result;
}

HOOK(0x007090F0, C_008629a4_ReleaseResources, "auto");
