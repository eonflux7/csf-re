#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_GLOBAL(int, g_GlobalValue_8BB404, 0x008BB404);

// Resolves entry `a2` of the table owned by `a1`. When the global hook object is
// installed, its function at +0x14 is asked for the entry, and a successful result
// is tagged with its owner `a1` at +0x38. Otherwise the entry is computed from the
// table at a1+0x58 (64-byte records), bounded by the count at a1+4.
static int __cdecl RtFS_ResolveEntryByIndex(int a1, unsigned int a2)
{
    int result;

    if (g_GlobalValue_8BB404)
    {
        uint8_t* hookObj = reinterpret_cast<uint8_t*>(g_GlobalValue_8BB404);
        auto resolveFn = reinterpret_cast<int(__cdecl*)(void*, unsigned int)>(
            CSF_AT(void*, hookObj, 0x14));
        result = resolveFn(hookObj, a2);
        if (result)
        {
            CSF_AT(int, reinterpret_cast<uint8_t*>(result), 0x38) = a1;
        }
    }
    else
    {
        uint8_t* owner = reinterpret_cast<uint8_t*>(a1);
        if (a2 >= CSF_AT(uint32_t, owner, 0x04))
        {
            return 0;
        }
        uint32_t tableBase = CSF_AT(uint32_t, owner, 0x58);
        return static_cast<int>(tableBase + (a2 << 6));
    }
    return result;
}

HOOK(0x0069FB40, RtFS_ResolveEntryByIndex, "auto");
