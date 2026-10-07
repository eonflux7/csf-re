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

// Returns bit 0 of the flag word at offset 0x58.
static int __fastcall C_00861e98_GetFlagBit0(C_00861e98* self, void* /*edx*/)
{
    return static_cast<int>(self->field_0058 & 1u);
}

HOOK(0x6E8510, C_00861e98_GetFlagBit0, "auto");
