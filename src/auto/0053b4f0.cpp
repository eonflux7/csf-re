#include "hook.h"
#include "globals.h"

// 0x00428540 is a __cdecl helper (ret 0) that is called with the first
// incoming argument; its body has not been recovered yet.
CSF_FUNC(int, __cdecl, MenuOptions_ProcessValue_428540, 0x428540, (int value));

// 0x0053B4F0: __stdcall wrapper. It ignores its second argument and simply
// forwards the first one to the 0x00428540 helper, returning its result.
static int __stdcall MenuGameOptions_ForwardFirstArgument(int value, int /*unused*/)
{
    return MenuOptions_ProcessValue_428540(value);
}

HOOK(0x53B4F0, MenuGameOptions_ForwardFirstArgument, "auto");
