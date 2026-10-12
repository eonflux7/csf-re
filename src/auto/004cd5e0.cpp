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

// Forwards a code-25 request to the child object at +0x14 (vtable slot 0x90 / 4 = 36).
// Returns the forwarded call's result; otherwise returns the request pointer.
static int __fastcall C_0083ab58_ForwardRequestIfCode25(C_0083ab58* self, void* /*edx*/, uint32_t* a2)
{
    int result = static_cast<int>(reinterpret_cast<intptr_t>(a2));
    if (*a2 == 25)
    {
        self->field_0018 |= 1u;

        void* target = reinterpret_cast<void*>(self->field_0014);
        using ForwardFn = int(__fastcall*)(void*, void*, uint32_t, uint32_t, int, int);
        ForwardFn forward = reinterpret_cast<ForwardFn>(CSF_AT(void**, target, 0)[36]);
        result = forward(target, nullptr, a2[1], a2[2], -1, 0);

        self->field_0040 = 1;
    }
    return result;
}

HOOK(0x4CD5E0, C_0083ab58_ForwardRequestIfCode25, "auto");
