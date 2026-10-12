#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Truncates a double toward zero to a 32-bit integer (the low 32 bits of the signed 64-bit conversion).
static int32_t C_008673b0_TruncateToInt32(double value) {
    return static_cast<int32_t>(static_cast<int64_t>(value));
}

// Scales two integer counts by four float factors and stores the truncated products into out[0..3].
// out[0] = count_x * field_001c, out[1] = count_y * field_0020,
// out[2] = count_x * field_0024, out[3] = count_y * field_0028. Returns out.
static uint32_t* __fastcall C_008673b0_ComputeScaledIntVector(C_008673b0* self, void* /*edx*/, uint32_t* out) {
    const double countX = static_cast<double>(static_cast<int32_t>(self->field_002c));
    out[0] = static_cast<uint32_t>(C_008673b0_TruncateToInt32(countX * self->field_001c));

    const double countY = static_cast<double>(static_cast<int32_t>(self->field_0030));
    out[1] = static_cast<uint32_t>(C_008673b0_TruncateToInt32(countY * self->field_0020));

    out[2] = static_cast<uint32_t>(C_008673b0_TruncateToInt32(countX * self->field_0024));
    out[3] = static_cast<uint32_t>(C_008673b0_TruncateToInt32(countY * self->field_0028));
    return out;
}

HOOK(0x738E40, C_008673b0_ComputeScaledIntVector, "auto");
