// Reimplementation of 0x00707870: copies the vector stored at this->field_0014 into
// the caller's 3-dword buffer, or the global zero vector when that pointer is null.
// Returns the destination buffer.
#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_GLOBAL(uint32_t, g_ZeroVectorX, 0x008BB420);
CSF_GLOBAL(uint32_t, g_ZeroVectorY, 0x008BB424);
CSF_GLOBAL(uint32_t, g_ZeroVectorZ, 0x008BB428);

static uint32_t* __fastcall C_008633a0_CopyVectorOrZero(C_008633a0* self, void* /*edx*/, uint32_t* destination) {
    uint32_t* sourceVector = reinterpret_cast<uint32_t*>(self->field_0014);
    uint32_t* result = destination;
    if (sourceVector) {
        destination[0] = sourceVector[0];
        destination[1] = sourceVector[1];
        destination[2] = sourceVector[2];
    } else {
        destination[0] = g_ZeroVectorX;
        destination[1] = g_ZeroVectorY;
        destination[2] = g_ZeroVectorZ;
    }
    return result;
}

HOOK(0x00707870, C_008633a0_CopyVectorOrZero, "auto");
