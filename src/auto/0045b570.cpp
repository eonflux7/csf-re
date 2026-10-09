#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>

#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(int, __cdecl, GetValue8B0554, 0x004E8C50, ());
CSF_FUNC(int, __fastcall, InvokeMemberVirtualSlot8, 0x004F5170, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalValue_4FBDE0, 0x004FBDE0, (float angleRadians));
CSF_FUNC(int, __fastcall, GetFirstElement, 0x004FC1D0, (void* self, void*));
CSF_FUNC(void*, __fastcall, GetCachedObjectProvider, 0x00686C10, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalValue_753A30, 0x00753A30, ());

CSF_GLOBAL(float, g_MissingMetricValue, 0x0082D4C4);
CSF_GLOBAL(float, g_DegreesToRadiansFactor, 0x008313E8);

static int __stdcall ComputeAngleAndInvokeProviderSlot8(int /*a1*/, int /*a2*/, int a3)
{
    // Resolve the angle source: a3 -> object -> sub-object -> float at +4.
    int* outerRef = reinterpret_cast<int*>(a3);
    int* inner = reinterpret_cast<int*>(*outerRef);
    float angleDegrees = *reinterpret_cast<float*>(static_cast<uintptr_t>(inner[1]) + 4);

    if (angleDegrees == g_MissingMetricValue)
    {
        // Fall back to the global metric value, then to the view object's angle.
        float globalAngle = CSF_AT(float, reinterpret_cast<void*>(
            static_cast<uintptr_t>(GetGlobalValue_753A30())), 0x13C);
        angleDegrees = globalAngle;

        int viewObject = GetValue8B0554();
        uint8_t* viewBase = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(viewObject));
        uint32_t overrideBits = CSF_AT(uint32_t, viewBase, 0x90C);

        if (overrideBits == 0xBF800000u)
        {
            angleDegrees = globalAngle;
        }
        else
        {
            angleDegrees = CSF_AT(float, viewBase, 0x54 + 0x8B8);
        }
    }

    float angleRadians = angleDegrees * g_DegreesToRadiansFactor;

    int angleSource = GetGlobalValue_4FBDE0(angleRadians);
    int firstElement = GetFirstElement(reinterpret_cast<void*>(static_cast<uintptr_t>(angleSource)), nullptr);
    void* provider = GetCachedObjectProvider(reinterpret_cast<void*>(static_cast<uintptr_t>(firstElement)), nullptr);
    return InvokeMemberVirtualSlot8(provider, nullptr);
}

HOOK(0x0045B570, ComputeAngleAndInvokeProviderSlot8, "auto");
