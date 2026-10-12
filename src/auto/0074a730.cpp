#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

// Returns the entry's own address (a1 - 0x6B0) when the word at +0x0C is zero,
// otherwise returns 0. The callee is cdecl, so nothing is popped (ret 0).
static int __cdecl SessionTable_FindEntry(int entryAddress) {
    int result = 0;
    if (CSF_AT(uint32_t, reinterpret_cast<void*>(entryAddress), 0x0C) == 0) {
        return entryAddress - 0x6B0;
    }
    return result;
}

HOOK(0x0074A730, SessionTable_FindEntry, "auto");
