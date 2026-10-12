#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Game heap free (0x691110), called by the original with one stack argument (cdecl, caller pops).
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

// Releases the four embedded heap blocks owned by C_008600d4 and nulls their slots.
// The vtable is restored first, as the original does.
static int __fastcall C_008600d4_ReleaseEmbeddedMembers(C_008600d4* self, void* /*edx*/)
{
    *reinterpret_cast<void***>(self) = CSF_VTBL(0x008600D4);

    if (self->field_0034) {
        Mem_Free(reinterpret_cast<void*>(self->field_0034));
        self->field_0034 = 0;
    }
    if (self->field_0024) {
        Mem_Free(reinterpret_cast<void*>(self->field_0024));
        self->field_0024 = 0;
    }
    if (self->field_0014) {
        Mem_Free(reinterpret_cast<void*>(self->field_0014));
        self->field_0014 = 0;
    }

    // The original returns the value of the last Mem_Free call when field_0004 is set,
    // otherwise the (zero) field value itself.
    int result = static_cast<int>(self->field_0004);
    if (result) {
        result = Mem_Free(reinterpret_cast<void*>(self->field_0004));
        self->field_0004 = 0;
    }
    return result;
}

HOOK(0x006AEEC0, C_008600d4_ReleaseEmbeddedMembers, "auto");
