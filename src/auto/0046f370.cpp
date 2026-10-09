#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "core/memory.h"

// Stores a3 at index a2 of the uint32 table pointed to by this->field_0074
// and returns the address of that slot.
static uint32_t* __fastcall MissionScript_SetIndexedSlot(void* self, void* /*edx*/, int index, uint32_t value) {
    uint32_t* table = CSF_AT(uint32_t*, self, 0x74);
    uint32_t* slot = &table[index];
    *slot = value;
    return slot;
}

HOOK(0x0046F370, MissionScript_SetIndexedSlot, "auto");
