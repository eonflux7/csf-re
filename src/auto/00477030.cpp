#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

static CSF_FUNC(uint64_t, __fastcall, CharActionFsm_GetPrimaryCapacity, 0x004769F0, (void* self, void* edx));

static int __fastcall C_008316d8_ConsumeReserveAndIncrementPrimary(
    C_008316d8* self, void* /*edx*/)
{
    const int32_t reserveCost = CSF_AT(
        int32_t, reinterpret_cast<void*>(self->field_0040), 0x104);
    int32_t result = static_cast<int32_t>(self->field_001c);

    if (result > reserveCost)
    {
        const int32_t primaryCount = static_cast<int32_t>(self->field_0020);
        self->field_001c = static_cast<uint32_t>(result)
                         - static_cast<uint32_t>(reserveCost);
        result = static_cast<int32_t>(
            static_cast<uint32_t>(CharActionFsm_GetPrimaryCapacity(self, nullptr)));
        if (primaryCount < result)
            self->field_0020 = static_cast<uint32_t>(primaryCount) + 1;
    }

    return result;
}

HOOK(0x00477030, C_008316d8_ConsumeReserveAndIncrementPrimary, "auto");
