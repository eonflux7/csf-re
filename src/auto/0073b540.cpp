#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// float constant at 0x830A00 = 0.0039215689f (1/255)
CSF_GLOBAL(float, g_kInvByteMax, 0x830A00);

// 0x0073B540: returns (value at this+0x10) scaled by 1/255 as a double.
static double __fastcall NormalizeByteToUnit(float* self, void* /*edx*/)
{
    return (double)(CSF_AT(float, self, 0x10) * g_kInvByteMax);
}

HOOK(0x0073B540, NormalizeByteToUnit, "auto");
