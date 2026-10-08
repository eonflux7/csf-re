#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"

// Callees
CSF_FUNC(int, __fastcall, MenuMessageBox_UpdateChildClipping, 0x529910, (void* self, void* edx, int childIndex));
CSF_FUNC(int, __fastcall, CMenuMessageBox_UpdateHorizontalAnimation, 0x529B90, (void* self, void* edx, int slideOffset));

// Constants: slide step threshold (0.1f) and seconds-to-milliseconds scale.
CSF_GLOBAL(float, g_flt_0082DC84, 0x0082DC84);
CSF_GLOBAL(float, g_flt_0082E25C, 0x0082E25C);

// Advances the menu message box slide animation by deltaTime seconds.
// Once the accumulated time reaches the threshold, the horizontal slide steps
// and the child clipping is refreshed; otherwise the two child animators are
// offset by the scaled delta.
static uint8_t __fastcall CMenuMessageBox_UpdateSlideAnimation(void* self, void* /*edx*/, float deltaTime)
{
    // Guard chain: every check must pass before any animation work happens.
    if (CSF_AT(void*, self, 0x5C) == nullptr)
        return 0;
    if (CSF_AT(uint8_t, self, 0x8B8) == 0)
        return 0;
    if (CSF_AT(void*, self, 0x60) == nullptr)
        return 0;
    if (CSF_AT(void*, self, 0x64) == nullptr)
        return 0;

    void* blockingObject = CSF_AT(void*, self, 0x68);
    if (blockingObject == nullptr)
    {
        // eax still holds the value loaded from [self+0x64] (non-zero): its low byte is returned.
        return static_cast<uint8_t>(CSF_AT(uint32_t, self, 0x64));
    }

    // Virtual slot 0xAC: if the blocking object reports busy, return its result unchanged.
    uint8_t blockingResult = ((uint8_t(__fastcall*)(void*, void*))CSF_AT(void**, blockingObject, 0)[43])(blockingObject, nullptr);
    if (blockingResult != 0)
        return blockingResult;

    // Accumulate the timer. The comparison uses the unrounded sum (x87 extended in the original).
    double exactSum = static_cast<double>(deltaTime) + static_cast<double>(CSF_AT(float, self, 0x99C));
    CSF_AT(float, self, 0x99C) = static_cast<float>(exactSum);

    if (exactSum >= static_cast<double>(g_flt_0082DC84))
    {
        // Threshold reached: reset the timer and step the horizontal slide.
        CSF_AT(float, self, 0x99C) = 0.0f;

        void* stepSource = CSF_AT(void*, self, 0x68);
        const int32_t* slidePair = static_cast<const int32_t*>(
            ((void* (__fastcall*)(void*, void*))CSF_AT(void**, stepSource, 0)[39])(stepSource, nullptr));
        int32_t slideOffset = slidePair[0] + CSF_AT(int32_t, self, 0x9A0);
        CMenuMessageBox_UpdateHorizontalAnimation(self, nullptr, slideOffset);

        MenuMessageBox_UpdateChildClipping(self, nullptr, 0);
        MenuMessageBox_UpdateChildClipping(self, nullptr, 1);
        return static_cast<uint8_t>(MenuMessageBox_UpdateChildClipping(self, nullptr, 2));
    }

    // Still animating: scale the delta to milliseconds and offset both child animators.
    float scaledDelta = deltaTime * g_flt_0082E25C;

    // First child animator: [self+0x64] -> sub-object (vslot 0x110) -> rect (vslot 0x64).
    void* animatorA = ((void* (__fastcall*)(void*, void*))CSF_AT(void**, CSF_AT(void*, self, 0x64), 0)[68])(
        CSF_AT(void*, self, 0x64), nullptr);
    float segment[4];
    std::memcpy(segment, ((const void* (__fastcall*)(void*, void*))CSF_AT(void**, animatorA, 0)[25])(animatorA, nullptr),
                sizeof segment);
    segment[3] = segment[3] - scaledDelta;
    ((void (__fastcall*)(void*, void*, void*))CSF_AT(void**, animatorA, 0)[24])(animatorA, nullptr, segment);

    // Second child animator: [self+0x60] -> sub-object (vslot 0x110) -> rect (vslot 0x64).
    void* animatorB = ((void* (__fastcall*)(void*, void*))CSF_AT(void**, CSF_AT(void*, self, 0x60), 0)[68])(
        CSF_AT(void*, self, 0x60), nullptr);
    std::memcpy(segment, ((const void* (__fastcall*)(void*, void*))CSF_AT(void**, animatorB, 0)[25])(animatorB, nullptr),
                sizeof segment);
    segment[3] = segment[3] - scaledDelta;
    return ((uint8_t(__fastcall*)(void*, void*, void*))CSF_AT(void**, animatorB, 0)[24])(animatorB, nullptr, segment);
}

HOOK(0x529EE0, CMenuMessageBox_UpdateSlideAnimation, "auto");
