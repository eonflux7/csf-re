#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(uint32_t*, __cdecl, AllocateSubobjectHandle, 0x0040A430, (int a1));
CSF_FUNC(char, __fastcall, PrepareInputForBinding, 0x0041E050, (void* self, void* edx, int a2, int a3));

static char __fastcall C_0082deb8_BindSourceAndSetFlag10(C_0082deb8* self, void* /*edx*/, int a2, int a3)
{
    char prepareResult = PrepareInputForBinding(self, nullptr, a2, a3);

    void* subobject = AllocateSubobjectHandle(0);
    self->field_0010 = reinterpret_cast<uint32_t>(subobject);

    // Virtual slot 8 (index 2) of the new subobject's vtable, __thiscall(this, a2).
    ((void(__fastcall*)(void*, void*, int))(CSF_AT(void**, subobject, 0)[2]))(subobject, nullptr, a2);

    self->field_0014 = CSF_AT(uint32_t, reinterpret_cast<uint8_t*>(a3), 4);
    self->field_0030 |= 0x10u;
    return prepareResult;
}

HOOK(0x0040FBA0, C_0082deb8_BindSourceAndSetFlag10, "auto");
