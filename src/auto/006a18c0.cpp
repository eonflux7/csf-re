#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

// Callee bound by address (name chosen here; signature per packet).
CSF_FUNC(int, __cdecl, ZlibLog_DetachStreamOwner, 0x006A5980, (uint32_t* a1, int a2));

// Releases the stream handle held at +0x1C of the log-stream record: if the
// handle's owner record (+0x14) is set, it is detached first; then the release
// callback at +0x24 is invoked with (handle-owner-data at +0x28, handle at +0x1C)
// and the handle slot is cleared. Returns 0 on success, -2 if the record,
// handle, or release callback is missing.
static int __cdecl ZlibLog_ReleaseStreamHandle(uint32_t* a1)
{
    if (!a1)
        return -2;

    uint32_t streamHandle = CSF_AT(uint32_t, a1, 0x1C);
    if (!streamHandle || !CSF_AT(uint32_t, a1, 0x24))
        return -2;

    uint32_t ownerRecord = CSF_AT(uint32_t, reinterpret_cast<uint8_t*>(streamHandle), 0x14);
    if (ownerRecord)
        ZlibLog_DetachStreamOwner(reinterpret_cast<uint32_t*>(ownerRecord),
                                  static_cast<int>(reinterpret_cast<uintptr_t>(a1)));

    auto releaseCallback = reinterpret_cast<void(__cdecl*)(uint32_t, uint32_t)>(
        static_cast<uintptr_t>(CSF_AT(uint32_t, a1, 0x24)));
    releaseCallback(CSF_AT(uint32_t, a1, 0x28), CSF_AT(uint32_t, a1, 0x1C));

    CSF_AT(uint32_t, a1, 0x1C) = 0;
    return 0;
}

HOOK(0x006A18C0, ZlibLog_ReleaseStreamHandle, "auto");
