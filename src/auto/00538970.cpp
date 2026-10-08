#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// 0x00538970: trivial accessor returning the 32-bit value at offset 0x2C.
// Disassembly: mov eax, [ecx+0x2c] / ret 8  (two __thiscall stack args, both unused)
static int __fastcall CMenuLoading_GetField002c(CMenuLoading* self, void* /*edx*/, int /*a2*/, int /*a3*/)
{
    return static_cast<int>(self->m_ownedObjectA);
}

HOOK(0x00538970, CMenuLoading_GetField002c, "auto");
