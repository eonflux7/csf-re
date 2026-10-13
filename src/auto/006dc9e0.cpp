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

// Tail-jumped callee at 0x006EF050: receives the overlay target object in ecx.
CSF_FUNC(int, __fastcall, CommitOverlayBuffer, 0x006EF050, (void* self, void* edx));

// Original: 0x006DC9E0, __thiscall, ret 0.
// Calls the global-flag virtual (vtable slot 0x34) and keeps its full eax.
// If the low byte is zero and field_0094 is non-null, tail-jumps to
// CommitOverlayBuffer with that target. Otherwise returns the full eax.
static int __fastcall C_00861508_CommitOverlayIfEnabled(C_00861508* self, void* /*edx*/)
{
    int result = ((int(__fastcall*)(void*, void*))CSF_AT(void**, self, 0)[0x34 / 4])(self, nullptr);
    if (!static_cast<uint8_t>(result))
    {
        if (self->field_0094)
        {
            return CommitOverlayBuffer(reinterpret_cast<void*>(self->field_0094), nullptr);
        }
    }
    return result;
}

HOOK(0x006DC9E0, C_00861508_CommitOverlayIfEnabled, "auto");
