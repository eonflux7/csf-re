#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

static int __fastcall C_008443c8_CheckDelegatedResult(
    C_008443c8* self, void* /*edx*/, int argument, int expected)
{
    void* delegate = reinterpret_cast<void*>(self->field_0004);
    int result = reinterpret_cast<int (__fastcall*)(void*, void*, int, int)>(
        CSF_AT(void**, delegate, 0)[2])(delegate, nullptr, argument, expected);
    return result == expected;
}

HOOK(0x0054E260, C_008443c8_CheckDelegatedResult, "auto");
