#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Original callee at 0x410DA0: __fastcall, takes the embedded member at this+0x08.
CSF_FUNC(int, __fastcall, Member08_Release, 0x00410DA0, (void* self, void* edx));

// Game heap free, used for the heap-allocated string buffer at +0x238.
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));

// Teardown for C_00869124: releases the heap buffer if it is in use (capacity >= 16),
// resets the buffer to the empty state (capacity 15, length 0, short-buffer flag cleared),
// tears down the embedded member at +0x08, then restores the base-class vtable.
static int __fastcall C_00869124_Destroy(C_00869124* self, void* /*edx*/)
{
    int result;

    *(void***)self = CSF_VTBL(0x00869124);

    if (self->field_024c >= 0x10 && self->field_0238)
    {
        Mem_FreeDebug(reinterpret_cast<void*>(self->field_0238));
    }

    self->field_024c = 15;
    self->field_0248 = 0;
    self->field_0238 &= 0xFFFFFF00u;   // LOBYTE(field_0238) = 0

    result = Member08_Release(reinterpret_cast<void*>(reinterpret_cast<uintptr_t>(self) + 8), nullptr);

    *(void***)self = CSF_VTBL(0x008446CC);
    return result;
}

HOOK(0x00753AC0, C_00869124_Destroy, "auto");
