#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, GetGlobalValue_8B0568, 0x004FDBB0, ());
CSF_FUNC(int, __fastcall, CGuiControl_GetColorAnim, 0x004FE4B0, (CGuiControl* self, void* edx));
CSF_FUNC(double, __fastcall, ComputeColorAnimValue, 0x00444E80, (void* self, void* edx));

// Reads the color-animation value of the global GUI control, computes a
// derived value from (colorAnim + 8) and stores it as a float at
// (*(a1 + 4)) + 4.
static void __stdcall StoreColorAnimDerivedValue(int a1, int a2, int a3)
{
    CGuiControl* globalControl = reinterpret_cast<CGuiControl*>(GetGlobalValue_8B0568());
    int colorAnim = CGuiControl_GetColorAnim(globalControl, nullptr);

    void* target = *CSF_AT(void**, reinterpret_cast<void*>(a1), 0x04);

    void* colorAnimSubObject = reinterpret_cast<void*>(static_cast<uintptr_t>(colorAnim) + 8);
    double derived = ComputeColorAnimValue(colorAnimSubObject, nullptr);

    *CSF_AT(float*, target, 0x04) = static_cast<float>(derived);
}

HOOK(0x0045ABF0, StoreColorAnimDerivedValue, "auto");
