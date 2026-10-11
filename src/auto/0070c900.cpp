#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

CSF_FUNC(char, __cdecl, SceneLoader_SerializeReference, 0x00442E70, (void* a3, bool* a2));

// Water slot state machine: refreshes the "ready" flag at a2+0, serializes the
// reference held at self+4 when not ready, then asks the slot's owner
// (a2->obj, vtable slot 31) whether it should be skipped.
static int __fastcall Water_ProcessSlotState(void* self, void* /*edx*/, void* a2)
{
    uint8_t readyFlag;
    if (CSF_AT(uint8_t, a2, 0) == 0)
    {
        void* obj = CSF_AT(void*, a2, 4);
        uint8_t stillBusy = ((uint8_t(__fastcall*)(void*, void*, int, void*))
                              CSF_AT(void**, obj, 0)[32])(obj, nullptr, 0, a2);
        readyFlag = (stillBusy == 0) ? 1 : 0;
    }
    else
    {
        readyFlag = 0;
    }

    int savedSlotValue = CSF_AT(int, a2, 0x1c);
    CSF_AT(uint8_t, a2, 0) = readyFlag;
    CSF_AT(int, a2, 0x1c) = 0;

    if (readyFlag == 0)
    {
        CSF_AT(uint8_t, a2, 0x15) = 0;
        CSF_AT(uint8_t, a2, 0x16) = 0;
        SceneLoader_SerializeReference((void*)((uint8_t*)self + 4), (bool*)a2);
    }

    bool flagIsClear = (CSF_AT(uint8_t, a2, 0) == 0);
    CSF_AT(int, a2, 0x1c) = savedSlotValue;

    if (flagIsClear)
    {
        void* obj = CSF_AT(void*, a2, 4);
        if (((uint8_t(__fastcall*)(void*, void*, void*))
              CSF_AT(void**, obj, 0)[31])(obj, nullptr, a2))
        {
            CSF_AT(uint8_t, a2, 0) = 0;
            return 0;
        }
    }

    CSF_AT(uint8_t, a2, 0) = 1;
    return 1;
}

HOOK(0x0070C900, Water_ProcessSlotState, "auto");
