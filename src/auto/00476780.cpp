#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

static bool __fastcall C_008316d8_IsFireConditionSatisfied(
    C_008316d8* self, void* /*edx*/)
{
    return !self->vSlot9C()
        || static_cast<int32_t>(self->field_001c) > 0
        || CSF_AT(uint32_t, reinterpret_cast<void*>(self->field_0040), 0xD4) != 0;
}

HOOK(0x00476780, C_008316d8_IsFireConditionSatisfied, "auto");
