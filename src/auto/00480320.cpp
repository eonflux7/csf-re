#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// 0x00480320: non-virtual wrapper that invokes the virtual method at
// vtable slot 78 (byte offset 0x138) with the constant argument 9.
static int __fastcall C_00833228_InvokeVirtual138(C_00833228* self, void* /*edx*/)
{
    return ((int(__fastcall*)(void*, void*, int))self->vtbl[0x138 / 4])(self, nullptr, 9);
}

HOOK(0x00480320, C_00833228_InvokeVirtual138, "auto");
