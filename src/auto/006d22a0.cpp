#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Per-light detach helper (__thiscall, one stack arg, ret 4).
CSF_FUNC(char, __fastcall, sub_6D1D30, 0x006D1D30, (void* self, void* edx, void* a2));

// Walks the light pointer array of this object and detaches each entry
// through sub_6D1D30. Returns the (re-read) light count, or 0 when the
// gating flag at +0x90 is clear.
static unsigned int __fastcall C_00860e68_DetachAllLights(C_00860e68* self, void* /*edx*/)
{
    unsigned int result = self->field_0090;
    if (result)
    {
        result = self->field_007c;
        for (unsigned int i = 0; i < result; ++i)
        {
            C_0085e7b8* light = reinterpret_cast<C_0085e7b8**>(self->field_0074)[i];
            sub_6D1D30(self, nullptr, light);
            result = self->field_007c;
        }
    }
    return result;
}

HOOK(0x006D22A0, C_00860e68_DetachAllLights, "auto");
