#include <cstdint>
#include "globals.h"
#include <cstddef>
#include "hook.h"
#include "types/auto_classes.h"

// Forwarder to the FSM's current-state getter (0x4834F0). The original calls it with ecx = fsm and no stack args.
CSF_FUNC(int, __fastcall, CharActionFsm_GetCurrentState, 0x4834F0, (void* self, void* edx));

// Returns 35 when the FSM's current state differs from 10, otherwise 51.
static int __fastcall C_00834640_SelectActionCodeByFsmState(C_00834640* self, void* /*edx*/) {
    void* fsm = reinterpret_cast<void*>(reinterpret_cast<C_008344f0*>(self->field_0008)->field_0044);
    int currentState = CharActionFsm_GetCurrentState(fsm, nullptr);
    return currentState != 10 ? 35 : 51;
}

HOOK(0x4A61F0, C_00834640_SelectActionCodeByFsmState, "auto");
