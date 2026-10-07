#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Returns true when field_000c (pointer at +0xC) is null.
static bool __fastcall CCharWeaponFsm_IsField000cNull(CCharWeaponFsm* self, void* /*edx*/)
{
    return self->field_000c == 0;  // NEGATIVE CONTROL: byte bool where the original returns a zero-extended 32-bit BOOL
}

HOOK(0x47F9B0, CCharWeaponFsm_IsField000cNull, "auto");
