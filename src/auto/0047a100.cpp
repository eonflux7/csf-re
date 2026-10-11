#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

static int __fastcall C_008318c8_GetFireActionCode(
    C_008318c8* self, void* /*edx*/, char alternate)
{
    const auto owner = reinterpret_cast<void*>(self->field_0004);

    if (CSF_AT(uint8_t, owner, 0x228) && !alternate)
        return 50;

    if ((CSF_AT(uint32_t, owner, 0x170) & 0x2000) != 0)
    {
        const auto configuration = reinterpret_cast<void*>(self->field_0040);
        const bool isModeOne = CSF_AT(uint32_t, configuration, 0x150) == 1;

        if (alternate)
            return isModeOne ? 40 : 50;
        return isModeOne ? 50 : 40;
    }

    return -1;
}

HOOK(0x0047A100, C_008318c8_GetFireActionCode, "auto");
