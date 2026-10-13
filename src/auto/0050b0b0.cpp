#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Original 0x0050A2E0: S_0050a2e0 default-flags initializer (__fastcall, no stack args).
CSF_FUNC(char, __fastcall, S_0050a2e0_InitDefaultFlags, 0x0050A2E0, (S_0050a2e0* self, void*));

// 0x3EB33333 is the IEEE-754 bit pattern of 0.35f, stored as raw dwords in the original.
static constexpr uint32_t kDefaultThresholdBits = 0x3EB33333u;

// __thiscall S_0050a2e0::ApplyDefaultValues: resets the default flags, then sets two
// threshold dwords (+0x0C, +0x08) and an enable byte (+0x10). Returns the threshold bits.
static int __fastcall S_0050a2e0_ApplyDefaultValues(S_0050a2e0* self, void* /*edx*/) {
    S_0050a2e0_InitDefaultFlags(self, nullptr);
    self->field_000c = kDefaultThresholdBits;
    self->field_0008 = kDefaultThresholdBits;
    self->field_0010 = 1;
    return static_cast<int>(kDefaultThresholdBits);
}

HOOK(0x0050B0B0, S_0050a2e0_ApplyDefaultValues, "auto");
