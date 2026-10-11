#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Entry at 0x0070CC90 (thiscall, ret 0).
// If the flag at +0x04 is set, call virtual slot 0x34 (vtable index 13) on the object.
// Then run the removal step that the original tail-jumps to at 0x006FDAB0, reproduced inline:
// clear the flag, and if the clump at +0x08 is non-null, call sub_58DF40 on it and, if that
// returns non-null, call RpWorldRemoveClump(result, clump). The result's low byte is returned.
CSF_FUNC(int, __cdecl, S_006fdfe0_FindWorldFromClump, 0x0058DF40, (int a1));
CSF_FUNC(int, __cdecl, S_006fdfe0_RpWorldRemoveClump, 0x0058E310, (int a1, int a2));

static char __fastcall S_006fdfe0_ShutdownAndRemoveClump(S_006fdfe0* self, void* /*edx*/)
{
    if (self->field_0004)
    {
        using ShutdownFn = void(__fastcall*)(void* self, void* edx);
        void** vtable = CSF_AT(void**, self, 0);
        ShutdownFn shutdown = reinterpret_cast<ShutdownFn>(vtable[13]);
        shutdown(self, nullptr);
    }

    // Removal step (same behavior as the code at 0x006FDAB0).
    uint32_t result = self->field_0004;
    if (result)
    {
        result = self->field_0008;
        self->field_0004 = 0;
        if (result)
        {
            result = static_cast<uint32_t>(S_006fdfe0_FindWorldFromClump(static_cast<int>(result)));
            if (result)
            {
                result = static_cast<uint32_t>(S_006fdfe0_RpWorldRemoveClump(
                    static_cast<int>(result), static_cast<int>(self->field_0008)));
            }
        }
    }
    return static_cast<char>(result);
}

HOOK(0x0070CC90, S_006fdfe0_ShutdownAndRemoveClump, "auto");
