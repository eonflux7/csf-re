#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"

CSF_FUNC(float, __fastcall, GetScaledSpeedMagnitude, 0x0040E380, (float* self, void*));

CSF_GLOBAL(float, g_SpeedScaleFactor, 0x0082FE7C);
CSF_GLOBAL(float, g_TransitionOne, 0x0082D4D4);

using VirtualSlotFn = int(__fastcall*)(void* self, void* edx, int arg);

static int __fastcall C_0082fdb8_RunPhase2(int* self, void* /*edx*/)
{
    int result = CSF_AT(int, self, 0x28);
    if (result != 0)
    {
        void* firstObject = reinterpret_cast<void*>(static_cast<uintptr_t>(result));
        VirtualSlotFn slotFn = reinterpret_cast<VirtualSlotFn>(CSF_AT(void**, firstObject, 0)[0x68 / 4]);

        void* owner = reinterpret_cast<void*>(static_cast<uintptr_t>(CSF_AT(int, self, 0x14)));
        int ownerSpeedField = CSF_AT(int, owner, 0x4C);

        // The original pushes the float constant 1.0f (0x3F800000) as the extra argument.
        constexpr uint32_t kUnitScaleBits = 0x3F800000;
        void* unitScaleArg = reinterpret_cast<void*>(static_cast<uintptr_t>(kUnitScaleBits));

        float scaledSpeed = GetScaledSpeedMagnitude(
            reinterpret_cast<float*>(static_cast<uintptr_t>(ownerSpeedField)), unitScaleArg);
        float phaseFactor = scaledSpeed * g_SpeedScaleFactor + g_TransitionOne;

        uint32_t phaseFactorBits = 0;
        std::memcpy(&phaseFactorBits, &phaseFactor, sizeof(phaseFactorBits));

        void* currentObject = reinterpret_cast<void*>(static_cast<uintptr_t>(CSF_AT(int, self, 0x28)));
        return slotFn(currentObject, nullptr, static_cast<int>(phaseFactorBits));
    }
    return result;
}

HOOK(0x004255C0, C_0082fdb8_RunPhase2, "auto");
