#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Returns true when field_000c (pointer at +0xC) is null.
static int __fastcall CCharWeaponFsm_IsField000cNull(CCharWeaponFsm* self, void* /*edx*/)
{
    return self->field_000c == 0 ? 1 : 0;  // full 32-bit BOOL: the original zero-extends (xor eax,eax; sete al)
}

HOOK(0x47F9B0, CCharWeaponFsm_IsField000cNull, "auto");
