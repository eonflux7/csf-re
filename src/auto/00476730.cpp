#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

static bool __fastcall C_008316d8_CanContinueFiring(
    C_008316d8* self, void* /*edx*/)
{
    const void* definition = reinterpret_cast<const void*>(self->field_0040);
    if (!CSF_AT(uint32_t, definition, 0xD4))
        return false;
    if (!self->field_0020)
        return false;

    const uint32_t limit = CSF_AT(uint32_t, definition, 0x104) * 2;
    return static_cast<int32_t>(self->field_001c) < static_cast<int32_t>(limit);
}

HOOK(0x00476730, C_008316d8_CanContinueFiring, "auto");
