#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Writes a 32-bit value (source field + 16) and a zero byte into the caller's
// out buffer, then returns the buffer pointer.
static int __fastcall C_00861ce0_FillAltNameRecord(C_00861ce0* self, void* /*edx*/, int outBuffer)
{
    uint8_t* sourceObject = reinterpret_cast<uint8_t*>(self->field_000c);
    uint32_t sourceValue = CSF_AT(uint32_t, sourceObject, 4);

    *reinterpret_cast<uint32_t*>(outBuffer) = sourceValue + 16;
    *reinterpret_cast<uint8_t*>(outBuffer + 4) = 0;
    return outBuffer;
}

HOOK(0x006E59F0, C_00861ce0_FillAltNameRecord, "auto");
