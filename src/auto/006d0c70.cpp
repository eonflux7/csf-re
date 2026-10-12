#include <cstdint>

#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));
CSF_FUNC(void, __fastcall, C_00860170_Construct, 0x006B2830, (C_00860170* self, void*));

static C_00860de0* __fastcall C_00860de0_Destroy(C_00860de0* self, void* /*edx*/, char flags)
{
    self->vtbl = CSF_VTBL(0x00860DE0);
    CSF_AT(uint32_t, self, 0x04) = CSF_ADDR_VT(0x00860DDC);

    C_00860170_Construct(reinterpret_cast<C_00860170*>(self), nullptr);

    if ((flags & 1) != 0)
        Mem_Free(self);

    return self;
}

HOOK(0x006D0C70, C_00860de0_Destroy, "auto");
