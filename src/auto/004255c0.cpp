#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"

CSF_FUNC(float, __fastcall, GetScaledSpeedMagnitude, 0x0040E380, (float* self, void*));
CSF_GLOBAL(float, g_SpeedScaleFactor, 0x0082FE7C);
CSF_GLOBAL(float, g_TransitionOne, 0x0082D4D4);

// Bit pattern of the float 1.0f, pushed as an extra stack argument by the original (0x3F800000).
static float OneFloatFromLiteral() {
    const uint32_t oneBits = 0x3F800000u;
    float one = 0.0f;
    std::memcpy(&one, &oneBits, sizeof(one));
    return one;
}

// If the object at this+0x28 is set, computes a scaled speed factor from the owner at this+0x14
// and calls the object's virtual slot 0x68 with that factor, followed by the leftover 1.0f stack slot.
static int __fastcall C_0082fdb8_RunPhase2(int* self, void* /*edx*/) {
    int result = CSF_AT(int, self, 0x28);
    if (result) {
        void** vtbl = CSF_AT(void**, result, 0);
        uint32_t owner = CSF_AT(uint32_t, self, 0x14);
        uint32_t ownerSpeedSource = CSF_AT(uint32_t, owner, 0x4C);

        // The original pushes 0x3F800000 (1.0f) before this call; it stays on the stack.
        float leftoverOne = OneFloatFromLiteral();
        float scaledSpeed = GetScaledSpeedMagnitude(
            reinterpret_cast<float*>(ownerSpeedSource), nullptr);
        float factor = scaledSpeed * g_SpeedScaleFactor + g_TransitionOne;

        // The target object is reloaded from this+0x28 after the speed call.
        int target = CSF_AT(int, self, 0x28);
        return ((int(__fastcall*)(void*, void*, float, float))vtbl[0x68 / 4])(
            reinterpret_cast<void*>(target), nullptr, factor, leftoverOne);
    }
    return result;
}

HOOK(0x4255C0, C_0082fdb8_RunPhase2, "auto");
