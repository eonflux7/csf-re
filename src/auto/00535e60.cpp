#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Original: C_00830578_ResetMenuController @ 0x0042CAB0, __thiscall, no stack args, returns char.
CSF_FUNC(char, __fastcall, C_00830578_ResetMenuController, 0x0042CAB0, (C_00830578* self, void*));

// Thunk: forwards to the menu controller reset. The original's `ret 4` pops the one stack argument (a2),
// which is ignored. The callee's result is returned unchanged in eax.
static int __fastcall C_00830578_ForwardToResetMenuController(C_00830578* self, void* /*edx*/, int /*a2*/)
{
    return C_00830578_ResetMenuController(self, nullptr);
}

HOOK(0x535E60, C_00830578_ForwardToResetMenuController, "auto");
