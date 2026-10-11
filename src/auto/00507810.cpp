#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, sub_507790, 0x00507790, (void* self, void* edx, float a2));

// Retained address binding from the packet's vtable annotation.
[[maybe_unused]] static const auto kAnimationVtable = CSF_VTBL(0x00507790);

static char __fastcall S_00507790_AdvanceAnimation(
    S_00507790* self, void* /*edx*/, float elapsedTime)
{
    if (!self->field_0000)
        return 1;

    const uint32_t frame = CSF_AT(uint32_t, self, 0x18);
    char finished = 0;
    if (frame == self->field_005c - 1)
    {
        CSF_AT(float, self, 0x10) = 0;
        CSF_AT(float, self, 0x14) = 0;
        finished = 1;
    }

    const float advance =
        CSF_AT(float, self, 0x0C) *
        CSF_AT(float, self, 0x08) *
        elapsedTime;

    auto* frameEntry =
        reinterpret_cast<uint8_t*>(self->field_0054) + 56 * frame;
    CSF_AT(float, self, 0x10) = advance + CSF_AT(float, self, 0x10);

    uint32_t animationAddress = CSF_AT(uint32_t, frameEntry, 0x04);
    if (!animationAddress)
        animationAddress = self->field_001c;

    void* animation = reinterpret_cast<void*>(animationAddress);
    const int duration =
        ((int(__fastcall*)(void*, void*, int))
            CSF_AT(void**, animation, 0)[0x18 / 4])(animation, nullptr, 1);

    const int previousFrame = CSF_AT(int, self, 0x18);
    const float frameDuration = static_cast<float>(duration);
    const int nextFrame =
        sub_507790(self, nullptr, CSF_AT(float, self, 0x10));

    const float frameProgress = advance + CSF_AT(float, self, 0x14);
    CSF_AT(int, self, 0x18) = nextFrame;
    CSF_AT(float, self, 0x14) = frameProgress;
    if (nextFrame != previousFrame)
        CSF_AT(float, self, 0x14) = frameProgress - frameDuration;

    return finished;
}

HOOK(0x00507810, S_00507790_AdvanceAnimation, "auto");
