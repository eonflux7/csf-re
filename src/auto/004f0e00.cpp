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

// Zeroes three periodically-spaced 32-bit slots (offsets 0x14, 0x24, 0x34).
static int __fastcall C_0083ef30_ClearTimerEntries(C_0083ef30* self, void* /*edx*/)
{
    self->field_0014 = 0;
    self->field_0024 = 0;
    self->field_0034 = 0;
    return 0;
}

HOOK(0x4F0E00, C_0083ef30_ClearTimerEntries, "auto");
