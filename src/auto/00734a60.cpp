#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// 0x00734A60: returns a pointer to the sub-region of `self` starting at offset 0xC0.
// The callee takes no stack arguments (ret 0), is __thiscall (this in ecx), and
// simply computes `this + 0xC0` without touching any memory at that address.
static char* __fastcall C_008670f0_GetPtrAt0C0(C_008670f0* self, void* /*edx*/)
{
    return reinterpret_cast<char*>(self) + 0xC0;
}

HOOK(0x00734A60, C_008670f0_GetPtrAt0C0, "auto");
