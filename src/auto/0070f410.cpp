#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// 0x0070F410 (7 bytes)
// Returns the float stored at offset 0x38 of the object referenced by this->field_0004.
static double __fastcall C_00863ba8_GetNestedFloat0038(C_00863ba8* self, void* /*edx*/)
{
    void* nested = reinterpret_cast<void*>(static_cast<uintptr_t>(self->field_0004));
    return static_cast<double>(CSF_AT(float, nested, 0x38));
}

HOOK(0x0070F410, C_00863ba8_GetNestedFloat0038, "auto");
