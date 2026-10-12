#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Resets the per-order combat state block of S_004ad990 to its default values.
// Original: 0x004B8C80, __thiscall, ret 0, returns 0.
static int __fastcall S_004ad990_ResetCombatOrderFields(S_004ad990* self, void* /*edx*/) {
    self->field_0028 = static_cast<uint32_t>(-1);
    self->field_002c = static_cast<uint32_t>(-1);
    self->field_0030 = static_cast<uint32_t>(-1);
    self->field_0038 = static_cast<uint32_t>(-1);
    self->field_003c = static_cast<uint32_t>(-1);
    self->field_0050 = static_cast<uint32_t>(-1);
    self->field_0054 = static_cast<uint32_t>(-1);
    self->field_005c = 0;
    self->field_0060 = 1065353216u;
    self->field_009c = 3.4028235e38f;
    self->field_0094 = 0.0f;
    self->field_00a8 = 0;
    self->field_00a9 = 0;
    self->field_00a0 = 0.0f;
    self->field_00a4 = 0;
    return 0;
}

HOOK(0x4B8C80, S_004ad990_ResetCombatOrderFields, "auto");
