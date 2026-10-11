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

// Resets the default state flags of an S_0050a2e0 object and returns true.
static char __fastcall S_0050a2e0_InitDefaultFlags(S_0050a2e0* self, void* /*edx*/)
{
    self->field_0004 = 1;
    self->field_0005 = 1;
    self->field_0006 = 0;
    self->field_0007 = 1;
    self->field_0010 = 0;
    self->field_0014 = 0xFFFFFFFFu;
    self->field_0018 = 1;
    return 1;
}

HOOK(0x0050A2E0, S_0050a2e0_InitDefaultFlags, "auto");
