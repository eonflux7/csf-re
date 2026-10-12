#include <cstdint>
#include "globals.h"
#include <cstddef>

#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Returns true when all eight tracked counters inside the object referenced by
// this->field_0010 are zero. Counter offsets, in the order the original checks them:
// 0xD8, 0xBC, 0xA0, 0x84, 0x68, 0x4C, 0x30, 0x14.
static bool __fastcall S0040b040_AreAllCountersZero(S_0040b040* self, void* /*edx*/)
{
    uint8_t* state = reinterpret_cast<uint8_t*>(self->field_0010);

    return CSF_AT(uint32_t, state, 0xD8) == 0
        && CSF_AT(uint32_t, state, 0xBC) == 0
        && CSF_AT(uint32_t, state, 0xA0) == 0
        && CSF_AT(uint32_t, state, 0x84) == 0
        && CSF_AT(uint32_t, state, 0x68) == 0
        && CSF_AT(uint32_t, state, 0x4C) == 0
        && CSF_AT(uint32_t, state, 0x30) == 0
        && CSF_AT(uint32_t, state, 0x14) == 0;
}

HOOK(0x0040B040, S0040b040_AreAllCountersZero, "auto");
