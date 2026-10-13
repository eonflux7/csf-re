#include <cstddef>
#include <cstdint>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __cdecl, _memcpy, 0x007AEE60, (void* destination, const void* source, std::size_t byteCount));

// Removes the entry equal to `entry` from the array of pointers at field_0004 (count in field_000c),
// shifting the tail down, then calls the entry's slot-1 virtual method with argument 1.
static void __fastcall C_00860db0_RemoveAndReleaseEntry(C_00860db0* self, void* /*edx*/, int a2)
{
    if (a2 == 0)
        return;

    uint32_t entryCount = self->field_000c;
    uint32_t index = 0;
    if (entryCount == 0)
        return;

    uint32_t* entries = reinterpret_cast<uint32_t*>(self->field_0004);
    while (entries[index] != static_cast<uint32_t>(a2))
    {
        if (++index >= entryCount)
            return;
    }

    if (index != static_cast<uint32_t>(-1))
    {
        uint32_t newCount = self->field_000c - 1;
        self->field_000c = newCount;
        if (index != newCount)
        {
            uint8_t* base = reinterpret_cast<uint8_t*>(self->field_0004);
            _memcpy(base + 4 * index,
                    base + 4 * index + 4,
                    static_cast<std::size_t>(4 * (newCount - index)));
        }

        void* target = reinterpret_cast<void*>(static_cast<uintptr_t>(a2));
        auto slot1 = reinterpret_cast<void(__fastcall*)(void*, void*, int)>(CSF_AT(void**, target, 0)[1]);
        slot1(target, nullptr, 1);
    }
}

HOOK(0x006D0650, C_00860db0_RemoveAndReleaseEntry, "auto");
