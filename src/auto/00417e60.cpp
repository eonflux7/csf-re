#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Original: int __thiscall sub_417E60(C_0082e128* this, uint32_t* a2); ret 4
// Sets the flag at +0x1FC, copies three dwords from a2 into +0x200..+0x208,
// sets the float at +0x20C to 0.2, and returns the address of +0x200.
static int __fastcall C_0082e128_SetVec3FromArray(C_0082e128* self, void* /*edx*/, uint32_t* a2)
{
    self->field_01fc = 1;
    self->field_0200 = a2[0];
    CSF_AT(uint32_t, self, 0x204) = a2[1];
    CSF_AT(uint32_t, self, 0x208) = a2[2];
    self->field_020c = 0.2f;
    return static_cast<int>(reinterpret_cast<uintptr_t>(&self->field_0200));
}

HOOK(0x417E60, C_0082e128_SetVec3FromArray, "auto");
