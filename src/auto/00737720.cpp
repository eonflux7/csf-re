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
static int32_t C_00867260_TruncateToInt32(double value) {
    return static_cast<int32_t>(static_cast<int64_t>(value));
}

// Scales two integer counts by four float factors and stores the truncated products into out[0..3].
// out[0] = count_x * field_0058, out[1] = count_y * field_005c,
// out[2] = count_x * field_0060, out[3] = count_y * field_0064. Returns out.
static uint32_t* __fastcall C_00867260_ComputeScaledIntVector(C_00867260* self, void* /*edx*/, uint32_t* out) {
    const double countX = static_cast<double>(CSF_AT(int32_t, self, 0x7c));
    const uint32_t scaledX0 = static_cast<uint32_t>(C_00867260_TruncateToInt32(countX * CSF_AT(float, self, 0x58)));
    const uint32_t scaledX1 = static_cast<uint32_t>(C_00867260_TruncateToInt32(countX * CSF_AT(float, self, 0x60)));

    const double countY = static_cast<double>(CSF_AT(int32_t, self, 0x80));
    const uint32_t scaledY0 = static_cast<uint32_t>(C_00867260_TruncateToInt32(countY * CSF_AT(float, self, 0x5c)));
    const uint32_t scaledY1 = static_cast<uint32_t>(C_00867260_TruncateToInt32(countY * CSF_AT(float, self, 0x64)));

    out[0] = scaledX0;
    out[1] = scaledY0;
    out[2] = scaledX1;
    out[3] = scaledY1;
    return out;
}

HOOK(0x737720, C_00867260_ComputeScaledIntVector, "auto");
