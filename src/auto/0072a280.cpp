#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"


CSF_FUNC(int, __fastcall, C_00863f60_Destroy, 0x00713F40,
         (C_00863f60* self, void* edx));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* memory));

static C_008657e0* __fastcall C_008657e0_Destroy(
    C_008657e0* self, void* /*edx*/, char flags)
{
    void* ownedObject = reinterpret_cast<void*>(self->field_0030);
    CSF_AT(void**, self, 0x00) = CSF_VTBL(0x008657E0);

    if (ownedObject)
    {
        reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, ownedObject, 0x00)[0])(
                ownedObject, nullptr, 1);
        self->field_0030 = 0;
    }

    C_00863f60_Destroy(reinterpret_cast<C_00863f60*>(self), nullptr);

    if (flags & 1)
        Mem_Free(self);

    return self;
}

HOOK(0x0072A280, C_008657e0_Destroy, "auto");
