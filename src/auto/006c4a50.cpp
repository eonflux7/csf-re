#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));
CSF_FUNC(void, __fastcall, C_00860170_Construct, 0x006B2830, (C_00860170* self, void*));

static void __fastcall C_00860894_DestroyEmbeddedMember8(C_00860894* self, void* /*edx*/) {
    uint32_t* embeddedBlock = reinterpret_cast<uint32_t*>(self->field_0008);
    self->vtbl = CSF_VTBL(0x00860894);
    self->field_000c = 0;
    if (embeddedBlock) {
        if (*embeddedBlock) {
            Mem_Free(reinterpret_cast<void*>(*embeddedBlock));
            *embeddedBlock = 0;
        }
        Mem_Free(embeddedBlock);
        self->field_0008 = 0;
    }
    C_00860170_Construct(reinterpret_cast<C_00860170*>(self), nullptr);
}

HOOK(0x006C4A50, C_00860894_DestroyEmbeddedMember8, "auto");
