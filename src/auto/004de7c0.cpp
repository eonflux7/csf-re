#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Original: sub_4DE060(*(S_004ddf20**)(this + 0x18), a2), reached by tail jmp.
CSF_FUNC(char, __fastcall, sub_4DE060, 0x004DE060, (void* self, void* edx, void* a2));

static char __fastcall C_00834050_ProcessSceneObject(C_00834050* self, void* /*edx*/, C_00867260* a2)
{
    // Tail jump: the callee receives the loaded pointer as 'this' and the
    // caller's stack argument unchanged; its return value passes through.
    S_004ddf20* target = CSF_AT(S_004ddf20*, self, 0x18);
    return sub_4DE060(target, nullptr, a2);
}

HOOK(0x004DE7C0, C_00834050_ProcessSceneObject, "auto");
