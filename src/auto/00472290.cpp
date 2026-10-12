#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __fastcall, CompareStringSubstring, 0x0042E200, (void* string, void* edx, unsigned int position, unsigned int count, const char* other, unsigned int otherCount));

static int __cdecl String_IsEqualToSubstring(void* string, const void* otherString)
{
    const unsigned int otherCount = CSF_AT(unsigned int, otherString, 0x14);
    const char* other;
    if (CSF_AT(unsigned int, otherString, 0x18) < 0x10u)
        other = reinterpret_cast<const char*>(otherString) + 4;
    else
        other = CSF_AT(const char*, otherString, 0x04);

    return CompareStringSubstring(
        string, nullptr, 0, CSF_AT(unsigned int, string, 0x14),
        other, otherCount) == 0;
}

HOOK(0x00472290, String_IsEqualToSubstring, "auto");
