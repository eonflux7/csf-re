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

// 0x00428580 (7 bytes)
// Sets the object's vtable pointer to 0x00830030 (constructor-style init).
static void __fastcall C_00830030_InitVtbl(C_00830030* self, void* /*edx*/)
{
    self->vtbl = CSF_VTBL(0x00830030);
}

HOOK(0x428580, C_00830030_InitVtbl, "auto");
