#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

static int __fastcall C_00861fac_Destroy(C_00861fac* self, void* /*edx*/)
{
    self->vtbl = CSF_VTBL(0x0086200C);
    self->field_000c = 0;
    self->field_0018 = 1;
    self->field_0019 = 0;
    self->field_0038 = 1;

    int result = static_cast<int>(self->field_0004);
    if (result)
    {
        result = Mem_Free(reinterpret_cast<void*>(self->field_0004));
        self->field_0004 = 0;
    }

    self->vtbl = CSF_VTBL(0x00861FAC);
    return result;
}

HOOK(0x006ED080, C_00861fac_Destroy, "auto");
