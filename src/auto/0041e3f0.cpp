#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// 0x0041E3F0
// Reads the pointer stored at this+0x14, then returns the dword at +0x58 of that object.
// mov eax, [ecx+0x14] ; mov eax, [eax+0x58] ; ret
static int __fastcall C_41e3f0_GetLinkedField58(void* self, void* /*edx*/)
{
    void* linked = CSF_AT(void*, self, 0x14);
    return CSF_AT(int, linked, 0x58);
}

HOOK(0x41E3F0, C_41e3f0_GetLinkedField58, "auto");
