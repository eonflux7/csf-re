#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, RwFrameDestroy, 0x00623B10, (int frame));
CSF_FUNC(int, __cdecl, RwRasterDestroy, 0x00627570, (int raster));
CSF_FUNC(int, __cdecl, RwCameraDestroy, 0x00629070, (int camera));
CSF_FUNC(int, __cdecl, RwTexDictionaryForAllTextures, 0x006316E0,
         (int dictionary, int (__cdecl* callback)(uint32_t*, int), int context));
CSF_FUNC(int, __cdecl, RwTexDictionaryDestroy, 0x00632340, (int dictionary));
CSF_FUNC(int, __cdecl, _rwObjectHasFrameSetFrame, 0x0063CE30, (int object, int frame));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));
CSF_FUNC(int, __fastcall, ReleaseObjectReference, 0x006923C0,
         (void* object, void* edx));
CSF_FUNC(int, __fastcall, DestroyAuxiliaryObject, 0x006AF830,
         (void* object, void* edx));
CSF_FUNC(int, __cdecl, ForwardArgumentsToProcessor_727370, 0x00727370,
         (int begin, int end));
CSF_FUNC(void*, __cdecl, FreeGlobalBlocks, 0x00739410, ());
CSF_FUNC(int, __cdecl, DestroyDictionaryTexture, 0x006C1120,
         (uint32_t* texture, int context));

static int __fastcall C_008655e8_Destroy(C_008655e8* self, void* /*edx*/)
{
    void* auxiliary = reinterpret_cast<void*>(self->field_0080);
    CSF_AT(void**, self, 0) = CSF_VTBL(0x008655E8);

    if (auxiliary)
    {
        DestroyAuxiliaryObject(auxiliary, nullptr);
        Mem_Free(auxiliary);
        self->field_0080 = 0;
    }

    const uint32_t recordsBegin = self->field_0048;
    uint32_t remaining = 0;
    if (recordsBegin)
    {
        const int32_t byteCount =
            static_cast<int32_t>(CSF_AT(uint32_t, self, 0x4C) - recordsBegin);
        remaining = static_cast<uint32_t>(byteCount / 36);
    }

    uint32_t recordOffset = 0;
    while (remaining != 0)
    {
        RwRasterDestroy(CSF_AT(int, reinterpret_cast<void*>(
            self->field_0048 + recordOffset), 0));
        recordOffset += 36;
        --remaining;
    }

    if (self->field_0048)
    {
        ForwardArgumentsToProcessor_727370(self->field_0048, CSF_AT(uint32_t, self, 0x4C));
        if (self->field_0048)
            Mem_FreeDebug(reinterpret_cast<void*>(self->field_0048));
    }
    self->field_0048 = 0;
    CSF_AT(uint32_t, self, 0x4C) = 0;
    CSF_AT(uint32_t, self, 0x50) = 0;

    uint32_t objectIndex = self->field_003c - 1;
    self->field_0054 = 0;
    while (static_cast<int32_t>(objectIndex) >= 0)
    {
        void* object = reinterpret_cast<void*>(CSF_AT(
            uint32_t, reinterpret_cast<void*>(self->field_0034),
            objectIndex * 4));
        if (object)
        {
            while (ReleaseObjectReference(object, nullptr) != 0)
            {
            }
        }
        --objectIndex;
    }

    const uint32_t dictionary = self->field_002c;
    self->field_003c = 0;
    if (dictionary)
    {
        RwTexDictionaryForAllTextures(dictionary, DestroyDictionaryTexture, 0);
        RwTexDictionaryDestroy(self->field_002c);
        self->field_002c = 0;
        self->field_0030 = 0;
    }

    void* camera = reinterpret_cast<void*>(self->field_000c);
    if (camera)
    {
        if (CSF_AT(uint32_t, camera, 4))
            _rwObjectHasFrameSetFrame(reinterpret_cast<int>(camera), 0);

        camera = reinterpret_cast<void*>(self->field_000c);
        const int colorRaster = CSF_AT(int, camera, 0x60);
        if (colorRaster)
        {
            CSF_AT(uint32_t, camera, 0x60) = 0;
            RwRasterDestroy(colorRaster);
        }

        camera = reinterpret_cast<void*>(self->field_000c);
        const int depthRaster = CSF_AT(int, camera, 0x64);
        if (depthRaster)
        {
            CSF_AT(uint32_t, camera, 0x64) = 0;
            RwRasterDestroy(depthRaster);
        }

        RwCameraDestroy(self->field_000c);
    }

    RwFrameDestroy(self->field_0008);
    FreeGlobalBlocks();

    if (self->field_006c)
    {
        Mem_Free(reinterpret_cast<void*>(self->field_006c));
        self->field_006c = 0;
    }
    if (self->field_0058)
    {
        Mem_Free(reinterpret_cast<void*>(self->field_0058));
        self->field_0058 = 0;
    }

    if (self->field_0048)
    {
        ForwardArgumentsToProcessor_727370(self->field_0048, CSF_AT(uint32_t, self, 0x4C));
        if (self->field_0048)
            Mem_FreeDebug(reinterpret_cast<void*>(self->field_0048));
    }
    self->field_0048 = 0;
    CSF_AT(uint32_t, self, 0x4C) = 0;
    CSF_AT(uint32_t, self, 0x50) = 0;

    if (self->field_0034)
    {
        Mem_Free(reinterpret_cast<void*>(self->field_0034));
        self->field_0034 = 0;
    }

    int result = static_cast<int>(self->field_001c);
    if (result)
        result = Mem_FreeDebug(reinterpret_cast<void*>(self->field_001c));

    self->field_001c = 0;
    self->field_0020 = 0;
    self->field_0024 = 0;
    CSF_AT(void**, self, 0) = CSF_VTBL(0x008655C0);
    return result;
}

HOOK(0x00727BD0, C_008655e8_Destroy, "auto");
