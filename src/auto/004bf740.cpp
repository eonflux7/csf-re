#include <cstdint>
#include "globals.h"
#include "hook.h"

// Binds to the original functions by address.
CSF_FUNC(int, __fastcall, GetLinkedObjectValue_4DB5C0, 0x004DB5C0, (void* self, void*));
CSF_FUNC(int, __fastcall, UnnamedApply_4DC180, 0x004DC180, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, UnnamedSetRangeParams_4BC3A0, 0x004BC3A0, (void* self, void* edx, float a2, float a3));

// Applies the debug-IA parameter set for an AI object: one linked-object sub-block
// gets an apply call (+0x440), and another sub-block gets range parameters (3.0, 8.0)
// at +0x50C. Returns the result of the range-parameter call.
static int __stdcall AiCore_ApplyDebugFinParams(int a1)
{
    void* self = reinterpret_cast<void*>(static_cast<uintptr_t>(a1));

    int linkedObjectValue = GetLinkedObjectValue_4DB5C0(self, nullptr);
    int applyBase = linkedObjectValue ? linkedObjectValue - 4 : 0;
    UnnamedApply_4DC180(self, nullptr, applyBase + 0x440);

    linkedObjectValue = GetLinkedObjectValue_4DB5C0(self, nullptr);
    int rangeBase = linkedObjectValue ? linkedObjectValue - 4 : 0;

    int result = UnnamedSetRangeParams_4BC3A0(
        reinterpret_cast<void*>(static_cast<uintptr_t>(rangeBase + 0x50C)),
        nullptr, 3.0f, 8.0f);
    return result;
}

HOOK(0x004BF740, AiCore_ApplyDebugFinParams, "auto");
