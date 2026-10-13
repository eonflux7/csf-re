#include <cstdint>
#include "globals.h"
#include "hook.h"

CSF_FUNC(int, __cdecl, RpMatFXAtomicQueryEffects, 0x005712A0, (int a1));
CSF_FUNC(uint32_t*, __cdecl, RpMaterialSetTexture, 0x00591560, (uint32_t* a1, int a2));
CSF_FUNC(int, __cdecl, RwTextureDestroy, 0x006317E0, (int texture));

CSF_GLOBAL(float, g_ColorChannelScale, 0x0082FEA8);

static int __cdecl GfxIm3d_RenderAtomicGrayscale(int atomic, float* intensity)
{
    if (RpMatFXAtomicQueryEffects(atomic))
        return atomic;

    void* atomicObject = reinterpret_cast<void*>(atomic);
    const uint32_t savedCallback = CSF_AT(uint32_t, atomicObject, 0x48);
    CSF_AT(uint32_t, atomicObject, 0x48) =
        reinterpret_cast<uintptr_t>(CSF_ADDR_FN(0x0058E680));

    const float scaledIntensity = *intensity * g_ColorChannelScale;
    void* geometry = CSF_AT(void*, atomicObject, 0x18);
    const uint8_t channel = static_cast<uint8_t>(
        static_cast<int64_t>(scaledIntensity));
    const uint32_t grayscale =
        uint32_t(channel) | (uint32_t(channel) << 8) |
        (uint32_t(channel) << 16) | 0xFF000000u;

    uint32_t savedMaterialValues[512];
    int materialCount = 0;
    uint32_t savedGeometryFlags = 0;

    if (geometry)
    {
        materialCount = CSF_AT(int, geometry, 0x24);
        if (materialCount > 256)
            materialCount = 256;

        for (int index = 0; index < materialCount; ++index)
        {
            uint32_t* material =
                CSF_AT(uint32_t**, geometry, 0x20)[index];
            if (material)
            {
                const uint32_t texture = CSF_AT(uint32_t, material, 0x0);
                savedMaterialValues[index + 256] = texture;
                if (texture)
                {
                    ++CSF_AT(uint32_t, reinterpret_cast<void*>(texture), 0x54);
                    RpMaterialSetTexture(material, 0);
                }

                savedMaterialValues[index] = CSF_AT(uint32_t, material, 0x4);
                CSF_AT(uint32_t, material, 0x4) = grayscale;
            }
            else
            {
                savedMaterialValues[index + 256] = 0;
            }
        }

        savedGeometryFlags = CSF_AT(uint32_t, geometry, 0x8);
        CSF_AT(uint32_t, geometry, 0x8) = savedGeometryFlags | 0x40;
    }

    reinterpret_cast<void (__cdecl*)(int)>(
        CSF_AT(uint32_t, atomicObject, 0x48))(atomic);

    if (geometry)
    {
        for (int index = 0; index < materialCount; ++index)
        {
            uint32_t* material =
                CSF_AT(uint32_t**, geometry, 0x20)[index];
            if (material)
            {
                const uint32_t texture = savedMaterialValues[index + 256];
                if (texture)
                {
                    RpMaterialSetTexture(material, static_cast<int>(texture));
                    RwTextureDestroy(static_cast<int>(texture));
                    CSF_AT(uint32_t, material, 0x4) = savedMaterialValues[index];
                }
            }
        }

        CSF_AT(uint32_t, geometry, 0x8) = savedGeometryFlags;
    }

    CSF_AT(uint32_t, atomicObject, 0x48) = savedCallback;
    if (!savedCallback)
    {
        CSF_AT(uint32_t, atomicObject, 0x48) =
            reinterpret_cast<uintptr_t>(CSF_ADDR_FN(0x0058E680));
    }

    return atomic;
}

HOOK(0x006E4300, GfxIm3d_RenderAtomicGrayscale, "auto");
