#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

static int __fastcall C_00833908_ForwardRequest(
    C_00833908* self, void* /*edx*/, int a2, int a3, int a4, int a5)
{
    CSF_AT(uint8_t, reinterpret_cast<void*>(self->field_00ac), 0x1) = 0;

    void* delegate = reinterpret_cast<void*>(self->field_0048);
    int result = -1;
    if (delegate)
    {
        result = reinterpret_cast<int (__fastcall*)(void*, void*, int, int, int, int)>(
            CSF_AT(void**, delegate, 0)[0x54 / 4])(
                delegate, nullptr, a2, a3, a4, a5);
    }

    self->field_0080 = -1;
    if (result == -1)
    {
        void* subobject = &self->field_0004;
        reinterpret_cast<void (__fastcall*)(void*, void*)>(
            CSF_AT(void**, subobject, 0)[0x0C / 4])(subobject, nullptr);
    }
    return result;
}

HOOK(0x0048F300, C_00833908_ForwardRequest, "auto");
