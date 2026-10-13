#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

// C_00833834 destructor-style teardown: resets the vtables, frees the owned
// buffer at +0x28 if present, and frees the object itself when bit 0 of a2 is set.
static void* __fastcall C_00833834_Destroy(C_00833834* self, void* /*edx*/, char a2)
{
    *(void***)self = CSF_VTBL(0x00833834);

    if (self->field_0028) {
        Mem_Free(reinterpret_cast<void*>(self->field_0028));
        self->field_0028 = 0;
    }

    self->field_0008 = static_cast<uint32_t>(reinterpret_cast<uintptr_t>(CSF_VTBL(0x0082D410)));
    *(void***)self = CSF_VTBL(0x0082D410);

    if ((a2 & 1) != 0) {
        Mem_Free(static_cast<void*>(self));
    }

    return static_cast<void*>(self);
}

HOOK(0x0048CF90, C_00833834_Destroy, "auto");
