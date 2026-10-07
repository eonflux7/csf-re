#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// 0x00483690: forwards to the virtual method at vtable offset 0x134 (slot 77)
// with the stack arguments (10, 0), and returns its int result.
static int __fastcall CCharActionFsm_InvokeVirtualSlot77(CCharActionFsm* self, void* /*edx*/)
{
    return ((int (__fastcall*)(void*, void*, int, int))self->vtbl[0x134 / 4])(self, nullptr, 10, 0);
}

HOOK(0x00483690, CCharActionFsm_InvokeVirtualSlot77, "auto");
