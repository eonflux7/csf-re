#include "hook.h"

// Returns true only when both pointers are non-null and the two
// null-terminated strings are identical. Compares byte by byte, as the
// original does, without calling any other function.
static bool __cdecl AreCStringsEqual(const char* a1, const char* a2)
{
    if (!a1 || !a2)
        return false;

    const char* p = a1;
    const char* q = a2;
    for (;;)
    {
        unsigned char cp = static_cast<unsigned char>(*p);
        unsigned char cq = static_cast<unsigned char>(*q);
        if (cp != cq)
            return false;
        if (cp == 0)
            return true;
        ++p;
        ++q;
    }
}

HOOK(0x4FDA40, AreCStringsEqual, "auto");
