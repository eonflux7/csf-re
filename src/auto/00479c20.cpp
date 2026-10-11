#include <cstdint>
#include "hook.h"
#include "types/auto_classes.h"

static int __fastcall C_008318c8_ConsumeReserveAmmo(C_008318c8* self, void* /*edx*/, int amount)
{
    const uint32_t remaining = self->field_0020 - static_cast<uint32_t>(amount);
    self->field_0020 = remaining;
    if (static_cast<int32_t>(remaining) < 0)
        self->field_0020 = 0;
    return amount;
}

HOOK(0x00479C20, C_008318c8_ConsumeReserveAmmo, "auto");
