#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, RwImageGetPath, 0x00625320, ());
CSF_FUNC(int, __cdecl, RwRasterDestroy, 0x00627570, (uint32_t raster));
CSF_FUNC(int, __cdecl, RwStreamClose, 0x00629810, (uint32_t* a1, uint32_t* a2));
CSF_FUNC(uint32_t*, __cdecl, RwStreamOpen, 0x006298E0, (int a3, int a4, int a5));
CSF_FUNC(int, __cdecl, RwTextureCreate, 0x00631780, (int a1));
CSF_FUNC(int, __cdecl, RwD3D9RasterStreamReadDDS, 0x0063A4D0, (uint32_t* arg0));
CSF_FUNC(char, __cdecl, Path_ReplaceExtension, 0x006947D0, (char* Str, int a2, char* a3));
CSF_FUNC(void, __cdecl, GfxParticleRender_RegisterTexture, 0x006E3020,
         (int a1, char* Source, char a3, char a4));
CSF_FUNC(char, __fastcall, GfxParticleRender_GetRasterTextureMode, 0x00705830,
         (int16_t a1, int a2));

static int __stdcall GfxParticleRender_LoadDDSTexture(char* source, char* textureMode)
{
    if (!source || !*source)
        return 0;

    int texture = 0;
    *textureMode = 0;

    alignas(8) uint8_t pathBytes[0x1F8];
    char* ddsPath = reinterpret_cast<char*>(pathBytes);
    char* imagePath = reinterpret_cast<char*>(pathBytes + 0xFC);

    std::strcpy(imagePath, reinterpret_cast<const char*>(RwImageGetPath()));

    const std::size_t sourceBytes = std::strlen(source) + 1;
    char* appendPosition = imagePath;
    while (*appendPosition)
        ++appendPosition;
    std::memcpy(appendPosition, source, sourceBytes);

    std::strcpy(ddsPath, imagePath);
    Path_ReplaceExtension(ddsPath, 250, const_cast<char*>(CSF_STR(0x00857EBC, ".dds")));

    uint32_t* stream = RwStreamOpen(2, 1, reinterpret_cast<int>(ddsPath));
    if (stream)
    {
        const int raster = RwD3D9RasterStreamReadDDS(stream);
        RwStreamClose(stream, nullptr);

        if (raster)
        {
            texture = RwTextureCreate(raster);
            if (texture)
            {
                const int16_t rasterFlags = static_cast<int16_t>(
                    static_cast<uint16_t>(
                        CSF_AT(uint8_t, reinterpret_cast<void*>(raster), 0x23)) << 8);

                CSF_AT(uint8_t, reinterpret_cast<void*>(texture), 0x50) =
                    rasterFlags < 0 ? 4 : 2;

                *textureMode = GfxParticleRender_GetRasterTextureMode(
                    rasterFlags,
                    CSF_AT(int, reinterpret_cast<void*>(raster), 0x14));
            }
            else
            {
                RwRasterDestroy(static_cast<uint32_t>(raster));
            }

            if (texture)
                GfxParticleRender_RegisterTexture(texture, source, *textureMode, 1);
        }
    }

    return texture;
}

HOOK(0x00705DD0, GfxParticleRender_LoadDDSTexture, "auto");
