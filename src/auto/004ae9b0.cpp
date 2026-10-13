#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void, __fastcall, C_0082d410_SetVtable, 0x004DBD40, (C_0082d410* self, void*));
CSF_FUNC(int, __cdecl, CoreMemory_Free, 0x00691110, (void* block));

// Original: 0x004AE9B0, __thiscall, ret 4 (one stack arg: char a2).
static uint32_t* __fastcall TFncIrAParapeto_Destroy(TFncIrAParapeto* self, void* /*edx*/, char releaseSelf)
{
    if (self->field_000c) {
        CoreMemory_Free(reinterpret_cast<void*>(self->field_000c));
        self->field_000c = 0;
    }

    C_0082d410_SetVtable(reinterpret_cast<C_0082d410*>(&self->pad_0004[4]), nullptr);
    C_0082d410_SetVtable(reinterpret_cast<C_0082d410*>(self->pad_0004), nullptr);
    C_0082d410_SetVtable(reinterpret_cast<C_0082d410*>(self), nullptr);

    if ((releaseSelf & 1) != 0) {
        CoreMemory_Free(self);
    }

    return reinterpret_cast<uint32_t*>(self);
}

HOOK(0x4AE9B0, TFncIrAParapeto_Destroy, "auto");
