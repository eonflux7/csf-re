#include <cstdint>
#include "globals.h"
#include <cstddef>

#include "hook.h"
#include "types/auto_classes.h"

// Virtual slot 0xA8 (vtable index 42) of C_0082fcd8: one stack argument, object in ecx.
static int __fastcall C_0082fcd8_ClearStateAndDispatchSlotA8(C_0082fcd8* self, void* /*edx*/) {
    void** vtbl = CSF_AT(void**, self, 0);
    uint32_t storedArg = CSF_AT(uint32_t, self, 0x44);
    CSF_AT(uint32_t, self, 0x20) = 0;
    return ((int(__fastcall*)(void*, void*, uint32_t))vtbl[0xA8 / 4])(self, nullptr, storedArg);
}

HOOK(0x00424FC0, C_0082fcd8_ClearStateAndDispatchSlotA8, "auto");
