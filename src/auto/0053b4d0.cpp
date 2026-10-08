#include "hook.h"
#include "globals.h"

// 0x004284C0 is a __cdecl helper (ret 0) that is called with the first
// incoming argument; its body has not been recovered yet.
CSF_FUNC(int, __cdecl, MenuOptions_ProcessValue_4284C0, 0x4284C0, (int value));

// 0x0053B4D0: __stdcall wrapper. It ignores its second argument and simply
// forwards the first one to the 0x004284C0 helper, returning its result.
static int __stdcall MenuGameOptions_ForwardFirstArgument(int value, int /*unused*/)
{
    return MenuOptions_ProcessValue_4284C0(value);
}

HOOK(0x53B4D0, MenuGameOptions_ForwardFirstArgument, "auto");
