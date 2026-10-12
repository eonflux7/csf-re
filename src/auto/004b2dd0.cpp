#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

static CSF_FUNC(void, __fastcall, C_0082d410_SetVtable, 0x004DBD40, (C_0082d410* self, void*));
static CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

static C_0082d410* __fastcall C_0082d410_Destroy(
    C_0082d410* self, void* /*edx*/, char flags)
{
    C_0082d410_SetVtable(
        reinterpret_cast<C_0082d410*>(reinterpret_cast<uint8_t*>(self) + 0x20),
        nullptr);

    if (self->field_0008)
    {
        Mem_Free(reinterpret_cast<void*>(self->field_0008));
        self->field_0008 = 0;
    }

    C_0082d410_SetVtable(
        reinterpret_cast<C_0082d410*>(&self->field_0004), nullptr);
    C_0082d410_SetVtable(self, nullptr);

    if (flags & 1)
        Mem_Free(self);

    return self;
}

HOOK(0x004B2DD0, C_0082d410_Destroy, "auto");
