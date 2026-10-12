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

// Grows the pointer array embedded at this+0x74 when full (capacity at +0x78, count at +0x7C).
CSF_FUNC(void*, __fastcall, S_0041e5f0_GrowArrayCapacity, 0x0041E5F0, (S_0041e5f0* self, void* /*edx*/, int additionalCapacity));

// Second step of the registration: called with the same light after it is appended.
CSF_FUNC(char, __fastcall, WorldLights_AttachLight, 0x006D1D30, (void* self, void* /*edx*/, void* a2));

static char __fastcall C_00860e68_AppendLightAndAttach(C_00860e68* self, void* /*edx*/, C_0085e7b8* a2)
{
    // Reference the growable-array vtable as the original's embedded sub-object does.
    (void)CSF_VTBL(0x0041E5F0);

    if (self->field_007c >= self->field_0078)
    {
        S_0041e5f0_GrowArrayCapacity(reinterpret_cast<S_0041e5f0*>(&self->field_0074), nullptr, (int)self->field_0080);
    }

    uint32_t index = self->field_007c;
    C_0085e7b8** slots = reinterpret_cast<C_0085e7b8**>(self->field_0074);
    slots[index] = a2;
    self->field_007c = index + 1;

    return WorldLights_AttachLight(self, nullptr, a2);
}

HOOK(0x006D20D0, C_00860e68_AppendLightAndAttach, "auto");
