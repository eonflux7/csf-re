#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));

// Copies a 12-byte-entry task state list from the source descriptor
// {ptr, count, copyCount, field3} into self. When count != 0 a fresh buffer of
// count entries is allocated, zeroed, and the first copyCount entries are copied
// from the source data. Returns self.
static void* __fastcall TaskStates_CopyListFromSource(void* self, void* /*edx*/, void* src)
{
    int entryCount = CSF_AT(int, src, 0x4);

    if (entryCount != 0)
    {
        void* buffer = nullptr;
        void* allocated = Mem_Alloc(12 * static_cast<std::size_t>(entryCount));
        if (allocated != nullptr)
        {
            int remaining = entryCount;
            if (remaining - 1 >= 0)
            {
                uint8_t* entry = static_cast<uint8_t*>(allocated);
                do
                {
                    CSF_AT(uint32_t, entry, 0x0) = 0;
                    CSF_AT(uint32_t, entry, 0x4) = 0;
                    CSF_AT(uint32_t, entry, 0x8) = 0;
                    entry += 12;
                    --remaining;
                } while (remaining != 0);
            }
            buffer = allocated;
        }

        CSF_AT(void*, self, 0x0) = buffer;
        std::memcpy(buffer,
                    CSF_AT(const void*, src, 0x0),
                    12 * static_cast<std::size_t>(CSF_AT(uint32_t, src, 0x8)));
    }
    else
    {
        CSF_AT(void*, self, 0x0) = nullptr;
    }

    CSF_AT(uint32_t, self, 0x8) = CSF_AT(uint32_t, src, 0x8);
    CSF_AT(uint32_t, self, 0x4) = CSF_AT(uint32_t, src, 0x4);
    CSF_AT(uint32_t, self, 0xC) = CSF_AT(uint32_t, src, 0xC);
    return self;
}

HOOK(0x41E890, TaskStates_CopyListFromSource, "auto");
