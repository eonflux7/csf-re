#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

// Bound callee: C runtime tolower, __cdecl, one int argument.
CSF_FUNC(int, __cdecl, _tolower, 0x007B1905, (int C));

// Compares two scene/path names case-insensitively. Runs of '/' and '\'
// are treated as equivalent separators and skipped on both sides.
// Returns true only if both names end together.
static bool CoreString_SceneNamesMatch(char* a1, char* a2)
{
    char* v3 = a1;
    char v4 = *a1;
    bool result = false;

    while (true)
    {
        if (v4 == 47 || v4 == 92)
        {
            if (*a2 != 47 && *a2 != 92)
                return false;

            // Skip separator runs in the first name.
            char v5;
            do
            {
                do
                    v5 = *++v3;
                while (v5 == 47);
            } while (v5 == 92);

            // Skip separator runs in the second name.
            char v6;
            do
            {
                do
                    v6 = *++a2;
                while (v6 == 47);
            } while (v6 == 92);
        }

        int v7 = _tolower(*v3);
        int v8 = _tolower(*a2);
        v4 = v3[1];
        result = (v7 == v8);
        ++v3;
        ++a2;

        if (!v4 || !*a2)
            break;
        if (!result)
            return result;
    }

    if (*v3 == *a2)
        return result;
    return false;
}

HOOK(0x694AA0, CoreString_SceneNamesMatch, "auto");
