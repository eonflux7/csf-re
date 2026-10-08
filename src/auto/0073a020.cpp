#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(std::int32_t**, __fastcall, FindUnsignedWordTreeEntry, 0x00739FA0,
         (std::int32_t** self, void* edx, std::int32_t** result,
          std::uint16_t* key));

CSF_GLOBAL(float, g_MissingMetricValue, 0x0082D4C4);

static double __fastcall C_00867444_GetScaledMetricValue(
    C_00867444* self, void* /*edx*/)
{
    std::int32_t* iterator;
    std::int32_t key = 77;

    std::int32_t* entry = *FindUnsignedWordTreeEntry(
        reinterpret_cast<std::int32_t**>(
            reinterpret_cast<std::uint8_t*>(self) + 0x28),
        nullptr, &iterator, reinterpret_cast<std::uint16_t*>(&key));

    if (entry == reinterpret_cast<std::int32_t*>(self->field_002c))
        return static_cast<double>(g_MissingMetricValue);

    return static_cast<double>(CSF_AT(std::int32_t, entry, 0x2c))
         * static_cast<double>(self->field_0058);
}

HOOK(0x0073A020, C_00867444_GetScaledMetricValue, "auto");
