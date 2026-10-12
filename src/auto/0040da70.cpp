#include <cstdint>
#include "hook.h"
#include "types/auto_classes.h"

// Clears bit 0 of the state word at +0x34, then sets bit 2 and returns the new value.
static int __fastcall C_0082dcf0_ClearStateBit0AndSetBit2(C_0082dcf0* self, void* /*edx*/) {
    uint32_t flags = self->field_0034 & 0xFFFFFFFEu;
    self->field_0034 = flags;
    flags |= 4u;
    self->field_0034 = flags;
    return static_cast<int>(flags);
}

HOOK(0x40DA70, C_0082dcf0_ClearStateBit0AndSetBit2, "auto");
