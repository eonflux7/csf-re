#include <cstdint>
#include "globals.h"
#include <cstddef>

#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Returns the address of entry `entryIndex` (164-byte records).
// self->field_0004 holds a pointer to a table header; the records base is the dword at header+4.
static int __fastcall NetPerfCounters_ResolveEntry(C_00872abc* self, void* /*edx*/, int entryIndex) {
    uint32_t headerAddress = self->field_0004;
    uint8_t* header = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(headerAddress));
    uint32_t recordsBase = CSF_AT(uint32_t, header, 4);
    uint32_t recordOffset = static_cast<uint32_t>(entryIndex) * 0xA4u;
    return static_cast<int>(recordsBase + recordOffset);
}

HOOK(0x0078BB40, NetPerfCounters_ResolveEntry, "auto");
