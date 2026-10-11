#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_GLOBAL(float, g_ColorBlack, 0x8A7DA0);
CSF_GLOBAL(float, g_ColorBlack_G, 0x8A7DA4);
CSF_GLOBAL(float, g_ColorBlack_B, 0x8A7DA8);
CSF_GLOBAL(float, g_ColorBlack_A, 0x8A7DAC);

// Zeroes the counters and flags of the object, sets its color to the
// black constant (RGBA), and returns 0.
static int __fastcall ResetColorAndCountersToBlack(void* self, void* /*edx*/)
{
    CSF_AT(int, self, 0x0C) = 0;
    CSF_AT(int, self, 0x10) = 0;
    CSF_AT(int, self, 0x14) = 0;
    CSF_AT(int, self, 0x18) = 0;
    CSF_AT(int, self, 0x30) = 0;
    CSF_AT(int, self, 0x1C) = 0;

    CSF_AT(float, self, 0x20) = g_ColorBlack;
    CSF_AT(float, self, 0x24) = g_ColorBlack_G;
    CSF_AT(float, self, 0x28) = g_ColorBlack_B;
    CSF_AT(float, self, 0x2C) = g_ColorBlack_A;

    CSF_AT(int, self, 0x64) = 0;
    CSF_AT(int, self, 0x04) = 0;
    CSF_AT(int, self, 0x08) = 0;
    return 0;
}

HOOK_KEEP_ECX(0x507730, ResetColorAndCountersToBlack, 0, "app_main");
