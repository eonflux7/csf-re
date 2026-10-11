#include <cstdint>
#include "globals.h"
#include <cstddef>

#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Returns the address of sector record `sectorIndex` (556-byte records).
// self+0x008 holds a pointer to a table header; the records base is the dword at header+4.
static int __fastcall SceneLoader_GetSectorRecord(void* self, void* /*edx*/, int sectorIndex) {
    uint32_t headerAddress = CSF_AT(uint32_t, self, 0x008);
    uint8_t* header = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(headerAddress));
    uint32_t recordsBase = CSF_AT(uint32_t, header, 4);
    uint32_t recordOffset = static_cast<uint32_t>(sectorIndex) * 0x22Cu;
    return static_cast<int>(recordsBase + recordOffset);
}

HOOK(0x00762470, SceneLoader_GetSectorRecord, "auto");
