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

// Original 0x0048EC00: C_00833908 destructor body, reached by tail jump.
CSF_FUNC(int, __fastcall, C_00833908_Destroy_48EC00, 0x0048EC00, (C_00833908* self, void*));

// Resets the object's vtable pointers to the C_00834050 hierarchy's
// per-base tables, then tail-calls the base-class destroy.
static int __fastcall C_00834050_ResetVtablesAndDestroyBase(C_00834050* self, void* /*edx*/)
{
    *(void***)self = CSF_VTBL(0x00834050);
    self->field_0004 = (uint32_t)CSF_VTBL(0x00833FD0);
    CSF_AT(uint32_t, self, 0x008) = (uint32_t)CSF_VTBL(0x00833FCC);
    self->field_000c = (uint32_t)CSF_VTBL(0x00833FC0);
    CSF_AT(uint32_t, self, 0x010) = (uint32_t)CSF_ADDR_DATA(0x00833FB8);
    self->field_0014 = (uint32_t)CSF_VTBL(0x00833FB4);
    self->field_01a8 = (uint32_t)CSF_VTBL(0x0082D410);
    self->field_0188 = (uint32_t)CSF_VTBL(0x0082D410);
    return C_00833908_Destroy_48EC00(self, nullptr);
}

HOOK(0x49D3C0, C_00834050_ResetVtablesAndDestroyBase, "auto");
