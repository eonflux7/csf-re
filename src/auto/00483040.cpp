// 0x00483040 - CCharActionFsm::SetFlags / reset of two byte state fields.
//
//   mov byte ptr [ecx + 0xbd], 1    ; field_00bd = 1
//   mov byte ptr [ecx + 0xc4], 0    ; field_00c4 = 0
//   ret                             ; ret 0 -> plain __thiscall

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

// Sets field_00bd to 1 and clears field_00c4. 15 bytes, __thiscall, pops 0.
static void __fastcall CCharActionFsm_SetByteState(CCharActionFsm* self, void* /*edx*/)
{
    self->field_00bd = 1;
    self->field_00c4 = 0;
}

HOOK(0x00483040, CCharActionFsm_SetByteState, "auto");
