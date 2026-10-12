#include <cstdint>
#include <cstddef>
#include <cstring>

#include "hook.h"

// Walks a range of (pointer, pointer) pairs from first to last. For each pair,
// copies the 64-byte block at *source into the buffer at pair[0].
// The source pointer is re-read from *source on every iteration, as in the original.
static void** __cdecl GfxObject_CopyTemplateToPairedTargets(void** first, void** last, const void** source)
{
    void** cursor = first;
    while (cursor != last)
    {
        void* destination = *cursor;
        std::memcpy(destination, *source, 0x40);
        cursor += 2;
    }
    return cursor;
}

HOOK(0x6B3370, GfxObject_CopyTemplateToPairedTargets, "auto");
