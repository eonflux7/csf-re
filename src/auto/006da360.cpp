#include "hook.h"
#include "globals.h"

CSF_FUNC(void**, __fastcall, sub_6DA380, 0x006DA380, (void* self, void* edx, char a2));

// Forwards to 0x6DA380 with a2 = 1 when the stack flag is non-zero.
// The original leaves eax unset when the flag is zero; we return 0 there.
static int __stdcall CallEnableIfFlagSet(int flag)
{
    int result = 0;
    if (flag)
    {
        // ecx still holds the flag value at the call site; pass it as `this`.
        void** r = sub_6DA380(reinterpret_cast<void*>(static_cast<uintptr_t>(flag)), nullptr, 1);
        result = static_cast<int>(reinterpret_cast<uintptr_t>(r));
    }
    return result;
}

HOOK(0x006DA360, CallEnableIfFlagSet, "auto");
