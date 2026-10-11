#include <cstdint>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// CompareStringSubstring: __fastcall, ecx = entry pointer, edx unused,
// 4 stack args (position, count, other, otherCount), ret 16.
CSF_FUNC(int, __fastcall, CompareStringSubstring, 0x0042E200,
         (void* string, void* edx, unsigned int position, unsigned int count,
          const char* other, unsigned int otherCount));

// Scans the scene entry table (28-byte records, name string at +0x14) and
// returns the index of the first entry whose name matches the given buffer,
// or -1 when no entry matches or the table is empty.
static int __fastcall SceneEntries_FindMatchingIndex(S_006cd2d0* self, void* /*edx*/, uint32_t* a2)
{
    uint32_t entryOffset = 0;
    int matchIndex = 0;

    if (self->field_0008 == 0)
        return -1;

    while (true)
    {
        uint8_t* entry = reinterpret_cast<uint8_t*>(self->field_0000 + entryOffset);

        const char* other = (a2[6] < 0x10u)
            ? reinterpret_cast<const char*>(a2 + 1)
            : reinterpret_cast<const char*>(static_cast<uintptr_t>(a2[1]));

        if (CompareStringSubstring(entry, nullptr, 0u, CSF_AT(uint32_t, entry, 0x14), other, a2[5]) == 0)
            break;

        entryOffset += 0x1c;
        if (++matchIndex >= static_cast<int>(self->field_0008))
            return -1;
    }
    return matchIndex;
}

HOOK(0x006CCE10, SceneEntries_FindMatchingIndex, "auto");
