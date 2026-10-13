#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

// Callee bound by address (cdecl, caller pops the 3 args).
CSF_FUNC(char*, __cdecl, Hud_ExtractResourcePath, 0x00694960, (char* a1, uint8_t* a2, unsigned int a3));

// Extracts a resource path from a1 into a3 (bounded by a4), then appends the
// NUL-terminated string a2 (including its terminator) at the end of a3.
// Returns strlen(a2) + 1, the number of bytes copied.
static unsigned int __cdecl CoreString_ExtractResourcePathAndAppend(const char* a1, const char* a2, char* a3, unsigned int a4)
{
    Hud_ExtractResourcePath(const_cast<char*>(a1), reinterpret_cast<uint8_t*>(a3), a4);

    // Length of a2 including the terminating NUL.
    unsigned int copyCount = 0;
    while (a2[copyCount] != '\0')
    {
        ++copyCount;
    }
    ++copyCount;

    // Find the terminator of a3 (after the extraction) and append at that position.
    size_t destIndex = 0;
    while (a3[destIndex] != '\0')
    {
        ++destIndex;
    }

    for (unsigned int i = 0; i < copyCount; ++i)
    {
        a3[destIndex + i] = a2[i];
    }

    return copyCount;
}

HOOK(0x00694B20, CoreString_ExtractResourcePathAndAppend, "auto");
