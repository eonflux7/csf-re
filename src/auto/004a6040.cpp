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

// Forwarder to the FSM's current-state getter. Caller passes the FSM in ecx, no stack args.
CSF_FUNC(int, __fastcall, CharActionFsm_GetCurrentState, 0x4834F0, (void* self, void* edx));

// Returns a result code describing the char action state: 39 when the FSM is in state 10,
// 2 when the owner's flag bit 0x40 (at +0x2E0) is clear, otherwise 7.
static int __fastcall C_00834640_ResolveActionResultCode(C_00834640* self, void* /*edx*/) {
    uint8_t* owner = reinterpret_cast<uint8_t*>(self->field_0008);
    void* fsm = reinterpret_cast<void*>(CSF_AT(uint32_t, owner, 0x44));

    if (CharActionFsm_GetCurrentState(fsm, nullptr) == 10) {
        return 39;
    }
    if ((CSF_AT(uint8_t, owner, 0x2E0) & 0x40) == 0) {
        return 2;
    }
    return 7;
}

HOOK(0x4A6040, C_00834640_ResolveActionResultCode, "auto");
