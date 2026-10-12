#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Game heap free, used for the heap-allocated string buffer at +0x118.
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));

// Original callee at 0x75FE80: __fastcall, takes the embedded member at this+0x08.
CSF_FUNC(char, __fastcall, Member08_ResetSubobject, 0x0075FE80, (void* self, void* edx));

// Teardown for C_0086c2b4: releases the heap buffer if it is in use (capacity >= 16),
// resets the buffer to the empty state (capacity 15, length 0, short-buffer flag cleared),
// tears down the embedded member at +0x08, then restores the base-class vtable.
static int __fastcall C_0086c2b4_Destroy(C_0086c2b4* self, void* /*edx*/)
{
    int result;

    *(void***)self = CSF_VTBL(0x0086C2B4);

    if (self->field_012c >= 0x10 && self->field_0118)
    {
        Mem_FreeDebug(reinterpret_cast<void*>(self->field_0118));
    }

    self->field_012c = 15;
    self->field_0128 = 0;
    self->field_0118 &= 0xFFFFFF00u;   // LOBYTE(field_0118) = 0

    result = Member08_ResetSubobject(reinterpret_cast<void*>(reinterpret_cast<uintptr_t>(self) + 8), nullptr);

    *(void***)self = CSF_VTBL(0x008446CC);
    return result;
}

HOOK(0x0075FFE0, C_0086c2b4_Destroy, "auto");
