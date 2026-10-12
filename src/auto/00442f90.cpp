#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __stdcall, DynamicObj_ReleaseEntry, 0x00442DF0, (int a1));
CSF_FUNC(void*, __cdecl, _memcpy, 0x007AEE60, (void* destination, const void* source, std::size_t byteCount));

// Walks the entry array from last to first, releasing every entry whose flag byte has bit 0x2 set
// and compacting the array. Always returns 0.
static int __fastcall DynamicObj_PruneMarkedEntries(S_00442f90* self, void* /*edx*/)
{
    int result = static_cast<int>(self->field_0014);
    if (result != 0)
    {
        int index = result - 1;          // v3
        int offset = 4 * result - 4;     // v4
        int remaining = result;          // v6

        do
        {
            uint32_t entryAddr = *reinterpret_cast<uint32_t*>(
                static_cast<uintptr_t>(self->field_000c + static_cast<uint32_t>(offset)));
            uint8_t* entry = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(entryAddr));

            if ((CSF_AT(uint8_t, entry, 0x0C) & 2) != 0)
            {
                DynamicObj_ReleaseEntry(static_cast<int>(entryAddr));

                uint32_t newCount = self->field_0014 - 1;
                self->field_0014 = newCount;
                if (static_cast<uint32_t>(index) != newCount)
                {
                    uint8_t* slot = reinterpret_cast<uint8_t*>(
                        static_cast<uintptr_t>(self->field_000c + static_cast<uint32_t>(offset)));
                    _memcpy(slot,
                            slot + 4,
                            static_cast<std::size_t>((newCount - static_cast<uint32_t>(index)) << 2));
                }
            }

            offset -= 4;
            --index;
            --remaining;
            result = remaining;
        } while (remaining != 0);
    }
    return result;
}

HOOK(0x00442F90, DynamicObj_PruneMarkedEntries, "auto");
