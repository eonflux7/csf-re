#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(void*, __cdecl, _memcpy, 0x007AEE60, (void* destination, const void* source, std::size_t byteCount));

// Removes the 12-byte entry whose first dword equals (handle | 0x80000000) from the
// array held by the list object at self->field_0008. Entries after it shift down by one.
// Returns the list pointer when nothing was moved, or the memcpy destination otherwise.
static void* __fastcall GfxSkin_RemoveTaggedEntry(void* self, void* /*edx*/, uint32_t handle)
{
    uint8_t* list = reinterpret_cast<uint8_t*>(CSF_AT(uint32_t, self, 0x08));
    void* result = list;
    if (!list)
        return result;

    const uint32_t key = handle | 0x80000000u;
    bool found = false;
    uint32_t foundIndex = 0;

    if (CSF_AT(uint32_t, list, 0x08) != 0)
    {
        uint32_t index = 0;
        uint32_t byteOffset = 0;
        while (!found && index < CSF_AT(uint32_t, list, 0x08))
        {
            uint8_t* entries = reinterpret_cast<uint8_t*>(CSF_AT(uint32_t, list, 0x00));
            if (CSF_AT(uint32_t, entries, byteOffset) == key)
            {
                found = true;
                foundIndex = index;
            }
            ++index;
            byteOffset += 12;
            if (index >= CSF_AT(uint32_t, list, 0x08))
            {
                if (!found)
                    return result;
                break;
            }
        }

        const uint32_t newCount = CSF_AT(uint32_t, list, 0x08) - 1;
        CSF_AT(uint32_t, list, 0x08) = newCount;
        if (foundIndex != newCount)
        {
            uint8_t* entries = reinterpret_cast<uint8_t*>(CSF_AT(uint32_t, list, 0x00));
            uint8_t* dest = entries + 12 * foundIndex;
            return _memcpy(dest, dest + 12, 12 * (newCount - foundIndex));
        }
    }
    return result;
}

HOOK(0x006C46B0, GfxSkin_RemoveTaggedEntry, "auto");
