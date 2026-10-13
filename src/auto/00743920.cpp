#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"

// Callee: NetSpawnMessages_ContainsValue (__stdcall-style, ret 4). Bound per the packet's signature.
CSF_FUNC(bool, __fastcall, NetSpawnMessages_ContainsValue, 0x00751EC0, (void* self, void* /*edx*/, int value));

// Global flag: dword_8BC140
CSF_GLOBAL(uint32_t, g_AlternateStateFlag, 0x008BC140);

static char __cdecl NetCallbacks_IsAlternatePlayerState(int a1)
{
    // The original keeps the loaded flag in ecx across the call, so the callee receives it as `this`.
    uint32_t flagValue = g_AlternateStateFlag;
    if (flagValue != 0)
    {
        return NetSpawnMessages_ContainsValue(reinterpret_cast<void*>(flagValue), nullptr, a1);
    }
    return 0;
}

HOOK(0x00743920, NetCallbacks_IsAlternatePlayerState, "auto");
