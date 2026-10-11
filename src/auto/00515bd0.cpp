#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Returns the index of the first slot whose active flag (byte at +76, slots 80 bytes apart)
// is nonzero, or -1 when there are no slots or none is active.
static int __fastcall C_00840af8_ComputeChildHandleFromSlot(C_00840af8* self, void* /*edx*/) {
    uint32_t slotArrayBegin = self->field_0008;
    uint32_t slotCount = 0;
    if (slotArrayBegin != 0) {
        int32_t byteSpan = static_cast<int32_t>(self->field_000c - slotArrayBegin);
        slotCount = static_cast<uint32_t>(byteSpan / 80);
    }

    int result = 0;
    if (slotCount == 0) {
        return -1;
    }

    for (uint8_t* activeFlag = reinterpret_cast<uint8_t*>(self->field_0008) + 76;
         *activeFlag == 0;
         activeFlag += 80) {
        if (static_cast<uint32_t>(++result) >= slotCount) {
            return -1;
        }
    }
    return result;
}

HOOK(0x00515BD0, C_00840af8_ComputeChildHandleFromSlot, "auto");
