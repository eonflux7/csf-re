#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

static int __fastcall SetCoordinatePairAndDirtyFlags(void* self, void* /*edx*/,
                                                     const std::uint32_t* coordinates)
{
    void* linkedObject = CSF_AT(void*, self, 0xA8);
    CSF_AT(std::uint8_t, self, 0x171) = 1;
    CSF_AT(std::uint8_t, linkedObject, 0x11) = 1;
    CSF_AT(std::uint32_t, self, 0x118) = coordinates[0];
    const std::uint32_t secondCoordinate = coordinates[1];
    CSF_AT(std::uint32_t, self, 0x11C) = secondCoordinate;
    return static_cast<std::int32_t>(secondCoordinate);
}

HOOK(0x0048F760, SetCoordinatePairAndDirtyFlags, "auto");
