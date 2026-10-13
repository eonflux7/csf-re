#include <cstdint>
#include "hook.h"
#include "types/auto_classes.h"

// Returns nonzero while either field of the target record holds a real value
// (anything other than the -1 "no target" sentinel). Checks field_002c first and
// only reads field_0030 when field_002c is the sentinel, as the original does.
static int __fastcall S_004ad990_IsTargetRecordValid(S_004ad990* self, void* /*edx*/) {
    if (self->field_002c != 0xFFFFFFFFu) {
        return 1;
    }
    if (self->field_0030 != 0xFFFFFFFFu) {
        return 1;
    }
    return 0;
}

HOOK_KEEP_ECX(0x4B2290, S_004ad990_IsTargetRecordValid, 0, "ai_alert_states");
