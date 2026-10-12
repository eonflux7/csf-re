#include <cstdint>
#include <cstddef>
#include "hook.h"
#include "types/auto_classes.h"

// Writes `tag` into byte 2 of the buffer at this->field_0084, then clears byte 0.
// Returns the buffer pointer.
static uint8_t* __fastcall C_00860288_WriteTagAndClearFirstByte(C_00860288* self, void* /*edx*/, char tag) {
    uint8_t* result = reinterpret_cast<uint8_t*>(self->field_0084);
    result[2] = static_cast<uint8_t>(tag);
    result[0] = 0;
    return result;
}

HOOK(0x006B2AD0, C_00860288_WriteTagAndClearFirstByte, "auto");
