#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

static CSF_FUNC(int, __cdecl, AllocateFrameForKey, 0x004723D0, (int a1));
static CSF_FUNC(char*, __fastcall, ReleaseCurrentFrame, 0x00472AB0, (void* self, void* edx));

static std::uint8_t* __fastcall C_008313f8_LookupFrameByKey(
    C_008313f8* self, void* /*edx*/, int key)
{
    auto* frame = reinterpret_cast<std::uint8_t*>(self->field_0004);
    if (static_cast<int>(*frame) != key)
    {
        ReleaseCurrentFrame(self, nullptr);
        frame = reinterpret_cast<std::uint8_t*>(AllocateFrameForKey(key));
        self->field_0004 = reinterpret_cast<std::uint32_t>(frame);
        if (frame)
            *frame = static_cast<std::uint8_t>(key);
    }
    return frame;
}

HOOK(0x00472B20, C_008313f8_LookupFrameByKey, "auto");
