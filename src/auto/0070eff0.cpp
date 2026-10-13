#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, sub_7DC630, 0x007DC630, (int a1, uint32_t* a2, uint32_t* a3));
CSF_FUNC(int, __cdecl, sub_7DC410, 0x007DC410, (int a1, int a2));
CSF_FUNC(char, __cdecl, AudioDetect_ProbeDirectSoundDevices, 0x0070EF60, ());
CSF_FUNC(int, __cdecl, sub_7EFD80, 0x007EFD80, (int* a1, char* a2, int a3, int (__cdecl* a4)(int, int, char*, int), int a5));

// Stack record filled by sub_7DC410 / read back here. Offsets relative to its base
// (the original's ebp-0x28): +0x08 = v7, +0x0C = v8, +0x1A = v9 (word), +0x24 = v10.
static int __fastcall C_00863ac0_ProbeAndStoreAudioFlags(C_00863ac0* self, void* /*edx*/, char a2)
{
    alignas(8) uint8_t probeRecord[0x28];

    int deviceHandle = sub_7DC630(0, reinterpret_cast<uint32_t*>(CSF_ADDR_DATA(0x008848F8)), nullptr);
    sub_7DC410(deviceHandle, reinterpret_cast<int>(probeRecord));

    uint8_t usable = (CSF_AT(int, probeRecord, 0x0C) && !CSF_AT(int, probeRecord, 0x08)
                      && AudioDetect_ProbeDirectSoundDevices()) ? 1 : 0;
    self->field_0038 = usable;

    if (a2) {
        self->field_0039 = usable;
    } else {
        self->field_0039 = 0;
        CSF_AT(int, probeRecord, 0x0C) = 0;
    }

    self->field_0034 = CSF_AT(uint16_t, probeRecord, 0x1A);
    CSF_AT(int, probeRecord, 0x24) = 1;

    return sub_7EFD80(nullptr, reinterpret_cast<char*>(probeRecord), 6, nullptr, 0);
}

HOOK(0x0070EFF0, C_00863ac0_ProbeAndStoreAudioFlags, "auto");
