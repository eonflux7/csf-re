#include "globals.h"
#include "hook.h"

// Callee bound by address. __fastcall: ecx = this, edx unused, a2 on the stack.
CSF_FUNC(void, __fastcall, sub_4DBFE0, 0x004DBFE0, (void* self, void* edx, int a2));

// TAiState method at 0x4DC180 (__thiscall, one stack arg, ret 4).
// Forwards the argument to 0x4DBFE0 and returns it unchanged.
static int __fastcall TAiState_ApplyValueReturnsInput(void* self, void* /*edx*/, int a2)
{
    sub_4DBFE0(self, nullptr, a2);
    return a2;
}

HOOK(0x004DC180, TAiState_ApplyValueReturnsInput, "auto");
