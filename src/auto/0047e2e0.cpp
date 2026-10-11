#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));

// Releases the three owned buffers of an C_00832f18 task object (fields 0x34, 0x1C, 0x0C),
// clearing their bookkeeping fields and walking the vtable back through each base class.
static int __fastcall C_00832f18_ReleaseOwnedBuffers(C_00832f18* self, void* /*edx*/)
{
    *(void***)self = CSF_VTBL(0x00832F18);
    if (self->field_0034)
        Mem_FreeDebug(reinterpret_cast<void*>(static_cast<uintptr_t>(self->field_0034)));
    self->field_0034 = 0;
    self->field_0038 = 0;
    self->field_003c = 0;

    *(void***)self = CSF_VTBL(0x00832EEC);
    if (self->field_001c)
        Mem_FreeDebug(reinterpret_cast<void*>(static_cast<uintptr_t>(self->field_001c)));
    self->field_001c = 0;
    self->field_0020 = 0;
    self->field_0024 = 0;

    int result = static_cast<int>(self->field_000c);
    if (result)
        result = Mem_FreeDebug(reinterpret_cast<void*>(static_cast<uintptr_t>(self->field_000c)));
    self->field_000c = 0;
    self->field_0010 = 0;
    self->field_0014 = 0;

    *(void***)self = CSF_VTBL(0x00832BB0);
    return result;
}

HOOK(0x0047E2E0, C_00832f18_ReleaseOwnedBuffers, "auto");
