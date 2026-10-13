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

// Removes the first entry equal to 'entryValue' from the script pointer array
// (field_0028, count field_0030), closing the gap by shifting later entries down.
static void __fastcall CMissionScript_RemoveObjectEntry(CMissionScript* self, void* /*edx*/, int entryValue)
{
    uint32_t count = self->field_0030;
    uint32_t index = 0;
    if (count == 0)
        return;

    uint32_t* entry = reinterpret_cast<uint32_t*>(static_cast<uintptr_t>(self->field_0028));
    for (; *entry != static_cast<uint32_t>(entryValue); ++entry)
    {
        if (++index >= count)
            return;
    }

    uint32_t newCount = self->field_0030 - 1;
    self->field_0030 = newCount;
    if (index != newCount)
    {
        uint8_t* base = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(self->field_0028));
        _memcpy(base + 4 * index, base + 4 * index + 4, static_cast<std::size_t>(4 * (newCount - index)));
    }
}

HOOK(0x00475480, CMissionScript_RemoveObjectEntry, "auto");
