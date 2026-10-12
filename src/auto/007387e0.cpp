#include <cstdint>
#include "hook.h"
#include "types/auto_classes.h"

// Appends a 16-bit value to the buffer at field_0010 and returns the index it was written to.
static int __fastcall C_00867358_AddOutlineIndex(C_00867358* self, void* /*edx*/, int16_t value) {
    int index = static_cast<int>(self->field_0018);
    uint16_t* indexArray = reinterpret_cast<uint16_t*>(self->field_0010);
    indexArray[index] = static_cast<uint16_t>(value);
    ++self->field_0018;
    return index;
}

HOOK(0x007387E0, C_00867358_AddOutlineIndex, "auto");
