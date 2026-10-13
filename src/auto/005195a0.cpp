#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Game heap allocator, called exactly as the original does.
CSF_FUNC(int, __cdecl, CoreMemory_AllocUntagged, 0x006910F0, (std::size_t size));

// Grows the edge-entry array. __thiscall on the array sub-object at this+0x14.
CSF_FUNC(void*, __fastcall, Polygon_GrowEdgeArray, 0x006D9D80, (void* self, void* , int extraSlots));

// Appends a 16-byte entry {a2, a3, a4, copy of name} to the array at this+0x14.
// Array sub-object layout: +0 data ptr, +4 capacity, +8 count, +0xC grow argument.
static uint32_t* __fastcall C_0085ec18_AppendEntryWithCopiedName(
    C_0085ec18* self, void* /*edx*/, uint32_t a2, uint32_t a3, uint32_t a4, const char* name)
{
    // Measure and copy the name into a freshly allocated buffer (length + NUL).
    std::size_t length = 0;
    while (name[length] != '\0') {
        ++length;
    }
    char* nameCopy = reinterpret_cast<char*>(
        static_cast<uintptr_t>(CoreMemory_AllocUntagged(length + 1)));
    for (std::size_t i = 0; i <= length; ++i) {
        nameCopy[i] = name[i];
    }

    // Array sub-object embedded at this+0x14.
    void* arrayObj = reinterpret_cast<uint8_t*>(self) + 0x14;

    // Grow when count >= capacity (unsigned compare).
    uint32_t capacity = CSF_AT(uint32_t, self, 0x18);
    uint32_t count = CSF_AT(uint32_t, self, 0x1C);
    if (count >= capacity) {
        Polygon_GrowEdgeArray(arrayObj, nullptr, static_cast<int>(CSF_AT(uint32_t, self, 0x20)));
    }

    // Re-read count and data after a possible grow, then append.
    uint32_t index = CSF_AT(uint32_t, self, 0x1C);
    uint8_t* dataBase = reinterpret_cast<uint8_t*>(
        static_cast<uintptr_t>(CSF_AT(uint32_t, self, 0x14)));
    uint32_t* entry = reinterpret_cast<uint32_t*>(dataBase + (index << 4));
    CSF_AT(uint32_t, self, 0x1C) = index + 1;

    entry[0] = a2;
    entry[1] = a3;
    entry[2] = a4;
    entry[3] = static_cast<uint32_t>(reinterpret_cast<uintptr_t>(nameCopy));
    return entry;
}

HOOK(0x005195A0, C_0085ec18_AppendEntryWithCopiedName, "auto");
