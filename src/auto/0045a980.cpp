#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_GLOBAL(float, g_ColorBlack,   0x008A7DA0);
CSF_GLOBAL(float, g_ColorBlack_G, 0x008A7DA4);
CSF_GLOBAL(float, g_ColorBlack_B, 0x008A7DA8);
CSF_GLOBAL(float, g_ColorBlack_A, 0x008A7DAC);

static int __fastcall HudStatusIcons_InitializeColors(char* self, void* /*edx*/)
{
    CSF_AT(std::uint32_t, self, 0x00) = 0;
    CSF_AT(std::uint32_t, self, 0x04) = 0;

    CSF_AT(float, self, 0x08) = g_ColorBlack;
    CSF_AT(float, self, 0x0C) = g_ColorBlack_G;
    CSF_AT(float, self, 0x10) = g_ColorBlack_B;
    CSF_AT(float, self, 0x14) = g_ColorBlack_A;

    CSF_AT(float, self, 0x18) = g_ColorBlack;
    CSF_AT(float, self, 0x1C) = g_ColorBlack_G;
    CSF_AT(float, self, 0x20) = g_ColorBlack_B;
    const float secondAlpha = g_ColorBlack_A;
    CSF_AT(std::uint32_t, self, 0x28) = 0;
    CSF_AT(float, self, 0x24) = secondAlpha;

    CSF_AT(float, self, 0x2C) = g_ColorBlack;
    CSF_AT(float, self, 0x30) = g_ColorBlack_G;
    CSF_AT(float, self, 0x34) = g_ColorBlack_B;
    CSF_AT(float, self, 0x38) = g_ColorBlack_A;

    CSF_AT(float, self, 0x3C) = g_ColorBlack;
    CSF_AT(float, self, 0x40) = g_ColorBlack_G;
    const float lastBlue = g_ColorBlack_B;
    CSF_AT(float, self, 0x44) = lastBlue;
    CSF_AT(float, self, 0x48) = g_ColorBlack_A;

    int result;
    std::memcpy(&result, &lastBlue, sizeof(result));
    return result;
}

HOOK(0x0045A980, HudStatusIcons_InitializeColors, "auto");
