#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

// 0x7A8ED0 - library routine; takes the caller's first stack argument and
// returns a value whose low byte is tested by the caller.
CSF_FUNC(int, __cdecl, Lib_CheckCondition, 0x7A8ED0, (int a));

// __thiscall: ecx = this, pushes a2 then calls the lib routine (cdecl, ret 0),
// pops the argument, and if the low byte of the result is non-zero sets the
// byte flag at this+0x24 to 1. Returns the full eax result untouched.
static int __fastcall MenuAccounts_SetFlagIfCheckPasses(uint8_t* self, void* /*edx*/, int a2, int a3)
{
    int result = Lib_CheckCondition(a2);
    if ((uint8_t)result)
        CSF_AT(uint8_t, self, 0x24) = 1;
    return result;
}

HOOK(0x5656D0, MenuAccounts_SetFlagIfCheckPasses, "auto");
