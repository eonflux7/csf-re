#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void, __fastcall, sub_4DE550, 0x004DE550, (void* self, void* edx));

// S_004de550's vtable is referenced by the original's call target at 0x4DE550.
[[maybe_unused]] static const void* const s_S_004de550_Vtable = CSF_VTBL(0x004DE550);

static void __fastcall S_004de550_ClearFlagAndRunCleanup(S_004de550* self, void* /*edx*/)
{
    if (self->field_0000)
    {
        sub_4DE550(self, nullptr);
        self->field_0000 = 0;
    }
}

HOOK(0x004DE660, S_004de550_ClearFlagAndRunCleanup, "auto");
