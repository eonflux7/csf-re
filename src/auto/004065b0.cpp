#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_GLOBAL(float, g_HalfDurationScale, 0x0082D444);
CSF_GLOBAL(float, g_UnsignedIntegerBias, 0x0082D744);

// Scales the unsigned argument by 0.5 and forwards it to the channel's slot-21 method.
// Chain: owner->GetChild() -> child->GetChannel(512) -> channel->GetSource(channel) -> source->SetTime(time, 0)
static int __fastcall S_004054a0_SetHalfTimeOnActiveChannel(S_004054a0* self, void* /*edx*/, uint32_t a2)
{
    void* owner = reinterpret_cast<void*>(self->field_0008);

    // owner->vtbl[0x28 / 4]: no-arg getter returning the child object.
    using GetChildFn = uint32_t(__fastcall*)(void*, void*);
    uint32_t child = reinterpret_cast<GetChildFn>(CSF_AT(void**, owner, 0)[10])(owner, nullptr);
    if (child == 0)
        return 0;

    void* childPtr = reinterpret_cast<void*>(child);

    // child->vtbl[0x88 / 4]: GetChannel(512)
    using GetChannelFn = uint32_t(__fastcall*)(void*, void*, int);
    uint32_t channel = reinterpret_cast<GetChannelFn>(CSF_AT(void**, childPtr, 0)[34])(childPtr, nullptr, 512);

    // child->vtbl[0x80 / 4]: takes the channel, returns its source object.
    using GetSourceFn = uint32_t(__fastcall*)(void*, void*, uint32_t);
    uint32_t source = reinterpret_cast<GetSourceFn>(CSF_AT(void**, childPtr, 0)[32])(childPtr, nullptr, channel);
    if (source == 0)
        return static_cast<int>(source);

    void* sourcePtr = reinterpret_cast<void*>(source);

    // Signed-to-float conversion of a2, with the unsigned correction when the sign bit is set.
    int signedArg = static_cast<int>(a2);
    float halfTime = static_cast<float>(signedArg);
    if (signedArg < 0)
        halfTime += g_UnsignedIntegerBias;
    halfTime *= g_HalfDurationScale;

    // source->vtbl[0x54 / 4]: SetTime(halfTime, 0)
    using SetTimeFn = int(__fastcall*)(void*, void*, float, int);
    return reinterpret_cast<SetTimeFn>(CSF_AT(void**, sourcePtr, 0)[21])(sourcePtr, nullptr, halfTime, 0);
}

HOOK(0x004065B0, S_004054a0_SetHalfTimeOnActiveChannel, "auto");
