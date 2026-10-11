#include <cstdint>
#include "globals.h"
#include "hook.h"

// Callee at 0x6AF1C0: __thiscall taking one float on the stack (ret 4).
CSF_FUNC(int, __fastcall, sub_6AF1C0, 0x006AF1C0, (void* self, void* edx, float a2));

// Original: 0x41E7A0, __thiscall, one stack float arg, ret 4.
// Reads the pointer stored at this+0x14. If it is non-null, forwards the float
// to the sub-object at (ptr + 4). Returns ptr + 0x5C, without a null check.
static int __fastcall AiTaskStates_UpdateAndGetRecordAddress(void* self, void* /*edx*/, float a2)
{
    uint8_t* stateBlock = CSF_AT(uint8_t*, self, 0x14);
    if (stateBlock)
    {
        sub_6AF1C0(stateBlock + 4, nullptr, a2);
    }

    // Re-read the pointer after the call, as the original does.
    uint8_t* currentBlock = CSF_AT(uint8_t*, self, 0x14);
    return static_cast<int>(reinterpret_cast<uintptr_t>(currentBlock) + 0x5C);
}

HOOK(0x41E7A0, AiTaskStates_UpdateAndGetRecordAddress, "auto");
