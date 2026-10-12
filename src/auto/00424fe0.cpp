#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"
#include "core/memory.h"

// Clears bits 1-2 of field_0018, zeroes field_0020, then calls vtable slot 0xA8
// (stdcall, one argument = field_0044) and returns its result.
static int __fastcall C_0082fcd8_ResetFlagsAndCallSlotA8(C_0082fcd8* self, void* /*edx*/) {
    self->field_0018 &= 0xFFFFFFF9u;
    uint32_t pendingArg = CSF_AT(uint32_t, self, 0x44);
    void** vtbl = CSF_AT(void**, self, 0);
    CSF_AT(uint32_t, self, 0x20) = 0;
    return ((int(__stdcall*)(uint32_t))vtbl[0xA8 / 4])(pendingArg);
}

HOOK(0x424FE0, C_0082fcd8_ResetFlagsAndCallSlotA8, "auto");
