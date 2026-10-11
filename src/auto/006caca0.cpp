#include <cstdint>
#include "hook.h"
#include "types/auto_classes.h"

// Writes `value` into the byte at offset +1 of the buffer whose address is stored in
// field_00bc, and returns that buffer address.
static int __fastcall C_00860bb8_WriteByteAtBufferPlusOne(C_00860bb8* self, void* /*edx*/, char value) {
    uint8_t* buffer = reinterpret_cast<uint8_t*>(self->field_00bc);
    buffer[1] = static_cast<uint8_t>(value);
    return static_cast<int>(self->field_00bc);
}

HOOK(0x006CACA0, C_00860bb8_WriteByteAtBufferPlusOne, "auto");
