#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, RwImageDestroy, 0x00625F10, (uint32_t* a1));
CSF_FUNC(int, __cdecl, RwRasterClear, 0x006274B0, (int a1));
CSF_FUNC(int, __cdecl, RwRasterRenderFast, 0x00627510, (int a1, int a2, int a3));
CSF_FUNC(int, __cdecl, RwRasterPushContext, 0x006275B0, (int raster));
CSF_FUNC(int, __cdecl, RwRasterPopContext, 0x00627650, ());
CSF_FUNC(int, __cdecl, RwRasterShowRaster, 0x006276E0, (int a1, int a2, int a3));
CSF_FUNC(int, __cdecl, RwRasterCreate, 0x00627770, (int width, int height, int depth, int flags));
CSF_FUNC(int, __cdecl, RwRasterSetFromImage, 0x00633040, (int a1, uint8_t* a2));
CSF_FUNC(uint32_t*, __stdcall, GfxRenderEngine_LoadRgbImage, 0x006DBA70, (char* a1));

static int __fastcall GfxRenderEngine_LoadAndDisplayLoadingClock(
    uint32_t* self, void* /*edx*/)
{
    const char* clockFiles[4] = {
        CSF_STR(0x8613FC, "MENUS\\TEXTURES\\RELOJ.RAW"),
        CSF_STR(0x8613E0, "MENUS\\TEXTURES\\RELOJ1.RAW"),
        CSF_STR(0x8613C4, "MENUS\\TEXTURES\\RELOJ2.RAW"),
        CSF_STR(0x8613A8, "MENUS\\TEXTURES\\RELOJ3.RAW")
    };

    uint32_t* image =
        GfxRenderEngine_LoadRgbImage(const_cast<char*>(clockFiles[0]));
    int result = reinterpret_cast<int>(image);
    if (image)
    {
        int raster = RwRasterCreate(
            CSF_AT(int, image, 0x04), CSF_AT(int, image, 0x08), 0, 0);
        CSF_AT(int, self, 0x40) = raster;
        if (raster)
            RwRasterSetFromImage(raster, reinterpret_cast<uint8_t*>(image));
        result = RwImageDestroy(image);
    }

    int clockRaster = CSF_AT(int, self, 0x40);
    if (clockRaster)
    {
        int targetRaster = CSF_AT(int, self, 0x38);
        CSF_AT(int, self, 0x50) =
            8 * CSF_AT(int, reinterpret_cast<void*>(targetRaster), 0x0C) / 100;
        CSF_AT(int, self, 0x54) =
            92 * CSF_AT(int, reinterpret_cast<void*>(targetRaster), 0x10) / 100
            - CSF_AT(int, reinterpret_cast<void*>(clockRaster), 0x10);

        RwRasterPushContext(targetRaster);
        RwRasterClear(0);
        RwRasterRenderFast(
            clockRaster, CSF_AT(int, self, 0x50), CSF_AT(int, self, 0x54));
        RwRasterPopContext();
        RwRasterShowRaster(CSF_AT(int, self, 0x38), 0, 0);

        clockRaster = CSF_AT(int, self, 0x40);
        if (clockRaster)
        {
            RwRasterPushContext(CSF_AT(int, self, 0x38));
            RwRasterClear(0);
            RwRasterRenderFast(
                clockRaster, CSF_AT(int, self, 0x50), CSF_AT(int, self, 0x54));
            RwRasterPopContext();
        }
        RwRasterShowRaster(CSF_AT(int, self, 0x38), 0, 0);

        clockRaster = CSF_AT(int, self, 0x40);
        if (clockRaster)
        {
            RwRasterPushContext(CSF_AT(int, self, 0x38));
            RwRasterClear(0);
            RwRasterRenderFast(
                clockRaster, CSF_AT(int, self, 0x50), CSF_AT(int, self, 0x54));
            RwRasterPopContext();
        }
        RwRasterShowRaster(CSF_AT(int, self, 0x38), 0, 0);

        for (int frame = 1; frame < 4; ++frame)
        {
            image = GfxRenderEngine_LoadRgbImage(
                const_cast<char*>(clockFiles[frame]));
            result = reinterpret_cast<int>(image);
            if (image)
            {
                int raster = RwRasterCreate(
                    CSF_AT(int, image, 0x04), CSF_AT(int, image, 0x08), 0, 0);
                CSF_AT(int, self, 0x40 + frame * 4) = raster;
                if (raster)
                    RwRasterSetFromImage(
                        raster, reinterpret_cast<uint8_t*>(image));
                result = RwImageDestroy(image);
            }
        }
    }

    return result;
}

HOOK(0x006DBDB0, GfxRenderEngine_LoadAndDisplayLoadingClock, "auto");
