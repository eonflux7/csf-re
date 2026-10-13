#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, RwRasterUnlock, 0x00627490, (int a1));
CSF_FUNC(int, __cdecl, RwRasterLock, 0x00627810, (int a1, int a2, int a3));
CSF_FUNC(int, __fastcall, ApplyEffectPass, 0x00708DE0, (void* self, void* edx, int a2, char a3));
CSF_FUNC(int, __stdcall, ScreenEffects_PrepareLayer, 0x00709360, (int a1));

static int __fastcall C_00862b58_ApplyGrayscalePalette(
    C_00862b58* self, void* /*edx*/)
{
    const int raster = static_cast<int>(self->field_0060);
    self->field_0030 = 0;
    ScreenEffects_PrepareLayer(raster);

    auto* pixel = reinterpret_cast<uint8_t*>(
        RwRasterLock(static_cast<int>(self->field_0060), 0, 3));

    if (pixel)
    {
        const int rowPadding =
            CSF_AT(int, reinterpret_cast<void*>(self->field_0060), 0x18) - 2048;

        for (int row = 0; row < 512; ++row)
        {
            for (int group = 0; group < 128; ++group)
            {
                for (int componentPixel = 0; componentPixel < 4; ++componentPixel)
                {
                    const unsigned intensity =
                        (static_cast<unsigned>(pixel[0]) + pixel[1] + pixel[2]) / 3;
                    CSF_AT(uint32_t, pixel, 0) =
                        CSF_AT(uint32_t, self, 0x64 + intensity * 4);
                    pixel += 4;
                }
            }
            pixel += rowPadding;
        }
    }

    RwRasterUnlock(static_cast<int>(self->field_0060));
    return ApplyEffectPass(
        self, nullptr, static_cast<int>(self->field_0060), static_cast<char>(255));
}

HOOK(0x00709430, C_00862b58_ApplyGrayscalePalette, "auto");
