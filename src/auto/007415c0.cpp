#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

// Walks the circular list headed at self+0x6D4 (count at self+0x6D8) for the first
// node whose record pointer (node+8) is non-null and not yet flagged. The record's
// flag byte at +0x24 is set to 1 to mark the spawn point as taken, and the record
// pointer is returned. Returns 0 if the list is empty or no pending record is found.
static int __fastcall MpSpawnPoints_GetPendingSpawn(uint8_t* self, void* /*edx*/) {
    int result = 0;

    if (CSF_AT(uint32_t, self, 0x6D8) != 0) {
        uint8_t* listHead = CSF_AT(uint8_t*, self, 0x6D4);
        uint8_t* node = CSF_AT(uint8_t*, listHead, 0);

        if (node != CSF_AT(uint8_t*, self, 0x6D4)) {
            do {
                if (result != 0) {
                    break;
                }

                uint8_t* record = CSF_AT(uint8_t*, node, 8);
                if (record == nullptr || CSF_AT(uint8_t, record, 0x24) != 0) {
                    result = 0;
                } else {
                    CSF_AT(uint8_t, record, 0x24) = 1;
                    result = static_cast<int>(reinterpret_cast<uintptr_t>(record));
                }

                node = CSF_AT(uint8_t*, node, 0);
            } while (node != CSF_AT(uint8_t*, self, 0x6D4));
        }
    }

    return result;
}

HOOK(0x7415C0, MpSpawnPoints_GetPendingSpawn, "auto");
