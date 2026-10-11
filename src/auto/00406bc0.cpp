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

// Game heap free (cdecl, caller pops the argument).
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

// Releases the two owned buffers at +0x1C and +0x0C, then leaves the object
// in the C_0082d410 base state. Returns the Mem_Free result when the +0x0C
// buffer was freed, otherwise 0.
static int __fastcall C_0082d77c_ReleaseOwnedBuffersAndRevertToBase(C_0082d77c* self, void* /*edx*/) {
    *(void***)self = CSF_VTBL(0x0082D77C);
    self->field_0004 = (uint32_t)(uintptr_t)CSF_ADDR_DATA(0x0082D760);
    self->field_0008 = 0;

    if (self->field_001c) {
        Mem_Free(reinterpret_cast<void*>(self->field_001c));
        self->field_001c = 0;
    }

    int result = static_cast<int>(self->field_000c);
    if (result) {
        result = Mem_Free(reinterpret_cast<void*>(self->field_000c));
        self->field_000c = 0;
    }

    self->field_0004 = (uint32_t)(uintptr_t)CSF_VTBL(0x0082D410);
    *(void***)self = CSF_VTBL(0x0082D410);
    return result;
}

HOOK(0x406BC0, C_0082d77c_ReleaseOwnedBuffersAndRevertToBase, "auto");
