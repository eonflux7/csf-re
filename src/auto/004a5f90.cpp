#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(int, __fastcall, CharActionFsm_GetCurrentState, 0x4834F0, (void* self, void* edx));

// Maps the character's action FSM state and status flags to an animation/action code.
// Flag 0x10000 wins outright (returns 32). Flag 4 returns 38 when the FSM is in state 10, else 1.
// Otherwise returns 0x25 when the FSM is in state 10, else 0.
static int __fastcall C_00834640_GetAnimationCodeFromState(C_00834640* self, void* /*edx*/)
{
    C_00833908* actionOwner = reinterpret_cast<C_00833908*>(self->field_0008);
    int currentFsmState = CharActionFsm_GetCurrentState(
        reinterpret_cast<void*>(CSF_AT(uint32_t, actionOwner, 0x44)), nullptr);
    uint32_t stateFlags = CSF_AT(uint32_t, actionOwner, 0x2E0);
    bool isStateTen = (currentFsmState == 10);

    if ((stateFlags & 0x10000) != 0)
        return 32;
    if ((stateFlags & 4) != 0)
        return isStateTen ? 38 : 1;
    return isStateTen ? 0x25 : 0;
}

HOOK(0x4A5F90, C_00834640_GetAnimationCodeFromState, "auto");
