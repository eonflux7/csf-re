#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void, __fastcall, C_00840ea4_Construct, 0x0051C140, (C_00840ea4* self, void*));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

static uint32_t* __fastcall C_00840f28_Destroy(C_00840f28* self, void* /*edx*/, char flags)
{
    self->vtbl = CSF_VTBL(0x00840F28);
    if (self->field_0008)
    {
        Mem_Free(reinterpret_cast<void*>(self->field_0008));
        self->field_0008 = 0;
    }

    C_00840ea4_Construct(reinterpret_cast<C_00840ea4*>(self), nullptr);

    if ((flags & 1) != 0)
        Mem_Free(self);

    return reinterpret_cast<uint32_t*>(self);
}

HOOK(0x0051DF60, C_00840f28_Destroy, "auto");
