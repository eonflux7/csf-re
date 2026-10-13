#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(int, __fastcall, BddLoader_ReleasePendingHandle, 0x0050E250, (void* self, void*));

// Resets the per-action controller state of a character action FSM.
// If a BDD handle is pending (flag at +0x30), releases it through the global state.
static char __fastcall CharActionFsm_ResetController(void* self, void* /*edx*/)
{
    char result = CSF_AT(char, self, 0x30);
    if (result)
    {
        int globalState = GetGlobalState_50D190();
        result = (char)BddLoader_ReleasePendingHandle(
            reinterpret_cast<void*>(static_cast<intptr_t>(globalState)), nullptr);
        CSF_AT(uint8_t, self, 0x30) = 0;
    }
    CSF_AT(uint32_t, self, 0x08) = 0;
    CSF_AT(uint32_t, self, 0x0C) = 0;
    CSF_AT(uint32_t, self, 0x1C) = 0;
    CSF_AT(uint32_t, self, 0x20) = 0;
    CSF_AT(uint8_t, self, 0x30) = 0;
    CSF_AT(int32_t, self, 0x04) = -1;
    return result;
}

HOOK(0x0048D770, CharActionFsm_ResetController, "auto");
