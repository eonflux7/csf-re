#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Virtual slot 0x84 of C_00863ba8 (index 0x21). Forwards (a2, a3, TRUE, a4) to it
// and returns whether the slot's result was nonzero.
static int __fastcall C_00863ba8_CallSlot84ReturnsNonZero(C_00863ba8* self, void* /*edx*/, int a2, int a3, int a4) {
    using SlotFn = uint32_t(__fastcall*)(void* self, void* edx, int a2, int a3, int flag, int a4);
    SlotFn slot = reinterpret_cast<SlotFn>(CSF_AT(void**, self, 0)[0x84 / 4]);
    uint32_t result = slot(self, nullptr, a2, a3, 1, a4);
    return result != 0 ? 1 : 0;
}

HOOK(0x70F570, C_00863ba8_CallSlot84ReturnsNonZero, "auto");
