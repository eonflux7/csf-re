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

// Sets the byte flag at offset 0xC on the object to 1.
static void __fastcall C_00860a30_SetFlag0C(C_00860a30* self, void* /*edx*/, int /*a2*/)
{
    self->field_000c = 1;
}

HOOK(0x006C9FE0, C_00860a30_SetFlag0C, "auto");
