#include <cstdint>
#include "globals.h"
#include <cstddef>

#include "hook.h"
#include "core/memory.h"

// Returns the first dword of the 28-byte record at `index` in the table
// stored at this+0x0C. Thiscall with one stack argument, so ret 4.
static int __fastcall GetTableEntryDwordByIndex(void* self, void* /*edx*/, int index) {
    uint8_t* table = reinterpret_cast<uint8_t*>(CSF_AT(uint32_t, self, 0x0C));
    return CSF_AT(int, table, 28 * index);
}

HOOK(0x004084C0, GetTableEntryDwordByIndex, "auto");
