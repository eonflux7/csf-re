#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __cdecl, RwRasterGetNumLevels, 0x006276A0, (int a1));
CSF_FUNC(char, __cdecl, GfxSkin_ProbeEntryHandle, 0x006E2F10, (int a1));
CSF_FUNC(char, __cdecl, CheckObjectState, 0x006E2F30, (int object));

static int __stdcall GfxParticleRender_GetRasterStorageSize(uint32_t* entry)
{
    void* raster = reinterpret_cast<void*>(CSF_AT(uint32_t, entry, 0x0));
    uint32_t totalBytes = 0;
    if (!raster)
        return 0;

    const char compressed = CheckObjectState(reinterpret_cast<int>(entry));
    uint32_t height = CSF_AT(uint32_t, raster, 0x10);
    uint32_t width = CSF_AT(uint32_t, raster, 0xC);

    if (!compressed)
    {
        uint32_t levels = static_cast<uint32_t>(
            RwRasterGetNumLevels(reinterpret_cast<int>(raster)));
        if (levels != 0)
        {
            const uint32_t depth = CSF_AT(uint32_t, raster, 0x14);
            do
            {
                totalBytes += (width * height * depth) >> 3;
                width >>= 1;
                height >>= 1;
                --levels;
            } while (levels != 0);
        }
        return static_cast<int>(totalBytes);
    }

    uint32_t levels = static_cast<uint32_t>(
        RwRasterGetNumLevels(reinterpret_cast<int>(raster)));
    const unsigned blockSizeShift =
        (static_cast<unsigned char>(
             GfxSkin_ProbeEntryHandle(reinterpret_cast<int>(entry))) != 0) + 3;

    while (levels != 0)
    {
        uint32_t blockWidth = width >> 2;
        uint32_t blockHeight = height >> 2;
        if (blockWidth == 0)
            blockWidth = 1;
        if (blockHeight == 0)
            blockHeight = 1;

        width >>= 1;
        height >>= 1;
        totalBytes += (blockWidth * blockHeight) << blockSizeShift;
        --levels;
    }

    return static_cast<int>(totalBytes);
}

HOOK(0x00705A70, GfxParticleRender_GetRasterStorageSize, "auto");
