#include <cstdint>
#include "globals.h"
#include <cstring>
#include "hook.h"

// __thiscall(float* self, const float* a2, float a3), ret 8.
// Stores a3-scaled copy of a2[0..2] into self[0..2], a2[0..2] into self[3..5],
// sets the byte at +0x28 to 1, and returns the raw bits of a2[2].
static int __fastcall StoreScaledVectorAndSetFlag(float* self, void* /*edx*/, const float* a2, float a3) {
    self[0] = a3 * a2[0];
    self[1] = a3 * a2[1];
    self[2] = a3 * a2[2];
    self[3] = a2[0];
    self[4] = a2[1];
    self[5] = a2[2];

    uint32_t zBits;
    std::memcpy(&zBits, &a2[2], sizeof(zBits));
    int result = static_cast<int>(zBits);

    CSF_AT(uint8_t, self, 0x28) = 1;
    return result;
}

HOOK(0x0049E130, StoreScaledVectorAndSetFlag, "auto");
