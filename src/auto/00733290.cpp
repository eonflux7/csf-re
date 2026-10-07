#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// 0x00733290: trivial getter returning the dword at offset 0x80.
// Original code: mov eax, [ecx+0x80] ; ret
static int __fastcall C_008670f0_GetField0080(C_008670f0* self, void* /*edx*/)
{
    return static_cast<int>(self->field_0080);
}

HOOK(0x00733290, C_008670f0_GetField0080, "auto");
