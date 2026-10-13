#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, sub_7DE4E0, 0x007DE4E0, ());
CSF_FUNC(int, __cdecl, AudioEffect_ReleaseHandle_7DEF60, 0x007DEF60, (void* handle, void* callback, int flags));
CSF_FUNC(int, __cdecl, CoreMemory_Free, 0x00691110, (void* block));

// Releases every audio effect handle in the table at field_000c (count field_0010),
// then frees the table itself and clears the pointer.
static int __fastcall C_00863ba8_ReleaseEffectHandlesAndFreeTable(C_00863ba8* self, void* /*edx*/)
{
    int result = (int)self->field_000c;
    if (result) {
        sub_7DE4E0();
        for (int i = 0; i < (int)self->field_0010; ++i) {
            uint32_t handle = *reinterpret_cast<uint32_t*>(self->field_000c + 4u * (uint32_t)i);
            if (handle) {
                AudioEffect_ReleaseHandle_7DEF60((void*)handle, nullptr, 0);
            }
        }
        result = (int)self->field_000c;
        if (result) {
            result = CoreMemory_Free((void*)self->field_000c);
            self->field_000c = 0;
        }
    }
    return result;
}

HOOK(0x0070F220, C_00863ba8_ReleaseEffectHandlesAndFreeTable, "auto");
