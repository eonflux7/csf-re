#include <cstdint>
#include "globals.h"
#include "hook.h"

// Initializes a task-base object: owner pointer, a vtable for C_0082d448,
// two -1 sentinels, zeroed state, and three flag bytes (1, 0, 1).
static void* __fastcall AttackTasks_InitTaskBase(void* self, void* /*edx*/, uint32_t owner) {
    CSF_AT(uint32_t, self, 0x00) = owner;
    CSF_AT(uint32_t, self, 0x04) = 0;
    CSF_AT(void*, self, 0x08) = CSF_VTBL(0x0082D448);
    CSF_AT(int32_t, self, 0x0C) = -1;
    CSF_AT(int32_t, self, 0x10) = -1;
    CSF_AT(uint32_t, self, 0x14) = 0;
    CSF_AT(uint32_t, self, 0x18) = 0;
    CSF_AT(uint32_t, self, 0x1C) = 0;
    CSF_AT(uint8_t, self, 0x20) = 1;
    CSF_AT(uint8_t, self, 0x21) = 0;
    CSF_AT(uint8_t, self, 0x22) = 1;
    return self;
}

HOOK(0x0047C4D0, AttackTasks_InitTaskBase, "auto");
