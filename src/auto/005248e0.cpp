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

CSF_FUNC(int, __fastcall, sub_523CF0, 0x00523CF0, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, sub_449030, 0x00449030, (void* self, void* edx, void* a2));

// S_00523cf0 vtable reference (the packet lists it as referenced by the call to 0x523cf0).
[[maybe_unused]] static const void* const s_SoundStateVtbl = CSF_VTBL(0x00523CF0);

// Refreshes the playback state record at a2 for this sound-state object, then
// reports whether the sound has finished. Same shape as the 0x524970 variant but
// the "reset" branch calls 0x449030 (with this+8 as ecx) instead of 0x523C40.
// a2+0 = finished flag, a2+4 = owning sound object, a2+0x1C = saved timer/stack value.
static int __fastcall S_00523cf0_RefreshSoundState(S_00523cf0* self, void* /*edx*/, int a2)
{
    uint8_t* state = reinterpret_cast<uint8_t*>(a2);
    int savedValue = CSF_AT(int, state, 0x1C);

    // Finished = not previously finished AND the owner's vslot 32 reports no more playback.
    bool finished = false;
    if (CSF_AT(uint8_t, state, 0x00) == 0)
    {
        void* object = CSF_AT(void*, state, 0x04);
        auto isStillPlaying = reinterpret_cast<uint8_t(__fastcall*)(void*, void*, int, int)>(
            CSF_AT(void**, object, 0)[128 / 4]);
        uint8_t stillPlaying = isStillPlaying(object, nullptr, 0, a2);
        finished = (stillPlaying == 0);
    }
    CSF_AT(uint8_t, state, 0x00) = finished ? 1 : 0;

    sub_523CF0(self, nullptr, a2);

    CSF_AT(int, state, 0x1C) = 0;
    if (CSF_AT(uint8_t, state, 0x00) == 0)
    {
        CSF_AT(uint8_t, state, 0x15) = 0;
        CSF_AT(uint8_t, state, 0x16) = 0;
        sub_449030(reinterpret_cast<uint8_t*>(self) + 8, nullptr, state);
    }
    CSF_AT(int, state, 0x1C) = savedValue;

    // Re-check: if still flagged finished, or owner vslot 31 says not playing, report done.
    if (CSF_AT(uint8_t, state, 0x00) != 0)
    {
        CSF_AT(uint8_t, state, 0x00) = 1;
        return 1;
    }
    void* object = CSF_AT(void*, state, 0x04);
    auto queryPlaybackDone = reinterpret_cast<uint8_t(__fastcall*)(void*, void*, int)>(
        CSF_AT(void**, object, 0)[124 / 4]);
    if (queryPlaybackDone(object, nullptr, a2) == 0)
    {
        CSF_AT(uint8_t, state, 0x00) = 1;
        return 1;
    }
    CSF_AT(uint8_t, state, 0x00) = 0;
    return 0;
}

HOOK(0x005248E0, S_00523cf0_RefreshSoundState, "auto");
