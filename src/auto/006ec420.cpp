#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

static int __fastcall C_0086200c_AdvanceIteration(C_0086200c* self, void* /*edx*/)
{
    if (!self->field_0018)
    {
        const uint32_t nextIndex = CSF_AT(uint32_t, self, 0x14) + 1;
        const uint32_t limit = self->field_000c;
        CSF_AT(uint32_t, self, 0x14) = nextIndex;
        self->field_0018 = nextIndex < limit ? 0 : 1;
    }
    return self->field_0018 == 0;
}

HOOK(0x006EC420, C_0086200c_AdvanceIteration, "auto");
