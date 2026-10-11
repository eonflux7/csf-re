#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(unsigned int, __fastcall, S_00508360_RunCleanup, 0x00508360, (void* self, void* edx));

static unsigned int __fastcall S_00508360_ClearFlagAndRunCleanup(S_00508360* self, void* /*edx*/)
{
    unsigned int result = 0;
    if (self->field_0000)
    {
        result = S_00508360_RunCleanup(self, nullptr);
        self->field_0000 = 0;
    }
    return result;
}

HOOK(0x00508410, S_00508360_ClearFlagAndRunCleanup, "auto");
