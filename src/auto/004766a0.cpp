#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

static bool __fastcall C_008316d8_IsReadyForFireMode(
    C_008316d8* self, void* /*edx*/, char alternateMode)
{
    if (!self->vSlot9C())
        return false;

    const auto* definition =
        reinterpret_cast<const uint8_t*>(self->field_0040);
    const uint32_t mode = CSF_AT(uint32_t, definition, 0xD4);

    if (mode == 0)
        return true;

    if (mode == 1 || mode == 2)
    {
        const uint32_t requiredCount =
            CSF_AT(uint32_t, definition, 0x104) *
            ((alternateMode != 0) + 1);
        return self->field_001c == requiredCount;
    }

    return false;
}

HOOK(0x004766A0, C_008316d8_IsReadyForFireMode, "auto");
