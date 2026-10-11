#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

// Resets the attack-task object's buffers, switches its vtable back to the
// C_00832bb0 base, and frees the object itself when bit 0 of a2 is set.
static void** __fastcall C_00832eec_Destroy(C_00832eec* self, void* /*edx*/, char a2)
{
    self->vtbl = CSF_VTBL(0x00832EEC);

    if (self->field_001c)
        Mem_FreeDebug(reinterpret_cast<void*>(self->field_001c));
    self->field_001c = 0;
    self->field_0020 = 0;
    self->field_0024 = 0;

    if (self->field_000c)
        Mem_FreeDebug(reinterpret_cast<void*>(self->field_000c));
    self->field_000c = 0;
    self->field_0010 = 0;
    self->field_0014 = 0;

    self->vtbl = CSF_VTBL(0x00832BB0);

    if ((a2 & 1) != 0)
        Mem_Free(self);

    return reinterpret_cast<void**>(self);
}

HOOK(0x0047E690, C_00832eec_Destroy, "auto");
