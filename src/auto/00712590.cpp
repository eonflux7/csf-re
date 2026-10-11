#include <cstdint>
#include <cstring>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Reads a 7-dword format block (at +0x2C of the object referenced by +0x50 of this->field_0004)
// and returns (8 * block[2]) / (byte)block[3]. Returns 0 when the referenced object pointer is null.
static unsigned int __fastcall C_00863dd0_ComputeStreamBitrateRatio(C_00863dd0* self, void* /*edx*/) {
    const uint32_t sourceObject = CSF_AT(uint32_t, reinterpret_cast<void*>(self->field_0004), 0x50);
    if (sourceObject == 0) {
        return 0;
    }

    uint32_t blockDwords[7];
    std::memcpy(blockDwords,
                reinterpret_cast<const uint8_t*>(static_cast<uintptr_t>(sourceObject)) + 0x2C,
                sizeof(blockDwords));

    const uint32_t numerator = blockDwords[2] << 3;
    const uint8_t divisor = static_cast<uint8_t>(blockDwords[3]);
    return numerator / static_cast<unsigned int>(divisor);
}

HOOK(0x712590, C_00863dd0_ComputeStreamBitrateRatio, "auto");
