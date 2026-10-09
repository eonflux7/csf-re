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

CSF_FUNC(int, __fastcall, SoundBase_RefreshState, 0x00527E60, (void* self, void* edx, int a2));
CSF_FUNC(unsigned int, __stdcall, SoundSlot_ResetFlags, 0x0041F6C0, (uint32_t* a1, char* a2));

// Updates the playback state of a sound request block (a2). The request block holds a
// "done" flag byte at +0, a pointer to the owning sound object at +4 (its vtable slots
// 0x80 and 0x7C decide whether playback may start and whether it is finished), and a
// saved value at +0x1C. Returns 1 while the sound is still active, 0 when it has ended.
static int __fastcall C_00841128_UpdatePlaybackState(C_00841128* self, void* /*edx*/, int a2)
{
    void* request = reinterpret_cast<void*>(a2);

    bool canStart = CSF_AT(uint8_t, request, 0x00) == 0
        && !(reinterpret_cast<uint8_t(__fastcall*)(void*, void*, int, int)>(
                CSF_AT(void**, CSF_AT(void*, request, 0x04), 0)[32])
             (CSF_AT(void*, request, 0x04), nullptr, 0, a2));

    int savedValue = CSF_AT(int, request, 0x1C);
    CSF_AT(uint8_t, request, 0x00) = canStart ? 1 : 0;

    SoundBase_RefreshState(self, nullptr, a2);

    bool stillFlagClear = CSF_AT(uint8_t, request, 0x00) == 0;
    CSF_AT(int, request, 0x1C) = 0;
    if (stillFlagClear) {
        CSF_AT(uint8_t, request, 0x15) = 0;
        CSF_AT(uint8_t, request, 0x16) = 0;
        SoundSlot_ResetFlags(
            reinterpret_cast<uint32_t*>(reinterpret_cast<uint8_t*>(self) + 0x70),
            reinterpret_cast<char*>(request));
    }

    CSF_AT(int, request, 0x1C) = savedValue;

    if (CSF_AT(uint8_t, request, 0x00) == 0
        && reinterpret_cast<uint8_t(__fastcall*)(void*, void*, int)>(
               CSF_AT(void**, CSF_AT(void*, request, 0x04), 0)[31])
           (CSF_AT(void*, request, 0x04), nullptr, a2)) {
        CSF_AT(uint8_t, request, 0x00) = 0;
        return 0;
    }

    CSF_AT(uint8_t, request, 0x00) = 1;
    return 1;
}

HOOK(0x528580, C_00841128_UpdatePlaybackState, "auto");
