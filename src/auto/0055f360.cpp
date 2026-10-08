#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

// 0x006A6D70 - __cdecl, returns a pointer (in eax) to a global state object.
CSF_FUNC(int, __cdecl, GetGlobalState_6A6D70, 0x6A6D70, ());

// 0x0050D190 - __cdecl, returns a pointer (in eax) to a global state object.
CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x50D190, ());

// 0x0050E1E0 - __thiscall; `this` is the value returned by GetGlobalState_50D190.
CSF_FUNC(int, __fastcall, Sub_50E1E0, 0x50E1E0, (int* self, void* edx));

// Checks the global online state; when unavailable, refreshes the other global
// state object and then invokes this menu's virtual handler (vtable slot 19).
static int __fastcall CMenuMpOnline_HandleOnlineState(void* self, void* /*edx*/, int /*a2*/)
{
    void* state = reinterpret_cast<void*>(GetGlobalState_6A6D70());
    int result = ((int(__fastcall*)(void*, void*))CSF_AT(void**, state, 0)[7])(state, nullptr);
    if (!result)
    {
        void* state2 = reinterpret_cast<void*>(GetGlobalState_50D190());
        Sub_50E1E0(reinterpret_cast<int*>(state2), nullptr);
        return ((int(__fastcall*)(void*, void*))CSF_AT(void**, self, 0)[19])(self, nullptr);
    }
    return result;
}

HOOK(0x55F360, CMenuMpOnline_HandleOnlineState, "auto");
