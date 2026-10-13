#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void, __fastcall, C_00860170_Construct, 0x006B2830, (C_00860170* self, void*));
CSF_FUNC(int, __cdecl, CoreMemory_Free, 0x00691110, (void* block));

static uint32_t* __fastcall C_00860a30_DestroyAndMaybeFree(C_00860a30* self, void* /*edx*/, char a2) {
    uint32_t childObject = self->field_001c;

    self->vtbl = CSF_VTBL(0x00860A30);
    self->field_0004 = reinterpret_cast<uint32_t>(CSF_ADDR_DATA(0x00860A24));
    self->field_0008 = reinterpret_cast<uint32_t>(CSF_ADDR_DATA(0x00860A08));

    if (childObject) {
        reinterpret_cast<C_00860178*>(childObject)->vSlot4(1);
        self->field_001c = 0;
    }

    self->field_0008 = reinterpret_cast<uint32_t>(CSF_ADDR_DATA(0x0082D410));
    self->vtbl = CSF_VTBL(0x00860990);
    self->field_0004 = reinterpret_cast<uint32_t>(CSF_ADDR_DATA(0x00860988));
    self->field_0004 = reinterpret_cast<uint32_t>(CSF_ADDR_DATA(0x0082D410));

    C_00860170_Construct(reinterpret_cast<C_00860170*>(self), nullptr);

    if ((a2 & 1) != 0) {
        CoreMemory_Free(self);
    }

    return reinterpret_cast<uint32_t*>(self);
}

HOOK(0x006CA3A0, C_00860a30_DestroyAndMaybeFree, "auto");
