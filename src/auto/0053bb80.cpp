#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

// Original function: 0x0053BB80, __thiscall, ret 0.
// Returns 1 and sets state 6 when the count reported by the object at +0x14 is >= g_MinimumCountThreshold,
// otherwise sets state -1 and returns 0.

CSF_GLOBAL(int, g_MinimumCountThreshold, 0x840A80);

CSF_FUNC(int, __cdecl, GetGlobalState, 0x50D190, ());
CSF_FUNC(unsigned int, __fastcall, GlobalState_ApplyEvent, 0x50F790,
         (void* self, void* edx, int a2, int a3, int a4, int a5));

static char __fastcall MenuProfiles_CheckProfileCountAndSetState(void* self, void* /*edx*/)
{
    // Virtual call on the sub-object at +0x14, vtable slot 0x274 / 4 = 157.
    void* countSource = CSF_AT(void*, self, 0x14);
    int profileCount = ((int(__fastcall*)(void*, void*))CSF_AT(void**, countSource, 0)[157])(countSource, nullptr);

    if (profileCount < g_MinimumCountThreshold) {
        CSF_AT(int, self, 0x38) = -1;
        return 0;
    }

    void* globalState = reinterpret_cast<void*>(static_cast<intptr_t>(GetGlobalState()));
    GlobalState_ApplyEvent(globalState, nullptr, 0x47453138, 1, 0x10, 0);
    CSF_AT(int, self, 0x38) = 6;
    return 1;
}

HOOK(0x0053BB80, MenuProfiles_CheckProfileCountAndSetState, "auto");
