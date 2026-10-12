#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __cdecl, RwImageRead, 0x006254C0, (int a1));
CSF_FUNC(int, __cdecl, RwImageDestroy, 0x00625F10, (uint32_t* a1));
CSF_FUNC(int, __cdecl, RwRasterCreate, 0x00627770, (int width, int height, int depth, int flags));
CSF_FUNC(int, __cdecl, RwRasterSetFromImage, 0x00633040, (int a1, uint8_t* a2));
CSF_FUNC(char, __cdecl, Path_ReplaceExtension, 0x006947D0, (char* Str, int a2, char* a3));

static int __stdcall GfxParticleRender_LoadRasterFromImage(char* filename)
{
    if (!filename || !*filename)
        return 0;

    alignas(8) uint8_t pathBytes[0xC8];
    char* path = reinterpret_cast<char*>(pathBytes);
    char* destination = path;
    const char* source = filename;
    char character;
    do
    {
        character = *source++;
        *destination++ = character;
    } while (character);

    Path_ReplaceExtension(
        path, 200, reinterpret_cast<char*>(CSF_ADDR_DATA(0x0086103C)));

    int raster = 0;
    const int imageAddress = RwImageRead(reinterpret_cast<int>(path));
    if (imageAddress)
    {
        uint8_t* image = reinterpret_cast<uint8_t*>(imageAddress);
        const int depth = CSF_AT(int, image, 0x0C);
        if (depth == 8 || depth == 32)
        {
            const int width = CSF_AT(int, image, 0x04);
            const int height = CSF_AT(int, image, 0x08);
            raster = RwRasterCreate(width, height, 0, 0);
            if (raster)
                RwRasterSetFromImage(raster, image);
        }
        RwImageDestroy(reinterpret_cast<uint32_t*>(image));
    }
    return raster;
}

HOOK(0x00705D10, GfxParticleRender_LoadRasterFromImage, "auto");
