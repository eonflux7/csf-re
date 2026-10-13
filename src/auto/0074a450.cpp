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

CSF_FUNC(uint32_t*, __fastcall, sub_74A3E0, 0x0074A3E0, (void* self, void* edx));

static bool __fastcall S_0074a3e0_RefreshAndMarkReady(S_0074a3e0* self, void* /*edx*/)
{
    if (self->field_0000)
        self->field_0000 = 0;
    sub_74A3E0(self, nullptr);
    self->field_0000 = 1;
    return true;
}

HOOK(0x74A450, S_0074a3e0_RefreshAndMarkReady, "auto");
