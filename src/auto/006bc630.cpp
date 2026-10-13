#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(int, __fastcall, GlobalInt_ResolveAccessor, 0x006B13A0, (void* self, void* edx));

static int __fastcall C_008605d8_StoreValueAndApplyToAccessor(C_008605d8* self, void* /*edx*/, int a2)
{
    self->field_017c = static_cast<uint32_t>(a2);

    C_00860130* globalInt = reinterpret_cast<C_00860130*>(GetGlobalInt());
    void* accessor = reinterpret_cast<void*>(GlobalInt_ResolveAccessor(globalInt, nullptr));

    // accessor->vtbl[0x10 / 4]() : no arguments, returns the nested object
    void* nestedObject = reinterpret_cast<void*>(
        ((int(__fastcall*)(void*, void*))CSF_AT(void**, accessor, 0)[0x10 / 4])(accessor, nullptr));

    // nestedObject->vtbl[0x44 / 4](a2)
    return ((int(__fastcall*)(void*, void*, int))CSF_AT(void**, nestedObject, 0)[0x44 / 4])(nestedObject, nullptr, a2);
}

HOOK(0x006BC630, C_008605d8_StoreValueAndApplyToAccessor, "auto");
