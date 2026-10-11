#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Returns 77 when the flag at +0x1C4 of the state sub-object is set. Otherwise returns a
// code from the selector at +0x1D8: 0/1 -> 12, 2/3 -> 43, anything else -> this.
static int __fastcall C_00834598_GetStateResultCode(C_00834598* self, void* /*edx*/)
{
    C_00833908* stateObject = reinterpret_cast<C_00833908*>(self->field_0008);

    if (CSF_AT(uint32_t, stateObject, 0x1C4) != 0)
        return 77;

    uint32_t selector = CSF_AT(uint32_t, stateObject, 0x1D8);
    switch (selector)
    {
    case 0u:
    case 1u:
        return 12;
    case 2u:
    case 3u:
        return 43;
    default:
        return static_cast<int>(reinterpret_cast<std::uintptr_t>(self));
    }
}

HOOK(0x004A46A0, C_00834598_GetStateResultCode, "auto");
