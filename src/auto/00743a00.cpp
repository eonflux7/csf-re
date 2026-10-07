#include <cstdint>
#include "globals.h"
#include "hook.h"

// 0x669B40 — cdecl, takes one argument (a callback/function pointer) and returns an int.
// Its real signature is unknown, but the original pushes exactly one 4-byte value and
// cleans the stack itself, so declare one pointer-sized parameter.
CSF_FUNC(int, __cdecl, InstallCallback, 0x669B40, (void* callback));

// 0x7437E0 — a function whose ADDRESS (not its body) is the sole argument handed to
// InstallCallback. Bound by address so the exact constant 0x7437E0 appears in this file.
CSF_FUNC(void, __cdecl, Callback_7437E0, 0x7437E0, (void));

// 0x743A00: installs callback 0x7437E0 via 0x669B40 and forwards its return value.
//   push 0x7437e0 / call 0x669b40 / pop ecx / ret
static int __cdecl RegisterCallback_743A00()
{
    return InstallCallback(reinterpret_cast<void*>(Callback_7437E0));
}

HOOK(0x743A00, RegisterCallback_743A00, "auto");
