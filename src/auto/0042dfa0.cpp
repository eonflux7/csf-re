#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"

// Returns the global state pointer/handle (cdecl, no arguments).
CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());

// Reads a flag from the inner object of the menu system (__fastcall, ecx = self).
CSF_FUNC(char, __fastcall, Menus_GetInnerObjectFlag, 0x0050D800, (void* self, void*));

// True when the multiplayer stats overlay should be drawn; the flag comes from the global state.
static int __cdecl IsMpStatsDisplayActive()
{
    int globalState = GetGlobalState_50D190();
    void* stateObject = reinterpret_cast<void*>(static_cast<uintptr_t>(globalState));
    return static_cast<int>(Menus_GetInnerObjectFlag(stateObject, nullptr));
}

HOOK(0x0042DFA0, IsMpStatsDisplayActive, "auto");
