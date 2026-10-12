#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

static int __fastcall C_0083f160_Destroy(C_0083f160* self, void* /*edx*/)
{
    int result = 0;
    void* first = reinterpret_cast<void*>(self->field_0004);
    *reinterpret_cast<void***>(self) = CSF_VTBL(0x0083F160);

    if (first)
    {
        result = reinterpret_cast<int (__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, first, 0)[0])(first, nullptr, 1);
        self->field_0004 = 0;
    }

    void* second = reinterpret_cast<void*>(self->field_0008);
    if (second)
    {
        result = reinterpret_cast<int (__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, second, 0)[0])(second, nullptr, 1);
        self->field_0008 = 0;
    }

    void* third = reinterpret_cast<void*>(self->field_000c);
    if (third)
    {
        result = reinterpret_cast<int (__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, third, 0)[0])(third, nullptr, 1);
        self->field_000c = 0;
    }

    *reinterpret_cast<void***>(self) = CSF_VTBL(0x0082D410);
    return result;
}

HOOK(0x004F4A80, C_0083f160_Destroy, "auto");
