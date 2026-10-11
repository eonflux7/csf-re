#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, sub_4D1700, 0x004D1700, (void* self, void* edx));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

// Releases the buffer at +0x4C (if any), zeroes it, then resets the float/state fields at +0x40..+0x48.
static int __fastcall C_0083ad48_ReleaseBufferAndResetFields(C_0083ad48* self, void* /*edx*/)
{
    sub_4D1700(self, nullptr);

    int result = static_cast<int>(self->field_004c);
    if (result != 0) {
        result = Mem_Free(reinterpret_cast<void*>(self->field_004c));
        self->field_004c = 0;
    }
    self->field_0040 = 0;
    self->field_0044 = 0.0f;
    self->field_0048 = 0.0f;
    return result;
}

HOOK(0x004CE900, C_0083ad48_ReleaseBufferAndResetFields, "auto");
