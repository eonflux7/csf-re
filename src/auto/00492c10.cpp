#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, C_00834598_SetEntryValue, 0x004A55C0, (C_00834598* self, void* /*edx*/, int key, int value));

// The original loads ecx = [ecx+0x48] and tail-jumps into C_00834598_SetEntryValue,
// so the callee's result is returned unchanged. Two stack args, callee-popped (ret 8).
static int __fastcall AutonomySetPendingState(void* self, void* /*edx*/, int a1, int a2)
{
    C_00834598* pendingEntry = CSF_AT(C_00834598*, self, 0x48);
    return C_00834598_SetEntryValue(pendingEntry, nullptr, a1, a2);
}

HOOK(0x00492C10, AutonomySetPendingState, "auto");
