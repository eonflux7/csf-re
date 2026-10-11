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

CSF_FUNC(double, __fastcall, GetTimerRemainingSeconds, 0x0048E510, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, TCharTask_HandleFallback, 0x004DB440, (void* self, void* edx, int a2));
CSF_GLOBAL(float, g_fZero, 0x0082D4CC);

static int __fastcall C_00834fa8_UpdateByDelayTimer(C_00834fa8* self, void* /*edx*/, int /*a2*/) {
    // Timer owner is the object at [this->field_0008 + 0x14]
    void* statesManager = reinterpret_cast<void*>(CSF_AT(uint32_t, self, 0x08));
    void* timerOwner = reinterpret_cast<void*>(CSF_AT(uint32_t, statesManager, 0x14));
    double remaining = GetTimerRemainingSeconds(timerOwner, nullptr, 11);

    // Unordered (NaN) also takes the fallback branch, matching the fcomp/test ah,0x41 flags.
    if (!(remaining > g_fZero)) {
        return TCharTask_HandleFallback(self, nullptr,
                                        static_cast<int>(CSF_AT(uint32_t, self, 0x0C) + 0x54));
    }

    CSF_AT(uint8_t, self, 0x14) = 1;

    // Re-read this->field_0008 and its +0x14 pointer after the callee, as the original does.
    void* statesManagerAfter = reinterpret_cast<void*>(CSF_AT(uint32_t, self, 0x08));
    void* target = reinterpret_cast<void*>(CSF_AT(uint32_t, statesManagerAfter, 0x14));
    void** targetVtbl = CSF_AT(void**, target, 0);
    return reinterpret_cast<int(__fastcall*)(void*, void*, int, int, int, int)>(targetVtbl[35])(
        target, nullptr, 11, 0, -1, 0);
}

HOOK(0x4211D0, C_00834fa8_UpdateByDelayTimer, "auto");
