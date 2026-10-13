#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"

CSF_FUNC(int, __fastcall, C_00834050_SetActionMode, 0x00492760, (void* self, void* , int newActionMode));
CSF_FUNC(int, __fastcall, GetLinkedObjectValue_4DB5C0, 0x004DB5C0, (void* self, void*));

// Clears the action mode of the object linked to `a1`. When no linked object
// is found, the original falls through to reading the action-mode owner at
// absolute address 0x2C (a null-page read, reproduced as-is).
static char __stdcall AiCoverTasks_ClearActionModeFromLinked(int a1)
{
    int linkedObjectValue = GetLinkedObjectValue_4DB5C0(reinterpret_cast<void*>(static_cast<uintptr_t>(a1)), nullptr);
    if (linkedObjectValue)
    {
        void* linkedObject = reinterpret_cast<void*>(static_cast<uintptr_t>(linkedObjectValue));
        C_00834050_SetActionMode(CSF_AT(void*, linkedObject, 0x28), nullptr, 0);
    }
    else
    {
        // Null base pointer: CSF_AT(void*, base, 0x2C) reads the word at absolute address 0x2C.
        void* nullBase = reinterpret_cast<void*>(static_cast<uintptr_t>(0));
        C_00834050_SetActionMode(CSF_AT(void*, nullBase, 0x2C), nullptr, 0);
    }
    return 0;
}

HOOK(0x004AC550, AiCoverTasks_ClearActionModeFromLinked, "auto");
