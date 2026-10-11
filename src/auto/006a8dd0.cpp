#include <cstdint>
#include "globals.h"
#include "hook.h"

// Takes ownership of the pointer stored in src+4, clearing src's slot. Releases the
// object previously held in this+4 through its vtable slot 0 (scalar deleting dtor, arg 1),
// then stores the taken pointer into this+4. Returns this.
static void* __fastcall NetGatheringDdl_TransferAndReleaseHeld(void* self, void* /*edx*/, void* src)
{
    uint32_t incomingValue = CSF_AT(uint32_t, src, 4);
    CSF_AT(uint32_t, src, 4) = 0;

    void* previousHeld = reinterpret_cast<void*>(CSF_AT(uint32_t, self, 4));
    if (previousHeld) {
        using ReleaseFn = void(__fastcall*)(void*, void*, int);
        ReleaseFn releaseSlot0 = reinterpret_cast<ReleaseFn>(CSF_AT(void**, previousHeld, 0)[0]);
        releaseSlot0(previousHeld, nullptr, 1);
    }

    CSF_AT(uint32_t, self, 4) = incomingValue;
    return self;
}

HOOK(0x6A8DD0, NetGatheringDdl_TransferAndReleaseHeld, "auto");
