#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, sub_6E8A50, 0x006E8A50, (void* self, void* edx, int a2));

// Forwards to sub_6E8A50 when the pointer at +0x1C is set; otherwise returns 0.
// The original reaches sub_6E8A50 by a tail jump, so the callee's result is returned unchanged.
static int __fastcall C_00861e98_ForwardWhenFieldSet(C_00861e98* self, void* /*edx*/, int a2)
{
    if (CSF_AT(uint32_t, self, 0x1C) != 0)
        return sub_6E8A50(self, nullptr, a2);
    return 0;
}

HOOK(0x006E9860, C_00861e98_ForwardWhenFieldSet, "auto");
