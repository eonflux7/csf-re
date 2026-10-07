#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Forwarder for the virtual method at vtable byte offset 8 (slot 2).
// The original is a bare tail-jump: load vtbl, jump to entry 2 with `this` in ecx.
static int __fastcall C_008406e8_InvokeVtblSlot2(C_008406e8* self, void* /*edx*/)
{
    void** vtbl = CSF_AT(void**, self, 0);
    return ((int(__fastcall*)(void*, void*))vtbl[2])(self, nullptr);
}

HOOK(0x510470, C_008406e8_InvokeVtblSlot2, "auto");
