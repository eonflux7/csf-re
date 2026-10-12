#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, C_00833908_Destroy_48EC00, 0x0048EC00, (C_00833908* self, void*));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

// Deleting destructor of C_00834050: resets the vtable chain, runs the base destroy,
// and frees the object when bit 0 of the flags argument is set.
static void* __fastcall C_00834050_DeletingDestructor(C_00834050* self, void* /*edx*/, char a2)
{
    self->vtbl = CSF_VTBL(0x00834050);
    self->field_0004 = reinterpret_cast<uint32_t>(CSF_VTBL(0x00833FD0));
    CSF_AT(uint32_t, self, 0x08) = reinterpret_cast<uint32_t>(CSF_VTBL(0x00833FCC));
    self->field_000c = reinterpret_cast<uint32_t>(CSF_VTBL(0x00833FC0));
    CSF_AT(uint32_t, self, 0x10) = reinterpret_cast<uint32_t>(CSF_ADDR_DATA(0x00833FB8));
    self->field_0014 = reinterpret_cast<uint32_t>(CSF_VTBL(0x00833FB4));
    self->field_01a8 = reinterpret_cast<uint32_t>(CSF_VTBL(0x0082D410));
    self->field_0188 = reinterpret_cast<uint32_t>(CSF_VTBL(0x0082D410));

    C_00833908_Destroy_48EC00(self, nullptr);

    if ((a2 & 1) != 0) {
        Mem_Free(self);
    }
    return self;
}

HOOK(0x0049EA60, C_00834050_DeletingDestructor, "auto");
