#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));
CSF_FUNC(int, __fastcall, BaseC0086c1e0_ResetState, 0x0075B650, (void* self, void* edx));

static void** __fastcall C_00840e38_Destroy(C_00840e38* self, void* /*edx*/, char a2)
{
    self->vtbl = CSF_VTBL(0x00840E38);

    if (self->field_000c)
        Mem_FreeDebug(reinterpret_cast<void*>(self->field_000c));
    self->field_000c = 0;
    self->field_0010 = 0;
    self->field_0014 = 0;

    BaseC0086c1e0_ResetState(self, nullptr);

    if ((a2 & 1) != 0)
        Mem_Free(self);

    return reinterpret_cast<void**>(&self->vtbl);
}

HOOK(0x0051BBC0, C_00840e38_Destroy, "auto");
