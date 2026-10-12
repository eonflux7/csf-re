#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Original 0x006B31E0: validates the object's atomic/skin data (sub_6B31E0).
CSF_FUNC(char, __fastcall, ProbeObjectSkinSupport, 0x006B31E0, (void* self, void* edx));

// Original 0x006B3820 (__thiscall, ret 0).
// Returns whether the object is supported for skinning. Also clears the
// 0x20000 "pending" flag and resets the float at +0x78 to -1.0f when it was set.
static bool __fastcall C_0085e7b8_IsGfxSkinSupported(C_0085e7b8* self, void* /*edx*/)
{
    bool result;
    if ((CSF_AT(uint32_t, self, 0x64) & 0x140) == 0x140 &&
        ProbeObjectSkinSupport(self, nullptr) != 0)
    {
        result = true;
    }
    else
    {
        result = (CSF_AT(uint8_t, self, 0x64) & 8) != 0;
    }

    uint32_t flags = CSF_AT(uint32_t, self, 0x64);
    if ((flags & 0x20000) != 0)
    {
        CSF_AT(float, self, 0x78) = -1.0f;
        CSF_AT(uint32_t, self, 0x64) = flags & 0xFFFDFFFF;
    }
    return result;
}

HOOK(0x006B3820, C_0085e7b8_IsGfxSkinSupported, "auto");
