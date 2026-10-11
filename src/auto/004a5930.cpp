#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"

// Original: 0x0042E200. Compares a substring of `string` against a buffer.
CSF_FUNC(int, __fastcall, CompareStringSubstring, 0x0042E200,
         (void* string, void* edx, unsigned int position, unsigned int count,
          const char* other, unsigned int otherCount));

// Returns 1 when CompareStringSubstring reports a match, 0 otherwise.
// `a1` holds a string whose data pointer lives at +0x14. `a2` is a
// std::string-style buffer: +0x04 = heap pointer, +0x14 = length,
// +0x18 = capacity; short strings (capacity < 16) are stored inline at +0x04.
static int __cdecl StringMatchesStoredSubstring(int a1, uint32_t* a2)
{
    const char* other;
    if (CSF_AT(uint32_t, a2, 0x18) < 0x10u)
        other = reinterpret_cast<const char*>(a2) + 4;
    else
        other = reinterpret_cast<const char*>(CSF_AT(uint32_t, a2, 0x04));

    const int matched = CompareStringSubstring(
        reinterpret_cast<void*>(a1), nullptr,
        0u,
        static_cast<unsigned int>(CSF_AT(uint32_t, a1, 0x14)),
        other,
        static_cast<unsigned int>(CSF_AT(uint32_t, a2, 0x14)));

    return matched != 0 ? 1 : 0;
}

HOOK(0x4A5930, StringMatchesStoredSubstring, "auto");
