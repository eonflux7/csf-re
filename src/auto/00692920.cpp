#include <cstdint>
#include "globals.h"
#include <cstddef>
#include "hook.h"

// Standard C runtime isalpha, cdecl, one int argument.
CSF_FUNC(int, __cdecl, _isalpha, 0x007B1927, (int character));

// Classifies a parser token. a1 points to a two-dword record:
//   a1[0] = token length (characters to scan), a1[1] = token text pointer.
// Returns 5 if the text starts with a letter or a preceding quote character,
// 4 if a '.' occurs within the length, otherwise 3.
static int __stdcall Parser_ClassifyToken(uint32_t* a1)
{
    char* textStart = reinterpret_cast<char*>(a1[1]);

    if (_isalpha(*textStart) != 0 || textStart[-1] == 0x22)
        return 5;

    uint32_t length = a1[0];
    if (length == 0)
        return 3;

    const uint8_t* scan = reinterpret_cast<const uint8_t*>(a1[1]);
    uint32_t index = 0;
    while (scan[index] != 0x2E)
    {
        if (++index >= length)
            return 3;
    }
    return 4;
}

HOOK(0x692920, Parser_ClassifyToken, "auto");
