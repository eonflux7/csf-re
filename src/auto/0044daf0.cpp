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

// Vtable of S_0044d940, referenced by the original (call 0x44DB1C site).
[[maybe_unused]] static const auto* s_S0044d940Vtable = CSF_VTBL(0x0044D940);

// Scoreboard refresh interval (float), subtracted from the object's field_0004.
CSF_GLOBAL(float, g_ScoreboardRefreshInterval, 0x0082DAF0);

// Helper computing the upper limit for the entry time (original sub_44D940, __thiscall with 3 stack args).
CSF_FUNC(double, __fastcall, S_0044d940_ComputeWindowLimit, 0x0044D940,
         (void* self, void* edx, int arg0, int a2, int a4));

static bool __fastcall S_0044d940_IsWithinRefreshWindow(S_0044d940* self, void* /*edx*/,
                                                        int a2, int a3, int a4) {
    // Entry timestamp stored at a2+4 (float).
    float entryTime = *reinterpret_cast<float*>(static_cast<uintptr_t>(a2) + 4);

    if (entryTime < self->field_0004 - g_ScoreboardRefreshInterval)
        return false;

    double windowLimit = S_0044d940_ComputeWindowLimit(self, nullptr, a2, a3, a4);
    bool result = true;
    if (windowLimit > entryTime)
        return false;
    return result;
}

HOOK(0x0044DAF0, S_0044d940_IsWithinRefreshWindow, "auto");
