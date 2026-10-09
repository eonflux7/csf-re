#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, Sound_ResetVoiceSlot, 0x00527AF0, (void* self, void* edx, void* a2));

static int __fastcall SoundSlot_TickPlaybackState(S_00526d40* self, void* /*edx*/, S_00691860* a2)
{
    // v2 = "voice should be released": not already flagged and the object's
    // virtual slot 32 reports done.
    bool releaseVoice = false;
    if (!a2->pad_0000[0]) {
        void* obj = reinterpret_cast<void*>(a2->field_0004);
        uint8_t slot32Done = ((uint8_t(__fastcall*)(void*, void*, uint32_t, S_00691860*))
                                 CSF_AT(void**, obj, 0)[32])(obj, nullptr, 0, a2);
        releaseVoice = !slot32Done;
    }

    uint32_t savedAuxHandle = CSF_AT(uint32_t, a2, 0x1C);
    a2->pad_0000[0] = releaseVoice;
    CSF_AT(uint32_t, a2, 0x1C) = 0;

    if (!releaseVoice) {
        CSF_AT(uint8_t, a2, 0x15) = 0;
        CSF_AT(uint8_t, a2, 0x16) = 0;
        Sound_ResetVoiceSlot(reinterpret_cast<uint8_t*>(self) + 4, nullptr, a2);
    }

    bool stillFlagClear = (a2->pad_0000[0] == 0);
    CSF_AT(uint32_t, a2, 0x1C) = savedAuxHandle;

    if (stillFlagClear) {
        void* obj = reinterpret_cast<void*>(a2->field_0004);
        uint8_t slot31Done = ((uint8_t(__fastcall*)(void*, void*, S_00691860*))
                                 CSF_AT(void**, obj, 0)[31])(obj, nullptr, a2);
        if (slot31Done) {
            a2->pad_0000[0] = 0;
            return 0;
        }
    }

    a2->pad_0000[0] = 1;
    return 1;
}

HOOK(0x00528610, SoundSlot_TickPlaybackState, "auto");
