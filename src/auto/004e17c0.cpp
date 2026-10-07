#include <cstdint>

#include "globals.h"
#include "hook.h"
#include "core/memory.h"

// Callee bound by address (original ends with `jmp 0x4B2A20`).
CSF_FUNC(void, __stdcall, C_0085ec18_NullSub, 0x004B2A20, (int a1));

// Thunk at 0x4E17C0 (push ebp / mov ebp,esp / pop ebp / jmp 0x4B2A20):
// forwards its single argument to the nullsub, which is a __stdcall taking 1 dword.
static void __stdcall ForwardToNullSub(int a1)
{
    C_0085ec18_NullSub(a1);
}

HOOK(0x004E17C0, ForwardToNullSub, "auto");
