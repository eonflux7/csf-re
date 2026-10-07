#include <cstdint>
#include "types/auto_classes.h"
#include "hook.h"

// Returns the signed 32-bit field at +0x48 as a double, optionally scaled by the
// float factor at +0x58 when the flag argument is non-zero.
static double __fastcall C_00867444_GetScaledValue(C_00867444* self, void* /*edx*/, char a2)
{
    double result = static_cast<double>(static_cast<int32_t>(self->field_0048));
    if (a2)
        result *= self->field_0058;
    return result;
}

HOOK(0x00739440, C_00867444_GetScaledValue, "auto");
