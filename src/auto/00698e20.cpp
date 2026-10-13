#include <cstdint>
#include "hook.h"

// qsort/bsearch comparator: orders two 8-byte-or-larger table entries by the
// dword at offset 4. Subtraction is done unsigned and reinterpreted as int to
// match the original 32-bit `sub eax, [ecx+4]` wraparound.
static int __cdecl CompareEntriesBySecondDword(uint32_t* entryA, uint32_t* entryB) {
    return static_cast<int>(entryA[1] - entryB[1]);
}

HOOK(0x698E20, CompareEntriesBySecondDword, "auto");
