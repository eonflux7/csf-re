#include <cstdint>

#include "hook.h"

// Shifts the second dword pair of a 16-byte record into the first pair,
// then clears the second pair. Returns the record pointer.
static uint32_t* __fastcall AttackTasks_ShiftPairAndClear(uint32_t* self, void* /*edx*/) {
    uint32_t secondLow = self[2];
    uint32_t secondHigh = self[3];
    self[0] = secondLow;
    self[1] = secondHigh;
    self[2] = 0;
    self[3] = 0;
    return self;
}

HOOK(0x0047AE70, AttackTasks_ShiftPairAndClear, "auto");
