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
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

// Object layout (no declared class):
//   +0x0  const void* entries   (pair table: {int32 key = -1, int32 value = 0})
//   +0x4  uint32_t    count     (number of entries)
//   +0x8  uint32_t    usedCount (entries to carry over on resize)
// Grows the entry table by growBy entries, carrying over the first usedCount
// entries, frees the old table and returns the old table pointer.
static const void* __fastcall Hud_ResizeEntryTable(void* self, void* /*edx*/, int growBy)
{
    int32_t newCount = growBy + static_cast<int32_t>(CSF_AT(uint32_t, self, 0x4));
    void* newTable = Mem_Alloc(static_cast<std::size_t>(newCount) * 8u);

    if (newTable)
    {
        int32_t* entry = static_cast<int32_t*>(newTable);
        for (int32_t i = 0; i < newCount; ++i)
        {
            entry[0] = -1;
            entry[1] = 0;
            entry += 2;
        }
    }

    uint32_t usedCount = CSF_AT(uint32_t, self, 0x8);
    if (usedCount)
    {
        std::memcpy(newTable, CSF_AT(const void*, self, 0x0), static_cast<std::size_t>(usedCount) * 8u);
    }

    const void* oldTable = CSF_AT(const void*, self, 0x0);
    CSF_AT(uint32_t, self, 0x4) += static_cast<uint32_t>(growBy);

    if (oldTable != nullptr)
    {
        Mem_Free(const_cast<void*>(oldTable));
        CSF_AT(const void*, self, 0x0) = nullptr;
    }

    CSF_AT(void*, self, 0x0) = newTable;
    return oldTable;
}

HOOK(0x0042E280, Hud_ResizeEntryTable, "auto");
