#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void, __fastcall, sub_4DBFE0, 0x004DBFE0, (void* self, void* edx, int a2));

static int __fastcall TAiState_SetCurrentStateIfChanged(TAiState* self, void* /*edx*/, int newState)
{
    int currentState = 0;
    uint32_t stateSourcePtr = CSF_AT(uint32_t, self, 0x0C);
    if (stateSourcePtr != 0) {
        currentState = CSF_AT(int, reinterpret_cast<void*>(stateSourcePtr), 0x10);
    }
    if (newState != currentState) {
        sub_4DBFE0(self, nullptr, newState);
    }
    return newState;
}

HOOK(0x004DC150, TAiState_SetCurrentStateIfChanged, "auto");
