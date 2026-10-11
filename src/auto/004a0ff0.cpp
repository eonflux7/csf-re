#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Debug free of a string buffer (heap block when capacity >= 16).
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
// Release of the array block that owns the element objects at field_0004.
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

static int __fastcall C_00831278_CleanupBase(C_00831278* self, void* /*edx*/)
{
    int result = 0;

    self->vtbl = CSF_VTBL(0x00831278);

    // Heap-allocated buffer is used only when capacity >= 16 (unsigned compare).
    if (self->field_002c >= 0x10) {
        result = static_cast<int>(self->field_0018);
        if (result) {
            result = Mem_FreeDebug(reinterpret_cast<void*>(self->field_0018));
        }
    }

    // Reset to the inline (short) state.
    self->field_002c = 15;
    self->field_0028 = 0;
    self->field_0018 &= 0xFFFFFF00u;

    // field_0004 points at an element array; the element count header sits 4 bytes before it.
    void* elements = reinterpret_cast<void*>(self->field_0004);
    if (elements) {
        uint32_t* header = reinterpret_cast<uint32_t*>(self->field_0004) - 1;
        if (header[0]) {
            // Destroy the first element through its vtable slot 0 with argument 3 (delete flag).
            result = ((int(__fastcall*)(void*, void*, int))CSF_AT(void**, elements, 0)[0])(elements, nullptr, 3);
            self->field_0004 = 0;
            self->vtbl = CSF_VTBL(0x0082D410);
            return result;
        }
        Mem_Free(header);
        self->field_0004 = 0;
    }

    self->vtbl = CSF_VTBL(0x0082D410);
    return result;
}

HOOK(0x4A0FF0, C_00831278_CleanupBase, "auto");
