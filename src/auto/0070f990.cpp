#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"

// Comparator for DirectSound/RW audio-effect entries: orders by the unsigned
// key stored at +0x34 of the object each argument points to.
// Returns -1 if a < b, 0 if equal, 1 if a > b (unsigned compare).
static int __cdecl CompareEntriesByField34(const void* a1, const void* a2)
{
    void* entryA = CSF_AT(void*, const_cast<void*>(a1), 0);
    void* entryB = CSF_AT(void*, const_cast<void*>(a2), 0);

    uint32_t keyA = CSF_AT(uint32_t, entryA, 0x34);
    uint32_t keyB = CSF_AT(uint32_t, entryB, 0x34);

    if (keyB <= keyA)
        return keyB < keyA ? -1 : 0;
    return 1;
}

HOOK(0x70F990, CompareEntriesByField34, "auto");
