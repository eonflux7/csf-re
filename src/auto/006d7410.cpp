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

// 0x006D7410 (7 bytes) - trivial getter: returns the byte field at offset 0xA0.
static uint8_t __fastcall C_00864508_GetField00a0(C_00864508* self, void* /*edx*/)
{
    return self->field_00a0;
}

HOOK(0x6D7410, C_00864508_GetField00a0, "auto");
