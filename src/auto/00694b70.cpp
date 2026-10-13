#include <cstddef>
#include "globals.h"
#include "hook.h"

CSF_FUNC(char*, __cdecl, _strrchr, 0x007B2E90, (char* Str, int Ch));

static std::size_t CoreString_MeasureLength(const char* text)
{
    std::size_t length = 0;
    while (text[length] != '\0')
        ++length;
    return length;
}

static bool __cdecl CoreString_IsShortDosFileName(char* Str)
{
    if (!Str || !*Str)
        return true;

    char* lastBackslash = _strrchr(Str, 92);
    char* namePart = lastBackslash ? lastBackslash + 1 : Str;

    char* lastDot = _strrchr(Str, 46);
    if (lastDot)
        return lastDot - namePart <= 8 &&
               CoreString_MeasureLength(lastDot + 1) <= 3;

    return CoreString_MeasureLength(namePart) <= 8;
}

HOOK(0x00694B70, CoreString_IsShortDosFileName, "auto");
