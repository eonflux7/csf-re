#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(char, __fastcall, CorePaths_ClearSlotState, 0x00693370, (void* self, void* edx));
CSF_FUNC(void, __cdecl, CoreParser_ReleaseAllocation, 0x00692A70, (S_0069b1c0* allocation));

// Releases the path-parser allocation held in a slot when the slot's state byte is 0xFF,
// then marks the slot state as 0xFF again.
static void __fastcall CorePaths_ReleaseOwnedPathSlot(S_0069b1c0** self, void* /*edx*/)
{
    if (CSF_AT(uint8_t, self, 0x0B) == 0xFF)
    {
        S_0069b1c0* owned = *self;
        if (owned)
        {
            CorePaths_ClearSlotState(owned, nullptr);
            CoreParser_ReleaseAllocation(owned);
            *self = nullptr;
        }
        CSF_AT(uint8_t, self, 0x0B) = 0xFF;
    }
    else
    {
        CSF_AT(uint8_t, self, 0x0B) = 0xFF;
    }
}

HOOK(0x00693B40, CorePaths_ReleaseOwnedPathSlot, "auto");
