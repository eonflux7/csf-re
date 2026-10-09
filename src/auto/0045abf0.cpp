#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, GetGlobalValue_8B0568, 0x004FDBB0, ());
CSF_FUNC(int, __fastcall, CGuiControl_GetColorAnim, 0x004FE4B0, (CGuiControl* self, void*));
CSF_FUNC(double, __fastcall, ComputeColorAnimValue, 0x00444E80, (void* self, void* edx));

static void __stdcall StoreColorAnimDerivedValue(int a1, int /*a2*/, int /*a3*/)
{
    // Global GUI control, then its color animation object.
    CGuiControl* globalControl = reinterpret_cast<CGuiControl*>(GetGlobalValue_8B0568());
    int colorAnim = CGuiControl_GetColorAnim(globalControl, nullptr);

    // Target object pointer is re-read from a1+4 after the calls above.
    uint8_t* holder = reinterpret_cast<uint8_t*>(a1);
    int targetObject = *reinterpret_cast<int*>(holder + 4);

    // Original passes (colorAnim + 8) as the 'this' argument of ComputeColorAnimValue.
    void* animSub = reinterpret_cast<void*>(static_cast<uintptr_t>(colorAnim + 8));
    float computed = static_cast<float>(ComputeColorAnimValue(animSub, nullptr));

    *reinterpret_cast<float*>(reinterpret_cast<uint8_t*>(targetObject) + 4) = computed;
}

HOOK(0x0045ABF0, StoreColorAnimDerivedValue, "auto");
