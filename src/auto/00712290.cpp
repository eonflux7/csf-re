#include <cstdint>
#include "globals.h"
#include <cstddef>
#include "hook.h"
#include "types/auto_classes.h"

// sub_7DF740: __cdecl, two int args, caller pops 8 bytes.
CSF_FUNC(int, __cdecl, ReleaseAudioHandle, 0x007DF740, (int a1, int a2));

static int __fastcall C_00863dd0_ReleaseHandleAndReset(C_00863dd0* self, void* /*edx*/)
{
    int result = ReleaseAudioHandle((int)self->field_0004, 0);
    self->field_000c = 0;
    return result;
}

HOOK(0x712290, C_00863dd0_ReleaseHandleAndReset, "auto");
