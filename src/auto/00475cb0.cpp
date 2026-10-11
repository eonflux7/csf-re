#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

static char __fastcall C_008316d8_AddAvailableAmmo(
    C_008316d8* self, void* /*edx*/, int amount, char skipNotification)
{
    const int available = static_cast<int32_t>(self->field_0024);
    if (amount < available)
    {
        self->field_001c += static_cast<uint32_t>(amount);
        if (skipNotification)
            return skipNotification;

        return reinterpret_cast<char(__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, self, 0)[0x74 / 4])(self, nullptr, amount);
    }

    if (available > 0)
    {
        self->field_001c += static_cast<uint32_t>(available);
        if (!skipNotification)
        {
            return reinterpret_cast<char(__fastcall*)(void*, void*, int)>(
                CSF_AT(void**, self, 0)[0x74 / 4])(self, nullptr, available);
        }
    }
    return static_cast<char>(available);
}

HOOK(0x00475CB0, C_008316d8_AddAvailableAmmo, "auto");
