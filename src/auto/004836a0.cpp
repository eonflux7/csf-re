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

// 0x004836A0: clears the byte flag at offset 0xBC.
static void __fastcall C_008334f0_ClearFlagBC(C_008334f0* self, void* /*edx*/)
{
    self->field_00bc = 0;
}

HOOK(0x4836A0, C_008334f0_ClearFlagBC, "auto");
