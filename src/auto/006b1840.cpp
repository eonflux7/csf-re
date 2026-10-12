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

CSF_FUNC(void*, __cdecl, _memcpy, 0x007AEE60, (void* destination, const void* source, std::size_t byteCount));

// Removes `item` from the uint32 pointer list at +0x08 (count at +0x10), compacting the tail.
// If `item` is non-null, its vtable slot 1 (+0x4) is invoked with 1 and that call's result
// becomes the return value. Otherwise returns the found index, or the count when not found
// (or the memcpy destination when a compaction copy happened).
static unsigned int __fastcall SceneEntryList_RemoveValueAndNotify(void* self, void* /*edx*/, int item)
{
    unsigned int result = 0;
    const uint32_t initialCount = CSF_AT(uint32_t, self, 0x10);

    if (initialCount != 0)
    {
        uint32_t* items = reinterpret_cast<uint32_t*>(static_cast<uintptr_t>(CSF_AT(uint32_t, self, 0x08)));
        bool found = false;

        for (; result < initialCount; ++result)
        {
            if (items[result] == static_cast<uint32_t>(item))
            {
                found = true;
                break;
            }
        }

        if (found)
        {
            uint32_t newCount = CSF_AT(uint32_t, self, 0x10) - 1;
            CSF_AT(uint32_t, self, 0x10) = newCount;

            if (result != newCount)
            {
                uint32_t* list = reinterpret_cast<uint32_t*>(static_cast<uintptr_t>(CSF_AT(uint32_t, self, 0x08)));
                uint32_t* dst = list + result;
                const uint32_t* src = dst + 1;
                void* moved = _memcpy(dst, src, 4 * static_cast<std::size_t>(newCount - result));
                result = static_cast<unsigned int>(reinterpret_cast<uintptr_t>(moved));
            }
        }
    }

    if (item != 0)
    {
        return ((unsigned int(__fastcall*)(void*, void*, int))CSF_AT(void**, item, 0)[1])(
            reinterpret_cast<void*>(item), nullptr, 1);
    }

    return result;
}

HOOK(0x6B1840, SceneEntryList_RemoveValueAndNotify, "auto");
