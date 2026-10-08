#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

// Raw dword copies of the four float constants. The original copies their bit
// patterns with mov, so they are bound as uint32_t to avoid float conversion.
CSF_GLOBAL(uint32_t, g_ColorBlack, 0x008A7DA0);
CSF_GLOBAL(uint32_t, g_ColorBlack_G, 0x008A7DA4);
CSF_GLOBAL(uint32_t, g_ColorBlack_B, 0x008A7DA8);
CSF_GLOBAL(uint32_t, g_ColorBlack_A, 0x008A7DAC);

// Resets a menu server-info style record to its default state:
// opaque white alpha at +0x14, zeroed color/flag fields, and black color
// copied into the RGBA slots at +0x08..+0x14.
static uint32_t* __fastcall MpServerInfo_InitDefaultStyle(uint32_t* self, void* /*edx*/)
{
    CSF_AT(uint32_t, self, 0x14) = 0x3F800000u;
    CSF_AT(uint32_t, self, 0x08) = 0;
    CSF_AT(uint32_t, self, 0x0C) = 0;
    CSF_AT(uint32_t, self, 0x10) = 0;
    CSF_AT(uint32_t, self, 0x30) = 7;
    CSF_AT(uint32_t, self, 0x2C) = 0;
    CSF_AT(uint16_t, self, 0x1C) = 0;
    CSF_AT(uint32_t, self, 0x34) = 0;
    CSF_AT(uint32_t, self, 0x38) = 0;
    CSF_AT(uint32_t, self, 0x00) = 1;
    CSF_AT(uint32_t, self, 0x04) = 1;

    CSF_AT(uint32_t, self, 0x08) = g_ColorBlack;
    CSF_AT(uint32_t, self, 0x0C) = g_ColorBlack_G;
    CSF_AT(uint32_t, self, 0x10) = g_ColorBlack_B;
    CSF_AT(uint32_t, self, 0x14) = g_ColorBlack_A;
    return self;
}

HOOK(0x530AB0, MpServerInfo_InitDefaultStyle, "auto");
