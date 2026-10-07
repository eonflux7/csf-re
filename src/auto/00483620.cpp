#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// 0x00483620 - calls the virtual function at vtable offset 0x134 (slot 77)
// with the constant arguments (4, 0) and returns its result.
static int __fastcall C_008334f0_SetMode4(C_008334f0* self, void* /*edx*/)
{
    return ((int(__fastcall*)(C_008334f0*, void*, int, int))self->vtbl[0x134 / 4])(self, nullptr, 4, 0);
}

HOOK(0x483620, C_008334f0_SetMode4, "auto");
