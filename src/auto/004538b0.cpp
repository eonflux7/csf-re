#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Walks an array of 24-byte records from first up to (not including) last and
// stamps the vtable pointer for C_0082d410 into the record's first and fourth
// dwords. Returns the end iterator, which equals last on exit.
static uint32_t* __stdcall FillRangeVtable(uint32_t* first, uint32_t* last) {
    uint32_t* current = first;
    while (current != last) {
        uint32_t vtblAddr = static_cast<uint32_t>(reinterpret_cast<uintptr_t>(CSF_VTBL(0x0082D410)));
        current[3] = vtblAddr;
        current[0] = vtblAddr;
        current += 6;
    }
    return current;
}

HOOK(0x004538B0, FillRangeVtable, "auto");
