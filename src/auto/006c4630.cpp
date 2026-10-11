#include <cstddef>
#include "globals.h"
#include <cstdint>

#include "core/memory.h"
#include "hook.h"
#include "types/auto_classes.h"

// Binds the original memcpy by address (bind-as line copied from the packet).
CSF_FUNC(void*, __cdecl, _memcpy, 0x007AEE60, (void* destination, const void* source, std::size_t byteCount));

// Removes the first 12-byte entry whose leading dword equals `key` from the
// entry list owned by this object (list header: +0 data pointer, +8 count).
static void __fastcall C_008670f0_RemoveMatchingEntry(C_008670f0* self, void* /*edx*/, int key)
{
    const uint32_t listAddress = self->field_0008;
    if (listAddress == 0)
        return;

    uint8_t* list = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(listAddress));
    uint32_t count = CSF_AT(uint32_t, list, 0x8);
    if (count == 0)
        return;

    // Find the first entry whose key matches.
    int matchIndex = -1;
    for (uint32_t i = 0; i < count; ++i)
    {
        uint8_t* entries = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(CSF_AT(uint32_t, list, 0x0)));
        if (CSF_AT(uint32_t, entries, i * 12u) == static_cast<uint32_t>(key))
        {
            matchIndex = static_cast<int>(i);
            break;
        }
    }
    if (matchIndex < 0)
        return;

    // Shrink the count and close the gap by shifting the tail down one entry.
    uint32_t newCount = count - 1u;
    CSF_AT(uint32_t, list, 0x8) = newCount;

    if (matchIndex != static_cast<int>(newCount))
    {
        uint8_t* entries = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(CSF_AT(uint32_t, list, 0x0)));
        uint8_t* hole = entries + 12u * static_cast<uint32_t>(matchIndex);
        uint32_t tailEntries = newCount - static_cast<uint32_t>(matchIndex);
        _memcpy(hole, hole + 12, static_cast<std::size_t>(12u * tailEntries));
    }
}

HOOK(0x6C4630, C_008670f0_RemoveMatchingEntry, "auto");
