#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

// Resets the session-state holder: releases the object at +0x90 (if any and not
// already in state 1), then sets the state fields to their idle values.
static void __fastcall NetGatheringDdl_ResetSessionState(void* self, void* /*edx*/, int /*unusedArg*/)
{
    void* stateObject = CSF_AT(void*, self, 0x90);
    if (stateObject != nullptr)
    {
        int stateCode = CSF_AT(int, stateObject, 0xC);
        if (stateCode == 1)
        {
            return;
        }
        // Every non-1 state releases the object through its vtable slot 0 (deleting dtor flag 1).
        ((void(__fastcall*)(void*, void*, int))CSF_AT(void**, stateObject, 0)[0])(stateObject, nullptr, 1);
        CSF_AT(void*, self, 0x90) = nullptr;
    }
    CSF_AT(int, self, 0x8) = 2;
    CSF_AT(int, self, 0xC) = 0;
    CSF_AT(uint8_t, self, 0x334) = 0;
}

HOOK(0x006A8730, NetGatheringDdl_ResetSessionState, "auto");
