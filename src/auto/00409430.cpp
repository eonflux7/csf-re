#include <cstdint>
#include <cmath>

#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Stores the absolute value of the given float into this+0x18.
static void __fastcall StoreAbsoluteFloatAt0x18(void* self, void* /*edx*/, float value)
{
    CSF_AT(float, self, 0x18) = std::fabs(value);
}

HOOK(0x00409430, StoreAbsoluteFloatAt0x18, "auto");
