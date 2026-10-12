#include <cstdint>
#include "globals.h"
#include <cstddef>
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));
CSF_FUNC(void, __fastcall, C_00860170_Construct, 0x006B2830, (C_00860170* self, void*));

// Destructor for C_008630e0 (deleting variant when bit 0 of a2 is set).
static int __fastcall C_008630e0_Destroy(C_008630e0* self, void* /*edx*/, char flags) {
    self->vtbl = CSF_VTBL(0x008630E0);
    if (self->field_0014) {
        Mem_Free(reinterpret_cast<void*>(self->field_0014));
        self->field_0014 = 0;
    }
    self->vtbl = CSF_VTBL(0x00863070);
    C_00860170_Construct(reinterpret_cast<C_00860170*>(self), nullptr);
    if ((flags & 1) != 0) {
        Mem_Free(reinterpret_cast<void*>(self));
    }
    return reinterpret_cast<int>(self);
}

HOOK(0x700460, C_008630e0_Destroy, "auto");
