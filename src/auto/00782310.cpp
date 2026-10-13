#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(uint32_t*, __fastcall, sub_7822B0, 0x007822B0, (void* self, void* edx));

// S_007822b0's vtable is referenced by the original's call target at 0x7822B0.
[[maybe_unused]] static const void* const s_S_007822b0_Vtable = CSF_VTBL(0x007822B0);

static uint32_t* __fastcall MissionData_ReleaseAndClearFlag(S_007822b0* self, void* /*edx*/)
{
    uint32_t* result = nullptr;
    if (self->field_0000)
    {
        result = sub_7822B0(self, nullptr);
        self->field_0000 = 0;
    }
    return result;
}

HOOK(0x00782310, MissionData_ReleaseAndClearFlag, "auto");
