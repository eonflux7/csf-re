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

CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));

// Releases the entry buffer owned by a CErrorLog-style sub-object: each
// 320-byte entry has a nested object at +300 that is released recursively,
// then the buffer itself is freed and the three pointers are cleared.
static int __fastcall CErrorLog_ReleaseEntries(void* self, void* /*edx*/)
{
    uint32_t firstEntry = CSF_AT(uint32_t, self, 0x4);
    if (firstEntry != 0)
    {
        uint32_t endEntry = CSF_AT(uint32_t, self, 0x8);
        for (uint32_t entry = firstEntry; entry != endEntry; entry += 320)
        {
            CErrorLog_ReleaseEntries(reinterpret_cast<void*>(entry + 300), nullptr);
        }
        if (CSF_AT(uint32_t, self, 0x4) != 0)
        {
            Mem_FreeDebug(reinterpret_cast<void*>(CSF_AT(uint32_t, self, 0x4)));
        }
    }
    CSF_AT(uint32_t, self, 0x4) = 0;
    CSF_AT(uint32_t, self, 0x8) = 0;
    CSF_AT(uint32_t, self, 0xC) = 0;
    return 0;
}

HOOK(0x0069D840, CErrorLog_ReleaseEntries, "auto");
