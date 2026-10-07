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

// 0x004A3080: writes 0 to the dword at offset 0x64 (declared float field_0064).
static void __fastcall C_008344f0_ClearField0064(C_008344f0* self, void* /*edx*/)
{
    self->field_0064 = 0.0f;
}

HOOK(0x4A3080, C_008344f0_ClearField0064, "auto");
