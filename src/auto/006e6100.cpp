#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(uint32_t*, __cdecl, RpMaterialSetTexture, 0x00591560, (uint32_t* a1, int a2));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));
CSF_FUNC(int, __cdecl, GfxFrames_CollectGeometryMaterials, 0x006C0BF0, (int geometryHandle, int collectorHandle));

static void __fastcall S_006e6100_ApplyMaterialTextures(
    S_006e6100* self, void* /*edx*/)
{
    uint32_t texturesBegin = self->field_0034;
    if (!texturesBegin)
        return;
    if ((static_cast<int32_t>(self->field_0038 - texturesBegin) >> 2) == 0)
        return;

    const int geometryHandle =
        CSF_AT(int, reinterpret_cast<void*>(self->field_000c), 0x18);
    if (!geometryHandle)
        return;

    alignas(8) uint8_t collectorBytes[0x10];
    CSF_AT(uint32_t, collectorBytes, 0x0) = 0;
    CSF_AT(uint32_t, collectorBytes, 0x4) = 0;
    CSF_AT(uint32_t, collectorBytes, 0x8) = 0;
    CSF_AT(int, collectorBytes, 0xC) = 10;

    GfxFrames_CollectGeometryMaterials(
        geometryHandle, reinterpret_cast<int>(collectorBytes));

    texturesBegin = self->field_0034;
    const int32_t textureCount = texturesBegin
        ? (static_cast<int32_t>(self->field_0038 - texturesBegin) >> 2)
        : 0;

    const uint32_t materialCount = CSF_AT(uint32_t, collectorBytes, 0x8);
    if (materialCount == static_cast<uint32_t>(textureCount))
    {
        for (uint32_t index = 0; index < materialCount; ++index)
        {
            const int texture = CSF_AT(
                int, reinterpret_cast<void*>(self->field_0034), index * 4);
            if (texture)
            {
                const uint32_t material = CSF_AT(
                    uint32_t,
                    reinterpret_cast<void*>(CSF_AT(uint32_t, collectorBytes, 0x0)),
                    index * 4);
                RpMaterialSetTexture(
                    reinterpret_cast<uint32_t*>(material), texture);
            }
        }
    }

    if (CSF_AT(uint32_t, collectorBytes, 0x0))
        Mem_Free(reinterpret_cast<void*>(CSF_AT(uint32_t, collectorBytes, 0x0)));
}

HOOK(0x006E6100, S_006e6100_ApplyMaterialTextures, "auto");
