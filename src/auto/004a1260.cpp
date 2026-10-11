#include <cstdint>
#include "globals.h"
#include "hook.h"

// Callee bindings (original addresses)
CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(int, __fastcall, GlobalStateValue_50DCF0, 0x0050DCF0, (void* self, void* edx));

// Calls GetGlobalState, feeds its result to GlobalStateValue, and maps the
// parity of that value to +1 (odd) or -1 (even).
static int GetDirectionFromGlobalState()
{
    int globalState = GetGlobalState_50D190();
    int stateValue = GlobalStateValue_50DCF0(reinterpret_cast<void*>(static_cast<uintptr_t>(static_cast<uint32_t>(globalState))), nullptr);
    return stateValue % 2 != 0 ? 1 : -1;
}

// ToggleTimer_SetState: __thiscall, one stack arg (a2), ret 4.
static char __fastcall ToggleTimer_SetState(uint8_t* self, void* /*edx*/, char a2)
{
    char result = a2;

    if (CSF_AT(uint8_t, self, 0x1C) != static_cast<uint8_t>(a2))
    {
        CSF_AT(uint8_t, self, 0x1C) = static_cast<uint8_t>(a2);
        CSF_AT(int32_t, self, 0x24) = 0;
        CSF_AT(int32_t, self, 0x2C) = 0;
        CSF_AT(int32_t, self, 0x30) = 0;
        CSF_AT(int32_t, self, 0x34) = 0;
        CSF_AT(int32_t, self, 0x38) = 0;

        CSF_AT(int32_t, self, 0x44) = GetDirectionFromGlobalState();
        int secondDirection = GetDirectionFromGlobalState();
        CSF_AT(int32_t, self, 0x48) = secondDirection;
        result = static_cast<char>(secondDirection);
    }

    return result;
}

HOOK(0x004A1260, ToggleTimer_SetState, "auto");
