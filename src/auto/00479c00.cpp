#include <cstdint>
#include "hook.h"
#include "types/auto_classes.h"

static int __fastcall C_008318c8_ConsumeAmmo(C_008318c8* self, void* /*edx*/, int amount)
{
    const uint32_t remaining = self->field_0024 - static_cast<uint32_t>(amount);
    self->field_0024 = remaining;
    if (static_cast<int32_t>(remaining) < 0)
        self->field_0024 = 0;
    return amount;
}

HOOK(0x00479C00, C_008318c8_ConsumeAmmo, "auto");
