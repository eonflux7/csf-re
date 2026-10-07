#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// 0x00480630 - Calls virtual method at vtable slot 71 (offset 0x11C) on this,
// passing the constant 14. Returns that call's result.
static int __fastcall C_00833228_InvokeVtblSlot71With14(C_00833228* self, void* /*edx*/)
{
    void** vtbl = self->vtbl;
    return ((int(__fastcall*)(void*, void*, int))vtbl[0x11c / 4])(self, nullptr, 14);
}

HOOK(0x00480630, C_00833228_InvokeVtblSlot71With14, "auto");
