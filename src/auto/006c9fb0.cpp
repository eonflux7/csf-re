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

// 0x006C9FB0 (9 bytes)
// Sets the byte flag at +0x0C to 1 and ORs the dword at +0x14 with 0x10.
static void __fastcall C_00860a30_SetFlagAndOrBits(C_00860a30* self, void* /*edx*/)
{
    self->field_000c = 1;
    self->field_0014 |= 0x10u;
}

HOOK(0x6C9FB0, C_00860a30_SetFlagAndOrBits, "auto");
