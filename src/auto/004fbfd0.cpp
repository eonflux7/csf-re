#include <cstdint>
#include "globals.h"
#include "hook.h"

// Callee: DevConsole_ResetState(this, char preserveSecondaryState), __thiscall, ret 4.
CSF_FUNC(int, __fastcall, DevConsole_ResetState, 0x004F91C0, (void* self, void* /*edx*/, char preserveSecondaryState));

// Original: mov ecx, [ecx+0x74]; push 0; call DevConsole_ResetState; ret
static int __fastcall SelectionManager_Refresh(void* self, void* /*edx*/)
{
    void* subject = CSF_AT(void*, self, 0x74);
    return DevConsole_ResetState(subject, nullptr, 0);
}

HOOK(0x004FBFD0, SelectionManager_Refresh, "auto");
