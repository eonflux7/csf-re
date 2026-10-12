#include <cstdint>
#include "globals.h"
#include "hook.h"

// Callee at 0x00775360 is __cdecl. The caller pushes its two stack arguments
// and ECX in the same order as the 0x0040B890 clone, then cleans up with
// `add esp, 0x10`. The bind line below lists two parameters, so only those are
// forwarded here.
CSF_FUNC(int, __cdecl, ReleaseNativeHandle, 0x00775360, (int a1, int a2));

// 0x007759D0: __stdcall thunk, callee-cleaned (ret 8). Forwards both stack
// arguments to ReleaseNativeHandle and returns its result unchanged.
static int __stdcall ForwardReleaseNativeHandle(int a1, int a2)
{
    return ReleaseNativeHandle(a1, a2);
}

HOOK(0x007759D0, ForwardReleaseNativeHandle, "auto");
