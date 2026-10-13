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

CSF_FUNC(int, __fastcall, sub_4D6510, 0x004D6510, (void* self, void* edx));

static int __fastcall C_0083c478_ResetState(C_0083c478* self, void* /*edx*/)
{
    sub_4D6510(self, nullptr);
    self->field_0044 = static_cast<uint32_t>(-1);
    self->field_0048 = static_cast<uint32_t>(-1);
    self->field_004c = 0;
    self->field_0050 = 2;
    self->field_0054 = 1;
    self->field_005c = 0;
    return -256;
}

HOOK(0x004D6A90, C_0083c478_ResetState, "auto");
