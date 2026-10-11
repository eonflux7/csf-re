#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

static int __fastcall SetCoordinatePairAndMarkDirty(void* self, void* /*edx*/,
                                                    const void* coordinates)
{
    void* linkedObject = CSF_AT(void*, self, 0xA8);
    CSF_AT(std::uint8_t, self, 0x171) = 1;
    CSF_AT(std::uint8_t, linkedObject, 0x17) = 1;
    CSF_AT(std::int32_t, self, 0x13C) =
        CSF_AT(std::int32_t, coordinates, 0x4);
    const std::int32_t secondCoordinate =
        CSF_AT(std::int32_t, coordinates, 0x8);
    CSF_AT(std::int32_t, self, 0x140) = secondCoordinate;
    return secondCoordinate;
}

HOOK(0x0048F870, SetCoordinatePairAndMarkDirty, "auto");
