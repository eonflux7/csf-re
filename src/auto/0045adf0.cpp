#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());

// Stores the global state's field at +0xCC into the DWORD at +4 of the
// structure pointed to by the first argument. Returns the stored value.
static int __stdcall StoreGlobalStateCcIntoTarget(uint32_t targetRecord, int unusedArg2, int unusedArg3)
{
    (void)unusedArg2;
    (void)unusedArg3;

    uint32_t globalState = (uint32_t)GetGlobalState_50D190();
    int stateValue = (int)CSF_AT(uint32_t, globalState, 0xCC);

    CSF_AT(int, CSF_AT(uint32_t, targetRecord, 0x04), 0x04) = stateValue;
    return stateValue;
}

HOOK(0x0045ADF0, StoreGlobalStateCcIntoTarget, "auto");
