#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

static void* __fastcall C_0086200c_Destroy(
    C_0086200c* self, void* /*edx*/, char flags)
{
    *(void***)self = CSF_VTBL(0x0086200C);
    self->field_000c = 0;
    self->field_0018 = 1;
    self->field_0019 = 0;
    self->field_0038 = 1;

    if (self->field_0004)
    {
        Mem_Free(reinterpret_cast<void*>(self->field_0004));
        self->field_0004 = 0;
    }

    *(void***)self = CSF_VTBL(0x00861FAC);
    if (flags & 1)
        Mem_Free(self);

    return self;
}

HOOK(0x006ED2F0, C_0086200c_Destroy, "auto");
