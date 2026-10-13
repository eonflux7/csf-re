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

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);
CSF_GLOBAL(float, g_MaxPercent, 0x0082E120);
CSF_GLOBAL(float, g_DefaultTransitionValue, 0x00833C54);

// __thiscall int C_00833b88::ApplyTransitionValue(float value)
// Non-positive input: returns the stored flag at +0x1C4 and sets +0x1D4 to
// 100.0 when that flag is nonzero, otherwise to the default 170.0.
// Positive input: stores it at +0x1D4 and returns its raw bit pattern.
static int __fastcall C_00833b88_ApplyTransitionValue(C_00833b88* self, void* /*edx*/, float value)
{
    if (value <= g_fZero)
    {
        int flagValue = CSF_AT(int, self, 0x1C4);
        if (flagValue != 0)
            CSF_AT(float, self, 0x1D4) = g_MaxPercent;
        else
            CSF_AT(float, self, 0x1D4) = g_DefaultTransitionValue;
        return flagValue;
    }

    CSF_AT(float, self, 0x1D4) = value;
    uint32_t valueBits = 0;
    std::memcpy(&valueBits, &value, sizeof(valueBits));
    return static_cast<int>(valueBits);
}

HOOK(0x00492500, C_00833b88_ApplyTransitionValue, "auto");
