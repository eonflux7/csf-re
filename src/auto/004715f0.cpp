#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

static bool __fastcall S_004725f0_IsGreaterOrEqual(
    S_004725f0* self, void* /*edx*/, int other)
{
    const auto* value = reinterpret_cast<const uint8_t*>(self->field_0004);

    switch (CSF_AT(uint8_t, value, 0))
    {
    case 0x16:
    {
        const float left = CSF_AT(float, value, 0x4);
        const auto* right = reinterpret_cast<const uint8_t*>(
            CSF_AT(uint32_t, reinterpret_cast<const void*>(other), 0x4));
        return left >= CSF_AT(float, right, 0x4);
    }
    case 0x1A:
    case 0x1B:
    case 0x30:
    case 0x31:
    case 0x32:
    {
        const auto* right = reinterpret_cast<const uint8_t*>(
            CSF_AT(uint32_t, reinterpret_cast<const void*>(other), 0x4));
        return CSF_AT(uint32_t, value, 0x4) >=
               CSF_AT(uint32_t, right, 0x4);
    }
    default:
        return false;
    }
}

HOOK(0x004715F0, S_004725f0_IsGreaterOrEqual, "auto");
