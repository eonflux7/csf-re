#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// 0x4834F0 is a forwarder with no stack arguments: ecx = state object, returns state id in eax.
CSF_FUNC(int, __fastcall, CharActionFsm_GetCurrentState, 0x4834F0, (void* self, void* edx));

// Returns a result code for the character action state machine owned by this object.
// 33 when flag 0x10000 is set at +0x2E0; otherwise 8/9/40/41 depending on whether the
// FSM is in state 10, and whether the byte/dword at +0x1C4 is non-zero.
static int __fastcall C_00834640_GetActionResultCode(C_00834640* self, void* /*edx*/)
{
    uint8_t* actionObj = reinterpret_cast<uint8_t*>(self->field_0008);

    void* fsmObj = reinterpret_cast<void*>(CSF_AT(uint32_t, actionObj, 0x44));
    bool isStateTen = CharActionFsm_GetCurrentState(fsmObj, nullptr) == 10;

    uint32_t flags = CSF_AT(uint32_t, actionObj, 0x2E0);
    if ((flags & 0x10000) != 0)
        return 33;

    if (CSF_AT(uint32_t, actionObj, 0x1C4) != 0)
        return isStateTen ? 41 : 9;

    return isStateTen ? 40 : 8;
}

HOOK(0x004A5FD0, C_00834640_GetActionResultCode, "auto");
