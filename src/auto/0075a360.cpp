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

// Callee at 0x004DE9B0: __thiscall-style (ecx = this), one stack arg, ret 4.
CSF_FUNC(void*, __fastcall, sub_4DE9B0, 0x004DE9B0, (void* self, void* edx, int a2));

// Constructor for C_0086c12c: installs the vtable, zeroes the three counters,
// sets the limit to 10, and calls sub_4DE9B0 with the remaining count if it is below 10.
static uint32_t* __fastcall C_0086c12c_Construct(C_0086c12c* self, void* /*edx*/)
{
    // Base-class part (C_0082d410) is not written separately here; the vtable store
    // matches the original's first store into [this+0].
    *(void***)self = CSF_VTBL(0x0086C12C);

    C_0086c12c_field_0004_ptr:;
    self->field_0004 = 0;
    self->field_0008 = 0;
    CSF_AT(uint32_t, self, 0x0C) = 0;
    CSF_AT(uint32_t, self, 0x10) = 10;

    const uint32_t initialCount = 0;
    if (initialCount < 10u)
    {
        int remaining = 10 - (int)initialCount;
        sub_4DE9B0((void*)((uint8_t*)self + 4), nullptr, remaining);
    }

    return reinterpret_cast<uint32_t*>(self);
}

HOOK(0x0075A360, C_0086c12c_Construct, "auto");
