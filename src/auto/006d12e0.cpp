#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_GLOBAL(float, g_ColorBlack, 0x008A7DA0);
CSF_GLOBAL(float, g_ColorBlack_G, 0x008A7DA4);
CSF_GLOBAL(float, g_ColorBlack_B, 0x008A7DA8);
CSF_GLOBAL(float, g_ColorBlack_A, 0x008A7DAC);

// Stores a 4-float RGBA color into the object (at +0x1C..+0x28) and sets the
// flag at +0x1A to 1 when the color is not equal to the black constant, else 0.
// Returns the bit pattern of the blue component as the original does.
static int __fastcall C_00860e68_SetColorAndNonBlackFlag(C_00860e68* self, void* /*edx*/, float* a2)
{
    const uint32_t* colorWords = reinterpret_cast<const uint32_t*>(a2);

    bool isBlack = g_ColorBlack == a2[0] && g_ColorBlack_G == a2[1] &&
                   g_ColorBlack_B == a2[2] && g_ColorBlack_A == a2[3];
    self->field_001a = isBlack ? 0 : 1;

    CSF_AT(uint32_t, self, 0x1C) = colorWords[0];
    CSF_AT(uint32_t, self, 0x20) = colorWords[1];
    CSF_AT(uint32_t, self, 0x24) = colorWords[2];
    CSF_AT(uint32_t, self, 0x28) = colorWords[3];

    return static_cast<int>(colorWords[2]);
}

HOOK(0x006D12E0, C_00860e68_SetColorAndNonBlackFlag, "auto");
