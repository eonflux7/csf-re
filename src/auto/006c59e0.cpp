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

// Resets the per-skin counters/pointers at +0xF8..+0x120 to zero and sets the
// "dirty" flag byte at +0xF4 to 1. Returns 0.
static int __fastcall GfxSkin_ResetSkinState(void* self, void* /*edx*/)
{
    CSF_AT(int, self, 0xF8) = 0;
    CSF_AT(int, self, 0xFC) = 0;
    CSF_AT(int, self, 0x100) = 0;
    CSF_AT(int, self, 0x104) = 0;
    CSF_AT(int, self, 0x108) = 0;
    CSF_AT(int, self, 0x10C) = 0;
    CSF_AT(int, self, 0x114) = 0;
    CSF_AT(int, self, 0x118) = 0;
    CSF_AT(int, self, 0x11C) = 0;
    CSF_AT(int, self, 0x120) = 0;
    CSF_AT(int, self, 0x110) = 0;
    CSF_AT(uint8_t, self, 0xF4) = 1;
    return 0;
}

HOOK(0x006C59E0, GfxSkin_ResetSkinState, "auto");
