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

// Callee bound by address (bind-as line from packet).
CSF_FUNC(void, __cdecl, sub_7DF700, 0x007DF700, (int a1, int a2));

// Reads the object referenced by this->field_0004; if its +0x50 slot is
// nonzero, calls the audio routine on it and clears this->field_000c.
static int __fastcall C_00863dd0_ReleaseInnerIfActive(C_00863dd0* self, void* /*edx*/)
{
    uint8_t* inner = reinterpret_cast<uint8_t*>(self->field_0004);
    int result = static_cast<int>(reinterpret_cast<uintptr_t>(inner));

    if (CSF_AT(uint32_t, inner, 0x50) != 0)
    {
        sub_7DF700(static_cast<int>(reinterpret_cast<uintptr_t>(inner)), 0);
        self->field_000c = 0;
    }
    return result;
}

HOOK(0x007123E0, C_00863dd0_ReleaseInnerIfActive, "auto");
