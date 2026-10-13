#include <cstdint>
#include "globals.h"
#include <cstddef>
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void, __fastcall, AiState_ApplyTransitionValue, 0x004DBFE0, (void* self, void* edx, int a2));

// Reads the handler id from this state's slot +0x24. If it is non-zero and differs from
// the handler stored at the linked state's +0x10 (linked state pointer at this+0x0C),
// the value is forwarded to the linked state through AiState_ApplyTransitionValue.
static void __fastcall TAiState_PropagateSlot9Handler(TAiState* self, void* /*edx*/, TAiState* linkedOwner)
{
    // Virtual call: vtbl slot 0x24 / 4 = 9.
    int handlerId = ((int(__fastcall*)(void*, void*))CSF_AT(void**, self, 0)[9])(self, nullptr);
    if (handlerId != 0)
    {
        void* linkedState = CSF_AT(void*, linkedOwner, 0x0C);
        int linkedHandler = 0;
        if (linkedState != nullptr)
        {
            linkedHandler = CSF_AT(int, linkedState, 0x10);
        }
        if (handlerId != linkedHandler)
        {
            AiState_ApplyTransitionValue(linkedOwner, nullptr, handlerId);
        }
    }
}

HOOK(0x004DC1A0, TAiState_PropagateSlot9Handler, "auto");
