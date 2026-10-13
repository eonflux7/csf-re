#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_GLOBAL(float, g_UnsignedIntegerBias, 0x0082D744);

// Converts a 32-bit unsigned value to double: the bit pattern is loaded as signed (fild),
// and 2^32 is added back when the top bit was set.
static double UnsignedDwordToDouble(uint32_t raw) {
    int32_t asSigned = static_cast<int32_t>(raw);
    double value = static_cast<double>(asSigned);
    if (asSigned < 0) {
        value += g_UnsignedIntegerBias;
    }
    return value;
}

// Returns width / (height / depth) computed from the three dwords of the inner block
// that field_000c points to (offsets 0x0C, 0x14, 0x18).
static double __fastcall C_00861a00_ComputeAspectRatio(C_00861a00* self, void* /*edx*/) {
    auto* inner = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(self->field_000c));
    double numerator = UnsignedDwordToDouble(CSF_AT(uint32_t, inner, 0x0C));
    double denominatorA = UnsignedDwordToDouble(CSF_AT(uint32_t, inner, 0x14));
    double denominatorB = UnsignedDwordToDouble(CSF_AT(uint32_t, inner, 0x18));
    return numerator / (denominatorA / denominatorB);
}

HOOK(0x702C70, C_00861a00_ComputeAspectRatio, "auto");
