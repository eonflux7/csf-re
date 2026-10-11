#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

static int __fastcall C_00833908_ForwardRequest(
    C_00833908* self, void* /*edx*/,
    int request, int arg2, int arg3, int arg4)
{
    CSF_AT(uint8_t, reinterpret_cast<void*>(self->field_00ac), 0x1) = 0;

    void* delegate = reinterpret_cast<void*>(self->field_0048);
    int result = -1;
    if (delegate)
    {
        result = reinterpret_cast<int (__fastcall*)(void*, void*, int, int, int, int)>(
            CSF_AT(void**, delegate, 0)[0x50 / 4])(
                delegate, nullptr, request, arg2, arg3, arg4);
    }

    self->field_0080 = request;
    if (result == -1)
    {
        void* member = &self->field_0004;
        reinterpret_cast<void (__fastcall*)(void*, void*)>(
            CSF_AT(void**, member, 0)[0x0C / 4])(member, nullptr);
    }
    return result;
}

HOOK(0x0048F2A0, C_00833908_ForwardRequest, "auto");
