#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, RwRGBAToPixel, 0x00625870, (int a1, int a2));
CSF_FUNC(int, __cdecl, RwRasterClear, 0x006274B0, (int a1));
CSF_FUNC(int, __cdecl, RwRasterPushContext, 0x006275B0, (int raster));
CSF_FUNC(int, __cdecl, RwRasterPopContext, 0x00627650, ());

// Clears the render raster three times with the color derived from the raster's
// byte at +0x23, presenting after each clear. Returns the last vSlot2C result,
// or the raster pointer value (0) if there is no raster.
static int __fastcall CRenderEngine_ClearRasterThrice(CRenderEngine* self, void* /*edx*/)
{
    int result = static_cast<int>(self->field_0038);
    if (result)
    {
        uint8_t pixelBytes[4];
        pixelBytes[0] = 0;
        pixelBytes[1] = 0;
        pixelBytes[2] = 0;
        pixelBytes[3] = 0;

        uint8_t* rasterBytes = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(self->field_0038));
        uint32_t highByte = CSF_AT(uint8_t, rasterBytes, 0x23);
        int colorWord = static_cast<int>(highByte << 8);
        int pixelValue = RwRGBAToPixel(reinterpret_cast<int>(pixelBytes), colorWord);

        int remaining = 3;
        do
        {
            RwRasterPushContext(static_cast<int>(self->field_0038));
            RwRasterClear(pixelValue);
            RwRasterPopContext();
            result = self->vSlot2C();
            --remaining;
        } while (remaining != 0);
    }
    return result;
}

HOOK(0x6DB850, CRenderEngine_ClearRasterThrice, "auto");
