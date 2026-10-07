#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// 0x006C9FE0 - void __thiscall sub_6C9FE0(_BYTE* this, int a2)  (ret 4)
CSF_FUNC(void, __fastcall, C_00860a30_SetFlag0C, 0x6C9FE0, (C_00860a30* self, void* /*edx*/, int a2));

// 0x006F2660 - pure relay: forwards its argument to the base-class method
// C_00860a30_SetFlag0C (tail jump, so the callee's `ret 4` cleans the stack).
static void __fastcall C_00862598_SetFlag0C(C_00862598* self, void* /*edx*/, int a2)
{
    C_00860a30_SetFlag0C(self, nullptr, a2);
}

HOOK(0x6F2660, C_00862598_SetFlag0C, "auto");
