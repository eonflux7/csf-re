#include <cstdint>
#include <cstddef>
#include <cmath>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(double, __fastcall, sub_4E0F60, 0x004E0F60, (void* self, void* edx, float* a2));
CSF_GLOBAL(float, g_Const200_0f, 0x0082DDEC);

// Returns 1 when a1 is non-null and the value computed by sub_4E0F60 for a2's
// float payload has absolute value <= 200.0. Returns 0 otherwise (including NaN).
static int __cdecl AiTaskStates_IsDistanceWithin200(S_004e0c20* a1, S_004e0c20* a2)
{
    if (a1 == nullptr) {
        return 0;
    }
    double measured = sub_4E0F60(a1, nullptr, reinterpret_cast<float*>(a2->pad_0000));
    return std::fabs(measured) <= g_Const200_0f ? 1 : 0;
}

HOOK(0x41E350, AiTaskStates_IsDistanceWithin200, "auto");
