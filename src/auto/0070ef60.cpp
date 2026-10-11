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

// Forwarder to a DirectSound/device query; the target pops its two stack args.
CSF_FUNC(int, __fastcall, GetDeviceID, 0x7F78E6, (void* self, void* edx, void* guidPtr, void* outPtr));
// Forwarder to DirectSoundEnumerateA; the target pops its two stack args.
CSF_FUNC(int, __fastcall, DirectSoundEnumerateA, 0x7F78E0, (void* self, void* edx, void* callback, void* context));

// Returns 1 when a DirectSound device ID query fails or the enumeration reports failure;
// otherwise returns the enumeration flag, which the original initializes to 1.
static char __cdecl AudioDetect_ProbeDirectSoundDevices()
{
    alignas(8) uint32_t deviceId[4];
    deviceId[0] = 0;
    deviceId[1] = 0;
    deviceId[2] = 0;
    deviceId[3] = 0;

    if (GetDeviceID(deviceId, nullptr, reinterpret_cast<void*>(CSF_ADDR_DATA(0x872C24)), deviceId) != 0)
        return 1;

    uint32_t* enumContext = deviceId;
    char enumFlag = 1;
    int enumResult = DirectSoundEnumerateA(nullptr, nullptr,
                                           reinterpret_cast<void*>(CSF_ADDR_FN(0x70EED0)),
                                           &enumContext);
    if (enumResult != 0)
        return 1;
    return enumFlag;
}

HOOK(0x70EF60, AudioDetect_ProbeDirectSoundDevices, "auto");
