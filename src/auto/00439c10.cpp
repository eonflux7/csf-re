#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Virtual slots on the object at field_0030 (byte offset / 4):
//   +0x3C -> slot 15 (query count), +0x2C -> slot 11 (set value), +0x20 -> slot 8 (step/commit).
// Virtual slots on the object at field_00d0 use the same slots 11 and 8.
static uint32_t __fastcall S_00439ba0_ResetOrAdvanceCounter(S_00439ba0* self, void* /*edx*/)
{
    const uint32_t cycleLimit = 6 * (self->field_003c + 7);

    void* counterObj = reinterpret_cast<void*>(self->field_0030);
    uint32_t result = ((uint32_t(__fastcall*)(void*, void*))CSF_AT(void**, counterObj, 0)[15])(counterObj, nullptr);

    if (result != 0) {
        if (result < cycleLimit) {
            counterObj = reinterpret_cast<void*>(self->field_0030);
            ((void(__fastcall*)(void*, void*, uint32_t, int))CSF_AT(void**, counterObj, 0)[11])(counterObj, nullptr, cycleLimit, 0);
            counterObj = reinterpret_cast<void*>(self->field_0030);
            return ((uint32_t(__fastcall*)(void*, void*, int))CSF_AT(void**, counterObj, 0)[8])(counterObj, nullptr, 1);
        }
    } else {
        counterObj = reinterpret_cast<void*>(self->field_0030);
        ((void(__fastcall*)(void*, void*, int, int))CSF_AT(void**, counterObj, 0)[11])(counterObj, nullptr, 162, 0);
        counterObj = reinterpret_cast<void*>(self->field_0030);
        ((uint32_t(__fastcall*)(void*, void*, int))CSF_AT(void**, counterObj, 0)[8])(counterObj, nullptr, 1);

        void* secondaryObj = reinterpret_cast<void*>(self->field_00d0);
        ((void(__fastcall*)(void*, void*, int, int))CSF_AT(void**, secondaryObj, 0)[11])(secondaryObj, nullptr, 66, 0);
        secondaryObj = reinterpret_cast<void*>(self->field_00d0);
        return ((uint32_t(__fastcall*)(void*, void*, int))CSF_AT(void**, secondaryObj, 0)[8])(secondaryObj, nullptr, 1);
    }
    return result;
}

HOOK(0x439C10, S_00439ba0_ResetOrAdvanceCounter, "auto");
