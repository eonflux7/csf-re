#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void, __fastcall, C_00840ea4_Construct_51C140, 0x0051C140, (C_00840ea4* self, void*));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

static void __fastcall C_00840ec0_Destroy(C_00840ec0* self, void* /*edx*/)
{
    self->vtbl = CSF_VTBL(0x00840EC0);

    if (self->field_00a8)
    {
        Mem_Free(reinterpret_cast<void*>(self->field_00a8));
        self->field_00a8 = 0;
    }
    if (self->field_0098)
    {
        Mem_Free(reinterpret_cast<void*>(self->field_0098));
        self->field_0098 = 0;
    }
    if (self->field_0088)
    {
        Mem_Free(reinterpret_cast<void*>(self->field_0088));
        self->field_0088 = 0;
    }
    if (self->field_0078)
    {
        Mem_Free(reinterpret_cast<void*>(self->field_0078));
        self->field_0078 = 0;
    }
    if (self->field_0068)
    {
        Mem_Free(reinterpret_cast<void*>(self->field_0068));
        self->field_0068 = 0;
    }
    if (self->field_0058)
    {
        Mem_Free(reinterpret_cast<void*>(self->field_0058));
        self->field_0058 = 0;
    }
    if (self->field_0048)
    {
        Mem_Free(reinterpret_cast<void*>(self->field_0048));
        self->field_0048 = 0;
    }
    if (self->field_0038)
    {
        Mem_Free(reinterpret_cast<void*>(self->field_0038));
        self->field_0038 = 0;
    }
    if (self->field_0028)
    {
        Mem_Free(reinterpret_cast<void*>(self->field_0028));
        self->field_0028 = 0;
    }
    if (self->field_0018)
    {
        Mem_Free(reinterpret_cast<void*>(self->field_0018));
        self->field_0018 = 0;
    }
    if (self->field_0008)
    {
        Mem_Free(reinterpret_cast<void*>(self->field_0008));
        self->field_0008 = 0;
    }

    C_00840ea4_Construct_51C140(reinterpret_cast<C_00840ea4*>(self), nullptr);
}

HOOK(0x0051C310, C_00840ec0_Destroy, "auto");
