#include "hook.h"
#include "globals.h"

// MSVC CRT _stricmp, bound by address (lib).
CSF_FUNC(int, __cdecl, __stricmp, 0x007B0C41, (const char* left, const char* right));

// Returns non-zero when both strings are non-null and equal ignoring ASCII case.
static int __cdecl Hud_IsStringEqualIgnoreCase(char* String1, char* String2)
{
    if (String1 == nullptr || String2 == nullptr) {
        return 0;
    }
    return __stricmp(String1, String2) == 0 ? 1 : 0;
}

HOOK(0x0042D5F0, Hud_IsStringEqualIgnoreCase, "auto");
