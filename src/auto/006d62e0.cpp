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

// 0x006D62E0: clears the byte flag at offset 0xD8.
static void __fastcall C_00861120_ClearField00D8(C_00861120* self, void* /*edx*/)
{
    self->field_00d8 = 0;
}

HOOK(0x6D62E0, C_00861120_ClearField00D8, "auto");
