#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Callee at 0x004DB440: takes `this` in ecx and one stack argument (ret 4).
CSF_FUNC(int, __fastcall, TaskStates_Dispatch, 0x004DB440, (void* self, void* edx, int a2));

static int __fastcall C_00834fa8_DispatchStateBySubobjectFlag(C_00834fa8* self, void* /*edx*/)
{
    // Sub-object pointer stored at +0x0C of this task.
    uintptr_t subObjectAddress = CSF_AT(uint32_t, self, 0x0C);
    uint8_t* subObject = reinterpret_cast<uint8_t*>(subObjectAddress);

    // Flag byte at sub-object +0x50 selects which embedded sub-object is dispatched.
    if (CSF_AT(uint8_t, subObject, 0x50) != 0)
    {
        int embeddedAt28 = static_cast<int>(reinterpret_cast<uintptr_t>(subObject + 0x28));
        return TaskStates_Dispatch(self, nullptr, embeddedAt28);
    }

    int embeddedAt14 = static_cast<int>(reinterpret_cast<uintptr_t>(subObject + 0x14));
    return TaskStates_Dispatch(self, nullptr, embeddedAt14);
}

HOOK(0x004212C0, C_00834fa8_DispatchStateBySubobjectFlag, "auto");
