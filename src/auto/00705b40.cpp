#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __cdecl, RwImageRead, 0x006254C0, (int a1));
CSF_FUNC(int, __cdecl, RwImageDestroy, 0x00625F10, (uint32_t* a1));
CSF_FUNC(int, __cdecl, RwRasterDestroy, 0x00627570, (uint32_t raster));
CSF_FUNC(int, __cdecl, RwRasterCreate, 0x00627770, (int width, int height, int depth, int flags));
CSF_FUNC(int, __cdecl, RwTextureCreate, 0x00631780, (int a1));
CSF_FUNC(int, __cdecl, RwRasterSetFromImage, 0x00633040, (int a1, uint8_t* a2));
CSF_FUNC(int, __cdecl, RwImageFindRasterFormat, 0x00633070, (int a1, int a2, uint32_t* a3, uint32_t* a4, uint32_t* a5, int* a6));
CSF_FUNC(char, __cdecl, Path_ReplaceExtension, 0x006947D0, (char* Str, int a2, char* a3));
CSF_FUNC(void, __cdecl, GfxParticleRender_RegisterTexture, 0x006E3020, (int a1, char* Source, char a3, char a4));
CSF_FUNC(char, __fastcall, GfxParticleRender_ClassifyTextureAlpha, 0x00705830, (int16_t a1, int a2));

static int __stdcall GfxParticleRender_LoadTexture(
    char* source, char mipmapping, char* alphaType)
{
    if (!source || !*source)
        return 0;

    int texture = 0;
    *alphaType = 0;

    alignas(8) uint8_t pathBytes[0xC8];
    char* path = reinterpret_cast<char*>(pathBytes);
    for (int i = 0;; ++i)
    {
        path[i] = source[i];
        if (!path[i])
            break;
    }

    Path_ReplaceExtension(
        path, 200, reinterpret_cast<char*>(CSF_ADDR_DATA(0x0086103C)));
    int image = RwImageRead(reinterpret_cast<int>(path));
    if (!image)
        return 0;

    alignas(8) uint8_t mipImageBytes[0x28];
    alignas(8) uint8_t widthBytes[0x4];
    alignas(8) uint8_t heightBytes[0x4];
    alignas(8) uint8_t depthBytes[0x4];
    alignas(8) uint8_t formatBytes[0x8];

    uint32_t* mipImages = reinterpret_cast<uint32_t*>(mipImageBytes);
    uint32_t& width = *reinterpret_cast<uint32_t*>(widthBytes);
    uint32_t& height = *reinterpret_cast<uint32_t*>(heightBytes);
    uint32_t& depth = *reinterpret_cast<uint32_t*>(depthBytes);
    int& format = *reinterpret_cast<int*>(formatBytes);
    char& hasMipImages = *reinterpret_cast<char*>(formatBytes + 7);

    hook::Regs loadMipRegs{};
    loadMipRegs.edx = reinterpret_cast<uint32_t>(mipImageBytes);
    uint32_t loadMipArgs[] = {reinterpret_cast<uint32_t>(source), 10};
    hasMipImages = static_cast<char>(
        hook::call_regs(CSF_ADDR_FN(0x00705990), &loadMipRegs, loadMipArgs, 2));

    width = CSF_AT(uint32_t, reinterpret_cast<void*>(image), 0x4);
    height = CSF_AT(uint32_t, reinterpret_cast<void*>(image), 0x8);
    depth = CSF_AT(uint32_t, reinterpret_cast<void*>(image), 0xC);

    hook::Regs dimensionRegs{};
    dimensionRegs.eax = width;
    dimensionRegs.edx = height;
    dimensionRegs.ebx = depth;
    dimensionRegs.ecx = static_cast<uint8_t>(hasMipImages);
    dimensionRegs.esi = static_cast<uint32_t>(image);
    const bool validWidth = static_cast<uint8_t>(
        hook::call_regs(CSF_ADDR_FN(0x007057C0), &dimensionRegs, nullptr, 0)) != 0;

    if (validWidth)
    {
        dimensionRegs.eax = height;
        const bool validHeight = static_cast<uint8_t>(
            hook::call_regs(CSF_ADDR_FN(0x007057C0), &dimensionRegs, nullptr, 0)) != 0;

        if (validHeight)
        {
            format = 4;
            if (mipmapping)
            {
                format = 32772;
                if (!hasMipImages)
                    format = 36868;
            }

            image = RwImageFindRasterFormat(
                image, format, &width, &height, &depth, &format);
            if (image)
            {
                const int raster = RwRasterCreate(width, height, depth, format);
                if (raster)
                {
                    RwRasterSetFromImage(raster, reinterpret_cast<uint8_t*>(image));
                    if (mipmapping && hasMipImages)
                    {
                        hook::Regs setMipRegs{};
                        setMipRegs.ebx = static_cast<uint32_t>(raster);
                        setMipRegs.ecx = reinterpret_cast<uint32_t>(mipImageBytes);
                        uint32_t setMipArgs[] = {
                            reinterpret_cast<uint32_t>(mipImageBytes), 10
                        };
                        hook::call_regs(
                            CSF_ADDR_FN(0x00705A10), &setMipRegs, setMipArgs, 2);
                    }

                    texture = RwTextureCreate(raster);
                    if (texture)
                    {
                        CSF_AT(uint8_t, reinterpret_cast<void*>(texture), 0x50) =
                            static_cast<uint8_t>(2 * (mipmapping != 0) + 2);
                        *alphaType = GfxParticleRender_ClassifyTextureAlpha(
                            static_cast<int16_t>(format), static_cast<int>(depth));
                    }
                    else
                    {
                        RwRasterDestroy(static_cast<uint32_t>(raster));
                    }
                }
            }
        }
    }

    for (int i = 0; i < 10; ++i)
    {
        if (mipImages[i])
            RwImageDestroy(reinterpret_cast<uint32_t*>(mipImages[i]));
    }
    RwImageDestroy(reinterpret_cast<uint32_t*>(image));

    if (texture)
        GfxParticleRender_RegisterTexture(texture, source, *alphaType, 0);
    return texture;
}

HOOK(0x00705B40, GfxParticleRender_LoadTexture, "auto");
