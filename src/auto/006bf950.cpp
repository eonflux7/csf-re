#include <cstdint>
#include "hook.h"
#include "types/auto_classes.h"

// Fills a 5-byte out-record at `out`: a 32-bit value (field_000c + 16) followed by a cleared flag byte.
// Returns `out` in eax, as the original does.
static int __fastcall C_00860700_FillRangeHeader(C_00860700* self, void* /*edx*/, int out) {
    uint32_t* outValue = reinterpret_cast<uint32_t*>(out);
    *outValue = self->field_000c + 16;
    uint8_t* outFlag = reinterpret_cast<uint8_t*>(out + 4);
    *outFlag = 0;
    return out;
}

HOOK(0x006BF950, C_00860700_FillRangeHeader, "auto");
