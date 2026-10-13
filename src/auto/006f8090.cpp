#include <cstdint>
#include "globals.h"
#include "hook.h"

CSF_FUNC(uint32_t*, __cdecl, RpMaterialSetTexture, 0x00591560, (uint32_t* a1, int a2));

static int __cdecl GfxMaterials_RenderWithTexture(int atomicAddress, int textureAddress)
{
    using RenderCallback = void (__cdecl*)(void*);

    void* atomic = reinterpret_cast<void*>(atomicAddress);
    RenderCallback savedCallback = CSF_AT(RenderCallback, atomic, 0x48);
    void* geometry = CSF_AT(void*, atomic, 0x18);

    CSF_AT(RenderCallback, atomic, 0x48) =
        reinterpret_cast<RenderCallback>(CSF_ADDR_FN(0x0058E680));

    uint32_t savedColors[256]{};
    int savedTextures[256]{};
    int materialCount = 0;

    if (geometry)
    {
        materialCount = CSF_AT(int, geometry, 0x24);
        if (materialCount > 256)
            materialCount = 256;

        for (int index = 0; index < materialCount; ++index)
        {
            uint32_t* material = CSF_AT(uint32_t**, geometry, 0x20)[index];
            if (material)
            {
                void* texture = CSF_AT(void*, material, 0);
                savedTextures[index] = reinterpret_cast<int>(texture);
                if (texture)
                    ++CSF_AT(uint32_t, texture, 0x54);

                RpMaterialSetTexture(material, textureAddress);
                savedColors[index] = CSF_AT(uint32_t, material, 0x04);
                CSF_AT(uint32_t, material, 0x04) = 0xFFFFFFFF;
            }
            else
            {
                savedTextures[index] = 0;
            }
        }
    }

    CSF_AT(RenderCallback, atomic, 0x48)(atomic);

    for (int index = 0; index < materialCount; ++index)
    {
        uint32_t* material = CSF_AT(uint32_t**, geometry, 0x20)[index];
        if (material)
        {
            RpMaterialSetTexture(material, savedTextures[index]);
            CSF_AT(uint32_t, material, 0x04) = savedColors[index];
        }
    }

    CSF_AT(RenderCallback, atomic, 0x48) = savedCallback;
    if (!savedCallback)
    {
        CSF_AT(RenderCallback, atomic, 0x48) =
            reinterpret_cast<RenderCallback>(CSF_ADDR_FN(0x0058E680));
    }

    return atomicAddress;
}

HOOK(0x006F8090, GfxMaterials_RenderWithTexture, "auto");
