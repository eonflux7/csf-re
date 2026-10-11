#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

// Sets bit 0x1 in the flags dword of the sub-object at self+8, then calls
// virtual slot 35 (byte offset 0x8C) on that sub-object's child at +0x14
// with the arguments (17, 0, -1, 0). Returns the slot's result.
static int __fastcall AutonomoMarkFlagAndInvokeChildCode17(void* self, void* /*edx*/)
{
    CSF_AT(uint32_t, CSF_AT(void*, self, 0x08), 0x18) |= 1u;

    void* child = CSF_AT(void*, CSF_AT(void*, self, 0x08), 0x14);
    return ((int (__fastcall*)(void*, void*, int, int, int, int))
            (CSF_AT(void**, child, 0)[0x8C / 4]))(child, nullptr, 17, 0, -1, 0);
}

HOOK(0x4D8160, AutonomoMarkFlagAndInvokeChildCode17, "auto");
