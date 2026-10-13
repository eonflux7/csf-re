#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, GetGlobalValue_8BC1E0, 0x00756A10, ());
CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(int, __fastcall, GetStateValue, 0x0050DD30, (void* self, void* edx, int a2));

CSF_GLOBAL(float, g_ProgressScale, 0x0082E03C);

// Returns the value the original hands back in eax (GetStateValue's result).
static int __fastcall C_00833b88_ComputeProgressBlend(C_00833b88* self, void* /*edx*/)
{
    // Current and target values from the global value block.
    int startValue = CSF_AT(int, reinterpret_cast<void*>(static_cast<uintptr_t>(GetGlobalValue_8BC1E0())), 0x0AD0);
    int endValue = CSF_AT(int, reinterpret_cast<void*>(static_cast<uintptr_t>(GetGlobalValue_8BC1E0())), 0x0AD4);

    // 100 * (|end - start| + 1), computed as abs via sign mask as the original does.
    int diff = endValue - startValue;
    int sign = diff >> 31;
    int absDiff = (diff ^ sign) - sign;
    int stateKey = 100 * (absDiff + 1);

    // Pick the smaller of the two values (ties go to the end value).
    int localStart = startValue;
    int localEnd = endValue;
    int* smallerValue = &localStart;
    if (startValue >= endValue)
        smallerValue = &localEnd;

    // stateKey is pushed before GetGlobalState; GetStateValue pops it as its a2 argument.
    void* stateObject = reinterpret_cast<void*>(static_cast<uintptr_t>(GetGlobalState_50D190()));
    int result = GetStateValue(stateObject, nullptr, stateKey);

    CSF_AT(int, self, 0x0200) = 0;
    CSF_AT(float, self, 0x01FC) = static_cast<float>(result) * g_ProgressScale
                                + static_cast<float>(*smallerValue);
    return result;
}

HOOK(0x00492D70, C_00833b88_ComputeProgressBlend, "auto");
