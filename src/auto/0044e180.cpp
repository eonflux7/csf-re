#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Callee at 0x44DA20 (called when the accumulated timer passes the threshold).
CSF_FUNC(void, __fastcall, S_0044da20_OnTimerExpired, 0x0044DA20, (void* self, void* edx));

CSF_GLOBAL(float, g_MessageBoxAnimationScale, 0x0082D7C0);

// Low 16 bits of the FPU status word the original leaves in ax after `fcomp` (x87 C3/C2/C0 bits).
static uint32_t S_0044da20_FpuCompareStatus(float value, float threshold)
{
    if (value < threshold) {
        return 0x0100u;  // C0 set
    }
    if (value == threshold) {
        return 0x4000u;  // C3 set
    }
    if (value > threshold) {
        return 0x0000u;  // greater: C0 and C3 clear
    }
    return 0x4500u;      // unordered: C3, C2, C0 set
}

// __thiscall(this, float a2) with ret 4 -> __fastcall wrapper, a2 is the only stack argument.
// Returns the value the original leaves in eax (the fnstsw result in ax).
static int __fastcall S_0044da20_UpdateLinkedController(S_0044da20* self, void* /*edx*/, float a2)
{
    // The original also references the vtable at 0x44DA20 (no store is performed here).
    static_cast<void>(CSF_VTBL(0x0044DA20));

    self->field_0004 = a2 + self->field_0004;
    self->field_0008 = a2 + self->field_0008;

    // Original compares the running value against the float global, not a literal 10.0.
    uint32_t statusWord = S_0044da20_FpuCompareStatus(self->field_0008, g_MessageBoxAnimationScale);
    if (statusWord == 0x0000u) {
        S_0044da20_OnTimerExpired(self, nullptr);
        self->field_0008 = 0.0f;
    }
    return static_cast<int>(statusWord);
}

HOOK(0x0044E180, S_0044da20_UpdateLinkedController, "auto");
