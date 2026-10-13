#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

static int __fastcall C_008443c8_DelegatedCallMatchesExpected(
    C_008443c8* self, void* /*edx*/, int firstArgument, int expectedValue)
{
    void* delegate = reinterpret_cast<void*>(self->field_0004);
    int result = reinterpret_cast<int (__fastcall*)(void*, void*, int, int)>(
        CSF_AT(void**, delegate, 0)[3])(delegate, nullptr, firstArgument, expectedValue);
    return result == expectedValue;
}

HOOK(0x0054E240, C_008443c8_DelegatedCallMatchesExpected, "auto");
