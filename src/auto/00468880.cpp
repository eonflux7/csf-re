#include <cstdint>
#include "globals.h"
#include <cstddef>
#include "hook.h"
#include "core/memory.h"

// Returns non-zero (BOOL TRUE) when the masked id is present in this object's id table.
// Ids must carry flag 0x8000000; that flag is stripped before the table search.
static int __fastcall SceneManager_IsIdActive(void* self, void* /*edx*/, uint32_t id)
{
    if (CSF_AT(uint8_t, self, 0xAC) == 0)
        return 0;

    if ((id & 0x8000000u) == 0)
        return 0;

    uint32_t entryCount = CSF_AT(uint32_t, self, 0xB8);
    if (entryCount == 0)
        return 0;

    uint32_t searchKey = id & 0xF7FFFFFFu;
    uint32_t* entries = reinterpret_cast<uint32_t*>(CSF_AT(uint32_t, self, 0xB0));

    for (uint32_t i = 0; i < entryCount; ++i)
    {
        if (entries[i] == searchKey)
            return 1;
    }
    return 0;
}

HOOK(0x00468880, SceneManager_IsIdActive, "auto");
