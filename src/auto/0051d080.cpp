#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void, __fastcall, C_00840ea4_Construct, 0x0051C140, (C_00840ea4* self, void*));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

static uint32_t* __fastcall C_00840ee0_Destroy(C_00840ee0* self, void* /*edx*/, char flags)
{
    self->vtbl = CSF_VTBL(0x00840EE0);
    if (self->field_000c)
    {
        Mem_Free(reinterpret_cast<void*>(self->field_000c));
        self->field_000c = 0;
    }

    C_00840ea4_Construct(reinterpret_cast<C_00840ea4*>(self), nullptr);

    if ((flags & 1) != 0)
        Mem_Free(self);

    return reinterpret_cast<uint32_t*>(self);
}

HOOK(0x0051D080, C_00840ee0_Destroy, "auto");
