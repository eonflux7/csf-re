#include <cstdint>
#include "types/auto_classes.h"
#include "hook.h"

// 0x006C3620 (9 bytes) — __thiscall, ret 0
// Clears two dword fields (+0x0C, +0x18) and returns 0.
static int __fastcall C_00867358_ClearFields(C_00867358* self, void* /*edx*/)
{
    self->field_000c = 0;
    self->field_0018 = 0;
    return 0;
}

// LTCG callers (0x700DA0) keep a value in ecx across the call: the original never writes ecx.
HOOK_KEEP_ECX(0x006C3620, C_00867358_ClearFields, 0, "auto");
