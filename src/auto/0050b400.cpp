#include <cstdint>
#include "globals.h"
#include "core/memory.h"
#include "hook.h"

CSF_FUNC(int, __cdecl, IsGlobalValueNonZero, 0x00521BF0, ());
CSF_FUNC(int, __cdecl, GetGlobalValue_70DDE0, 0x0070DDE0, ());

static char __fastcall COptionsFile_SetFlagThenQueryGlobal(uint8_t* self, void* /*edx*/, char a2, char a3)
{
    CSF_AT(char, self, 0x10) = a2;
    char result = a3;
    if (a3)
    {
        result = static_cast<char>(IsGlobalValueNonZero());
        if (result)
            return static_cast<char>(GetGlobalValue_70DDE0());
    }
    return result;
}

HOOK(0x0050B400, COptionsFile_SetFlagThenQueryGlobal, "auto");
