#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(int, __fastcall, GlobalStateValue_50DCF0, 0x0050DCF0, (void* self, void* edx));

// Scales a percent threshold between field_0014 (base) and field_0018 (max)
// according to a2 relative to field_0008, then rolls it against a global-state value.
static int __fastcall RollScaledThresholdPercent(S_0048c6c0* self, void* /*edx*/, float a2)
{
    const uint32_t baseThreshold = self->field_0014;
    self->field_001c = baseThreshold;

    if (a2 < self->field_0008) {
        const int span = static_cast<int>(self->field_0018 - baseThreshold);
        if (span > 0) {
            const double ratio = (self->field_0008 - a2) / self->field_0008;
            const int64_t offset = static_cast<int64_t>(ratio * static_cast<double>(span));
            self->field_001c = baseThreshold + static_cast<uint32_t>(offset);
        }
    }

    const int globalState = GetGlobalState_50D190();
    const int stateValue = GlobalStateValue_50DCF0(
        reinterpret_cast<void*>(static_cast<uintptr_t>(globalState)), nullptr);
    const int32_t remainder = stateValue % 100;
    return static_cast<uint32_t>(remainder) < self->field_001c ? 1 : 0;
}

HOOK(0x0048C6C0, RollScaledThresholdPercent, "auto");
