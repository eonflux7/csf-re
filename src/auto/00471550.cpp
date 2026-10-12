#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

static bool __fastcall S_004725f0_IsGreaterThan(
    S_004725f0* self, void* /*edx*/, int other)
{
    const auto* value = reinterpret_cast<const uint8_t*>(self->field_0004);

    switch (CSF_AT(uint8_t, value, 0x00))
    {
    case 0x16:
    {
        const auto* otherValue = reinterpret_cast<const uint8_t*>(
            CSF_AT(uint32_t, reinterpret_cast<const void*>(other), 0x04));
        return CSF_AT(float, value, 0x04) >
               CSF_AT(float, otherValue, 0x04);
    }
    case 0x1A:
    case 0x1B:
    case 0x30:
    case 0x31:
    case 0x32:
    {
        const auto* otherValue = reinterpret_cast<const uint8_t*>(
            CSF_AT(uint32_t, reinterpret_cast<const void*>(other), 0x04));
        return CSF_AT(uint32_t, value, 0x04) >
               CSF_AT(uint32_t, otherValue, 0x04);
    }
    default:
        return false;
    }
}

HOOK(0x00471550, S_004725f0_IsGreaterThan, "auto");
