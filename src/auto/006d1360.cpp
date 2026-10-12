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

// Removes `item` from the uint32 pointer list at field_0064 (count in field_006c),
// compacting the tail. If `item` is non-null, its vtable slot 1 (+0x4) is invoked with 1
// and that call's result becomes the return value. Otherwise returns the found index, or
// the count when not found (or the memcpy destination when a compaction copy happened).
static unsigned int __fastcall C_00860e68_RemoveItemAndNotify(C_00860e68* self, void* /*edx*/, int item)
{
    unsigned int result = 0;

    if (self->field_006c != 0)
    {
        uint32_t* items = reinterpret_cast<uint32_t*>(self->field_0064);
        bool found = false;

        for (; result < self->field_006c; ++result)
        {
            if (items[result] == static_cast<uint32_t>(item))
            {
                found = true;
                break;
            }
        }

        if (found)
        {
            uint32_t newCount = self->field_006c - 1;
            self->field_006c = newCount;

            if (result != newCount)
            {
                uint32_t* dst = reinterpret_cast<uint32_t*>(self->field_0064) + result;
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

HOOK(0x6D1360, C_00860e68_RemoveItemAndNotify, "auto");
