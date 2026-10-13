#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"

CSF_FUNC(char, __fastcall, SyncWheelTravelOnce, 0x004E2B10, (uint8_t* self, void*));

// Original: if state (+0x58) != 6, call vtable slot 35 of the object at +0x14 with
// (16, 0, -1, 0), set state to 6, then tail-jump to SyncWheelTravelOnce(owner->+0x38).
static char __fastcall WheelTravel_SetStateSixAndSync(uint8_t* self, void* /*edx*/)
{
    if (CSF_AT(int, self, 0x58) == 6)
        return 0;

    void* obj = CSF_AT(void*, self, 0x14);
    ((void(__fastcall*)(void*, void*, int, int, int, int))CSF_AT(void**, obj, 0)[0x8C / 4])(
        obj, nullptr, 16, 0, -1, 0);

    // The original reloads the owner pointer after the virtual call.
    void* owner = CSF_AT(void*, self, 0x14);
    CSF_AT(int, self, 0x58) = 6;
    uint8_t* travelTarget = CSF_AT(uint8_t*, owner, 0x38);
    return SyncWheelTravelOnce(travelTarget, nullptr);
}

HOOK(0x004D1F40, WheelTravel_SetStateSixAndSync, "auto");
