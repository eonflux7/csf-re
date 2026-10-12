#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(char, __fastcall, sub_6F5660, 0x006F5660, (void* self, void* edx));
CSF_FUNC(int, __fastcall, C_00861fac_Destroy, 0x006ED080, (C_00861fac* self, void*));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

// Destructor for C_008627d8: resets vtable, runs base cleanup, frees the owned
// block at +0x004 and, when flags bit 0 is set, frees the object itself.
static int __fastcall C_008627d8_Destruct(C_008627d8* self, void* /*edx*/, char flags)
{
    *reinterpret_cast<void***>(self) = CSF_VTBL(0x008627D8);
    sub_6F5660(self, nullptr);
    C_00861fac_Destroy(reinterpret_cast<C_00861fac*>(&self->field_0014), nullptr);
    if (self->field_0004)
    {
        Mem_Free(reinterpret_cast<void*>(self->field_0004));
        self->field_0004 = 0;
    }
    *reinterpret_cast<void***>(self) = CSF_VTBL(0x00861FC8);
    if ((flags & 1) != 0)
    {
        Mem_Free(self);
    }
    return reinterpret_cast<int>(self);
}

HOOK(0x006F5A70, C_008627d8_Destruct, "auto");
